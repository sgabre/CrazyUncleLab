#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "stm32f4xx_hal.h"

#include "Error.h"

typedef struct ShellContext_ts
{
    bool 	IsSetup;

}ErrorContext_ts;

static ErrorContext_ts mErrorContext;

void Error_HardFault(uint32_t* aAbortAddress)
{
	  char acMsg[128];

	  // Check if the HardFault was forced
	  if (SCB->HFSR & SCB_HFSR_FORCED_Msk)
	  {
	    if (SCB->CFSR & SCB_CFSR_MEMFAULTSR_Msk)
	    {
	      sprintf(acMsg, "Memory Fault: MMAR=0x%08X", (unsigned int)SCB->MMFAR);
	    }
	    else if (SCB->CFSR & SCB_CFSR_BUSFAULTSR_Msk)
	    {
	      sprintf(acMsg, "Bus Fault: BFAR=0x%08X", (unsigned int)SCB->BFAR);
	    }
	    else if (SCB->CFSR & SCB_CFSR_USGFAULTSR_Msk)
	    {
	      sprintf(acMsg, "Usage Fault: CFSR=0x%08X", (unsigned int)SCB->CFSR);
	    }
	    else
	    {
	      sprintf(acMsg, "Unknown Fault: CFSR=0x%08X", (unsigned int)SCB->CFSR);
	    }
	  }
	  else
	  {
	    sprintf(acMsg, "Non-Forced HardFault: PC=0x%08X", (unsigned int)aAbortAddress);
	  }

	  // Log the fault
	  //ASSERT_FAIL_MSG(acMsg);

	  // Save fault details for post-mortem debugging
	  //SaveFaultLog(aAbortAddress);

	  // If debugger connected, halt; otherwise, reset
	  if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
	  {
	    __BKPT(0); // Breakpoint for debugging
	  }
	  else
	  {
	    NVIC_SystemReset(); // Reset the system
	  }
}

const char* gInterruptName;

void Logs(const char* aMessage)
{
	char TimestampString[128] = {0};

	//time_t timestamp = time( NULL );
	//struct tm * pTime = localtime( & timestamp );
	//strftime( TimestampString, 128, "%d/%m/%Y %H:%M:%S", pTime );

	//RTC_TimeTypeDef sTime;
	//RTC_DateTypeDef sDate;

	//HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	//HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

	//sprintf(timestamp, "[%02d-%02d-%04d %02d:%02d:%02d] ", sDate.Date, sDate.Month, 2000 + sDate.Year, sTime.Hours, sTime.Minutes, sTime.Seconds);

	if(strlen(TimestampString) == 0)
	{
		printf("%s", aMessage);
	}
	else
	{
		printf("%s; %s", TimestampString, aMessage);
	}


#if ( SAVE_IN_RAM  == 1)
#endif

#if ( SAVE_IN_ROM == 1)
#endif


}

void Format(const char *format, ...)
{
	char buffer[128]; // Adjust size as needed
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer), format, args);
	va_end(args);

	Logs(buffer); // Replace with HAL_UART_Transmit for UART logging
}

void Failure(const char* aFunction, uint32_t aLine, const char* aMessage, const char* aThreadName)
{
	const char* Source = NULL;
	char buffer[128] = {0};

	// If no function name is given, use const name "UnknownFunction"
	if (aFunction == NULL)
		aFunction = "Unknown";

	// get name of thread / IRQ, which produced assert
	Source = gInterruptName ? gInterruptName : aThreadName;

	snprintf(buffer, sizeof(buffer), "FAILURE: In function: '%s' @ line %d, %s: %s", aFunction, (int)aLine,
		gInterruptName ? "IRQ" : "Thread", Source ? Source : "Unknown");

	if (aMessage)
	{
		snprintf(buffer + strlen(buffer), sizeof(buffer)-1 - strlen(buffer), "\r\nAdditional Info: %s\r\n", aMessage);
	}

	Logs(buffer);

	/*
		  __disable_irq();
		  while (1)
		  {
		  }
		  */

}


void Error_Setup(void)
{

	if (mErrorContext.IsSetup == false)
	{


		mErrorContext.IsSetup = true;
	}

	//iError()->Handler = mShell_Handler;

}
