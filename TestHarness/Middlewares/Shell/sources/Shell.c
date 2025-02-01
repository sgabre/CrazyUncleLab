#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Shell.h"

// Key codes
#define END_OF_LINE '\0'
#define SPACE ' '
#define TAB '\t'
#define NEW_LINE '\n'
#define CARRIAGE_RETURN '\r'
#define BACK_SPACE '\b'
#define DELETE '\177'
#define ESCAPE '\33'
#define SQUARE_BRACKET_OPEN '\133'
#define UP_ARROW '\101'

typedef int (*CommandCallBack)(int argc, char **argv);

typedef struct ShellCommand_ts
{
	const char*				Name;
	const char* 			Help;
	CommandCallBack 		CallBack;
} ShellCommand_ts;


typedef struct ShellLine_ts
{
	char    	Buffer[SHELL_MAX_BUFFER_LENGHT];
	uint16_t   	Position;
	uint16_t   	Size;
}ShellLine_ts;

typedef struct ShellContext_ts
{
    bool 	IsSetup;
    bool 	IsEnable;
    bool    IsExecute;
    char* 	WorkDirectory;
    uint8_t CommandIndex;
    ShellCommand_ts * Commands;
    int ExecutionStatus;
}ShellContext_ts;

static ShellContext_ts mShellContext;

static  char  __WORK_DIRECTRY_BUFFER__[SHELL_MAX_COMMANDS_NAME_SIZE];

static  ShellCommand_ts __CMD_TABLE_START__[SHELL_MAX_COMMANDS];


static void mShell_Execute(int argc, char **argv);


static void mShell_SetDefaultDrive(void)
{
  strcpy(mShellContext.WorkDirectory, DEFAULT_DISK);
}

static void mShell_Prompt(void)
{
	printf("\r\n%s>>", mShellContext.WorkDirectory);
}

uint8_t mShell_IsEnable(void)
{
	return mShellContext.IsEnable;
}

static int mShell_ParseLine(char **argv, char *line_buff, int argument_size)
{
	 int argc = 0;
	  int pos = 0;
	  int length = strlen(line_buff);

	  while (pos <= length)
	  {

		  if (line_buff[pos] != '\t' && line_buff[pos] != SPACE && line_buff[pos] != END_OF_LINE)
		  {
			  argv[argc++] = &line_buff[pos];
		  }

	    for (; line_buff[pos] != '\t' && line_buff[pos] != SPACE && line_buff[pos] != END_OF_LINE; pos++)
	    {
	      ;
	    }

	    if (line_buff[pos] == '\t' || line_buff[pos] == SPACE)
	    {
	      line_buff[pos] = END_OF_LINE;
	    }

	    pos++;

	  }

	  return argc;
}


int mShell_Exec(char* aLine)
{
  int argc;

  // TODO: this takes too much stack space. Optimize!
  char* argv[SHELL_MAX_NUM_PARAMS];

  // parse the line_buff
  argc =  mShell_ParseLine(argv, aLine, SHELL_MAX_NUM_PARAMS);

  // execute the parsed commands
  if (argc > 0)
  {
	  mShell_Execute(argc, argv);
  }

  return mShellContext.ExecutionStatus;
}

static void mShell_Execute(int argc, char **argv)
{
  int match_found = false;

  for (int i = 0; mShellContext.Commands[i].Name != NULL; i++)
  {
    if (strcmp(argv[0], mShellContext.Commands[i].Name) == 0)
    {
      mShellContext.ExecutionStatus = mShellContext.Commands[i].CallBack(argc, &argv[0]);
      match_found = true;
      break;
    }
  }

  if (match_found == false)
  {
    printf("\"%s\": command not found. Use \"help\" to list all command.\n", argv[0]);
    mShellContext.ExecutionStatus = -1;
  }
}

void mShell_Handler(void)
{
	int argc = 0;
	char* argv[SHELL_MAX_NUM_PARAMS];
	printf("Shell_Handler\n");

	ShellLine_ts Line = {0};
	do
	{
		mShell_Prompt();

		memset(&Line, 0, sizeof(ShellLine_ts) );

		while (mShellContext.IsExecute == false)
		{
			int  InByte = getchar();

			if (InByte != EOF)
			{
				mShellContext.IsExecute = true;
			}
		}

		// parse the line_buff
		argc = mShell_ParseLine(argv, Line.Buffer, SHELL_MAX_NUM_PARAMS);

		  // execute the parsed commands
		  if (argc > 0)
		  {
			  mShell_Execute(argc, argv);
		  }

	} while (mShell_IsEnable() == SHELL_FLAG_ACTIVE);
}


void SHELL_ADD_CMD(const char *name, const char* help, CommandCallBack aCommand)
{

    if(mShellContext.CommandIndex < SHELL_MAX_COMMANDS)
    {
    	mShellContext.Commands[mShellContext.CommandIndex].Name = name;
        mShellContext.Commands[mShellContext.CommandIndex].Help = help;
        mShellContext.Commands[mShellContext.CommandIndex].CallBack = aCommand;
        mShellContext.CommandIndex++;
    }
}

int Help_Handler(int argc, char **argv)
{
  int i = 0;
  /* Default to Verbose */
  bool verbose = true;

  if (argc > 1 && (strcmp(argv[1], "-l")==0))
  {
    verbose = false;
  }
  else
  {
    printf("use: help -l for list only.\n\n");
  }

  while (mShellContext.Commands[i].Name != NULL)
  {
    printf(mShellContext.Commands[i].Name);

    if (verbose)
    {
      printf("\n\t");
      printf(mShellContext.Commands[i].Help);
    }

    printf("\n");
    i++;
  }

  return 0;
}

void Shell_Setup(void)
{

	if (mShellContext.IsSetup == false)
	{
		mShellContext.WorkDirectory = &__WORK_DIRECTRY_BUFFER__[0];
		mShellContext.Commands = (ShellCommand_ts*)&__CMD_TABLE_START__;

		mShell_SetDefaultDrive();

		SHELL_ADD_CMD("help", "Prints all available commands", Help_Handler);

		mShellContext.IsSetup = true;
	}
	iShell()->Handler = mShell_Handler;

}
