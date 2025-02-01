
#include "stm32f4xx.h" // To do: Remplace by Platform or Board or Target...

#include "iError.h"


__weak void Error_Dummy(void)
{
}

// Weak Function(s) for Initialization
__weak void Error_Setup(void)
{
}

/*============================================================================*/
/*== Instantiation of UI Object ==========================================*/
/*============================================================================*/

/*---------------------------------------------------------------------------*/
static iError_ts mErrorObject =
{
  .Handler = Error_Dummy,
};


iError_ts* iError(void)
{
	return (iError_ts*)&mErrorObject;
}

void iError_Setup(void)
{
	Error_Setup();
}

/// @}
  
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

