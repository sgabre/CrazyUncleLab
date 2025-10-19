
#ifndef __BLINKY_CONF_H_
#define __BLINKY_CONF_H_

#ifdef __cplusplus
 extern "C" {
#endif

 // <h> Options
 //   <o.0>   Default
 //   <o.1>   Message Queue
 //   <o.2>   Semaphore
 //   <o.3>   Timer
 //   <o.5>   Event Flags
 // </h>
#define  	BLINKY_OPTIONS				 0
#define 	BLINKY_DELAY                 500            /** In ms */
#define 	BLINKY_NUMBER_OF_ITEM	 	 5

#ifdef __cplusplus
}
#endif

#endif /* __BLINKY_CONF_H_ */
