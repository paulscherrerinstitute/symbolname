#ifndef symbolname_h
#define symbolname_h

#ifndef epicsShareFunc
#define epicsShareFunc
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Resolve pointer to function or variable name in allocated buffer */
/* release with free() */
epicsShareFunc char* symbolName(const void* ptr, unsigned int flags);
/* flags: */
#define F_SYMBOL_NAME_WITH_FILE     0x01
#define F_SYMBOL_NAME_WITH_PATH     0x02
#define F_SYMBOL_NAME_DEMANGLE_FULL 0x10
#define F_SYMBOL_NAME_DEMANGLE_NONE 0x20
#define F_SYMBOL_NAME_ALLOW_OFFSET  0x40

/* There was a typo in earlier versions that may still be used */
#define F_SYMBOL_NAME_DEMANGE_FULL F_SYMBOL_NAME_DEMANGLE_FULL
#define F_SYMBOL_NAME_DEMANGE_NONE F_SYMBOL_NAME_DEMANGLE_NONE

/* Resolve function or variable name to pointer */
epicsShareFunc void* symbolAddr(const char* name);

#ifdef __cplusplus
}
#endif

#endif /* symbolname_h */
