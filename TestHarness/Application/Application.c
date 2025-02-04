#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "DEVICE_IO.h"

//#include "iShell.h"

void PowerOnReset_Setup(void)
{
	printf("Test Harness - VER: X.X.X - REV: XXXXXXXXXXXXXXX\r\n");

	//iShell_Setup();

	//Software Component...
	int i = 0;
	while(1)
	{
		printf("%d\r\n", i++);
		HAL_Delay(500);
		//iShell()->Handler();
	};
}
