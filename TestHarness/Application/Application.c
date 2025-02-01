#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "DEVICE_IO.h"

#include "iShell.h"

void PowerOnReset_Setup(void)
{
	printf("Test Harness - VER: X.X.X - REV: XXXXXXXXXXXXXXX\n");

	iShell_Setup();

	//Software Component...

	while(1)
	{
		iShell()->Handler();
	};
}
