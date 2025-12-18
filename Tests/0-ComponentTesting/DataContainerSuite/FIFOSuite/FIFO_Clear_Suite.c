#include "unity.h"
#include "FIFO.h"

void setUp(void) { }
void tearDown(void) { }

void test_FIFO_Clear_ShouldResetFIFO(void)
{
    int buffer[5];
    FIFO_ts fifo_instance;
    FIFO_ts* fifo = FIFO_Create(&fifo_instance, buffer, 5, sizeof(int));

    int value = 42;
    FIFO_Push(fifo, &value);
    FIFO_Clear(fifo);

    TEST_ASSERT_EQUAL(0, FIFO_Size(fifo));
    TEST_ASSERT_TRUE(FIFO_IsEmpty(fifo));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_FIFO_Clear_ShouldResetFIFO);
    return UNITY_END();
}
