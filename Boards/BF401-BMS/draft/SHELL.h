#ifndef __SHELL_H__
#define __SHELL_H__

/** @defgroup   SHELL  SHELL-Group
                Command Line Interpreter.
The SHELL supports a lot of built in functions, which are active, even if for no
other module the SHELL is enabled. However, the command 'help' outputs the help
summary of all available commands. If you want the get help about the usage for
a specific command, you may enter 'help <command>'. e.g.
  \code /micSHell/>M:>>help help \endcode
will show you even more, e.g. that you can use wildcards to search for a specific
command... try it!

<b>Syntax of command line:\n</b>
The command line consists of four parts:
 - command
 - parameters
 - redirection
 - 'fork': argument to let command start as own thread (is known
           as 'fork' in unix workl).
 
<b>Examples:\n</b>
Simple command. Output goes to stdout (= UART7 usually)
  \code /micSHell/>M:>>ls \endcode

Command with parameters. Parameters are delimited by space; however, if
inside a parameter a space is needed, all has to be grouped by quotes.
  \code /micSHell/>M:>>create myfile.txt "this is the content of file" \endcode

Command with redirection:
  \code /micSHell/>M:>>ls > myfiles.txt \endcode
Here, the command will not write anything to stdout; all output is redirected to
file myfiles.txt. The file is rewritten, if it already exists. You may let the
redirection append content to an already existing file:
  \code /micSHell/>M:>>ls >> myfiles.txt \endcode
If you may get rid of an output but not write a file, you may use the virtual file
/dev/null:
  \code /micSHell/>M:>>cat file.txt > /dev/null \endcode
This will open and read the content of file.txt but "through output away".

Fork: A command can be executed in an own thread with operator & as last argument:
  \code /micSHell/>M:>>cp U:\largefile.bin M:\largefile.bin & \endcode
Here, the SHELL is ready again immediately since the copy is running in an own
thread. If needed, the priority can be specified with &(prio) - e.g.
  \code  /micSHell/>M:>>cp U:\largefile.bin M:\largefile.bin &(100) \endcode
will perform the copy with thread priority 100 rather than 9.

<b>Changing directories/drives:\n</b>
The current directory or drive can be changed by using cd:
  \code  /micSHell/>M:>>cd U: \endcode
will change the active directory to Memory-Stick, if attached.\n
  \code  /micSHell/>M:\config\periph>>cd \endcode
will go back to M:\.
You may also use
  \code  /micSHell/>M:\config\periph>>cd .. \endcode
to go one level back. You may also enter combinations:
  \code  /micSHell/>M:\config\periph>>cd ..\vmc \endcode
to end up here:
  \code  /micSHell/>M:\config\vmc>> \endcode
  
    @{
********************************************************************************
*   @name       SHELL
*   @file       SHELL.h
*
*   @brief      Command Line Interpreter.
*   
*   @authors    A. Bader
*   @date       21.05.2014
*
*   @copyright (C) 2014 by: MICROTRONIC AG
*                           Duennernstrasse 32
*                           CH-4702 Oensingen
*******************************************************************************/

/*== Includes ================================================================*/
#include "LIST.h"


/*== Defines =================================================================*/

//< Returned by SHELL_AddCommand, if SHELL has not been initialized yet.
#define E_SHELL_NOT_INITIALIZED         1

//< Error code, which a command handler function can return, which will force to
//< display the function's usage text.
#define E_SHELL_CMD_WRONG_PARAMETERS    0xFF000001

//< Error code of SHELL Exec, if command not found
#define E_SHELL_CMD_NOT_FOUND           0xFF000002


/*== Structures ==============================================================*/

/// struct passed to each handler function.
typedef struct
{
  char    *pacCommand;            //< command
  char    *pacCmdLine;            //< whole command line
  char    *pacCurrentDir;         //< current directory
  FILE    *pfOut;                 //< stream to output debug output (fprintf)
  FILE    *pfIn;                  //< stream to read input from
  t_List  *ptParams;              //< t_List with parameters passed to command
  uint8    ucNumParams;           //< number of parameters in t_List
} t_ShellCmdContext;


/*== Function Types ==========================================================*/
typedef uint32 (*pfCmdHandler)(const t_ShellCmdContext *c_ptContext);


/*== Module Interfaces =======================================================*/

/*----------------------------------------------------------------------------*/
/** @brief      Add a SHELL command.
   
    @param[in]  c_pacCmdName  Name of SHELL command
    @param[in]  pfHandler     Pointer to handler function
    @param[in]  c_pacSummary  Pointer to help summary text. PLEASE ENTER FOR
                              EACH FUNCTION A GOOD SUMMARY!
    @param[in]  c_pacUsage    Pointer to help usage text. PLEASE ENTER FOR
                              EACH FUNCTION A COMPLATE USAGE INFORMATION!
    @return     0 = Successful
                E_SHELL_NOT_INITIALIZED in case SHELL is not initialized yet.

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
uint32 SHELL_AddCommand(const char *c_pacCmdName, pfCmdHandler pfHandler, const char *c_pacSummary, const char *c_pacUsage);

/*----------------------------------------------------------------------------*/
/** @brief      Execute a SHELL Command manually by programming.
   
    @param[in]  c_pacCmdLine    Command Line
    @return     0 = Successfull / E_SHELL_xx in case of error
                or functions return value

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
uint32 SHELL_ExecuteCommand(const char *c_pacCmdLine);

/*----------------------------------------------------------------------------*/
/** @brief      Check, if the number of parameters (variable list with values
                to check) and the
                "sub command" (first parameter) match.
                This function is typically used inside command handler functions
                to check for subcommands / arguments / number of params.
   
    @param[in]  c_ptContext   command context (with parameters)
    @param[in]  c_pacParamStr compare this string with parameter at index ucParIdx
    @param[in]  lNumArgs      number of values following...
    @param[in]  ...           this number of params allowed
    @return     true = c_pacParamStr and one number matches the effective number
                       of parameters / false otherwise

    @author     A.Bader                                     @date 11.07.2014  */
/*----------------------------------------------------------------------------*/
bool SHELL_CheckCmdParamEx(const t_ShellCmdContext *c_ptContext, const char *c_pacParamStr, int lNumArgs, ...);

/*----------------------------------------------------------------------------*/
/** @brief      Check, if the number of parameters and the
                "sub command" (first parameter) match.
                This function is typically used inside command handler functions
                to check for subcommands / arguments / number of params.
   
    @param[in]  c_ptContext   command context (with parameters)
    @param[in]  c_pacParamStr compare this string with first parameter
    @param[in]  ucNumParams   number of parameters needed
    @return     true = all matches / false otherwise

    @author     A.Bader                                     @date 11.07.2014  */
/*----------------------------------------------------------------------------*/
bool SHELL_CheckCmdParam(const t_ShellCmdContext *c_ptContext, const char *c_pacParamStr, uint8 ucNumParams);

/*----------------------------------------------------------------------------*/
/** @brief      Initialize SHELL Module

    @param[in]  bActivate     true => Activate Shell immediately

    @return     None

    @author     A.Bader                                     @date 21.05.2014  */
/*----------------------------------------------------------------------------*/
void SHELL_Init(bool bActivate);

/*----------------------------------------------------------------------------*/
/** @brief      Activate [Enable] SHELL

    @params     None

    @return     None

    @author     R. Ammann                                   @date 09.04.2020  */
/*----------------------------------------------------------------------------*/
void SHELL_Activate(void);

/*----------------------------------------------------------------------------*/
/** @brief      Deactivate [Disable] SHELL

    @params     None

    @return     None

    @author     R. Ammann                                   @date 09.04.2020  */
/*----------------------------------------------------------------------------*/
void SHELL_Deactivate(void);

/// @}   // End of Group

#endif   // __SHELL_H__

