#ifndef H_ED_FIFO_20050123193452
#define H_ED_FIFO_20050123193452

#ifdef __cplusplus
extern "C"
{
#endif

/* ---------------------------------------------------------------------
   (c) ED 1995-2007
   Projet       : CLIB
   Fonction     : Gestion des FIFOS
   Module       : FIFO
   Fichier      : FIFO.H
   Creation     : 30-04-1998
   Modification : 17-08-2007
   --------------------------------------------------------------------- */

/* ---------------------------------------------------------------------
   Journal

   1.12   17-08-2007 Ajoute FIFO_resize()
   1.11   13-07-2002 LCLint
   1.10   27-05-2002 Ajoute FIFO_changed() utile quand on trace en temps
   .                 reel
   1.9 du 31-01-2002 Version statique par defaut.
   1.8 du 17-02-2001 Suppression assert() inutiles dans les fonctions
   .                 publiques. Optimisation. Mise au point types
   1.7 du 26-11-1999 Remplace create() par init(),
   .                 init() par reinit() et kill() par end().
   .                 Mise au point init() (err memoire)
   1.6 du 27-10-1999 Remplace calloc/free par sys_*. Ajoute initialisation
   .                 formelle des pointeurs a NULL
   1.5 du 12-08-1998 Si le taux depasse 100, il est fixe a 101.
   1.4 du 10-08-1998 Deplace les fonctions de debug en FIFOX
   1.3 du 09-08-1998 Remplace FIFO par sFIFO
   1.2 du 07-08-1998 LOCK = 0 C'est a l'application de gerer le masquage d'IT
   .                 Ajoute fFillRate()
   1.1 du 05-08-1998 Remplace 'malloc' -> calloc
   .                 Ajoute controles :
   .                 - Ecriture fifo pleine : -> trap
   .                 - Lecture fifo vide    : -> trap
   1.0 du 30-04-1998 Version operationelle
   0.0 du 30-04-1998 Creation

   --------------------------------------------------------------------- */

#include "ed/inc/sys.h"

#if (defined(__LARGE__) \
  || defined(__FLAT__)  \
  || defined(__GNUC__)  \
  || defined(_MRI)      \
  || defined(_MSC_VER)  \
  || defined(__DIAB)    \
  )
#else
#error Seul les Modes LARGE et FLAT sont autorises avec FIFO
#endif

/* macros ============================================================== */

#ifdef FIFO_DYN
#define FIFO_MODE FIFO_MODE_DYN
#else
#define FIFO_MODE FIFO_MODE_STA
#endif

/* 0 = err 1 = OK */
#define FIFO_CHK_LIB (FIFO_mode () == FIFO_MODE)

/* constants =========================================================== */

   typedef enum
   {
      FIFO_MODE_STA,
      FIFO_MODE_DYN,
      FIFO_MODE_NB
   }
   eFIFO_MODE;

/* types =============================================================== */
/* structures ========================================================== */

   typedef struct
   {
      size_t Nbr;               /* nombre d'elements              */
      int Fir;                  /* Fill Rate                      */
      int Frm;                  /* Fill Rate Max                  */
      ulong Ffc;                /* Fifo Full Count                */
      ulong Fec;                /* Fifo Empty Count               */
   }
   sFIFO_STS;

   typedef struct
   {
      uchar *pBuf;              /* pointeur sur le buffer d'octet */
      size_t Max;               /* longueur de la FIFO            */
      size_t Ecr;               /* index d'ecriture               */
      size_t Lec;               /* index de lecture               */
      uint Ful:1;               /* 1=FIFO vide                    */
      uint Emp:1;               /* 1=FIFO pleine                  */
      uint Chg:1;               /* 1=Changement (read and clear)  */
      sFIFO_STS *pSts;

   }
   sFIFO;

/* entry points ======================================================== */

/* ---------------------------------------------------------------------
   FIFO_sver()
   ---------------------------------------------------------------------
   Role : Retourne une chaine "Version"
   ---------------------------------------------------------------------
   E :
   S : Pointeur de chaine ASCIIZ
   --------------------------------------------------------------------- */
   const char *FIFO_sver (void);

/* ---------------------------------------------------------------------
   FIFO_sid()
   ---------------------------------------------------------------------
   Role : Retourne une chaine "Identification"
   ---------------------------------------------------------------------
   E :
   S : Pointeur de chaine ASCIIZ
   --------------------------------------------------------------------- */
   const char *FIFO_sid (void);

/* ---------------------------------------------------------------------
   FIFO_mode()
   ---------------------------------------------------------------------
   Role : Retourne le mode de compilation
   ---------------------------------------------------------------------
   E :
   S : Mode de compilation FIFO_MODE_STA FIFO_MODE_DYN
   --------------------------------------------------------------------- */
   eFIFO_MODE FIFO_mode (void);

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
   sFIFO *FIFO_init (size_t const Max);
#define FIFO_create(i) FIFO_init(i)
#else
   void FIFO_init (sFIFO * self
                   ,uchar * pbuf
                   ,size_t const Max
                   ,sFIFO_STS * pSts
   );
#endif
/* ---------------------------------------------------------------------
   FIFO_end()
   ---------------------------------------------------------------------
   Role : destruction DEFINITIVE d'une FIFO
   ---------------------------------------------------------------------
   E : Pointeur sur la FIFO
   S :
   --------------------------------------------------------------------- */
   void FIFO_end (sFIFO * self);
#define FIFO_kill(self) FIFO_end(self)
#define FIFO_delete(self) FIFO_end(self)

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
   void FIFO_reinit (sFIFO * self, size_t const Max);

/* ---------------------------------------------------------------------
   FIFO_resize()
   ---------------------------------------------------------------------
   Role : Modification de la longueur de la FIFO
   La zone de donnees est inchangée. La FIFO n'est pas pleine.
   ---------------------------------------------------------------------
   E : Pointeur sur la FIFO
   E : Nouvelle longueur
   S :
   --------------------------------------------------------------------- */
   void FIFO_resize (sFIFO * self, size_t const Max);
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
   void FIFO_put (sFIFO * self, const int o);

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
   int FIFO_get (sFIFO * self);

/* ---------------------------------------------------------------------
   FIFO_count()
   ---------------------------------------------------------------------
   Role : lecture du remplissage (nombre d'octets dans la FIFO)
   ---------------------------------------------------------------------
   E : Pointeur sur la FIFO
   S : Nombre d'octets dans la FIFO
   --------------------------------------------------------------------- */
   size_t FIFO_count (sFIFO * self);

/* ---------------------------------------------------------------------
   FIFO_empty()
   ---------------------------------------------------------------------
   Role : retourne 1 si FIFO vide
   ---------------------------------------------------------------------
   E : Pointeur sur la FIFO
   S : Etat de la FIFO : 0=NON VIDE 1=VIDE
   --------------------------------------------------------------------- */
   int FIFO_empty (sFIFO * self);

/* ---------------------------------------------------------------------
   FIFO_full()
   ---------------------------------------------------------------------
   Role : retourne 1 si FIFO pleine
   ---------------------------------------------------------------------
   E : Pointeur sur la FIFO
   S : Etat de la FIFO : 0=NON PLEINE 1=PLEINE
   --------------------------------------------------------------------- */
   int FIFO_full (sFIFO * self);

/* ---------------------------------------------------------------------
   FIFO_changed()
   ---------------------------------------------------------------------
   Role : retourne 1 si la FIFO a change
   ---------------------------------------------------------------------
   E : Pointeur sur la FIFO
   S : Etat de la FIFO : 0=NON pas de changement 1=Changement
   --------------------------------------------------------------------- */
   int FIFO_changed (sFIFO * self);

/* ---------------------------------------------------------------------
   FIFO_clear()
   ---------------------------------------------------------------------
   Role : vider la FIFO sans la lire
   La zone de donnees est initialisee a 0. La FIFO est vide.
   ---------------------------------------------------------------------
   E : Pointeur sur la FIFO
   S :
   --------------------------------------------------------------------- */
   void FIFO_clear (sFIFO * self);

/* ---------------------------------------------------------------------
   FIFO_fill_rate()
   ---------------------------------------------------------------------
   Role : retourne le taux de remplissage en %
   ---------------------------------------------------------------------
   E : Pointeur sur la FIFO
   S : 0-100
   --------------------------------------------------------------------- */
   int FIFO_fill_rate (sFIFO * self);

/* ---------------------------------------------------------------------
   FIFO_fill_rate_max()
   ---------------------------------------------------------------------
   Role : retourne le taux de remplissage max atteint en %
   ---------------------------------------------------------------------
   E : Pointeur sur la FIFO
   S : 0-100
   --------------------------------------------------------------------- */
   int FIFO_fill_rate_max (sFIFO * self);

/* ---------------------------------------------------------------------
   FIFO_sts()
   ---------------------------------------------------------------------
   Role : retourne les informations d'etat de la fifo
   ---------------------------------------------------------------------
   E : Pointeur sur la FIFO
   E : Structure d'information
   S :
   --------------------------------------------------------------------- */
   void FIFO_sts (sFIFO * self, sFIFO_STS * pSts);

/* public variables ==================================================== */

/* File generated by 'NEW.EXE' Ver 1.10 (c) ED 1998 */

#ifdef __cplusplus
}
#endif

#endif                          /* guard */

/* Guards added by GUARD (c) ED 2000-2003 Jan  5 2005 Ver. 1.6 */
