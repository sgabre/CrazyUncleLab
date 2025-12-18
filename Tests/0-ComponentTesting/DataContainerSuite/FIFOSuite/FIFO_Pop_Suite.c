#include "unity.h"
#include "FIFO.h"

void setUp(void) { }
void tearDown(void) { }

void test_FIFO_Pop_ShouldReturnElement(void)
{
    int buffer[5];
    FIFO_ts fifo_instance;
    FIFO_ts* fifo = FIFO_Create(&fifo_instance, buffer, 5, sizeof(int));

    int value = 42;
    FIFO_Push(fifo, &value);

    int popped = 0;
    bool result = FIFO_Pop(fifo, &popped);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL(value, popped);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_FIFO_Pop_ShouldReturnElement);
    return UNITY_END();
}
