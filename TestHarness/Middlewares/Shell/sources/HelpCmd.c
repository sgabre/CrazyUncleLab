#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "Shell.h"
#include "ShellCmd.h"
#include "RebootCmd.h"

#include "iString.h"

static const char mc_acSeparatorLine[] = "--------------------------------------------------------------------------------\r\n";

static const char mHelpUsage[] =
		  "help [command]\r\n"
		  "  command (optional): If specified, displays the help summary and usage of this command\r\n"
		  "                      If not specified, displays a list of all commands and their help\r\n"
		  "                      summary.\r\n"
		  "                      command may also contain wildcards (* or ?) - in this case, all\r\n"
		  "                      help summaries of matching commands are displayed.\r\n";


static List_ts* mCommands;


static uint32_t HelpCmd_Hander(const ShellCommandContext_ts *aContext)
{
  uint32_t  Error = 0;

  if (aContext->NumParams <= 1)
  {
    char*				pacCommand = NULL;
    bool             	bShowUsage = false;
    CommandEntry_ts *	ptEntry = NULL;
    uint16_t           	Index;

    if (aContext->NumParams == 1)
    {
      pacCommand = (char *)iList_Get(aContext->Parameters, 0, NULL);

      if (strchr(pacCommand, '*') == NULL && strchr(pacCommand, '?') == NULL)
        {
          bShowUsage = true;
        }
    }

    fprintf(aContext->Out, "\r\n");

    // Go through every registered command
    for (Index = 0; Index < iList_Count(mCommands); Index++)
    {
      ptEntry = (CommandEntry_ts *)iList_Get(mCommands, Index, NULL);

      // display help summary, if all commands shall be displayed or mask matches
      if (!pacCommand || iString_wildcmp(pacCommand, ptEntry->Name))
      {
        fprintf(aContext->Out, mc_acSeparatorLine);
       fprintf(aContext->Out, "%s\r\n", ptEntry->Name);

        if (bShowUsage)
        {
          // if only help of one specific command asked: Display also usage.
          fprintf(aContext->Out, mc_acSeparatorLine);
          fprintf(aContext->Out, "Usage: %s\r\n", ptEntry->Usage);
          fprintf(aContext->Out, mc_acSeparatorLine);
          // display help only for one command!
          break;
        }

      }
    }
  }
  else
  {
    Error = E_SHELL_CMD_WRONG_PARAMETERS;
  }

  return Error;
}

void HelpCmd_Register(List_ts* aCommandList)
{
	if(aCommandList != NULL)
	{
		mCommands = aCommandList;
		SHELL_ADD_CMD("help", HelpCmd_Hander, mHelpUsage);
	}
}
