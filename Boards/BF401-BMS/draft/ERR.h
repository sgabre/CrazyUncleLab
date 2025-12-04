#ifndef __ERR_H__
#define __ERR_H__

/** @defgroup   ERR  ERR-Group
                Error Handler
    @{
********************************************************************************
*   @name       ERR
*   @file       ERR.h
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


/*== Defines =================================================================*/

/*----------------------------------------------------------------------------*/
/** @brief      Check if reader is started up in save-mode

    @param      None

    @return     0 if success, else error code                                 */
/*----------------------------------------------------------------------------*/
typedef uint8 (*pf_IsSaveMode) (void);
  
  
/*== Enumerations ============================================================*/


/*== Structures ==============================================================*/

/// Structure needs to be a multiply of 4, because of crc32 check
__packed typedef struct
{
  uint32  ulTimeStamp;
  uint8   ucFlags;       ///< Bit 0: 0=Assert/1=Abort; Bit 1: 0=TaskName/1=IRQName; Bit 3: 0=No AdditionInfo/1=Additional-Info
  uint32  ulParam;       ///< Assert: Line-No / Abort: R14
  char    acText[103];   ///< Assert: <Function>\n<Taskname/Irqname>\r\nAdditionalInfo\n\0 Abort: <Type of Abort>\n\<Taskname/Irqname>\n\0
} t_ErrAssertBuffer;


/*== Internal Functions ======================================================*/


/*== Module Interfaces =======================================================*/

/*----------------------------------------------------------------------------*/
/** @brief      Writes an assert log to SRAM

    @param[in]  c_pcFunc    Name of function which called ASSERT()
    @param[in]  ulLine      Code Line which called ASSERT()
    @param[in]  c_pcMsg     Additional message, could be NULL
    @param[in]  c_pcSource  Thread- or IRQ-Name 

    @return     None

    @author     S.Schuepbach                                @date 24.07.2014  */
/*----------------------------------------------------------------------------*/
void ERR_WriteAssertToSRAM(const char *c_pcFunc, uint32 ulLine, const char *c_pcMsg, const char *c_pcSource);

/*----------------------------------------------------------------------------*/
/** @brief      Writes an abort log to SRAM
   
    @param[in]  c_pcStr  String to declare source
    @param[in]  ulR14    contains return address

    @return     None

    @author     S.Schuepbach                                @date 24.07.2014  */
/*----------------------------------------------------------------------------*/
void ERR_WriteExceptionToSRAM(const char *c_pcStr, uint32 ulR14);

/*----------------------------------------------------------------------------*/
/** @brief      Writes an error log to file
   
    @param[in]  pcFmt  argumentslist
    @param[in]  ...    arguments

    @return     None

    @author     S.Schuepbach                                @date 24.07.2014  */
/*----------------------------------------------------------------------------*/
void ERR_WriteErrorLog(char *pcFmt, ... );

/*----------------------------------------------------------------------------*/
/** @brief      Check, if there is a stored message of an Assert/Abort in the SRAM,
                which needs to be copied to error.log.
   
    @param      None

    @return     None

    @author     S.Schuepbach                                @date 24.07.2014  */
/*----------------------------------------------------------------------------*/
void ERR_CheckWriteErrorLogFromSRAM(void);

/*----------------------------------------------------------------------------*/
/** @brief      Delete "*.log" ("Error.log")
   
    @param      None

    @return     None

    @author     S.Schuepbach                                @date 24.07.2014  */
/*----------------------------------------------------------------------------*/
void ERR_ClearLogs(void);
  
/*----------------------------------------------------------------------------*/
/** @brief      If g_tRFM.tParam.tSystem.ulMISC_ProtocolLevel = 0, we also do not need
                to write to error.log!
   
    @param      None

    @return     None

    @author     S.Schuepbach                                @date 24.07.2014  */
/*----------------------------------------------------------------------------*/
void ERR_DisableWritingErrorLog(void);

/*----------------------------------------------------------------------------*/
/** @brief      Initialize ERR module 

    @param[in]  pfSaveMode   Check if reader is start up in save-mode
    @return     None

    @author     S.Schuepbach                                @date 24.07.2014  */
/*----------------------------------------------------------------------------*/
void ERR_Init(pf_IsSaveMode pfSaveMode);

/// @}   // end of Group

#endif   // __ERR_H__
