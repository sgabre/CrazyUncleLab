#ifdef __cplusplus
#error This is not C++. Please use a C compiler.
#endif

#include "Error.h"

static uint8_t mError;


void Failure(uint8_t aError)
{
   mError = aError;
}

uint8_t LastFailure(void)
{
   return mError;
}

void ClearFailure(void)
{
   mError = 0;
}