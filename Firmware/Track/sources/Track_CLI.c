#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#ifdef RTE_CMSIS_RTOS2
#include "Shell.h"
#endif

//#include "LSTime_Config.h"

#include "Track.h"
#include "Track_CLI.h"

#include "getopt.h"

#ifdef RTE_CMSIS_RTOS2
static int mTrack_ShellHandler(int argc, char **argv);
/*--------------------------------------------------------------------------*/
/// this text will be displayed, in the shell-command overview
static const char mTrackSummary[] = 
"The default action is to add or replace zipfile entries from list, which \r\n"
" can include the special name - to compress standard input..\r\n";

/// this text will be displayed, if this module return E_SHELL_CMD_WRONG_PARAMETERS
static const char mTrackUsage[] =
"zip <function> \r\n"
"\r\n"
"Functions:\r\n"
"'zip create <archive_name>'     \t-- create empty file archive - don't forget to arc close after\r\n"
"'zip append <archive_name>'     \t-- open a file archive to append a file\r\n"
"'zip open <archive_name>'       \t-- open a file archive read only\r\n"
"'zip add <file_name>'           \t-- add a file to the archive\r\n"
"'zip add_comp <file_name> <comp_file_name>'      \t-- add a compressed file to the archive <comp_file_name> is deleted automatically\r\n"
"'zip close'                     \t-- close a file or archive\r\n"
"'zip extract <archive_name>'    \t-- extract all files of an archive\r\n"
"'zip list <archive_name>'       \t-- list all files of an archive\r\n"
"'zip help <file_name>'          \t-- show this help function\r\n";


void LSTimeShell_Setup(void)
{
    // use module name as parameter or check with shell-cmd "help" that parameter isn't used
    //SHELL_ADD_CMD("zip", mArchive_ShellHandler, mArchiveSummary, mArchiveUsage);
    SHELL_ADD_CMD("lstime", mTrackSummary, mTrack_ShellHandler);
}
#endif

struct option TrackOption[] = {
    {"asc", required_argument, 0, 'a'},
    {"dec", required_argument, 0, 'd'},
    {"version", no_argument, 0, 'v'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}
};

#ifdef RTE_CMSIS_RTOS2
int mTrack_ShellHandler(int argc, char **argv)
#else
int main(int argc, char **argv) 
#endif
{
    int opt;
    char DateTimeString[80] = {0};
    
    #ifdef RTE_CMSIS_RTOS2
    clropt();
    #endif
    
    //while((opt = getopt(argc, argv, ":if:lrx")) != -1)  
    while ((opt = getopt_long(argc, argv, "q:L:l:a:i:D:T:h", TrackOption, NULL)) != -1)
    {  
        switch(opt)  
        {  
            case 'a': 
                printf("LAT: %s\r\n",optarg);
                break;
            case 'd': 
                printf("LAT: %s\r\n",optarg);
                break;
            case 'v': 
                printf("Track - Version: 0.0.0 - Revision: 0123456789ABCDEF - Build Date & Time: 27/07/1979 @ 10:30:45\r\n");
                break;
            case 'q': 
                printf("quit\r\n");
                break;
            case 'h': 
                printf("Uasge...\r\n");
                break;
            case '?':  
                printf("unknown option: %c\n", optopt); 
                break;  
        }  
    }  
    
    // optind is for the extra arguments 
    // which are not parsed 
    for(; optind < argc; optind++)
    {    
        printf("extra arguments: %s\n", argv[optind]); 
    }     

    //Track(aAscension, aDeclinaison);

    
    //printf("Local Sidereal Time: %g @ LNG: %g @ %s\r\n", LSTime, aLongitude, DateTimeString); 
    return 0;
}

