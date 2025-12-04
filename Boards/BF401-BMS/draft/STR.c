/** @addtogroup   STRING  
    @{
********************************************************************************
*   @name       STRING
*   @file       STRING.c
*
*   @brief      Implementation of various string functions.
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


/*== Defines =================================================================*/


/*== Enumerations ============================================================*/


/*== Structures ==============================================================*/


/*== Global Variables ========================================================*/


/*== Module Constants ========================================================*/


/*== Function Prototypes =====================================================*/
#if SHELL_STR > 0
  static void mSTRING_InitShell(void);
#endif


/*== Module Variables ========================================================*/


/*== Weak Functions to uncouple Modules ======================================*/


/*============================================================================*/
/*== Implementation of Public Functions ======================================*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/
bool STR_wildcmp(const char *c_pcWild, const char *c_pcString)
{
  // from http://www.codeproject.com/KB/string/wild.aspx
  // Written by Jack Handy - jakkhandy@hotmail.com
  const char *cp = NULL, *mp = NULL;

  while ((*c_pcString) && (*c_pcWild != '*'))
  {
    if ((*c_pcWild != *c_pcString) && (*c_pcWild != '?'))
    {
      return 0;
    }
    c_pcWild++;
    c_pcString++;
  }

  while (*c_pcString)
  {
    if (*c_pcWild == '*')
    {
      if (!*++c_pcWild)
      {
        return 1;
      }
      mp = c_pcWild;
      cp = c_pcString + 1;
    }
    else if ((*c_pcWild == *c_pcString) || (*c_pcWild == '?'))
    {
      c_pcWild++;
      c_pcString++;
    }
    else
    {
      c_pcWild = mp;
      c_pcString = cp++;
    }
  }

  while (*c_pcWild == '*')
  {
    c_pcWild++;
  }

  return !*c_pcWild;
}

/*----------------------------------------------------------------------------*/
uint32 STR_toUL(const char *c_pcStr)
{
  // Example: "abc " => 0x61626320
  uint32   ulRet = 0x20202020;
  char    *p = (char *)&ulRet;
  uint8    ucI = 0;

  for (ucI = 0; (*c_pcStr) && (ucI < sizeof(ulRet)); ucI++)
    *p++ = *c_pcStr++;

  return ulRet;
}

/*----------------------------------------------------------------------------*/
char *STR_delete(char *pcStr, const char *c_pcSearch)
{
  char    *pcFound;
  uint16   unSearchLen = strlen(c_pcSearch);

  // as long as c_pcSearch is found in pcStr...
  while ((pcFound = strstr(pcStr, c_pcSearch)) != NULL)
  {
    uint16 unStrLen = strlen(pcStr);
    // copy rest of string after finding c_pcSearch as many characters forward as len of c_pcSearch
    memmove(pcFound, pcFound + unSearchLen, unStrLen - (pcFound - pcStr) - unSearchLen + 1);
  }

  return pcStr;
}

/*----------------------------------------------------------------------------*/
char *STR_replace(char *pcStr, const char *c_pcSearch, const char *c_pcReplace, uint16 unMaxLen)
{
  char   *pcFound = pcStr;
  int32   nSearchLen = strlen(c_pcSearch);
  int32   nReplLen = strlen(c_pcReplace);

  // as long as c_pcSearch is found in pcStr...
  while ((pcFound = strstr(pcFound, c_pcSearch)) != NULL)
  {
    int32  nToMove = strlen(pcStr) - (pcFound - pcStr) - (nSearchLen - nReplLen) + 1;
    int32  lNewLen = strlen(pcStr) + nSearchLen - nReplLen;

    // if search/replace have same len: do not move mem around
    if (nSearchLen == nReplLen)
    {
      memcpy(pcFound, c_pcReplace, nReplLen);
      pcFound += nReplLen;
    }
    // if new string len is within valid range
    else if ((nToMove > 0) && (lNewLen <= unMaxLen))
    {
      memmove(pcFound + nReplLen, pcFound + nSearchLen, nToMove);
      memcpy(pcFound, c_pcReplace, nReplLen);
      pcFound += nReplLen;
    }
    // if noting moved/copied, increase search pointer anyway to avoid
    // endless loop
    else
      pcFound += nSearchLen;
  }

  return pcStr;
}

/*----------------------------------------------------------------------------*/
char *STR_replace_dyn(char *pcStr, const char *c_pcSearch, const char *c_pcReplace)
{
  char    *pcFound = pcStr;
  int32    nSearchLen = strlen(c_pcSearch);
  int32    nReplLen = strlen(c_pcReplace);
  uint32   ulCount = 0;
  uint32   ulBufLen = strlen(pcStr) + 1;

  // as long as c_pcSearch is found in pcStr... count number of occurences
  while ((pcFound = strstr(pcFound, c_pcSearch)) != NULL)
  {
    pcFound += nSearchLen;
    ulCount++;
  }
  
  if (ulCount)
  {
    // if replace string longer than search string
    if (nSearchLen < nReplLen)
    {
      // calculate new buffer size and enlarge buffer
      ulBufLen += ulCount * (nReplLen - nSearchLen);
      pcStr = HEAP_REALLOC(pcStr, ulBufLen);
    }
    
    // Perform replacement...
    STR_replace(pcStr, c_pcSearch, c_pcReplace, ulBufLen);
    
    // if replace string shorter than search string
    if (nReplLen < nSearchLen)
    {
      // shrink buffer
      pcStr = HEAP_REALLOC(pcStr, ulBufLen - ulCount * (nSearchLen - nReplLen));
    }
  }
  
  return pcStr;
}

/*----------------------------------------------------------------------------*/
char *STR_replace_list(char *pcStr, const char *c_apcSearch[], const char *c_apcReplace[], uint16 unMaxLen)
{
  uint16  unI = 0;
  bool    bOneReplace = c_apcReplace[1] == NULL ? true : false;

  while (c_apcSearch[unI])
  {
    STR_replace(pcStr, c_apcSearch[unI], bOneReplace ? c_apcReplace[0] : c_apcReplace[unI], unMaxLen);
    unI++;
  }

  return pcStr;
}

/*----------------------------------------------------------------------------*/
t_List *STR_split(const char *c_pcStr, const char *c_apcSearch[], uint16 unItemLen, uint16 unItemCount)
{
  uint16       unDataLen = strlen(c_pcStr) + 1;
  uint8       *paucData = HEAP_MALLOC(unDataLen);
  uint8       *paucDataOrig = paucData;
  const char  *c_pcRepl[2] = { "\n", 0 };
  t_List      *ptList = LIST_Create(unItemCount);
  uint16       unCurrentLen = 0;
  uint8       *pucCurrentString;

  // create a new buffer containing only one split character instead of a
  // list with different "splitters".
  memcpy(paucData, c_pcStr, unDataLen);
  STR_replace_list((char *)paucData, c_apcSearch, c_pcRepl, unDataLen);

  while (paucData[unCurrentLen])
  {
    if (paucData[unCurrentLen] == '\n')
    {
      // Create String with 0-Termination
      pucCurrentString = (uint8 *)HEAP_MALLOC(unCurrentLen + 1);
      memcpy(pucCurrentString, paucData, unCurrentLen);
      pucCurrentString[unCurrentLen] = 0;
      // The split character occurred
      // Add buffer to list
      LIST_AddItem(ptList, pucCurrentString, unCurrentLen + 1);
      HEAP_FREE(pucCurrentString);
      // point to next part
      paucData = paucData + unCurrentLen + 1;
      unCurrentLen = 0;
    }
    else if (unItemLen && (unCurrentLen >= unItemLen))
    {
      // Create String with 0-Termination
      pucCurrentString = (uint8 *)HEAP_MALLOC(unCurrentLen + 1);
      memcpy(pucCurrentString, paucData, unCurrentLen);
      pucCurrentString[unCurrentLen] = 0;
      // The max item length was reached
      // Add buffer to list
      LIST_AddItem(ptList, pucCurrentString, unCurrentLen + 1);
      HEAP_FREE(pucCurrentString);
      // point to next part
      paucData = paucData + unCurrentLen;
      unCurrentLen = 0;
    }
    else
      unCurrentLen++;
  }

  // if there is no split-condition at the end, add remaining data to list
  if (unCurrentLen)
  {
    // Create String with 0-Termination
    pucCurrentString = (uint8 *)HEAP_MALLOC(unCurrentLen + 1);
    memcpy(pucCurrentString, paucData, unCurrentLen);
    pucCurrentString[unCurrentLen] = 0;

    LIST_AddItem(ptList, pucCurrentString, unCurrentLen + 1);
    HEAP_FREE(pucCurrentString);
  }

  HEAP_FREE(paucDataOrig);
  return ptList;
}

/*----------------------------------------------------------------------------*/
char *STR_clear_whitespaces(const char *c_pcStr, bool bLeft, bool bRight)
{
  int16   nLen = strlen(c_pcStr);
  char   *pcOut = (char *)HEAP_MALLOC(nLen + 1);
  char   *pcPtr = pcOut;
  int16   nStart = nLen + 1;
  int16   nEnd = -1;
  int16   nI;

  if (bLeft)
  {
    for (nI = 0; nI < nLen; nI++)
    {
      if (c_pcStr[nI] > 32)
      {
        nStart = nI;
        break;
      }
    }
  }

  if (bRight)
  {
    for (nI = nLen - 1; nI >= 0; nI--)
    {
      if (c_pcStr[nI] > 32)
      {
        nEnd = nI;
        break;
      }
    }
  }

  for (nI = nStart; nI <= nEnd; nI++)
  {
      *pcPtr = c_pcStr[nI];
      pcPtr++;
  }
  *pcPtr = 0;

  return pcOut;
}

/*---------------------------------------------------------------------------*/
void *STR_ClearNewlines(char *pcBuf, uint16 unS)
{
  uint16  unI = 0;

  for (unI = 0; unI < unS; unI++)
  {
    if ((pcBuf[unI] == '\n') || (pcBuf[unI] == '\r'))
      pcBuf[unI] = 0;
  }

  return 0;
}

/*---------------------------------------------------------------------------*/
char *STR_GetStringLine(const char *c_paucStr, const char **ppNextStr)
{
  char    *pucPosN = strchr(c_paucStr, '\n');
  char    *pucPosR = strchr(c_paucStr, '\r');
  uint32   ulPos = 0;
  char    *pacStr = NULL;
  
  if (pucPosN && pucPosR)
    ulPos = min(pucPosN - c_paucStr, pucPosR - c_paucStr);
  else if (pucPosN)
    ulPos = pucPosN - c_paucStr;
  else if (pucPosR)
    ulPos = pucPosR - c_paucStr;
  else
    ulPos = strlen(c_paucStr);

  if (ppNextStr)
    *ppNextStr = NULL;

  if (ulPos)
  {
    pacStr = HEAP_MALLOC(ulPos + 1);
    memcpy(pacStr, c_paucStr, ulPos);
    pacStr[ulPos] = 0;

    while ((c_paucStr[ulPos] == '\n') || (c_paucStr[ulPos] == '\r'))
      ulPos++;

    if (ppNextStr)
    {
      if (c_paucStr[ulPos] != 0)
        *ppNextStr = c_paucStr + ulPos;
    }    
  }

  return pacStr;
}

/*----------------------------------------------------------------------------*/
char *STR_cpy(const char *c_paucStr)
{
  uint32   ulLen = strlen(c_paucStr) + 1;
  char    *pacBuf = HEAP_MALLOC(ulLen);
  memcpy(pacBuf, c_paucStr, ulLen);
  return pacBuf;
}

/*----------------------------------------------------------------------------*/
char *STR_cat(char *pacStr, int nNumStrings, ...)
{
  uint32    ulTotLen = 0;
  char     *pacPtr;
  int       nI;
  va_list   vl;
  
  va_start(vl, nNumStrings);
  for (nI = 0; nI < nNumStrings; nI++)
  {
    pacPtr = va_arg(vl, char *);
    if (pacPtr)
      ulTotLen += strlen(pacPtr);
  }
  va_end(vl);

  if (pacStr)
    pacStr = HEAP_REALLOC(pacStr, ulTotLen + strlen(pacStr) + 1);
  else
  {
    pacStr = HEAP_MALLOC(ulTotLen + 1);
    pacStr[0] = 0;
  }
  
  va_start(vl, nNumStrings);
  for (nI = 0; nI < nNumStrings; nI++)
  {
    pacPtr = va_arg(vl, char *);
    if (pacPtr)
      strcat(pacStr, pacPtr);
  }
  va_end(vl);

  return pacStr;
}

/*----------------------------------------------------------------------------*/
uint8 *STR_AsciiHexToBuf(const char *c_pacStr, uint32 *pulLen)
{
  char     acVal[3] = { 0, 0, 0 };
  uint32   ulBufLen = strlen(c_pacStr) / 2;
  uint32   ulI;
  uint8   *paucBuf = NULL;
  
  // Allocate only, if string not empty!
  if (ulBufLen)
    paucBuf = HEAP_MALLOC(ulBufLen);

  for (ulI = 0; ulI < ulBufLen; ulI++)
  {
    memcpy(acVal, &c_pacStr[ulI * 2], 2);
    paucBuf[ulI] = (uint8)strtol(acVal, NULL, 16);
  }

  if (pulLen)
    *pulLen = ulBufLen;

  return paucBuf;
}

/*----------------------------------------------------------------------------*/
uint8 *STR_AsciiHexToBufSafe(const char *c_pacStr, uint32 *pulLen)
{
  char     acVal[3] = { 0, 0, 0 };
  uint32   ulBufLen = strlen(c_pacStr) / 2;
  uint32   ulI;
  uint8   *paucBuf = NULL;
  uint32   ulDataLen = 0;
  bool     bErr = false;
  
  // Allocate only, if string not empty!
  if (ulBufLen)
    paucBuf = HEAP_MALLOC(ulBufLen);

  for (ulI = 0; !bErr && (ulI < ulBufLen); ulI++)
  {
    memcpy (acVal, &c_pacStr[ulI*2], 2);
    if ( ((acVal[0] >= '0' && acVal[0] <= '9') || (acVal[0] >= 'a' && acVal[0] <= 'f') || (acVal[0] >= 'A' && acVal[0] <= 'F')) && 
         ((acVal[1] >= '0' && acVal[1] <= '9') || (acVal[1] >= 'a' && acVal[1] <= 'f') || (acVal[1] >= 'A' && acVal[1] <= 'F')) )
    {
      paucBuf[ulI] = (uint8)strtol(acVal, NULL, 16);
      ulDataLen++;
    }
    else
      bErr = true;    
  }

  if (pulLen)
    *pulLen = ulDataLen;

  return paucBuf;
}

/*---------------------------------------------------------------------------*/
char *STR_BufToAsciiHex(const uint8 *c_paucBuf, uint32 ulBufLen)
{
  uint32   ulAsciiBufLen = ulBufLen * 2 + 4;
  char    *pacStr = (char *)HEAP_MALLOC(ulAsciiBufLen);
  uint32   ulI = 0;

  for (ulI = 0; ulI < ulBufLen; ulI++)
    sprintf(pacStr + (ulI * 2), "%02X", c_paucBuf[ulI]);

  return pacStr;
}

/*---------------------------------------------------------------------------*/
char *STR_ToUpperCase(char *pacStr)
{
  uint8  ucLen = strlen(pacStr);
  uint8  ucI = 0;

  for (ucI = 0; ucI < ucLen; ucI++)
  {
    if ((pacStr[ucI] >= 97) && (pacStr[ucI] <= 122))
      pacStr[ucI] -= 32;
  }

  return pacStr;
}

/*---------------------------------------------------------------------------*/
char *STR_ToLowerCase(char *pacStr)
{
  uint8  ucLen = strlen(pacStr);
  uint8  ucI = 0;

  for (ucI = 0; ucI < ucLen; ucI++)
  {
    if ((pacStr[ucI] >= 65) && (pacStr[ucI] <= 90))
      pacStr[ucI] += 32;
  }

  return pacStr;
}

/*----------------------------------------------------------------------------*/
char *STR_chr(const char *c_pacStr, const char cFind, int16 nIndex)
{
  char *pcPos;
  
  // Init Result [None]
  char *pcRes = NULL;
  
  // Check String [Ignore, if NULL] & Index [Ignore, if Zero]
  if (c_pacStr && nIndex)
  {
    // Valid: Check Search Direction [Forward (> 0) / Backward (< 0)]
    if (nIndex > 0)
    {
      // Forward: Parse String for Character until <No more Occurrences> / <Index reaches Zero>
      for (pcPos = strchr(c_pacStr, cFind), nIndex--; pcPos && nIndex; pcPos = strchr(pcPos + 1, cFind), nIndex--)
        ;
      // Update Result [if n-th Occurrence found]
      if (pcPos && !nIndex)
        pcRes = pcPos;
    }
    else
    {
      // Backward: Parse String backwards until <Start of String> / <Index reaches Zero>
      for (pcPos = strchr(c_pacStr, '\0'); (pcPos >= c_pacStr) && nIndex; pcPos--)
      {
        // Check Character [No Match => Ignore]
        if (*pcPos == cFind)
        {
          // Match: Update & Check Index
          if (!(++nIndex))
          {
            // Update Result [n-th Occurrence found]
            pcRes = pcPos;
          }
        }
      }
    }
  }
  
  // Return Result
  return pcRes;
}

/*----------------------------------------------------------------------------*/
int STR_sprintf(char **ppacBuf, const char *c_pcFmt, ... )
{
  int      nLen = 0;
  uint32   ulOffs = 0;
  va_list  pArgs1;
  
  va_start(pArgs1, c_pcFmt);
  nLen = vsnprintf(0, 0, c_pcFmt, pArgs1) + 1;
  if (*ppacBuf)
    ulOffs = strlen(*ppacBuf);
  *ppacBuf = HEAP_REALLOC(*ppacBuf, nLen + ulOffs);
  nLen = vsnprintf(*ppacBuf + ulOffs, nLen, c_pcFmt, pArgs1);
  va_end(pArgs1);

  return nLen;
}

/*----------------------------------------------------------------------------*/
uint32 STR_ParseInt(char *pacStr, uint8 unLength)
{
  uint32   ulValue;
  char    *pcTemp = STR_cpy(pacStr);

  // check boundaries
  ASSERT(unLength);
  ASSERT(strlen(pacStr) >= unLength);

  pcTemp[unLength] = 0;
  ulValue = atoi(pcTemp);
  HEAP_FREE(pcTemp);
  return ulValue;
}

/*---------------------------------------------------------------------------*/
bool STR_ParseKey_Range(const char *c_pcKey, const char *c_pcBuf, uint32 *pulStart, uint32 *pulEnd)
{
  // Example: "Rev:1041"  -> *pulStart = *pulEnd = 1041
  //          "Rev:1-99"  -> *pulStart = 1; *pulEnd = 99
  uint16   unKeyLen = strlen (c_pcKey);
  char    *pcKeyPos = strstr (c_pcBuf, c_pcKey);

  // if "key" found within string c_pcBuf...
  if (pcKeyPos)
  {
    char *pcEndPtr;

    // do two times a strtol, if there is a "-" between the two numbers.
    *pulStart = *pulEnd = strtol(pcKeyPos + unKeyLen, &pcEndPtr, 10);
    if (pcEndPtr[0] == '-')
      *pulEnd = strtol(pcEndPtr + 1, NULL, 10);

    // yes, "key" found
    return true;
  }

  // "key" not found
  return false;
}

/*----------------------------------------------------------------------------*/
void STR_memset(void *pDest, const void *pPattern, uint16 unBufSz, uint16 unPatSz)
{
  uint16  unI = 0;

  for (unI = 0; unI < unBufSz; unI++)
    ((uint8 *)pDest)[unI] = ((uint8 *)pPattern)[unI % unPatSz];
}

/*----------------------------------------------------------------------------*/
char *STR_ncpy(const char *c_paucStr, size_t uiSize)
{
  char *pacBuf = HEAP_MALLOC(uiSize);
  memcpy(pacBuf, c_paucStr, uiSize);

  return pacBuf;
}

/*----------------------------------------------------------------------------*/
uint8 *STR_AddPadding(uint8 *pData, size_t unInputSize, size_t *unOutputLength)
{
  int  nPos = 0;

  // Define the Number of Padding Bytes needing to be added

  // --> When the block size is 16 Bytes how many the padding is required ?
  uint16 unPaddingSize = PADDING_BLOCK_LENGTH - (unInputSize % PADDING_BLOCK_LENGTH); 
  uint16 unOutputSize = unInputSize + unPaddingSize;
  *unOutputLength = unOutputSize;

  // Add Padding Bytes [Resize Memory Block addressed by the previously allocated Pointer]
  pData = HEAP_REALLOC(pData, (unOutputSize) * sizeof(uint8));

  if (pData != NULL) 
  {
    for (nPos = unInputSize; nPos < unOutputSize; nPos++)
      pData[nPos] = unPaddingSize;
  }

  return pData;
}

/*----------------------------------------------------------------------------*/
uint8 *STR_RemovePadding(uint8 *pData, size_t unInputSize, size_t *unOutputSize)
{
  uint8 ucPaddingSize = pData[unInputSize - 1];
  if (ucPaddingSize <= 0x10)
  {
    *unOutputSize = unInputSize - ucPaddingSize;
    pData = HEAP_REALLOC(pData, (*unOutputSize) * sizeof(uint8));
  }
  else
  {
    *unOutputSize = unInputSize;
  }

  return pData;
}

/*----------------------------------------------------------------------------*/
uint8 *STR_Prepend(uint8 *paucPrepend, size_t unPrependSize, uint8 *paucData, size_t unDataSize, size_t* unOutputSize)
{
  *unOutputSize = unPrependSize + unDataSize;
  paucData = HEAP_REALLOC(paucData,(*unOutputSize) *  sizeof(uint8));
  memmove(paucData + unPrependSize, paucData, unDataSize);
  memmove(paucData, paucPrepend, unPrependSize);
  
  return paucData;
}

/*----------------------------------------------------------------------------*/
void STR_RemoveSubString(char *pcInString, const char *c_pcToRemove)
{
  uint32 ulLen = strlen(c_pcToRemove);
  do
  {
    pcInString = strstr(pcInString, c_pcToRemove);
    if (pcInString)
      memmove(pcInString, pcInString + ulLen, 1 + strlen(pcInString + ulLen));
  }
  while (pcInString);
}

/*----------------------------------------------------------------------------*/
void STR_Init(void)
{
#if SHELL_STR > 0
  // Register Shell Command
  mSTRING_InitShell();
#endif
}


/*----------------------------------------------------------------------------*/
// SHELL Interface
/*----------------------------------------------------------------------------*/

#if SHELL_STR > 0
  #include "SHELL.h"

  /*--------------------------------------------------------------------------*/
  /// this text will be displayed, in the shell-command overview 
  static const char mc_acSummaryString[] =
    "Functional description of STRING-Shell-Modul. (not yet implemenetd)\r\n";

  /// this text will be displayed, if this module return E_SHELL_CMD_WRONG_PARAMETERS 
  static const char mc_acUsageString[] =
    "str <pad>\r\n"
    "  pad: filename (may include an absolut/relative path) to copy from.\r\n"
    "  upad:   filename (may include an absolut/relative path) to copy to.\r\n"
    "\r\n"
    "Examples: 'str pad qwerty\r\n"
    "          'str unpad qwerty0000000000\r\n";

  /*--------------------------------------------------------------------------*/
  /** @brief      this function is called, if a registered command of this module is
                  entered in the shell

      @param[in]  c_ptContext    entered Text
      @return     ulRet           0 (No error)
                                  E_SHELL_CMD_WRONG_PARAMETERS (Parameter doesn't match)
      @author     A.Bader                                   @date 21.05.2014  */
  /*--------------------------------------------------------------------------*/
  static size_t Size = 4;

  static uint32 mSTRING_ShellString(const t_ShellCmdContext *c_ptContext)
  {
    int     i = 0;
    uint32  ulErr = 0;

    if (c_ptContext->ucNumParams == 0)
    {
      fprintf(c_ptContext->pfOut, "Info\r\n");
    }
    else if (SHELL_CheckCmdParam(c_ptContext, "pad", 1))
    {
      uint8   *Data = NULL;
      size_t  InputSize = Size;
      size_t  OutputSize = Size;
      
      // Set Data (the data size increases by one each call)

      Data = HEAP_MALLOC(Size);

      // Setup Data Pattern
      for (i = 0; i < Size; i++)
      {
        Data[i] = i;
      }
      // Display Input
      fprintf(c_ptContext->pfOut, "Data[%d]:[", InputSize);
      for (i = 0; i < InputSize; i++)
      {
        fprintf(c_ptContext->pfOut, "%.2X ", Data[i]);
      }

      fprintf(c_ptContext->pfOut, "]\r\n");

      // Padding
      fprintf(c_ptContext->pfOut, "Padding\r\n");
      Data = STR_AddPadding((uint8*)Data, InputSize, &OutputSize);

      // Display Output
      fprintf(c_ptContext->pfOut, "Data[%d]:[", OutputSize);
      for (i = 0; i < OutputSize; i++)
      {
        fprintf(c_ptContext->pfOut, "%.2X ", Data[i]);
      }

      fprintf(c_ptContext->pfOut, "]\r\n");

      // Un-Padding
      fprintf(c_ptContext->pfOut, "Un-Padding\r\n");
      InputSize = OutputSize;
      Data = STR_RemovePadding(Data, InputSize, &OutputSize);
      // Display Output
      fprintf(c_ptContext->pfOut, "Data[%d]:[", OutputSize);
      for (i = 0; i < OutputSize; i++)
      {
        fprintf(c_ptContext->pfOut, "%.2X ", Data[i]);
      }

      fprintf(c_ptContext->pfOut, "]\r\n");

      HEAP_FREE(Data);
      Size++;
      
    }
    //else if (SHELL_CheckCmdParamEx(c_ptContext, 2, 4, 0, "cmd2"))
    //{
    //  --- cmd2, 3 or 5 parameters
    //  ; // add functionality
    //}
    else if (c_ptContext->ucNumParams == 0)
    {
      ; // grow old
    }
    else
    {
      // mc_acUsageString will be displayed
      ulErr = E_SHELL_CMD_WRONG_PARAMETERS;
    }

    return ulErr;
  }

  /*--------------------------------------------------------------------------*/
  /** @brief      Register Shell Commands of STRING Module
   
      @param      None
      @return     None

      @author     A.Bader                                   @date 21.05.2014  */
  /*--------------------------------------------------------------------------*/
  static void mSTRING_InitShell(void)
  {
    // use module name as parameter or check with shell-cmd "help" that parameter isn't used
    SHELL_AddCommand("str", mSTRING_ShellString, mc_acSummaryString, mc_acUsageString);
  }

#endif   // SHELL_STR

/// @}   // End of Group

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
