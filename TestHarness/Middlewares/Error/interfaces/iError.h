#ifndef _ERROR_API_H__
#define _ERROR_API_H__

//#include "Shell_Config.h"
//#include "Shell.h"


typedef struct
{
  void (*Handler) (void);
} iError_ts;

iError_ts* iError(void);

void iError_Setup(void);


#endif //#ifndef _ERROR_API_H__
