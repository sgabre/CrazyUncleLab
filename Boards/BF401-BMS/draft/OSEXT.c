/** @addtogroup   OSEXT  
    @{
********************************************************************************
*   @name       OSEXT
*   @file       OSEXT.c
*
*   @brief      Extensions to RTX.
*               Stackwatcher, OS Watcher, wrapper to own functions...
*
*   @authors    A.Bader
*   @date       05.06.2014
*
*   @copyright (C) 2014 by: MICROTRONIC AG
*                           Duennernstrasse 32
*                           CH-4702 Oensingen
*******************************************************************************/

/*== Includes ================================================================*/
#include "Target.h"

#include "LIST.h"
#include "IOT.h"
//#include "stm32f4xx_hal.h"
#include "HEAP.h"
#include "rtx_os.h"

#include "OSEXT.h"


/*== Defines =================================================================*/
#define OSEXT_MAX_EVT_THREAD_LIST_ENTRIES       5

#define OSEXT_MAX_MESSAGE_QUEUE_LIST_ENTRIES   10
#define OSEXT_MAX_THREAD_LIST_ENTRIES          50
#define OSEXT_MAX_MUTEX_LIST_ENTRIES           10
#define OSEXT_MAX_EVENT_FLAG_LIST_ENTRIES      10


/*== Enumerations ============================================================*/
typedef enum
{
  OSEXT_EVT_THREAD_BLOCKED,
  OSEXT_EVT_THREAD_UNBLOCKED,
} e_EvtRecord;


/*== Structures ==============================================================*/
typedef struct
{
  const char  *pcName;
  const char  *pcWaitObjectName;
  char        *pcDWaitObjectType;
  uint32       ulDelay;
} t_OsextWaitObject;

// Task Stack Information
typedef struct
{
  char    *pcTaskName;
  uint32   ulStackSize;
  void    *paucMemory;
} t_StackInfo;


/*== Global Variables ========================================================*/

// CCM Stack for USB Task [No DMA Usage]
uint8 m_arBufUSB[4096]   __attribute__ ((section(".ccm"))) = { 0 };

// CCM Stack for MDB Master & Peripheral Tasks [No DMA Usage]
uint8 m_arBufMDBM    [2048] __attribute__ ((section(".ccm"))) = { 0 };
uint8 m_arBufMDBCG   [1024] __attribute__ ((section(".ccm"))) = { 0 };
uint8 m_arBufMDBBV   [1024] __attribute__ ((section(".ccm"))) = { 0 };
uint8 m_arBufRFMSEL  [1024] __attribute__ ((section(".ccm"))) = { 0 };
uint8 m_arBufRFMPRICE[1024] __attribute__ ((section(".ccm"))) = { 0 };

// Array of Task Stack Information [see OSEXT_ThreadCCM()]
t_StackInfo m_arStackInfo[] = {
  { "USB_Thread",      sizeof(m_arBufUSB),      m_arBufUSB      },
  { "MDBM_Thread",     sizeof(m_arBufMDBM),     m_arBufMDBM     },
  { "MDBCG_Thread",    sizeof(m_arBufMDBCG),    m_arBufMDBCG    },
  { "MDBBV_Thread",    sizeof(m_arBufMDBBV),    m_arBufMDBBV    },
  { "RFMSEL_Thread",   sizeof(m_arBufRFMSEL),   m_arBufRFMSEL   },
  { "RFMPRICE_Thread", sizeof(m_arBufRFMPRICE), m_arBufRFMPRICE }
};


/*== Module Constants ========================================================*/
#if OSEXT_DEBUG_OPTIONS & 1
  static const char mc_acRunning[] = "Running";
#endif


/*== Function Prototypes =====================================================*/
#if SHELL_OSEXT > 0
  static char *mOSEXT_ShellGetGenericStateName(uint8 ucState);
  static void mOSEXT_InitShell(void);
  static void mOSEXT_SendEvt(e_EvtRecord eEvtRecord, pt_OsextThreadId ptThreadId, uint8 ucIndex, uint32 ulTimeout);

  pt_OsextThreadId         m_ptThreadEvtRecord = NULL;
  pt_OsextMessageQueueId   m_ptMessageQueueEvtRecord = NULL;
  t_List                  *m_ptListEvtThreadIds = NULL;
#endif


/*== Module Variables ========================================================*/
#if OSEXT_DEBUG_OPTIONS & 1
  static char m_acWaitStringBuf[2048];
#endif

static t_List *m_ptMessageQueue = NULL;

#ifndef RTE_CMSIS_RTOS2
  static t_OsextV1Thread *m_ptThreadList = NULL;
#endif





/*== Function Prototypes =====================================================*/
#if SHELL_OSEXT > 0
  static void mOSEXT_InitShell(void);
#endif


/*============================================================================*/
/*== Static Module Functions =================================================*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/
#if OSEXT_DEBUG_OPTIONS & 1
  static void mOSEXT_CheckInit(void)
  {
    static bool s_bInit = false;

    if (!s_bInit)
    {
      memset(m_acWaitStringBuf, 0, sizeof(m_acWaitStringBuf));
      s_bInit = true;
    }
  }
#endif

/*----------------------------------------------------------------------------*/



/*============================================================================*/
/*== Implementation of Public Functions ======================================*/
/*============================================================================*/

#ifdef RTE_CMSIS_RTOS2
/*----------------------------------------------------------------------------*/
pt_OsextMessageQueueId OSEXT_MessageQueueNew(uint32 ulMessageCount, uint32 ulMessageSize, const char *c_pcName)
{
  const osMessageQueueAttr_t tAttr = {
    .name = c_pcName
  };

  pt_OsextMessageQueueId ptId = osMessageQueueNew(ulMessageCount, ulMessageSize, &tAttr);
  {
    uint32 ulValue = (uint32)ptId;

    if (!m_ptMessageQueue)
       m_ptMessageQueue = LIST_Create(OSEXT_MAX_MESSAGE_QUEUE_LIST_ENTRIES);

    LIST_AddItem(m_ptMessageQueue, (uint8 *)&ulValue, 4);
  }

  return ptId;
}

/*----------------------------------------------------------------------------*/
pt_OsextMutexId OSEXT_MutexNew(const char *c_pcName)
{
  const osMutexAttr_t tAttr = {
    .name = c_pcName
  };

  return osMutexNew(&tAttr);
}

/*----------------------------------------------------------------------------*/
pt_OsextThreadId OSEXT_ThreadNew(pf_OsextThreadFunction pfFunc, void *pArg, const char *c_pcName, uint32 ulStackSize, uint8 ucPriority)
{
  // Create Task Attributes & Task [NULL Pointer for Memory => Allocated by OS Function]
  const osThreadAttr_t tAttr = {
    .name = c_pcName,
    .stack_size = ulStackSize,
    .stack_mem = NULL,
    .priority = (osPriority_t)ucPriority
  };

  return osThreadNew(pfFunc, pArg, &tAttr);
}

/*----------------------------------------------------------------------------*/
pt_OsextThreadId OSEXT_ThreadCCM(pf_OsextThreadFunction pfFunc, void *pArg, const char *c_pcName, uint8 ucPriority)
{
  pt_OsextThreadId  ptThreadId = NULL;
  uint8             ucIndex = 0;
  uint8             ucCount = sizeof(m_arStackInfo) / sizeof(t_StackInfo);

  // Get Stack Info by Task Name
  for (ucIndex = 0; ucIndex < ucCount; ucIndex++)
  {
    t_StackInfo  tInfo = m_arStackInfo[ucIndex];

    if (!strcmp(c_pcName, tInfo.pcTaskName))
    {
      // Found: Create Task Attributes & Task
      const osThreadAttr_t tAttr = {
        .name = c_pcName,
        .stack_size = tInfo.ulStackSize,
        .stack_mem = tInfo.paucMemory,
        .priority = (osPriority_t)ucPriority
      };

      ptThreadId = osThreadNew(pfFunc, pArg, &tAttr);
      break;
    }
  }

  ASSERT_MSG(ptThreadId != NULL, "OSEXT_ThreadCCM(): No Thread created!");

  return ptThreadId;
}

/*----------------------------------------------------------------------------*/
uint8 counter = 0;
pt_OsextTimerId OSEXT_TimerNew(pf_OsextTimerFunction pfFunc, e_OsextTimerType eType, void *pArg, const char *c_pcName)
{
  if (!strcmp(c_pcName, "HSM_Timer"))
  {    
    counter++;
  }

  const osTimerAttr_t tAttr = {
    .name = c_pcName
  };

  return osTimerNew(pfFunc, eType, pArg, &tAttr);
}

/*----------------------------------------------------------------------------*/
pt_OsextEventFlagId OSEXT_EventFlagsNew(const char *c_pcName)
{
  const osEventFlagsAttr_t tAttr = {
    .name = c_pcName
  };

  return osEventFlagsNew(&tAttr);
}

/*----------------------------------------------------------------------------*/
pt_OsextSemaphoreId OSEXT_SemaphoreNew(uint32 ulMaxCount, uint32 ulInitialCount, const char *c_pcName)
{
  const osSemaphoreAttr_t tAttr = {
    .name = c_pcName
  };

  return osSemaphoreNew(ulMaxCount, ulInitialCount, &tAttr);
}

/*----------------------------------------------------------------------------*/
pt_OsextMemoryPoolId OSEXT_MemoryPoolNew(uint32 ulBlockCount, uint32 ulBlockSize, const char *c_pcName)
{
  const osMemoryPoolAttr_t tAttr = {
    .name = c_pcName
    /// @todo Add more members
  };

  return osMemoryPoolNew(ulBlockCount, ulBlockSize, &tAttr);
}
#else
/*----------------------------------------------------------------------------*/
pt_OsextMessageQueueId OSEXT_MessageQueueNew(uint32 ulMessageCount, uint32 ulMessageSize, const char *c_pcName)
{
  pt_OsextMessageQueueId ptMsgQueue = HEAP_MALLOC(sizeof(t_OsextV1MessageQueue));

  ptMsgQueue->pcName = (char*)c_pcName;
  ptMsgQueue->ulCount = ulMessageCount;
  ptMsgQueue->ulSize = ulMessageSize;
  ptMsgQueue->ulPendingMessages = 0;
  ptMsgQueue->osMessageQDef.queue_sz = ulMessageCount;
  ptMsgQueue->osMessageQDef.pool = HEAP_MALLOC(8 * (ulMessageCount*ulMessageSize + 4));
  memset(ptMsgQueue->osMessageQDef.pool, 0, 8 * (ulMessageCount*ulMessageSize + 4));
  ptMsgQueue->osMsgQueue = osMessageCreate(&ptMsgQueue->osMessageQDef, NULL);

  return ptMsgQueue;
}

/*----------------------------------------------------------------------------*/
pt_OsextMutexId OSEXT_MutexNew(const char *c_pcName)
{
  pt_OsextMutexId ptMutex = HEAP_MALLOC(sizeof(t_OsextV1Mutex));
  
  ptMutex->ptOwner = NULL;
  ptMutex->pcName = (char *)c_pcName;
  ptMutex->osMutexDef.mutex = HEAP_MALLOC(8 * 4);
  ptMutex->osMutex = osMutexCreate(&ptMutex->osMutexDef);

  return ptMutex;
}

/*----------------------------------------------------------------------------*/
pt_OsextThreadId OSEXT_ThreadNew(pf_OsextThreadFunction func, void *arg, const char *c_pcName, uint32 ulStackSize, uint8 ucPriority)
{
  uint8 ucI = 0;

  if (m_ptThreadList == NULL)
  {
    m_ptThreadList = HEAP_MALLOC(sizeof(t_OsextV1Thread) * OSEXT_MAX_THREAD_LIST_ENTRIES);
    memset(m_ptThreadList, 0, sizeof(t_OsextV1Thread) * OSEXT_MAX_THREAD_LIST_ENTRIES);
  }

  for (ucI = 0; (m_ptThreadList[ucI].ptID != 0) && (ucI < OSEXT_MAX_EVT_THREAD_LIST_ENTRIES); ucI++)
    ;
    
  ASSERT(ucI < OSEXT_MAX_EVT_THREAD_LIST_ENTRIES);

  m_ptThreadList[ucI].osThreadDef.stacksize = ulStackSize;
  m_ptThreadList[ucI].osThreadDef.tpriority = (osPriority) (int8)(ucPriority / 9) - 3;
  m_ptThreadList[ucI].osThreadDef.instances = 1;
  m_ptThreadList[ucI].osThreadDef.pthread = func;
  m_ptThreadList[ucI].pcName = (char*)c_pcName;
  m_ptThreadList[ucI].ptID = osThreadCreate(&m_ptThreadList[ucI].osThreadDef, arg);

  return &m_ptThreadList[ucI];
}

/*----------------------------------------------------------------------------*/
pt_OsextTimerId OSEXT_TimerNew(pf_OsextTimerFunction pfFunc, e_OsextTimerType eType, void *pArg, const char *c_pcName)
{
  pt_OsextTimerId ptOsTimer = HEAP_MALLOC(sizeof(t_OsextV1Timer));
  printf("Timer: %s \r\n" , c_pcName);
  ptOsTimer->pcName = (char*)c_pcName;
  ptOsTimer->osTimerDef.ptimer = pfFunc;
  ptOsTimer->osTimerDef.timer =  ptOsTimer->aulBuffer;
  ptOsTimer->ptID = osTimerCreate(&ptOsTimer->osTimerDef, (os_timer_type)eType, pArg);

  return ptOsTimer;
}

/*----------------------------------------------------------------------------*/
pt_OsextEventFlagId OSEXT_EventFlagsNew(const char *c_pcName)
{
  pt_OsextEventFlagId ptEventFlag = HEAP_MALLOC(sizeof(t_OsextV1EventFlag));

  ptEventFlag->ptMsgQueue = OSEXT_MessageQueueNew(20, sizeof(uint32), c_pcName);
  ptEventFlag->ulFlags = 0;

  return ptEventFlag;
}

/*----------------------------------------------------------------------------*/
pt_OsextSemaphoreId OSEXT_SemaphoreNew(uint32 ulMaxCount, uint32 ulInitialCount, const char *c_pcName)
{
  
}

/*----------------------------------------------------------------------------*/
pt_OsextMemoryPoolId OSEXT_MemoryPoolNew(uint32 ulBlockCount, uint32 ulBlockSize, const char *c_pcName)
{
  
}
#endif


/*----------------------------------------------------------------------------*/
// -- Functions, which are wrapped directly, without modifications
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
e_OsextStatus OSEXT_KernelInitialize(void)
{
  return osKernelInitialize();
}

/*----------------------------------------------------------------------------*/
e_OsextStatus OSEXT_KernelGetInfo(t_OsextVersion *ptVersion, char *pcBuf, uint32 ulSize)
{
#ifdef RTE_CMSIS_RTOS2
  return osKernelGetInfo(ptVersion, pcBuf, ulSize);
#else
  return osErrorOS;
#endif
}

/*----------------------------------------------------------------------------*/
e_OsextKernelState OSEXT_KernelGetState(void)
{
#ifdef RTE_CMSIS_RTOS2
  return osKernelGetState();   // Allowed in ISR [Interrupt Service Routine] starting from RTOS 2.1.2 [CMSIS 5.2.0]
#else
  return osErrorOS;
#endif
}

/*----------------------------------------------------------------------------*/
uint8 OSEXT_KernelIsRunning(void)
{
#ifdef RTE_CMSIS_RTOS2
  return OSEXT_KernelGetState() == osKernelRunning;
#else
  return osKernelRunning();
#endif
}

/*----------------------------------------------------------------------------*/
e_OsextStatus OSEXT_KernelStart(void)
{
  return osKernelStart();
}

/*----------------------------------------------------------------------------*/
int32 OSEXT_KernelLock(void)
{
#ifdef RTE_CMSIS_RTOS2
  return osKernelLock();
#else
  return osErrorOS;
#endif
}

/*----------------------------------------------------------------------------*/
int32 OSEXT_KernelUnlock(void)
{
#ifdef RTE_CMSIS_RTOS2
  return osKernelUnlock();
#else
  return osErrorOS;
#endif
}

/*----------------------------------------------------------------------------*/
int32 OSEXT_KernelRestoreLock(int32 lLock)
{
#ifdef RTE_CMSIS_RTOS2
  return osKernelRestoreLock (lLock);
#else
  return osErrorOS;
#endif
}

/*----------------------------------------------------------------------------*/
uint32 OSEXT_KernelSuspend(void)
{
#ifdef RTE_CMSIS_RTOS2
  return osKernelSuspend();
#else
  return osErrorOS;
#endif
}

/*----------------------------------------------------------------------------*/
void OSEXT_KernelResume(uint32 ulSleepTicks)
{
#ifdef RTE_CMSIS_RTOS2
  osKernelResume(ulSleepTicks);
#else

#endif
}

/*----------------------------------------------------------------------------*/
uint64 OSEXT_KernelGetTickCount(void)
{
#ifdef RTE_CMSIS_RTOS2
  return osKernelGetTickCount();   // Allowed in ISR [Interrupt Service Routine] starting from RTOS 2.1.1 [CMSIS 5.1.1]
#else
  return osKernelSysTick();
#endif
}

/*----------------------------------------------------------------------------*/
uint32 OSEXT_KernelGetTickFreq(void)
{
#ifdef RTE_CMSIS_RTOS2
  return osKernelGetTickFreq();   // Allowed in ISR [Interrupt Service Routine] starting from RTOS 2.1.1 [CMSIS 5.1.1]
#else
  return osKernelSysTickFrequency;
#endif
}

/*----------------------------------------------------------------------------*/
uint32 OSEXT_KernelGetSysTimerCount(void)
{
#ifdef RTE_CMSIS_RTOS2
  return osKernelGetSysTimerCount();   // Allowed in ISR [Interrupt Service Routine] starting from RTOS 2.1.2 [CMSIS 5.2.0]
#else
  return osKernelSysTick();
#endif
}

/*----------------------------------------------------------------------------*/
uint32 OSEXT_KernelGetSysTimerFreq(void)
{
#ifdef RTE_CMSIS_RTOS2
  return osKernelGetSysTimerFreq();   // Allowed in ISR [Interrupt Service Routine] starting from RTOS 2.1.2 [CMSIS 5.2.0]
#else
  return osKernelSysTickFrequency;
#endif
}


/*== Thread Management Functions =============================================*/

/*----------------------------------------------------------------------------*/
const char *OSEXT_ThreadGetName(pt_OsextThreadId ptThreadId)
{
#ifdef RTE_CMSIS_RTOS2
  return osThreadGetName(ptThreadId);
#else
  return ptThreadId->pcName;
#endif
}

/*----------------------------------------------------------------------------*/
pt_OsextThreadId OSEXT_ThreadGetId(void)
{
#ifdef RTE_CMSIS_RTOS2
  return osThreadGetId();
#else
  osThreadId osId = osThreadGetId();
  uint8 ucI = 0;

  for (ucI = 0; (m_ptThreadList[ucI].ptID != osId) && (ucI < OSEXT_MAX_EVT_THREAD_LIST_ENTRIES); ucI++)
    ;

  ASSERT(ucI < OSEXT_MAX_EVT_THREAD_LIST_ENTRIES);
  
  return &m_ptThreadList[ucI];
#endif
}

/*----------------------------------------------------------------------------*/
e_OsextThreadState OSEXT_ThreadGetState(pt_OsextThreadId ptThreadId)
{
#ifdef RTE_CMSIS_RTOS2
  return osThreadGetState(ptThreadId);
#else
  return osErrorOS;
#endif
}

/*----------------------------------------------------------------------------*/
uint32 OSEXT_ThreadGetStackSize(pt_OsextThreadId ptThreadId)
{
#ifdef RTE_CMSIS_RTOS2
  return osThreadGetStackSize(ptThreadId);
#else
  return ptThreadId->osThreadDef.stacksize;
#endif
}

/*----------------------------------------------------------------------------*/
uint32 OSEXT_ThreadGetStackSpace(pt_OsextThreadId ptThreadId)
{
#ifdef RTE_CMSIS_RTOS2
  return osThreadGetStackSpace(ptThreadId);
#else
  return 0;
#endif
}

/*----------------------------------------------------------------------------*/
e_OsextStatus OSEXT_ThreadSetPriority(pt_OsextThreadId ptThreadId, e_OsextPriority ePriority)
{
#ifdef RTE_CMSIS_RTOS2
  return osThreadSetPriority(ptThreadId, ePriority);
#else
  return osThreadSetPriority(ptThreadId->ptID, ePriority);
#endif
}

/*----------------------------------------------------------------------------*/
e_OsextPriority OSEXT_ThreadGetPriority(pt_OsextThreadId ptThreadId)
{
#ifdef RTE_CMSIS_RTOS2
  return osThreadGetPriority(ptThreadId);
#else
  return osThreadGetPriority(ptThreadId->ptID);
#endif
}

/*----------------------------------------------------------------------------*/
e_OsextStatus OSEXT_ThreadYield(void)
{
  return osThreadYield();
}

/*----------------------------------------------------------------------------*/
e_OsextStatus OSEXT_ThreadSuspend(pt_OsextThreadId ptThreadId)
{
#ifdef RTE_CMSIS_RTOS2
  return osThreadSuspend(ptThreadId);
#else
  return osErrorOS;
#endif
}

/*----------------------------------------------------------------------------*/
e_OsextStatus OSEXT_ThreadResume(pt_OsextThreadId ptThreadId)
{
#ifdef RTE_CMSIS_RTOS2
  return osThreadResume(ptThreadId);
#else
  return osErrorOS;
#endif
}

/*----------------------------------------------------------------------------*/
e_OsextStatus OSEXT_ThreadDetach(pt_OsextThreadId ptThreadId)
{
#ifdef RTE_CMSIS_RTOS2
  return osThreadDetach(ptThreadId);
#else
  return osErrorOS;
#endif
}

/*----------------------------------------------------------------------------*/
e_OsextStatus OSEXT_ThreadJoin(pt_OsextThreadId ptThreadId)
{
#ifdef RTE_CMSIS_RTOS2
  return osThreadJoin(ptThreadId);
#else
  return osErrorOS;
#endif
}

/*----------------------------------------------------------------------------*/
void OSEXT_ThreadExit(void)
{
#ifdef RTE_CMSIS_RTOS2
  osThreadExit();
#else
  ASSERT_FAIL();
#endif
}

/*----------------------------------------------------------------------------*/
e_OsextStatus OSEXT_ThreadTerminate(pt_OsextThreadId ptThreadId)
{
#ifdef RTE_CMSIS_RTOS2
  return osThreadTerminate(ptThreadId);
#else
  osThreadId osID = ptThreadId->ptID;
  ptThreadId->ptID = 0;

  return osThreadTerminate(osID);
#endif
}

/*----------------------------------------------------------------------------*/
uint32 OSEXT_ThreadGetCount(void)
{
#ifdef RTE_CMSIS_RTOS2
  return osThreadGetCount();
#else
  uint32  ulCount = 0;
  uint8   ucI = 0;

  for (ucI = 0; ucI < OSEXT_MAX_EVT_THREAD_LIST_ENTRIES; ucI++)
  {
    if (m_ptThreadList[ucI].ptID != 0)
      ulCount++;
  }

  return ulCount;
#endif
}

/*----------------------------------------------------------------------------*/
uint32 OSEXT_ThreadEnumerate(pt_OsextThreadId *pptThreadId, uint32 ulNumItems)
{
#ifdef RTE_CMSIS_RTOS2
  return osThreadEnumerate(pptThreadId, ulNumItems);
#else
  uint8  ucPos = 0;
  uint8  ucI = 0;

  for (ucI = 0; (ucPos < ulNumItems) && (ucI < OSEXT_MAX_EVT_THREAD_LIST_ENTRIES); ucI++)
  {
    if (m_ptThreadList[ucI].ptID != 0)
      memcpy(&pptThreadId[ucPos++], &m_ptThreadList[ucI], sizeof(t_OsextV1Thread));
  }

  return osOK;
#endif
}


/*== Thread Flags Functions ==================================================*/

/*----------------------------------------------------------------------------*/
uint32 OSEXT_ThreadFlagsSet(pt_OsextThreadId ptThreadId, uint32 ulFlags)
{
#ifdef RTE_CMSIS_RTOS2
  return osThreadFlagsSet(ptThreadId, ulFlags);
#else
  return 0;
#endif
}

/*----------------------------------------------------------------------------*/
uint32 OSEXT_ThreadFlagsClear(uint32 ulFlags)
{
#ifdef RTE_CMSIS_RTOS2
  return osThreadFlagsClear(ulFlags);
#else
  return 0;
#endif
}

/*----------------------------------------------------------------------------*/
uint32 OSEXT_ThreadFlagsGet(void)
{
#ifdef RTE_CMSIS_RTOS2
  return osThreadFlagsGet();
#else
  return 0;
#endif
}

/*----------------------------------------------------------------------------*/
uint32 OSEXT_ThreadFlagsWait(uint32 ulFlags, uint32 ulOptions, uint32 ulTimeout)
{
#ifdef RTE_CMSIS_RTOS2
  return osThreadFlagsWait(ulFlags, ulOptions, ulTimeout);
#else
  return 0;
#endif
}


/*== Generic Wait Functions ==================================================*/

/*----------------------------------------------------------------------------*/
e_OsextStatus OSEXT_Delay(uint32 ulTicks)
{
  return osDelay(ulTicks);
}

/*----------------------------------------------------------------------------*/
e_OsextStatus OSEXT_DelayUntil(uint64 uqTicks)
{
#ifdef RTE_CMSIS_RTOS2
  return osDelayUntil(uqTicks);
#else
  return osErrorOS;
#endif
}


/*== Timer Management Functions ==============================================*/

/*----------------------------------------------------------------------------*/
const char *OSEXT_TimerGetName(pt_OsextTimerId ptTimerId)
{
#ifdef RTE_CMSIS_RTOS2
  return osTimerGetName(ptTimerId);
#else
  return ptTimerId->pcName;
#endif
}

/*----------------------------------------------------------------------------*/
e_OsextStatus OSEXT_TimerStart(pt_OsextTimerId ptTimerId, uint32 ulTicks)
{
#ifdef RTE_CMSIS_RTOS2
  return osTimerStart(ptTimerId, ulTicks);
#else
  return osTimerStart(ptTimerId->ptID, ulTicks);
#endif
}

/*----------------------------------------------------------------------------*/
e_OsextStatus OSEXT_TimerStop(pt_OsextTimerId ptTimerId)
{
#ifdef RTE_CMSIS_RTOS2
  return osTimerStop(ptTimerId);
#else
  return osTimerStop(ptTimerId->ptID);
#endif
}

/*----------------------------------------------------------------------------*/
uint32 OSEXT_TimerIsRunning(pt_OsextTimerId ptTimerId)
{
#ifdef RTE_CMSIS_RTOS2
  return osTimerIsRunning(ptTimerId);
#else
  return 0;
#endif
}

/*----------------------------------------------------------------------------*/
e_OsextStatus OSEXT_TimerDelete(pt_OsextTimerId ptTimerId)
{
#ifdef RTE_CMSIS_RTOS2
  return osTimerDelete(ptTimerId);
#else
  e_OsextStatus osStatus = osTimerDelete(ptTimerId->ptID);

  HEAP_FREE(ptTimerId);

  return osStatus;
#endif
}


/*== Event Flags Management Functions ========================================*/

/*----------------------------------------------------------------------------*/
const char *OSEXT_EventFlagsGetName(pt_OsextEventFlagId ptEventFlagId)
{
#ifdef RTE_CMSIS_RTOS2
  return osEventFlagsGetName(ptEventFlagId);
#else
  return ptEventFlagId->ptMsgQueue->pcName;
#endif
}

/*----------------------------------------------------------------------------*/
uint32 OSEXT_EventFlagsSet(pt_OsextEventFlagId ptEventFlagId, uint32 ulFlags)
{
#ifdef RTE_CMSIS_RTOS2
  return osEventFlagsSet(ptEventFlagId, ulFlags);
#else
  ptEventFlagId->ulFlags |= ulFlags;

  OSEXT_MessageQueuePut(ptEventFlagId->ptMsgQueue, &ptEventFlagId->ulFlags, 0, 0);

  return ptEventFlagId->ulFlags;
#endif
}

/*----------------------------------------------------------------------------*/
uint32 OSEXT_EventFlagsClear(pt_OsextEventFlagId ptEventFlagId, uint32 ulFlags)
{
#ifdef RTE_CMSIS_RTOS2
  return osEventFlagsClear(ptEventFlagId, ulFlags);
#else
  ptEventFlagId->ulFlags &= ~ulFlags;

  return ptEventFlagId->ulFlags;
#endif
}

/*----------------------------------------------------------------------------*/
uint32 OSEXT_EventFlagsGet(pt_OsextEventFlagId ptEventFlagId)
{
#ifdef RTE_CMSIS_RTOS2
  return osEventFlagsGet(ptEventFlagId);
#else
  return ptEventFlagId->ulFlags;
#endif
}

#ifndef RTE_CMSIS_RTOS2
static bool mOSEXT_HandleFlag(pt_OsextEventFlagId ptEventFlagId, uint32 ulFlags, uint32 ulOptions)
{
  bool bRetVal = false;

  if (((ulOptions & osFlagsWaitAll) == osFlagsWaitAll) && ((ptEventFlagId->ulFlags & ulFlags) == ulFlags))
  {
    bRetVal = true;
  }
  else if (((ulOptions & osFlagsWaitAny) == osFlagsWaitAny) && (ptEventFlagId->ulFlags & ulFlags))
  {
    bRetVal = true;
  }

  if (bRetVal && ((ulOptions & osFlagsNoClear) == 0))
  {
    ptEventFlagId->ulFlags &= ~ulFlags;
  }

  return bRetVal;
}
#endif

/*----------------------------------------------------------------------------*/
uint32 OSEXT_EventFlagsWait(pt_OsextEventFlagId ptEventFlagId, uint32 ulFlags, uint32 ulOptions, uint32 ulTimeout)
{
#ifdef RTE_CMSIS_RTOS2
  return osEventFlagsWait(ptEventFlagId, ulFlags, ulOptions, ulTimeout);
#else
  uint32  ulMessageFlags;
  bool    bRetVal;
  uint32  ulTime = 0;

  while (!(bRetVal = mOSEXT_HandleFlag(ptEventFlagId, ulFlags, ulOptions)) && (ulTimeout > ulTime))
  {
    uint32 ulTemp = (uint32)IOT_GetMilliSec();

    OSEXT_MessageQueueGet(ptEventFlagId->ptMsgQueue, &ulMessageFlags, NULL, ulTimeout - ulTime);

    ulTime += IOT_GetMilliSec() - ulTemp;
  }

  return bRetVal ? ptEventFlagId->ulFlags : (uint32)osErrorOS;
#endif
}

/*----------------------------------------------------------------------------*/
e_OsextStatus OSEXT_EventFlagsDelete(pt_OsextEventFlagId ptEventFlagId)
{
#ifdef RTE_CMSIS_RTOS2
  return osEventFlagsDelete(ptEventFlagId);
#else
  e_OsextStatus osStatus = OSEXT_MessageQueueDelete(ptEventFlagId->ptMsgQueue);

  HEAP_FREE(ptEventFlagId);

  return osStatus;
#endif
}


/*== Mutex Management Functions ==============================================*/

/*----------------------------------------------------------------------------*/
const char *OSEXT_MutexGetName(pt_OsextMutexId ptMutexId)
{
#ifdef RTE_CMSIS_RTOS2
  return osMutexGetName(ptMutexId);
#else
  return ptMutexId->pcName;
#endif
}

/*----------------------------------------------------------------------------*/
e_OsextStatus OSEXT_MutexAcquire(pt_OsextMutexId ptMutexId, uint32 ulTimeout)
{
#ifdef RTE_CMSIS_RTOS2
  return osMutexAcquire(ptMutexId, ulTimeout);
#else
  e_OsextStatus osStatus = osMutexWait(ptMutexId->osMutex, ulTimeout);

  if (osStatus == osOK)
    ptMutexId->ptOwner = osThreadGetId();

  return osStatus;
#endif
}

/*----------------------------------------------------------------------------*/
e_OsextStatus OSEXT_MutexRelease(pt_OsextMutexId ptMutexId)
{
#ifdef RTE_CMSIS_RTOS2
  return osMutexRelease(ptMutexId);
#else
  e_OsextStatus osStatus = osMutexRelease(ptMutexId->osMutex);;

  if (osStatus == osOK)
    ptMutexId->ptOwner = NULL;

  return osStatus;
#endif
}

/*----------------------------------------------------------------------------*/
pt_OsextThreadId OSEXT_MutexGetOwner(pt_OsextMutexId ptMutexId)
{
#ifdef RTE_CMSIS_RTOS2
  return osMutexGetOwner(ptMutexId);
#else
  uint8  ucI = 0;

  for (ucI = 0; ucI < OSEXT_MAX_EVT_THREAD_LIST_ENTRIES; ucI++)
  {
    if (m_ptThreadList[ucI].ptID == ptMutexId->ptOwner)
      return &m_ptThreadList[ucI];
  }

  return NULL;
#endif
}

/*----------------------------------------------------------------------------*/
e_OsextStatus OSEXT_MutexDelete(pt_OsextMutexId ptMutexId)
{
#ifdef RTE_CMSIS_RTOS2
  return osMutexDelete(ptMutexId);
#else
  e_OsextStatus osStatus = osMutexDelete(ptMutexId->osMutex);

  HEAP_FREE(ptMutexId->osMutexDef.mutex);
  HEAP_FREE(ptMutexId);

  return osStatus;
#endif
}


/*== Semaphore Management Functions ==========================================*/

/*----------------------------------------------------------------------------*/
const char *OSEXT_SemaphoreGetName(pt_OsextSemaphoreId ptSemaphoreId)
{
#ifdef RTE_CMSIS_RTOS2
  return osSemaphoreGetName(ptSemaphoreId);
#else
  return "";
#endif
}

/*----------------------------------------------------------------------------*/
e_OsextStatus OSEXT_SemaphoreAcquire(pt_OsextSemaphoreId ptSemaphoreId, uint32 ulTimeout)
{
#ifdef RTE_CMSIS_RTOS2
  return osSemaphoreAcquire(ptSemaphoreId, ulTimeout);
#else
  return osErrorOS;
#endif
}

/*----------------------------------------------------------------------------*/
e_OsextStatus OSEXT_SemaphoreRelease(pt_OsextSemaphoreId ptSemaphoreId)
{
#ifdef RTE_CMSIS_RTOS2
  return osSemaphoreRelease(ptSemaphoreId);
#else
  return osErrorOS;
#endif
}

/*----------------------------------------------------------------------------*/
uint32 OSEXT_SemaphoreGetCount(pt_OsextSemaphoreId ptSemaphoreId)
{
#ifdef RTE_CMSIS_RTOS2
  return osSemaphoreGetCount(ptSemaphoreId);
#else
  return osErrorOS;
#endif
}

/*----------------------------------------------------------------------------*/
e_OsextStatus OSEXT_SemaphoreDelete(pt_OsextSemaphoreId ptSemaphoreId)
{
#ifdef RTE_CMSIS_RTOS2
  return osSemaphoreDelete(ptSemaphoreId);
#else
  return osErrorOS;
#endif
}


/*== Memory Pool Management Functions ========================================*/

/*----------------------------------------------------------------------------*/
const char *OSEXT_MemoryPoolGetName(pt_OsextMemoryPoolId ptMemoryPoolId)
{
#ifdef RTE_CMSIS_RTOS2
  return osMemoryPoolGetName(ptMemoryPoolId);
#else
  return "";
#endif
}

/*----------------------------------------------------------------------------*/
void *OSEXT_MemoryPoolAlloc(pt_OsextMemoryPoolId ptMemoryPoolId, uint32 ulTimeout)
{
#ifdef RTE_CMSIS_RTOS2
  return osMemoryPoolAlloc(ptMemoryPoolId, ulTimeout);
#else
  return NULL;
#endif
}

/*----------------------------------------------------------------------------*/
e_OsextStatus OSEXT_MemoryPoolFree(pt_OsextMemoryPoolId ptMemoryPoolId, void *pBlock)
{
#ifdef RTE_CMSIS_RTOS2
  return osMemoryPoolFree(ptMemoryPoolId, pBlock);
#else
  return osErrorOS;
#endif
}

/*----------------------------------------------------------------------------*/
uint32 OSEXT_MemoryPoolGetCapacity(pt_OsextMemoryPoolId ptMemoryPoolId)
{
#ifdef RTE_CMSIS_RTOS2
  return osMemoryPoolGetCapacity(ptMemoryPoolId);
#else
  return osErrorOS;
#endif
}

/*----------------------------------------------------------------------------*/
uint32 OSEXT_MemoryPoolGetBlockSize(pt_OsextMemoryPoolId ptMemoryPoolId)
{
#ifdef RTE_CMSIS_RTOS2
  return osMemoryPoolGetBlockSize(ptMemoryPoolId);
#else
  return osErrorOS;
#endif
}

/*----------------------------------------------------------------------------*/
uint32 OSEXT_MemoryPoolGetCount(pt_OsextMemoryPoolId ptMemoryPoolId)
{
#ifdef RTE_CMSIS_RTOS2
  return osMemoryPoolGetCount(ptMemoryPoolId);
#else
  return osErrorOS;
#endif
}

/*----------------------------------------------------------------------------*/
uint32 OSEXT_MemoryPoolGetSpace(pt_OsextMemoryPoolId ptMemoryPoolId)
{
#ifdef RTE_CMSIS_RTOS2
  return osMemoryPoolGetSpace(ptMemoryPoolId);
#else
  return osErrorOS;
#endif
}

/*----------------------------------------------------------------------------*/
e_OsextStatus OSEXT_MemoryPoolDelete(pt_OsextMemoryPoolId ptMemoryPoolId)
{
#ifdef RTE_CMSIS_RTOS2
  return osMemoryPoolDelete(ptMemoryPoolId);
#else
  return osErrorOS;
#endif
}


/*== Message Queue Management Functions ======================================*/

/*----------------------------------------------------------------------------*/
const char *OSEXT_MessageQueueGetName(pt_OsextMessageQueueId ptMessageQueueId)
{
#ifdef RTE_CMSIS_RTOS2
  return osMessageQueueGetName(ptMessageQueueId);
#else
  return ptMessageQueueId->pcName;
#endif
}

/*----------------------------------------------------------------------------*/
e_OsextStatus OSEXT_MessageQueuePut(pt_OsextMessageQueueId ptMessageQueueId, const void *c_pPtr, uint8 ucMsgPrio, uint32 ulTimeout)
{
#ifdef RTE_CMSIS_RTOS2
  return osMessageQueuePut(ptMessageQueueId, c_pPtr, ucMsgPrio, ulTimeout);
#else
  e_OsextStatus osStatus = osMessagePut(ptMessageQueueId->osMsgQueue, (uint32)c_pPtr, ulTimeout);

  if (osStatus == osOK)
    ptMessageQueueId->ulPendingMessages++;
  else
    osStatus = osOK;

  return osStatus;
#endif
}

/*----------------------------------------------------------------------------*/
e_OsextStatus OSEXT_MessageQueueGet(pt_OsextMessageQueueId ptMessageQueueId, uint32 *pulMessage, uint8 *pucMsgPrio, uint32 ulTimeout)
{
#ifdef RTE_CMSIS_RTOS2
  return osMessageQueueGet(ptMessageQueueId, pulMessage, pucMsgPrio, ulTimeout);
#else
  osEvent osEvt = osMessageGet(ptMessageQueueId->osMsgQueue, ulTimeout);

  if (osEvt.status == osEventMessage)
  {
    if (pulMessage)
      *pulMessage = osEvt.value.v;

    osEvt.status = osOK;

    ptMessageQueueId->ulPendingMessages--;
  }

  return osEvt.status;
#endif
}

/*----------------------------------------------------------------------------*/
uint32 OSEXT_MessageQueueGetCapacity(pt_OsextMessageQueueId ptMessageQueueId)
{
#ifdef RTE_CMSIS_RTOS2
  return osMessageQueueGetCapacity(ptMessageQueueId);
#else
  return ptMessageQueueId->ulCount;
#endif
}

/*----------------------------------------------------------------------------*/
uint32 OSEXT_MessageQueueGetMsgSize(pt_OsextMessageQueueId ptMessageQueueId)
{
#ifdef RTE_CMSIS_RTOS2
  return osMessageQueueGetMsgSize(ptMessageQueueId);
#else
  return ptMessageQueueId->ulSize;
#endif
}

/*----------------------------------------------------------------------------*/
uint32 OSEXT_MessageQueueGetCount(pt_OsextMessageQueueId ptMessageQueueId)
{
#ifdef RTE_CMSIS_RTOS2
  return osMessageQueueGetCount(ptMessageQueueId);
#else
  return ptMessageQueueId->ulPendingMessages;
#endif
}

/*----------------------------------------------------------------------------*/
uint32 OSEXT_MessageQueueGetSpace(pt_OsextMessageQueueId ptMessageQueueId)
{
#ifdef RTE_CMSIS_RTOS2
  return osMessageQueueGetSpace(ptMessageQueueId);
#else
  return ptMessageQueueId->ulCount - ptMessageQueueId->ulPendingMessages;
#endif
}

/*----------------------------------------------------------------------------*/
e_OsextStatus OSEXT_MessageQueueReset(pt_OsextMessageQueueId ptMessageQueueId)
{
#ifdef RTE_CMSIS_RTOS2
  return osMessageQueueReset(ptMessageQueueId);
#else
  uint32 ulAttemps = 50;

  while ((ptMessageQueueId->ulPendingMessages > 0) && ulAttemps--)
    OSEXT_MessageQueueGet(ptMessageQueueId, NULL, NULL, 0);

  return ulAttemps == 0 ? osErrorOS : osOK;
#endif
}

/*----------------------------------------------------------------------------*/
e_OsextStatus OSEXT_MessageQueueDelete(pt_OsextMessageQueueId ptMessageQueueId)
{
#ifdef RTE_CMSIS_RTOS2
  return osMessageQueueDelete(ptMessageQueueId);
#else
  HEAP_FREE(ptMessageQueueId->osMessageQDef.pool);
  HEAP_FREE(ptMessageQueueId);

  return osOK;
#endif
}




/*----------------------------------------------------------------------------*/
#if defined(RTE_CMSIS_RTOS2) && STDIO_INIT > 0
uint32_t osRtxErrorNotify(uint32_t code, void *object_id)
{
  char            acText[100];
  osRtxThread_t  *ptThreadID = (osRtxThread_t *)object_id;

  switch (code) {
    case osRtxErrorStackUnderflow:
      sprintf(acText, "Stack overflow detected for thread (0x%x, %s)", (uint32)object_id, ptThreadID->name);
      break;

    case osRtxErrorISRQueueOverflow:
      sprintf(acText, "ISR Queue overflow detected when inserting object (0x%x)", (uint32)object_id);
      break;

    case osRtxErrorTimerQueueOverflow:
      sprintf(acText, "User Timer Callback Queue overflow detected for timer (0x%x)", (uint32)object_id);
      break;

    case osRtxErrorClibSpace:
      sprintf(acText, "Standard C/C++ library libspace not available: increase OS_THREAD_LIBSPACE_NUM");
      break;

    case osRtxErrorClibMutex:
      sprintf(acText, "Standard C/C++ library mutex initialization failed");
      break;

    default:
      sprintf(acText, "UNKNOWN ERROR CODE %d", code);
      break;
  }

  ASSERT_FAIL_MSG(acText);

  // never reached
  return 0;
}
#endif

/*----------------------------------------------------------------------------*/
void OSEXT_IdleThread(void *pArg)
{
  while (true)
  {
    HEAP_IdleAction();
  }
}

/*----------------------------------------------------------------------------*/
void OSEXT_Init(void)
{
  if (!m_ptMessageQueue)
    m_ptMessageQueue = LIST_Create(OSEXT_MAX_MESSAGE_QUEUE_LIST_ENTRIES);

  // create idle thread
  OSEXT_IdleThreadNew(OSEXT_IdleThread, NULL);

#if SHELL_OSEXT > 0
  // register shell-command
#endif
}


/*----------------------------------------------------------------------------*/
// SHELL Interface
/*----------------------------------------------------------------------------*/
#if SHELL_OSEXT > 0
  #include "SHELL.h"
  #include "DT.h"

  /*--------------------------------------------------------------------------*/
  static const char mc_acLine[] = "================================================================================\r\n";

  /*--------------------------------------------------------------------------*/
  extern uint16 os_maxtaskrun;

  /*--------------------------------------------------------------------------*/
  /// this text will be displayed, in the shell-command overview 
  static const char mc_acSummaryOsext[] =
    "OS Extensions: Display list of tasks and other informations.\r\n";

  /// this text will be displayed, if this module return E_SHELL_CMD_WRONG_PARAMETERS 
  static const char mc_acUsageOsext[] =
    "osext ['stack' | 'wait' | 'evt']\r\n"
    "  stack: display stack usage of each task\r\n"
    "  wait:  display, where each task is waiting\r\n"
    "  evt:   <start><id> start/stop event printing\r\n"
    "    start:  1 (Start), 0 (Stop)\r\n"
    "    id:     If start == 1 -> id of thread\r\n"
    "            If start == 0 -> list index or empty to stop everything\r\n"
    "\r\n"
    "Examples: 'osext'\r\n"
    "          'osext wait'\r\n"
    "          'osext evt 1 20000530'\r\n"
    "          'osext evt 0'\r\n"
    "          'osext evt 0 1'\r\n";

  /*----------------------------------------------------------------------------*/
  /** @brief      send an event to the message box

      @param[in]  eEvtRecord      record type
      @param[in]  ptThreadId      thread id
      @param[in]  ucIndex         index of list entry
      @param[in]  ulTimeout       delay in ms
      @return     None

      @author     S.Schuepbach                                @date 28.04.2017  */
  /*----------------------------------------------------------------------------*/
  static void mOSEXT_SendEvt(e_EvtRecord eEvtRecord, pt_OsextThreadId ptThreadId, uint8 ucIndex, uint32 ulTimeout)
  {
    if (m_ptMessageQueueEvtRecord != NULL)
    {
      t_OsextWaitObject  *ptOsextWaitObject = HEAP_MALLOC(sizeof(t_OsextWaitObject));
      t_Msg              *ptMsg = HEAP_MALLOC(sizeof(t_Msg));
      osRtxThread_t      *threadID = (osRtxThread_t *)ptThreadId;

      memset(ptMsg, 0, sizeof(t_Msg));
      memset(ptOsextWaitObject, 0, sizeof(t_OsextWaitObject));

      ptMsg->ucEvent = eEvtRecord;
      ptMsg->p1 = ptThreadId;
      ptMsg->p2 = ptOsextWaitObject;
      ptMsg->ul1 = IOT_GetMilliSec();
      ptMsg->ul2 = ucIndex;

      ptOsextWaitObject->pcName = threadID->name;
      ptOsextWaitObject->ulDelay = ulTimeout;

      if (eEvtRecord == OSEXT_EVT_THREAD_BLOCKED)
      {
        ptOsextWaitObject->ulDelay = threadID->delay;

        if (threadID->thread_prev)
          ptOsextWaitObject->pcWaitObjectName = threadID->thread_prev->name;

        ptOsextWaitObject->pcDWaitObjectType = mOSEXT_ShellGetGenericStateName(threadID->state);
      }

      OSEXT_MessageQueuePut(m_ptMessageQueueEvtRecord, &ptMsg, 0, 0);
    }
  }

  /*--------------------------------------------------------------------------*/
  static char *mOSEXT_ShellGetGenericStateName(uint8 ucState)
  {
    char *pacName = NULL;

    switch (ucState)
    {
      case osRtxThreadWaitingDelay       :
        pacName = "Delay";
        break;

      case osRtxThreadWaitingJoin        :
        pacName = "Join";
        break;

      case osRtxThreadWaitingThreadFlags :
        pacName = "ThreadFlags";
        break;

      case osRtxThreadWaitingEventFlags  :
        pacName = "EventFlags";
        break;

      case osRtxThreadWaitingMutex       :
        pacName = "Mutex";
        break;

      case osRtxThreadWaitingSemaphore   :
        pacName = "Semaphore";
        break;

      case osRtxThreadWaitingMemoryPool  :
        pacName = "MemoryPool";
        break;

      case osRtxThreadWaitingMessageGet  :
        pacName = "MessageGet";
        break;

      case osRtxThreadWaitingMessagePut  :
        pacName = "MessagePut";
        break;
    }

    return pacName;
  }
  
  /*--------------------------------------------------------------------------*/
  static char *mOSEXT_ShellGetThreadStateName(e_OsextThreadState eState)
  {
    char *pacName;

    switch ((uint32) eState)
    {
      case osThreadInactive:
        pacName = "Inactive";
        break;

      case osThreadReady:
        pacName = "Ready";
        break;

      case osThreadRunning:
        pacName = "Running";
        break;

      case osThreadBlocked:
        pacName = "Blocked";
        break;

      case osThreadTerminated:
        pacName = "Terminated";
        break;

      default:
        pacName = "Error";
    }

    return pacName;
  }

  /*--------------------------------------------------------------------------*/
  static void mOSEXT_ShellListTasks(const t_ShellCmdContext *c_ptContext, bool bShowStack, bool bShowWait)
  {
    uint32             ulNumItems = OSEXT_ThreadGetCount();
    pt_OsextThreadId  *patThreads = HEAP_MALLOC(ulNumItems * sizeof(pt_OsextThreadId));
    uint32             ulI;

    uint32   ulTotSize = 0;
    uint32   ulTotUsage = 0;
    char    *pacDateTime = DT_GetDateTimeStr(NULL, 0, "%Y-%m-%d / %H:%M:%S", NULL);

    fprintf(c_ptContext->pfOut, mc_acLine);
    fprintf(c_ptContext->pfOut, "=== %s %.54s\r\n", pacDateTime, mc_acLine);
    fprintf(c_ptContext->pfOut, mc_acLine);

    HEAP_FREE(pacDateTime);

    if (bShowWait)
    {
      fprintf(c_ptContext->pfOut, " %-12.12s | %-18.18s| %-8.8s| %-12.12s| %s \r\n", "Thread ID", "Thread Name", "Delay",
              "Wait Type", "Wait Object Name");
      fprintf(c_ptContext->pfOut, mc_acLine);
    }

    OSEXT_ThreadEnumerate(patThreads, ulNumItems);

    for (ulI = 0; ulI < ulNumItems; ulI++)
    {
      uint32   ulSize = OSEXT_ThreadGetStackSize(patThreads[ulI]);
      uint32   ulUsage = ulSize - OSEXT_ThreadGetStackSpace(patThreads[ulI]);
      char    *pacThreadName = (char *) OSEXT_ThreadGetName(patThreads[ulI]);

      // internal rtx thread structure
      osRtxThread_t  *threadID = (osRtxThread_t *) patThreads[ulI];
      uint32          ulCurrentUsage = ulSize - (threadID->sp - (uint32) threadID->stack_mem);
      int32           lTimeout = 0;

      char   acWaitStr[30] = {0};
      char   acTime[10] = {0};
      char  *pacState = mOSEXT_ShellGetThreadStateName(OSEXT_ThreadGetState(patThreads[ulI]));

      ulTotSize += ulSize;
      ulTotUsage += ulUsage;

      if (!pacThreadName)
        pacThreadName = "[System Thread?]";

      if (bShowStack)
      {
        fprintf(c_ptContext->pfOut, " ID:0x%3x - Stack[%4d] -> cur:%2d%% [%4d], max:%2d%% [%4d] - %s\r\n",
                patThreads[ulI],
                ulSize,
                (uint8) ((double) ulCurrentUsage / (double) ulSize * 100),
                ulCurrentUsage,
                (uint8) ((double) ulUsage / (double) ulSize * 100),
                ulUsage,
                pacThreadName);
      } 
      else if (bShowWait)
      {
        if (OSEXT_ThreadGetState(patThreads[ulI]) == osThreadBlocked)
        {
          lTimeout = threadID->delay;
          pacState = mOSEXT_ShellGetGenericStateName(threadID->state);

          if (threadID->thread_prev && threadID->thread_prev->name)
            sprintf(acWaitStr, "| %s", threadID->thread_prev->name);

          if (lTimeout > 0)
            sprintf(acTime, "%4d ms | ", lTimeout);
          else
            sprintf(acTime, "ForEver | ");
        }

        fprintf(c_ptContext->pfOut, " 0x%3x   | %-18.18s| %s%-12.12s%s\r\n",
                patThreads[ulI],
                pacThreadName,
                acTime,
                pacState,
                acWaitStr);
      }
      else
      {
        fprintf(c_ptContext->pfOut, " ID:0x%3x - State: %-10.10s - Prio: %2d - %s\r\n",
                patThreads[ulI],
                mOSEXT_ShellGetThreadStateName(OSEXT_ThreadGetState(patThreads[ulI])),
                OSEXT_ThreadGetPriority(patThreads[ulI]),
                pacThreadName);
      }
    }

    if (bShowStack)
    {
      fprintf(c_ptContext->pfOut, mc_acLine);
      fprintf(c_ptContext->pfOut, "=> TOTAL: Stack-Size:[%5d]            Stack-Usage:%2d%% [%d]\r\n",
              ulTotSize,
              (uint8) ((double) ulTotUsage / (double) ulTotSize * 100),
              ulTotUsage);
    }

    fprintf(c_ptContext->pfOut, "%s\r\n", mc_acLine);

    HEAP_FREE(patThreads);
  }

  /*--------------------------------------------------------------------------*/
  static void mOSEXT_EvtThread(void *pArgs)
  {
    t_Msg  *ptMsg;
    char    acTime[50] = {0};

    fprintf(pArgs, "Start Eevent Recording\r\n");

    while (1)
    {
      memset(acTime, 0, sizeof(acTime));

      // wait for events
      OSEXT_MessageQueueGet(m_ptMessageQueueEvtRecord, (uint32*)&ptMsg, NULL, osWaitForever);

      t_OsextWaitObject *ptOsextWaitObject = (t_OsextWaitObject *)ptMsg->p2;

      if ((ptOsextWaitObject->ulDelay != 0) && ptOsextWaitObject->pcDWaitObjectType)
      {
        if (ptOsextWaitObject->ulDelay != 0xFFFFFFFF)
          sprintf(acTime, " - %s (%4d ms, %s)", ptOsextWaitObject->pcDWaitObjectType, ptOsextWaitObject->ulDelay, ptOsextWaitObject->pcWaitObjectName ? ptOsextWaitObject->pcWaitObjectName : "");
        else
          sprintf(acTime, " - %s (ForEver, %s)", ptOsextWaitObject->pcDWaitObjectType, ptOsextWaitObject->pcWaitObjectName ? ptOsextWaitObject->pcWaitObjectName : "");
      }

      fprintf(pArgs, "[%5.3f] %10.10s: %15.15s(%d)%s\r\n",((double)ptMsg->ul1) / 1000.0,
              (ptMsg->ucEvent == OSEXT_EVT_THREAD_BLOCKED) ? "Blocked" : "Run",
              ptOsextWaitObject->pcName, ptMsg->ul2, acTime);

      HEAP_FREE(ptOsextWaitObject);
      HEAP_FREE(ptMsg);
    }
  }

  /*--------------------------------------------------------------------------*/
  static void mOSEXT_ShellShowEvents(const t_ShellCmdContext *c_ptContext, bool bStart, pt_OsextThreadId ptThreadId)
  {
    uint32  ulI;

    // start event recorder
    if (bStart)
    {
      // check if thread id is valid
      if (OSEXT_ThreadGetState(ptThreadId) == osThreadError)
      {
        fprintf(c_ptContext->pfOut, "ERROR: THREAD ID UNKOWN\r\n");
      }
      else
      {
        // check if list is already created
        if (m_ptListEvtThreadIds == NULL)
          m_ptListEvtThreadIds = LIST_Create(OSEXT_MAX_EVT_THREAD_LIST_ENTRIES);

        // check if there is enough space in list
        if (LIST_ItemCount(m_ptListEvtThreadIds) < OSEXT_MAX_EVT_THREAD_LIST_ENTRIES)
        {
          // add entry to list -> start task if necessary -> create message queue if necessary
          uint32 ulValue = (uint32)ptThreadId;

          fprintf(c_ptContext->pfOut, "Add Thread %s to List -> Index:%d\r\n", OSEXT_ThreadGetName(ptThreadId),
                  LIST_AddItem(m_ptListEvtThreadIds, (uint8 *)&ulValue, sizeof(ulValue)));

          if (m_ptThreadEvtRecord == NULL)
            m_ptThreadEvtRecord = OSEXT_ShellThreadEvtRecordNew(mOSEXT_EvtThread, c_ptContext->pfOut);
          if (m_ptMessageQueueEvtRecord == NULL)
            m_ptMessageQueueEvtRecord = OSEXT_ShellMessageQueueEvtRecordNew();
        }
        else
          fprintf(c_ptContext->pfOut, "ERROR: NOT ENOUGH SPACE IN LIST\r\n");
      }
    }
    // stop event recorder
    else
    {
      // stop only a single thread
      if (ptThreadId != NULL)
      {
        ulI = (uint32)ptThreadId;
        ulI -= 1;

        if (ulI < LIST_ItemCount(m_ptListEvtThreadIds))
        {
          uint32 ulThreadID = *(uint32 *)LIST_GetItem(m_ptListEvtThreadIds, ulI, NULL);

          fprintf(c_ptContext->pfOut, "Remove Thread %s from List -> %s\r\n", OSEXT_ThreadGetName((osThreadId)ulThreadID),
                  LIST_DeleteItem(m_ptListEvtThreadIds, ulI) == 0? "Successful" : "Failed");
        }
        else
          fprintf(c_ptContext->pfOut, "ERROR: Index %d out of range", ptThreadId);
      }

      // if all thread should be stopped or if no thread is left in list
      if ((ptThreadId == NULL) || (m_ptListEvtThreadIds != NULL && LIST_ItemCount(m_ptListEvtThreadIds) == 0))
      {
        LIST_Free(m_ptListEvtThreadIds);

        OSEXT_ThreadTerminate(m_ptThreadEvtRecord);
        OSEXT_MessageQueueDelete(m_ptMessageQueueEvtRecord);

        m_ptListEvtThreadIds = NULL;
        m_ptThreadEvtRecord = NULL;
        m_ptMessageQueueEvtRecord = NULL;

        fprintf(c_ptContext->pfOut, "Stop Eevent Recording\r\n");
      }
    }
  }

  /*--------------------------------------------------------------------------*/
  static uint32 mOSEXT_ShellOsext(const t_ShellCmdContext *c_ptContext)
  {
    uint32  ulErr = 0;
    uint8   ucI = 0;

    if (SHELL_CheckCmdParam(c_ptContext, "stack", 1))
      mOSEXT_ShellListTasks(c_ptContext, true, false);
    else if (SHELL_CheckCmdParam(c_ptContext, "wait", 1))
      mOSEXT_ShellListTasks(c_ptContext, false, true);
    else if (SHELL_CheckCmdParamEx(c_ptContext, "evt", 2, 2, 3))
    {
      uint8 ucStart = (uint8)atoi((char *)LIST_GetItem(c_ptContext->ptParams, 1, NULL));

      pt_OsextThreadId ptThreadId = (c_ptContext->ucNumParams > 2)
        ? (pt_OsextThreadId)strtol((char *)LIST_GetItem(c_ptContext->ptParams, 2, NULL), NULL, 16)
        : NULL;

      mOSEXT_ShellShowEvents(c_ptContext, (bool)ucStart > 0, ptThreadId);
    }
    else if (SHELL_CheckCmdParam(c_ptContext, "messages", 1))
    {
      for (ucI = 0; ucI < LIST_ItemCount(m_ptMessageQueue); ucI++)
      {
        osRtxMessageQueue_t *ptMessageQueueID = (osRtxMessageQueue_t *)*(uint32 *)LIST_GetItem(m_ptMessageQueue, ucI, NULL);

        fprintf(c_ptContext->pfOut, " --- %-15.15s ---------------- (%d of %d)\r\n", ptMessageQueueID->name,
                ptMessageQueueID->msg_count, ptMessageQueueID->mp_info.max_blocks);

        osRtxMessage_t * ptMessageID = ptMessageQueueID->msg_first;

        if (ptMessageID)
        {
          char   *pcTxt = HEAP_MALLOC(ptMessageQueueID->msg_size + 1);
          uint8   ucK = 1;

          do
          {
            memset(pcTxt, 0, ptMessageQueueID->msg_size + 1);
            strncpy(pcTxt, (const char *)ptMessageID + sizeof(os_message_t), ptMessageQueueID->msg_size);
            fprintf(c_ptContext->pfOut, "   %d. %s\r\n", ucK++, pcTxt);
          } while ((ptMessageID = ptMessageID->next) != 0);

          HEAP_FREE(pcTxt);
        }

        fprintf(c_ptContext->pfOut, "\r\n");
      }
    }
    else if (c_ptContext->ucNumParams != 0)
      ulErr = E_SHELL_CMD_WRONG_PARAMETERS;
    else
      mOSEXT_ShellListTasks(c_ptContext, false, false);

    return ulErr;
  }

  /*--------------------------------------------------------------------------*/
  static void mOSEXT_InitShell(void)
  {
    SHELL_AddCommand("osext", mOSEXT_ShellOsext, mc_acSummaryOsext, mc_acUsageOsext);
  }
#endif   // SHELL_OSEXT
  
/// @}   // End of Group
  
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

