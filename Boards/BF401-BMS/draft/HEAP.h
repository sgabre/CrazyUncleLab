#ifndef __HEAP_H__
#define __HEAP_H__

/** @defgroup   HEAP  HEAP-Group
                Wrappers for heap functions to detect buffer overflows
    @{
********************************************************************************
*   @name       HEAP
*   @file       HEAP.h
*
*   @brief      Heap Watcher: Wrappers for malloc, realloc, free
*
*   @authors    A.Bader
*   @date       24.06.2014
*
*   @copyright (C) 2014 by: MICROTRONIC AG
*                           Duennernstrasse 32
*                           CH-4702 Oensingen
*******************************************************************************/

/*== Includes ================================================================*/
#include <stdlib.h>
#include <basetype.h>

/*== Defines =================================================================*/


/*== Enumerations ============================================================*/


/*== Structures ==============================================================*/


/*== Module Interfaces =======================================================*/

/*----------------------------------------------------------------------------*/
/** @brief      Wrapper function for malloc, called by macro HEAP_MALLOC.

    @param[in]  ulSize    Size of block to allocate. Internally, a block of
                          ulSize + 4 will be allocated.
    @param[in]  c_pacFunc function name of caller (due to macro call with __func__)
    @param[in]  ulLine    code line of caller (due to macro call with __LINE__)
    @return     allocated pointer. NULL is never returned because this would
                internally already cause an ASSERT.

    @author     A.Bader                                     @date 24.06.2014  */
/*----------------------------------------------------------------------------*/
void *__heap_malloc(uint32 ulSize, const char *c_pcFunc, uint32 ulLine);

/*----------------------------------------------------------------------------*/
/** @brief      Wrapper function for realloc, called by macro HEAP_REALLOC.

    @param[in]  ptr       Pointer already allocated or NULL, if new block
                          shall be allocated.
    @param[in]  ulSize    New size of block. Internally, a block of
                          ulSize + 4 will be allocated.
    @param[in]  c_pacFunc function name of caller (due to macro call with __func__)
    @param[in]  ulLine    code line of caller (due to macro call with __LINE__)
    @return     allocated pointer. NULL is never returned because this would
                internally already cause an ASSERT.

    @author     A.Bader                                     @date 24.06.2014  */
/*----------------------------------------------------------------------------*/
void *__heap_realloc(void *ptr, uint32 ulSize, const char *c_pcFunc, uint32 ulLine);

/*----------------------------------------------------------------------------*/
/** @brief      Wrapper function for calloc, called by macro HEAP_CALLOC.
                Internally, malloc is called (ulNum*ulSize+4) and memory
                initialized with 0 (as calloc also does).

    @param[in]  ulNum     Number of elements to allocate.
    @param[in]  ulSize    Size of each element.
    @param[in]  c_pacFunc function name of caller (due to macro call with __func__)
    @param[in]  ulLine    code line of caller (due to macro call with __LINE__)
    @return     allocated pointer. NULL is never returned because this would
                internally already cause an ASSERT.

    @author     A.Bader                                     @date 24.06.2014  */
/*----------------------------------------------------------------------------*/
void *__heap_calloc(uint32 ulNum, uint32 ulSize, const char *c_pcFunc, uint32 ulLine);

/*----------------------------------------------------------------------------*/
/** @brief      Wrapper function for free, called by macro HEAP_FREE.

    @param[in]  ptr       Pointer to free up.
                          INFO: in the calling macro, ptr is set to NULL
                          after free!
    @param[in]  c_pacFunc function name of caller (due to macro call with __func__)
    @param[in]  ulLine    code line of caller (due to macro call with __LINE__)
    @return     allocated pointer. NULL is never returned because this would
                internally already cause an ASSERT.

    @author     A.Bader                                     @date 24.06.2014  */
/*----------------------------------------------------------------------------*/
void __heap_free(void *ptr, const char *c_cFunc, uint32 ulLine);

/*----------------------------------------------------------------------------*/
/** @brief      Output free heap space (and other infos, if enabled) to 
                stream given by pF.
   
    @param[in]  pF    stream pointer (may be stdout or an opened file)
    @return     None

    @author     A.Bader                                     @date 24.06.2014  */
/*----------------------------------------------------------------------------*/
void HEAP_OutputHeapInfo(FILE *pF);

/*----------------------------------------------------------------------------*/
/** @brief      Output internal block table, if activated to stream given by
                pF. Output every block found in table, if c_pacFilter = NULL.
                Otherwise, a strstr must find c_pacFilter in the stored
                function name.
   
    @param[in]  pF            stream pointer (may be stdout or an opened file)
    @param[in]  c_pacFilter   NULL: All blocks displayed; otherise, point to
                              a 0-terminated string, which must be found within
                              the stored function name (of caller to the 
                              allocation function) to output this block.
    @param[in]  ulMinTime     Time offset of block. The stored time offset in
                              the block table must be >= ulMinTime. This
                              allows to filter out blocks, which are always
                              allocated for system initialisation (they
                              have a low time offset, because they are allocated
                              just after poweron).
    @return     None

    @author     A.Bader                                     @date 24.06.2014  */
/*----------------------------------------------------------------------------*/
void HEAP_OutputAllocationTable(FILE *pF, const char *c_pacFilter, uint32 ulMinTime);

/*----------------------------------------------------------------------------*/
/** @brief      Return amount of free space on heap. Note: this space is probably
                not available within one single block - the heap may be fragmented;
                therefore, it is not assured that a malloc of this amount will succeed!
   
    @param      None
    @return     amount of free space

    @author     A.Bader                                     @date 24.06.2014  */
/*----------------------------------------------------------------------------*/
uint32 HEAP_GetFreeSpace(void);

/*----------------------------------------------------------------------------*/
/** @brief      Fills up the buffer with pre-allocated t_Msg. There are used by IRQ.

    @param      None
    @return     None

    @author     S.Schüpbach                                 @date 19.12.2017  */
/*----------------------------------------------------------------------------*/
void HEAP_IdleAction(void);

/*----------------------------------------------------------------------------*/
/** @brief      Initialize HEAP module. Currently, only Shell-Command is
                registered (if enabled)
                Modul initialization is done on first usage of HEAP_MALLOC,
                HEAP_REALLOC, HEAP_FREE (if Heap Watcher enabled).

    @param      None
    @return     None

    @author     A.Bader                                     @date 24.06.2014  */
/*----------------------------------------------------------------------------*/
void HEAP_Init(void);

/*----------------------------------------------------------------------------*/
#if HEAP_WATCHER > 0
  #define HEAP_MALLOC(UL_SIZE)         __heap_malloc(UL_SIZE, __func__, __LINE__)
  #define HEAP_REALLOC(PTR, UL_SIZE)   __heap_realloc(PTR, UL_SIZE, __func__, __LINE__)
  #define HEAP_CALLOC(UL_NUM, UL_SIZE) __heap_calloc(UL_NUM, UL_SIZE, __func__, __LINE__)
  #define HEAP_FREE(PTR)               { __heap_free(PTR, __func__, __LINE__); (PTR) = NULL; }
#else

  #define HEAP_MALLOC(UL_SIZE)         malloc(UL_SIZE)
  #define HEAP_REALLOC(PTR, UL_SIZE)   realloc(PTR, UL_SIZE)
  #define HEAP_CALLOC(UL_NUM, UL_SIZE) calloc(UL_NUM, UL_SIZE)
  #define HEAP_FREE(PTR)               free(PTR)
#endif

/// @}   // End of Group

#endif   // __HEAP_H__
