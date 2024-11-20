#include <stdbool.h>
#include <stdint.h>

#include "main.h"//Change to RTE_Device.h
#include "cmsis_os.h"

#include "PowerOnReset.h"
#include "Blinky.h"

const osThreadAttr_t attributes =
{
	.name = "defaultTask",
	.stack_size = 128 * 4,
	.priority = (osPriority_t) osPriorityNormal,
};

typedef struct PowerOnResetContext_ts
{
	bool IsRunning;
	osThreadId_t TaskID;
}PowerOnResetContext_ts;

static PowerOnResetContext_ts mPowerOnResetContext;

void PowerOnReset_Task(void *argument)
{
	//Board_Setup();
	Blinky_Setup();
	//Button_Setup();
	//Shell_Setup();
	
	// Kill Initial / Main Task
	 // OSEXT_ThreadTerminate(OSEXT_ThreadGetId());

}

void PowerOnReset_Setup(void)
{
	mPowerOnResetContext.TaskID = osThreadNew(PowerOnReset_Task, NULL, &attributes);
	//Change  osThreadNew to osalThreadNew(PowerOnReset_Task,128 * 4, osPriorityNormal)
}
