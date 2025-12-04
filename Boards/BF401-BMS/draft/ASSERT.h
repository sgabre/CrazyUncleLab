#ifndef __ASSERT_H__
#define __ASSERT_H__

/** @defgroup   ASSERT  ASSERT-Group
                Assert implementation.
    @{
********************************************************************************
*   @name       ASSERT                                                           
*   @file       ASSERT.h                                                         
* 
*   @brief      Assert implementation.
*   
*   @authors    A.Bader                                                    
*   @date       05.06.2014                                                    
*                                                                             
*   @copyright (C) 2014 by: MICROTRONIC AG                             
*                           Duennernstrasse 32                                  
*                           CH-4702 Oensingen                                   
*******************************************************************************/

/*== Includes ================================================================*/


/*== Defines =================================================================*/


/*== Enumerations ============================================================*/


/*== Structures ==============================================================*/


/*== External Declarations ===================================================*/
extern const char *gc_pacLastIrqName;


/*== Modul Interfaces ========================================================*/

/*----------------------------------------------------------------------------*/
/** @brief      Initialize ASSERT-module. Currently, register only
                Shell Command, if enabled.

    @param      None
    @return     None

    @author     A.Bader                                     @date 05.06.2014  */
/*----------------------------------------------------------------------------*/
void ASSERT_Init(void);

/*----------------------------------------------------------------------------*/
/** @brief      Perfrom Assert. Go Either first to SVC, or if already,
                perform assert directly.
   
    @param[in]  c_pacFunc     Function Name
    @param[in]  ulLine        Line Number
    @param[in]  c_pacMsg      Additional message or NULL, if none
    @return     No return

    @author     A.Bader                                     @date 05.06.2014  */
/*----------------------------------------------------------------------------*/
void Assert(const char *c_pacFunc, uint32 ulLine, const char *c_pacMsg);

/*----------------------------------------------------------------------------*/
/** @brief      Macro definition to get calling function / line number.
   
    @param[in]  param     param, which is evaluated: 
                             == 0 -> Assert 
                             != 0, -> No Assert
    @return     None

    @author     A.Bader                                     @date 05.06.2014  */
/*----------------------------------------------------------------------------*/
#define ASSERT(param) ( (param) ? (void)0 : Assert(__func__, __LINE__, NULL) )

/*----------------------------------------------------------------------------*/
/** @brief      Macro definition to get calling function / line number.
   
    @param[in]  param     param, which is evaluated: 
                             == 0 -> Assert 
                             != 0, -> No Assert
    @param[in]  msg       message to pass. will be added as "Addition Info".
    @return     None

    @author     A.Bader                                     @date 05.06.2014  */
/*----------------------------------------------------------------------------*/
#define ASSERT_MSG(param, msg) ( (param) ? (void)0 : Assert(__func__, __LINE__, (msg)) )

/*----------------------------------------------------------------------------*/
/** @brief      Macro definition to get calling function / line number.
                Do an ASSERT in every case.
   
    @param      None
    @return     None

    @author     A.Bader                                     @date 05.06.2014  */
/*----------------------------------------------------------------------------*/
#define ASSERT_FAIL() Assert(__func__, __LINE__, NULL)

/*----------------------------------------------------------------------------*/
/** @brief      Macro definition to get calling function / line number.
                Do an ASSERT in every case.
   
    @param[in]  msg       message to pass. will be added as "Addition Info".
    @return     None

    @author     A.Bader                                     @date 05.06.2014  */
/*----------------------------------------------------------------------------*/
#define ASSERT_FAIL_MSG(msg) Assert(__func__, __LINE__, (msg))

/*----------------------------------------------------------------------------*/
/** @brief      Wrapper function from HAL library

    @param      func    function name
    @param      line    line number
    @return     None

    @author     A.Bader                                     @date 25.04.2017  */
/*----------------------------------------------------------------------------*/
void assert_failed(const char* func, uint32 line);

/// @}   // end of Group

#endif   // __ASSERT_H__
