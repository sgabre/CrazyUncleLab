#include <unity.h>
#include <stdint.h>
#include <FIFO.h>

#define MEM_SIZE_BUFER 32
static uint8_t MEM_BUFFER[MEM_SIZE_BUFER];
FIFO_Status_ts mStatus;
//void FIFO_Create (FIFO_ts * this, uint8_t * aBuffer, size_t const Max);
//void FIFO_Push (FIFO_ts* this, const int o);
//int FIFO_Pull (FIFO_ts * this);
//size_t FIFO_Count (FIFO_ts * this);
//bool FIFO_IsEmpty (FIFO_ts* this);
//bool FIFO_IsFull (FIFO_ts * this);
//bool FIFO_HasChanged (FIFO_ts * this);
//void FIFO_Clear (FIFO_ts * this);
//int FIFO_FillRate(FIFO_ts * this);
//int FIFO_FillRateMax (FIFO_ts * this);
//void FIFO_Status (FIFO_ts * this, FIFO_Status_ts * pSts);

void test_FIFO_Create_1(void)
{
    FIFO_ts  oFIFO =  {0};
    
    ClearFailure();
    
    // TC #x: Invalid Parameter
    FIFO_Create (&oFIFO, &MEM_BUFFER, MEM_SIZE_BUFER, &mStatus);
    //oFIFO = FIFO_Create(0);
    //TEST_ASSERT_NOT_NULL(oFIFO);
    
    FIFO_Delete (&oFIFO);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_FIFO_Create_1);

    return UNITY_END();
}