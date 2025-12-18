#ifndef _DUMBER_H_
#define _DUMBER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "iMem.h"
#include "iSPI.h"
#include "iI2C.h"

void Dumber_Init(void);
uint32_t Dumber_Send(uint8_t* data, uint32_t len);
uint32_t Dumber_Receive(uint8_t* data, uint32_t len);
void Dumber_Config(uint32_t config);

#ifdef __cplusplus
}
#endif

#endif /* _DUMBER_H_ */