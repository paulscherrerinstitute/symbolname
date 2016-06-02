#if defined _DEBUG
#include <dbghelp.h>
#endif

char* WINAPI symbolName(void* ptr, int withFilename /* 1=file, 2=full path */)
{
    /* handle NULL pointer */
    if (!ptr) return _strdup("NULL");

#if defined _DEBUG
    CRITICAL_SECTION lock;
    static HANDLE processHandle;
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

#if defined _DEBUG
    if (withFilename > 0)
    {
        IMAGEHLP_LINE64 line;
        line.SizeOfStruct = sizeof(line);
        EnterCriticalSection(&lock);
        success = SymGetLineFromAddr64(processHandle, addr, &dis, &line);
        LeaveCriticalSection(&lock);
        if (success)
        {
            const char *file = line.FileName;
            if (withFilename == 1)
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
