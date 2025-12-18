#include "unity.h"
#include "FIFO.h"

void setUp(void) { }
void tearDown(void) { }

#if MEMORY_MANAGEMENT == MEMORY_DYNAMIC
void test_FIFO_Destroy_ShouldFreeMemory(void)
{
    FIFO_ts* fifo = FIFO_Create(5, sizeof(int));
    TEST_ASSERT_NOT_NULL(fifo);
    FIFO_Destroy(fifo);
}
#endif

int main(void)
{
    UNITY_BEGIN();

#if MEMORY_MANAGEMENT == MEMORY_DYNAMIC
    RUN_TEST(test_FIFO_Destroy_ShouldFreeMemory);
#endif

    return UNITY_END();
}
