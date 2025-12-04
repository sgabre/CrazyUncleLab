/** @addtogroup   SHELL  
    @{
********************************************************************************
*   @name       SHELL
*   @file       SHELL.c
*
*   @brief      Command Line Interpreter.
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

#ifdef RTE_CMSIS_RTOS2
  #include "rtx_os.h"
#endif

#include "HWF.h"
#include "HWIN.h"
#include "LIST.h"
#include "STR.h"
#include "DBG.h"
#include "FS.h"
#include "STDIO.h"

#include "SHELL.h"
#include "RDP.h"


// Include the whole module only in case Shell is activated in ProjectConfig.h
#if SHELL

/*== Defines =================================================================*/

#define LINE_BUF_SIZE       256
#define MAX_NUM_PARAMS       32
#define MAX_FILENAME_LEN    256
#define MAX_COMMANDS        250

// Internal Error Code, if Redirection File could not be opened
#define E_SHELL_OPEN_REDIR_FILE     1

#define SHELL_MAX_CMD_HISTORY       20

#define SHELL_CMD_HISTORY_FILE_NAME     "Shell_cmd_history.temp"

#ifdef  __MDBBORD
  #define  DEFAULT_DISK  "R:"
  #define  DISK_TYPE  "RAM "
#else
  #define  DEFAULT_DISK  "M:"
  #define  DISK_TYPE  ""
#endif


/*== Enumerations ============================================================*/
enum
{
  SHELL_FLAG_ACTIVE = 1 << 0
};


/*== Structures ==============================================================*/

/// entry, which is added to the Shells available command list (t_List).
typedef struct
{
  char           acName[16];     ///< a command must not be longer than 15 chars
  pfCmdHandler   pfHandler;      ///< function pointer of handler
  const char    *c_pacSummary;   ///< help text for summary
  const char    *c_pacUsage;     ///< help text for usage (parameter description)
} t_CommandEntry;


/*== Module Constants ========================================================*/
static const char mc_acSeparatorLine[] = "--------------------------------------------------------------------------------\r\n";

static const char mc_acSummaryHelp[] =
  "Displays the help of a command and/or the list of all available commands.\r\n";

static const char mc_acUsageHelp[] =
  "help [command]\r\n"
  "  command (optional): If specified, displays the help summary and usage of this command\r\n"
  "                      If not specified, displays a list of all commands and their help\r\n"
  "                      summary.\r\n"
  "                      command may also contain wildcards (* or ?) - in this case, all\r\n"
  "                      help summaries of matching commands are displayed.\r\n";

#ifdef RTE_FileSystem_Core
static const char mc_acSummaryLs[] =
  "Displays the file/folder list of current directory or of a specified directory.\r\n"
  "and specified search mask.\r\n";
static const char mc_acUsageLs[] =
  "ls [searchmask]\r\n"
  "  searchmask (optional): Path and searchmask. If not specified, current dir\\* is taken\r\n"
  "\r\n"
  "Examples: 'ls' - list all entries in current directory\r\n"
  "          'ls subdir\\*.txt' - list all text files in subdir\r\n"
  "          'ls U:\\dir\\aa*' - list all entries on memory stick in folder dir, starting with 'aa'\r\n";

static const char mc_acSummaryCd[] =
  "Change current directory. May be a relative or absolut path.\r\n";
static const char mc_acUsageCd[] = 
  "cd [dir]\r\n"
  "  dir (optional): Absolut path (if beginning with a drive) or relative path to change to.\r\n"
  "                  '..' is supported to go one directory level back.\r\n"
  "                  If not specified, go to standard directory, which is 'M:'\r\n"
  "\r\n"
  "Examples: 'cd U:' (may also be 'U:\\')\r\n"
  "          'cd M:config\\periph' (may also be 'M:\\')\r\n"
  "          'cd ..\r\n"
  "          'cd ..\\subfolder\r\n";

static const char mc_acSummaryCreate[] = 
  "Creates a new file with given content.\r\n";
static const char mc_acUsageCreate[] =
  "create <file> <content>"
  "  file: filename (may include an absolut/relative path) to create. If directory does not exist, it will\r\n"
  "        be created.\r\n"
  "  content: text content to write to file.\r\n";

static const char mc_acSummaryCat[] =
  "Displays the content of a file, if specified, as hex dump.\r\n";
static const char mc_acUsageCat[] =
  "cat <file> [-h]\r\n"
  "  file: filename (may include an absolut/relative path) to output\r\n"
  "  -h (optional): if specified, display as hex dump\r\n";

static const char mc_acSummaryCp[] =
  "Copy a file from source to destination.\r\n";
static const char mc_acUsageCp[] =
  "cp <source> <dest>\r\n"
  "  source: filename (may include an absolut/relative path) to copy from.\r\n"
  "  dest:   filename (may include an absolut/relative path) to copy to.\r\n"
  "\r\n"
  "Examples: 'cp M:\\folder\\file.bin U:\\MyDir\\file.bin' (SD-Card -> Memory-Stick)\r\n"
  "          'cp ..\\..\\myfile.txt ..\\..\\..\\folder\\mynewfile.txt'\r\n";

static const char mc_acSummaryRm[] =
  "Delete one or more file(s).\r\n";
static const char mc_acUsageRm[] =
  "rm <file mask>\r\n"
  "  file mask:  File name to delete. May contain wildcards.\r\n"
  "              directories won't be deleted. Use rmdir to\r\n"
  "              remove directories.\r\n";

static const char mc_acSummaryRmdir[] =
  "Delete a directory recursively.\r\n";
static const char mc_acUsageRmdir[] =
  "rmdir <dir>\r\n"
  "  dir:  Directory to delete. All files/folders/subfolders\r\n"
  "        are deleted.\r\n"
  "        NOTE: Due to a bug in RL-FlashFS, it does not work correctly at the moment!\r\n";

static const char mc_acSummaryMkdir[] =
  "Create a directory.\r\n";
static const char mc_acUsageMkdir[] =
  "mkdir <dir>\r\n"
  "  dir:  Directory to create.\r\n"
  "        Since this operation is not supported directly, first, a temp-\r\n"
  "        file is created in this directory and then deleted.\r\n";

static const char mc_acSummaryFormat[] =
  "Format the current file system.\r\n";
static const char mc_acUsageFormat[] =
  "format [label]\r\n"
  "  label: if label given, set this volume lable. Otherwise do not touch it.\r\n";

static const char mc_acSummaryDf[] =
  "Display Drive's free space.\r\n";
static const char mc_acUsageDf[] =
  "df\r\n";

static const char mc_acSummaryMem[] =
  "Read or write from/to a specific address.\r\n";
static const char mc_acUsageMem[] =
  "mem <subcommand> [...]\r\n"
  "  subcommands:\r\n"
  "    read <addr> <length> - dumpout <length> bytes at <addr> from memory.\r\n"
  "    write <addr> <data>  - writes the given <data> to <addr> in memory.\r\n"
  "                           <data> is a string containing hex digits.\r\n"
  "    fill <addr> <len> <byte> - fill the given address range with <byte>.\r\n"
  "\r\n"
  "Examples:  'mem write 0x20000000 AABBCCDDEEFF'\r\n"
  "           'mem read 0x2000CAE8 28'\r\n";
#endif

static const char mc_acSummaryKill[] =
  "Terminates a thread with given osThreadId.\r\n";
static const char mc_acUsageKill[] =
  "kill <osThreadId>\r\n"
  "  <osThreadId> threadId - may be retrieved with 'osext'\r\n"
  "               note: this is not the TID (task id)!\r\n";

static const char mc_acSummaryReboot[] =
  "Reboot device.\r\n";
static const char mc_acUsageReboot[] =
  "reboot\r\n";


/*== Function Prototypes =====================================================*/
static void tSHELL_Handler(void *arg);
static void tSHELL_SubThread(void *arg);


/*== Module Variables ========================================================*/
static char                 *m_pacDir = NULL;
static t_List               *m_ptCmdList = NULL;
static pt_OsextMutexId       m_ptOsMutIdCmdList;
static pt_OsextEventFlagId   m_ptOsEventActive;


/*============================================================================*/
/*== Static Module Functions =================================================*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/
static int mSHELL_SortCommandListCB (const void *a, const void *b)
{
  t_ListEntry     *pucA = (t_ListEntry *)a;
  t_ListEntry     *pucB = (t_ListEntry *)b;
  t_CommandEntry  *ptEA = (t_CommandEntry *)pucA->pucEntry;
  t_CommandEntry  *ptEB = (t_CommandEntry *)pucB->pucEntry;

  return (strcmp(ptEA->acName, ptEB->acName));
}

/*----------------------------------------------------------------------------*/
static void mSHELL_SortCommandList(void)
{
  uint16  unCount = LIST_ItemCount(m_ptCmdList);
  qsort(m_ptCmdList->patEntries, unCount, sizeof(t_ListEntry), mSHELL_SortCommandListCB);
}

/*----------------------------------------------------------------------------*/
static void mSHELL_DisplayProgramInfo(void)
{/*
  printf("\r\n%s linked %s %s\r\n", PRJV_GetFullNameString(), PRJV_GetLinkDateString(), PRJV_GetLinkTimeString());
  printf("%s, ID101: %d\r\n", HWF_GetHwStr(E_HWSTRING_DEFAULT), HWIN_GetID101());
  printf("JTAG Status: %s\r\n", (JTAG_Status() ? "enabled" : "disabled"));
	*/
  printf("Programm Info");
}

/*----------------------------------------------------------------------------*/
static void mSHELL_DisplayPrompt(void)
{
  printf("\r\n\r\n/micSHell/>%s>>", m_pacDir);
}

/*----------------------------------------------------------------------------*/
static void mSHELL_SetDefaultDrive(void)
{
  strcpy(m_pacDir, DEFAULT_DISK);
}

/*----------------------------------------------------------------------------*/
uint32 mSHELL_Exec(const char *c_pacCmdLine)
{
  uint32  ulErr = 0;

  // used to split the string into a space delimited list
  const char  *c_apacSearch[] = {" ", NULL};

  // split the command line
  t_List  *ptLstParams = STR_split(c_pacCmdLine, c_apacSearch, LINE_BUF_SIZE, MAX_NUM_PARAMS + 1);

  bool  bOwnThread = false;
  
  // Check, if the last "parameter" starts with a "&" - this would cause to start the function in an own thread.
  // Perform the check before "parameter grouping" to be able to easily escape a real & parameter by "&".
  if (LIST_ItemCount(ptLstParams) >= 1)
  {
    uint16   unLastItem = LIST_ItemCount(ptLstParams) - 1;
    char    *pacLastParam = (char *)LIST_GetItem(ptLstParams, unLastItem, NULL);
    if (pacLastParam[0] == '&')
    {
      uint32   ulPrio = 0;
      char    *pacCmdLine = NULL;

      sscanf(pacLastParam, "&(%d)", &ulPrio);
      if ((ulPrio < 1) || (ulPrio > 254))
        ulPrio = 20;

      // Delete &(x) from list
      LIST_DeleteItem(ptLstParams, unLastItem);

      pacCmdLine = LIST_ToString(ptLstParams, " ");

      // the space delimited list we no longer use
      LIST_Free(ptLstParams);

      // Create new thread
      OSEXT_ThreadNew(tSHELL_SubThread, pacCmdLine, "SHELL_SubThread", 1024,  ulPrio);

      bOwnThread = true;
    }
  }

  if (!bOwnThread)
  {
    t_ShellCmdContext   tContext;
    uint16              unI;
    bool                bQuoteActive = false;
    char               *pacCurParam = NULL;

    // build the context base - it will be overgiven to the called function handler
    // ptParams is a LIST, which will be filled below with the "parameter list" (parameters overgiven to the shell function)
    memset(&tContext, 0, sizeof(t_ShellCmdContext));
    tContext.pacCmdLine = (char *)c_pacCmdLine;
    tContext.pacCurrentDir = STR_cpy(m_pacDir);
    tContext.ptParams = LIST_Create(MAX_NUM_PARAMS);
    tContext.pfIn = stdin;
    tContext.pfOut = stdout;

    // go through each element of the space delimited command line
    // and fill our parameter list -> group parameters, which are surrounded by "..."
    for (unI = 0; unI < LIST_ItemCount(ptLstParams); unI++)
    {
      // The first element is the command itself -> copy it as c_pacCommand
      if (unI == 0)
        tContext.pacCommand = STR_cpy((char *)LIST_GetItem(ptLstParams, unI, NULL));
      else
      {
        // now parse for open quotes (the first character of a "parameter"
        // and closing quotes (the last character of a paramter)
        // -> combine space delimited elements into one parameter, if quotes are around them
        const char  *c_pacParam = (char *)LIST_GetItem(ptLstParams, unI, NULL);
        if (!bQuoteActive)
        {
          if (c_pacParam[0] != '\"')
            // no " active -> just take the element and at it to our parameter list
            LIST_AddItem(tContext.ptParams, (uint8 *)c_pacParam, strlen(c_pacParam) + 1);
          else
            // quote active -> manage it below.
            bQuoteActive = true;
        }

        if (bQuoteActive)
        {
          // We have an opened quote.
          // just append the element to pacCurParam -> it will be added to our parameter list, as soon as the closing quote arrives
          uint16  unLen;
          pacCurParam = (pacCurParam == NULL) ? STR_cpy(c_pacParam + 1) : STR_cat(pacCurParam, 2, " ", c_pacParam);
          unLen = strlen(pacCurParam);
          if (pacCurParam[unLen - 1] == '\"')
          {
            // here we have the closing quote -> add the whole construct as single parameter (without last quote character)
            pacCurParam[unLen - 1] = 0;
            LIST_AddItem(tContext.ptParams, (uint8 *)pacCurParam, unLen);
            bQuoteActive = false;
            HEAP_FREE(pacCurParam);
            pacCurParam = NULL;
          }
        }
      }
    }

    // the space delimited list we no longer use
    LIST_Free(ptLstParams);

    // Check, if the last but one entry is a ">" or ">>" - in this case, we have to redirtect the output to a file instead of stdout
    if (LIST_ItemCount(tContext.ptParams) >= 2)
    {
      bool     bOutputRedirect = false;
      uint16   unLastItem = LIST_ItemCount(tContext.ptParams) - 1;
      char    *pacRedir = (char *)LIST_GetItem(tContext.ptParams, unLastItem - 1, NULL);
      char    *pacFile = (char *)LIST_GetItem(tContext.ptParams, unLastItem, NULL);

      // Operator > will rewrite a new file.
      // Operator >> will append to an existing file; if it does not exist, create a new one.
      if (strcmp(pacRedir, ">") == 0)
      {
        tContext.pfOut = fopen(pacFile, "w");
        bOutputRedirect = true;
      }
      else if (strcmp(pacRedir, ">>") == 0)
      {
        tContext.pfOut = fopen(pacFile, "a");
        bOutputRedirect = true;
      }

      if (bOutputRedirect)
      {
        if (!tContext.pfOut)
        {
          printf("\r\nERROR: Cannot open redirection file '%s'\r\n", pacFile);
          ulErr = E_SHELL_OPEN_REDIR_FILE;
        }
        else
        {
          // remove these two entries - the command shall not get these as parameters.
          LIST_DeleteItem(tContext.ptParams, unLastItem);
          LIST_DeleteItem(tContext.ptParams, unLastItem - 1);
        }
      }
    }

    if (!ulErr)
    {
      // Pointer to Command Entry in the dynamic LIST
      t_CommandEntry  *ptEntry = NULL;
      t_CommandEntry   tEntry;

      // set ucNumParams to have faster access to the number of parameters in the handler functions
      tContext.ucNumParams = LIST_ItemCount(tContext.ptParams);

      // Protect the Command List (no one shall be able to add a new command and resort the list...)
      OSEXT_MutexAcquire(m_ptOsMutIdCmdList, osWaitForever);

      // Search the command in the registered command set
      ptEntry = (t_CommandEntry *)LIST_SearchStrItem(m_ptCmdList, tContext.pacCommand, 0, NULL);
      memcpy(&tEntry, ptEntry, sizeof(t_CommandEntry));
      OSEXT_MutexRelease(m_ptOsMutIdCmdList);

      // If Command has been found in the list...
      if (ptEntry)
      {
        uint32  ulErr;

        printf("\r\n");

        // Command found -> execute it
        ulErr = tEntry.pfHandler(&tContext);

        // Check return code of called function
        if (ulErr)
        {
          if (ulErr == E_SHELL_CMD_WRONG_PARAMETERS)
            fprintf(tContext.pfOut, "\r\nERROR: Wrong/invalid parameters specified! Check usage below:\r\nUsage: %s\r\n", tEntry.c_pacUsage);
          else
            fprintf (tContext.pfOut, "\r\nERROR: '%s' has been terminated with return value %d (0x%X)\r\n", tEntry.acName, ulErr, ulErr);
        }
      }
      else
      {
        ulErr = E_SHELL_CMD_NOT_FOUND;

        // if just enter has been pressed -> do not display "command not found" error.
        if (strlen(c_pacCmdLine) > 0)
          printf("\r\nERROR: Command not found!");
      }
    }

    // if output was redirected to a file, close it!
    if (tContext.pfOut && tContext.pfOut != stdout)
      fclose(tContext.pfOut);    
    else
      clearerr(stdout);

    // Free all allocations used in handler context
    if (tContext.ptParams)
      LIST_Free(tContext.ptParams);
    if (tContext.pacCurrentDir)
      HEAP_FREE(tContext.pacCurrentDir);
    if (tContext.pacCommand)
      HEAP_FREE(tContext.pacCommand);
  }

  return ulErr;
}

#if SHELL_FILE  > 0

/*----------------------------------------------------------------------------*/
static char *mSHELL_GetDriveFromAbsPath(const char *c_pacPath, char *pacDriveBuf)
{
  pacDriveBuf[0] = c_pacPath[0];
  pacDriveBuf[1] = c_pacPath[1];

  if (c_pacPath[1] == ':')
  {
    pacDriveBuf[2] = 0;
  }
  else if (c_pacPath[2] == ':')
  {
    pacDriveBuf[2] = ':';
    pacDriveBuf[3] = 0;
  }
  else
    pacDriveBuf[0] = 0;

  return pacDriveBuf;
}

/*----------------------------------------------------------------------------*/
static char *mSHELL_BuildAbsolutePath(const char *c_pcBasePath, const char *c_pcPath)
{
  const char  *c_apcSep[] = {"\\", NULL};
  char        *pcPath = NULL;
  t_List      *ptList;
  uint32       ulIdx = 0;

  if ( (c_pcPath[1] == ':') || (c_pcPath[2] == ':') || (c_pcPath[0] == '/') )
  {
    // absolute path, if drive letter specified or '/' at beginning (own extensions)
    pcPath = STR_cpy(c_pcPath);
  }
  else
  {
    pcPath = STR_cat(NULL, 3, c_pcBasePath, "\\", c_pcPath);
  }

  // make drive letter uppercase
  if (pcPath[0] >= 97)
    pcPath[0] -= 32;

  // resolve special directories "." (current dir) and ".." (previous level)
  ptList = STR_split(pcPath + 2, c_apcSep, 100, 20);
  while (ulIdx < LIST_ItemCount(ptList))
  {
    if (strcmp((char *)LIST_GetItem(ptList, ulIdx, NULL), ".") == 0)
    {
      LIST_DeleteItem(ptList, ulIdx);
    }
    else if (strcmp((char *)LIST_GetItem(ptList, ulIdx, NULL), "..") == 0)
    {
      LIST_DeleteItem(ptList, ulIdx);

      if (ulIdx > 0)
      {
        ulIdx--;
        LIST_DeleteItem(ptList, ulIdx);
      }
    }
    else
      ulIdx++;
  }

  // build absolute path
  pcPath[2] = 0;
  for (ulIdx = 0; ulIdx < LIST_ItemCount(ptList); ulIdx++)
  {
    strcat(pcPath, (char *)LIST_GetItem(ptList, ulIdx, NULL));
    if (ulIdx < LIST_ItemCount(ptList) - 1)
      strcat(pcPath, "\\");
  }

  LIST_Free(ptList);

  return pcPath;
}

/*----------------------------------------------------------------------------*/
static uint32 mSHELL_CmdFormat(const t_ShellCmdContext *c_ptContext)
{
  uint32  ulErr = 0;

  if (c_ptContext->ucNumParams <= 1)
  {
    char  *pacLabel = NULL;
    char  *pacOptions = NULL;

    if (c_ptContext->ucNumParams == 1)
      pacLabel = (char *)LIST_GetItem(c_ptContext->ptParams, 0, NULL);
    if (pacLabel)
      STR_sprintf(&pacOptions, "/L %s", pacLabel);

    fprintf(c_ptContext->pfOut, "Formatting...\r\n");
    ulErr = fformat(m_pacDir, pacOptions);

    HEAP_FREE(pacOptions);
  }
  else
    ulErr = E_SHELL_CMD_WRONG_PARAMETERS;

  return ulErr;
}

/*----------------------------------------------------------------------------*/
static uint32 mSHELL_CmdDf(const t_ShellCmdContext *c_ptContext)
{
  uint32  ulErr = 0;

  if (c_ptContext->ucNumParams == 0)
  {
    char    acDrive[4];
    uint64  uqFree = ffree(m_pacDir);

    fprintf(c_ptContext->pfOut, "%sDrive %s\r\n", DISK_TYPE, mSHELL_GetDriveFromAbsPath(m_pacDir, acDrive));		
    fprintf(c_ptContext->pfOut, " o Free space: %lld bytes\r\n", uqFree);
  }
  else
    ulErr = E_SHELL_CMD_WRONG_PARAMETERS;

  return ulErr;
}

/*----------------------------------------------------------------------------*/
static uint32 mSHELL_CmdLs(const t_ShellCmdContext *c_ptContext)
{
  uint32  ulErr = 0;

  if (c_ptContext->ucNumParams <= 1)
  {
    char          acDrive[4];
    fsStatus      eStatus;
    fsDriveInfo   tDriveInfo;
    fsFileInfo   *ptInfo = HEAP_MALLOC(sizeof(fsFileInfo));
    char         *pacDir = NULL;
    uint32        unSize = 0;
    uint32        unFiles = 0;
    uint32        ulSerial;
    uint64        uqFree;
    char          acLabel[16];

    if (c_ptContext->ucNumParams > 0)
      pacDir = mSHELL_BuildAbsolutePath(m_pacDir, (char *)LIST_GetItem(c_ptContext->ptParams, 0, NULL));
    else
      pacDir = mSHELL_BuildAbsolutePath(m_pacDir, "*");

    fprintf(c_ptContext->pfOut, "%sDrive %s\r\n", DISK_TYPE, mSHELL_GetDriveFromAbsPath(pacDir, acDrive));

    // Get Volume label and serial number
    eStatus = fvol(pacDir, acLabel, &ulSerial);
    if (eStatus == fsOK)
      fprintf(c_ptContext->pfOut, " o Volume Label: '%s', Serial Number: %u\r\n", acLabel, ulSerial);

    // Get Drive Capacity and FS Type
    eStatus = finfo(pacDir, &tDriveInfo);

    if (eStatus == fsOK)
      fprintf(c_ptContext->pfOut, " o Drive Capacity: %lld Bytes (%.2f GB), FS Type: %s\r\n", tDriveInfo.capacity, tDriveInfo.capacity / 1000000000.0, FS_GetFsTypeString(tDriveInfo.fs_type));

    // Get Free Space
    uqFree = ffree(m_pacDir);
    fprintf(c_ptContext->pfOut, " o Free space: %lld Bytes (%.2f GB)\r\n", uqFree, uqFree / 1000000000.0);

    ptInfo->fileID = 0;

    fprintf(c_ptContext->pfOut, "\r\nList files (%s):\r\n", pacDir);
    while (ffind(pacDir, ptInfo) == fsOK)
    {
      char  aucAttr[] = "<....>";
      char  aucSize[12];

      sprintf(aucSize, "%uB", ptInfo->size);

      if (ptInfo->attrib & FS_FAT_ATTR_READ_ONLY)
        aucAttr[1] = 'R';
      if (ptInfo->attrib & FS_FAT_ATTR_HIDDEN)
        aucAttr[2] = 'H';
      if (ptInfo->attrib & FS_FAT_ATTR_SYSTEM)
        aucAttr[3] = 'S';
      if (ptInfo->attrib & FS_FAT_ATTR_ARCHIVE)
        aucAttr[4] = 'A';
      fprintf(c_ptContext->pfOut, "%48s %9s %02d.%02d.%04d %02d:%02d %s\r\n",
                ptInfo->name,
                (ptInfo->attrib & FS_FAT_ATTR_DIRECTORY) ? "<DIR>" : aucSize,
                ptInfo->time.day, ptInfo->time.mon, ptInfo->time.year, ptInfo->time.hr, ptInfo->time.min,
                (ptInfo->attrib & FS_FAT_ATTR_DIRECTORY) ? "" : aucAttr);

      unSize += ptInfo->size;
      unFiles++;
    }

    if (ptInfo->fileID == 0)
      fprintf(c_ptContext->pfOut, "No files found...\r\n");
    else
    {
      fprintf(c_ptContext->pfOut, "\r\n                 Total %d bytes in %d files.\r\n", unSize, unFiles);
    }

    HEAP_FREE(pacDir);
    HEAP_FREE(ptInfo);
  }
  else
    ulErr = E_SHELL_CMD_WRONG_PARAMETERS;

  return ulErr;
}

/*----------------------------------------------------------------------------*/
static uint32 mSHELL_CmdChangeDirectory(const t_ShellCmdContext *c_ptContext)
{
  uint32  ulErr = 0;

  if (c_ptContext->ucNumParams == 1)
  {
    char  *pacDir = mSHELL_BuildAbsolutePath(m_pacDir, (char *)LIST_GetItem(c_ptContext->ptParams, 0, NULL));

    if (FS_FolderExists(pacDir))
      strcpy(m_pacDir, pacDir);
    else
      fprintf(c_ptContext->pfOut, "Directory '%s' not found!\r\n", pacDir);

    HEAP_FREE(pacDir);
  }
  else if (c_ptContext->ucNumParams == 0)
    mSHELL_SetDefaultDrive();
  else
    ulErr = E_SHELL_CMD_WRONG_PARAMETERS;

  return ulErr;
}

/*----------------------------------------------------------------------------*/
static uint32 mSHELL_CmdCreate(const t_ShellCmdContext *c_ptContext)
{
  uint32  ulErr = 0;

  if (c_ptContext->ucNumParams == 2)
  {
    char  *pacFile = mSHELL_BuildAbsolutePath(m_pacDir, (char *)LIST_GetItem(c_ptContext->ptParams, 0, NULL));
    FILE  *pF = fopen(pacFile, "w");
    if (pF)
    {
      uint16   unSize = 0;
      uint16   unWriteSize = 0;
      uint8   *pcItem = LIST_GetItem(c_ptContext->ptParams, 1, &unSize);

      unWriteSize = fwrite(pcItem, 1, unSize, pF);

      if (unWriteSize != unSize)
        ulErr = E_FILE_WRITE_FILE;
      if (fclose(pF))
        ulErr = E_FILE_WRITE_FILE;
    }
    else
      fprintf(c_ptContext->pfOut, "Error: Cannot open file '%s'!\r\n", pacFile);

    HEAP_FREE(pacFile);
  }
  else
    ulErr = E_SHELL_CMD_WRONG_PARAMETERS;

  return ulErr;
}

/*----------------------------------------------------------------------------*/
static uint32 mSHELL_CmdCat(const t_ShellCmdContext *c_ptContext)
{
  uint32  ulErr = 0;
  bool    bHex = false;

  // -h will cause to dumpout as hex
  if ( (c_ptContext->ucNumParams == 2) && (strcmp((char *)LIST_GetItem(c_ptContext->ptParams, 1, NULL), "-h") == 0) )
  {
    bHex = true;
  }
  else if (c_ptContext->ucNumParams != 1)
    ulErr = E_SHELL_CMD_WRONG_PARAMETERS;

  if (!ulErr)
  {
    char    *pacFile = NULL;
    uint8   *paucBuf = HEAP_MALLOC(FS_COPY_FILE_BUF_SIZE);
    FILE    *pFile;
    uint16   unRead;
    uint32   ulSize;

    pacFile = mSHELL_BuildAbsolutePath(m_pacDir, (char *)LIST_GetItem(c_ptContext->ptParams, 0, NULL));
    pFile = fopen(pacFile, "r");

    if (pFile != NULL)
    {
      // Retrieve size of file
      ulSize = FS_fsize(pFile);
      fprintf(c_ptContext->pfOut, "File Size: %d\r\n", ulSize);

      do
      {
        unRead = fread(paucBuf, 1, FS_COPY_FILE_BUF_SIZE, pFile);

        if (!bHex)
          fwrite(paucBuf, 1, unRead, c_ptContext->pfOut);
        else
          DBG_dumpout(c_ptContext->pfOut, paucBuf, unRead);
      }
      while (unRead == FS_COPY_FILE_BUF_SIZE);

      fclose(pFile);
      fprintf(c_ptContext->pfOut, "\r\n");
    }
    else
      fprintf(c_ptContext->pfOut, "Error: Cannot open file '%s'!\r\n", pacFile);

    HEAP_FREE(pacFile);
    HEAP_FREE(paucBuf);
  }

  return ulErr;
}

/*----------------------------------------------------------------------------*/
static uint32 mSHELL_CmdCp(const t_ShellCmdContext *c_ptContext)
{
  uint32  ulErr = 0;

  if (c_ptContext->ucNumParams == 2)
  {
    char  *pacSrc = mSHELL_BuildAbsolutePath(m_pacDir, (char *)LIST_GetItem(c_ptContext->ptParams, 0, NULL));
    char  *pacDest = mSHELL_BuildAbsolutePath(m_pacDir, (char *)LIST_GetItem(c_ptContext->ptParams, 1, NULL));

    ulErr = FS_CopyFile(pacSrc, pacDest);
    // the error code will be displayed in the execution handler!

    HEAP_FREE(pacSrc);
    HEAP_FREE(pacDest);
  }
  else
    ulErr = E_SHELL_CMD_WRONG_PARAMETERS;

  return ulErr;
}

/*----------------------------------------------------------------------------*/
static uint32 mSHELL_CmdRm(const t_ShellCmdContext *c_ptContext)
{
  uint32  ulErr = 0;

  if (c_ptContext->ucNumParams == 1)
  {
    uint32   ulToDel;
    uint32   ulDel;
    char    *pacFile = mSHELL_BuildAbsolutePath(m_pacDir, (char *)LIST_GetItem(c_ptContext->ptParams, 0, NULL));

    FS_DeleteEx(pacFile, NULL, &ulToDel, &ulDel);
    fprintf(c_ptContext->pfOut, "Deleted %d of %d file(s)!\r\n", ulDel, ulToDel);

    HEAP_FREE(pacFile);
  }
  else
    ulErr = E_SHELL_CMD_WRONG_PARAMETERS;

  return ulErr;
}

/*----------------------------------------------------------------------------*/
static uint32 mSHELL_CmdRmdir(const t_ShellCmdContext *c_ptContext)
{
  uint32  ulErr = 0;

  if (c_ptContext->ucNumParams == 1)
  {
    bool   bOK = false;
    char  *pacDir = mSHELL_BuildAbsolutePath(m_pacDir, (char *)LIST_GetItem(c_ptContext->ptParams, 0, NULL));

    fprintf(c_ptContext->pfOut, "Delete Folder '%s'... ", pacDir);
    bOK = FS_DeleteFolder(pacDir);
    fprintf(c_ptContext->pfOut, "%s\r\n", bOK ? "OK." : "FAILED!");

    HEAP_FREE(pacDir);
  }
  else
    ulErr = E_SHELL_CMD_WRONG_PARAMETERS;

  return ulErr;
}

/*----------------------------------------------------------------------------*/
static uint32 mSHELL_CmdMkdir(const t_ShellCmdContext *c_ptContext)
{
  uint32  ulErr = 0;

  if (c_ptContext->ucNumParams == 1)
  {
    bool   bOK = false;
    char  *pacDir = mSHELL_BuildAbsolutePath(m_pacDir, (char *)LIST_GetItem(c_ptContext->ptParams, 0, NULL));

    bOK = FS_CreateFolder(pacDir);
    fprintf(c_ptContext->pfOut, "Create '%s': %s\r\n", pacDir, bOK ? "OK." : "FAILED!");

    HEAP_FREE(pacDir);
  }
  else
    ulErr = E_SHELL_CMD_WRONG_PARAMETERS;

  return ulErr;
}

/*----------------------------------------------------------------------------*/
static uint32 mSHELL_CmdMem(const t_ShellCmdContext *c_ptContext)
{
  uint32  ulErr = 0;

  if (SHELL_CheckCmdParam(c_ptContext, "read", 3))
  {
    // Dump out Memory
    uint32  ulAddr = strtol((char *)LIST_GetItem(c_ptContext->ptParams, 1, NULL), NULL, 0);
    uint32  ulLen = strtol((char *)LIST_GetItem(c_ptContext->ptParams, 2, NULL), NULL, 0);
    DBG_dumpout(c_ptContext->pfOut, (uint8 *)ulAddr, ulLen);
  }
  else if (SHELL_CheckCmdParam(c_ptContext, "write", 3))
  {
    // Write to Memory
    uint32   ulAddr = strtol((char *)LIST_GetItem(c_ptContext->ptParams, 1, NULL), NULL, 0);
    uint32   ulLen;
    uint8   *paucOutput = STR_AsciiHexToBuf((char *)LIST_GetItem(c_ptContext->ptParams, 2, NULL), &ulLen);

    memcpy((void *)ulAddr, paucOutput, ulLen);
    HEAP_FREE(paucOutput);
  }
  else if (SHELL_CheckCmdParam(c_ptContext, "fill", 4))
  {
    // Fill Memory
    uint32  ulAddr = strtol((char *)LIST_GetItem(c_ptContext->ptParams, 1, NULL), NULL, 0);
    uint32  ulLen = strtol((char *)LIST_GetItem(c_ptContext->ptParams, 2, NULL), NULL, 0);
    uint8   ucByte = strtol((char *)LIST_GetItem(c_ptContext->ptParams, 3, NULL), NULL, 0);

    memset((void *)ulAddr, ucByte, ulLen);
  }
  else
    ulErr = E_SHELL_CMD_WRONG_PARAMETERS;

  return ulErr;
}
#endif

/*----------------------------------------------------------------------------*/
static uint32 mSHELL_CmdKill(const t_ShellCmdContext *c_ptContext)
{
  uint32  ulErr = 0;

  if (c_ptContext->ucNumParams == 1)
  {
    pt_OsextThreadId  ptThreadId = (pt_OsextThreadId)strtol((char *)LIST_GetItem(c_ptContext->ptParams, 0, NULL), NULL, 0);
    e_OsextStatus     eStatus = OSEXT_ThreadTerminate(ptThreadId);

    if (eStatus != osOK)
      fprintf(c_ptContext->pfOut, "osxThreadTerminate returned: %d\r\n", eStatus);
    else
      fprintf(c_ptContext->pfOut, "OK.");
  }
  else
    ulErr = E_SHELL_CMD_WRONG_PARAMETERS;

  return ulErr;
}

/*----------------------------------------------------------------------------*/
static uint32 mSHELL_CmdHelp(const t_ShellCmdContext *c_ptContext)
{
  uint32  ulErr = 0;

  if (c_ptContext->ucNumParams <= 1)
  {
    char            *pacCommand = NULL;
    bool             bShowUsage = false;
    t_CommandEntry  *ptEntry = NULL;
    uint16           unI;

    if (c_ptContext->ucNumParams == 1)
    {
      pacCommand = (char *)LIST_GetItem(c_ptContext->ptParams, 0, NULL);

      if (strchr(pacCommand, '*') == NULL && strchr(pacCommand, '?') == NULL)
        bShowUsage = true;
    }

    fprintf(c_ptContext->pfOut, "\r\n");

    // Go through every registered command
    for (unI = 0; unI < LIST_ItemCount(m_ptCmdList); unI++)
    {
      ptEntry = (t_CommandEntry *)LIST_GetItem(m_ptCmdList, unI, NULL);

      // display help summary, if all commands shall be displayed or mask matches
      if (!pacCommand || STR_wildcmp(pacCommand, ptEntry->acName))
      {
        fprintf(c_ptContext->pfOut, mc_acSeparatorLine);
        fprintf(c_ptContext->pfOut, "%s - %s\r\n", ptEntry->acName, ptEntry->c_pacSummary);

        if (bShowUsage)
        {
          // if only help of one specific command asked: Display also usage.
          fprintf(c_ptContext->pfOut, mc_acSeparatorLine);
          fprintf(c_ptContext->pfOut, "Usage: %s\r\n", ptEntry->c_pacUsage);
          fprintf(c_ptContext->pfOut, mc_acSeparatorLine);
          // display help only for one command!
          break;
        }
      }
    }
  }
  else
    ulErr = E_SHELL_CMD_WRONG_PARAMETERS;

  return ulErr;
}

/*----------------------------------------------------------------------------*/
static uint32 mSHELL_CmdReboot(const t_ShellCmdContext *c_ptContext)
{
  uint32  ulErr = 0;

  if (c_ptContext->ucNumParams == 0)
  {
    // Clear Signature => Restart in Boot Loader [see also startup_stm32f437xx.s]
    uint32  *pSignature = (uint32 *)0x20000000;
    *pSignature = 0x00000000;

    NVIC_SystemReset();
  }
  else
    ulErr = E_SHELL_CMD_WRONG_PARAMETERS;

  return ulErr;
}

/*----------------------------------------------------------------------------*/
static void mSHELL_CmdHistorySave(t_List *ptList)
{
#ifdef RTE_FileSystem_Core
  uint8   ucI;
  FILE   *pF = fopen(SHELL_CMD_HISTORY_FILE_NAME, "w");

  if (pF)
  {
    // save the latest commands in file, separated by \r\n
    for (ucI = 0; ucI < ptList->unNumbers; ucI++)
    {
      fwrite(ptList->patEntries[ucI].pucEntry, 1, ptList->patEntries[ucI].unLen, pF);
      fwrite("\r\n", 1, 2, pF);
    }
    fclose(pF);
  }
#else
  (void)ptList;
#endif
}

/*----------------------------------------------------------------------------*/
static t_List *mSHELL_CmdHistoryLoad(void)
{
  t_List  *ptList;

#if SHELL_FILE > 0
  uint8   ucI;
  uint8  *paucData;

  // try to load commands from file
  if (FS_ReadFile(SHELL_CMD_HISTORY_FILE_NAME, &paucData, NULL) == 0)
  {
    const char  *c_apcSep[] = {"\r\n", NULL};
    ptList = STR_split((const char *)paucData, c_apcSep, LINE_BUF_SIZE, SHELL_MAX_CMD_HISTORY);

    // update command length (default LIST including \0 in length)
    for (ucI = 0; ucI < ptList->unNumbers; ucI++)
      ptList->patEntries[ucI].unLen -= 1;

    HEAP_FREE(paucData);
  }
  else
#endif
    ptList = LIST_Create(SHELL_MAX_CMD_HISTORY);

  return ptList;
}


/*============================================================================*/
/*== Task ====================================================================*/
/*============================================================================*/

static void tSHELL_SubThread(void *arg)
{
  char  *pacCmdLine = (char *)arg;

  printf("\r\nThread %s with osThreadId 0x%08X started.\r\n", OSEXT_ThreadGetName(OSEXT_ThreadGetId()), OSEXT_ThreadGetId());

  mSHELL_Exec((char *)pacCmdLine);
  HEAP_FREE(pacCmdLine);

  printf("\r\nThread %s with osThreadId 0x%08X terminated.\r\n", OSEXT_ThreadGetName(OSEXT_ThreadGetId()), OSEXT_ThreadGetId());

  mSHELL_DisplayPrompt();
}

/*----------------------------------------------------------------------------*/
/** @brief      Task of SHELL Module
   
    @param      None
    @return     None

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
static void tSHELL_Handler(void *arg)
{
  // Allocate Buffer to fetch input line
  char    *pacLineBuf = HEAP_MALLOC(LINE_BUF_SIZE);
  uint16   unPtr = 0;
  uint16   unCmdLen = 0;
  uint16   unTemp;

  // list contains the last commands
  t_List  *ptCmdHistory = mSHELL_CmdHistoryLoad();
  uint8    ucCurrentCmd = ptCmdHistory->unNumbers;

  bool     bExecute;

  // Wait for Activation Event Flag [Manual Clear => <No Clear> Option]
  OSEXT_EventFlagsWait(m_ptOsEventActive, SHELL_FLAG_ACTIVE, osFlagsWaitAny | osFlagsNoClear, osWaitForever);

  mSHELL_DisplayProgramInfo();

  // Process Input & Execute Commands
  do
  {
    mSHELL_DisplayPrompt();

    // reset data
    memset(pacLineBuf, 0, LINE_BUF_SIZE);
    unPtr = 0;
    unCmdLen = 0;
    bExecute = false;

    while (!bExecute)
    {
      // int nCh = fgetc(stdin);
      // fgetc seems to generate a deadlock, if other file functions are active...
      // -> workaround for the moment: call io_std_read directly.
      int  nCh = stdin_getchar();

      if (nCh != EOF)
      {
        if (nCh == 13)
          bExecute = true;
        else if (nCh == 8)
        {
          // if some letters are left to delete
          if (unPtr > 0)
          {
            // update pointers
            unPtr--;
            unCmdLen--;

            // shift character in buffer
            for (unTemp = unPtr; unTemp <= (unCmdLen + 1); unTemp++)
              pacLineBuf[unTemp] = pacLineBuf[unTemp + 1];

            // delete last letter, save cursor position, delete end of line
            fprintf(stdout, "\033[D\033[s\033[K"); 

            // print character right from cursor
            for (unTemp = unPtr; unTemp <= unCmdLen; unTemp++) 
              fputc(pacLineBuf[unTemp], stdout);

            // restore cursor position
            if (unPtr != unCmdLen)
              fprintf(stdout, "\033[u"); 
          }
        }
        else if (nCh == '\033')
        {
          nCh = stdin_getchar();   // skip '['
          nCh = stdin_getchar();

          if (nCh == '4')   // <Delete>
          {
            nCh = stdin_getchar();   // skip '~'

            if (unCmdLen > unPtr)
            {
              // update pointers
              unCmdLen--;

              // shift character in buffer
              for (unTemp = unPtr; unTemp <= (unCmdLen + 1); unTemp++)
                pacLineBuf[unTemp] = pacLineBuf[unTemp + 1];

              // save cursor position, delete end of line
              fprintf(stdout, "\033[s\033[K");

              // print character right from cursor
              for (unTemp = unPtr ; unTemp <= unCmdLen; unTemp++) 
                fputc(pacLineBuf[unTemp], stdout);

              // restore cursor position
              if (unPtr != unCmdLen)
                fprintf(stdout, "\033[u");
            }
          }
          else if ((nCh == 'A') || (nCh == 'B'))   // <Up> or <Down>
          {
            bool  bProcess = false;

            if ((nCh == 'A') && (ucCurrentCmd > 0))
            {
              bProcess = true;
              ucCurrentCmd--;
            }

            if ((nCh == 'B') && (ucCurrentCmd < (LIST_ItemCount(ptCmdHistory) - 1)))
            {
              bProcess = true;
              ucCurrentCmd++;
            }

            if (bProcess)
            {
              char  *pacTemp;

              // shift cursor to begin of line
              while (unPtr--)
                fprintf(stdout, "\033[D"); 

              // delete end of line
              fprintf(stdout, "\033[K");

              // get requested command from history
              pacTemp = (char *)LIST_GetItem(ptCmdHistory, ucCurrentCmd, &unCmdLen);

              // copy history command to line buffer and print it to console
              memset(pacLineBuf, 0, LINE_BUF_SIZE);
              memcpy(pacLineBuf, pacTemp, unCmdLen);
              unPtr = unCmdLen;
              fprintf(stdout, "%s", pacLineBuf); 
            }
          }
          else if (nCh == 'C')   // <right>
          {
            // if end of command isn't reached yet
            if (unPtr < unCmdLen)
            {
              unPtr++;
              printf("\033[C");
            }
          }
          else if (nCh == 'D')   // <left>
          {
            // if first letter isn't reached
            if (unPtr > 0)
            {
              unPtr--;
              printf("\033[D");
            }
          }
        }
        else if (nCh <= 31)
          ; // don't do anything
        else if (unCmdLen < (LINE_BUF_SIZE - 2))
        {    
          // The line buffer is full. Don't save char, don't echo it. Leave space for a possible CR LF
          unCmdLen++;
          unPtr++;
          if (unPtr != unCmdLen)
          {
            for (unTemp = unCmdLen; unTemp >= unPtr; unTemp--)
              pacLineBuf[unTemp] = pacLineBuf[unTemp - 1];

            fprintf(stdout, "\033[K\033[s");   // delete end of line; save cursor position
          }
 
          // add letter to buffer
          pacLineBuf[unPtr - 1] = (char)nCh;

          for (unTemp = unPtr - (uint16)1; unTemp < unCmdLen; unTemp++)
            fputc(pacLineBuf[unTemp], stdout);

          if (unPtr != unCmdLen)
            fprintf(stdout, "\033[u\033[C");   // restore cursor position and shift one letter right
        }
      }
    }

    if (bExecute && (OSEXT_EventFlagsGet(m_ptOsEventActive) & SHELL_FLAG_ACTIVE))
    {
      // Command complete & Shell still active: Execute Command
      ucCurrentCmd = (uint8)LIST_ItemCount(ptCmdHistory);

      if ((ucCurrentCmd == 0) || 
          (strncmp((char *)LIST_GetItem(ptCmdHistory, (uint16)(ucCurrentCmd - 1), &unTemp), pacLineBuf, unCmdLen) != 0) ||
          (unTemp != unCmdLen))
      {
        if (ucCurrentCmd >= SHELL_MAX_CMD_HISTORY)
          LIST_DeleteItem(ptCmdHistory, 0);

        ucCurrentCmd = LIST_AddItem(ptCmdHistory, (const uint8 *)pacLineBuf, strlen(pacLineBuf));
        mSHELL_CmdHistorySave(ptCmdHistory);
      }

      mSHELL_Exec(pacLineBuf);
    }
  } while (OSEXT_EventFlagsWait(m_ptOsEventActive, SHELL_FLAG_ACTIVE, osFlagsWaitAny | osFlagsNoClear, osWaitForever) == SHELL_FLAG_ACTIVE);
}


/*============================================================================*/
/*== Implementation of Public Functions ======================================*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/
uint32 SHELL_AddCommand(const char *c_pacCmdName, pfCmdHandler pfHandler, const char *c_pacSummary, const char *c_pacUsage)
{
  t_CommandEntry  tEntry;

  // SHELL_init must be called before Commands can be added!
  if (m_ptCmdList)
  {
    OSEXT_MutexAcquire(m_ptOsMutIdCmdList, osWaitForever);

    ASSERT(c_pacCmdName);
    ASSERT(pfHandler);

    memset(&tEntry, 0, sizeof(tEntry));
    strncpy(tEntry.acName, c_pacCmdName, sizeof(tEntry.acName) - 1);
    tEntry.pfHandler = pfHandler;
    tEntry.c_pacSummary = c_pacSummary;
    tEntry.c_pacUsage = c_pacUsage;

    // Add the command to the List - but only, if not already added
    // LIST_SearchStrItem does only work, because the "search key" is the first 0-terminated string in the entry struct!
    if (LIST_SearchStrItem(m_ptCmdList, c_pacCmdName, 0, NULL) == NULL)
    {
      if (LIST_AddItem(m_ptCmdList, (uint8*)&tEntry, sizeof(tEntry)) == NULL)
        ASSERT_FAIL();   // increase MAX_COMMANDS!!

      // make always sure the list is held in alphabetical order (for help display)
      mSHELL_SortCommandList();
    }

    OSEXT_MutexRelease(m_ptOsMutIdCmdList);
  }

  return 0;
}

/*----------------------------------------------------------------------------*/
bool SHELL_CheckCmdParamEx(const t_ShellCmdContext *c_ptContext, const char *c_pacParamStr, int lNumArgs, ...)
{
  int      nI;
  va_list  vl;
  int      lParam;
  bool     bMatch = false;

  if ( c_ptContext->ucNumParams && (strcmp((char *)LIST_GetItem(c_ptContext->ptParams, 0, NULL), c_pacParamStr) == 0) )
  {
    va_start(vl, lNumArgs);

    for (nI = 0; nI < lNumArgs; nI++)
    {
      lParam = va_arg(vl, int);
      if (c_ptContext->ucNumParams == lParam)
        bMatch = true;
    }
    va_end(vl);
  }

  return bMatch;
}

/*----------------------------------------------------------------------------*/
bool SHELL_CheckCmdParam(const t_ShellCmdContext *c_ptContext, const char *c_pacParamStr, uint8 ucNumParams)
{
  return SHELL_CheckCmdParamEx(c_ptContext, c_pacParamStr, 1, ucNumParams);
}

/*----------------------------------------------------------------------------*/
uint32 SHELL_ExecuteCommand(const char *c_pacCmdLine)
{
  return mSHELL_Exec(c_pacCmdLine);
}

/*----------------------------------------------------------------------------*/
void SHELL_Init(bool bActivate)
{
  // Mutex to protect Command List
  m_ptOsMutIdCmdList = OSEXT_ShellMutexCmdNew();

  // Create Task & Activate / Deactivate Event
  OSEXT_ShellThreadNew(tSHELL_Handler, NULL);
  m_ptOsEventActive = OSEXT_ShellEventNew();

  // Activate Shell immediately [Set Flag according to Parameter]
  if (bActivate)
    OSEXT_EventFlagsSet(m_ptOsEventActive, SHELL_FLAG_ACTIVE);

  // Allocate Buffer for current Directory
  m_pacDir = HEAP_MALLOC(MAX_FILENAME_LEN);

  // Create List to register commands
  m_ptCmdList = LIST_Create(MAX_COMMANDS);

  // The default drive is M: for every project (at the moment)
  mSHELL_SetDefaultDrive();

  // Register SHELL's common command set
  SHELL_AddCommand("help", mSHELL_CmdHelp, mc_acSummaryHelp, mc_acUsageHelp);

#if SHELL_FILE > 0
  SHELL_AddCommand("ls", mSHELL_CmdLs, mc_acSummaryLs, mc_acUsageLs);
  SHELL_AddCommand("cd", mSHELL_CmdChangeDirectory, mc_acSummaryCd, mc_acUsageCd);
  SHELL_AddCommand("create", mSHELL_CmdCreate, mc_acSummaryCreate, mc_acUsageCreate);
  SHELL_AddCommand("cat", mSHELL_CmdCat, mc_acSummaryCat, mc_acUsageCat);
  SHELL_AddCommand("cp", mSHELL_CmdCp, mc_acSummaryCp, mc_acUsageCp);
  SHELL_AddCommand("rm", mSHELL_CmdRm, mc_acSummaryRm, mc_acUsageRm);
  SHELL_AddCommand("rmdir", mSHELL_CmdRmdir, mc_acSummaryRmdir, mc_acUsageRmdir);
  SHELL_AddCommand("mkdir", mSHELL_CmdMkdir, mc_acSummaryMkdir, mc_acUsageMkdir);
  SHELL_AddCommand("format", mSHELL_CmdFormat, mc_acSummaryFormat, mc_acUsageFormat);
  SHELL_AddCommand("df", mSHELL_CmdDf, mc_acSummaryDf, mc_acUsageDf);
  SHELL_AddCommand("mem", mSHELL_CmdMem, mc_acSummaryMem, mc_acUsageMem);
#endif

  SHELL_AddCommand("kill", mSHELL_CmdKill, mc_acSummaryKill, mc_acUsageKill);
  SHELL_AddCommand("reboot", mSHELL_CmdReboot, mc_acSummaryReboot, mc_acUsageReboot);
}

/*----------------------------------------------------------------------------*/
void SHELL_Activate(void)
{
  // Activate Shell by setting Event Flag
  OSEXT_EventFlagsSet(m_ptOsEventActive, SHELL_FLAG_ACTIVE);
}

/*----------------------------------------------------------------------------*/
void SHELL_Deactivate(void)
{
  // Deactivate Shell by clearing Event Flag
  OSEXT_EventFlagsClear(m_ptOsEventActive, SHELL_FLAG_ACTIVE);
}

/*----------------------------------------------------------------------------*/
#endif   // SHELL == 1

/// @}   // End of Group

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
