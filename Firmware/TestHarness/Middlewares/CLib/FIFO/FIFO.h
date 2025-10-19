#ifndef _FIFO_H_
#define _FIFO_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdbool.h"
#include "stdint.h"

#include "FIFO_Config.h"

typedef struct FIFO_ts FIFO_s;

void FIFO_Create(FIFO_ts * this, uint8_t * aBuffer, size_t const aBufferSize, size_t const aItemSize)

//FIFO_ts* FIFO_Create (size_t size);
//void FIFO_Delete (FIFO_ts * this);
//void FIFO_Init (FIFO_ts * this, char const *sid);
//void FIFO_Push (FIFO_ts * this, OBJ_RO data);
//void FIFO_Pull (FIFO_ts * this, OBJ_RO * p_data);
//void FIFO_IsEmpty (FIFO_ts * this);

#ifdef __cplusplus
}
#endif

#endif /* _FIFO_H_ */

