#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "Version.h"
#include "Module.h"

void PowerOnReset_Setup(void)
{
	printf("\r\Module - VER: %s - REV: %s - BUILDTIME: %s\r\n", VERSION, LREVISION, BUILDTIME);

	
}
