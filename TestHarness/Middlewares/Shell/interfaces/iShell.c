
#include "stm32f4xx.h" // To do: Remplace by Platform or Board or Target...

#include "iShell.h"


__weak void Shell_Dummy(void)
{
}

// Weak Function(s) for Initialization
__weak void Shell_Setup(void)
{
}

/*============================================================================*/
/*== Instantiation of UI Object ==========================================*/
/*============================================================================*/

/*---------------------------------------------------------------------------*/
static iShell_ts mShellObject =
{
  .Handler = Shell_Dummy,
};


iShell_ts* iShell(void)
{
	return (iShell_ts*)&mShellObject;
}

void iShell_Setup(void)
{
	Shell_Setup();
}

/// @}
  
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

