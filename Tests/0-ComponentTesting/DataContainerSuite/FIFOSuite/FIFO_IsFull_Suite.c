#include "unity.h"
#include "FIFO.h"

void setUp(void) { }
void tearDown(void) { }

void test_FIFO_IsFull_ShouldReturnTrueWhenFull(void)
{
    int buffer[2];
    FIFO_ts fifo_instance;
    FIFO_ts* fifo = FIFO_Create(&fifo_instance, buffer, 2, sizeof(int));

    int a = 1, b = 2;
    FIFO_Push(fifo, &a);
    FIFO_Push(fifo, &b);

    TEST_ASSERT_TRUE(FIFO_IsFull(fifo));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_FIFO_IsFull_ShouldReturnTrueWhenFull);
    return UNITY_END();
}
