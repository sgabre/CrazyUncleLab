#include <stdio.h>
#include "main.h"//Change to RTE_Device.h
#include "cmsis_os.h"

#include "OSAL.h"
#include "Blinky.h"

typedef struct BlinkyContext_ts
{
	osThreadId_t TaskID;

#if (BLINKY_OPTIONS > 0)
	osThreadId_t DelayID;
#endif

#if (BLINKY_OPTIONS == 1)
	osMessageQueueId_t QueueID;
#endif

#if (BLINKY_OPTIONS == 2)
	osSemaphoreId_t SemaphoreID;
#endif

#if (BLINKY_OPTIONS == 3)
	osTimerId_t TimerID;
#endif

	/*

#if (BLINKY_OPTIONS & 2)
	osMessageQueueId_t QueueID;
#endif
#if (BLINKY_OPTIONS & 2)
	osSemaphoreId_t SemaphoreID;
	osMutexId_t MutexID;
	osEventFlagsId_t 	EventFlagID;
#endif
*/
}BlinkyContext_ts;

static BlinkyContext_ts mBlinkyContext;


void Blinky_Task(void *argument)
{
	uint16_t Delay = 500;

#if (BLINKY_OPTIONS == 1)
	osStatus_t status;

#endif



#if (BLINKY_OPTIONS != 3)
	while(1)
	{
#endif

#if (BLINKY_OPTIONS == 1)
		status = osMessageQueueGet(mBlinkyContext.QueueID, &Delay, 0, 10);
		    if(status == osOK)
		    {
#endif
#if (BLINKY_OPTIONS == 2)
		    osSemaphoreAcquire(mBlinkyContext.SemaphoreID, osWaitForever);
#endif

		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

#if (BLINKY_OPTIONS == 1)
		    }
#endif
#if (BLINKY_OPTIONS != 3)		
    osDelay(Delay);

	}
#endif
}

#if (BLINKY_OPTIONS > 0)
void Blinky_DelayTask(void *argument)
{
	uint16_t Delay = 750;
	while(1)
		{
			osDelay(Delay);
#if (BLINKY_OPTIONS == 1)
			osMessageQueuePut(mBlinkyContext.QueueID, &Delay, 0, osWaitForever);
#endif

#if (BLINKY_OPTIONS == 2)
			osSemaphoreRelease(mBlinkyContext.SemaphoreID);
#endif


		}

}
#endif



void Blinky_Setup(void)
{
	printf("Option #%d\r\n", BLINKY_OPTIONS);





#if (BLINKY_OPTIONS == 1)
	mBlinkyContext.QueueID = osMessageQueueNew(BLINKY_NUMBER_OF_ITEM, sizeof(uint16_t), NULL);
#endif

#if (BLINKY_OPTIONS == 2)
	mBlinkyContext.SemaphoreID = osSemaphoreNew(BLINKY_NUMBER_OF_ITEM, BLINKY_NUMBER_OF_ITEM, NULL);
#endif



#if (BLINKY_OPTIONS == 3)
  mBlinkyContext.TimerID = osTimerNew(Blinky_Task, osTimerPeriodic, NULL, NULL);
  osTimerStart(mBlinkyContext.TimerID, BLINKY_DELAY);
#endif  
  
  
#if ( (BLINKY_OPTIONS > 0) && (BLINKY_OPTIONS != 3) )
	mBlinkyContext.TaskID = osalThreadNew(Blinky_DelayTask, NULL, "Blinky_DelayTask-Delay-Task", 128 * 4, osPriorityNormal);
#endif

#if (BLINKY_OPTIONS != 3)
	mBlinkyContext.TaskID = osalThreadNew(Blinky_Task, NULL, "Blinky-Task", 128 * 4, osPriorityNormal);
#endif

}






/*
#if (BLINKY_OPTIONS & 2)
osStatus_t status;
#endif

#if (BLINKY_OPTIONS & 2)
uint16_t msg = 0;
#endif

#if (BLINKY_OPTIONS & 2)
while(1) 
{

status = osMessageQueueGet(mBlinkyContext.QueueID, &msg, 0, 10);
if(status == osOK)
delay = msg;
#endif

#if (BLINKY_OPTIONS & 2)
status = osMessageQueueGet(mBlinkyContext.QueueID, &msg, 0, 10);
if(status == osOK)
delay = msg;
#endif

#if (BLINKY_OPTIONS & 2)
osSemaphoreAcquire(mBlinkyContext.SemaphoreID, osWaitForever);
#endif
*/



/*
#if (BLINKY_OPTIONS & 2)
osDelay(500);
}
#endif
*/



/*
void blinkThread(void *argument) {

uint16_t msg = 0;
osStatus_t status;

while(1) {
status = osMessageQueueGet(msgQueueID, &msg, 0, 10);
if(status == osOK)
delay = msg;

HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
osDelay(delay);
}
}

void blinkThread(void *argument) {
while(1) {
osSemaphoreAcquire(semID, osWaitForever);
HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}
}

void delayThread(void *argument) {
while(1) {
osDelay(500);
osSemaphoreRelease(semID);
}
}

void blinkThread(void *argument) {
while(1) {
osEventFlagsWait(evtID, FLAG_LED_BLINK, osFlagsWaitAll, osWaitForever);
HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}
}

void delayThread(void *argument) {
uint8_t step = 100;
uint16_t delay = 500;

while(1) {
osEventFlagsSet(evtID, FLAG_LED_BLINK);

if(osEventFlagsWait(evtID, FLAG_CHANGE_FREQUENCY, osFlagsWaitAll, delay) != osFlagsErrorTimeout ) {
delay -= step;
switch(delay) {
case 100:
step = 50;
break;
case 50:
step = 25;
break;
case 0:
step = 100;
delay = 500;
break;
}
}
}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
if(GPIO_Pin == GPIO_PIN_13)
osEventFlagsSet(evtID, FLAG_CHANGE_FREQUENCY);
}
*/
