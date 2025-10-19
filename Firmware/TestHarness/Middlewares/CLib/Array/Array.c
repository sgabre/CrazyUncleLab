#ifdef __cplusplus
#error This is not C++. Please use a C compiler.
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "Array.h"




/* Private macro  ======================================================= */
#define ID "FARR"    ///< Unique identifier for the array module.

/* Private constants =================================================== */

/* Private types ======================================================= */

/* Private structures ================================================== */

/**
* @struct Array_s
* @brief A structure representing a generic array.
*
* This structure holds metadata and a pointer to an array of elements.
* The elements are stored as raw bytes (`char*`), which facilitates pointer arithmetic.
* No dereference is executed on `Ptr`.
*/
struct Array_s
{
   size_t ItemSize;  /**< Size of each item in bytes. */
   size_t Count;     /**< Number of items stored in the array. */
   size_t Index;     /**< Current index for iteration or access. */
   char* Ptr;        /**<  Pointer to the array data. */
};

/* Private functions =================================================== */

/* Public  interfaces ================================================== */
char const* Array_SID(void)
{
   return ID;
}

char const* Array_ErrorString (eArrayError err)
{
   char const *serr = "FARR_ERR_?";

   if (err < FARR_ERR_NB)
   {
      static char const *as[] =
      {
         "OK",
         #define ITEM(n_, s_)\
         #s_,
         #include "ArrayError.h"
         #undef ITEM
      };

      serr = as[err];
   }

   return serr;
}

Array_ts* Array_Create (size_t aItemSize)
{
   Array_ts* this = malloc(sizeof * this);

   if (this != NULL)
   {
      memset(this, 0, sizeof(Array_ts) );
      
      {
         size_t const counter = 2;
         size_t const size = aItemSize * counter;
         void* const  ptr = malloc(size);

         if(ptr)
         {
            memset (ptr, 0, size);

            this->ItemSize = aItemSize;
            this->Count    = counter;
            this->Ptr      = ptr;
         }
         else
         {
            Array_Delete(this), this = NULL;
         }
      }
   }
   return this;
}

void Array_Delete (Array_ts* this)
{
   if (this != NULL)
   {
      free(this->Ptr),this->Ptr = NULL;
      free(this);
   }
   else 
   {
      FAILURE(FARR_ERR_CONTEXT);
   }
}

void Array_Init (Array_ts * this)
{
   if (this != NULL)
   {
      this->Index = 0;
   }
   else
   {
      FAILURE(FARR_ERR_CONTEXT);
   }
}

void Array_Add(Array_ts * this, void const* aData)
{
   eArrayError err = FARR_OK;
   
   if (this != NULL)
   {
      if(aData == NULL)
      {
         FAILURE(FARR_ERR_INVALID_PARAM);
      }
      else if (this->Index == this->Count)
      {
         /* grow by 2 */
         size_t const count = this->Count * 2;
#if !DBG_MEM
         size_t const size = this->ItemSize * count;
         char *const ptr = realloc(this->Ptr, size);
#else
         char *const ptr = NULL;
#endif
         if (ptr != NULL)
         {
            /* clean the end of the array */
            memset (ptr + this->ItemSize * this->Count, 0 ,this->ItemSize * this->Count);

            /* record the new information */
            this->Count = count;
            this->Ptr   = ptr;
         }
         else
         {
            FAILURE(FARR_ERR_MEMORY);
         }
      }

      /* record the data into the array */
      if (err == FARR_OK)
      {
         size_t const ndx = this->Index * this->ItemSize;
         memcpy(this->Ptr + (ndx), aData, this->ItemSize);
#if DBG
         printf ("Array_Add(%zu) ndx=%u\n", (unsigned) this->Index, (unsigned) ndx);
         //SYS_dump (this->Ptr, this->ItemSize * this->Count);
#endif
         this->Index++;
      }
   }
   else
   {
      FAILURE(FARR_ERR_CONTEXT);
   }
}

void Array_Get (Array_ts * this, size_t aIndex, void* aData)
{
   if (this != NULL)
   {
      if (aIndex < this->Count)
      {
         if (aData != NULL)
         {
            /* make a copy of the data to the user */
            size_t const ndx = aIndex * this->ItemSize;
            memcpy (aData, this->Ptr + ndx, this->ItemSize);

#if DBG
            printf (MODULE "farr_get(%u) ndx=%u" EOL, (unsigned) i, (unsigned) ndx);
            SYS_dump (aData, this->ItemSize);
#endif
         }
         else
         {
            FAILURE(FARR_ERR_ADDRESS);
         }
      }
      else
      {
         FAILURE(FARR_ERR_INDEX);
      }
   }
   else
   {
      FAILURE(FARR_ERR_CONTEXT);
   }
}

void Array_Set(Array_ts * this, size_t aIndex, void const* aData)
{
   if (this != NULL)
   {
      if (aIndex < this->ItemSize)
      {
         if (aData != NULL)
         {
            /* make a copy of the data to the user */
            size_t const ndx = aIndex * this->ItemSize;
            memcpy (this->Ptr + ndx, aData, this->ItemSize);

#if DBG
            printf (MODULE "farr_put(%u) ndx=%u" EOL, (unsigned) i, (unsigned) ndx);
            SYS_dump (this->Ptr + ndx, this->ItemSize);
#endif
         }
         else
         {
            FAILURE(FARR_ERR_ADDRESS);
         }
      }
      else
      {
         FAILURE(FARR_ERR_INDEX);
      }
   }
   else
   {
      FAILURE(FARR_ERR_CONTEXT);
   }
}

void Array_Sort (Array_ts * this, Array_Comparator * aComparator)
{
   if (this != NULL)
   {
      if (this->Index > 1)
      {
         qsort (this->Ptr, this->Index, this->ItemSize, aComparator);
      }
   }
   else
   {
      FAILURE(FARR_ERR_CONTEXT);
   }
}

size_t Array_Size(Array_ts * this)
{
   size_t nb = 0;

   if (this != NULL)
   {
      nb = this->Count;
   }
   else 
   {
      FAILURE(FARR_ERR_CONTEXT);
   }
   
   return nb;
}


size_t Array_Count(Array_ts * this)
{
   size_t nb = 0;

   if (this != NULL)
   {
      nb = this->Index;
   }
   else 
   {
      FAILURE(FARR_ERR_CONTEXT);
   }
   return nb;
}

void Array_Walkthrough(Array_ts * this, farr_lst_f * aCallBack, void* aUser)
{
   if (this != NULL)
   {
      if (aCallBack != NULL)
      {
         size_t Index;

         //#if DBG
         printf(" Traversing array[%zu] from 0 to %zu\n", this->Count, this->Index);
//#endif

         for (Index = 0; Index < this->Index; Index++)
         {
            size_t const ndx = Index * this->ItemSize;
            void const* p_data = this->Ptr + ndx;
#if DBG
            printf (".");
#endif
            if (aCallBack (p_data, aUser) != 0)
            {
#if DBG
               printf ("interrupted");
#endif
               //FAILURE(FARR_ERR_INTERRUPTED);
               break;
            }
         }
#if DBG
         printf ("\n");
#endif
      }
      else
      {
         FAILURE(FARR_ERR_CALLBACK);
      }
   }
   else
   {
      FAILURE(FARR_ERR_CONTEXT);
   }
}
