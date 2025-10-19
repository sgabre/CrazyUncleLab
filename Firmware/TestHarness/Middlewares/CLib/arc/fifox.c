#ifdef __cplusplus
#error This source file is not C++ but rather C. Please use a C-compiler
#endif

#include "ed/inc/fifox.h"
#include "ed/inc/sysalloc.h"

/* macros ============================================================== */

#define MODULE "FIFOX"

/* constants =========================================================== */
/* types =============================================================== */
/* structures ========================================================== */
/* private variables =================================================== */

#if FIFO_TRACE
#define T_TAILLE 1204
static size_t G_Cpt = 0;
static struct sT
{
   sFIFO *fif;
   BYTE dat;
}
G_T[T_TAILLE];

#endif

/* private functions =================================================== */

/* -----------------------------------------------------------------------
   dump_get()
   -----------------------------------------------------------------------
   Lecture 'Special Dump' (pas de mise a jour des taux et compteurs)
   ----------------------------------------------------------------------- */
static uchar dump_get (sFIFO * const f)
{
   uchar c;

   ASSERT (f != NULL);
   ASSERT (!f->Emp);

   c = f->pBuf[f->Lec];

   f->Lec++;
   if (f->Lec == f->Max)
   {
      f->Lec = 0;
   }

   f->Emp = f->Ecr == f->Lec;
   f->Ful = 0;

   return c;
}

/* entry points ======================================================== */

/* -----------------------------------------------------------------------
   le dernier octet du dump est le dernier octet ecrit dans la FIFO
   ex : 1,2,3,4,_,_,_,_,_
   ^
   wEcr/wLec
   Le resultat est :

   _,_,_,_,_,1,2,3,4
   ----------------------------------------------------------------------- */
void FIFO_dump (sFIFO * f)
{
   if (f != NULL)
   {
      uchar *pBuf;
      size_t i;
/*   printf ("\nFIFO %p wLec=%04X wEcr=%04X bEmp=%01X bFul=%01X \n",f,f->iLec,f->iEcr,f->bEmp,f->bFul); */
      {
         sFIFO_STS *pSts = f->pSts;

         fprintf (stderr
                  ,"\nFIFO %p FillRate=%lu%% FillRateMax=%lu%%\n"
                  ,(void *) f
                  ,(ulong) pSts->Fir
                  ,(ulong) pSts->Frm
            );
         fprintf (stderr
                  ,"FullErr=%lu EmptyErr=%lu\n"
                  ,(ulong) pSts->Ffc
                  ,(ulong) pSts->Fec
            );
      }

      pBuf = calloc (1U, f->Max);

      if (pBuf != NULL)
      {
         /* recopier la FIFO dans le buffer CHRONOLOGIQUEMENT */
         f->Emp = 0;
         for (i = 0; !FIFO_empty (f); i++)
         {
            pBuf[i] = dump_get (f);
         }

         SYS_dump (pBuf, f->Max);

         free (pBuf);
      }
   }
}

#if FIFO_TRACE
/* -----------------------------------------------------------------------
   ----------------------------------------------------------------------- */
void FIFO_dumpall (void)
{
   size_t i;

   for (i = 0; i < G_Cpt; i++)
   {
      printf ("%p %02lX\n"
              ,(void *) G_T[i].fif
              ,(ulong) G_T[i].dat
         );
   }
}
#endif

/* public variables ==================================================== */
