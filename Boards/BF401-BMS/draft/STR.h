#ifndef __STRING_H__
#define __STRING_H__

/** @defgroup   STRING  STRING-Group
                Implementation of various string functions.
    @{
********************************************************************************
*   @name       STRING                                                           
*   @file       STRING.h                                                         
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

/*== Includes ===============================================================*/
#include "Target.h"

#include "LIST.h"


/*== Defines =================================================================*/
#define PADDING_BLOCK_LENGTH 16


/*== Enumerations ============================================================*/


/*== Structures ==============================================================*/


/*== Module Interfaces =======================================================*/

/*----------------------------------------------------------------------------*/
/** @brief      Does a wildcard string compare. Supported are the wildcards
                "*" and "?"
   
    @param[in]  c_pcWild    pointer to buffer.
    @param[in]  c_pcString  size of data stored at pcBuf.
    @return     true, if pattern matches

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
bool STR_wildcmp(const char *c_pcWild, const char *c_pcString);

/*----------------------------------------------------------------------------*/
/** @brief      Returns uint32 value of a string of max 4 bytes length
                (space padded)
                Example: "abc " => 0x61626320
                  
    @param[in]  c_pcStr    pointer to string (may be 0-terminated, if shorter
                           than 4 chars).
    @return     value

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
uint32 STR_toUL(const char *c_pcStr);

/*----------------------------------------------------------------------------*/
/** @brief      Delete a sub string in a given string.
   
    @param[in]  pcStr       pointer to string to modify
    @param[in]  c_pcSearch  pointer to string to search
    @return     pointer to pcStr

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
char *STR_delete(char *pcStr, const char *c_pcSearch);

/*----------------------------------------------------------------------------*/
/** @brief      Search for substring in a string and replace any occurences
                by the given replace string.
   
    @param[in]  pcStr        pointer to string to modify
    @param[in]  c_pcSearch   pointer to string to search for
    @param[in]  c_pcReplace  pointer to string to replace search string
    @param[in]  unMaxLen     if new string is longer than original string,
                             avoid modification, if new length is greater
                             than unMaxLen.
    @return     pointer to pcStr

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
char *STR_replace(char *pcStr, const char *c_pcSearch, const char *c_pcReplace, uint16 unMaxLen);

/*---------------------------------------------------------------------------*/
/** @brief      search in pcStr for c_pcSearch and replace all occurrences
                by c_pcReplace.
                pcStr is enlarged / shrinked as needed!

    @param[in]  pcStr       input string, which will be modified.
                            This buffer must be allocated on HEAP because
                            HEAP_REALLOC is used internally to enlarge/shrink
                            buffer, if needed!
    @param[in]  c_pcSearch  points to 0-terminated string to search
    @param[in]  c_pcReplace points to 0-terminated string to replace
    @return     pcStr (may be different than input pcStr, if reallocated!)

    @author     A.Bader                                     @date 25.06.2014  */
/*----------------------------------------------------------------------------*/
char *STR_replace_dyn(char *pcStr, const char *c_pcSearch, const char *c_pcReplace);

/*----------------------------------------------------------------------------*/
/** @brief      Search for a list of strings in a given string and replace
                any occurences by the given replacement.
   
    @param[in]  pcStr         pointer to string to modify
    @param[in]  c_apcSearch   pointer to list of strings to search for
                              (null terminated)
    @param[in]  c_apcReplace  pointer to list of strings to replace search
                              strings with.
    @param[in]  unMaxLen      if new string is longer than original string,
                              avoid modification, if new length is greater
                              than unMaxLen.
    @return     pointer to pcStr

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
char *STR_replace_list(char *pcStr, const char *c_apcSearch[], const char *c_apcReplace[], uint16 unMaxLen);

/*----------------------------------------------------------------------------*/
/** @brief      Split string by a given separator list or max item length.
                The list is returned in a "dynamic list". Call HEAP_FREE as
                soon as the list is no more needed! Note: The separated
                strings are terminated with a 0!
   
    @param[in]  c_pcStr      pointer to input string
    @param[in]  c_apcSearch  pointer to list of strings to use as separator
    @param[in]  unItemLen    Max length of one item. An item is splittet
                             add this position, if no separator matched before.
    @param[in]  unItemCount  Max count of splited elements to allocate
    @return     pointer to dynamic list object

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
t_List *STR_split(const char *c_pcStr, const char *c_apcSearch[], uint16 unItemLen, uint16 unItemCount);

/*----------------------------------------------------------------------------*/
/** @brief      Create a new string without whitespaces
               (characters <= ASCII Code 32) on the left and/or on the right
               side. The new string is allocated on the HEAP and must therefore
               be freed by the user. The new string is 0-terminated.
   
    @param[in]  c_pcStr  pointer to input string
    @param[in]  bLeft    true: clear whitespaces on left
    @param[in]  bRight   true: clear whitespaces on right
    @return     pointer to new string allocated on HEAP

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
char *STR_clear_whitespaces(const char *c_pcStr, bool bLeft, bool bRight);

/*----------------------------------------------------------------------------*/
/** @brief      Create new buffer and read one text line from c_paucStr. The
                line can be terminated by \r and/or \n. The function may be
                used to loop through a input buffer until ppNextStr is NULL!
   
    @param[in]  c_paucStr   pointer to input buffer containing null terminated
                            list of text lines (e.g. a text file).
    @param[out] ppNextStr   pointer to next line. NULL, if end of c_paucStr.
    @return     pointer to created line buffer. MUST BE FREED BY CALLER!

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
char *STR_GetStringLine(const char *c_paucStr, const char **ppNextStr);

/*---------------------------------------------------------------------------*/
/** Replace '\n' and '\r' each by '\0'.

  @param    pcBuf   [in] pointer to buffer.
  @param    unSize  [in] size of data stored at pcBuf
  @version  2.00, 18.10.18, grj, port from MISC G6 to STR G7                 */
/*          1.00, 17.03.09, aba ,  1. Release                                */
/*          Version  Date   Name    Description                              */
/*---------------------------------------------------------------------------*/
void *STR_ClearNewlines (char *pcBuf, uint16 unS);


/*----------------------------------------------------------------------------*/
/** @brief      Copy the string pointed to by c_paucStr to a new allocated
                memory block and return a pointer to this new block.
                User must free this block later on!
   
    @param[in]  c_paucStr   pointer to null terminated string
    @return     pointer to new string. MUST BE FREED BY CALLER!

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
char *STR_cpy(const char *c_paucStr);

/*----------------------------------------------------------------------------*/
/** @brief      Append the given strings to the pacStr. There may be multiple
                parameters. pacStr is enlarged dynamically to the right size.
   
    @param[in]  pacStr       existing string or NULL
    @param[in]  nNumStrings  number of following const char * parameters
    @return     pointer to whole string. MUST BE FREED BY CALLER!

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
char *STR_cat(char *pacStr, int nNumStrings, ...);

/*----------------------------------------------------------------------------*/
/** @brief      Find the n-th occurrence of a given character within c_pacStr
                and return a pointer to it. Positive values for nIndex will
                start search from the beginning, negative values will start
                search at the end of the string. If the character requested
                is not found, the function returns NULL.
   
    @param[in]  c_pacStr  String to search character within
    @param[in]  cFind     Character to search for
    @param[in]  nIndex    Occurrence index [beginning / end]
    @return      Pointer to character [if found] / NULL [otherwise]

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
char *STR_chr(const char *c_pacStr, const char cFind, int16 nIndex);

/*---------------------------------------------------------------------------*/
/** @brief      Read a string as raw hex values and copy it as raw buffer
                into the returned allocated buffer.
                If a digit is invalid, 0 will be placed in output buffer.

    @param[in]  c_pacStr  input string containing ascii hex values
    @param[out] pulLen    length of output buffer (= strlen(c_pacStr) / 2)
    @return     allocated buffer. NEEDS TO BE FREED BY USER!

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
uint8 *STR_AsciiHexToBuf(const char *c_pacStr, uint32 *pulLen);

/*---------------------------------------------------------------------------*/
/** @brief      Read a string as raw hex values and copy it as raw buffer
                into the returned allocated buffer.
                Check the digits. As soon as an invalid digit arrives,
                the function terminates without processing this digit.
                The pulLen will be the number of correct processed
                bytes.

    @param[in]  c_pacStr  input string containing ascii hex values
    @param[out] pulLen    length of output data
    @return     allocated buffer. NEEDS TO BE FREED BY USER!

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
uint8 *STR_AsciiHexToBufSafe(const char *c_pacStr, uint32 *pulLen);

/*---------------------------------------------------------------------------*/
/** @brief      Writes a binary buffer to a string containing all hex values
                of the binary buffer.

    @param[in]  c_paucBuf binary buffer
    @param[in]  ulBufLen  length of binary buffer
    @return     allocated buffer. NEEDS TO BE FREED BY USER!

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
char *STR_BufToAsciiHex(const uint8 *c_paucBuf, uint32 ulBufLen);

/*---------------------------------------------------------------------------*/
/** @brief      sprintf compatible function, which writes content to a dynamic
                allocated/reallocated buffer instead of a fixed one.
                If the buffer is already allocated, the new string is appended.

    @param[out] ppacBuf  pointer to pointer of created buffer (to return
                         allocated block).
                         *ppacBuf may by NULL: in this case, it will be created.
                         if *ppacBuf is not NULL, new content is appended, the
                         buffer is reallocated to new size. ATTENTION: if *ppacBuf
                         is not NULL, it must be a pointer allocated with
                         HEAP_XXALLOC!
    @param[in]  c_pcFmt  format string
    @return     length of appended string.

    @author     A.Bader                                     @date 25.06.2014  */
/*----------------------------------------------------------------------------*/
int STR_sprintf(char **ppacBuf, const char *c_pcFmt, ... );

/*----------------------------------------------------------------------------*/
/** @brief      converts a string to upper case.
                Please note that the origin string is replaced by the converted one

    @param[in]  pacStr      string to convert
    @return     converted string (pointer to pacStr)

    @author     S.Schüpbach                                 @date 28.07.2017  */
/*----------------------------------------------------------------------------*/
char *STR_ToUpperCase(char *pacStr);

/*----------------------------------------------------------------------------*/
/** @brief      converts a string to lower case.
                Please note that the origin string is replaced by the converted one

    @param[in]  pacStr      string to convert
    @return     converted string (pointer to pacStr)

    @author     S.Schüpbach                                 @date 28.07.2017  */
/*----------------------------------------------------------------------------*/
char *STR_ToLowerCase(char *pacStr);

/*----------------------------------------------------------------------------*/
/** @brief      Converts a string to a number with the possibility to parse a defined length only
                e.g. STR_ParseInt("123456", 3) -> 123

    @param[in]  pacStr      string to convert
    @param[in]  unLength    the number of processed letters
    @return     converted string (pointer to pacStr)

    @author     S.Schüpbach                                 @date 31.01.2018  */
/*----------------------------------------------------------------------------*/
uint32 STR_ParseInt(char *pacStr, uint8 unLength);

/*---------------------------------------------------------------------------*/
/** @brief Parses for a string called "key" in another string. After this key,
           an ASCII-Coded integer value or a value range is expected.
           *pulStart is set to the ASCII-Coded value after key; if there
           is a "-" afterwards, a second value is read and stored in *pulEnd; 
           else, *pulEnd is set to *pulStart.

  @param    c_pcKey   [in] pointer to buffer.
  @param    c_pcBuf   [in] size of data stored at pcBuf
  @param    pulStart  [out] size of data stored at pcBuf
  @param    pulEnd    [out] size of data stored at pcBuf
  @return   true, if "key" found; otherwise false
  @version    1.00, 25.03.09, aba ,  1. Release                              */
/*          Version  Date   Name    Description                              */
/*---------------------------------------------------------------------------*/
bool STR_ParseKey_Range (const char *c_pcKey, const char *c_pcBuf, uint32 *pulStart, uint32 *pulEnd);

/*---------------------------------------------------------------------------*/
/** Fills a buffer with a given pattern.

  @param      pDest         [out]  Pointer to destination buffer
  @param      pPattern      [in]   Pointer to pattern buffer
  @param      unBufSz       [in]   Size of buffer in bytes
  @param      unPatSz       [in]   Size of pattern in bytes
  @version    1.00, 12.09.08, aba ,  1. Release                              */
/*          Version  Date   Name    Description                              */
/*---------------------------------------------------------------------------*/
void STR_memset (void *pDest, const void *pPattern, uint16 unBufSz, uint16 unPatSz);

/*---------------------------------------------------------------------------*/
/** @brief PKCS#7 Padding - Fills a the end of a buffer with a pattern.
    
    Detailed description.
    Padding is in whole bytes. 
    The value of each added byte is the number of bytes that are added, 
    i.e. N bytes, each of value N are added. 
    The number of bytes added will depend on the block boundary to which the message needs to be extended.
  
    The padding will be one of:
    
    01
    02 02
    03 03 03
    04 04 04 04
    05 05 05 05 05
    06 06 06 06 06 06
    07 07 07 07 07 07 07 
    08 08 08 08 08 08 08 08
    09 09 09 09 09 09 09 09 09
    0A 0A 0A 0A 0A 0A 0A 0A 0A 0A
    0B 0B 0B 0B 0B 0B 0B 0B 0B 0B 0B
    0C 0C 0C 0C 0C 0C 0C 0C 0C 0C 0C 0C
    0D 0D 0D 0D 0D 0D 0D 0D 0D 0D 0D 0D 0D
    0E 0E 0E 0E 0E 0E 0E 0E 0E 0E 0E 0E 0E 0E
    0F 0F 0F 0F 0F 0F 0F 0F 0F 0F 0F 0F 0F 0F 0F
    10 10 10 10 10 10 10 10 10 10 10 10 10 10 10 10
  
    @param[in,out] pData       Pointer to destination buffer
    @param[in]     unInputSize Size of the original input buffer in bytes: 
                   The range is between 0 to USHRT_MAX.                       */
 /*---------------------------------------------------------------------------*/
uint8* STR_AddPadding(uint8* pData, size_t unInputSize, size_t* unOutputLenght);
/*---------------------------------------------------------------------------*/
/** @brief PKCS#7 Padding - Remove the padding at the end of a buffer.
    @param[in,out] pData        Pointer to destination buffer
    @param[in]     unInputSize  InputSize
    @param[out]    unOutputSize OutPutSize (can't be NULL)                    */
 /*---------------------------------------------------------------------------*/
uint8* STR_RemovePadding(uint8 *pData, size_t unInputSize, size_t* unOutputSize);

/*----------------------------------------------------------------------------*/
/** @brief      Prepend a string to an other

    @param[in]  paucPrepend   data to prepend
    @param[in]  unPrependSize Size of Data to Prepend
    @param[in]  paucData      data
    @param[in]  unDataSize    Size of Orig Data
    @param[out] unOutputSize  Size of output data
    @return     output data   [Pointer to paucData] must be freed by caller

    @author     J.Grossenbacher                             @date 18.06.2019  */
/*----------------------------------------------------------------------------*/
uint8 *STR_Prepend(uint8 *paucPrepend, size_t unPrependSize, uint8 *paucData, size_t unDataSize, size_t* unOutputSize);

/*----------------------------------------------------------------------------*/
/** @brief      Remove a string from an other

    @param[in]  pcInString     Input String
    @param[in]  c_pcToRemove   String to Remove
    @return     NONE

    @author     J.Grossenbacher                             @date 17.09.2019  */
/*----------------------------------------------------------------------------*/
void STR_RemoveSubString(char *pcInString, const char *c_pcToRemove);

/*----------------------------------------------------------------------------*/
/** @brief      Initialize STR module. Currently, only Shell-Command is
                registered (if enabled).

    @param      None
    @return     None

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
void STR_Init(void);

/// @}   // End of Group

#endif   // __STRING_H__
