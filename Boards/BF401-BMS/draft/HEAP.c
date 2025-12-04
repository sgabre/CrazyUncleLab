/** @addtogroup   HEAP  
    @{
********************************************************************************
*   @name       HEAP
*   @file       HEAP.c
*
*   @brief      Wrappers for heap functions.
*               Heap Watcher: Wrappers for malloc, realloc, free
*
*   @authors    A.Bader
*   @date       24.06.2014
*
*   @copyright (C) 2014 by: MICROTRONIC AG
*                           Duennernstrasse 32
*                           CH-4702 Oensingen
*******************************************************************************/

/*== Includes ================================================================*/
#include "Target.h"

#include "DT.h"
#include "DBG.h"
#include "STR.h"
#include "IOT.h"
#include "FIFO.h"

#include "HEAP.h"


/*== Defines =================================================================*/
///< Number of entries in table. Difference between malloc/free must not exceed
///< this limit; ASSERT otherwise
///< Total size of table in RAM: (NUM_HEAP_ENTRIES = 500) * 16 -> 8kB
#ifdef __BOOT
  #define   NUM_HEAP_ENTRIES          100
#else
  #define   NUM_HEAP_ENTRIES          850
#endif

#define HEAP_IRQ_FIFO_SIZE            100


/*== Enumerations ============================================================*/

/*== Structures ==============================================================*/
typedef struct
{
  void        *ptr;         // pointer of block
  uint32       ulSize;      // size of block
  uint16       unLine;      // code line where allocated
  uint16       unTime;      // time offset in [s] after last poweron
  const char  *c_pacFunc;   // where allocated
} t_HeapEntry;


/*== Global Variables ========================================================*/


/*== Module Constants ========================================================*/


/*== Function Prototypes =====================================================*/
#if SHELL_HEAP > 0
  static void mHEAP_InitShell(void);
#endif


/*== Module Variables ========================================================*/
#if HEAP_WATCHER & 1   // maintain block table / buffer overflow check
  static bool             m_bTableInit = false;
  static t_HeapEntry      m_tHeapEntries[NUM_HEAP_ENTRIES] __attribute__ ((section(".ccm")));
  static pt_OsextMutexId  m_ptOsMutIdTable;
#endif

#if HEAP_WATCHER & 2   // count allocations / frees
  static uint32  m_ulMalloc = 0;
  static uint32  m_ulFree = 0;
#endif

#if HEAP_WATCHER & 4   // check and hold min heap space after every malloc
  static uint32   m_ulMinHeap = -1UL;
#endif

// FIFO buffer for pre allocated IRQ messages
static t_Fifo *m_ptIrqMsgFifo = NULL;


/*== Weak Functions to uncouple Modules ======================================*/


/*============================================================================*/
/*== Static Module Functions =================================================*/
/*============================================================================*/

#if HEAP_WATCHER & 1   // maintain block table / buffer overflow check
/*----------------------------------------------------------------------------*/
/** @brief      Initialize Heap Watcher Table.
                Called from every HEAP_xxx function to make sure
                it will be initialized on first usage, but only once.
   
    @param      None
    @return     None

    @author     A.Bader                                     @date 24.06.2014  */
/*----------------------------------------------------------------------------*/
static void mHEAP_CheckInit(void)
{
  // Initialize only once...
  if (!m_bTableInit)
  {
    // Initialize only once...
    m_bTableInit = true;
    // Mutex to protect Table
    m_ptOsMutIdTable = OSEXT_HeapMutexTableNew();
    // Initialize Table
    memset(&m_tHeapEntries, 0, sizeof(m_tHeapEntries));
  }
}
#endif

#if HEAP_WATCHER & 1   // maintain block table / buffer overflow check
/*----------------------------------------------------------------------------*/
/** @brief      Insert infos about new allocated block in lookup table.
   
    @param[in]  ptEntry   entry to insert in table
    @param[in]  c_pacFunc function name of HEAP_xxx caller
    @param[in]  ulLine    code line of HEAP_xxx caller
    @return     None (ASSERT in case of error)

    @author     A.Bader                                     @date 24.06.2014  */
/*----------------------------------------------------------------------------*/
static void mHEAP_InsertEntry(void *ptr, uint32 ulSize, const char *c_pacFunc, uint32 ulLine)
{
  uint16  unI;

  for (unI = 0; unI < sizeof(m_tHeapEntries) / sizeof(m_tHeapEntries[0]); unI++)
  {
    if (!m_tHeapEntries[unI].ptr)
    {
      m_tHeapEntries[unI].ptr = ptr;
      m_tHeapEntries[unI].ulSize = ulSize;
      m_tHeapEntries[unI].c_pacFunc = c_pacFunc;
      m_tHeapEntries[unI].unLine = ulLine;
      m_tHeapEntries[unI].unTime = IOT_GetSec();
      return;
    }
  }

  // Memory Leak or m_tHeapEntries table too small!
  {
    char acText[100];
    snprintf(acText, sizeof(acText), "Memory Leak? no space in table for new block @ '%s', Line %d", c_pacFunc, ulLine);
    ASSERT_FAIL_MSG(acText);
  }
}
#endif

#if HEAP_WATCHER & 1   // maintain block table / buffer overflow check
/*----------------------------------------------------------------------------*/
/** @brief      Search ptr in internal block table.
   
    @param[in]  ptr       pointer to search for. Must be created by HEAP_XXALLOC
    @return     pointer to table entry or NULL, if ptr not found

    @author     A.Bader                                     @date 24.06.2014  */
/*----------------------------------------------------------------------------*/
static t_HeapEntry *mHEAP_GetEntry(void *ptr)
{
  uint16  unI;

  for (unI = 0; unI < sizeof(m_tHeapEntries) / sizeof(m_tHeapEntries[0]); unI++)
  {
    if (m_tHeapEntries[unI].ptr == ptr)
      return &m_tHeapEntries[unI];
  }
  
  return NULL;
}
#endif 
  
#if HEAP_WATCHER & 1   // maintain block table / buffer overflow check
/*----------------------------------------------------------------------------*/
/** @brief      Delete entry from internal block table. (Typically used in
                HEAP_FREE.)
                The ptr is searched; if not found, ASSERT.
   
    @param[in]  ptr       pointer to search for. Must be created by HEAP_XXALLOC
    @param[in]  c_pcFunc  Name of calling function of HEAP_FREE
    @param[in]  ulLine    code line of HEAP_xxx caller
    @return     None (ASSERT in case of ptr not found in table)

    @author     A.Bader                                     @date 24.06.2014  */
/*----------------------------------------------------------------------------*/
static void mHEAP_DeleteEntry(void *ptr, const char *c_pcFunc, uint32 ulLine)
{
  t_HeapEntry *ptEntry = mHEAP_GetEntry(ptr);

  if (ptEntry)
    memset(ptEntry, 0, sizeof(t_HeapEntry));
  else
  {
    char acBuf[80];
    
#ifdef __DEBUG
    printf ("HEAP_FREE: Pointer not found! (0x%x)\r\n", ptr);
    printf("Function: %s Line: %d", c_pcFunc, ulLine);
    printf ("Dump: ptr-32 to ptr+32r\n");
    DBG_dumpout(stdout, (uint8*)ptr-32, 64);
#endif

    snprintf(acBuf, sizeof(acBuf), "ptr(%x) not allocated; HEAP_FREE @ '%s', Line %d", ptr, c_pcFunc, ulLine);
    ASSERT_FAIL_MSG(acBuf);
  }
}
#endif
  
#if HEAP_WATCHER & 1   // maintain block table / buffer overflow check
/*----------------------------------------------------------------------------*/
static void mHEAP_OutputCorrupt(FILE *pF, t_HeapEntry *ptEntry)
{
  char  acDateBuf[48];
  DT_GetDateTimeStr(acDateBuf, sizeof(acDateBuf), "%d.%m.%Y %H:%M:%S", NULL);
  fprintf (pF, "%s / Heap corrupted! ptr created @ '%s', Line %d, size %d\r\n", acDateBuf, ptEntry->c_pacFunc, ptEntry->unLine, ptEntry->ulSize);
  DBG_dumpout(pF, (uint8 *)ptEntry->ptr, ptEntry->ulSize + 4);
}
#endif

#if HEAP_WATCHER & 1   // maintain block table / buffer overflow check
/*----------------------------------------------------------------------------*/
/** @brief      Go through every entry in table and check that the blocks
                are not corrupted (i.e. $$$$ at the end of blocks)! -> ASSERT otherwise.
   
    @param[in]  c_pacFunc  function name of HEAP_xxx caller
    @param[in]  ulLine    code line of HEAP_xxx caller
    @return     None (ASSERT in case of error)

    @author     A.Bader                                     @date 24.06.2014  */
/*----------------------------------------------------------------------------*/
static void mHEAP_CheckHeap(const char *c_pacFunc, uint32 ulLine)
{
  uint16        unI;
  t_HeapEntry   *ptCorruptEntry = NULL;

  // Go through every entry in the watcher table
  for (unI = 0; unI < sizeof(m_tHeapEntries) / sizeof(m_tHeapEntries[0]); unI++)
  {
    uint8   *ptr = m_tHeapEntries[unI].ptr;
    uint32   ulSize = m_tHeapEntries[unI].ulSize;
    
    // if the last 4 bytes are no more $$$$, there was a write beyond the created buffer size!!
    if (ptr && ((ptr[ulSize] != '$') || (ptr[ulSize + 1] != '$') || (ptr[ulSize + 2] != '$') || (ptr[ulSize + 3] != '$')))
    {
      ptCorruptEntry = &m_tHeapEntries[unI];
      mHEAP_OutputCorrupt(stdout, ptCorruptEntry);
    }
  }

  if (ptCorruptEntry)
  {
    char acText[200];
    snprintf(acText, sizeof(acText), "Heap corrupt; detected @ '%s', Line %d. Block created @ '%s', Line %d, Size: %d, Addr: 0x%08X", 
             c_pacFunc, ulLine, ptCorruptEntry->c_pacFunc, ptCorruptEntry->unLine, ptCorruptEntry->ulSize, ptCorruptEntry->ptr);
    ASSERT_FAIL_MSG(acText);
  }
}
#endif
  
/*----------------------------------------------------------------------------*/
/** @brief      Helperfunction to retrieve free heap space from __heapstats
                (must be 'printf' compatible function...)
   
    @param[out] pulFree     pointer to uint32 to fetch first format string argument
    @return     0

    @author     A.Bader                                     @date 24.06.2014  */
/*----------------------------------------------------------------------------*/
static int __getfreeheapspace (int32 *pulFree, const char *c_pacInput, ... )
{
  va_list pArgs;
  
  va_start(pArgs, c_pacInput);
  if (*pulFree == -1UL)
    *pulFree = va_arg(pArgs, uint32);
  va_end(pArgs);
  
  return 0;
}

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
#if HEAP_WATCHER & 1
static void mHEAP_OutputAllocationTable(FILE *pF, const char *c_pacFilter, uint32 ulMinTime)
{
  uint32  ulSpace = 0;
  uint32  ulCount = 0, ulIdleCount = 0;
  uint16  unI;

  if (c_pacFilter)
    fprintf(pF, "Filtered Output: Display blocks allocated in a function containing '%s'\r\n", c_pacFilter);
  if (ulMinTime)
    fprintf(pF, "Filtered Output: Display ony blocks created after time offset %ds!\r\n", ulMinTime);

  for (unI = 0; unI < sizeof(m_tHeapEntries) / sizeof(m_tHeapEntries[0]); unI++)
  {
    uint8 *ptr = m_tHeapEntries[unI].ptr;
    if (ptr && (!c_pacFilter || strstr(m_tHeapEntries[unI].c_pacFunc, c_pacFilter)) && (m_tHeapEntries[unI].unTime >= ulMinTime))
    {
      uint32 ulSize = m_tHeapEntries[unI].ulSize;

      // Update counter / size to display at the end of table
      ulCount++;
      ulSpace += ulSize;

      if (strstr(m_tHeapEntries[unI].c_pacFunc, "HEAP_IdleAction"))
        ulIdleCount++;
      else
      {
        // print block info
        fprintf(pF, " o 0x%08x, size: %05d, time: %05d, @%s, Line %d", ptr, ulSize, m_tHeapEntries[unI].unTime,
                m_tHeapEntries[unI].c_pacFunc, m_tHeapEntries[unI].unLine);
        // check, if corrupted
        if ((ptr[ulSize] != '$') || (ptr[ulSize + 1] != '$') || (ptr[ulSize + 2] != '$') || (ptr[ulSize + 3] != '$'))
        {
          fprintf(pF, " --> Block corrupted!\r\n");
          DBG_dumpout(pF, ptr, ulSize);
        }
        else
          fprintf(pF, "\r\n");
      }
    }
  }

  fprintf(pF, "Displayed %d (thereof %d IrqFifo) blocks, occupying %d bytes on heap.\r\n", ulCount, ulIdleCount, ulSpace);
}
#endif  // HEAP_WATCHER & 1


/*============================================================================*/
/*== Implementation of Public Functions ======================================*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/
uint32 HEAP_GetFreeSpace(void)
{
  uint32 ulFree = -1UL;
  __heapstats((__heapprt)__getfreeheapspace, &ulFree);
  return ulFree;
}
  
/*----------------------------------------------------------------------------*/
void HEAP_OutputHeapInfo(FILE *pF)
{
#if HEAP_WATCHER & 2  // count allocations / frees
  fprintf(pF, "Heap usage:\r\n");
  fprintf(pF, " o  Num malloc's:    %d\r\n", m_ulMalloc);
  fprintf(pF, " o  Num free's:      %d\r\n", m_ulFree);
  fprintf(pF, " => Difference:      %d\r\n\r\n", m_ulMalloc - m_ulFree);
#endif

#if HEAP_WATCHER & 4  // check and hold min heap space after every malloc
  fprintf(pF, "Maximal Heap Allocation:\r\n");
  fprintf(pF, " => Min. Free. Heap: %d\r\n\r\n", m_ulMinHeap);
#endif
  
  // __heapstats outputs the freespace and free blocks.
  // It writes directly to a 'printf'-compatible function
  fprintf(pF, "Free Heap Space:\r\n");
  __heapstats((__heapprt)fprintf, pF);
  
  // __heapvalid outputs error infos, if the heap is corrupted.
  // It writes directly to a 'printf'-compatible function
  __heapvalid((__heapprt)fprintf, pF, 0);
}

/*----------------------------------------------------------------------------*/
void HEAP_OutputAllocationTable(FILE *pF, const char *c_pacFilter, uint32 ulMinTime)
{
#if HEAP_WATCHER & 1   // use table / buffer overflow check
  // Make sure Mutex and Table are initialized
  mHEAP_CheckInit();
  // Lock access to table
  OSEXT_MutexAcquire(m_ptOsMutIdTable, osWaitForever);

  mHEAP_OutputAllocationTable(pF, c_pacFilter, ulMinTime);
  
  // free access to table
  OSEXT_MutexRelease(m_ptOsMutIdTable);
#else
  fprintf(pF, "HEAP_WATCHER is not configured to maintain a block table! Activate it in ProjectConfig.h");
#endif
}

/*----------------------------------------------------------------------------*/
void *__heap_malloc(uint32 ulSize, const char *c_pacFunc, uint32 ulLine)
{
  uint8  *ptr = NULL;

  if (__get_IPSR())
  {
    ASSERT(m_ptIrqMsgFifo);
    ASSERT(ulSize == sizeof(t_Msg));
    ptr = (uint8*)FIFO_Get(m_ptIrqMsgFifo);
    ASSERT(ptr);
    
#if HEAP_WATCHER & 1
    uint16  unI = 0;

    for (unI = 0; unI < NUM_HEAP_ENTRIES; unI++)
    {
      if (m_tHeapEntries[unI].ptr == ptr)
      {
        m_tHeapEntries[unI].c_pacFunc = c_pacFunc;
        m_tHeapEntries[unI].unLine = ulLine;
        continue;
      }
    }
#endif
  }
  else
  {
#if HEAP_WATCHER & 1   // use table / buffer overflow check
    // Make sure Mutex and Table are initialized
    mHEAP_CheckInit();
    
    // Lock access to table
    OSEXT_MutexAcquire(m_ptOsMutIdTable, osWaitForever);
    
    // make sure the heap is not already corrupted -> this would end up in a crash during malloc
    mHEAP_CheckHeap(c_pacFunc, ulLine);
    
    // do the allocation
    ptr = malloc(ulSize + 4);
    if (!ptr)
    {
      HEAP_OutputHeapInfo(stdout);
      mHEAP_OutputAllocationTable(stdout, NULL, NULL);
    
      OSEXT_Delay(500);
    
      char  acBuf[80];
      snprintf(acBuf, sizeof(acBuf), "Allocation failed: func(%s), line(%d), bytes(%d)", c_pacFunc, ulLine, ulSize);
      ASSERT_FAIL_MSG(acBuf);
    }
    
    // append signature
    ptr[ulSize] = '$'; ptr[ulSize + 1] = '$'; ptr[ulSize + 2] = '$'; ptr[ulSize + 3] = '$';
    
    // insert the entry in the table
    mHEAP_InsertEntry(ptr, ulSize, c_pacFunc, ulLine);
    
    // free access to table
    OSEXT_MutexRelease(m_ptOsMutIdTable);
#else
    ptr = malloc(ulSize);
    ASSERT(ptr);
#endif

#if HEAP_WATCHER & 2  // count allocations / frees
    m_ulMalloc++;
#endif

#if HEAP_WATCHER & 4  // check and hold min heap space after every malloc
    {
      // adapt the minimum free heap space ever seen.
      uint32 ulHeapFree = HEAP_GetFreeSpace();
      if (ulHeapFree < m_ulMinHeap)
        m_ulMinHeap = ulHeapFree;
    }
#endif
  }
  
  return ptr;
}

/*----------------------------------------------------------------------------*/
void *__heap_realloc(void *ptr, uint32 ulSize, const char *c_pacFunc, uint32 ulLine)
{
#if HEAP_WATCHER & 2  // count allocations / frees
  // If realloc with NULL -> allocate block
  if (!ptr)
    m_ulMalloc++;
  
  // if realloc with size 0 -> do free
  if (!ulSize)
    m_ulFree++;
#endif

#if HEAP_WATCHER & 1   // use table / buffer overflow check
  {
    t_HeapEntry *ptEntry;
  
    // Make sure Mutex and Table are initialized
    mHEAP_CheckInit();
  
    // Lock access to table
    OSEXT_MutexAcquire(m_ptOsMutIdTable, osWaitForever);
  
    // make sure the heap is not already corrupted -> this would end up in a crash during malloc
    mHEAP_CheckHeap(c_pacFunc, ulLine);

    // if block at ptr already allocated -> update it later
    if (ptr)
    {
      ptEntry = mHEAP_GetEntry(ptr);
      
      // if pointer given, it must be in internal block table! -> ASSERT otherwise.
      if (!ptEntry)
      {
        char  acBuf[80];
        snprintf(acBuf, sizeof(acBuf), "ptr(%x) not allocated; HEAP_FREE @ '%s', Line %d", ptr, c_pacFunc, ulLine);
        ASSERT_FAIL_MSG(acBuf);
      }
    }
     
    // do the reallocation
    ptr = realloc(ptr, ulSize ? ulSize + 4 : 0);
    
    if (ulSize)
    {
      ASSERT(ptr);
      
      // append signature
      ((uint8*)ptr)[ulSize] = '$';
      ((uint8*)ptr)[ulSize + 1] = '$';
      ((uint8*)ptr)[ulSize + 2] = '$';
      ((uint8*)ptr)[ulSize + 3] = '$';
      
      if (ptEntry)
      {
        // If entry already exists, update it!
        ptEntry->ptr = ptr;
        ptEntry->ulSize = ulSize;
        ptEntry->c_pacFunc = c_pacFunc;
        ptEntry->unLine = ulLine;
      }
      else
      {
        // insert the entry in the table
        mHEAP_InsertEntry(ptr, ulSize, c_pacFunc, ulLine);
      }
    }
    else if (ptEntry)
    {
      memset (ptEntry, 0, sizeof(t_HeapEntry));
    }
    
    // free access to table
    OSEXT_MutexRelease(m_ptOsMutIdTable);
  }
#else
  ptr = realloc(ptr, ulSize);
  ASSERT(ptr);
#endif
  
  return ptr;
}

/*----------------------------------------------------------------------------*/
void *__heap_calloc(uint32 ulNum, uint32 ulSize, const char *c_pacFunc, uint32 ulLine)
{
  void  *ptr = NULL;
  
#if HEAP_WATCHER & 1   // use table / buffer overflow check
  ptr = __heap_malloc(ulNum * ulSize, c_pacFunc, ulLine);
  memset(ptr, 0, ulNum * ulSize);
#else
  ptr = calloc(ulNum, ulSize);
  ASSERT(ptr);
#endif
  
  return ptr;  
}

/*----------------------------------------------------------------------------*/
void __heap_free(void *ptr, const char *c_pacFunc, uint32 ulLine)
{
#if HEAP_WATCHER & 1   // maintain block table / buffer overflow check
  if (ptr)   // ptr may be null!
  {
    // Make sure Mutex and Table are initialized
    mHEAP_CheckInit();
  
    // Lock access to table
    OSEXT_MutexAcquire(m_ptOsMutIdTable, osWaitForever);
  
    // make sure the heap is not already corrupted -> this would end up in a crash during malloc
    mHEAP_CheckHeap(c_pacFunc, ulLine);
  
    // remove the entry from table -> ASSERT, if ptr not found
    mHEAP_DeleteEntry(ptr, c_pacFunc, ulLine);
  
    // do the free
    free(ptr);
  
    // free access to table
    OSEXT_MutexRelease(m_ptOsMutIdTable);
  }
#else
  if (ptr)
  {
    free(ptr);
    ptr = NULL;
  }
#endif
  
#if HEAP_WATCHER & 2  // count allocations / frees
  m_ulFree++;
#endif
}


/*----------------------------------------------------------------------------*/
void HEAP_IdleAction(void)
{
  // prepare buffer for IRQ
  while (m_ptIrqMsgFifo && (FIFO_GetSpace(m_ptIrqMsgFifo) > 1))
    FIFO_Add(m_ptIrqMsgFifo, (uint32)HEAP_MALLOC(sizeof(t_Msg)));
}

/*----------------------------------------------------------------------------*/
void HEAP_Init(void)
{
  m_ptIrqMsgFifo = FIFO_Create(HEAP_IRQ_FIFO_SIZE, false);
  
  while (FIFO_GetSpace(m_ptIrqMsgFifo) > 1)
    FIFO_Add(m_ptIrqMsgFifo, (uint32)HEAP_MALLOC(sizeof(t_Msg)));

#if SHELL_HEAP > 0
  // register shell-command
  mHEAP_InitShell();
#endif
}


/*----------------------------------------------------------------------------*/
// SHELL Interface
/*----------------------------------------------------------------------------*/

#if SHELL_HEAP > 0
#include "SHELL.h"

/*--------------------------------------------------------------------------*/
/// this text will be displayed, in the shell-command overview 
static const char mc_acSummaryHeap[] =
  "Show free heap space, allocated blocks, etc.\r\n";

/// this text will be displayed, if this module return E_SHELL_CMD_WRONG_PARAMETERS 
static const char mc_acUsageHeap[] =
  "heap [subcommand] [...]\r\n"
  "  subcommands:\r\n"
  "    blocks [filter] [-t:<time>]\r\n"
  "        show allocated blocks. if [filter] passed, output only\r\n"
  "        entries, which contain the [filter]-string in the function\r\n"
  "        name.\r\n"
  "        if -t:<time> is passed, output only blocks created after the\r\n"
  "        given time offset in [s].\r\n"
  "        if -t is not specified, -t:2 is used internally. This means,\r\n"
  "        if all blocks shall be output, use 'heap blocks -t:0'!\r\n"
  
#if (HEAP_WATCHER & 1) == 0 // maintain block table / buffer overflow check
  "                ==>  Only available, if configured in 'ProjectConfig.h'.\r\n"
#endif

  "\r\n"
  "  If no subcommand given, free heap space is displayed.\r\n";

/*--------------------------------------------------------------------------*/
/** @brief      this function is called, if a registered command of this module is
                entered in the shell
 
    @param[in]  c_ptContext    entered Text
    @return     ulRet           0 (No error)
                                E_SHELL_CMD_WRONG_PARAMETERS (Parameter doesn't match)
    @author     A.Bader                                   @date 24.06.2014  */
/*--------------------------------------------------------------------------*/
static uint32 mHEAP_ShellHeap(const t_ShellCmdContext *c_ptContext)
{
  uint32  ulErr = 0;
  
  if (c_ptContext->ucNumParams == 0)
  {
    HEAP_OutputHeapInfo(c_ptContext->pfOut);
  }
  else if (SHELL_CheckCmdParamEx(c_ptContext, "blocks", 3, 1, 2, 3))
  {
    // 'Filter' on second param.
    char    *pacFilter = NULL;
    char    *pacTime = NULL;
    uint32   ulTime = 2;
    
    pacTime = (char *)LIST_SearchStrItem(c_ptContext->ptParams, "-t:*", 0, NULL);
    if (pacTime)
    {
      if (sscanf(pacTime, "-t:%d", &ulTime) != 1)
        ulTime = 2;
    }
    
    // [filter]: two params, if no time filter; three params, if time filter
    if (c_ptContext->ucNumParams == (2 + (pacTime ? 1 : 0)))
      pacFilter = (char *)LIST_GetItem(c_ptContext->ptParams, 1, NULL);
      
    HEAP_OutputAllocationTable(c_ptContext->pfOut, pacFilter, ulTime);
  }
  else
  {
    // mc_acUsageHeap will be displayed
    ulErr = E_SHELL_CMD_WRONG_PARAMETERS;    
  }
    
  return ulErr;
}

/*--------------------------------------------------------------------------*/
/** @brief      register shell-commands of HEAP module
 
    @param      None
    @return     None

    @author     A.Bader                                   @date 24.06.2014  */
/*--------------------------------------------------------------------------*/
static void mHEAP_InitShell(void)
{
  // use module name as parameter or check with shell-cmd "help" that parameter isn't used
  SHELL_AddCommand("heap", mHEAP_ShellHeap, mc_acSummaryHeap, mc_acUsageHeap);
}
#endif // SHELL_HEAP
  
/// @}
  
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
