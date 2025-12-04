/** @addtogroup   LIST  
    @{
********************************************************************************
*   @name       LIST
*   @file       LIST.c
*
*   @brief      Implementation of a dynamic list.
*               List on HEAP. Implemented as array of pointers
*               to the effective data of the items.
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
#include "STR.h"
#include "LIST.h"

/*== Defines =================================================================*/
  
/*== Enumerations ============================================================*/

/*== Structures ==============================================================*/

/*== Global Variables ========================================================*/

/*== Module Constants ========================================================*/

/*== Function Prototypes =====================================================*/
#if SHELL_LIST > 0
  static void mLIST_InitShell(void);
#endif
  
/*== Module Variables ========================================================*/

/*== Weak Functions to uncouple Modules ======================================*/

/*============================================================================*/
/*== Implementation of Public Functions ======================================*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/
t_List *LIST_Create(uint16 unMaxEntries)
{
  uint32         ulLen;
  t_List         *ptList;

  // allocat memory of list
  ulLen = sizeof(t_List);
  ptList = HEAP_MALLOC(ulLen);

  // allocat memory entries
  ulLen =  (sizeof(t_ListEntry)* unMaxEntries);
  ptList->patEntries = HEAP_MALLOC(ulLen);

  // init list
  ptList->unMaxNumber = unMaxEntries;
  ptList->unNumbers = 0;

  return ptList;
}

/*----------------------------------------------------------------------------*/
uint16 LIST_ItemCount(const t_List *ptList)
{
  return ptList->unNumbers;
}

/*----------------------------------------------------------------------------*/
uint16 LIST_AddItem(t_List *ptList, const uint8 *c_paucBuf, uint16 unLen)
{
  uint16   unIdx = LIST_ItemCount(ptList);

  if (unIdx >= ptList->unMaxNumber)
    return 0;
  ASSERT(c_paucBuf);
  ptList->patEntries[unIdx].pucEntry = HEAP_MALLOC(unLen);
  memcpy(ptList->patEntries[unIdx].pucEntry, c_paucBuf, unLen);
  ptList->patEntries[unIdx].unLen = unLen;

  unIdx++;
  ptList->unNumbers = unIdx;

  return unIdx;
}

/*----------------------------------------------------------------------------*/
uint16 LIST_DeleteItem(t_List *ptList, uint16 unIdx)
{
  uint8 *pucItem = LIST_GetItem(ptList, unIdx, NULL);

  if (pucItem)
  {
    uint16  unCount = LIST_ItemCount(ptList);
    uint16  unToMove =  unCount - unIdx - 1;

    if (unCount <= unIdx)
      ASSERT_FAIL();  

    HEAP_FREE(pucItem);
    memmove((uint8*)&ptList->patEntries[unIdx], (uint8*)&ptList->patEntries[unIdx+1], unToMove*sizeof(t_ListEntry));

    ASSERT(ptList->unNumbers);
    ptList->unNumbers--;    
    return 0;
  }
  else
    return 1;
}

/*----------------------------------------------------------------------------*/
uint8 *LIST_GetItem(const t_List *ptList, uint16 unIdx, uint16 *punLen)
{
  if (unIdx >= LIST_ItemCount(ptList))
    return 0;

  if (punLen)
    *punLen = ptList->patEntries[unIdx].unLen;

  return ptList->patEntries[unIdx].pucEntry;
}

/*----------------------------------------------------------------------------*/
uint8 *LIST_CopyItem(const t_List *ptList, uint16 unIdx, uint16 *punLen)
{
  uint8 *paucBuf = NULL;
  
  if (unIdx >= LIST_ItemCount(ptList))
    return 0;

  paucBuf = HEAP_MALLOC(ptList->patEntries[unIdx].unLen);
  memcpy(paucBuf, ptList->patEntries[unIdx].pucEntry, ptList->patEntries[unIdx].unLen);

  if (punLen)
    *punLen = ptList->patEntries[unIdx].unLen;

  return paucBuf;
}

/*----------------------------------------------------------------------------*/
uint8 *LIST_SearchStrItem(const t_List *ptList, const char *c_pacSearch, uint16 unStartIdx, uint16 *punFoundIdx)
{
  uint16  unIdx;
  uint16  unItemCount = LIST_ItemCount(ptList);

  for (unIdx = unStartIdx; unIdx < unItemCount; unIdx++)
  {
    uint16  unItemLen;
    uint8   *paucItem = LIST_GetItem(ptList, unIdx, &unItemLen);

    if (STR_wildcmp(c_pacSearch, (char*)paucItem))
    {
      if (punFoundIdx)
        *punFoundIdx = unIdx;
      return paucItem;
    }
  }
  
  return NULL;
}

/*----------------------------------------------------------------------------*/
uint8 *LIST_SearchMaskStr(const t_List *ptList, const char *c_pacSearch, uint16 unStartIdx, uint16 *punFoundIdx)
{
  uint16  unIdx;
  uint16  unItemCount = LIST_ItemCount(ptList);

  for (unIdx = unStartIdx; unIdx < unItemCount; unIdx++)
  {
    uint16  unItemLen;
    uint8   *paucItem = LIST_GetItem(ptList, unIdx, &unItemLen);

    if (STR_wildcmp((char*)paucItem, c_pacSearch))
    {
      if (punFoundIdx)
        *punFoundIdx = unIdx;
      return paucItem;
    }
  }
  
  return NULL;
}

/*----------------------------------------------------------------------------*/
uint16 LIST_DeleteStrItems(t_List *ptList, const char *c_pacSearch, uint16 unStartIdx)
{
  uint16  unIdx;
  uint16  unDelCount = 0;
  
  while (LIST_SearchStrItem(ptList, c_pacSearch, unStartIdx, &unIdx) != NULL)
  {
    if (LIST_DeleteItem(ptList, unIdx) != 0)
      ASSERT_FAIL();
    unDelCount++;
  }
  
  return unDelCount;
}

/*----------------------------------------------------------------------------*/
int LIST_SortCmpUp (const void * a, const void * b)
{
  t_ListEntry *pucA = (t_ListEntry *)a, *pucB = (t_ListEntry *)b;
  return ( memcmp((void *)pucA->pucEntry, (void *)pucB->pucEntry, min(pucA->unLen, pucB->unLen) ));
}

int LIST_SortCmpDown (const void * a, const void * b)
{
  t_ListEntry *pucA = (t_ListEntry *)a, *pucB = (t_ListEntry *)b;
  return ( memcmp((void *)pucB->pucEntry, (void *)pucA->pucEntry, min(pucA->unLen, pucB->unLen) ));
}

void LIST_Sort(t_List *ptList, bool bUpDown)
{
  uint16 unCount = LIST_ItemCount(ptList);
  qsort(ptList->patEntries, unCount, sizeof(t_ListEntry), bUpDown ? LIST_SortCmpUp : LIST_SortCmpDown);
}

/*----------------------------------------------------------------------------*/
char *LIST_ToString(const t_List *ptList, const char *c_pacDelimiter)
{
  uint16  unIdx;
  uint16  unItemCount = LIST_ItemCount(ptList);
  char *pacString = NULL;

  for (unIdx = 0; unIdx < unItemCount; unIdx++)
  {
    uint16  unItemLen;
    uint8   *paucItem = LIST_GetItem(ptList, unIdx, &unItemLen);
    
    if (pacString == NULL)
      pacString = STR_cpy((char *)paucItem);
    else
      pacString = STR_cat(pacString, 2, c_pacDelimiter, (char *)paucItem);
  }
  
  return pacString;
}

/*----------------------------------------------------------------------------*/
void LIST_Free(t_List *ptList)
{
  uint16 unI, unCount = LIST_ItemCount(ptList);

  for (unI = 0; unI < unCount; unI++)
  {
    uint8 *pucEntry = LIST_GetItem(ptList, unI, NULL);
    HEAP_FREE(pucEntry);
  }
  HEAP_FREE(ptList->patEntries);
  HEAP_FREE(ptList);
}

/*----------------------------------------------------------------------------*/
void LIST_Init(void)
{
  #if SHELL_LIST > 0
    // register shell-command
    mLIST_InitShell();
  #endif
}

/*----------------------------------------------------------------------------*/
// SHELL Interface
/*----------------------------------------------------------------------------*/

#if SHELL_LIST > 0
  #include "SHELL.h"

  /*--------------------------------------------------------------------------*/
  /// this text will be displayed, in the shell-command overview 
  static const char mc_acSummaryList[] =
    "Functional description of LIST-Shell-Modul. (not yet implemented!)\r\n";
  
  /// this text will be displayed, if this module return E_SHELL_CMD_WRONG_PARAMETERS 
  static const char mc_acUsageList[] =
    "list <source> <dest>\r\n"
    "  source: filename (may include an absolut/relative path) to copy from.\r\n"
    "  dest:   filename (may include an absolut/relative path) to copy to.\r\n"
    "\r\n"
    "Examples: 'list M:\\folder\\file.bin U:\\MyDir\\file.bin' (SD-Card -> Memory-Stick)\r\n"
    "          'list ..\\..\\myfile.txt ..\\..\\..\\folder\\mynewfile.txt'\r\n";
  
  /*--------------------------------------------------------------------------*/
  /** @brief      this function is called, if a registered command of this module is
                  entered in the shell
   
      @param[in]  c_ptContext    entered Text
      @return     ulRet           0 (No error)
                                  E_SHELL_CMD_WRONG_PARAMETERS (Parameter doesn't match)
      @author     A.Bader                                   @date 21.05.2014  */
  /*--------------------------------------------------------------------------*/
  static uint32 mLIST_ShellList(const t_ShellCmdContext *c_ptContext)
  {
    uint32  ulErr = 0;
    
    if (SHELL_CheckCmdParam(c_ptContext, "cmd1", 4))
    {
      // --- cmd1, 4 parameters
      // Do not use printf to have the output on the correct stream (if output of command
      // has been redirected to a file!)
      fprintf(c_ptContext->pfOut, "Do not use 'printf' -> use fprintf...\r\n");
      ; // add functionality
    }
    else if (SHELL_CheckCmdParamEx(c_ptContext, 2, 4, 0, "cmd2"))
    {
      // --- cmd2, 3 or 5 parameters
      ; // add functionality
    }
    else if (c_ptContext->ucNumParams == 0)
    {
      ; // grow old
    }
    else
    {
      // mc_acUsageList will be displayed
      ulErr = E_SHELL_CMD_WRONG_PARAMETERS;    
    }
      
    return ulErr;
  }

  /*--------------------------------------------------------------------------*/
  /** @brief      register shell-commands of LIST-Module
   
      @param      None
      @return     None

      @author     A.Bader                                   @date 21.05.2014  */
  /*--------------------------------------------------------------------------*/
  static void mLIST_InitShell(void)
  {
    // use module name as parameter or check with shell-cmd "help" that parameter isn't used
    SHELL_AddCommand("list", mLIST_ShellList, mc_acSummaryList, mc_acUsageList);
  }
  
#endif // SHELL_LIST
  
/// @}
/// @}
  
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

