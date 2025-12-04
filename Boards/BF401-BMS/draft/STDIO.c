/** @addtogroup   STDIO  
    @{
********************************************************************************
*   @name       STDIO
*   @file       STDIO.c
*
*   @brief      Wrapping stdout/stdin to selected device.
*
*   @authors    A.Bader
*   @date       21.05.2014
*
*   @copyright (C) 2014 by: MICROTRONIC AG
*                           Duennernstrasse 32
*                           CH-4702 Oensingen
*******************************************************************************/

/*== Includes ================================================================*/
#include "Target.h"

#include "Driver_USART.h"
#include "FIFO.h"
#include "IOT.h"

#include "STDIO.h"


/*== Defines =================================================================*/
#define STDIO_RX_BUFFER_SIZE  30


/*== Enumerations ============================================================*/


/*== Structures ==============================================================*/


/*== Global Variables ========================================================*/


/*== Module Constants ========================================================*/


/*== Function Prototypes =====================================================*/


/*== Module Variables ========================================================*/
extern ARM_DRIVER_USART         RTE_STDIO_USART_DRIVER;

static ARM_DRIVER_USART        *m_ptUart = NULL;
static pt_OsextEventFlagId      m_ptOsEventRx;
static pt_OsextEventFlagId      m_ptOsEventTx;

#if STDIO_INIT == 1
static pt_OsextMutexId          m_ptOsMutexRx;
static pt_OsextMutexId          m_ptOsMutexTx;

static t_Fifo                  *m_ptFifo = NULL;
#endif

static pt_OsextMessageQueueId   m_ptOsMbxId;


/*== Weak Functions to uncouple Modules ======================================*/


/*============================================================================*/
/*== Static Module Functions =================================================*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/** @brief      Event, if character received.
   
    @param[in]  event   Event from USART CMSIS Driver
    @return     None

    @author     A. Bader                                     @date 21.05.2014 */
/*----------------------------------------------------------------------------*/
static void mSTDIO_UsartEvent(uint32 ulEvent)
{
  // wait for end of receive process or for a inter character timeout
  // the system isn't fast enough if you paste something to the terminal and receive every single character one by one
  // therefore you have to wait for multiple characters, but get them after a inter character timeout
  if ((ulEvent & (ARM_USART_EVENT_RECEIVE_COMPLETE)) || (ulEvent & (ARM_USART_EVENT_RX_TIMEOUT))) 
  {
    // receive process FINISHED
    OSEXT_EventFlagsSet(m_ptOsEventRx, 0x01);
  }
  else if (ulEvent & (ARM_USART_EVENT_SEND_COMPLETE)) {
    // send process FINISHED
    OSEXT_EventFlagsSet(m_ptOsEventTx, 0x01);
  }
  else
  {
    // error handling
  }
}


/*============================================================================*/
/*== Module Task =============================================================*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/
static void tSTDIO_Thread(void *pArg)
{
  t_Msg  *ptMsg;

  while (1)
  {
    OSEXT_MessageQueueGet(m_ptOsMbxId, (uint32 *)&ptMsg, NULL, osWaitForever);

#if STDIO_INIT == 1
    if (OSEXT_MutexAcquire(m_ptOsMutexTx, osWaitForever) == osOK)
    {
      if (m_ptUart)
      {
        if (m_ptUart->Send(ptMsg->p1, ptMsg->ul1) == ARM_DRIVER_OK)
        {
          OSEXT_EventFlagsWait(m_ptOsEventTx, 1, osFlagsWaitAny, osWaitForever);
        }
      }
    }

    OSEXT_MutexRelease(m_ptOsMutexTx);
#endif

    HEAP_FREE(ptMsg->p1);
    HEAP_FREE(ptMsg);
  }
}


/*============================================================================*/
/*== Implementation of Public Functions ======================================*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/
int32 STDIO_Send(uint8 *paucBuf, int32 lLen)
{
  int32  lRet = -1;

#if STDIO_INIT == 1 && (STDIO_SOURCE & 1)
  if (m_ptUart)
  {
    if (m_ptUart->Send(paucBuf, lLen) == ARM_DRIVER_OK)
    {
      lRet = lLen;

      // in case of IRQ, poll until message is sent
      if (__get_IPSR())
			{
        while (m_ptUart->GetStatus().tx_busy);
      
			}
			else
			{
        OSEXT_EventFlagsWait(m_ptOsEventTx, 1, osFlagsWaitAny, osWaitForever);
			}
    }
  }
#endif

  return lRet;
}

/*----------------------------------------------------------------------------*/
int32 stdout_putchar(int32 lValue)
{
  int32  lRet = -1;

#if (STDIO_INIT == 1) && (STDIO_SOURCE & 1)
  if (OSEXT_MutexAcquire(m_ptOsMutexTx, osWaitForever) == osOK)
  {
    if (m_ptUart)
    {
      uint8  ucBuf = (uint8)lValue;

      if (m_ptUart->Send(&ucBuf, 1) == ARM_DRIVER_OK)
      {
        lRet = lValue;
        OSEXT_EventFlagsWait(m_ptOsEventTx, 1, osFlagsWaitAny, osWaitForever);
      }
    }

    OSEXT_MutexRelease(m_ptOsMutexTx);
  }
#endif

  return lRet;
}

/*----------------------------------------------------------------------------*/
int32 stderr_putchar(int32 lValue)
{
  return stdout_putchar(lValue);
}

/*----------------------------------------------------------------------------*/
int32 stdin_getchar (void)
{
  int32  lRet = 0;

#if STDIO_INIT == 1
  #if STDIO_SOURCE & 1
  if (OSEXT_MutexAcquire(m_ptOsMutexRx, osWaitForever) == osOK)
  {
    // check if there are remaining elements in FIFO buffer
    if (FIFO_GetCount(m_ptFifo) > 0)
      lRet = FIFO_Get(m_ptFifo);
    else if (m_ptUart)
    {
      uint8  ucBuf[STDIO_RX_BUFFER_SIZE] = {0};
      uint8  ucI;
      uint8  ucCount;

      // wait for multiple characters
      if (m_ptUart->Receive(ucBuf, STDIO_RX_BUFFER_SIZE) == ARM_DRIVER_OK)
      {
        // get some data
        OSEXT_EventFlagsWait(m_ptOsEventRx, 1, osFlagsWaitAny, osWaitForever);

        // get number of received bytes
        ucCount = (uint8)m_ptUart->GetRxCount();

        // return first byte
        lRet = ucBuf[0];

        // if more than one character is received -> add remaining elements to FIFO buffer
        if (ucCount > 1)
        {
          for (ucI = 1; ucI < ucCount; ucI++)
            FIFO_Add(m_ptFifo, ucBuf[ucI]);
        }

        // abort receive process
        m_ptUart->Control(ARM_USART_ABORT_RECEIVE, 0);
      }
    }
  }

  OSEXT_MutexRelease(m_ptOsMutexRx);
  #else
  OSEXT_Delay(osWaitForever);
  #endif
#else
  OSEXT_Delay(osWaitForever);
#endif

  return lRet;
}

/*----------------------------------------------------------------------------*/
void STDIO_AsyncPrintf(uint8 ucSourceType, char const *c_pcFmt, ...)
{
#if STDIO_INIT == 1
  bool    bSendMessage = false;
  t_Msg  *ptMsg = HEAP_MALLOC(sizeof(t_Msg));
  char   *pcTemp = ptMsg->p1 = HEAP_MALLOC(111);

  ptMsg->ul1 = 0;

  if (ucSourceType == 1)
  {
#if STDIO_SOURCE & 1
/*
    uint32 ulMilliSec = IOT_GetMilliSec();
    ptMsg->ul1 = sprintf(pcTemp, "[%d.%03d] ", ulMilliSec / 1000, ulMilliSec % 1000);
    pcTemp += ptMsg->ul1;
*/
    bSendMessage = true;
#endif
  }
  else if (ucSourceType == 2)
  {
#if STDIO_SOURCE & 2
    bSendMessage = true;
#endif
  }

  if (bSendMessage)
  {
    va_list pArgs;
    va_start(pArgs, c_pcFmt);
    ptMsg->ul1 += vsprintf(pcTemp, c_pcFmt, pArgs);
    va_end(pArgs);

    if (ptMsg->ul1 > 111)
      ASSERT_FAIL();
    if (OSEXT_MessageQueuePut(m_ptOsMbxId, (uint32 *)&ptMsg, 0, 0) != osOK)
    {
      // you can comment this line below, but then it isn't guaranteed that all messages will be displayed
      ASSERT_FAIL_MSG("Too much output to proceed!!!");
      bSendMessage = false;
    }
  }

  if (!bSendMessage)
  {
    HEAP_FREE(ptMsg->p1);
    HEAP_FREE(ptMsg);
  }
#endif
}

/*----------------------------------------------------------------------------*/
void STDIO_Init(void)
{
  // create events
  m_ptOsEventRx = OSEXT_SdioEventRxNew();
  m_ptOsEventTx = OSEXT_SdioEventTxNew();

#if STDIO_INIT == 1
  m_ptOsMutexRx = OSEXT_SdioMutexRxNew();
  m_ptOsMutexTx = OSEXT_SdioMutexTxNew();

  m_ptFifo = FIFO_Create(STDIO_RX_BUFFER_SIZE, false);
#endif

  // getchar shall end up in reading only one character (io_std_read) and
  // not pre-read to cache!
  setbuf(stdin, NULL);
  clearerr(stdin);

  // printf shall output data immediately - no internal buffering.
  setbuf(stdout, NULL);
  clearerr(stdout);

  m_ptUart = &RTE_STDIO_USART_DRIVER;

  // register event callback
  uint32_t err = m_ptUart->Initialize(mSTDIO_UsartEvent);

  // configure USART
  m_ptUart->PowerControl(ARM_POWER_FULL);
  m_ptUart->Control(ARM_USART_MODE_ASYNCHRONOUS |
                    ARM_USART_DATA_BITS_8 | 
                    ARM_USART_PARITY_NONE | 
                    ARM_USART_STOP_BITS_1 |
                    ARM_USART_FLOW_CONTROL_NONE,
                    115200);

  // enable RX and TX
  m_ptUart->Control(ARM_USART_CONTROL_RX, 1);
  m_ptUart->Control(ARM_USART_CONTROL_TX, 1);

  m_ptOsMbxId = OSEXT_MessageQueueNew(30, sizeof(t_Msg *), "STDIO");
  OSEXT_ThreadNew(tSTDIO_Thread, NULL, "STDIO", 1024, 3);
}

/// @}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
