#ifndef _SHELL_API_H__
#define _SHELL_API_H__

//#include "Shell_Config.h"
//#include "Shell.h"


typedef struct
{
  void (*Handler) (void);
} iShell_ts;

iShell_ts* iShell(void);

void iShell_Setup(void);


#endif //#ifndef _SHELL_API_H__
