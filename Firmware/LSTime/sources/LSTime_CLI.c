#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#ifdef RTE_CMSIS_RTOS2
#include "Shell.h"
#endif

//#include "LSTime_Config.h"

#include "LSTime.h"
#include "LSTime_CLI.h"

#include "getopt.h"

#ifdef RTE_CMSIS_RTOS2
static int mLocalSiderealTime_ShellHandler(int argc, char **argv);
/*--------------------------------------------------------------------------*/
/// this text will be displayed, in the shell-command overview
static const char mLocalSiderealTimeSummary[] = 
"The default action is to add or replace zipfile entries from list, which \r\n"
" can include the special name - to compress standard input..\r\n";

/// this text will be displayed, if this module return E_SHELL_CMD_WRONG_PARAMETERS
static const char mLocalSiderealTimeUsage[] =
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
    SHELL_ADD_CMD("lstime", mLocalSiderealTimeSummary, mLocalSiderealTime_ShellHandler);
}
#endif

struct option LocalSiderealTimeOption[] = {
    {"lat", required_argument, 0, 'L'},
    {"lng", required_argument, 0, 'l'},
    {"alt", required_argument, 0, 'a'},
    {"date", required_argument, 0, 'D'},
    {"time", required_argument, 0, 'T'},
    {"version", no_argument, 0, 'v'},
    {"help", no_argument, 0, 'h'},
    {0, 0, 0, 0}
};

#ifdef RTE_CMSIS_RTOS2
int mLocalSiderealTime_ShellHandler(int argc, char **argv)
#else
int main(int argc, char **argv) 
#endif
{
    int opt;
    double aLongitude = 0;

    bool HasEntry = false;
    
    int day = 1, month = January, year = 1968;
    int hours = 0, minutes = 0, seconds  = 0;
    time_t aTimestamp = time( NULL );
    char DateTimeString[80] = {0};
    
    #ifdef RTE_CMSIS_RTOS2
    clropt();
    #endif
    
    //while((opt = getopt(argc, argv, ":if:lrx")) != -1)  
    while ((opt = getopt_long(argc, argv, "q:L:l:a:i:D:T:h", LocalSiderealTimeOption, NULL)) != -1)
    {  
        switch(opt)  
        {  
            case 'L': 
                printf("LAT: %s\r\n",optarg);
                break;
            case 'l': 
                //printf("LNG: %s\r\n",optarg);
                aLongitude = atof(optarg);
                break;
            case 'a': 
                printf("ALT:  %s\r\n",optarg);
                break;
            case 'D': 
                sscanf(optarg, "%d/%d/%d", &day, &month, &year);
                month--;
                HasEntry = true;
                break;
            case 'T': 
                sscanf(optarg, "%d:%d:%d", &hours, &minutes, &seconds);
                HasEntry = true;
                break;
            case 'v': 
                printf("Version:\r\n");
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

    struct tm aDate  = DateTime(day, month, year, hours, minutes, seconds);
    
    if( HasEntry == true)
    {
        aTimestamp = mktime( &aDate );
    }

    double LSTime = LocalSiderealTime(aTimestamp, aLongitude );

    struct tm* pTime = localtime( & aTimestamp );
    strftime( DateTimeString, 80, "%d/%m/%Y %H:%M:%S", pTime );
    
    printf("Local Sidereal Time: %g @ LNG: %g @ %s\r\n", LSTime, aLongitude, DateTimeString); 
    
    return 0;
}

