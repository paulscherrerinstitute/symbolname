#ifndef symbolname_h
#define symbolname_h

#include "shareLib.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Resolve pointer to function or variable name in allocated buffer */
/* release with free() */
epicsShareFunc char* symbolName(void* ptr,
    int withFilename /* 0=no file name, 1=with file name, 2=with full path */);

/* Resolve function or variable name to pointer */
epicsShareFunc void* symbolAddr(const char* name);

#ifdef __cplusplus
}
#endif

#endif /* symbolname_h */
