#include <vxWorks.h>
#include <symLib.h>
#include <private/symLibP.h>
#include <sysSymTbl.h>
#include <moduleLib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <cplusLib.h>

#include "symbolname.h"

char* symbolName(const void* ptr, unsigned int flags)
{
    char* result = NULL;
    SYMBOL* symId;
    char buffer[256] = {0};
    char* name;
    size_t offs = 0;

    /* handle NULL pointer */
    if (ptr == NULL)
    {
        result = malloc(5);
        if (result) strcpy(result, "NULL");
        return result;
    }

    /* resolve address and check for exact match */
    if (symFindSymbol(sysSymTbl, NULL, (void*)ptr, SYM_TEXT, SYM_MASK_ALL-SYM_GLOBAL, &symId) != OK
        || ((offs = ptr - symId->value) && !(flags & F_SYMBOL_NAME_ALLOW_OFFSET)))
    {
        result = malloc(sizeof(void*)*2+3);
        if (result) sprintf(result, "%p", ptr);
        return result;
    }

    /* demangle C++ function names */
    if (flags & F_SYMBOL_NAME_DEMANGLE_NONE)
        name = symId->name;
    else
    {
        int oldmode = cplusDemanglerMode;
        if (flags & F_SYMBOL_NAME_DEMANGLE_FULL)
            cplusDemanglerSet(COMPLETE);
        else
            cplusDemanglerSet(TERSE);
        name = cplusDemangle(symId->name, buffer, sizeof(buffer)-1);
        cplusDemanglerSet(oldmode);
    }
    if (offs)
    {
        char *plainname = name;
        name = malloc(strlen(plainname)+sizeof(void*)*2+4);
        if (name) sprintf(name, "%s+%#x", plainname, offs);
    }

    /* add file name */
    if (flags & (F_SYMBOL_NAME_WITH_FILE | F_SYMBOL_NAME_WITH_PATH))
    {
        const char* file = moduleNameGet(moduleFindByGroup(symId->group));
        if (!(flags & F_SYMBOL_NAME_WITH_PATH))
        {
            const char *p = strrchr(file, '/');
            if (p) file = p+1;
        }
        result = malloc(strlen(name) + strlen(file) + 4);
        if (result) sprintf(result, "%s [%s]", name, file);
        if (name != symId->name && name != buffer) free(name);
        return result;
    }
    if (name != symId->name && name != buffer) return name;
    result = malloc(strlen(name) + 1);
    if (result) strcpy(result, name);
    return result;
}

void* symbolAddr(const char* name)
{
    char* addr = NULL;
    SYM_TYPE type;

    if (!name) return NULL;
    symFindByName(sysSymTbl, (char*)name, &addr, &type);
    return addr;
}
