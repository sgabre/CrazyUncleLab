/** @addtogroup   ERR  
    @{
********************************************************************************
*   @name       ERR
*   @file       ERR.c
*
*   @brief      Error Functions
*
*   @authors    S.Schuepbach
*   @date       24.07.2014
*
*   @copyright (C) 2014 by: MICROTRONIC AG
*                           Duennernstrasse 32
*                           CH-4702 Oensingen
*******************************************************************************/

/*== Includes ================================================================*/
#include "Target.h"

#include "CRC.h"
#include "ASSERT.h"
#include "DT.h"
#include "STR.h"
#include "LIST.h"
#include "I_CTRL.h"

#include "ERR.h"


/*== Defines =================================================================*/


/*== Enumerations ============================================================*/


/*== Structures ==============================================================*/

/// struct with function-pointers to uncouple modules
typedef struct
{
  pf_IsSaveMode  pfIsSaveMode;   ///< check if reader is in save mode
} t_ErrWeak;


/*== Global Variables ========================================================*/
const char  gc_acERR_ErrorLog[] = "Error.log";   ///< name of file to log fatal errors to


/*== Module Constants ========================================================*/


/*== Function Prototypes =====================================================*/
#if SHELL_ERR > 0
  static void mERR_InitShell(void);
#endif


/*== Module Variables ========================================================*/

/// struct with function pointer of uncoupled modules
static t_ErrWeak  m_tErrWeak = { NULL };

static bool       m_bDisableWritingErrorLog = false;   ///< TRUE - disable error log generation


/*== Weak Functions to uncouple Modules ======================================*/


/*============================================================================*/
/*== Internal Functions ======================================================*/
/*============================================================================*/


/*============================================================================*/
/*== Static Module Functions =================================================*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/** @brief      write error log entry to sram

    @param[in]  ptBuffer  struct with error log data

    @return     None

    @author     S.Schuepbach                                @date 25.07.2014  */
/*----------------------------------------------------------------------------*/
static void mERR_WriteToSram(t_ErrAssertBuffer *ptBuffer)
{
  time_t  tDT;

  // fill up struct and get crc over all except the 4 first bytes
  time(&tDT);
  ptBuffer->ulTimeStamp = (uint32)tDT;

  // write struct to SRAM
  I_CTRL_GetSram()->Write(SRAM_FIELD_LOG, (uint8 *)ptBuffer, sizeof(t_ErrAssertBuffer), false);
}


/*============================================================================*/
/*== Implementation of Public Functions ======================================*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/
void ERR_WriteAssertToSRAM(const char *c_pcFunc, uint32 ulLine, const char *c_pcMsg, const char *c_pcSource)
{
  // if functionality isn't disabled
  if (!m_bDisableWritingErrorLog)
  {
    t_ErrAssertBuffer  tBuffer;

    // Clear Buffer
    memset(&tBuffer, 0, sizeof(t_ErrAssertBuffer));

    // fill up with code line and information text
    tBuffer.ulParam = ulLine;
    snprintf(tBuffer.acText, sizeof(tBuffer.acText) - 1, "%s\n%s\n%s\n", c_pcFunc, c_pcSource ? c_pcSource : "Unknown", c_pcMsg);

    // set flags - Bit 0: 0 = Assert / 1 = Abort; Bit 1: 0 = TaskName / 1 = IRQName; Bit 3: 0 = No Additional Info / 1 = Additional Info
    if (gc_pacLastIrqName)
      tBuffer.ucFlags |= 2;
    if (c_pcMsg)
      tBuffer.ucFlags |= 4;

    // write data to SRAM
    mERR_WriteToSram(&tBuffer);
  }
}

/*----------------------------------------------------------------------------*/
void ERR_WriteErrorLog(char *pcFmt, ... )
{
  // If not in Safe-Mode and error log genration is enabled
  if ((m_tErrWeak.pfIsSaveMode ? (m_tErrWeak.pfIsSaveMode() != SAFEMODE_MAIN_DRIVE_PROBLEM) : true) && !m_bDisableWritingErrorLog)
  {
    // Open File for append
    FILE  *pFile = fopen (gc_acERR_ErrorLog, "a");
    
    // if file could get opened
    if (pFile)
    {
      char     acDateBuf[24];
      va_list  pArgs;

      // get time and date ascii formatted
      DT_GetDateTimeStr(acDateBuf, sizeof(acDateBuf), "%d.%m.%Y %H:%M:%S", NULL);
      fprintf(pFile, "%s / ", acDateBuf);

      // add other argument
      va_start(pArgs, pcFmt);
      vfprintf(pFile, pcFmt, pArgs);
      va_end(pArgs);

      fprintf(pFile, "\r\n");

      fclose(pFile);
    }
  }
}

/*----------------------------------------------------------------------------*/
void ERR_CheckWriteErrorLogFromSRAM(void)
{
  // Get used space in sram
  uint16  unLen = I_CTRL_GetSram()->GetUsedSpace(SRAM_FIELD_LOG);
  uint16  unTempLen = 0;

  // if some data were saved
  if (unLen)
  {
    // struct to handle sram data
    t_ErrAssertBuffer  *ptBuffer;

    // buffer for sram data
    uint8  *pucBuf = HEAP_MALLOC(unLen);

    // clear sram at end of function
    bool  bClearSram = true;

    // get sram data
    if ((m_tErrWeak.pfIsSaveMode ? !m_tErrWeak.pfIsSaveMode() : true) && I_CTRL_GetSram()->Read(SRAM_FIELD_LOG, pucBuf, &unLen, false))
    {
      t_DateTime   tDT;
      time_t       tTime;
      const char  *c_apacSplit[] = {"\n", NULL};

      // Split the text in lines to extract Function-Name, IRQ/Task-Name, Additional Text
      t_List      *ptList;
      char        *pacSource;
      FILE        *pFile;

      // while not all logs are saved in file
      while (unTempLen < unLen)
      {
        // point struct to the next entry
        ptBuffer = (t_ErrAssertBuffer *)(pucBuf + unTempLen);

        // Retrieve Time Stamp
        tTime = (time_t)ptBuffer->ulTimeStamp;

        // Split the text in lines to extract Function-Name, IRQ/Task-Name, Additional Text
        ptList = STR_split(ptBuffer->acText, c_apacSplit, 80, 5);
        pacSource = (ptBuffer->ucFlags & 2) ? "Irq" : "Task";

        // convert time stamp to date/time structure
        localtime_r(&tTime, &tDT);

        // open file
        pFile = fopen(gc_acERR_ErrorLog, "a");
        if (pFile)
        {
          // get date and time in ascii format
          char  acDateBuf[24];
          DT_GetDateTimeStr(acDateBuf, sizeof(acDateBuf), "%d.%m.%Y %H:%M:%S", &tDT);

          // check format of saved struct - Abort or Assert
          if (((ptBuffer->ucFlags & 1) == 1) && (LIST_ItemCount(ptList) == 2))
          {
            // It was an Abort
            fprintf(pFile, "%s / Abort: %s. R14=0x%X, %s:%s\r\n", acDateBuf, (char *)LIST_GetItem(ptList, 0, NULL), 
                    ptBuffer->ulParam, pacSource, (char *)LIST_GetItem(ptList, 1, NULL));
          }
          else if (((ptBuffer->ucFlags & 1) == 0) && (LIST_ItemCount(ptList) == 3))
          {
            // It was an Assert
            fprintf(pFile, "%s / ASSERT: in function: '%s' @ line %d, %s:%s\r\n", acDateBuf, (char *)LIST_GetItem(ptList, 0, NULL),
                    ptBuffer->ulParam, pacSource, (char *)LIST_GetItem(ptList, 1, NULL));

            // Additional Info available
            if (ptBuffer->ucFlags & 4)
              fprintf(pFile, "Additional Info: %s\r\n", (char *)LIST_GetItem(ptList, 2, NULL));
          }

          // close file and save state
          fclose (pFile);
          bClearSram &= true;
        }
        else
        {
          // if file couldn't get open, don't clear sram
          bClearSram = false;
        }

        // free list
        LIST_Free(ptList);

        // update temporary length
        unTempLen += sizeof(t_ErrAssertBuffer);
      }
    }
    
    // clear log entry in sram
    if (bClearSram)
      I_CTRL_GetSram()->SetWritePtr(SRAM_FIELD_LOG, 0, true);
    
    HEAP_FREE(pucBuf);
  }
}

/*----------------------------------------------------------------------------*/
void ERR_ClearLogs(void)
{
#ifdef RTE_FileSystem_Core
  fdelete(gc_acERR_ErrorLog, NULL);
#endif
}

/*----------------------------------------------------------------------------*/
void ERR_DisableWritingErrorLog(void)
{
  m_bDisableWritingErrorLog = true;
}

/*----------------------------------------------------------------------------*/
void ERR_Init(pf_IsSaveMode pfSaveMode)
{
  // save extern function pointer
  m_tErrWeak.pfIsSaveMode = pfSaveMode;

//  OSEXT_Delay(100);

  ERR_CheckWriteErrorLogFromSRAM();

#if SHELL_ERR > 0
  // register shell-command
  mERR_InitShell();
#endif
}


/*----------------------------------------------------------------------------*/
// SHELL Interface
/*----------------------------------------------------------------------------*/
#if SHELL_ERR > 0
  #include "SHELL.h"

  /*--------------------------------------------------------------------------*/
  /// this text will be displayed, in the shell-command overview 
  static const char mc_acSummaryErr[] =
    "Functional description of ERR-Shell-Modul. (Not yet implemented!)\r\n";

  /// this text will be displayed, if this module return E_SHELL_CMD_WRONG_PARAMETERS 
  static const char mc_acUsageErr[] =
    "err shell functions not implemented yet\r\n"
    "  s\r\n"
    "  \r\n"
    "\r\n"
    "Examples: 'err \r\n";
  
  /*--------------------------------------------------------------------------*/
  /** @brief      this function is called, if a registered command of this module is
                  entered in the shell

      @param[in]  c_ptContext    entered Text

      @return     ulRet          0 (No error)
                                 E_SHELL_CMD_WRONG_PARAMETERS (Parameter doesn't match)

      @author     S.Schuepbach                              @date 24.07.2014  */
  /*--------------------------------------------------------------------------*/
  static uint32 mERR_ShellErr(const t_ShellCmdContext *c_ptContext)
  {
    uint32  ulErr = 0;
    
    if (SHELL_CheckCmdParam(c_ptContext, "cmd1", 4))
    {
      // --- cmd1, 4 parameters
      // Do not use printf to have the output on the correct stream (if output of command
      // has been redirected to a file!)
      //fprintf(c_ptContext->pfOut, "Do not use 'printf' -> use fprintf...\r\n");
      // add functionality
    }
    else if (SHELL_CheckCmdParamEx(c_ptContext, "cmd2", 2, 3, 5))
    {
      // --- cmd2, 3 or 5 parameters
      // add functionality
    }
    else if (c_ptContext->ucNumParams == 0)
    {
      // grow old
    }
    else
    {
      // mc_acUsageErr will be displayed
      ulErr = E_SHELL_CMD_WRONG_PARAMETERS;    
    }
      
    return ulErr;
  }

  /*--------------------------------------------------------------------------*/
  /** @brief      register shell-commands of ERR module

      @param      None

      @return     None

      @author     S.Schuepbach                              @date 24.07.2014  */
  /*--------------------------------------------------------------------------*/
  static void mERR_InitShell(void)
  {
    // use module name as parameter or check with shell-cmd "help" that parameter isn't used
    SHELL_AddCommand("err", mERR_ShellErr, mc_acSummaryErr, mc_acUsageErr);
  }
#endif   // SHELL_ERR

/// @}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

