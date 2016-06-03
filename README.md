# Symbol Name support

Code "symbols" are addresses with names,
basically functions and global variables.

This module allows to convert symbols addresses to names and back.
This is for example useful for debug messages to print a function name
when only a function pointer is available.

Implementations for Linux and vxWorks are available.
An implementation for Windows is untested.

## symbolName

`char* symbolName(const void* ptr, unsigned int flags)`

Convert a pointer to symbol to a string with the symbol name.
If the pointer does not match a symbol, the returned
string contains the address in hex. If the pointer is the NULL
pointer, the returned string contains the string `"NULL"`.

The returned string is allocated from the heap and must be released
with `free()` after usage.

If not enough memory is available the function returns NULL and sets errno.

The following flags are supported (and can be or'ed when useful):
* `F_SYMBOL_NAME_WITH_FILE     (0x01)` 
Adds the name of the file containing the symbol in `[]`
* `F_SYMBOL_NAME_WITH_PATH     (0x02)` 
Adds the full path of the file containing the symbol in `[]`
* `F_SYMBOL_NAME_DEMANGE_FULL  (0x10)` 
Demangles C++ functions including arguments
* `F_SYMBOL_NAME_DEMANGE_NONE  (0x20)`
Does not demangle C++ functions

The default (flags = 0) is not to add the file name and to demangle
C++ function names to a terse format without arguments.

If the symbol address matches more than one name (for example if
a function has aliases) only one of those names is returned.

This function can be called from the ioc shell.

## symbolAddress

`epicsShareFunc void* symbolAddr(const char* name)`

Convert a symbol name to the matching address.
If no symbol with the given name is found, the NULL pointer is returned.
(Be aware that NULL can actually be the address of a valid symbol, but this is rare.)

At the moment, C++ symbols can only be found using the mangled name.

This function can be called from the ioc shell.
