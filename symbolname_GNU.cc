/* Utility: return function name in allocated buffer */
#include <dlfcn.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cxxabi.h>
#include "symbolname.h"

extern "C" {

char* symbolName(void* ptr, int withFilename /* 1=file, 2=full path */)
{
    Dl_info sym = {0};
    char *result;
    char *name;
    int demangleFailed;

    /* handle NULL pointer */
    if (!ptr) return strdup("NULL");

    /* resolve address and check for exact match */
    if (dladdr(ptr, &sym) == 0 || ptr != sym.dli_saddr)
    {
        asprintf(&result, "%p", ptr);
        return result;
    }

    /* demangle C++ function names */
    name = abi::__cxa_demangle(sym.dli_sname, NULL, NULL, &demangleFailed);
    if (demangleFailed) name = (char*)sym.dli_sname;
    
    /* handle file name */
    if (withFilename > 0 && sym.dli_fname != NULL)
    {
        const char *file = sym.dli_fname;
        if (withFilename == 1)
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
    void* addr = dlsym(RTLD_DEFAULT, name);
    return addr;
}

} // extern "C"
