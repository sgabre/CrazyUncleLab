#ifndef __FIFO_H__
#define __FIFO_H__

/** @defgroup   FIFO  FIFO-Group
                detailed description of FIFO group
    @{
********************************************************************************
*   @name       FIFO
*   @file       FIFO.h
*
*   @brief      FIFO buffer handling functionality
*
*   @authors    S.Schuepbach
*   @date       02.05.2017
*
*   @copyright (C) 2014 by: MICROTRONIC AG
*                           Duennernstrasse 32
*                           CH-4702 Oensingen
*******************************************************************************/

/*== Includes ================================================================*/
#include <stdbool.h>
#include "baseType.h"
#include <rtx_os.h>
#include "OSEXT.h"


/*== Defines =================================================================*/

/*== Enumerations ============================================================*/

/*== Structures ==============================================================*/
/// FIFO structure definition
typedef struct _t_Fifo
 {
  uint32 *paulBuf;
  uint16 unBufSize;
  uint16 unWritePos;
  uint16 unReadPos;
  pt_OsextMutexId ptMutexId;
}t_Fifo;
/*== Internal Functions ======================================================*/

/*== Modul Interfaces ========================================================*/

/*----------------------------------------------------------------------------*/
/** @brief      Creates a new FIFO buffer

    @param[in]  unSize              buffer size
    @param[in]  bMutexProtection    TRUE -> every FIFO handling function waits for a mutex
                                    FALSE -> no mutex protection inside FIFO functions
    @return     pointer to the created FIFO structure

    @author     S.Schuepbach                                @date 02.05.2017  */
/*----------------------------------------------------------------------------*/
t_Fifo *FIFO_Create(uint16 unSize, bool bMutexProtection);

/*----------------------------------------------------------------------------*/
/** @brief      Deletes a FIFO buffer

    @param[in]  ptFifo     pointer to FIFO structure
    @return     None

    @author     S.Schuepbach                                @date 02.05.2017  */
/*----------------------------------------------------------------------------*/
void FIFO_Delete(t_Fifo *ptFifo);

/*----------------------------------------------------------------------------*/
/** @brief      Gets the next elements from FIFO buffer.
                Don't call this on an empty buffer -> results in an ASSERT

    @param[in]  ptFifo     pointer to FIFO structure
    @return     next data element from buffer

    @author     S.Schuepbach                                @date 02.05.2017  */
/*----------------------------------------------------------------------------*/
uint32 FIFO_Get(t_Fifo *ptFifo);

/*----------------------------------------------------------------------------*/
/** @brief      Adds a new element to FIFO buffer.
                Check if there is space for another element -> results in an ASSERT in case of an overflow

    @param[in]  ptFifo     pointer to FIFO structure
    @param[in]  ulValue    data element to add to FIFO buffer
    @return     remaining space in FIFO buffer

    @author     S.Schuepbach                                @date 02.05.2017  */
/*----------------------------------------------------------------------------*/
uint16 FIFO_Add(t_Fifo *ptFifo, uint32 ulValue);

/*----------------------------------------------------------------------------*/
/** @brief      Return the space in FIFO buffer

    @param[in]  ptFifo     pointer to FIFO structure
    @return     remaining space in FIFO buffer

    @author     S.Schuepbach                                @date 02.05.2017  */
/*----------------------------------------------------------------------------*/
uint16 FIFO_GetSpace(t_Fifo *ptFifo);

/*----------------------------------------------------------------------------*/
/** @brief      Return the number of elements in FIFO buffer

    @param[in]  ptFifo     pointer to FIFO structure
    @return     number of data elements in FIFO buffer

    @author     S.Schuepbach                                @date 02.05.2017  */
/*----------------------------------------------------------------------------*/
uint16 FIFO_GetCount(t_Fifo *ptFifo);
/// @} // end of Group
/// @}

#endif  // __FIFO_H__
