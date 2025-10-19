#ifndef _SHELL_H_
#define _SHELL_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>

#include "Shell_Config.h"
#include "ShellCmd.h"

#include "Error.h"

   typedef enum
   {
      SHELL_OK,
      #define ITEM(n_, s_)\
      SHELL_ERR_##n_,
      #include "ShellError.h"
      #undef ITEM
      SHELL_ERR_SIZE
   }eShellError;


void mShell_Prompt(void);
void mShell_SetDefaultDrive(void);
int mShell_ParseLine(char **argv, char *line_buff) ;
int mShell_SortUp(void const* va, void const* vb);
void mShell_SortCommandList(void);
   
void Shell_Setup(void);
void Shell_Handler(void);


#ifdef __cplusplus
   }
#endif
   
#endif /* _SHELL_H_ */
   