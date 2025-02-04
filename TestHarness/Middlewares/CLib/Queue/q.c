
#ifdef __cplusplus
#error This source file is not C++ but rather C. Please use a C-compiler
#endif

#include "q.h"



/* macros ============================================================== */


#define ID "Q Module"

/* constants =========================================================== */
/* types =============================================================== */
/* structures ========================================================== */

typedef struct
{
   void* pNext;
}

sELEM;

/* private data ======================================================== */
/* private functions =================================================== */

/* ---------------------------------------------------------------------
   IsFull()
   ---------------------------------------------------------------------
   Role : FIFO ou LIFO pleine ?
   ---------------------------------------------------------------------
   E : Pointeur de donnees
   S : 0=Non pleine 1=pleine
   --------------------------------------------------------------------- */
static int IsFull (sQ * p)
{
   int ful = 0;

   if (p->limite)
   {
      ful = p->nb_elem == p->limite;
   }

   return ful;
}

/* internal public data ================================================ */
/* internal public functions =========================================== */
/* entry points ======================================================== */

const char *Q_sid (void)
{
   return ID;
}

sQ *Q_init (size_t limite)
{
   sQ *const p = malloc (sizeof *p);

   ENUM_CHECK ();

   if (p)
   {
      /* init a 0 */
      SYS_CLR (p, sQ);

      p->limite = limite;
   }
   return p;
}

void Q_end (sQ * p)
{
   if (p != NULL)
   {
      free (p);
   }
}

void Q_clear (sQ * p)
{
   p->pFront = NULL;
   p->pBack = NULL;
   p->nb_elem = 0;
}

void *Q_list (sQ * const p, fQ_CB * pfCallback, void *const pUser)
{
   void *pOld = NULL;
   int fin = 0;
   sELEM *pe = p->pFront;

   while (pe && !fin)
   {
      pOld = pe;
      /* Il faut le faire avant, sinon, on risque de se faire voler le
         pointeur par le callback (liberation...) */
      pe = pe->pNext;

      if (pfCallback)
      {
         /* LCLint */
         fin = (*pfCallback) (pOld, pUser);
      }
   }

   if (!fin)
   {
      pOld = NULL;
   }

   return pOld;
}

/* ---------------------------------------------------------------------
   Q_AddBack()
   ---------------------------------------------------------------------
   . add to the back of a Queue
   .
   . 1ere fois
   . ---------
   . front                    front
   . |                        |
   . v                        v
   . [0]         AddBack(a)   [a]-->[0]
   . ^                        ^
   . |                        |
   . back                     back
   .
   . Autres fois
   . -----------
   . front                    front
   . |                        |
   . v                        v
   . [a]-->[0]   AddBack(a)   [a]-->[b]-->[0]
   . ^                              ^
   . |                              |
   . back                           back
   .
   ---------------------------------------------------------------------
   E : a structure of type sQ
   E : pointer to element to add
   --------------------------------------------------------------------- */
void Q_AddBack (sQ * p, void *peNew)
{
   ASSERT (p != NULL);
   ASSERT (peNew != NULL);

   if (p->pFront == NULL)
   {
      /* Premiere fois (liste vide) */
      p->pFront = peNew;
   }
   else
   {
      /* Autres fois (liste non vide) */
      sELEM *pe = p->pBack;
      /* creer le lien */
      pe->pNext = peNew;
   }

   p->pBack = peNew;
}

/* ---------------------------------------------------------------------
   Q_AddFront()
   ---------------------------------------------------------------------
   add to the front of a Queue
   !!! ATTENTION !!! La chronologie est modifiee
   (OK pour evenement prioritaire)

   1ere fois
   ---------
   front                    front
   |                        |
   v                        v
   [0]         AddFront(a)   [a]-->[0]
   ^                        ^
   |                        |
   back                     back

   Autres fois
   -----------
   front                    front
   |                        |
   v                        v
   [a]-->[0]   AddFront(a)  [b]-->[a]-->[0]
   ^                              ^
   |                              |
   back                           back

   ---------------------------------------------------------------------
   E : a structure of type sQ
   E : pointer to element to add
   --------------------------------------------------------------------- */
void Q_AddFront (sQ * p, void *peNew)
{
   ASSERT (p != NULL);
   ASSERT (peNew != NULL);

   if (p->pBack == NULL)
   {
      /* Premiere fois (liste vide) */
      p->pBack = peNew;
   }
   else
   {
      /* Autres fois (liste non vide) */
      sELEM *pe = peNew;
      /* creer le lien */
      pe->pNext = p->pFront;
   }

   p->pFront = peNew;
}

/* ---------------------------------------------------------------------
   Q_IsEmpty()
   ---------------------------------------------------------------------
   Role : liste vide ?
   ---------------------------------------------------------------------
   E : pointeur sur la liste
   S : 0=Non vide 1=Vide
   --------------------------------------------------------------------- */
int Q_IsEmpty (sQ * p)
{
   return p->pFront == NULL;
}

/* ---------------------------------------------------------------------
   Q_FIFO_IsFull()
   ---------------------------------------------------------------------
   Role : FIFO pleine ?
   ---------------------------------------------------------------------
   E : Pointeur de donnees
   S : 0=Non pleine 1=pleine
   --------------------------------------------------------------------- */
int Q_FIFO_IsFull (sQ * p)
{
   return IsFull (p);
}

/* ---------------------------------------------------------------------
   Q_DisFront()
   ---------------------------------------------------------------------
   add to the front of a Queue
   !!! ATTENTION !!! La chronologie est modifiee
   (OK pour evenement prioritaire)

   Autres fois
   -----------
   front                        front
   |                            |
   v                            v
   [b]-->[a]-->[0] b=DisFront() [a]-->[0]
   ^                      ^
   |                      |
   back                   back

   ---------------------------------------------------------------------
   E : a structure of type sQ
   S : pointer to removed element
   --------------------------------------------------------------------- */
void *Q_DisFront (sQ * p)
{
   sELEM *pe;

   ASSERT (p != NULL);

   /* get element address */
   pe = p->pFront;

   ASSERT (pe != NULL);

   /* discard element */
   p->pFront = pe->pNext;
   if (p->pFront == NULL)
   {
      p->pBack = NULL;
   }
   /* returns element address */
   return pe;
}

/* ---------------------------------------------------------------------
   Q_ReadFront()
   ---------------------------------------------------------------------
   peek the front element of the queue

   front--.
   |
   v
   [y]-->[x]-->[0]
   ^
   |
   back

   x is returned

   ---------------------------------------------------------------------
   E : q a structure of type sQ
   S : address of the front element
   --------------------------------------------------------------------- */
void *Q_ReadFront (sQ * p)
{
   ASSERT (p != NULL);
   return (p->pFront);
}

/* ---------------------------------------------------------------------
   Q_ReadBack()
   ---------------------------------------------------------------------
   peek the back element of the queue

   front--.
   |
   v
   [y]-->[x]-->[0]
   ^
   |
   back

   y is returned

   ---------------------------------------------------------------------
   E : q a structure of type sQ
   S : address of the back element
   --------------------------------------------------------------------- */
void *Q_ReadBack (sQ * p)
{
   ASSERT (p != NULL);
   return (p->pBack);
}

/* ---------------------------------------------------------------------
   Q_IsFront()
   ---------------------------------------------------------------------
   Role : Cet element est-il au debut de la liste ?
   ---------------------------------------------------------------------
   E : Pointeur de donnees
   S : 0=Pas le debut 1=Debut
   --------------------------------------------------------------------- */
int Q_IsFront (sQ * p, void *pe)
{
   return (p->pFront == pe);
}

/* ---------------------------------------------------------------------
   Q_Insert()
   ---------------------------------------------------------------------
   Role : Inserer un element apres l'element courant ou avant si c'st
   le 1er
   C -> x  : C -> N -> x
   0       : x -> C -> N AddBack()

   ---------------------------------------------------------------------
   E : Pointeur de donnees
   E : Element Courant
   E : Element Nouveau
   --------------------------------------------------------------------- */
void Q_Insert (sQ * const pQ, void *const pC, void *const pN)
{
   if (pQ != NULL && pN != NULL)
   {
      if (pC)
      {
         sELEM *pCurr = pC;
         sELEM *pNew = pN;

         if (pCurr->pNext != NULL)
         {
            /* insert */
            pNew->pNext = pCurr->pNext;
            pCurr->pNext = pNew;
         }
         else
         {
            Q_AddFront (pQ, pN);
         }
      }
      else
      {
         Q_AddBack (pQ, pN);
      }
   }
}

/* ---------------------------------------------------------------------
   Q_FIFO_put()
   ---------------------------------------------------------------------
   Role : Mettre un element dans la liste (methode First In First Out)
   ---------------------------------------------------------------------
   E : Pointeur de donnees
   E : Element
   --------------------------------------------------------------------- */
void Q_FIFO_put (sQ * pQ, void *pe)
{
   if (pQ != NULL)
   {
      if (!Q_FIFO_IsFull (pQ))
      {
         /* Ne pas mettre d'element NULL */
         if (pe != NULL)
         {
            sELEM *pElem = pe;

            /* Ne pas mettre d'element autre que fin */
            if (pElem->pNext == NULL)
            {
               Q_AddBack (pQ, pe);
               pQ->nb_elem++;
            }
         }
      }
   }
}

/* ---------------------------------------------------------------------
   Q_FIFO_get()
   ---------------------------------------------------------------------
   Role : Retirer un element dans la liste (methode First In First Out)
   ---------------------------------------------------------------------
   E : Pointeur de donnees
   S : Element
   --------------------------------------------------------------------- */
void *Q_FIFO_get (sQ * pQ)
{
   void *pe = NULL;

   if (pQ != NULL)
   {
      if (!Q_IsEmpty (pQ))
      {
         /* Le plus ancien */
         pe = Q_DisFront (pQ);

         /* Il ne devrait pas y avoir d'element NULL */
         if (pe != NULL)
         {
            /* Nettoyer l'element */
            sELEM *pElem = pe;

            if (pElem->pNext)
            {
               pElem->pNext = NULL;
            }
         }
         pQ->nb_elem--;
      }
   }
   return pe;
}

/* ---------------------------------------------------------------------
   Q_FIFO_root()
   ---------------------------------------------------------------------
   Role : Retourne la racine de la liste constituant la FIFO
   ---------------------------------------------------------------------
   E : Pointeur de donnees
   S : 1 er Element ou NULL (rien dans la FIFO
   --------------------------------------------------------------------- */
void *Q_FIFO_root (sQ * pQ)
{
   return Q_ReadFront (pQ);
}

/* ---------------------------------------------------------------------
   Q_LIFO_IsFull()
   ---------------------------------------------------------------------
   Role : LIFO pleine ?
   ---------------------------------------------------------------------
   E : Pointeur de donnees
   S : 0=Non pleine 1=pleine
   --------------------------------------------------------------------- */
int Q_LIFO_IsFull (sQ * p)
{
   return IsFull (p);
}

/* ---------------------------------------------------------------------
   Q_LIFO_push()
   ---------------------------------------------------------------------
   Role : Mettre un element dans la liste (methode Last In First Out)
   ---------------------------------------------------------------------
   E : Pointeur de donnees
   E : Element
   --------------------------------------------------------------------- */
int Q_LIFO_push (sQ * pQ, void *pe)
{
   int err = 0;

   if (pQ != NULL)
   {
      if (!IsFull (pQ))
      {
         /* Ne pas mettre d'element NULL */
         if (pe != NULL)
         {
            sELEM *pElem = pe;

            /* Ne pas mettre d'element autre que fin */
            if (pElem->pNext == NULL)
            {
               Q_AddFront (pQ, pe);
               pQ->nb_elem++;
            }
         }
      }
      else
      {
         err = 1;
      }
   }
   else
   {
      err = 1;
   }
   return err;
}

/* ---------------------------------------------------------------------
   Q_LIFO_pop()
   ---------------------------------------------------------------------
   Role : Retirer un element dans la liste (methode Last In First Out)
   ---------------------------------------------------------------------
   E : Pointeur de donnees
   S : Element
   --------------------------------------------------------------------- */
void *Q_LIFO_pop (sQ * pQ)
{
   return Q_FIFO_get (pQ);
}

/* ---------------------------------------------------------------------
   Q_LOOP_add()
   ---------------------------------------------------------------------
   Role : Ajouter un element dans la boucle (n'importe ou en fait)
   ---------------------------------------------------------------------
   E : Pointeur de donnees
   E : Element
   --------------------------------------------------------------------- */
void Q_LOOP_add (sQ * pQ, void *pe)
{
   if (pQ != NULL)
   {
      if (pQ->pFront == NULL)
      {
         pQ->pBack = pe;
      }
      else
      {
         sELEM *pFront = pQ->pFront;
         pFront->pNext = pe;
      }

      pQ->pFront = pe;

      {
         sELEM *pElem = pQ->pFront;
         pElem->pNext = pQ->pBack;  /* boucle */
      }

      pQ->nb_elem++;

   }
}

/* ---------------------------------------------------------------------
   Q_LOOP_rem()
   ---------------------------------------------------------------------
   Role : Supprimer un element dans la boucle (n'importe le quel)
   ---------------------------------------------------------------------
   E : Pointeur de donnees
   E : Element
   --------------------------------------------------------------------- */
void Q_LOOP_rem (sQ * pQ, void *peRem)
{
   ASSERT (pQ != NULL);

   if ((pQ->pFront != NULL)
       && (pQ->pBack != NULL))
   {

      /* dernier element (celui qui est boucle) */
      if (peRem == ((sELEM *) peRem)->pNext)
      {
         pQ->pFront = NULL;
         pQ->pBack = NULL;
      }
      else
      {
         int fin = 0;
         size_t nb = 0;
         sELEM *pe = pQ->pBack;
         void *pPrev = NULL;

         /* parcourir la liste en recherchant pe */
         while (pe && !fin && (nb != pQ->nb_elem))
         {
            /* precedant */
            pPrev = pe;
            pe = pe->pNext;
            /*
               Est-ce que le suivant est celui
               qu'il faut supprimmer ?
             */
            fin = pe == peRem;
            nb++;
         }

/*
   Si on l'a trouve, le retirer de la liste
   Il est de la responsabilite de l'appelant de liberer l'element
 */
         if (fin)
         {
            /* si on supprimme le BACK */
            if (pQ->pBack == peRem)
            {
               pQ->pBack = ((sELEM *) peRem)->pNext;
            }
            /* ou si on supprimme le FRONT */
            else if (pQ->pFront == peRem)
            {
               pQ->pFront = pPrev;
            }
            /* autres cas */
            else
            {
               ((sELEM *) pPrev)->pNext = ((sELEM *) peRem)->pNext;
            }

            /* boucle (toujours vrai) (peut etre redondant...) */
            ((sELEM *) pQ->pFront)->pNext = pQ->pBack;

#if 1
/* ED/07-06-2000 : Ca me parait plus logique comme ca
 */
            /* mise a jour du nombre d'elements */
            if (pQ->nb_elem)
            {
               pQ->nb_elem--;
            }
#endif
         }
      }
#if 0
/* ED/07-06-2000 : bizarre... on retire 1
 *                 meme si l'element est absent...
 */
      /* mise a jour du nombre d'elements */
      if (pQ->nb_elem)
      {
         pQ->nb_elem--;
      }
#endif
   }

}

/* ---------------------------------------------------------------------
   Q_LOOP_list()
   ---------------------------------------------------------------------
   Role : Parcourir la liste tant que le callback retourne 0
   (cas d'une liste bouclee)
   ---------------------------------------------------------------------
   E : Pointeur de donnees
   E : Callback utilisateur
   E : Pointeur de donnees  utilisateur
   S :

   Exemple de callback :

   typedef struct
   {
   void *pNext; // Usage interne de Q
   int n;
   char *sz;
   }
   sELEM;

   static int cb_free_elem(void *p, void *pUser)
   {
   sUSER_ELEM *pe=p;
   ASSERT(pe!=NULL);

   if (pe->sz)
   free(pe->sz);

   free(pe);

   pUser=pUser; // Non utilise. Si il fallait acceder a quelques globales...

   return 0; // Ou 1 (fonctions de recherche...)
   }
   --------------------------------------------------------------------- */
void *Q_LOOP_list (sQ * p, fQ_CB * pfCallback, void *pUser)
{
   void *pOld = NULL;
   ASSERT (p != NULL);
   {
      int fin = 0;
      sELEM *pe = p->pBack;
      size_t nb = 0;

      while (pe && !fin && (nb != p->nb_elem))
      {
         pOld = pe;
         pe = pe->pNext;
         if (pfCallback)
         {
            /* LCLint */
            fin = (*pfCallback) (pOld, pUser);
         }
         nb++;
      }

      if (!fin)
      {
         pOld = NULL;
      }
   }
   return pOld;
}

/* public data ========================================================= */

/* File generated by 'NEW.EXE' Ver 1.10 (c) ED 1998 */
