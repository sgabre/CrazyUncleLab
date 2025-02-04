#ifndef _SHELL_CMD_H__
#define _SHELL_CMD_H__

#include <stdio.h>

#include "Shell_Config.h"

typedef uint32_t (*ShellCallBack)(int argc, char **argv);

typedef struct Command_ts
{
  const char*    Name;
  const char*    Help;
  ShellCallBack  CallBack;
} Command_ts;


void SHELL_ADD_CMD(const char* aName, const char* aHelp, ShellCallBack aCommand);
//bool Shell_CheckCmdParam(const ShellCommandContext_ts* aContext, const char* aParameterString, uint8_t NumParams);
//bool Shell_CheckCmdParamEx(const ShellCommandContext_ts* aContext, const char* ParameterString, int lNumArgs, ...);


#endif //#ifndef _SHELL_CMD_H__
