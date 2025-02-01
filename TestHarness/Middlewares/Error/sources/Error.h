#ifndef _ERROR_H__
#define _ERROR_H__

#include <stdint.h>

extern const char* gInterruptName;

void Logs(const char* aMessage);
void Format(const char *format, ...);
void Failure(const char* aFunction, uint32_t aLine, const char* aMessage, const char* aThreadName);
void Assert_Error(const char *file, int line);
void Error_Handler(void);

#define FAILURE(fmt, ...) 	Format("FAILURE; In function: '%s' @ line %d; " fmt, __func__, __LINE__, ##__VA_ARGS__)
#define INFO(fmt, ...) 	Format("INFO; In function: '%s' @ line %d; " fmt, __func__, __LINE__, ##__VA_ARGS__)
#define ERROR(fmt, ...) 	Format("ERROR; In function: '%s' @ line %d; " fmt, __func__, __LINE__, ##__VA_ARGS__)
#define WARNING(fmt, ...) 	Format("WARNING; In function: '%s' @ line %d; " fmt, __func__, __LINE__,  ##__VA_ARGS__)


#endif //#ifndef _ERROR_H__
