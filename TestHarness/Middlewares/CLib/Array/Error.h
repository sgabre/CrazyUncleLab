#ifndef _ERROR_H_
#define _ERROR_H_

#ifdef __cplusplus
extern "C"
{
#endif

   #include <stdint.h>

void Failure(uint8_t aError);
uint8_t LastFailure(void); 
void ClearFailure(void); 

#define FAILURE(aValue) Failure(aValue)


#ifdef __cplusplus
   }
#endif
   
#endif /* _ERROR_H_ */
