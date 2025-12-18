/**
 * @file    main.c
 * @brief   Testharness use for the Component Testing, Component Integration Testing.
 *
 *  Some/All the Option are defined in the TestHarness_Config.h file. ?
 * 
 * This is a minimal main file to run the tests.
 *  For the semi-hosting, the following option can be used
 *   - rdimon
 *   - RTT Segger 
 *   - ITM
 *   - UART
 *   - SWO
 *  For the coverage testing, the following option can be used
 *    - gcov
 *    - lcov
 *    - gcovr
 * 
 * @author  sgabre
 * @date    2025-10-19
 * 
 */

 /**
 * @file    main.c
 * @brief   TestHarness firmware used for component testing and integration testing.
 *
 * Options selected in TestHarness_Config.h:
 *   TRACE_BACKEND = { UART, ITM, SWO, SEGGER_RTT, SEMIHOST }
 *   COVERAGE      = { NONE, GCOV }
 *
 * Minimal firmware: blink LED + print trace message.
 */
#include "TestHarness_Config.h"

#include <stdio.h>

#include "iBoard.h" 
#ifdef USE_LED
#include "iLed.h"
#endif

int main(void)
{
    iBoard->Init();
    printf("TestHarness started.\n");

    while (1)
    {
        #ifdef USE_LED
        iLED->Toggle(DEFAULT_LED);
        #endif
        printf("Tick\n");
        iBoard->Delay(500000);
    }
}
