#ifdef __cplusplus
#error This source file is not C++ but rather C. Please use a C-compiler
#endif

#include <string.h>
#include <stdio.h>
#include <assert.h>
#ifdef FIFO_DYNAMIC_MEMORY
#include <stdlib.h>
#endif

#include "FIFO.h"

/* macros ============================================================== */
/* constants =========================================================== */
/* types =============================================================== */
/* structures ========================================================== */
struct FIFO_s
{
   char const* SID;
   void* Data;
   size_t ItemSize;  /**< Size of each item in bytes. */
   size_t Count;
   size_t Write;
   size_t Read;
   bool  Empty:1;
   bool Full:1;
};

/* private variables =================================================== */
/* private functions =================================================== */

//Array_ts* Array_Create(size_t aItemSize) //FIFO_DYNAMIC_MEMORY
//void Array_Create (Array_ts * this, uint8_t * aBuffer, size_t aItemSize) //FIFO_STATIC_MEMORY

void FIFO_Create(FIFO_ts * this, uint8_t * aBuffer, size_t const aBufferSize, size_t const aItemSize)
{
   if (this == NULL)
   {
      //FIFO_ERR_CONTEXT
   }
   else if(aBuffer == NULL)
   {
      //FIFO_ERR_INVALID_PARAM
   }
   else if( (aBufferSize == 0) || (aItemSize == 0) )
   
      //FIFO_ERR_INVALID_PARAM
   }
   else 
   {
      memset (this, 0, sizeof(FIFO_ts) );
      
      this->Data = aBuffer;

      if (this->Ptr)
      {
         memset (this->Data, 0, sizeof(aBuffer) );
         this->Empty = 1;

      }
      else
      {
         FIFO_Delete(this);
      }
   }
}

void FIFO_Delete(FIFO_ts* this)
{
   if (this == NULL)
   {
      //FIFO_ERR_CONTEXT
   }
   else
   {
      this->Ptr = NULL;
      
   }
}

void FIFO_Push(FIFO_ts * this, void* aItem)
{
   if (this == NULL)
   {
      //FIFO_ERR_CONTEXT
   }
   else if (aItem == NULL)
   {
      //FIFO_ERR_INVALID_PARAM
   }
   else 
   {
      //printf ("put %s : VP%u '%s'\n" ,this->sid, p_obj->vp, p_obj->buf);

      if (this->Full == false)
      {
         memcpy (this->Data[this->Write], aItem, this->ItemSize );
         
         this->Write++;
      
         if (this->Write == this->Count)
         {
            this->Write = 0;
         }
         this->Empty = 0;
         this->Full = this->Read == this->Write;
      }
      else
      {
         //FIFO_ERR_FULL
      }
   }

   return err;
}

void FIFO_Pull (FIFO_ts * this, void* aItem)
{
   if (this == NULL)
   {
      //FIFO_ERR_CONTEXT
   }
   else if (aItem == NULL)
   {
      //FIFO_ERR_INVALID_PARAM
   }
   else 
   {
      if (this->Empty == false)
      {
            memcpy (aItem, this->Data[this->Read], this->ItemSize );
            memset(this->Data[this->Read], 0, this->ItemSize );   
            
            /* gestion des index */
            this->Read++;
            if (this->Read == this->Count)
            {
                  this->Read = 0;
            }
         
            this->Full = 0;
            this->Empty = this->Read == this->Write;
            
         //printf ("get %s : VP%u '%s'\n", this->sid, p_obj->vp ,p_obj->buf);

      }
      else
      {
            //FIFO_ERR_EMPTY
      }
   }
}

bool FIFO_IsEmpty(FIFO_ts * this)
{
   if (this == NULL)
   {
      return false;
   }
   else
   {
      return this->Empty;
   }
}

bool FIFO_IsFull(FIFO_ts * this)
{
   if (this == NULL)
   {
      return false;
   }
   else
   {
      return this->Full;
   }
}