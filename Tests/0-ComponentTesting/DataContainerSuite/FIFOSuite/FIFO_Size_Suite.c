#include "unity.h"
#include "FIFO.h"

void setUp(void) { }
void tearDown(void) { }

void test_FIFO_Size_ShouldReturnCorrectCount(void)
{
    int buffer[5];
    FIFO_ts fifo_instance;
    FIFO_ts* fifo = FIFO_Create(&fifo_instance, buffer, 5, sizeof(int));

    TEST_ASSERT_EQUAL(0, FIFO_Size(fifo));

    int value = 1;
    FIFO_Push(fifo, &value);

    TEST_ASSERT_EQUAL(1, FIFO_Size(fifo));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_FIFO_Size_ShouldReturnCorrectCount);
    return UNITY_END();
}
