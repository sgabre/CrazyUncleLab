#include "main.h"//Change to RTE_Device.h
#include "cmsis_os.h"

#include "Blinky.h"

typedef struct BlinkyContext_ts
{
	osTimerId_t TimerID;
}BlinkyContext_ts;

static BlinkyContext_ts mBlinkyContext;


void Blinky_Task(void *argument)
{
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}


void Blinky_Setup(void)
{
	mBlinkyContext.TimerID = osTimerNew(Blinky_Task, osTimerPeriodic, NULL, NULL);


	osTimerStart(mBlinkyContext.TimerID, 500);

}
