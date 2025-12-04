#ifndef __OSEXT_H__
#define __OSEXT_H__

/** @defgroup   OSEXT  OSEXT-Group
                Extensions to RTX.
    @{
*******************************************************************************
*   @name       OSEXT
*   @file       OSEXT.h
*
*   @brief      Extensions to RTX
*
*   @authors    A.Bader
*   @date       05.06.2014
*
*   @copyright (C) 2014 by: MICROTRONIC AG
*                           Duennernstrasse 32
*                           CH-4702 Oensingen
*******************************************************************************/

/*== Includes ================================================================*/


/*== Defines =================================================================*/

// thread definitions
#if defined(__BOOT)
  #define OSEXT_BootThreadNew(func, arg)          OSEXT_ThreadNew(func, arg, "Boot_Thread",        2048, 55)
#elif defined(__TG000)
  #define OSEXT_TestThreadNew(func, arg)          OSEXT_ThreadNew(func, arg, "Test_Thread",        2048, 55)
#else
  #ifdef __MICROPAY
    #define OSEXT_InitThreadNew(func, arg)        OSEXT_ThreadNew(func, arg, "Init_Thread",        4096, 55)
  #else
    #define OSEXT_InitThreadNew(func, arg)        OSEXT_ThreadNew(func, arg, "Init_Thread",        4096, 55)
  #endif
#endif

#define OSEXT_UihThreadNew(func, arg)             OSEXT_ThreadNew(func, arg, "UIH_Thread",         1024, 54) // G6: 254 [Initial (Reduced by Task itself to 11 after Startup)]

#define OSEXT_MdbsThreadNew(func, arg)            OSEXT_ThreadNew(func, arg, "MDBS_Thread",        2048, 53) // G6 -> 100

// be aware of the OS_TIMER_THREAD_PRIO (48) -> configured in RTX_Config.h
#define OSEXT_MdbrdThreadNew(func, arg)           OSEXT_ThreadNew(func, arg, "MDBRD_Thread",       2048, 52) // G6 -> 65   NO CCM for Stack [HardFault] !!!
#define OSEXT_MdbrcThreadNew(func, arg)           OSEXT_ThreadNew(func, arg, "MDBRC_Thread",       2048, 52) // G6 -> 65

#define OSEXT_MdbcgThreadNew(func, arg)           OSEXT_ThreadCCM(func, arg, "MDBCG_Thread",             52) // G6 -> 65   Uses CCM for Stack [see OSEXT.c]
#define OSEXT_MdbbvThreadNew(func, arg)           OSEXT_ThreadCCM(func, arg, "MDBBV_Thread",             52) // G6 -> 65   Uses CCM for Stack [see OSEXT.c]
#define OSEXT_MdbmThreadNew(func, arg)            OSEXT_ThreadCCM(func, arg, "MDBM_Thread",              51) // G6 -> 62   Uses CCM for Stack [see OSEXT.c]
#define OSEXT_ExesThreadNew(func, arg)            OSEXT_ThreadNew(func, arg, "EXES_Thread",        1024, 51) // G6 -> 62

#define OSEXT_ExemThreadNew(func, arg)            OSEXT_ThreadNew(func, arg, "EXEM_Thread",        2048, 45) // G6 -> 61

#define OSEXT_ClhThreadNew(func, arg)             OSEXT_ThreadNew(func, arg, "CLH_Thread",         1024, 40) // G6 -> 40
#define OSEXT_CshThreadNew(func, arg)             OSEXT_ThreadNew(func, arg, "CSH_Thread",         1024, 40) // G6 -> 40

#define OSEXT_NfcThreadNew(func, arg)             OSEXT_ThreadNew(func, arg, "NFC_Thread",         2048, 39)
#define OSEXT_UkeyThreadNew(func, arg)            OSEXT_ThreadNew(func, arg, "UKEY_Thread",        3072, 38)

#ifdef __TG000
  #define OSEXT_HWTestThreadNew(func, arg)        OSEXT_ThreadNew(func, arg, "HWTest_Thread",      3072, 33)
#endif

#define OSEXT_UsbfsThreadNew(func, arg)           OSEXT_ThreadNew(func, arg, "USBFS_Thread",       3072, 32)
#define OSEXT_UsbhsThreadNew(func, arg)           OSEXT_ThreadNew(func, arg, "USBHS_Thread",       1024, 32)

#define OSEXT_ScardThreadNew(func, arg)           OSEXT_ThreadNew(func, arg, "SCARD_Thread",       1024, 32)
#define OSEXT_BleThreadNew(func, arg)             OSEXT_ThreadNew(func, arg, "BLE_Thread",         1024, 31) // BLE Module
#define OSEXT_LcdThreadNew(func, arg)             OSEXT_ThreadNew(func, arg, "LCD_Thread",         1024, 31)
#define OSEXT_TftThreadNew(func, arg)             OSEXT_ThreadNew(func, arg, "TFT_Thread",         1536, 31)
#define OSEXT_SouThreadNew(func, arg)             OSEXT_ThreadNew(func, arg, "SOU_Thread",         1024, 30)
#define OSEXT_EthThreadNew(func, arg)             OSEXT_ThreadNew(func, arg, "ETH_Thread",         2048, 29)
#define OSEXT_FtpcThreadNew(func, arg)            OSEXT_ThreadNew(func, arg, "FTPC_Thread",        1024, 28)
#define OSEXT_RtcmThreadNew(func, arg)            OSEXT_ThreadNew(func, arg, "RTCM_Thread",        1024+256, 28)

#define OSEXT_MdsThreadNew(func, arg)             OSEXT_ThreadNew(func, arg, "MDS_Thread",         1024, 26)
#define OSEXT_IrRemcThreadNew(func, arg)          OSEXT_ThreadNew(func, arg, "IRDA_Thread",        1024, 26)
#define OSEXT_CanThreadNew(func, arg)             OSEXT_ThreadNew(func, arg, "CAN_Thread",         1024, 25)
#define OSEXT_DcmpThreadNew(func, arg)            OSEXT_ThreadNew(func, arg, "DCMP_Thread",        3072, 53)
#define OSEXT_PuDcmpThreadNew(func, arg)          OSEXT_ThreadNew(func, arg, "PUDCMP_Thread",      2048, 24)
#define OSEXT_AppThreadNew(func, arg)             OSEXT_ThreadNew(func, arg, "APP_Thread",         4096+256, 20)
#define OSEXT_AppStmThreadNew(func, arg)          OSEXT_ThreadNew(func, arg, "APPSTM_Thread",      3072+256, 20)

#define OSEXT_QrcHalThreadNew(func, arg)          OSEXT_ThreadNew(func, arg, "QRCHAL_Thread",      1024, 13) // G6 -> 39
#define OSEXT_QrcThreadNew(func, arg)             OSEXT_ThreadNew(func, arg, "QRC_Thread",         1024, 12) // G6 -> 38
#define OSEXT_RfmselThreadNew(func, arg)          OSEXT_ThreadCCM(func, arg, "RFMSEL_Thread",            12) // G6 -> 15   Uses CCM for Stack [see OSEXT.c]
#define OSEXT_RfmpriceThreadNew(func, arg)        OSEXT_ThreadCCM(func, arg, "RFMPRICE_Thread",          12) // G6 -> 14   Uses CCM for Stack [see OSEXT.c]
#define OSEXT_RfmSigmaThreadNew(func, arg)        OSEXT_ThreadNew(func, arg, "RFMSigma_Thread",    1024, 12)
#define OSEXT_UsbThreadNew(func, arg)             OSEXT_ThreadCCM(func, arg, "USB_Thread",               12) // Uses CCM for Stack [see OSEXT.c]

#define OSEXT_ErhThreadNew(func, arg)             OSEXT_ThreadNew(func, arg, "ERH_Thread",         1024, 11) // G6 -> 50
#define OSEXT_ShellThreadNew(func, arg)           OSEXT_ThreadNew(func, arg, "SHELL_Thread",       3072, 10)
#define OSEXT_ShellThreadEvtRecordNew(func, arg)  OSEXT_ThreadNew(func, arg, "SHELL_ThreadEvtRec", 2048,  9)
#define OSEXT_WdgThreadNew(func, arg)             OSEXT_ThreadNew(func, arg, "WDG_Thread",         1024,  8)
#define OSEXT_OtrxThreadNew(func, arg)            OSEXT_ThreadNew(func, arg, "OTRX_Thread",        3072,  8) // G6 -> 31
#define OSEXT_VmcThreadNew(func, arg)             OSEXT_ThreadNew(func, arg, "VMC_Thread",         2048,  8) // G6 -> 20
#define OSEXT_ProtThreadNew(func, arg)            OSEXT_ThreadNew(func, arg, "PROT_Thread",        2048,  8) // G6 -> 13
#define OSEXT_IdleThreadNew(func, arg)            OSEXT_ThreadNew(func, arg, "IDLE_Thread",        1024,  2)

// message queue definitions                                            Count   Size                Name
#define OSEXT_AppStmMessageQueueNew()             OSEXT_MessageQueueNew(10,     sizeof(t_Msg *),   "APPSTM_Message")
#define OSEXT_RtcmMessageQueueNew()               OSEXT_MessageQueueNew(10,     sizeof(t_Msg *),   "RTCM_Message")
#define OSEXT_EthMessageQueueNew()                OSEXT_MessageQueueNew(10,     sizeof(t_Msg *),   "ETH_Message")
#define OSEXT_EthResponseQueueNew()               OSEXT_MessageQueueNew(1,      sizeof(t_Msg *),   "ETH_Response")
#define OSEXT_FtpcMessageQueueNew()               OSEXT_MessageQueueNew(10,     sizeof(t_Msg *),   "FTPC_Message")
#define OSEXT_SouMessageQueueNew()                OSEXT_MessageQueueNew(10,     sizeof(t_Msg *),   "SOU_Message")
  
#if defined(__TG000) || defined(__BOOT)
  #define OSEXT_AppMessageQueueNew()              OSEXT_MessageQueueNew(10,     sizeof(t_Msg *),   "APP_Message")
#endif

#ifdef __TG000
  #define OSEXT_HWTestMessageQueueNew()           OSEXT_MessageQueueNew(10,     sizeof(t_Msg *),   "HWTest_Message")
  #define OSEXT_HWTestResponseQueueNew()          OSEXT_MessageQueueNew(10,     sizeof(t_Msg *),   "HWTest_Response")
  #define OSEXT_NfcTestMessageQueueNew()          OSEXT_MessageQueueNew(10,     sizeof(t_Msg *),   "NfcTest_Message")
#endif

#define OSEXT_VmcUsartMessageQueueNew()           OSEXT_MessageQueueNew(10,     sizeof(t_Msg *),   "VMC_USART_Message")
#define OSEXT_VmcMessageQueueNew()                OSEXT_MessageQueueNew(20,     sizeof(t_Msg *),   "VMC_Message")
#define OSEXT_VmcResponseQueueNew()               OSEXT_MessageQueueNew(1,      sizeof(t_Msg *),   "VMC_Response")
#define OSEXT_MdbsMessageQueueNew()               OSEXT_MessageQueueNew(10,     sizeof(t_Msg *),   "MDBS_Message")
#define OSEXT_MdbcgMessageQueueNew()              OSEXT_MessageQueueNew(10,     sizeof(t_Msg *),   "MDBCG_Message")
#define OSEXT_MdbbvMessageQueueNew()              OSEXT_MessageQueueNew(10,     sizeof(t_Msg *),   "MDBBV_Message")
#define OSEXT_MdbrdMessageQueueNew()              OSEXT_MessageQueueNew(10,     sizeof(t_Msg *),   "MDBRD_Message")
#define OSEXT_MdbrcMessageQueueNew()              OSEXT_MessageQueueNew(10,     sizeof(t_Msg *),   "MDBRC_Message")
#define OSEXT_NfcMessageQueueNew()                OSEXT_MessageQueueNew(10,     sizeof(t_Msg *),   "NFC_Message")
#define OSEXT_NfcResponseQueueNew()               OSEXT_MessageQueueNew(10,     sizeof(t_Msg *),   "NFC_Response")
#define OSEXT_UkeyMessageQueueNew()               OSEXT_MessageQueueNew(10,     sizeof(t_Msg *),   "UKEY_Message")
#define OSEXT_UkeyResponseQueueNew()              OSEXT_MessageQueueNew(10,     sizeof(t_Msg *),   "UKEY_Response")
#define OSEXT_CshMessageQueueNew()                OSEXT_MessageQueueNew(10,     sizeof(t_Msg *),   "CSH_Message")
#define OSEXT_ClhMessageQueueNew()                OSEXT_MessageQueueNew(10,     sizeof(t_Msg *),   "CLH_Message")
#define OSEXT_UihMessageQueueNew()                OSEXT_MessageQueueNew(20,     sizeof(t_Msg *),   "UIH_Message")
#define OSEXT_ErhMessageQueueNew()                OSEXT_MessageQueueNew(5,      sizeof(t_Msg *),   "ERH_Message")
#define OSEXT_QrcMessageQueueNew()                OSEXT_MessageQueueNew(10,     sizeof(t_Msg *),   "QRC_Message")
#define OSEXT_ExesMessageQueueNew()               OSEXT_MessageQueueNew(10,     sizeof(t_Msg *),   "EXES_Message")
#define OSEXT_ExesResponseQueueNew()              OSEXT_MessageQueueNew(2,      sizeof(t_Msg *),   "EXES_Response")
#define OSEXT_ExemMessageQueueNew()               OSEXT_MessageQueueNew(10,     sizeof(t_Msg *),   "EXEM_Message")
#define OSEXT_OtrxMessageQueueNew()               OSEXT_MessageQueueNew(10,     sizeof(t_Msg *),   "OTRX_Message")
#define OSEXT_UsbMessageQueueNew()                OSEXT_MessageQueueNew(10,     sizeof(t_Msg *),   "USB_Message")
#define OSEXT_DcmpSmMessageQueueNew()             OSEXT_MessageQueueNew(10,     sizeof(t_Msg *),   "DCMP_SM_Message")
#define OSEXT_PudcmpMessageQueueNew()             OSEXT_MessageQueueNew(5,      sizeof(t_Msg *),   "PUDCMP_Message")
#define OSEXT_DcmpTo2MessageQueueNew()            OSEXT_MessageQueueNew(1,      sizeof(t_Msg *),   "DCMP_TO2_Message")
#define OSEXT_ShellMessageQueueEvtRecordNew()     OSEXT_MessageQueueNew(10,     sizeof(t_Msg *),   "SHELL_MessageEvtRecord")

// mutex definitions                                              Name
#define OSEXT_RtcmMutexWakeUpNew()                OSEXT_MutexNew("RTCM_MutexWakeUp")
#define OSEXT_RtcmMutexAlarmNew()                 OSEXT_MutexNew("RTCM_MutexAlarm")
#define OSEXT_HeapMutexTableNew()                 OSEXT_MutexNew("HEAP_MutexTable")
#define OSEXT_ShellMutexCmdNew()                  OSEXT_MutexNew("SHELL_MutexCmd")
#define OSEXT_SdioMutexRxNew()                    OSEXT_MutexNew("SDIO_MutexRx")
#define OSEXT_SdioMutexTxNew()                    OSEXT_MutexNew("SDIO_MutexTx")
#define OSEXT_FifoMutexNew()                      OSEXT_MutexNew("FIFO_Mutex")
#define OSEXT_LcdMutexTransferNew()               OSEXT_MutexNew("LCD_MutexTransfer")
#define OSEXT_TftMutexTransferNew()               OSEXT_MutexNew("TFT_MutexTransfer")
#define OSEXT_BleMutexTransferNew()               OSEXT_MutexNew("BLE_MutexTransfer")
#define OSEXT_HashmMutexNew()                     OSEXT_MutexNew("HASHM_Mutex")
#define OSEXT_CrypmMutexNew()                     OSEXT_MutexNew("CRYPM_Mutex")
#define OSEXT_NfcMutexTransferNew()               OSEXT_MutexNew("NFC_MutexTransfer")
#define OSEXT_NfcMutexNew()                       OSEXT_MutexNew("NFC_Mutex")
#define OSEXT_CrcMutexNew()                       OSEXT_MutexNew("CRC_Mutex")
#define OSEXT_FramioEMutexTransferNew()           OSEXT_MutexNew("FRAMIO_MutexTransfer")
#define OSEXT_StsafaEMutexTransferNew()           OSEXT_MutexNew("STSAFA_MutexTransfer")
#define OSEXT_LedMutexNew()                       OSEXT_MutexNew("LED_Mutex")
#define OSEXT_IrdaMutexRxNew()                    OSEXT_MutexNew("IRDA_MutexRx")
#define OSEXT_IrdaMutexTxNew()                    OSEXT_MutexNew("IRDA_MutexTx")
#define OSEXT_ScardMutexRxNew()                   OSEXT_MutexNew("SCARD_MutexRx")
#define OSEXT_ScardMutexTxNew()                   OSEXT_MutexNew("SCARD_MutexTx")
#define OSEXT_MdsMutexRxNew()                     OSEXT_MutexNew("MDS_MutexRx")
#define OSEXT_MdsMutexTxNew()                     OSEXT_MutexNew("MDS_MutexTx")
#define OSEXT_VmcMutexRxNew()                     OSEXT_MutexNew("VMC_MutexRx")
#define OSEXT_VmcMutexTxNew()                     OSEXT_MutexNew("VMC_MutexTx")
#define OSEXT_PeriMutexTxNew()                    OSEXT_MutexNew("PERI_MutexTx")
#define OSEXT_PeriMutexRxNew()                    OSEXT_MutexNew("PERI_MutexRx")
#define OSEXT_MdbrdMutexNew()                     OSEXT_MutexNew("MDBRD_Mutex")
#define OSEXT_MdbrcMutexNew()                     OSEXT_MutexNew("MDBRC_Mutex")
#define OSEXT_TmrMutexNew()                       OSEXT_MutexNew("TMR_Mutex")
#define OSEXT_FevaMutexNew()                      OSEXT_MutexNew("FEVA_Mutex")
#define OSEXT_RfmMutexNew()                       OSEXT_MutexNew("RFM_Mutex")
#define OSEXT_CanMutexRxNew()                     OSEXT_MutexNew("CAN_MutexRx")
#define OSEXT_CanMutexTxNew()                     OSEXT_MutexNew("CAN_MutexTx")
#define OSEXT_ShellMutexRxNew()                   OSEXT_MutexNew("SHELL_MutexRx")
#define OSEXT_ShellMutexTxNew()                   OSEXT_MutexNew("SHELL_MutexTx")
#define OSEXT_ProtMutexNew()                      OSEXT_MutexNew("PROT_Mutex")

// event flags definitions                                             Name
#define OSEXT_SdioEventTxNew()                    OSEXT_EventFlagsNew("SDIO_EventTx")
#define OSEXT_SdioEventRxNew()                    OSEXT_EventFlagsNew("SDIO_EventRx")
#define OSEXT_LcdEventTransferNew()               OSEXT_EventFlagsNew("LCD_EventTransfer")
#define OSEXT_TftEventTransferNew()               OSEXT_EventFlagsNew("TFT_EventTransfer")
#define OSEXT_BleEventTransferNew()               OSEXT_EventFlagsNew("BLE_EventTransfer")
#define OSEXT_BleEventFlagsNew()                  OSEXT_EventFlagsNew("BLE_EventFlags")
#define OSEXT_HashmEventNew()                     OSEXT_EventFlagsNew("HASHM_Event")
#define OSEXT_NfcEventTransferNew()               OSEXT_EventFlagsNew("NFC_EventTransfer")
#define OSEXT_bceT4TEventNew()                    OSEXT_EventFlagsNew("HceT4T")
#define OSEXT_bmtEventNew()                       OSEXT_EventFlagsNew("mtEvent")
#define OSEXT_bmcEventNew()                       OSEXT_EventFlagsNew("mcEvent")
#define OSEXT_bLlcpEventNew()                     OSEXT_EventFlagsNew("Llcp Event")
#define OSEXT_HalEventNew()                       OSEXT_EventFlagsNew("Hal")
#define OSEXT_UsbfsEventNew()                     OSEXT_EventFlagsNew("USBFS_Event")
#define OSEXT_UsbhsEventNew()                     OSEXT_EventFlagsNew("USBHS_Event")
#define OSEXT_FramioEventNew()                    OSEXT_EventFlagsNew("FRAMIO_EventTransfer")
#define OSEXT_StsafaEventNew()                    OSEXT_EventFlagsNew("STSAFA_EventTransfer")
#define OSEXT_IrdaEventTxNew()                    OSEXT_EventFlagsNew("IRDA_EventTx")
#define OSEXT_IrdaEventRxNew()                    OSEXT_EventFlagsNew("IRDA_EventRx")
// #define OSEXT_ScardEventTxNew()                   OSEXT_EventFlagsNew("SCARD_EventTx")
// #define OSEXT_ScardEventRxNew()                   OSEXT_EventFlagsNew("SCARD_EventRx")
#define OSEXT_MdsEventTxNew()                     OSEXT_EventFlagsNew("MDS_EventTx")
#define OSEXT_MdsEventRxNew()                     OSEXT_EventFlagsNew("MDS_EventRx")
#define OSEXT_VmcEventTxNew()                     OSEXT_EventFlagsNew("VMC_EventTx")
#define OSEXT_VmcEventRxNew()                     OSEXT_EventFlagsNew("VMC_EventRx")
#define OSEXT_PeriEventTxNew()                    OSEXT_EventFlagsNew("PERI_EventTx")
#define OSEXT_PeriEventRxNew()                    OSEXT_EventFlagsNew("PERI_EventRx")
#define OSEXT_MdbcgEventNew()                     OSEXT_EventFlagsNew("MDBCG_Event")
#define OSEXT_CanEventTxNew()                     OSEXT_EventFlagsNew("CAN_EventTx")
#define OSEXT_CanEventRxNew()                     OSEXT_EventFlagsNew("CAN_EventRx")
#define OSEXT_SelectionEventNew()                 OSEXT_EventFlagsNew("RFM_Selection")
#define OSEXT_PricelistEventNew()                 OSEXT_EventFlagsNew("RFM_Pricelist")
#define OSEXT_RfmSigmaEventNew()                  OSEXT_EventFlagsNew("RFM_Sigma")

#ifdef __TG000
  #define OSEXT_ShellEventTxNew()                 OSEXT_EventFlagsNew("SHELL_EventTx")
  #define OSEXT_ShellEventRxNew()                 OSEXT_EventFlagsNew("SHELL_EventRx")
#else
  #define OSEXT_ShellEventNew()                   OSEXT_EventFlagsNew("SHELL")
#endif

// Timer Definitions                                                     Type                   Name
#define OSEXT_BtnTimerNew(func, arg)                OSEXT_TimerNew(func, osTimerPeriodic, arg, "BTN_Timer")
#define OSEXT_CbxTimerNew(func, arg)                OSEXT_TimerNew(func, osTimerPeriodic, arg, "CBX_Timer")
#define OSEXT_LedTimerNew(func, arg)                OSEXT_TimerNew(func, osTimerPeriodic, arg, "LED_Timer")
#define OSEXT_PeriTimerNew(func, arg)               OSEXT_TimerNew(func, osTimerPeriodic, arg, "PERI_Timer")
#define OSEXT_LLCPSymTimerNew(func, arg)            OSEXT_TimerNew(func, osTimerOnce,     arg, "LLCPSymTimer")
#define OSEXT_LLCPLtoTimerNew(func, arg)            OSEXT_TimerNew(func, osTimerOnce,     arg, "LLCPLtoTimer")
#define OSEXT_NxpTimerNew(func, arg)                OSEXT_TimerNew(func, osTimerOnce,     arg, "Nxp_Timer")
#define OSEXT_NxpPollTimerNew(func, arg)            OSEXT_TimerNew(func, osTimerOnce,     arg, "Nxp_PollTimer")
#define OSEXT_VmcTimerNew(func, arg)                OSEXT_TimerNew(func, osTimerOnce,     arg, "VMC_Timer")
#define OSEXT_ClhTimerNew(func, arg)                OSEXT_TimerNew(func, osTimerOnce,     arg, "CLH_Timer")
#define OSEXT_OtrxTimerNew(func, arg)               OSEXT_TimerNew(func, osTimerOnce,     arg, "OTRX_Timer")
#define OSEXT_VendLockCheckTimerNew(func, arg)      OSEXT_TimerNew(func, osTimerOnce,     arg, "VendLock_Timer")
#define OSEXT_QrcSelectionTimerNew(func, arg)       OSEXT_TimerNew(func, osTimerOnce,     arg, "QRC_Selection_Timer")

#define OSEXT_MdbrdDev1JustResetTimerNew(func, arg) OSEXT_TimerNew(func, osTimerOnce,     arg, "MDBRD_DEV1_JustReset_Timer")
#define OSEXT_MdbrdDev2JustResetTimerNew(func, arg) OSEXT_TimerNew(func, osTimerOnce,     arg, "MDBRD_DEV2_JustReset_Timer")

#define OSEXT_MdbcgTubeStatusTimerNew(func, arg)    OSEXT_TimerNew(func, osTimerOnce,     arg, "MDBCG_TubeStatus_Timer")
#define OSEXT_MdbcgTubeStatDelayTimerNew(func, arg) OSEXT_TimerNew(func, osTimerOnce,     arg, "MDBCG_TubeStatus_Delay_Timer")
#define OSEXT_MdbcgJustResetTimerNew(func, arg)     OSEXT_TimerNew(func, osTimerOnce,     arg, "MDBCG_Just_Reset_Timer")

#define OSEXT_MdbbvWaitJustResetTimerNew(func, arg) OSEXT_TimerNew(func, osTimerOnce,     arg, "MDBBV_WaitJustReset_Timer")

#define OSEXT_MdbsSyncTimerNew(func, arg)           OSEXT_TimerNew(func, osTimerOnce,     arg, "MDBS_Sync_Timer")
#define OSEXT_MdbsTxFrameTimerNew(func, arg)        OSEXT_TimerNew(func, osTimerOnce,     arg, "MDBS_TxFrame_Timer")
#define OSEXT_MdbsFtlNoRespTimerNew(func, arg)      OSEXT_TimerNew(func, osTimerOnce,     arg, "MDBS_FtlNoResponse_Timer")
#define OSEXT_MdbsJustResetTimerNew(func, arg)      OSEXT_TimerNew(func, osTimerOnce,     arg, "MDBS_JustReset_Timer")
#define OSEXT_MdbsSkipStartupTimerNew(func, arg)    OSEXT_TimerNew(func, osTimerOnce,     arg, "MDBS_SkipStartup_Timer")
#define OSEXT_MdbsDummySessionTimerNew(func, arg)   OSEXT_TimerNew(func, osTimerOnce,     arg, "MDBS_DummySession_Timer")
#define OSEXT_MdbsDispenseToutTimerNew(func, arg)   OSEXT_TimerNew(func, osTimerOnce,     arg, "MDBS_Dispense_Timeout_Timer")
#define OSEXT_MdbsNoConnectionTimerNew(func, arg)   OSEXT_TimerNew(func, osTimerOnce,     arg, "MDBS_NoConnection_Timer")
#define OSEXT_MdbsNoResponseTimerNew(func, arg)     OSEXT_TimerNew(func, osTimerOnce,     arg, "MDBS_NoResponse_Timer")
#define OSEXT_MdbsNoAckTimerNew(func, arg)          OSEXT_TimerNew(func, osTimerOnce,     arg, "MDBS_NoAck_Timer")
#define OSEXT_MdbsExactChangeTimerNew(func, arg)    OSEXT_TimerNew(func, osTimerOnce,     arg, "MDBS_ExactChange_Timer")

#define OSEXT_UihIdleTimerNew(func, arg)            OSEXT_TimerNew(func, osTimerOnce,     arg, "UIH_Idle_Timer")
#define OSEXT_UihApplTimerNew(func, arg)            OSEXT_TimerNew(func, osTimerOnce,     arg, "UIH_Appl_Timer")
#define OSEXT_UihClearUserInfoTimerNew(func, arg)   OSEXT_TimerNew(func, osTimerOnce,     arg, "UIH_Clear_User_Info_Timer")
#define OSEXT_UihClearTechInfoTimerNew(func, arg)   OSEXT_TimerNew(func, osTimerOnce,     arg, "UIH_Clear_Tech_Info_Timer")

#define OSEXT_ExemProdInfoTimerNew(func, arg)       OSEXT_TimerNew(func, osTimerOnce,     arg, "EXEM_ProdInfo_Timer")
#define OSEXT_ExemTransactionTimerNew(func, arg)    OSEXT_TimerNew(func, osTimerOnce,     arg, "EXEM_Transaction_Timer")
#define OSEXT_ExemDispTimeoutTimerNew(func, arg)    OSEXT_TimerNew(func, osTimerOnce,     arg, "EXEM_Dispense_Timeout_Timer")

#define OSEXT_ExesTransTimeoutTimerNew(func, arg)   OSEXT_TimerNew(func, osTimerOnce,     arg, "EXES_Transaction_Timeout_Timer")

#define OSEXT_CshCashClearTimerNew(func, arg)       OSEXT_TimerNew(func, osTimerOnce,     arg, "CSH_Cash_Clear_Timer")
#define OSEXT_CshRetLevClearTimerNew(func, arg)     OSEXT_TimerNew(func, osTimerOnce,     arg, "CSH_Return_Lever_Clear_Notification_Timer")

#define OSEXT_AppStmWdgTimerNew(func, arg)          OSEXT_TimerNew(func, osTimerOnce,     arg, "AppStm_Watchdog_Timer")
#define OSEXT_AppStmUkeyAlarmTimerNew(func, arg)    OSEXT_TimerNew(func, osTimerOnce,     arg, "AppStm_Ukey_Alarm_Timer")
#define OSEXT_AppStmChgFillTimerNew(func, arg)      OSEXT_TimerNew(func, osTimerOnce,     arg, "AppStm_ChangeGiver_Fill_Timer")
#define OSEXT_AppStmServiceModeTimerNew(func, arg)  OSEXT_TimerNew(func, osTimerOnce,     arg, "AppStm_Service_Mode_Timer")
#define OSEXT_AppStmAcfgSetWaitTimerNew(func, arg)  OSEXT_TimerNew(func, osTimerOnce,     arg, "AppStm_AutoConfig_SetWait_Timer")
#define OSEXT_AppStmCCCCTryAgainTimerNew(func, arg) OSEXT_TimerNew(func, osTimerOnce,     arg, "AppStm_CCCC_TryAgain_Timer")

#define OSEXT_DcmpDeleteTaskTimerNew(func, arg)     OSEXT_TimerNew(func, osTimerOnce,     arg, "DCMP_DeleteTask_Timer")

// semaphore definitions                                               Count    Initial  Name
#define OSEXT_PeriSemaphoreTxNew()                  OSEXT_SemaphoreNew(1,       1,      "PERI_SemaphoreTx")
#define OSEXT_bLlcpxSemaphoreNew()                  OSEXT_SemaphoreNew(1,       1,      "LlcpxSema")
#define OSEXT_bLlcpxRxSemaphoreNew()                OSEXT_SemaphoreNew(1,       1,      "LlcpxRxSema")
#define OSEXT_bToolsxSemaphoreNew()                 OSEXT_SemaphoreNew(10,      0,      "ToolsxSema")
#define OSEXT_VmcSemaphoreTxNew()                   OSEXT_SemaphoreNew(1,       1,      "VMC_Tx")

// memory pool definitions                                              Block Count   Block Size        Name
#define OSEXT_ExesMemoryPoolNew()                   OSEXT_MemoryPoolNew(5,            sizeof(t_Msg),   "Exes_MemoryPool")


/*== Enumerations ============================================================*/


/*== Structures ==============================================================*/
#ifdef RTE_CMSIS_RTOS2
  // wrapped OS enums
  #define e_OsextStatus              osStatus_t
  #define e_OsextKernelState         osKernelState_t
  #define e_OsextThreadState         osThreadState_t
  #define e_OsextPriority            osPriority_t
  #define e_OsextTimerType           osTimerType_t


  // wrapper OS structure pointers
  #define pt_OsextThreadId           osThreadId_t
  #define pt_OsextMutexId            osMutexId_t
  #define pt_OsextMessageQueueId     osMessageQueueId_t
  #define pt_OsextTimerId            osTimerId_t
  #define pt_OsextEventFlagId        osEventFlagsId_t
  #define pt_OsextSemaphoreId        osSemaphoreId_t
  #define pt_OsextMemoryPoolId       osMemoryPoolId_t

  // wrapped OS callback functions
  #define pf_OsextThreadFunction     osThreadFunc_t
  #define pf_OsextTimerFunction      osTimerFunc_t

  // warpped OS structures
  #define t_OsextVersion             osVersion_t
#else
  typedef struct
  {
    osThreadId      ptID;
    char           *pcName;
    osThreadDef_t   osThreadDef;
  } t_OsextV1Thread;

  typedef struct
  {
    osTimerId      ptID;
    char          *pcName;
    uint32         aulBuffer[6];
    osTimerDef_t   osTimerDef;
  } t_OsextV1Timer;

  typedef struct
  {
    osMessageQDef_t   osMessageQDef;
    osMessageQId      osMsgQueue;
    char             *pcName;
    uint32            ulSize;
    uint32            ulCount;
    uint32            ulPendingMessages;
  } t_OsextV1MessageQueue;

  typedef struct
  {
    t_OsextV1MessageQueue  *ptMsgQueue;
    uint32                  ulFlags;
  } t_OsextV1EventFlag;

  typedef struct
  {
    osMutexDef_t   osMutexDef;
    osMutexId      osMutex;
    char          *pcName;
    osThreadId     ptOwner;
  } t_OsextV1Mutex;

  // wrapped OS enums
  #define e_OsextStatus              osStatus
  #define e_OsextKernelState         osStatus
  #define e_OsextThreadState         osStatus
  #define e_OsextPriority            osPriority
  #define e_OsextTimerType           uint32

  // wrapper OS structure pointers
  #define pt_OsextThreadId           t_OsextV1Thread*
  #define pt_OsextMutexId            t_OsextV1Mutex*
  #define pt_OsextMessageQueueId     t_OsextV1MessageQueue*
  #define pt_OsextTimerId            t_OsextV1Timer*
  #define pt_OsextEventFlagId        t_OsextV1EventFlag*
  #define pt_OsextSemaphoreId        osSemaphoreId
  #define pt_OsextMemoryPoolId       osPoolId

  // wrapped OS callback functions
//  #define pf_OsextThreadFunction     os_pthread
  typedef void (*pf_OsextThreadFunction) (void*);
  
  #define pf_OsextTimerFunction      os_ptimer

  // warpped OS structures
  #define t_OsextVersion             uint32
  
  #define osFlagsError                0
  #define osFlagsWaitAny        0x00000001U ///< Wait for any flag (default).
  #define osFlagsWaitAll        0x00000002U ///< Wait for all flags.
  #define osFlagsNoClear        0x00000004U ///< Do not clear flags which have been specified to wait for.
#endif


/*== Module Interfaces =======================================================*/


/*----------------------------------------------------------------------------*/
/** @brief      Creates a new timer

    @param[in]  pfFunc          tart address of a timer call back function.
    @param[in]  eType           osTimerOnce for one-shot or osTimerPeriodic for periodic behavior.
    @param[in]  pArg            argument to the timer call back function.
    @param[in]  c_pcName        name of timer
    @return     ID of created timer or NULL in case of error

    @author     S.Schüpbach                                 @date 28.04.2017  */
/*----------------------------------------------------------------------------*/
pt_OsextTimerId OSEXT_TimerNew(pf_OsextTimerFunction pfFunc, e_OsextTimerType eType, void *pArg, const char *c_pcName);

/*----------------------------------------------------------------------------*/
/** @brief      Creates a new event flag

    @param[in]  c_pcName        name of event flag
    @return     ID of created event flag or NULL in case of error

    @author     S.Schüpbach                                 @date 28.04.2017  */
/*----------------------------------------------------------------------------*/
pt_OsextEventFlagId OSEXT_EventFlagsNew(const char *c_pcName);

/*----------------------------------------------------------------------------*/
/** @brief      Creates a new semaphore

    @param[in]  ulMaxCount       maximum number of available tokens
    @param[in]  ulInitialCount   initial number of available tokens
    @param[in]  c_pcName         name of event flag
    @return     ID of created semaphore or NULL in case of error

    @author     S.Schüpbach                                 @date 28.04.2017  */
/*----------------------------------------------------------------------------*/
pt_OsextSemaphoreId OSEXT_SemaphoreNew(uint32 ulMaxCount, uint32 ulInitialCount, const char *c_pcName);

/*----------------------------------------------------------------------------*/
/** @brief      Creates a new memory pool

    @param[in]  ulBlockCount    maximum number of memory blocks in memory pool
    @param[in]  ulBlockSize     memory block size in bytes
    @param[in]  c_pcName        name of event flag
    @return     ID of created memory pool or NULL in case of error

    @author     S.Schüpbach                                 @date 28.04.2017  */
/*----------------------------------------------------------------------------*/
pt_OsextMemoryPoolId OSEXT_MemoryPoolNew(uint32 ulBlockCount, uint32 ulBlockSize, const char *c_pcName);

/*----------------------------------------------------------------------------*/
/** @brief      Creates a new message box

    @param[in]  ulMessageCount  number of messages
    @param[in]  ulMessageSize   size of single message
    @param[in]  c_pcName        name of message box
    @return     ID of created message box or NULL in case of error

    @author     S.Schüpbach                                 @date 28.04.2017  */
/*----------------------------------------------------------------------------*/
pt_OsextMessageQueueId OSEXT_MessageQueueNew(uint32 ulMessageCount, uint32 ulMessageSize, const char *c_pcName);

/*----------------------------------------------------------------------------*/
/** @brief      Creates a new mutex

    @param[in]  c_pcName        name of mutex
    @return     ID of created mutex or NULL in case of error

    @author     S.Schüpbach                                 @date 28.04.2017  */
/*----------------------------------------------------------------------------*/
pt_OsextMutexId OSEXT_MutexNew(const char *c_pcName);

/*----------------------------------------------------------------------------*/
/** @brief      Creates a new thread

    @param[in]  pfFunc        Thread Function
    @param[in]  pArg          Thread Function Argument
    @param[in]  c_pcName      Thread Name
    @param[in]  ulStackSize   Stack Size
    @param[in]  ucPriority    Priority Level

    @return     ID of created thread or NULL in case of error

    @author     S.Schüpbach                                 @date 28.04.2017  */
/*----------------------------------------------------------------------------*/
pt_OsextThreadId OSEXT_ThreadNew(pf_OsextThreadFunction pfFunc, void *pArg, const char *c_pcName, uint32 ulStackSize, uint8 ucPriority);

/*----------------------------------------------------------------------------*/
/** @brief      Create a new Thread using a CCM Memory Block for Stack [Size
                is declared in Function Implementation, as it must be declared
                by __attribute__ and cannot be allocated dynamically]

    @param[in]  pfFunc        Thread Function
    @param[in]  pArg           Thread Function Argument
    @param[in]  c_pcName      Thread Name
    @param[in]  ucPriority    Priority Level

    @return     Thread ID / ASSERT in case of Error                           */
/*----------------------------------------------------------------------------*/
pt_OsextThreadId OSEXT_ThreadCCM(pf_OsextThreadFunction pfFunc, void *pArg, const char *c_pcName, uint8 ucPriority);

/*----------------------------------------------------------------------------*/
/** @brief      Initialize OSEXT Module. Currently, add only Shell Command,
                if enabled.

    @param      None
    @return     None

    @author     A.Bader                                     @date 05.06.2014  */
/*----------------------------------------------------------------------------*/
void OSEXT_Init(void);


/*----------------------------------------------------------------------------*/
// -- Functions, which are wrapped directly, without modifications
/*----------------------------------------------------------------------------*/

//  ==== Kernel Management Functions ====
e_OsextStatus OSEXT_KernelInitialize(void);
uint8 OSEXT_KernelIsRunning(void);
e_OsextStatus OSEXT_KernelGetInfo(t_OsextVersion *ptVersion, char *pcBuf, uint32 ulSize);
e_OsextKernelState OSEXT_KernelGetState(void);
e_OsextStatus OSEXT_KernelStart(void);
int32 OSEXT_KernelLock(void);
int32 OSEXT_KernelUnlock(void);
int32 OSEXT_KernelRestoreLock(int32 lLock);
uint32 OSEXT_KernelSuspend(void);
void OSEXT_KernelResume(uint32 ulSleepTicks);
uint64 OSEXT_KernelGetTickCount(void);
uint32 OSEXT_KernelGetTickFreq(void);
uint32 OSEXT_KernelGetSysTimerCount (void);
uint32 OSEXT_KernelGetSysTimerFreq(void);

//  ==== Thread Management Functions ====
const char *OSEXT_ThreadGetName (pt_OsextThreadId ptThreadId);
pt_OsextThreadId OSEXT_ThreadGetId(void);
e_OsextThreadState OSEXT_ThreadGetState (pt_OsextThreadId ptThreadId);
uint32 OSEXT_ThreadGetStackSize(pt_OsextThreadId ptThreadId);
uint32 OSEXT_ThreadGetStackSpace(pt_OsextThreadId ptThreadId);
e_OsextStatus OSEXT_ThreadSetPriority(pt_OsextThreadId ptThreadId, e_OsextPriority ePriority);
e_OsextPriority OSEXT_ThreadGetPriority(pt_OsextThreadId ptThreadId);
e_OsextStatus OSEXT_ThreadYield(void);
e_OsextStatus OSEXT_ThreadSuspend(pt_OsextThreadId ptThreadId);
e_OsextStatus OSEXT_ThreadResume(pt_OsextThreadId ptThreadId);
e_OsextStatus OSEXT_ThreadDetach(pt_OsextThreadId ptThreadId);
e_OsextStatus OSEXT_ThreadJoin(pt_OsextThreadId ptThreadId);
void OSEXT_ThreadExit(void);
e_OsextStatus OSEXT_ThreadTerminate(pt_OsextThreadId ptThreadId);
uint32 OSEXT_ThreadGetCount(void);
uint32 OSEXT_ThreadEnumerate(pt_OsextThreadId *pptThreadId, uint32 ulNumItems);

//  ==== Thread Flags Functions ====
uint32 OSEXT_ThreadFlagsSet(pt_OsextThreadId ptThreadId, uint32 ulFlags);
uint32 OSEXT_ThreadFlagsClear(uint32 ulFlags);
uint32 OSEXT_ThreadFlagsGet(void);
uint32 OSEXT_ThreadFlagsWait(uint32 ulFlags, uint32 ulOptions, uint32 ulTimeout);

//  ==== Generic Wait Functions ====
e_OsextStatus OSEXT_Delay(uint32 ulTicks);
e_OsextStatus OSEXT_DelayUntil(uint64 uqTicks);

//  ==== Timer Management Functions ====
const char *OSEXT_TimerGetName(pt_OsextTimerId ptTimerId);
e_OsextStatus OSEXT_TimerStart(pt_OsextTimerId ptTimerId, uint32 ulTicks);
e_OsextStatus OSEXT_TimerStop(pt_OsextTimerId ptTimerId);
uint32 OSEXT_TimerIsRunning(pt_OsextTimerId ptTimerId);
e_OsextStatus OSEXT_TimerDelete(pt_OsextTimerId ptTimerId);

//  ==== Event Flags Management Functions ====
const char *OSEXT_EventFlagsGetName(pt_OsextEventFlagId ptEventFlagId);
uint32 OSEXT_EventFlagsSet(pt_OsextEventFlagId ptEventFlagId, uint32 ulFlags);
uint32 OSEXT_EventFlagsClear(pt_OsextEventFlagId ptEventFlagId, uint32 ulFlags);
uint32 OSEXT_EventFlagsGet(pt_OsextEventFlagId ptEventFlagId);
uint32 OSEXT_EventFlagsWait(pt_OsextEventFlagId ptEventFlagId, uint32 ulFlags, uint32 ulOptions, uint32 ulTimeout);
e_OsextStatus OSEXT_EventFlagsDelete(pt_OsextEventFlagId ptEventFlagId);

//  ==== Mutex Management Functions ====
const char *OSEXT_MutexGetName(pt_OsextMutexId ptMutexId);
e_OsextStatus OSEXT_MutexAcquire(pt_OsextMutexId ptMutexId, uint32 ulTimeout);
e_OsextStatus OSEXT_MutexRelease(pt_OsextMutexId ptMutexId);
pt_OsextThreadId OSEXT_MutexGetOwner(pt_OsextMutexId ptMutexId);
e_OsextStatus OSEXT_MutexDelete(pt_OsextMutexId ptMutexId);

//  ==== Semaphore Management Functions ====
const char *OSEXT_SemaphoreGetName(pt_OsextSemaphoreId ptSemaphoreId);
e_OsextStatus OSEXT_SemaphoreAcquire(pt_OsextSemaphoreId ptSemaphoreId, uint32 ulTimeout);
e_OsextStatus OSEXT_SemaphoreRelease(pt_OsextSemaphoreId ptSemaphoreId);
uint32 OSEXT_SemaphoreGetCount(pt_OsextSemaphoreId ptSemaphoreId);
e_OsextStatus OSEXT_SemaphoreDelete(pt_OsextSemaphoreId ptSemaphoreId);

//  ==== Memory Pool Management Functions ====
const char *OSEXT_MemoryPoolGetName(pt_OsextMemoryPoolId ptMemoryPoolId);
void *OSEXT_MemoryPoolAlloc(pt_OsextMemoryPoolId ptMemoryPoolId, uint32 ulTimeout);
e_OsextStatus OSEXT_MemoryPoolFree(pt_OsextMemoryPoolId ptMemoryPoolId, void *pBlock);
uint32 OSEXT_MemoryPoolGetCapacity(pt_OsextMemoryPoolId ptMemoryPoolId);
uint32 OSEXT_MemoryPoolGetBlockSize(pt_OsextMemoryPoolId ptMemoryPoolId);
uint32 OSEXT_MemoryPoolGetCount(pt_OsextMemoryPoolId ptMemoryPoolId);
uint32 OSEXT_MemoryPoolGetSpace(pt_OsextMemoryPoolId ptMemoryPoolId);
e_OsextStatus OSEXT_MemoryPoolDelete(pt_OsextMemoryPoolId ptMemoryPoolId);

//  ==== Message Queue Management Functions ====
const char *OSEXT_MessageQueueGetName(pt_OsextMessageQueueId ptMessageQueueId);
e_OsextStatus OSEXT_MessageQueuePut(pt_OsextMessageQueueId ptMessageQueueId, const void *c_pPtr, uint8 ucMsgPrio, uint32 ulTimeout);
e_OsextStatus OSEXT_MessageQueueGet(pt_OsextMessageQueueId ptMessageQueueId, uint32 *pulMessage, uint8 *pucMsgPrio, uint32 ulTimeout);
uint32 OSEXT_MessageQueueGetCapacity(pt_OsextMessageQueueId ptMessageQueueId);
uint32 OSEXT_MessageQueueGetMsgSize (pt_OsextMessageQueueId ptMessageQueueId);
uint32 OSEXT_MessageQueueGetCount(pt_OsextMessageQueueId ptMessageQueueId);
uint32 OSEXT_MessageQueueGetSpace(pt_OsextMessageQueueId ptMessageQueueId);
e_OsextStatus OSEXT_MessageQueueReset(pt_OsextMessageQueueId ptMessageQueueId);
e_OsextStatus OSEXT_MessageQueueDelete(pt_OsextMessageQueueId ptMessageQueueId);

/// @}   // End of Group

#endif   // __OSEXT_H__
