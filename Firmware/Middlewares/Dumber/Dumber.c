#include "Dumber.h"

void Dumber_Init(void)
{
    iMem->Init();
    iSPI->Init();
    iI2C->Init();
}

uint32_t Dumber_Send(uint8_t* data, uint32_t len)
{
    for(uint32_t i=0;i<len;i++)
        iSPI->Transfer(data[i]);
    return len;
}

uint32_t Dumber_Receive(uint8_t* data, uint32_t len)
{
    for(uint32_t i=0;i<len;i++)
        data[i] = iSPI->Transfer(0xFF);
    return len;
}

void Dumber_Config(uint32_t config)
{
    (void)config; // dummy implementation
}
