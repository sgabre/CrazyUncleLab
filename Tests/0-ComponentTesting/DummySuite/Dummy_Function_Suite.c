#include <unity.h>
#include <Dummy.h>

void setUp(void)
{
    /* Optional setup */
}

void tearDown(void)
{
    /* Optional cleanup */
}

void test_Dummy_PublicFunction_should_return_sum(void)
{
    TEST_ASSERT_EQUAL_INT32(5, Dummy_PublicFunction(2, 3));
}

void test_Dummy_PublicFunction_should_handle_negative_values(void)
{
    TEST_ASSERT_EQUAL_INT32(-1, Dummy_PublicFunction(2, -3));
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_Dummy_PublicFunction_should_return_sum);
    RUN_TEST(test_Dummy_PublicFunction_should_handle_negative_values);

    return UNITY_END();
}
