/* ---------------------------------------------------------------------
   (c) ED 2005
   Project      : CLIB
   Function     : Double linked list
   Module       : DQ
   File         : dq.c
   Created      : 22-01-2005
   Modified     : 22-01-2005
   --------------------------------------------------------------------- */

/* ---------------------------------------------------------------------
   Log

   0.0 22-01-2005 Created
   1.0 22-01-2005 Initial version

   --------------------------------------------------------------------- */
#ifdef __cplusplus
#error This is not C++. Please use a C compiler.
#endif

#include "ed/inc/dq.h"
#include "ed/inc/sysalloc.h"
#include "ed/inc/sys.h"

/* private macro definitions =========================================== */

#define ID "DQ"
#define VER "1.0"

/* private constants =================================================== */

/* private types ======================================================= */

/* private structures ================================================== */

typedef struct dblink
{
   /* double link */
   struct dblink *pr;
   struct dblink *sv;

   /* data reference */
   void *z;
   size_t siz;
}
dblink_s;

struct dq
{
   size_t nb;
   dblink_s *prem, *dern, *curr;
};

/* private functions =================================================== */

/* ---------------------------------------------------------------------
   allocell ()
   ---------------------------------------------------------------------

   ---------------------------------------------------------------------
   I:
   O:
   --------------------------------------------------------------------- */
static dblink_s *allocell (void *v, size_t siz)
{
   dblink_s *this = malloc (sizeof *this);

   if (this != NULL)
   {
      this->z = calloc (1, siz);

      if (this->z != NULL)
      {
         memcpy (this->z, v, siz);
         this->pr = 0;
         this->sv = 0;
         this->siz = siz;
      }
      else
      {
         FREE (this);
      }
   }
   return this;
}

#if 0
/* unused */
/* ---------------------------------------------------------------------
   ()
   ---------------------------------------------------------------------

   ---------------------------------------------------------------------
   I:
   O:
   --------------------------------------------------------------------- */
static int next (dq_s * this)
{
   ASSERT (this != NULL);

   if (this->curr->sv != NULL)
   {
      this->curr = this->curr->sv;
      return 1;
   }
   return 0;
}

/* ---------------------------------------------------------------------
   ()
   ---------------------------------------------------------------------

   ---------------------------------------------------------------------
   I:
   O:
   --------------------------------------------------------------------- */
static int prev (dq_s * this)
{
   ASSERT (this != NULL);

   if (this->curr->pr != NULL)
   {
      this->curr = this->curr->pr;
      return 1;
   }
   return 0;
}
#endif

/* ---------------------------------------------------------------------
   lesshead ()
   ---------------------------------------------------------------------

   ---------------------------------------------------------------------
   I:
   O:
   --------------------------------------------------------------------- */
static dq_err_e lesshead (dq_s * this)
{
   static dq_err_e err = DQ_OK;
   ASSERT (this != NULL);

   if (this->prem == NULL)
   {
   }
   else if (this->prem == this->dern)
   {
      err = dq_clear (this);
   }
   else
   {
      this->curr = this->prem;
      {
         dblink_s *e = this->curr;

         this->prem = this->curr = e->sv;

         if (e->z != NULL)
         {
            FREE (e->z);
         }

         if (e != NULL)
         {
            FREE (e);
         }
      }
      this->prem->pr = 0;
      this->nb--;
   }
   return err;
}

/* ---------------------------------------------------------------------
   lesstail ()
   ---------------------------------------------------------------------

   ---------------------------------------------------------------------
   I:
   O:
   --------------------------------------------------------------------- */
static dq_err_e lesstail (dq_s * this)
{
   dq_err_e err = DQ_OK;

   ASSERT (this != NULL);

   if (this->dern == NULL)
   {
   }
   else if (this->dern == this->prem)
   {
      err = dq_clear (this);
   }
   else
   {
      this->curr = this->dern;

      {
         dblink_s *e = this->curr;

         this->dern = this->curr = e->pr;

         if (e->z != NULL)
         {
            FREE (e->z);
         }

         if (e != NULL)
         {
            FREE (e);
         }
      }

      this->dern->sv = 0;
      this->nb--;
   }
   return err;
}

/* ---------------------------------------------------------------------
   ()
   ---------------------------------------------------------------------

   ---------------------------------------------------------------------
   I:
   O:
   --------------------------------------------------------------------- */
static dq_err_e removelink (dq_s * this, dblink_s * z)
{
   dq_err_e err = DQ_OK;

   ASSERT (this != NULL);

   if (z != NULL)
   {
      if (this->nb != 0)
      {
         if (z->pr != NULL && z->sv != NULL)
         {
            dblink_s *e = z;

            z = z->sv;
            e->pr->sv = e->sv;
            e->sv->pr = e->pr;

            if (e->z != NULL)
            {
               FREE (e->z);
            }

            if (e != NULL)
            {
               FREE (e);
            }
            this->nb--;
         }
         else if (z->pr == NULL && z->sv != NULL)
         {
            /* first element */
            err = lesshead (this);
         }
         else if (z->pr != NULL && z->sv == NULL)
         {
            /* last element */
            err = lesstail (this);
         }
         else if (z->pr == NULL && z->sv == NULL)
         {
            /* the list has one element left */
            this->prem = this->dern;
            err = dq_clear (this);
         }
         else
         {
            /* all the cases has been processed */
            ASSERT (0);
         }
      }
      else
      {
         err = DQ_ERR_EMPTY_LIST;
      }
   }
   else
   {
      err = DQ_ERR_DATA_NULL;
   }

   return err;
}

/* ---------------------------------------------------------------------
   dq_sid ()
   ---------------------------------------------------------------------

   ---------------------------------------------------------------------
   I:
   O:
   --------------------------------------------------------------------- */
static dblink_s *find (dq_s * this, void *v, size_t siz)
{
   ASSERT (this != NULL);
   {
      dblink_s *start = this->prem;

      while (start != 0)
      {
         if (start->siz == siz)
         {
            if (0 == memcmp (start->z, v, siz))
            {
               return start;
            }
         }
         start = start->sv;
      }
   }
   return NULL;
}

/* public internal functions =========================================== */

/* entry points ======================================================== */

/* ---------------------------------------------------------------------
   dq_sid ()
   ---------------------------------------------------------------------

   ---------------------------------------------------------------------
   I:
   O:
   --------------------------------------------------------------------- */
char const *dq_sid (void)
{
   return ID;
}

/* ---------------------------------------------------------------------
   dq_sver ()
   ---------------------------------------------------------------------

   ---------------------------------------------------------------------
   I:
   O:
   --------------------------------------------------------------------- */
char const *dq_sver (void)
{
   return VER;
}

/* ---------------------------------------------------------------------
   dq_create ()
   ---------------------------------------------------------------------

   ---------------------------------------------------------------------
   I:
   O:
   --------------------------------------------------------------------- */
dq_s *dq_create (void)
{
   dq_s *this = malloc (sizeof *this);
   if (this != NULL)
   {
      CLR (this, dq_s);
   }
   return this;
}

/* ---------------------------------------------------------------------
   dq_delete ()
   ---------------------------------------------------------------------

   ---------------------------------------------------------------------
   I:
   O:
   --------------------------------------------------------------------- */
void dq_delete (dq_s * this)
{
   if (this != NULL)
   {
      free (this);
   }
}

/* ---------------------------------------------------------------------
   dq_getsize ()
   ---------------------------------------------------------------------

   ---------------------------------------------------------------------
   I:
   O:
   --------------------------------------------------------------------- */
dq_err_e dq_getsize (dq_s * this, size_t * p_nb)
{
   dq_err_e err = DQ_OK;

   if (this != NULL)
   {
      if (p_nb != NULL)
      {
         *p_nb = this->nb;
      }
   }
   else
   {
      err = DQ_ERR_CONTEXT;
   }
   return err;
}

/* ---------------------------------------------------------------------
   dq_clear ()
   ---------------------------------------------------------------------

   ---------------------------------------------------------------------
   I:
   O:
   --------------------------------------------------------------------- */
dq_err_e dq_clear (dq_s * this)
{
   dq_err_e err = DQ_OK;

   if (this != NULL)
   {
      dblink_s *z = this->prem;

      while (z != NULL)
      {
         dblink_s *t = z->sv;

         if (z->z)
         {
            FREE (z->z);
         }

         if (z != NULL)
         {
            FREE (z);
         }

         z = t;
      }
      this->nb = 0;
   }
   else
   {
      err = DQ_ERR_CONTEXT;
   }
   return err;
}

/* ---------------------------------------------------------------------
   dq_addtail ()
   ---------------------------------------------------------------------

   ---------------------------------------------------------------------
   I:
   O:
   --------------------------------------------------------------------- */
dq_err_e dq_addtail (dq_s * this, void *v, size_t siz)
{
   dq_err_e err = DQ_OK;

   if (this != NULL)
   {
      dblink_s *p = allocell (v, siz);

      if (p != NULL)
      {
         if (this->prem == 0)
         {
            this->prem = this->dern = p;
         }
         else
         {
            this->dern->sv = p;
            p->pr = this->dern;
         }
         this->dern = p;
         this->nb++;
      }
      else
      {
         err = DQ_ERR_MEMORY;
      }
   }
   else
   {
      err = DQ_ERR_CONTEXT;
   }
   return err;
}

/* ---------------------------------------------------------------------
   ()
   ---------------------------------------------------------------------

   ---------------------------------------------------------------------
   I:
   O:
   --------------------------------------------------------------------- */
dq_err_e dq_addhead (dq_s * this, void *v, size_t siz)
{
   dq_err_e err = DQ_OK;

   if (this != NULL)
   {
      dblink_s *p = allocell (v, siz);

      if (p != NULL)
      {

         if (this->prem == 0)
         {
            this->prem = this->dern = p;
         }
         else
         {
            this->prem->pr = p;
            p->sv = this->prem;
         }
         this->prem = p;
         this->nb++;
      }
      else
      {
         err = DQ_ERR_MEMORY;
      }
   }
   else
   {
      err = DQ_ERR_CONTEXT;
   }
   return err;
}

/* ---------------------------------------------------------------------
   ()
   ---------------------------------------------------------------------

   ---------------------------------------------------------------------
   I:
   O:
   --------------------------------------------------------------------- */
dq_err_e dq_remove (dq_s * this, void *v, size_t siz)
{
   dq_err_e err = DQ_OK;

   if (this != NULL)
   {
      dblink_s *z = find (this, v, siz);
      err = removelink (this, z);
   }
   else
   {
      err = DQ_ERR_CONTEXT;
   }
   return err;
}

/* ---------------------------------------------------------------------
   ()
   ---------------------------------------------------------------------

   ---------------------------------------------------------------------
   I:
   O:
   --------------------------------------------------------------------- */
dq_err_e dq_list (dq_s * this, int rev, dq_list_f * pf, void *p_user)
{
   dq_err_e err = DQ_OK;

   if (this != NULL)
   {
      int stop = 0;
      if (!rev)
      {

         this->curr = this->prem;
         while (!stop && this->curr != NULL)
         {
            if (pf != NULL)
            {
               stop = pf (p_user, this->curr->z);
            }
            this->curr = this->curr->sv;
         }
      }
      else
      {
         this->curr = this->dern;
         while (!stop && this->curr != NULL)
         {
            if (pf != NULL)
            {
               stop = pf (p_user, this->curr->z);
            }
            this->curr = this->curr->pr;
         }
      }
   }
   else
   {
      err = DQ_ERR_CONTEXT;
   }
   return err;
}

/* public data ========================================================= */

/* ---------------------------------------------------------------------
   Generated by NEW (c) ED 2.5
   Powered by C-code generator (c) ED 2003 1.0
   --------------------------------------------------------------------- */
