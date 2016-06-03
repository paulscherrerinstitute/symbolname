/* Utility: return function name in allocated buffer */
#define _GNU_SOURCE /* needed for dladdr() */
#include <dlfcn.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* #include <cxxabi.h> hard to find from non C++ code */
extern char*
__cxa_demangle(const char* __mangled_name, char* __output_buffer,
		 size_t* __length, int* __status);

#include "epicsExport.h"
#include "symbolname.h"

char* symbolName(void* ptr, unsigned int flags)
{
    Dl_info sym = {0};
    char *result;
    char *name;
    int demangleFailed = 1;

    /* handle NULL pointer */
    if (!ptr) return strdup("NULL");

    /* resolve address and check for exact match */
    if (dladdr(ptr, &sym) == 0 || ptr != sym.dli_saddr)
    {
        asprintf(&result, "%p", ptr);
        return result;
    }

    name = (char*)sym.dli_sname;
    /* demangle C++ function names */
    if (!(flags & F_SYMBOL_NAME_DEMANGE_NONE))
    {
        name = __cxa_demangle(sym.dli_sname, NULL, NULL, &demangleFailed);
        if (demangleFailed) name = (char*)sym.dli_sname;
        else if (!(flags & F_SYMBOL_NAME_DEMANGE_FULL))
        {
            char *p = strrchr(name, '(');
            if (p) *p = 0;
        }
    }
    
    /* add file name */
    if (sym.dli_fname && flags & (F_SYMBOL_NAME_WITH_FILE | F_SYMBOL_NAME_WITH_PATH))
    {
        const char *file = sym.dli_fname;
        if (!(flags & F_SYMBOL_NAME_WITH_PATH))
        {
            const char *p = strrchr(file, '/');
            if (p) file = p+1;
        }
        asprintf(&result, "%s [%s]", name, file);
        if (!demangleFailed) free(name);
        return result;
    }
    if (!demangleFailed) return name;
    return strdup(name);
}

void* symbolAddr(const char* name)
{
    if (!name) return NULL;
    void* addr = dlsym(RTLD_DEFAULT, name);
    return addr;
}
