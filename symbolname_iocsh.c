#include <stdio.h>
#include <stdlib.h>
#include "iocsh.h"
#include "symbolname.h"
#include "epicsExport.h"

static const iocshFuncDef symbolNameDef =
    { "symbolName", 2, (const iocshArg *[]) {
    &(iocshArg) { "ptr", iocshArgString },
    &(iocshArg) { "flags", iocshArgInt },
}};

static void symbolNameFunc (const iocshArgBuf *args)
{
    size_t addr = 0;
    char* name;
    
    if (args[0].sval) addr = strtoul(args[0].sval, NULL, 0);
    name = symbolName((void*)addr, args[1].ival);
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
