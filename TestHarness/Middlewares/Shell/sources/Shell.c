#ifdef __cplusplus
#error This is not C++. Please use a C compiler.
#endif

#include <string.h>

#include "Shell.h"
#include "Array.h"

//FIFO, LIFO, ARRAY, QUEUE, LIST, DLIST,
// une Zone Memory en RAM ou ROM
// Iteam = struct , IteamSize =  sizeof(Iteam_ts)

//ARRAY
//Create create, link and initialize l'array 
//Delate libere les element  l'array 
//Set copy un iteam a l'index i 
//Get retourne  une copy d'iteam de l'index i 
//Count return nombre d'item l'array 

//FIFO
//Create create, link and initialize l'FIFO 
//Delate libere les element  l'FIFO 
//Push copy un iteam  l index d'ecriture 
//Pop retourne  une copy d'iteam de l'lecture 
//Count return nombre d'item dans la FIFO

//LIFO
//Create create, link and initialize l'LIFO 
//Delate libere les element  l'LIFO 
//Push copy un iteam  l index d'ecriture 
//Pop retourne  une copy d'iteam de l'lecture 
//Count return nombre d'item dans la LIFO

//LIST
//Create create, link and initialize l'LIFO 
//Delate libere les element  l'LIFO 
//Push copy un iteam  l index d'ecriture 
//Pop retourne  une copy d'iteam de l'lecture 
//Count return nombre d'item dans la LIFO


#define KEY_BACKSPACE 	 8
#define KEY_ESC     	 27
#define KEY_CR			 13	//(carriage return) Key
#define KEY_TAB		 '\t'
#define KEY_SPACE		 ' '

static  Command_ts __CMD_BUFFER__[SHELL_COMMAND_SIZE];

//Command_ts *volatile Commands = (Command_ts *)&__CMD_BUFFER__;

typedef struct Shell_ts
{
   bool   IsSetup;
   char   WorkDir[8];
   Array_ts* Commands;
} Shell_ts;

static Shell_ts mShell;


void mShell_Prompt(void)
{
   printf("\r\n%s>>", mShell.WorkDir);
}

void mShell_SetDefaultDrive(void)
{
   strcpy(mShell.WorkDir, SHELL_DEFAULT_DISK);
}



int mShell_ParseLine(char **argv, char* aStream) 
{
   
   int argc = 0;
   int pos = 0;

   // Tokenize the input stream using space as a delimiter
   char *token = strtok(aStream, " ");
   
   while ( (token != NULL) && (argc < (SHELL_MAX_ARGS - 1)  ) )
   {
      argv[argc++] = token;   // Store token
      token = strtok(NULL, " ");
   }
   
   argv[argc] = NULL;
   return argc;
}

static int mShell_Search(void const *aData, void* aUser)
{
   int Return = 0;
   Command_ts const* oCommand = aData;
   
   if (oCommand != NULL)
   {
      char* key = (char*) aUser;
      
      if (key != NULL)
      {
         if (strcmp(oCommand->Name, key) == 0)
         {
            Return = 0;
         }         
      }
      else
      {
         Return = -1;
      }
   }
   else
   {
      Return = -1;
   }
   
   return Return;
}

int mShell_strcmp(const void *a, const void *b) 
{
   return strcmp(*(const char **)a, *(const char **)b);
}

static void mShell_Execute(int argc, char **argv) 
{
   

   

      //const char *words[] = { "apple", "banana", "cherry", "date", "grape", "mango", "orange" };
      //size_t n = sizeof(words) / sizeof(words[0]);
      
      //const char *key = "cherry";
      //char **result = (char **)bsearch(argv[0], mShell.Commands, Array_Count(mShell.Commands), sizeof(char*), mShell_strcmp);

   //mShell.Commands
   // Check, if the last "parameter" starts with a "&" - this would cause to start the function in an own thread.
   // Perform the check before "parameter grouping" to be able to easily escape a real & parameter by "&".
   
   // Check, if the last but one entry is a ">" or ">>" - in this case, we have to redirect the output to a file instead of stdout
   
      /*
   int match_found = false;
   
   for (int i = 0; table[i].Name != NULL; i++) 
   {
      if (strcmp(argv[0], table[i].Name) == 0) 
      {
         __cmd_exec_status = table[i].CallBack(argc, &argv[0]);
         match_found = TRUE;
         break;
      }
   }
   
   if (match_found == FALSE) 
   {
      printf("\"%s\": command not found. Use \"help\" to list all command.\n", argv[0]);
      __cmd_exec_status = -1;
   }
   */
}


int mShell_SortUp(void const* va, void const* vb)
{
   Command_ts const *pa = (Command_ts*)va;
   Command_ts const *pb = (Command_ts*)vb;
   
   return strcmp(pa->Name, pb->Name);
}

void mShell_SortCommandList(void)
{
   Array_Sort(mShell.Commands, mShell_SortUp);
}

void SHELL_ADD_CMD(const char* aName, const char* aHelp, ShellCallBack aCommand) 
{
   if(aName == NULL)
   {
      FAILURE(SHELL_ERR_INVALID_PARAM);
   }
   else if(aHelp == NULL)
   {
      FAILURE(SHELL_ERR_INVALID_PARAM);
   }
   else if(aCommand == NULL)
   {
      FAILURE(SHELL_ERR_INVALID_PARAM);
   }
   else 
   {
      //printf("ADD(.Name = %s\n", aName);
      if( Array_Count(mShell.Commands) < SHELL_COMMAND_SIZE)
      {
         Command_ts  oCommand = {.Name = aName, .Help = aHelp, .CallBack = aCommand};
         Array_Add(mShell.Commands, &oCommand);
      }
      else 
      {
         FAILURE(SHELL_ERR_MEMORY);
      }
      
   }
}

void Shell_Handler(void)
{
   mShell_Prompt();
}

bool Shell_IsEnable(void)
{
   return true;
}

void Shell_Enable(void)
{
}

void Shell_Disable(void)
{
}

void Shell_Setup(void)
{
   if(mShell.IsSetup == false)
   {
      mShell.Commands = Array_Create( sizeof(Command_ts)) ;
      mShell_SetDefaultDrive();
      
      mShell.IsSetup = true;
   }
}
   