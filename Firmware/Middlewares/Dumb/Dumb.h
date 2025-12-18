#ifndef _DUMMY_H_
#define _DUMMY_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "Dumber.h"

typedef enum {
    DUMB_STATE_IDLE,
    DUMB_STATE_BUSY,
    DUMB_STATE_ERROR
} Dumb_State_t;

void Dumb_Init(void);
void Dumb_Handler(void);
Dumb_State_t Dumb_GetState(void);


#ifdef __cplusplus
}
#endif

#endif /* _DUMMY_H_ */