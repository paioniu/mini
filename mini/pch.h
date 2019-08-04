#ifndef MINI_PCH_H
#define MINI_PCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "projconf.h"
#include "base/error.h"
#include "base/type.h"

#if SYSTEM == WINDOWS
    //#include <windows.h>
    #include <stdarg.h>
    #include <windef.h>
    #include <winnt.h>
    #include <winbase.h>
    #include <imagehlp.h>
#endif

#endif // MINI_PCH_H
