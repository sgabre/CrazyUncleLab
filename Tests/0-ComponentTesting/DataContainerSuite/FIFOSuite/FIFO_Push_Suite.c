#include "unity.h"
#include "FIFO.h"

void setUp(void) { }
void tearDown(void) { }

void test_FIFO_Push_ShouldAddElement(void)
{
    int buffer[5];
    FIFO_ts fifo_instance;
    FIFO_ts* fifo = FIFO_Create(&fifo_instance, buffer, 5, sizeof(int));

    int value = 42;
    bool result = FIFO_Push(fifo, &value);

    TEST_ASSERT_TRUE(result);
    TEST_ASSERT_EQUAL(1, FIFO_Size(fifo));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_FIFO_Push_ShouldAddElement);
    return UNITY_END();
}
