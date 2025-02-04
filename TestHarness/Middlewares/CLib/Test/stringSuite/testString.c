#ifdef __cplusplus
#error This is not C++. Please use a C compiler.
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Array.h"

static uint8_t        mError;
#define FAILURE(aError) mError = aError;


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
         #include "Array.itm"
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
      if (this->Index == this->Count)
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
         printf (MODULE "farr_add(%u) ndx=%u" EOL, (unsigned) this->Index, (unsigned) ndx);
         SYS_dump (this->Ptr, this->ItemSize * this->Count);
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

void Array_Put (Array_ts * this, size_t aIndex, void const* aData)
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
   return nb;
}


size_t Array_Count(Array_ts * this)
{
   size_t nb = 0;

   if (this != NULL)
   {
      nb = this->Index;
   }
   return nb;
}

void Array_Traverse (Array_ts * this, farr_lst_f * pf, void* p_user)
{
   if (this != NULL)
   {
      if (pf != NULL)
      {
         size_t Index;

#if DBG
         printf(MODULE " Traversing array from 0 to %zu\n", this->Index);
#endif

         for (Index = 0; Index < this->Index; Index++)
         {
            size_t const ndx = Index * this->ItemSize;
            void const* p_data = this->Ptr + ndx;
#if DBG
            printf (".");
#endif
            if (pf (p_data, p_user) != 0)
            {
#if DBG
               printf ("interrupted");
#endif
               FAILURE(FARR_ERR_INTERRUPTED);
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


typedef struct Dummy_ts
{
   uint8_t IsSetup;
   uint8_t Data[32];
}Dummy_ts;

typedef struct WrongDummy_ts
{
   uint32_t IsSetup;
   uint8_t Data[32];
}WrongDummy_ts;

int UpComparator(void const* va, void const* vb)
{
   return 0;
}

int DownComparator(void const* va, void const* vb)
{
   return 0;
}


















void test_Array_Create(void)
{
   Array_ts*      oArray = NULL;
   Dummy_ts       aDummy = {0};
   
   // TC #x: Invalid Parameter
   oArray = Array_Create(0);
   TEST_ASSERT_NULL(oArray);
   
   // TC #x: Normal
   oArray = Array_Create( sizeof(Dummy_ts) );
   TEST_ASSERT_NOT_NULL(oArray);
   TEST_ASSERT_EQUAL(mError, FARR_OK);
   
   Array_Delete (oArray);
   TEST_ASSERT_NULL(oArray);
}

void test_Array_Delete(void)
{
   Array_ts*      oArray = NULL;
   
   // Setup
   oArray = Array_Create( sizeof(Dummy_ts) );
   TEST_ASSERT_NOT_NULL(oArray);
   
   // TC #x: Invalid Parameter
   Array_Delete (NULL);
   //????
   
   // TC #x: Normal
   Array_Delete (oArray);
   TEST_ASSERT_NULL(oArray);
}

void test_Array_Init(void)
{
   Array_ts*      oArray = NULL;
   
   // Setup
   oArray = Array_Create( sizeof(Dummy_ts) );
   TEST_ASSERT_NOT_NULL(oArray);
   
   // TC #x: Invalid Parameter
   Array_Init(NULL);
   
   // TC #x: 
   Array_Init(oArray);
   
   // Clean-up
   Array_Delete (oArray);
   TEST_ASSERT_NULL(oArray);
}

void test_Array_Add(void)
{
   Array_ts*      oArray = NULL;
   Dummy_ts       aDummy = {0};
   WrongDummy_ts  aWrongDummy = {0};  // Supposons que ce type d'entrée soit invalide
   
   // Setup
   oArray = Array_Create( sizeof(Dummy_ts) );
   TEST_ASSERT_NOT_NULL(oArray);
   
   Array_Init(oArray);

   // TC #2: Invalid Parameter #1 (context == NULL)
   Array_Add(NULL, &aDummy);
   TEST_ASSERT_EQUAL(mError, FARR_ERR_CONTEXT);
   
   // TC #3: Invalid Parameter #2 (item == NULL)
   Array_Add(oArray, NULL);
   TEST_ASSERT_EQUAL(mError, FARR_ERR_CALLBACK);
   
   // TC #4: Invalid Parameter #3 (Wrong Data)
   Array_Add(oArray, &aWrongDummy);
   TEST_ASSERT_EQUAL(mError, 0);
   
   // TC #5: Ajout normal
   Array_Add(oArray, &aDummy);
   TEST_ASSERT_EQUAL(mError, FARR_OK);
   
   // Clean-up
   Array_Delete (oArray);
   TEST_ASSERT_NULL(oArray);
}

void test_Array_Get(void)
{
   Array_ts*      oArray = NULL;
   Dummy_ts       aBackDummy = {0};
   
   // Setup
   oArray = Array_Create( sizeof(Dummy_ts) );
   TEST_ASSERT_NOT_NULL(oArray);
   
   Array_Init(oArray);
   
   // TC #x: Invalid Parameter
   Array_Get(NULL, 0, &aBackDummy);
   TEST_ASSERT_EQUAL(mError, FARR_ERR_CONTEXT);
   
   // TC #x: Invalid Parameter
   Array_Get(oArray, 25, &aBackDummy);
   TEST_ASSERT_EQUAL(mError, FARR_ERR_INVALID_PARAM);
   
   // TC #x: Invalid Parameter
   Array_Get(oArray, 25, NULL);
   TEST_ASSERT_EQUAL(mError, FARR_ERR_ADDRESS);
   
   // TC #x: Normal
   Array_Get(oArray, 0, &aBackDummy);
   TEST_ASSERT_EQUAL(mError, FARR_OK);
   
   // Clean-up
   Array_Delete (oArray);
   TEST_ASSERT_NULL(oArray);
}

void test_Array_Put(void)
{
   Array_ts*      oArray = NULL;
   Dummy_ts       aDummy = {0};
   WrongDummy_ts  aWrongDummy = {0}; 
   
   // Setup
   oArray = Array_Create( sizeof(Dummy_ts) );
   TEST_ASSERT_NOT_NULL(oArray);
   
   // TC #x: Invalid Parameter #1
   Array_Put (NULL, 1, &aDummy);
   
   // TC #x: Invalid Parameter #2
   Array_Put (oArray, 30, &aDummy);
   
   // TC #x: Invalid Parameter #3
   Array_Put (oArray, 30, NULL);
   
   // TC #x: Invalid Parameter #3
   Array_Put (oArray, 1, &aWrongDummy);
   
   // TC #x: Normal
   Array_Put (oArray, 1, &aDummy);
   
   // Clean-up
   Array_Delete (oArray);
   TEST_ASSERT_NULL(oArray);
}

void test_Array_Sort(void)
{
   Array_ts*      oArray = NULL;
   
   // Setup
   oArray = Array_Create( sizeof(Dummy_ts) );
   TEST_ASSERT_NOT_NULL(oArray);
   
   // TC #x: Invalid Parameter #1
   Array_Sort (NULL, UpComparator);
   
   // TC #x: Invalid Parameter #2
   Array_Sort (oArray, NULL);
   
   // TC #x: Normal
   Array_Sort (oArray, UpComparator);
   
   // TC #x: Normal
   Array_Sort (oArray, DownComparator);
   
   // Clean-up
   Array_Delete (oArray);
   TEST_ASSERT_NULL(oArray);
}

void test_Array_Count(void)
{
   Array_ts*      oArray = NULL;
   
   // Setup
   oArray = Array_Create( sizeof(Dummy_ts) );
   TEST_ASSERT_NOT_NULL(oArray);
   
   // TC #x: Invalid Parameter
   Size = Array_Count(NULL);
   
   // TC #x: Normal
   Size = Array_Count(oArray);
   
   // Clean-up
   Array_Delete (oArray);
   TEST_ASSERT_NULL(oArray);
}

void test_Array_Size(void)
{
   size_t Size = 0;
   Array_ts*      oArray = NULL;
   
   // Setup
   oArray = Array_Create( sizeof(Dummy_ts) );
   TEST_ASSERT_NOT_NULL(oArray);
   
   // TC #x: Invalid Parameter
   Size = Array_Size(NULL);
   
   // TC #x: Normal
   Size = Array_Size(oArray);
   
   // Clean-up
   Array_Delete (oArray);
   TEST_ASSERT_NULL(oArray);
}

void test_Array_Traverse(void)
{
   Array_ts*      oArray = NULL;
   
   // Setup
   oArray = Array_Create( sizeof(Dummy_ts) );
   TEST_ASSERT_NOT_NULL(oArray);
   
   // TC #x: Invalid Parameter
   //Array_Traverse(NULL, farr_lst_f * pf, void* p_user)
   
   
   // TC #x: Invalid Parameter
   //Array_Traverse(oArray, NULL, void* p_user)
   
   // TC #x: Invalid Parameter
   //Array_Traverse(oArray, farr_lst_f * pf, NULL)
   
   
   // TC #x: Normal
   //Array_Traverse(oArray, farr_lst_f * pf, void* p_user)
   
   // Clean-up
   Array_Delete (oArray);
   TEST_ASSERT_NULL(oArray);
}

#define RAM_SIZE (20*1024) ///< 20 ko
void test_malloc(void) 
{
   void* ptr = NULL;
   
   // TC #x: Invalid Parameter
   TEST_ASSERT_NULL(ptr);
   
   // TC #x: Boundery - Lower Limit (memorySize =  0)
   ptr = malloc(0);
   TEST_ASSERT_NOT_NULL(ptr);
   free(ptr), ptr = NULL;
   TEST_ASSERT_NULL(ptr);
   
   // TC #x: Boundery - Upper Limit (memorySize =  RAM_SIZE)
   ptr = malloc(RAM_SIZE);
   TEST_ASSERT_NOT_NULL(ptr);
   free(ptr), ptr = NULL;
   TEST_ASSERT_NULL(ptr);
   
   // TC #x: Boundery - Upper Limit (memorySize =  (RAM_SIZE+1)
   ptr = malloc(RAM_SIZE+1);
   TEST_ASSERT_NULL(ptr);
   free(ptr), ptr = NULL;
   TEST_ASSERT_NULL(ptr);
   
   // TC #x: Boundery - Lower Limit (memorySize =  -1)
   ptr = malloc(-1);
   TEST_ASSERT_NULL(ptr);
   free(ptr), ptr = NULL;
   TEST_ASSERT_NULL(ptr);
   
   // TC #x: Normal
   ptr = malloc(10); // Appel de votre fonction my_malloc
   TEST_ASSERT_NOT_NULL(ptr);
   free(ptr), ptr = NULL;
   TEST_ASSERT_NULL(ptr);
}


void test_calloc(void) 
{
   uint8_t Expected[20] = {0};
   
   size_t ItemCount = 5;
   size_t ItemSize = sizeof(int); //sizeof(int) = 4 Bytes
   
   ///Unit test: Verify calloc allocates the correct memory size
   void *ptr = calloc(ItemCount, ItemSize); // ItemCount * ItemSize = 4 * 5 = 20 Bytes
   TEST_ASSERT_NOT_NULL(ptr);  
   TEST_ASSERT_EQUAL_UINT32(ItemCount * ItemSize, malloc_asked_size(ptr));
   
   // Unit test: Verify the allocated memory is initialized to zero
   TEST_ASSERT_EQUAL_MEMORY(Expected, ptr, 20);
   
   // Unit test: Verify the allocated memory is initialized to zero
   for (size_t i = 0; i < ItemCount; i++) 
   {
      TEST_ASSERT_EQUAL_INT(0, ((int*)ptr)[i]);  // Verify each element is zero
   }
   
   // Free memory after test
   free(ptr), ptr = NULL;
   TEST_ASSERT_NULL(ptr);
}

void test_memset(void) 
{
   uint8_t Expected[10] = {0};
   uint8_t Partial[10] = {255, 255, 255, 255, 255, 0, 0, 0, 0, 0,};
   uint8_t buffer[10] = {0}; 
   
   // Set-up
   // Initialize the buffer with some non-zero values
   for (size_t i = 0; i < 10; i++) 
   {
      buffer[i] = i + 1;
   }
   
   
   // TC #x: Normal - Now use memset to set all values to 0
   memset(buffer, 0, 10);
   
   // Verify that all values in the buffer are set to 0
   TEST_ASSERT_EQUAL_MEMORY(Expected, buffer, 10);
   
   // Verify that all values in the buffer are set to 0
   for (size_t i = 0; i < 10; i++) 
   {
      TEST_ASSERT_EQUAL_UINT8(0, buffer[i] );
   }
   
   // TC #x: Normal - Use memset to set only the first 5 bytes to 255
   memset(buffer, 255, 5);
   
   TEST_ASSERT_EQUAL_MEMORY(Partial, buffer, 10);
   
   for (size_t i = 0; i < 5; i++) 
   {
      TEST_ASSERT_EQUAL_UINT8(255, buffer[i]);  // First 5 bytes should be 255
   }
   
   for (size_t i = 5; i < 10; i++) 
   {
      TEST_ASSERT_EQUAL_UINT8(i + 1, buffer[i]);  // Last 5 bytes should be unchanged (1, 2, 3, ..., 5)
   }
   
}

/*
#if __STDC_VERSION__ >= 202311L
// Code for C2x or later
char * strdup( const char * source );
void * memccpy( void * restrict destination, const void * restrict source, int ch, size_t maxSize );
char * strndup( const char * source, size_t size );
#else
// Code for older C standards
#endif
*/
// Main function to run the tests
int main(void) 
{
   UNITY_BEGIN();
   RUN_TEST(test_memset_sets_memory_correctly);
   RUN_TEST(test_memset_partial_write);
   return UNITY_END();
}