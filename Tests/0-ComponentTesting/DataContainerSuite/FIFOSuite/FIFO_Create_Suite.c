#include "unity.h"
#include "FIFO.h"

void setUp(void) { }
void tearDown(void) { }

#if MEMORY_MANAGEMENT == MEMORY_DYNAMIC
void test_FIFO_Create_ShouldAllocateMemory(void)
{
    FIFO_ts* fifo = FIFO_Create(10, sizeof(int));
    TEST_ASSERT_NOT_NULL(fifo);
    TEST_ASSERT_NOT_NULL(fifo->Buffer);
    TEST_ASSERT_EQUAL(0, fifo->Count);
    FIFO_Destroy(fifo);
}
#else
void test_FIFO_Create_ShouldInitializeStaticFIFO(void)
{
    int buffer[10];
    FIFO_ts fifo_instance;
    FIFO_ts* fifo = FIFO_Create(&fifo_instance, buffer, 10, sizeof(int));
    TEST_ASSERT_NOT_NULL(fifo);
    TEST_ASSERT_EQUAL_PTR(buffer, fifo->Buffer);
    TEST_ASSERT_EQUAL(0, fifo->Count);
}
#endif

int main(void)
{
    UNITY_BEGIN();

#if MEMORY_MANAGEMENT == MEMORY_DYNAMIC
    RUN_TEST(test_FIFO_Create_ShouldAllocateMemory);
#else
    RUN_TEST(test_FIFO_Create_ShouldInitializeStaticFIFO);
#endif

    return UNITY_END();
}
