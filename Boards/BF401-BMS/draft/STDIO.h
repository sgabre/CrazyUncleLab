#ifndef __STDIO_H__
#define __STDIO_H__

/** @defgroup   STDIO  STDIO-Group
                initializes standard input / output on the
                device configured in ProjectConfig.h
    @{
********************************************************************************
*   @name       STDIO                                                           
*   @file       STDIO.h                                                         
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


/*== Defines =================================================================*/
#if STDIO_SOURCE & 1
  #define STDIO_Async(...)       STDIO_AsyncPrintf(1, __VA_ARGS__)
#else
  #define STDIO_Async(...)       ((void)0)
#endif

#if STDIO_SOURCE & 2
  #define STDIO_AsyncProt(...)   STDIO_AsyncPrintf(2, __VA_ARGS__)
#else
  #define STDIO_AsyncProt(...)   ((void)0)
#endif


/*== Enumerations ============================================================*/


/*== Structures ==============================================================*/


/*== Module Interfaces =======================================================*/

/*----------------------------------------------------------------------------*/
/** @brief      Interface function, which is typically called from _sys_write
                (fs_config.h) in case a write to stdout shall be performed
                (e.g. printf, ...) However, it may also be called directly,
                e.g. from ASSERT to avoid Mutex-Handling (printf will wait
                for Mutex internally).
   
    @param[in]  nFH        File Handle. Ignored.
    @param[in]  c_paucBuf  Buffer to send
    @param[in]  ulLen      Data length in Buffer
    @return     number of bytes NOT written | error flags -> see _sys_write

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
//int io_std_write (int nFH, const unsigned char *c_paucBuf, unsigned int ulLen);
int32 STDIO_Send(uint8 *paucBuf, int32 lLen);

/*----------------------------------------------------------------------------*/
/** @brief      Interface function, which is typically called from _sys_read
                (fs_config.h) in case a read from stdin is performed
                (e.g. scanf, ...).
   
    @param[in]  nFH        File Handle. Ignored.
    @param[out] c_paucBuf  Buffer to fill
    @param[in]  ulLen      Data length to read
    @return     number of bytes NOT read | error flags -> see _sys_read

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
//int io_std_read (int nFH, unsigned char *paucBuf, unsigned int ulLen);
int stdin_getchar(void);

/*----------------------------------------------------------------------------*/
/** @brief      Sends data asynchronous to target (depends on STDIO_INIT)

    @param[in]  ucSourceType    1= Debug Ouput, 2 = Protocol Entry
    @param[in]  c_pcFmt         Data to write to target
    @param[in]  ...
    @return     none

    @attention  Don't send messages greather than 100 bytes

    @author     S.Schüpbach                                 @date 07.12.2017  */
/*----------------------------------------------------------------------------*/
void STDIO_AsyncPrintf(uint8 ucSourceType, char const *c_pcFmt, ...);

/*----------------------------------------------------------------------------*/
/** @brief      Initialize STDIO (standard input output) to use USART 1. 
                -> printf(), putchar(), ... / scanf(), getchar(), ... 
                can be used.

    @param      None
    @return     None

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
void STDIO_Init(void);

/// @}   // end of Group

#endif   // __STDIO_H__
