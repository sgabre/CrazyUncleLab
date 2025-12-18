#include "unity.h"
#include "FIFO.h"

void setUp(void) { }
void tearDown(void) { }

void test_FIFO_Peek_ShouldReturnFirstElementWithoutRemoving(void)
{
    int buffer[5];
    FIFO_ts fifo_instance;
    FIFO_ts* fifo = FIFO_Create(&fifo_instance, buffer, 5, sizeof(int));

    int value = 42;
    FIFO_Push(fifo, &value);

    int peeked = 0;
    bool result = FIFO_Peek(fifo, &peeked);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL(value, peeked);
    TEST_ASSERT_EQUAL(1, FIFO_Size(fifo));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_FIFO_Peek_ShouldReturnFirstElementWithoutRemoving);
    return UNITY_END();
}
