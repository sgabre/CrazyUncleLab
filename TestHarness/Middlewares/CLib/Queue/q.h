#ifndef H_ED_Q_20051110162839
#define H_ED_Q_20051110162839

#ifdef __cplusplus
extern "C"
{
#endif

/* ---------------------------------------------------------------------
   (c) ED 1998-2002
   Projet       : CLIB
   Fonction     : Gestion des listes chainees
   Module       : Q
   Fichier      : Q.H
   Creation     : 04-08-1998
   Modification : 26-05-2003
   --------------------------------------------------------------------- */

/* ---------------------------------------------------------------------
   Journal

   0.0 du 04-08-1998 Creation
   1.0 du 04-08-1998 Version operationelle
   1.1 du 07-06-1999 Ajout de quelques controles
   .                 Mise au point
   .                 Suppression des //
   1.2 du 05-10-1999 Ajout des fonction Q_LOOP
   1.3 du 13-10-1999 remplacement des calloc/free par sys_malloc/sys_free
   1.4 du 08-12-1999 Implementation des FIFO limitees
   1.5 du 07-01-2000 Casting Strict ANSI
   1.6 du 22-02-2000 Ajoute Q_IsFront() et Q_Insert()
   1.7 du 07-06-2000 Mise au point LOOP
   1.8 du 13-07-2002 LCLint
   1.9 du 26-05-2003 Implementation de LIFO (push/pop)

   Specifications
   --------------

   Les outils suivant permettent de gerer une liste chainee d'elements.

   Le pointeur 'front' designe l'avant de la file
   Le pointeur 'back' designe l'arriere de la file

   La seule contrainte est que le premier champ de la structure de
   l'element doit etre un pointeur. Celui-ci est gere par l'objet,
   et inaccessible par l'utilisateur (void*)

   typedef struct
   {
   void *pNext; // Obligatoirement en tete
   int UserData;

   } USER_ELEM



   front
   |
   v
   [a]-->[b]-->[c]-->[d]->[0]
   ^
   |
   back

   Les fonctions sont :


   - init      : Creation d'une liste vide
   - end       : Suppression de la liste
   - Clear     : Effacement de la liste (tout est perdu)

   - AddBack   : Ajout d'un element en fin de liste (EnQueue)
   - AddFront  : Ajout d'un element en debut de liste

   - GetBack   : Retrait d'un element en fin de liste
   - DisFront  : Retrait d'un element en debut de liste (DeQueue)

   - ReadBack  : Lecture de l'element de fin de liste
   - ReadFront : Lecture de l'element de debut de liste

   Par une bonne utilisation des fonctions d'ajout et de retrait,
   il est possible de realiser une FIFO ou une LIFO


   FIFO (ou file, pipe, buffer tourant... ) :

   fifo=init()                             [0]

   Mise en file :


   put(fifo,pe) -> AddBack(p,pe)   >a   [a]-->[0]
   put(fifo,pe) -> AddBack(p,pe)   >b   [b]-->[a]-->[0]
   put(fifo,pe) -> AddBack(p,pe)   >c   [c]-->[b]-->[a]-->[0]

   Recuperation

   pe=get(fifo)  -> DisFront(p,pe) a<   [c]-->[b]-->[0]
   pe=get(fifo)  -> DisFront(p,pe) b<   [c]-->[0]
   pe=get(fifo)  -> DisFront(p,pe) c<   [0]


   LIFO (ou pile) :

   lifo=init()                             [0]

   Mise en file :

   put(lifo,pe) -> AddBack(p,pe)   >a   [a]-->[0]
   put(lifo,pe) -> AddBack(p,pe)   >b   [b]-->[a]-->[0]
   put(lifo,pe) -> AddBack(p,pe)   >c   [c]-->[b]-->[a]-->[0]

   Recuperation

   pe=get(lifo)  -> DisBack(p,pe)  <c   [b]-->[a]-->[0]
   pe=get(lifo)  -> DisBack(p,pe)  <b   [a]-->[0]
   pe=get(lifo)  -> DisBack(p,pe)  <a   [0]


   NOTA : Il n'est pas de la responsabilite de l'objet Q de gerer la
   liberation eventuelle des elements d'une file.


   LOOP (Liste bouclee

   loop = init()                             [0]

   ajout :

   add(loop,a)                               [a]-->[a]
   add(loop,b)                               [a]-->[b]-->[a]
   add(loop,c)                               [a]-->[b]-->[c]->[a]

   retrait :

   rem(loop,b)                               [a]-->[c]->[a]


   Une fonction de liste avec callback applicatif est fournie :

   list(p,callback)

   le callback doit etre de type int callback (void*)
   Le parametre est l'element courant. Si il vaut NULL, il y a erreur

   Si le retour vaut 0, on continue. Si il vaut 1, le parcours de la liste
   est interrompu.

   La fonction list retourne l'adresse du dernier element lu


   --------------------------------------------------------------------- */

/* Exemple d'utilisation ----------------------------------------------- */

#if 0
   typedef struct USER_ELEM
   {
      struct USER_ELEM *Pnext;  /* Obligatoirement en tete */
      int data;
      union
      {
         seek;
      }
      param;

   }
   USER_ELEM;

   static int print_list (void *p, void *pUser)
   {
      USER_ELEM *pe = p;
        printf ("pe=%p data=%u\n", pe, pe->data);
        return 0;
   }

   static int free_elem (void *p, void *pUser)
   {
      free (p);
      return 0;
   }

   static int seek_elem (void *p, void *pUser)
   {
      return (p->data == (int *) pUser);
   }

   {
      sQ *list = Q_init ();

      USER_ELEM *p = ReadFront (liste);

      /* Affichage de la liste des elements */
        Q_list (list, print_elem);

      /* Recherche d'un element dans la liste */
      {
         int data = 3;
           p = Q_list (list, seek_elem, &data);
           print_elem (p, NULL);
      }

      /* Liberation des elements */
      Q_list (list, free_elem, NULL);

      Q_end (list);
   }
#endif

/* Exemple de callback ------------------------------------------------- */
#if 0
   typedef struct
   {
      /* Usage interne de Q */
      void *pNext;
      int n;
      char *sz;
   }
   sELEM;

   static int cb_free_elem (void *p, void *pUser)
   {
      sUSER_ELEM *pe = p;
        assert (pe != NULL);

      if (pe->sz)
      {
         free (pe->sz);
      }
      free (pe);

      /* Non utilise. Si il fallait acceder a quelques globales... */
      (void) pUser;

      /* Ou 1 (fonctions de recherche...) */
      return 0;
   }
#endif

/* --------------------------------------------------------------------- */

#include <stddef.h>

/* macros ============================================================== */
/* constants =========================================================== */
/* types =============================================================== */

   typedef int fQ_CB (void *pElem, void *pUser);

/* structures ========================================================== */

   typedef struct
   {
      void *pFront;
      void *pBack;
      size_t nb_elem;           /* LOOP */
      size_t limite;            /* FIFO a taille fixe */
   }
   sQ;

/* internal public data ================================================ */
/* internal public functions =========================================== */

/* ---------------------------------------------------------------------
   Q_sver()
   ---------------------------------------------------------------------
   Role : Retourne une chaine "Version"
   ---------------------------------------------------------------------
   E :
   S : Pointeur de chaine ASCIIZ
   --------------------------------------------------------------------- */
   const char *Q_sver (void);

/* ---------------------------------------------------------------------
   Q_sid()
   ---------------------------------------------------------------------
   Role : Retourne une chaine "Identification"
   ---------------------------------------------------------------------
   E :
   S : Pointeur de chaine ASCIIZ
   --------------------------------------------------------------------- */
   const char *Q_sid (void);

/* ---------------------------------------------------------------------
   Q_init()
   ---------------------------------------------------------------------
   Role : Creation/Initialisation
   ---------------------------------------------------------------------
   E :
   S : Pointeur de donnees
   --------------------------------------------------------------------- */
   sQ *Q_init (size_t limite);

/* ---------------------------------------------------------------------
   Q_end()
   ---------------------------------------------------------------------
   Role : Suppression
   ---------------------------------------------------------------------
   E : Pointeur de donnees
   S :
   --------------------------------------------------------------------- */
   void Q_end (sQ * p);

/* ---------------------------------------------------------------------
   Q_list()
   ---------------------------------------------------------------------
   Role : Parcourir la liste tant que le callback retourne 0

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
   assert(pe!=NULL);

   if (pe->sz)
   free(pe->sz);

   free(pe);

   pUser=pUser; // Non utilise. Si il fallait acceder a quelques globales...

   return 0; // Ou 1 (fonctions de recherche...)
   }
   --------------------------------------------------------------------- */
   void *Q_list (sQ * p, fQ_CB * pfCallback, void *pUser);

/* ---------------------------------------------------------------------
   Q_clear()
   ---------------------------------------------------------------------
   Clear a Queue, all the content of the queue is lost!

   front
   |
   v
   [0]
   ^
   |
   back

   ---------------------------------------------------------------------
   E : q a structure of type Q
   --------------------------------------------------------------------- */
   void Q_clear (sQ * q);

/* ---------------------------------------------------------------------
   Q_AddBack()
   ---------------------------------------------------------------------
   add to the back of a Queue

   1ere fois
   ---------
   front                    front
   |                        |
   v                        v
   [0]         AddBack(a)   [a]-->[0]
   ^                        ^
   |                        |
   back                     back

   Autres fois
   -----------
   front                    front
   |                        |
   v                        v
   [a]-->[0]   AddBack(a)   [a]-->[b]-->[0]
   ^                              ^
   |                              |
   back                           back

   ---------------------------------------------------------------------
   E : structure of type sQ
   E : pointer to element to add
   --------------------------------------------------------------------- */
   void Q_AddBack (sQ * p, void *pe);
#define Q_EnQueue(p,pe)  Q_AddBack(p,pe)

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
   void Q_AddFront (sQ * p, void *peNew);

/* ---------------------------------------------------------------------
   Q_DisFront()
   ---------------------------------------------------------------------
   remove from the front of a Queue

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
   void *Q_DisFront (sQ * p);

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
   void *Q_ReadFront (sQ * q);

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
   void *Q_ReadBack (sQ * q);

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
   void Q_Insert (sQ * const pQ, void *const pC, void *const pN);

/* ---------------------------------------------------------------------
   Q_IsFront()
   ---------------------------------------------------------------------
   Role : Cet element est-il au debut de la liste ?
   ---------------------------------------------------------------------
   E : Pointeur de donnees
   S : 0=Pas le debut 1=Debut
   --------------------------------------------------------------------- */
   int Q_IsFront (sQ * p, void *pe);

/* ---------------------------------------------------------------------
   Q_IsEmpty()
   ---------------------------------------------------------------------
   Role : liste vide ?
   ---------------------------------------------------------------------
   E : Pointeur de donnees
   S : 0=Non vide 1=Vide
   --------------------------------------------------------------------- */
   int Q_IsEmpty (sQ * p);

/* ---------------------------------------------------------------------
   Q_FIFO_IsFull()
   ---------------------------------------------------------------------
   Role : liste pleine ?
   ---------------------------------------------------------------------
   E : Pointeur de donnees
   S : 0=Non pleine 1=pleine
   --------------------------------------------------------------------- */
   int Q_FIFO_IsFull (sQ * p);

/* ---------------------------------------------------------------------
   Q_FIFO_put()
   ---------------------------------------------------------------------
   Role : Mettre un element dans la liste (methode First In First Out)
   ---------------------------------------------------------------------
   E : Pointeur de donnees
   E : Element
   --------------------------------------------------------------------- */
   void Q_FIFO_put (sQ * list, void *pe);

/* ---------------------------------------------------------------------
   Q_FIFO_get()
   ---------------------------------------------------------------------
   Role : Retirer un element dans la liste (methode First In First Out)
   ---------------------------------------------------------------------
   E : Pointeur de donnees
   S : Element
   --------------------------------------------------------------------- */
   void *Q_FIFO_get (sQ * list);

/* ---------------------------------------------------------------------
   Q_FIFO_root()
   ---------------------------------------------------------------------
   Role : Retourne la racine de la liste constituant la FIFO
   ---------------------------------------------------------------------
   E : Pointeur de donnees
   S : 1 er Element ou NULL (rien dans la FIFO
   --------------------------------------------------------------------- */
   void *Q_FIFO_root (sQ * list);


/* ---------------------------------------------------------------------
   Q_LIFO_push()
   ---------------------------------------------------------------------
   Role : Mettre un element dans la liste (methode Last In First Out)
   ---------------------------------------------------------------------
   E : Pointeur de donnees
   E : Element
   O: 0 = OK 1 = FULL
   --------------------------------------------------------------------- */
   int Q_LIFO_push (sQ * pQ, void *pe);

/* ---------------------------------------------------------------------
   Q_LIFO_pop()
   ---------------------------------------------------------------------
   Role : Retirer un element dans la liste (methode Last In First Out)
   ---------------------------------------------------------------------
   E : Pointeur de donnees
   S : Element
   --------------------------------------------------------------------- */
   void *Q_LIFO_pop (sQ * pQ);

/* ---------------------------------------------------------------------
   Q_LIFO_IsFull()
   ---------------------------------------------------------------------
   Role : LIFO pleine ?
   ---------------------------------------------------------------------
   E : Pointeur de donnees
   S : 0=Non pleine 1=pleine
   --------------------------------------------------------------------- */
   int Q_LIFO_IsFull (sQ * p);

/* ---------------------------------------------------------------------
   Q_LOOP_add()
   ---------------------------------------------------------------------
   Role : Ajouter un element dans la boucle (n'importe ou en fait)
   ---------------------------------------------------------------------
   E : Pointeur de donnees
   E : Element
   --------------------------------------------------------------------- */
   void Q_LOOP_add (sQ * list, void *pe);

/* ---------------------------------------------------------------------
   Q_LOOP_rem()
   ---------------------------------------------------------------------
   Role : Supprimer un element dans la boucle (n'importe le quel)
   ---------------------------------------------------------------------
   E : Pointeur de donnees
   E : Element
   --------------------------------------------------------------------- */
   void Q_LOOP_rem (sQ * list, void *pe);

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
   --------------------------------------------------------------------- */
   void *Q_LOOP_list (sQ * p, fQ_CB * pfCallback, void *pUser);

/* entry points ======================================================== */
/* public data ========================================================= */

/* --------------------------------------------------------------------- */

/* File generated by 'NEW.EXE' Ver 1.10 (c) ED 1998 */

#ifdef __cplusplus
}
#endif

#endif /* guard */

/* Guards added by GUARD (c) ED 2000-2005 Jan 17 2005 Ver. 1.7 */
