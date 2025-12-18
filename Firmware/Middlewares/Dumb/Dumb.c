#include "Dumb.h"

static Dumb_State_t state = DUMB_STATE_IDLE;

void Dumb_Init(void)
{
    Dumber_Init();
    state = DUMB_STATE_IDLE;
}

void Dumb_Handler(void)
{
    uint8_t buffer[4];
    if(Dumber_Receive(buffer, 4) == 4)
        state = DUMB_STATE_BUSY;
    else
        state = DUMB_STATE_ERROR;
}

Dumb_State_t Dumb_GetState(void)
{
    return state;
}
