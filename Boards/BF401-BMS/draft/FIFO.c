/** @addtogroup   FIFO  
    @{
********************************************************************************
*   @name       FIFO
*   @file       FIFO.c
*
*   @brief      Create a FIFO of 4 bytes user data
*
*   @authors    S.Schuepbach
*   @date       02.05.2017
*
*   @copyright (C) 2014 by: MICROTRONIC AG
*                           Duennernstrasse 32
*                           CH-4702 Oensingen
*******************************************************************************/

/*== Includes ================================================================*/
#include "Target.h"
#include "OSEXT.h"
#include "ASSERT.h"
#include "HEAP.h"

#include "FIFO.h"

/*== Defines =================================================================*/

/*== Enumerations ============================================================*/

/*== Structures ==============================================================*/

/*== Global Variables ========================================================*/

/*== Module Constants ========================================================*/

/*== Function Prototypes =====================================================*/

/*== Module Variables ========================================================*/

/*== Weak Functions to uncouple Modules ======================================*/

/*============================================================================*/
/*== Internal Functions ======================================================*/
/*============================================================================*/

/*============================================================================*/
/*== Static Module Functions =================================================*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/** @brief      update write or read pointer

    @param[in]          unSize     fifo size
    @param[in, out]     punValue   pointer to update
    @return     None

    @author     S.Schuepbach                                @date 02.05.2017  */
/*----------------------------------------------------------------------------*/
static void mFIFO_IncrementPointer(uint16 unSize, uint16 *punValue)
{
  (*punValue)++;
  if (*punValue >= unSize)
    *punValue = 0;
}

/*============================================================================*/
/*== Implementation of Public Functions ======================================*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/
t_Fifo *FIFO_Create(uint16 unSize, bool bMutexProtection)
{
  t_Fifo *ptFifo = HEAP_MALLOC(sizeof(t_Fifo));
  ptFifo->paulBuf = HEAP_MALLOC(unSize * sizeof(uint32));
  ptFifo->unBufSize = unSize;
  ptFifo->unReadPos = 0;
  ptFifo->unWritePos = 0;
  ptFifo->ptMutexId = 0;
  if (bMutexProtection)
  {
    ptFifo->ptMutexId = OSEXT_FifoMutexNew();
    ASSERT(ptFifo->ptMutexId);
  }

  return ptFifo;
}

/*----------------------------------------------------------------------------*/
void FIFO_Delete(t_Fifo *ptFifo)
{
  HEAP_FREE(ptFifo->paulBuf);
  HEAP_FREE(ptFifo);
}

/*----------------------------------------------------------------------------*/
uint32 FIFO_Get(t_Fifo *ptFifo)
{
  uint32 ulResult = 0;
  if (ptFifo->ptMutexId == 0 || OSEXT_MutexAcquire(ptFifo->ptMutexId, osWaitForever) == osOK)
  {

    __disable_irq();
    ASSERT(ptFifo->unReadPos != ptFifo->unWritePos);
    ulResult = ptFifo->paulBuf[ptFifo->unReadPos];
    mFIFO_IncrementPointer(ptFifo->unBufSize, &ptFifo->unReadPos);
    __enable_irq();

    if (ptFifo->ptMutexId)
      OSEXT_MutexRelease(ptFifo->ptMutexId);
  }
  else
    ASSERT_FAIL();
  return ulResult;
}

/*----------------------------------------------------------------------------*/
uint16 FIFO_Add(t_Fifo *ptFifo, uint32 ulValue)
{
  uint16 unResult = 0;
  if (ptFifo->ptMutexId == 0 || OSEXT_MutexAcquire(ptFifo->ptMutexId, osWaitForever) == osOK)
  {
    __disable_irq();
    ptFifo->paulBuf[ptFifo->unWritePos] = ulValue;
    mFIFO_IncrementPointer(ptFifo->unBufSize, &ptFifo->unWritePos);
    ASSERT(ptFifo->unReadPos != ptFifo->unWritePos);
    unResult = FIFO_GetSpace(ptFifo);
    __enable_irq();
    if (ptFifo->ptMutexId)
      OSEXT_MutexRelease(ptFifo->ptMutexId);
  }
  else
    ASSERT_FAIL();
  return unResult;
}

/*----------------------------------------------------------------------------*/
uint16 FIFO_GetSpace(t_Fifo *ptFifo)
{
  uint16 unResult = 0;
  if (ptFifo->ptMutexId == 0 || OSEXT_MutexAcquire(ptFifo->ptMutexId, osWaitForever) == osOK)
  {
    unResult = ptFifo->unBufSize - FIFO_GetCount(ptFifo);
    if (ptFifo->ptMutexId)
      OSEXT_MutexRelease(ptFifo->ptMutexId);
  }
  else
    ASSERT_FAIL();
  return unResult;
}

/*----------------------------------------------------------------------------*/
uint16 FIFO_GetCount(t_Fifo *ptFifo)
{
  int32 lCount = 0;
  if ((ptFifo->ptMutexId == 0) || (OSEXT_MutexAcquire(ptFifo->ptMutexId, osWaitForever) == osOK))
  {
    lCount = ptFifo->unWritePos - ptFifo->unReadPos;
    if (lCount < 0)
      lCount += ptFifo->unBufSize;
    if (ptFifo->ptMutexId)
      OSEXT_MutexRelease(ptFifo->ptMutexId);
  }
  else
    ASSERT_FAIL();
  return (uint16) lCount;
}

/// @}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

