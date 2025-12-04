/** @addtogroup   ASSERT  
    @{
********************************************************************************
*   @name       ASSERT
*   @file       ASSERT.c
*
*   @brief      Assert implementation.
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

#include "STDIO.h"

#include "ERR.h"

#ifdef WATCHDOG_ENABLE
#include "I_CTRL.h"
#endif
#include "I_UI.h"

#include "ASSERT.h"


/*== Defines =================================================================*/
#define USER_SVC_COUNT  1       // Number of user SVC functions


/*== Enumerations ============================================================*/


/*== Structures ==============================================================*/


/*== Global Variables ========================================================*/
const char  *gc_pacLastIrqName = NULL;


/*== Module Constants ========================================================*/
const char  mc_pacFunctionUnknown[] = "Unknown";


/*== Function Prototypes =====================================================*/
void __SVC_1 (const char *c_pacFunc, uint32 ulLine, const char *c_pacMsg, const char *c_pacThreadName);

#if SHELL_ASSERT
static void mASSERT_InitShell(void);
#endif


/*== Module Variables ========================================================*/

// override weak SVC Table
extern void * const osRtxUserSVC[1 + USER_SVC_COUNT];
       void * const osRtxUserSVC[1 + USER_SVC_COUNT] =
{
  (void *)USER_SVC_COUNT,
  (void *)__SVC_1,
};

// use to prevent recursive asserts
static bool m_bInsideAssert = false;


/*== Weak Functions to uncouple Modules ======================================*/


/*============================================================================*/
/*== Static Module Functions =================================================*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/** @brief      Assert function. May be called from SVC ('Handler Mode')
   
    @param[in]  c_pacFunc         Function Name
    @param[in]  ulLine            Line Number
    @param[in]  c_pacMsg          Additional message or NULL, if none
    @param[in]  c_pacThreadName   Thread Name
    @return     No return

    @author     A.Bader                                     @date 05.06.2014  */
/*----------------------------------------------------------------------------*/
void mASSERT_Do(const char *c_pacFunc, uint32 ulLine, const char *c_pacMsg, const char *c_pacThreadName)
{
  // The size of this buffer does not matter; the stack may overflow...
  char  acTextBuf[400];
  const char *c_pcSource = NULL;

  m_bInsideAssert = true;

  // If no function name is given, use const name "UnknownFunction"
  if (!c_pacFunc)
    c_pacFunc = mc_pacFunctionUnknown;
 
  // get name of thread / IRQ, which produced assert
  c_pcSource = gc_pacLastIrqName ? gc_pacLastIrqName : c_pacThreadName;

#ifndef __BOOT
  // Write info to SRAM
  ERR_WriteAssertToSRAM(c_pacFunc, ulLine, c_pacMsg, c_pcSource);
#endif

  //I_UI_GetDisp()->DisplayText(DISP_LINE1, false, "ASSERT: in func\n%s\n@ line %4u", c_pacFunc, ulLine);

  snprintf(acTextBuf, sizeof(acTextBuf), "\r\nASSERT: In function: '%s' @ line %d, %s: %s\r\n", c_pacFunc, ulLine,
           gc_pacLastIrqName ? "Irq" : "Thread", c_pcSource ? c_pcSource : "Unknown");
  
#if STDIO_INIT == 1 
  // Do not use printf because this would wait for a Mutex which is not allowed in SVC!
  // Instead, call the function, which printf would also call after taking Mutex -> more or less direct output to UART...
  if (c_pacMsg)
  {
    snprintf(acTextBuf + strlen(acTextBuf), sizeof(acTextBuf)-1 - strlen(acTextBuf), "Additional Info: %s\r\n", c_pacMsg);
    STDIO_Send((uint8 *)acTextBuf, strlen(acTextBuf));
  }
  else
    STDIO_Send((uint8 *)acTextBuf, strlen(acTextBuf));
#endif
  
  __disable_irq();
#ifdef __DEBUG
  while (1)
  {
    // We do not want a Watchdog Reset while staying in ASSERT!
		#ifdef WATCHDOG_ENABLE
    I_CTRL_GetWdg()->Restart();
		#endif
  }
#else
  {
     uint32  ulI;
    // This loop takes around 5s with WDT_Restart() inside - this will prevent optimization of empty loop.
    // Do not use timers to be as independent as possible (timers may no more run correctly or are probably not yet initialized...)
    for (ulI = 0; ulI < 33000000; ulI++)
    {
      // We do not want a Watchdog Reset while staying in ASSERT!
      // WDG_Restart();
      I_CTRL_GetWdg()->Restart();
    }

    // Clear Signature => Restart in Boot Loader [see also startup_stm32f437xx.s]
    uint32 *pSignature = (uint32 *)0x20000000;
    *pSignature = 0x00000000;

    NVIC_SystemReset();
  }
#endif
}

/*----------------------------------------------------------------------------*/
/** @brief      Called by Hardfault handler
   
    @param[in]  pulAbortAddress     Last successful instruction
    @return     No return

    @author     S.Schüpbach                                 @date 19.12.2017  */
/*----------------------------------------------------------------------------*/
void mASSERT_HardFault(uint32 *pulAbortAddress)
{
  char acMsg[50];

  //@todo maybe add more output
//  // check forced bit
//  if (SCB->HFSR & SCB_HFSR_FORCED_Msk)
//  {
//    // mem fault
//    if (SCB->CFSR & SCB_CFSR_MEMFAULTSR_Msk)
//    {
//
//    }
//    // bus fault
//    else if (SCB->CFSR & SCB_CFSR_BUSFAULTSR_Msk)
//    {
//
//    }
//    // usage fault
//    else if (SCB->CFSR & SCB_CFSR_USGFAULTSR_Msk)
//    {
//
//    }
//  }
//  else
//  {
//
//  }

  sprintf(acMsg, "Abort: CFSR=0x%08X(forced=%d), PC=0x%08X", SCB->CFSR, !!(SCB->HFSR & SCB_HFSR_FORCED_Msk), pulAbortAddress);
  ASSERT_FAIL_MSG(acMsg);
}

/*----------------------------------------------------------------------------*/
/** @brief      Go to SVC 1 and perform Assert. NOT ALLOWED, if already in
                'Handler Mode'. Use only, if in 'Thread Mode'.
   
    @param[in]  c_pacFunc         Function Name
    @param[in]  ulLine            Line Number
    @param[in]  c_pacMsg          Additional message or NULL, if none
    @param[in]  c_pacThreadName   Thread Name
    @return     No return

    @author     A.Bader                                     @date 05.06.2014  */
/*----------------------------------------------------------------------------*/
void __svc(1) mASSERT_EnterSVC (const char *c_pacFunc, uint32 ulLine, const char *c_pacMsg, const char *c_pacThreadName);
void __SVC_1 (const char *c_pacFunc, uint32 ulLine, const char *c_pacMsg, const char *c_pacThreadName)
{
  mASSERT_Do(c_pacFunc, ulLine, c_pacMsg, c_pacThreadName);
}


/*============================================================================*/
/*== Implementation of Public Functions ======================================*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/
void ASSERT_Init(void)
{
#if SHELL_ASSERT > 0
  // register shell-command
  mASSERT_InitShell();
#endif
}

/*----------------------------------------------------------------------------*/
void Assert(const char *c_pacFunc, uint32 ulLine, const char *c_pacMsg)
{
  // prevent recursive ASSERT calls
  if (!m_bInsideAssert)
  {
    // if already in SVC / Interrupt, do not enter SVC to avoid hard fault exception
    if (!__get_IPSR())
      mASSERT_EnterSVC(c_pacFunc, ulLine, c_pacMsg, OSEXT_ThreadGetName(OSEXT_ThreadGetId()));
    else
      mASSERT_Do(c_pacFunc, ulLine, c_pacMsg, NULL);
  }
}

/*----------------------------------------------------------------------------*/
void assert_failed(const char* func, uint32 line)
{
  Assert(func, line, NULL);
}

/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
// SHELL Interface
/*----------------------------------------------------------------------------*/
#if SHELL_ASSERT > 0
  #include "SHELL.h"

  /*--------------------------------------------------------------------------*/
  /// this text will be displayed, in the shell-command overview 
  static const char mc_acSummaryAssert[] =
    "Test the ASSERT functions.\r\n";

  /// this text will be displayed, if this module return E_SHELL_CMD_WRONG_PARAMETERS 
  static const char mc_acUsageAssert[] =
    "assert <param> | ['fail'] [<msg>]\r\n"
    "  <param>: to check against 0 for ASSERT condition\r\n"
    "  instead of <param> 'fail' can be passed to force ASSERT_FAIL\r\n"
    "  <msg> (optional): to call either ASSERT_MSG or ASSERT_FAIL_MSG with\r\n"
    "                    given message.\r\n"
    "\r\n"
    "Examples:\r\n"
    "  'assert 1' -> ASSERT(1)\r\n"
    "  'assert 0' -> ASSERT(0)\r\n"
    "  'assert 0 \"Test Assert!\"' -> ASSERT_MSG(0, ..)\r\n"
    "  'assert fail' -> ASSERT_FAIL()\r\n"
    "  'assert fail \"Test Assert!\"' -> ASSERT_FAIL_MSG(..)\r\n";
  
  /*--------------------------------------------------------------------------*/
  /** @brief      this function is called, if a registered command of this module is
                  entered in the shell
   
      @param[in]  c_ptContext    entered Text
      @return     ulRet          0 (No error)
                                 E_SHELL_CMD_WRONG_PARAMETERS (Parameter doesn't match)
      @author     A.Bader                                   @date 05.06.2014  */
  /*--------------------------------------------------------------------------*/
  static uint32 mASSERT_ShellAssert(const t_ShellCmdContext *c_ptContext)
  {
    uint32  ulErr = 0;
    
    if (SHELL_CheckCmdParam(c_ptContext, "fail", 1))
    {
      ASSERT_FAIL();
    }
    else if (SHELL_CheckCmdParam(c_ptContext, "fail", 2))
    {
      ASSERT_FAIL_MSG((char *)LIST_GetItem(c_ptContext->ptParams, 1, NULL));
    }
    else if ( c_ptContext->ucNumParams == 1)
    {
      ASSERT(atoi((char *)LIST_GetItem(c_ptContext->ptParams, 0, NULL)));
    }
    else if ( c_ptContext->ucNumParams == 2)
    {
      ASSERT_MSG(atoi((char *)LIST_GetItem(c_ptContext->ptParams, 0, NULL)), (char *)LIST_GetItem(c_ptContext->ptParams, 1, NULL));
    }
    else
      ulErr = E_SHELL_CMD_WRONG_PARAMETERS;
      
    return ulErr;
  }

  /*--------------------------------------------------------------------------*/
  /** @brief      register shell-commands of ASSERT-Module
   
      @param      None
      @return     None

      @author     A.Bader                                   @date 05.06.2014  */
  /*--------------------------------------------------------------------------*/
  static void mASSERT_InitShell(void)
  {
    SHELL_AddCommand("assert", mASSERT_ShellAssert, mc_acSummaryAssert, mc_acUsageAssert);
  }

#endif // SHELL_ASSERT

/// @}
/// @}
  
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
