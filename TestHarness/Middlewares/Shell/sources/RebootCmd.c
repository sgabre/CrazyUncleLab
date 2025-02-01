#include <stdbool.h>
#include <stdint.h>

#include "Shell.h"
#include "RebootCmd.h"


static const char mRebootUsage[] = "reboot\r\n";

static uint32_t CmdReboot_Handler(const ShellCommandContext_ts *aContext)
{
  uint32_t  Error = 0;
  return Error;
}

void RebootCmd_Register(void)
{
	SHELL_ADD_CMD("reboot", CmdReboot_Handler, mRebootUsage);
}
