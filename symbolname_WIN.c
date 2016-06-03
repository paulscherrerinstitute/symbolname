#if defined _DEBUG
#include <dbghelp.h>
static HANDLE processHandle;
static CRITICAL_SECTION lock;
#else
#warning Not built with debug information.
#endif

#include "epicsExport.h"
#include "symbolname.h"

char* symbolName(const void* ptr, unsigned int flags)
{
    /* handle NULL pointer */
    if (!ptr) return _strdup("NULL");

#if defined _DEBUG
    char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME*sizeof(TCHAR)] = {0};
    DWORD64 dis = 0;
    PSYMBOL_INFO pSym = (PSYMBOL_INFO)buffer;
    BOOL success;

    pSym->SizeOfStruct = sizeof(PSYMBOL_INFO);
    pSym->MaxNameLength = MAX_SYM_NAME;

    EnterCriticalSection(&lock);
    if (!processHandle)
    {
        processHandle = GetCurrentProcess();
        SymInitialize(processHandle, NULL, TRUE);
    }

    success = SymFromAddr(processHandle, ptr, &dis, pSym);
    LeaveCriticalSection(&lock);

    if (!success || dis != 0)
#endif
    {
        result = malloc(sizeof(void*)*2+3)
        if (result) sprintf(result, "%p", ptr);
        return result;
    }

    /* demangle C++ function names */
    /* TODO How does this work? */    
    
#if defined _DEBUG
    /* add file name */
    if (flags & (F_SYMBOL_NAME_WITH_FILE | F_SYMBOL_NAME_WITH_PATH))
    {
        IMAGEHLP_LINE64 line;
        line.SizeOfStruct = sizeof(line);
        EnterCriticalSection(&lock);
        success = SymGetLineFromAddr64(processHandle, addr, &dis, &line);
        LeaveCriticalSection(&lock);
        if (success)
        {
            const char *file = line.FileName;
            if (!(flags & F_SYMBOL_NAME_WITH_PATH))
            {
                const char *p = strrchr(file, '\\');
                if (p) file = p+1;
            }
            result = malloc(strlen(pSym->Name) + strlen(file) + 24);
            sprintf(result, "%s [%s:%d]", name, file, line.LineNumber);
            return result;
        }
    }
    return _strdup(pSym->Name);
#endif
}

void* symbolAddr(const char* name)
{
#if defined _DEBUG
    SYMBOL_INFO sym = {0};
    BOOL success;

    if (!name) return NULL;
    sym.SizeOfStruct = sizeof(PSYMBOL_INFO);
    sym.MaxNameLength = 0;

    EnterCriticalSection(&lock);
    if (!processHandle)
    {
        processHandle = GetCurrentProcess();
        SymInitialize(processHandle, NULL, TRUE);
    }

    success = SymFromName(processHandle, name, &sym);
    LeaveCriticalSection(&lock);

    if (success)
    {
        return sym.Address;
    }
#endif
    return NULL;
}
