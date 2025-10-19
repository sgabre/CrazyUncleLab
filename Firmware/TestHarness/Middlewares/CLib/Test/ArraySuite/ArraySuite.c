#include <unity.h>
#include <stdint.h>
#include <Array.h>

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

static long const Data[] = {0x1234, 0x5678, 0x9ABC, 0x5978, 0x4567, 0x89AB, 0xCDEF};
static size_t DataSize =  (sizeof(Data)/sizeof(long));

int SortUp(void const* va, void const* vb)
{
    long const *pa = va;
    long const *pb = vb;
    
    return *pa - *pb;
}

int SortDown(void const* va, void const* vb)
{
    long const *pa = va;
    long const *pb = vb;
    
    return *pb - *pa;
}

static int PrintCallBack (void const *aData, void* aUser)
{
    int fin = 0;
    long const *pd = aData;
    
    if (pd != NULL)
    {
        unsigned long* pi =(unsigned long*) aUser;
        
        if (pi != NULL)
        {
            printf ("[%u] = %lXh\n", (unsigned long) *pi, (unsigned long) *pd);
            
            (*pi)++;
            
        }
        else
        {
            fin = 1;
        }
    }
    else
    {
        fin = 1;
    }
    
    return fin;
}

void test_Array_Create_1(void)
{
    Array_ts*      oArray = NULL;
    Dummy_ts       aDummy = {0};
    
    ClearFailure();
    
    // TC #x: Invalid Parameter
    oArray = Array_Create(0);
    TEST_ASSERT_NOT_NULL(oArray);
    
    Array_Delete (oArray);
    //TEST_ASSERT_NULL(oArray);
}

void test_Array_Create_2(void)
{
    Array_ts*      oArray = NULL;
    Dummy_ts       aDummy = {0};
    
    ClearFailure();
    
    // TC #x: Normal
    oArray = Array_Create( sizeof(Dummy_ts) );
    TEST_ASSERT_NOT_NULL(oArray);
    TEST_ASSERT_EQUAL(FARR_OK, LastFailure());
    
    Array_Delete (oArray);
    //TEST_ASSERT_NULL(oArray);
}

void test_Array_Delete_1(void)
{
    Array_ts*      oArray = NULL;
    
    ClearFailure();
    
    // TC #x: Invalid Parameter
    Array_Delete (oArray);
    TEST_ASSERT_NULL(oArray);
    TEST_ASSERT_EQUAL(FARR_ERR_CONTEXT, LastFailure());
}

void test_Array_Delete_2(void)
{
    Array_ts* oArray = NULL;
    
    // Setup
    ClearFailure();
    oArray = Array_Create( sizeof(Dummy_ts) );
    TEST_ASSERT_NOT_NULL(oArray);

    // TC #x: Normal
    Array_Delete (oArray);
    //TEST_ASSERT_NULL(&oArray->Ptr);
}

void test_Array_Init_1(void)
{    
    ClearFailure();
    
    // TC #x: Invalid Parameter
    Array_Init(NULL);
    TEST_ASSERT_EQUAL(FARR_ERR_CONTEXT, LastFailure());
}

void test_Array_Init_2(void)
{
    Array_ts*      oArray = NULL;
    uint8_t Expected[13] = {4, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0};
    
    // Setup
    ClearFailure();
    oArray = Array_Create( sizeof(int) );
    TEST_ASSERT_NOT_NULL(oArray);
    
    // TC #x: 
    Array_Init(oArray);
    TEST_ASSERT_EQUAL_MEMORY(Expected, oArray, 13 ); 

    // Clean-up
    Array_Delete (oArray);
    //TEST_ASSERT_NULL(oArray);
}

void test_Array_Add_1(void)
{
    Array_ts*      oArray = NULL;
    Dummy_ts       aDummy = {0};
    WrongDummy_ts  aWrongDummy = {0};  // Supposons que ce type d'entrée soit invalide
    
    // Setup
    ClearFailure();
    oArray = Array_Create( sizeof(Dummy_ts) );
    TEST_ASSERT_NOT_NULL(oArray);
    
    Array_Init(oArray);
    
    // TC #2: Invalid Parameter #1 (context == NULL)
    Array_Add(NULL, &aDummy);
    TEST_ASSERT_EQUAL(FARR_ERR_CONTEXT, LastFailure());
    

    // Clean-up
    Array_Delete (oArray);
    //TEST_ASSERT_NULL(oArray);
}

void test_Array_Add_2(void)
{
    Array_ts*      oArray = NULL;
    Dummy_ts       aDummy = {0};
    WrongDummy_ts  aWrongDummy = {0};  // Supposons que ce type d'entrée soit invalide
    
    // Setup
    ClearFailure();
    oArray = Array_Create( sizeof(Dummy_ts) );
    TEST_ASSERT_NOT_NULL(oArray);
    
    Array_Init(oArray);
    
    // TC #3: Invalid Parameter #2 (item == NULL)
    //Array_Add(oArray, NULL);
    TEST_ASSERT_EQUAL(FARR_ERR_INVALID_PARAM, LastFailure());
    
    // Clean-up
    Array_Delete (oArray);
    //TEST_ASSERT_NULL(oArray);
}

void test_Array_Add_3(void)
{
    Array_ts*      oArray = NULL;
    Dummy_ts       aDummy = {0};
    WrongDummy_ts  aWrongDummy = {0};  // Supposons que ce type d'entrée soit invalide
    
    // Setup
    ClearFailure();
    oArray = Array_Create( sizeof(Dummy_ts) );
    TEST_ASSERT_NOT_NULL(oArray);
    
    Array_Init(oArray);
    
    // TC #4: Invalid Parameter #3 (Wrong Data)
    Array_Add(oArray, &aWrongDummy);
    TEST_ASSERT_EQUAL(0, LastFailure());
    
    // Clean-up
    Array_Delete (oArray);
    //TEST_ASSERT_NULL(oArray);
}

void test_Array_Add_4(void)
{
    Array_ts*      oArray = NULL;
    Dummy_ts       aDummy = {0};
    
    // Setup
    ClearFailure();
    oArray = Array_Create( sizeof(Dummy_ts) );
    TEST_ASSERT_NOT_NULL(oArray);
    
    Array_Init(oArray);
    
    // TC #5: Ajout normal
    Array_Add(oArray, &aDummy);
    TEST_ASSERT_EQUAL(FARR_OK, LastFailure());
    TEST_ASSERT_EQUAL(1, Array_Count(oArray));
    
    // Clean-up
    Array_Delete (oArray);
    //TEST_ASSERT_NULL(oArray);
}



void test_Array_Get_1(void)
{
    Array_ts*      oArray = NULL;
    Dummy_ts       aBackDummy = {0};
    
    // Setup
    ClearFailure();
    oArray = Array_Create( sizeof(Dummy_ts) );
    TEST_ASSERT_NOT_NULL(oArray);
    
    Array_Init(oArray);
    
    // TC #x: Invalid Parameter
    Array_Get(NULL, 0, &aBackDummy);
    TEST_ASSERT_EQUAL( FARR_ERR_CONTEXT, LastFailure() );
    
    // Clean-up
    Array_Delete (oArray);
}

void test_Array_Get_2(void)
{
    Array_ts*      oArray = NULL;
    Dummy_ts       aBackDummy = {0};
    
    // Setup
    ClearFailure();
    oArray = Array_Create( sizeof(Dummy_ts) );
    TEST_ASSERT_NOT_NULL(oArray);
    
    Array_Init(oArray);
    
        
    // TC #x: Invalid Parameter
    Array_Get(oArray, 25, &aBackDummy);
    TEST_ASSERT_EQUAL(FARR_ERR_INVALID_PARAM, LastFailure());
    
    // Clean-up
    Array_Delete (oArray);
    //TEST_ASSERT_NULL(oArray);
}



void test_Array_Get_3(void)
{
    Array_ts*      oArray = NULL;
    Dummy_ts       aBackDummy = {0};
    
    // Setup
    ClearFailure();
    oArray = Array_Create( sizeof(Dummy_ts) );
    TEST_ASSERT_NOT_NULL(oArray);
    
    Array_Init(oArray);
    
    // TC #x: Normal
    Array_Get(oArray, 0, &aBackDummy);
    TEST_ASSERT_EQUAL(FARR_OK, LastFailure());
    
    // Clean-up
    Array_Delete (oArray);
    //TEST_ASSERT_NULL(oArray);
}

void test_Array_Put_1(void)
{
    Array_ts*      oArray = NULL;
    Dummy_ts       aDummy = {0};
    WrongDummy_ts  aWrongDummy = {0}; 
    
    // Setup
    ClearFailure();
    oArray = Array_Create( sizeof(Dummy_ts) );
    TEST_ASSERT_NOT_NULL(oArray);
    
    // TC #x: Invalid Parameter #1
    Array_Put (NULL, 1, &aDummy);
    TEST_ASSERT_EQUAL(FARR_ERR_CONTEXT, LastFailure());
        
    // Clean-up
    Array_Delete (oArray);
    //TEST_ASSERT_NULL(oArray);
}

void test_Array_Put_2(void)
{
    Array_ts*      oArray = NULL;
    Dummy_ts       aDummy = {0};
    WrongDummy_ts  aWrongDummy = {0}; 
    
    // Setup
    ClearFailure();
    oArray = Array_Create( sizeof(Dummy_ts) );
    TEST_ASSERT_NOT_NULL(oArray);
    

    // TC #x: Invalid Parameter #2
    Array_Put (oArray, 30, &aDummy);
    TEST_ASSERT_EQUAL(FARR_ERR_INDEX, LastFailure());
    
    // Clean-up
    Array_Delete (oArray);
    //TEST_ASSERT_NULL(oArray);
}

void test_Array_Put_3(void)
{
    Array_ts*      oArray = NULL;
    Dummy_ts       aDummy = {0};
    WrongDummy_ts  aWrongDummy = {0}; 
    
    // Setup
    ClearFailure();
    oArray = Array_Create( sizeof(Dummy_ts) );
    TEST_ASSERT_NOT_NULL(oArray);
    
    
    // TC #x: Invalid Parameter #3
    Array_Put (oArray, 30, NULL);
    TEST_ASSERT_EQUAL(FARR_ERR_ADDRESS, LastFailure());
    

    // Clean-up
    Array_Delete (oArray);
    //TEST_ASSERT_NULL(oArray);
}

void test_Array_Put_4(void)
{
    Array_ts*      oArray = NULL;
    Dummy_ts       aDummy = {0};
    WrongDummy_ts  aWrongDummy = {0}; 
    
    // Setup
    ClearFailure();
    oArray = Array_Create( sizeof(Dummy_ts) );
    TEST_ASSERT_NOT_NULL(oArray);
    
    // TC #x: Invalid Parameter #3
    Array_Put (oArray, 1, &aWrongDummy);
    TEST_ASSERT_EQUAL(FARR_OK, LastFailure());
        
    // Clean-up
    Array_Delete (oArray);
    //TEST_ASSERT_NULL(oArray);
}


void test_Array_Put_5(void)
{
    Array_ts*      oArray = NULL;
    Dummy_ts       aDummy = {0};
    WrongDummy_ts  aWrongDummy = {0}; 
    
    // Setup
    ClearFailure();
    oArray = Array_Create( sizeof(Dummy_ts) );
    TEST_ASSERT_NOT_NULL(oArray);
    

    // TC #x: Normal
    Array_Put (oArray, 1, &aDummy);
    TEST_ASSERT_EQUAL(FARR_OK, LastFailure());
    //TEST_ASSERT_EQUAL(1, Array_Count(oArray));
    TEST_ASSERT_EQUAL(2, Array_Size(oArray));
    
    // Clean-up
    Array_Delete (oArray);
    //TEST_ASSERT_NULL(oArray);
}

void test_Array_Sort_1(void)
{
    Array_ts*      oArray = NULL;
    
    // Setup
    ClearFailure();
    oArray = Array_Create( sizeof(long) );
    TEST_ASSERT_NOT_NULL(oArray);
    
    for(int i=0; i< DataSize;i++)
    {
        Array_Add(oArray, &Data[i]);
        TEST_ASSERT_EQUAL(FARR_OK, LastFailure());
        TEST_ASSERT_EQUAL(i+1, Array_Count(oArray));
    }
    

    // TC #x: Invalid Parameter #1
    Array_Sort (NULL, SortUp);
    TEST_ASSERT_EQUAL(FARR_ERR_CONTEXT, LastFailure());
    
    // Clean-up
    Array_Delete (oArray);
    //TEST_ASSERT_NULL(oArray);
}

void test_Array_Sort_2(void)
{
    Array_ts*      oArray = NULL;

    // Setup
    ClearFailure();
    oArray = Array_Create( sizeof(long) );
    TEST_ASSERT_NOT_NULL(oArray);
    
    for(int i=0; i< DataSize;i++)
    {
        Array_Add(oArray, &Data[i]);
        TEST_ASSERT_EQUAL(FARR_OK, LastFailure());
        TEST_ASSERT_EQUAL(i+1, Array_Count(oArray));
    }
    
    // TC #x: Invalid Parameter #1
    Array_Sort (oArray, NULL);
    TEST_ASSERT_EQUAL(FARR_ERR_CONTEXT, LastFailure());    

    
    // Clean-up
    Array_Delete (oArray);
    //TEST_ASSERT_NULL(oArray);
}

void test_Array_Sort_3(void)
{
    Array_ts*      oArray = NULL;

    // Setup
    ClearFailure();
    oArray = Array_Create( sizeof(long) );
    TEST_ASSERT_NOT_NULL(oArray);
    
    for(int i=0; i< DataSize;i++)
    {
        Array_Add(oArray, &Data[i]);
        TEST_ASSERT_EQUAL(FARR_OK, LastFailure());
        TEST_ASSERT_EQUAL(i+1, Array_Count(oArray));
    }
    
    // TC #x: Invalid Parameter #1
    Array_Sort (oArray, SortUp);
    TEST_ASSERT_EQUAL(FARR_OK, LastFailure());

    // Clean-up
    Array_Delete (oArray);
    //TEST_ASSERT_NULL(oArray);
}

void test_Array_Count_1(void)
{
    size_t Size = 0;
    Array_ts*      oArray = NULL;
    
    // Setup
    ClearFailure();
    oArray = Array_Create( sizeof(Dummy_ts) );
    TEST_ASSERT_NOT_NULL(oArray);
    
    // TC #x: Invalid Parameter
    Size = Array_Count(NULL);
    TEST_ASSERT_EQUAL(FARR_ERR_CONTEXT, LastFailure());
    TEST_ASSERT_EQUAL(0, Size);
    // Clean-up
    Array_Delete (oArray);
    //TEST_ASSERT_NULL(oArray);
}

void test_Array_Count_2(void)
{
    size_t Size = 0;
    Array_ts*      oArray = NULL;
    
    // Setup
    ClearFailure();
    oArray = Array_Create( sizeof(Dummy_ts) );
    TEST_ASSERT_NOT_NULL(oArray);
        
    // TC #x: Normal
    Size = Array_Count(oArray);
    TEST_ASSERT_EQUAL(FARR_OK, LastFailure());
    TEST_ASSERT_EQUAL(0, Size);
    
    // Clean-up
    Array_Delete (oArray);
    //TEST_ASSERT_NULL(oArray);
}

void test_Array_Size_1(void)
{
    size_t Size = 0;
    Array_ts*      oArray = NULL;
    
    // Setup
    ClearFailure();
    oArray = Array_Create( sizeof(Dummy_ts) );
    TEST_ASSERT_NOT_NULL(oArray);
    
    // TC #x: Invalid Parameter
    Size = Array_Size(NULL);
    TEST_ASSERT_EQUAL(FARR_ERR_CONTEXT, LastFailure());
    //TEST_ASSERT_EQUAL(0, Size);
    
    // Clean-up
    Array_Delete (oArray);
    //TEST_ASSERT_NULL(oArray);
}

void test_Array_Size_2(void)
{
    size_t Size = 0;
    Array_ts*      oArray = NULL;
    
    // Setup
    ClearFailure();
    oArray = Array_Create( sizeof(Dummy_ts) );
    TEST_ASSERT_NOT_NULL(oArray);
        
    // TC #x: Normal
    Size = Array_Size(oArray);
    TEST_ASSERT_EQUAL(FARR_OK, LastFailure());
    TEST_ASSERT_EQUAL(2, Size);
    
    // Clean-up
    Array_Delete (oArray);
    //TEST_ASSERT_NULL(oArray);
}

void test_Array_Dump_1(void)
{
    Array_ts*      oArray = NULL;

    size_t i = 0;
    
    // Setup
    ClearFailure();
    oArray = Array_Create( sizeof(long) );
    TEST_ASSERT_NOT_NULL(oArray);
    
    for(int i=0; i<DataSize;i++)
    {
        Array_Add(oArray, &Data[i]);
        TEST_ASSERT_EQUAL(FARR_OK, LastFailure());
        TEST_ASSERT_EQUAL(i+1, Array_Count(oArray));
    }
    
    i = 0;
    //Array_Dump (oArray, PrintCallBack, &i);
    TEST_ASSERT_EQUAL(FARR_OK, LastFailure());
    
    // TC #x: Invalid Parameter #1
    Array_Sort (oArray, SortUp);
    TEST_ASSERT_EQUAL(FARR_OK, LastFailure());
    
    i = 0;
    //Array_Dump (oArray, PrintCallBack, &i);
    TEST_ASSERT_EQUAL(FARR_OK, LastFailure());
    
    // Clean-up
    Array_Delete (oArray);
    
    
    
    //TEST_ASSERT_NULL(oArray);    
    // TC #x: Invalid Parameter
    //Array_Dump(NULL,PrintCallBack, &i)
    //TEST_ASSERT_EQUAL(FARR_ERR_CONTEXT, LastFailure());
    
    // TC #x: Invalid Parameter
    //Array_Dump(oArray, NULL, &i)
    //TEST_ASSERT_EQUAL(FARR_ERR_CALLBACK, LastFailure());
    
    // TC #x: Invalid Parameter
    //Array_Dump(oArray, PrintCallBack, NULL)
    //TEST_ASSERT_EQUAL(FARR_ERR_INTERRUPTED, LastFailure());

}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_Array_Create_1);
    RUN_TEST(test_Array_Create_2);
    RUN_TEST(test_Array_Delete_1);
    RUN_TEST(test_Array_Delete_2);
    RUN_TEST(test_Array_Init_1);
    RUN_TEST(test_Array_Init_2);
    RUN_TEST(test_Array_Add_1);
    //RUN_TEST(test_Array_Add_2);
    RUN_TEST(test_Array_Add_3);
    RUN_TEST(test_Array_Add_4);
    RUN_TEST(test_Array_Get_1);
    //RUN_TEST(test_Array_Get_2);
    RUN_TEST(test_Array_Get_3);
    RUN_TEST(test_Array_Put_1);
    //RUN_TEST(test_Array_Put_2);
    RUN_TEST(test_Array_Put_3);
    RUN_TEST(test_Array_Put_4);
    RUN_TEST(test_Array_Put_5);
    RUN_TEST(test_Array_Sort_1);
    //RUN_TEST(test_Array_Sort_2);
    RUN_TEST(test_Array_Sort_3);
    RUN_TEST(test_Array_Count_1);
    RUN_TEST(test_Array_Count_2);
    RUN_TEST(test_Array_Size_1);
    RUN_TEST(test_Array_Size_2);
    RUN_TEST(test_Array_Dump_1);
    
    
    

    return UNITY_END();
}