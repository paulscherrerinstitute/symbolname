#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "iocsh.h"
#include "symbolname.h"
#include "epicsExport.h"

#ifndef __linux
#define strcasestr strstr
#endif

static const iocshFuncDef symbolNameDef =
    { "symbolName", 2, (const iocshArg *[]) {
    &(iocshArg) { "ptr", iocshArgString },
    &(iocshArg) { "flags", iocshArgString },
}};

static void symbolNameFunc (const iocshArgBuf *args)
{
    size_t addr = 0;
    char* name;
    char* flagstr = args[1].sval;
    long flags = 0;

    if (args[0].sval) addr = strtoul(args[0].sval, NULL, 0);

    if (flagstr) {
        flags = strtol(flagstr, NULL, 0);
        if (strcasestr(flagstr, "FILE")) flags |= F_SYMBOL_NAME_WITH_FILE;
        if (strcasestr(flagstr, "PATH")) flags |= F_SYMBOL_NAME_WITH_PATH;
        if (strcasestr(flagstr, "OFF"))  flags |= F_SYMBOL_NAME_ALLOW_OFFSET;
        if (strcasestr(flagstr, "FULL")) flags |= F_SYMBOL_NAME_DEMANGLE_FULL;
        if (strcasestr(flagstr, "NODEM")) flags |= F_SYMBOL_NAME_DEMANGLE_NONE;
    }

    name = symbolName((void*)addr, flags);
    printf("%s\n", name);
    free(name);
}

static const iocshFuncDef symbolAddrDef =
    { "symbolAddr", 1, (const iocshArg *[]) {
    &(iocshArg) { "name", iocshArgString },
}};

static void symbolAddrFunc (const iocshArgBuf *args)
{
    void* addr = symbolAddr(args[0].sval);
    if (!addr) printf ("NULL\n");
    else printf("%p\n", addr);
}

static void symbolnameRegistrar(void)
{
    iocshRegister(&symbolNameDef, symbolNameFunc);
    iocshRegister(&symbolAddrDef, symbolAddrFunc);
}

epicsExportRegistrar(symbolnameRegistrar);
