/* ---------------------------------------------------------------------
   (c) ED 1995-2002
   Projet       : CLIB
   Fonction     : Gestion des FIFOs
   Module       : FIFO
   Fichier      : FIFO.C
   Creation     : 30-04-1998
   Modification : 13-07-2002
   --------------------------------------------------------------------- */
#ifdef __cplusplus
#error This source file is not C++ but rather C. Please use a C-compiler
#endif

/* ---------------------------------------------------------------------
   Journal

   0.0 du 30-04-1998 Creation
   1.0 du 30-04-1998 Version operationelle
   1.1 du 05-08-1998 Remplace 'malloc' -> calloc
   Ajoute controles :
   - Ecriture fifo pleine : -> trap
   - Lecture fifo vide    : -> trap

   1.2 du 07-08-1998 LOCK = 0 C'est a l'application de gerer le masquage d'IT
   .                 Ajoute fFillRate()
   1.3 du 09-08-1998 Remplace FIFO par sFIFO
   1.4 du 10-08-1998 Deplace les fonctions de debug en FIFOX
   1.5 du 12-08-1998 Si le taux depasse 100, il est fixe a 101.
   1.6 du 27-10-1999 Remplace calloc/free par sys_*. Ajoute initialisation
   .                 formelle des pointeurs a NULL
   1.7 du 26-11-1999 Remplace create() par init(),
   .                 init() par reinit() et kill() par end().
   .                 Mise au point init() (err memoire)
   1.8 du 17-02-2001 Suppression assert() inutiles dans les fonctions
   .                 publiques.
   .                 Optimisation. Mise au point types
   1.9 du 31-01-2002 Version statique par defaut.
   1.10   27-05-2002 Ajoute FIFO_changed() utile quand on trace en temps
   .                 reel
   1.11   13-07-2002 LCLint

   --------------------------------------------------------------------- */
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "ed/inc/fifo.h"

#ifdef FIFO_DYN
#include "ed/inc/sysalloc.h"
#endif

/* macros ============================================================== */

#define VER "1.11"

#ifdef FIFO_DYN
#define DYN "DYN"
#else
#define DYN ""
#endif

#define ID "FIFO " DYN " Module \"C\" (c) ED 1995-2002"

/* constants =========================================================== */
/* types =============================================================== */
/* structures ========================================================== */
/* private variables =================================================== */
/* private functions =================================================== */

/* ---------------------------------------------------------------------
   maj_rate()
   ---------------------------------------------------------------------
   Met a jour les taux
   ---------------------------------------------------------------------
   E :
   S :
   --------------------------------------------------------------------- */
static void maj_rate (sFIFO_STS * const pSts, size_t const Max)
{
   STACK_CHK ();
   ASSERT (pSts != NULL);
   ASSERT (Max != 0);
   {
      ulong rate = (pSts->Nbr * 100UL) / Max;

      if (rate > 100UL)
      {
         rate = 101UL;          /* depassement */
      }

      pSts->Fir = (int) rate;
      if (pSts->Frm < (int) rate)
      {
         pSts->Frm = (int) rate;
      }
   }
}

/* entry points ======================================================== */

/* ---------------------------------------------------------------------
   FIFO_sver()
   ---------------------------------------------------------------------
   Role : Retourne une chaine "Version"
   ---------------------------------------------------------------------
   E :
   S : Pointeur de chaine ASCIIZ
   --------------------------------------------------------------------- */
const char *FIFO_sver (void)
{
   STACK_CHK ();
   return VER;
}

/* ---------------------------------------------------------------------
   FIFO_sid()
   ---------------------------------------------------------------------
   Role : Retourne une chaine "Identification"
   ---------------------------------------------------------------------
   E :
   S : Pointeur de chaine ASCIIZ
   --------------------------------------------------------------------- */
const char *FIFO_sid (void)
{
   STACK_CHK ();
   return ID;
}

/* ---------------------------------------------------------------------
   FIFO_mode()
   ---------------------------------------------------------------------
   Role : Retourne le mode de compilation
   ---------------------------------------------------------------------
   E :
   S : Mode de compilation de la bibliotheque
   FIFO_MODE_STA
   FIFO_MODE_DYN
   --------------------------------------------------------------------- */
eFIFO_MODE FIFO_mode (void)
{
   STACK_CHK ();
#ifdef FIFO_DYN
   return FIFO_MODE_DYN;
#else
   return FIFO_MODE_STA;
#endif
}

/* ---------------------------------------------------------------------
   FIFO_init()
   ---------------------------------------------------------------------
   Role : creation d'une FIFO de longueur max l
   La zone de donnees est initialisee a 0. La FIFO est vide.
   ---------------------------------------------------------------------
   E : longueur
   S : Pointeur sur la FIFO. Si il vaut NULL, la FIFO n'a pas pu etre cree.
   --------------------------------------------------------------------- */
#ifdef FIFO_DYN
sFIFO *FIFO_init (size_t const Max)
#else
void FIFO_init (sFIFO * this
                ,uchar * pbuf
                ,size_t const Max
                ,sFIFO_STS * pSts
)
#endif
{
#ifdef FIFO_DYN
   sFIFO *this = malloc (sizeof *this);
#endif

   STACK_CHK ();
   ENUM_CHECK ();

   if (this)
   {

      SYS_CLR (this, sFIFO);

#ifdef FIFO_DYN
      this->pBuf = malloc (sizeof *this->pBuf * Max);
#else
      this->pBuf = pbuf;
#endif

      if (this->pBuf)
      {
         memset (this->pBuf, 0, Max);
         this->Chg = 1;
         this->Max = Max;
         this->Emp = 1;

         {
#ifdef FIFO_DYN
            sFIFO_STS *const pSts = malloc (sizeof *pSts);
#endif

            if (pSts != NULL)
            {
               SYS_CLR (pSts, sFIFO_STS);

               this->pSts = pSts;
               maj_rate (pSts, this->Max);
            }
            else
            {
#ifdef FIFO_DYN
               FIFO_end (this);
               this = NULL;
#endif
            }
         }
      }
      else
      {
         FIFO_end (this);
#ifdef FIFO_DYN
         this = NULL;
#endif
      }
   }

#ifdef FIFO_DYN
   return this;
#endif
}

/* ---------------------------------------------------------------------
   FIFO_end()
   ---------------------------------------------------------------------
   Role : destruction DEFINITIVE d'une FIFO
   ---------------------------------------------------------------------
   E : Pointeur sur la FIFO
   S :
   --------------------------------------------------------------------- */
void FIFO_end (sFIFO * this)
{
   STACK_CHK ();
   if (this)
   {
      if (this->pSts)
      {
#ifdef FIFO_DYN
         free (this->pSts);
#endif
         this->pSts = NULL;
      }

      if (this->pBuf)
      {
#ifdef FIFO_DYN
         free (this->pBuf);
#endif
         this->pBuf = NULL;
      }

#ifdef FIFO_DYN
      free (this);
#endif
   }
}

#ifdef FIFO_DYN
/* ---------------------------------------------------------------------
   FIFO_reinit()
   ---------------------------------------------------------------------
   Role : Modification de la longueur de la FIFO
   La zone de donnees est initialisee a 0. La FIFO est vide.
   ---------------------------------------------------------------------
   E : Pointeur sur la FIFO
   E : Nouvelle longueur
   S :
   --------------------------------------------------------------------- */
void FIFO_reinit (sFIFO * this, size_t const Max)
{
   STACK_CHK ();
   if (this != NULL)
   {
      uchar *const pBuf = realloc (this->pBuf, Max);

      if (this->pBuf != NULL)
      {
         this->pBuf = pBuf;
         memset (this->pBuf, 0, Max);
         this->Chg = 1;
         this->Ecr = 0;
         this->Lec = 0;
         this->Max = Max;
         this->Ful = 0;
         this->Emp = 1;
      }

      if (this->pSts != NULL)
      {
         this->pSts->Nbr = 0;
         this->pSts->Ffc = 0;
         this->pSts->Fec = 0;

         maj_rate (this->pSts, this->Max);
      }
   }
}
#endif

/* ---------------------------------------------------------------------
   FIFO_put()
   ---------------------------------------------------------------------
   Role : ecriture d'un octet
   Si la fifo est pleine, la valeur est perdue. Il faut tester avant avec
   FIFO_full()
   ---------------------------------------------------------------------
   E : Pointeur sur la FIFO
   E : octet a ecrire
   S :
   --------------------------------------------------------------------- */
void FIFO_put (sFIFO * this, const int c)
{
   STACK_CHK ();
   if (this != NULL)
   {
      if (!this->Ful)           /* securite */
      {

#ifdef FIFO_TRACE
         T[Cpt].fif = this;
         T[Cpt].dat = c;
         Cpt = (Cpt + 1) % T_TAILLE;
#endif

         this->pBuf[this->Ecr] = (uchar) c;
         this->Chg = 1;
         this->Ecr++;
         if (this->Ecr == this->Max)
         {
            this->Ecr = 0;
         }
         this->Ful = this->Ecr == this->Lec;
         this->Emp = 0;

         if (this->pSts)
         {
            this->pSts->Nbr++;
            maj_rate (this->pSts, this->Max);
         }
      }
      else
      {
         if (this->pSts != NULL)
         {
            this->pSts->Ffc++;  /* trace 'Fifo Full Count' */
         }
      }
   }
}

/* ---------------------------------------------------------------------
   FIFO_get()
   ---------------------------------------------------------------------
   Role : lecture d'un octet
   Si la fifo est vide, la valeur retournee est 0. Il faut tester avant
   avec FIFO_empty()
   ---------------------------------------------------------------------
   E : Pointeur sur la FIFO
   S : octet lu
   --------------------------------------------------------------------- */
int FIFO_get (sFIFO * this)
{
   int c = EOF;

   STACK_CHK ();
   if (this != NULL)
   {
      if (!this->Emp)
      {
         c = this->pBuf[this->Lec];
         this->Chg = 1;
         this->Lec++;
         if (this->Lec == this->Max)
         {
            this->Lec = 0;
         }
         this->Emp = this->Ecr == this->Lec;
         this->Ful = 0;

         if (this->pSts)
         {
            this->pSts->Nbr--;
            maj_rate (this->pSts, this->Max);
         }
      }
      else
      {
         if (this->pSts)
         {
            this->pSts->Fec++;  /* trace 'Fifo Empty Count...' */
         }
      }
   }
   return c;
}

/* ---------------------------------------------------------------------
   FIFO_count()
   ---------------------------------------------------------------------
   Role : lecture remplissage (nombre d'octets dans la FIFO)
   ---------------------------------------------------------------------
   E : Pointeur sur la FIFO
   S : Nombre d'octets dans la FIFO
   --------------------------------------------------------------------- */
size_t FIFO_count (sFIFO * this)
{
   size_t count = (size_t) - 1;
   STACK_CHK ();

   if (this != NULL)
   {
      if (this->pSts != NULL)
      {
         count = this->pSts->Nbr;
      }
   }
   return count;
}

/* ---------------------------------------------------------------------
   FIFO_empty()
   ---------------------------------------------------------------------
   Role : retourne 1 si FIFO vide
   ---------------------------------------------------------------------
   E : Pointeur sur la FIFO
   S : Etat de la FIFO : 0=NON VIDE 1=VIDE
   --------------------------------------------------------------------- */
int FIFO_empty (sFIFO * this)
{
   int sts = -1;
   STACK_CHK ();

   if (this != NULL)
   {
      /* LCLint */
      sts = (int) this->Emp;
   }

   return sts;
}

/* ---------------------------------------------------------------------
   FIFO_full()
   ---------------------------------------------------------------------
   Role : retourne 1 si FIFO pleine
   ---------------------------------------------------------------------
   E : Pointeur sur la FIFO
   S : Etat de la FIFO : 0=NON PLEINE 1=PLEINE
   --------------------------------------------------------------------- */
int FIFO_full (sFIFO * this)
{
   int sts = -1;
   STACK_CHK ();

   if (this != NULL)
   {
      /* LCLint */
      sts = (int) this->Ful;
   }

   return sts;
}

/* ---------------------------------------------------------------------
   FIFO_changed()
   ---------------------------------------------------------------------
   Role : retourne 1 si la FIFO a change. Efface le bit.
   ---------------------------------------------------------------------
   E : Pointeur sur la FIFO
   S : Etat de la FIFO : 0=NON pas de changement 1=Changement
   --------------------------------------------------------------------- */
int FIFO_changed (sFIFO * this)
{
   int sts = -1;
   STACK_CHK ();

   if (this != NULL)
   {
      /* LCLint */
      sts = (int) this->Chg;
      this->Chg = 0;
   }

   return sts;
}

/* ---------------------------------------------------------------------
   FIFO_clear()
   ---------------------------------------------------------------------
   Role : vider la FIFO sans la lire
   La zone de donnees est initialisee a 0. La FIFO est vide.
   ---------------------------------------------------------------------
   E : Pointeur sur la FIFO
   S :
   --------------------------------------------------------------------- */
void FIFO_clear (sFIFO * this)
{
   STACK_CHK ();
   if (this != NULL)
   {
      memset (this->pBuf, 0, this->Max);
      this->Ecr = this->Lec;
      this->Ful = 0;
      this->Emp = 1;

      if (this->pSts != NULL)
      {
         this->pSts->Nbr = 0;
         maj_rate (this->pSts, this->Max);
      }
   }
}

/* ---------------------------------------------------------------------
   FIFO_fill_rate()
   ---------------------------------------------------------------------
   Role : retourne le taux de remplissage en %
   ---------------------------------------------------------------------
   E : Pointeur sur la FIFO
   S : 0-100
   --------------------------------------------------------------------- */
int FIFO_fill_rate (sFIFO * this)
{
   int Fir = -1;
   STACK_CHK ();

   if (this != NULL)
   {
      if (this->pSts != NULL)
      {
         Fir = this->pSts->Fir;
      }
   }
   return Fir;
}

/* ---------------------------------------------------------------------
   FIFO_fill_rate_max()
   ---------------------------------------------------------------------
   Role : retourne le taux de remplissage max atteint en %
   ---------------------------------------------------------------------
   E : Pointeur sur la FIFO
   S : 0-100
   --------------------------------------------------------------------- */
int FIFO_fill_rate_max (sFIFO * this)
{
   int Frm = -1;
   STACK_CHK ();

   if (this != NULL)
   {
      if (this->pSts != NULL)
      {
         Frm = this->pSts->Frm;
      }
   }
   return Frm;
}

/* ---------------------------------------------------------------------
   FIFO_sts()
   ---------------------------------------------------------------------
   Role : retourne les informations d'etat de la fifo
   ---------------------------------------------------------------------
   E : Pointeur sur la FIFO
   E : Structure d'information
   S :
   --------------------------------------------------------------------- */
void FIFO_sts (sFIFO * this, sFIFO_STS * pSts)
{
   STACK_CHK ();
   if (pSts)
   {
      if (this != NULL)
      {
         if (this->pSts != NULL)
         {
            *pSts = *this->pSts;  /* C'est une copie, donc stable pendant la lecture) */
         }
      }
   }
}

/* public variables ==================================================== */

/* File generated by 'NEW.EXE' Ver 1.10 (c) ED 1998 */
