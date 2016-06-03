#ifndef symbolname_h
#define symbolname_h

#include "shareLib.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Resolve pointer to function or variable name in allocated buffer */
/* release with free() */
epicsShareFunc char* symbolName(void* ptr, unsigned int flags);
/* flags: */
#define F_SYMBOL_NAME_WITH_FILE    0x01
#define F_SYMBOL_NAME_WITH_PATH    0x02
#define F_SYMBOL_NAME_DEMANGE_FULL 0x10
#define F_SYMBOL_NAME_DEMANGE_NONE 0x20

/* Resolve function or variable name to pointer */
epicsShareFunc void* symbolAddr(const char* name);

#ifdef __cplusplus
}
#endif

#endif /* symbolname_h */
