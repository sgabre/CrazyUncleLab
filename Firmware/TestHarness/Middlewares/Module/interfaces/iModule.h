#ifndef __MODULE_API_H__
#define __MODULE_API_H__

/*== Includes ================================================================*/
#include <stdint.h>
/*== Defines =================================================================*/

typedef void (*iModule_CallBack) (uint32_t ID);

/*== Enumerations ============================================================*/
typedef enum
{
  MODULE_NO_ERROR,              ///< No error occurs
  MODULE_INVALID_PARAM_ERROR,   ///< au moins un paramètre est invalide.
  MODULE_INVALID_CONTEXT_ERROR, ///< au moins un paramètre du context est invalide.
} eModuleError;

/*== Structures ==============================================================*/

/*== Declaration of Module Object ===============================================*/


typedef struct Module_ts
{
  
  eModuleError (*Enable) (iModule_CallBack aCallBack, uint32_t aTimeout);
  char* (*Disable) (void);

} Module_ts;

/*== Internal Functions ======================================================*/

/*== Module Interfaces ========================================================*/

Module_ts * iModule(void);

#endif  // __MODULE_API_H__
