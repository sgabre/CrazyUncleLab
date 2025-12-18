#include "unity.h"
#include "FIFO.h"

void setUp(void) { }
void tearDown(void) { }

void test_FIFO_IsEmpty_ShouldReturnTrueWhenEmpty(void)
{
    int buffer[2];
    FIFO_ts fifo_instance;
    FIFO_ts* fifo = FIFO_Create(&fifo_instance, buffer, 2, sizeof(int));

    TEST_ASSERT_TRUE(FIFO_IsEmpty(fifo));

    int value = 1;
    FIFO_Push(fifo, &value);

    TEST_ASSERT_FALSE(FIFO_IsEmpty(fifo));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_FIFO_IsEmpty_ShouldReturnTrueWhenEmpty);
    return UNITY_END();
}
