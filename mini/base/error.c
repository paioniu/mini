#include "../pch.h"

#if SYSTEM == LINUX
    #include <execinfo.h>
#endif

Private int		g_errorlog		= 1;
Private int		g_errorstatus	= 0;
Private int		g_errorprintstack = 0;

Private char    *g_mini_program_name = "program";

#if SYSTEM == WINDOWS
    static void set_signal_handler();
    static int addr2line(char const * const program_name, void const * const addr);
#endif

void 	ErrorDisableLog()
{
    g_errorlog = 0;
}

void 	ErrorEnableLog()
{
    g_errorlog = 1;
}

void 	ErrorToggleLog()
{
    if( g_errorlog )
        ErrorDisableLog();
    else
        ErrorEnableLog();
}

void 	ErrorDisablePrintstack()
{
    g_errorprintstack = 0;
}

void 	ErrorEnablePrintstack()
{
    g_errorprintstack = 1;
}

void 	ErrorTogglePrintstack()
{
    if( g_errorprintstack )
        ErrorDisablePrintstack();
    else
        ErrorEnablePrintstack();
}

void 	ErrorResetStatus()
{
    g_errorstatus = 0;
}

int 	ErrorGetStatus()
{
    return g_errorstatus;
}

int 	ErrorGetPrintstackStatus()
{
    return g_errorprintstack;
}

int 	ErrorSetStatus(int val)
{
    return g_errorstatus = val;
}

int 	ErrorGetLogStatus()
{
    return g_errorlog;
}

void ErrorInit(int argc, char **argv)
{
    g_mini_program_name = argv[0];
    #if SYSTEM == WINDOWS
        set_signal_handler();
    #endif
}

#if SYSTEM == WINDOWS
    void ErrorPrintStack()
    {
        CONTEXT context;

        STACKFRAME frame = { 0 };
        SymInitialize(GetCurrentProcess(), 0, TRUE);
        GetThreadContext(GetCurrentThread(), &context);

        /* setup initial stack frame */
        frame.AddrPC.Offset         = context.Eip;
        frame.AddrPC.Mode           = AddrModeFlat;
        frame.AddrStack.Offset      = context.Esp;
        frame.AddrStack.Mode        = AddrModeFlat;
        frame.AddrFrame.Offset      = context.Ebp;
        frame.AddrFrame.Mode        = AddrModeFlat;

        while (StackWalk(IMAGE_FILE_MACHINE_I386,
                         GetCurrentProcess(),
                         GetCurrentThread(),
                         &frame,
                         &context,
                         0,
                         SymFunctionTableAccess,
                         SymGetModuleBase,
                         0 ) )
        {
            addr2line(g_mini_program_name, (void*)frame.AddrPC.Offset);
        }

        SymCleanup( GetCurrentProcess() );
    }

    static LONG WINAPI windows_exception_handler(EXCEPTION_POINTERS * ExceptionInfo)
    {
        switch(ExceptionInfo->ExceptionRecord->ExceptionCode)
        {
        case EXCEPTION_ACCESS_VIOLATION:
            fputs("Error: EXCEPTION_ACCESS_VIOLATION\n", stderr);
            break;
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
            fputs("Error: EXCEPTION_ARRAY_BOUNDS_EXCEEDED\n", stderr);
            break;
        case EXCEPTION_BREAKPOINT:
            fputs("Error: EXCEPTION_BREAKPOINT\n", stderr);
            break;
        case EXCEPTION_DATATYPE_MISALIGNMENT:
            fputs("Error: EXCEPTION_DATATYPE_MISALIGNMENT\n", stderr);
            break;
        case EXCEPTION_FLT_DENORMAL_OPERAND:
            fputs("Error: EXCEPTION_FLT_DENORMAL_OPERAND\n", stderr);
            break;
        case EXCEPTION_FLT_DIVIDE_BY_ZERO:
            fputs("Error: EXCEPTION_FLT_DIVIDE_BY_ZERO\n", stderr);
            break;
        case EXCEPTION_FLT_INEXACT_RESULT:
            fputs("Error: EXCEPTION_FLT_INEXACT_RESULT\n", stderr);
            break;
        case EXCEPTION_FLT_INVALID_OPERATION:
            fputs("Error: EXCEPTION_FLT_INVALID_OPERATION\n", stderr);
            break;
        case EXCEPTION_FLT_OVERFLOW:
            fputs("Error: EXCEPTION_FLT_OVERFLOW\n", stderr);
            break;
        case EXCEPTION_FLT_STACK_CHECK:
            fputs("Error: EXCEPTION_FLT_STACK_CHECK\n", stderr);
            break;
        case EXCEPTION_FLT_UNDERFLOW:
            fputs("Error: EXCEPTION_FLT_UNDERFLOW\n", stderr);
            break;
        case EXCEPTION_ILLEGAL_INSTRUCTION:
            fputs("Error: EXCEPTION_ILLEGAL_INSTRUCTION\n", stderr);
            break;
        case EXCEPTION_IN_PAGE_ERROR:
            fputs("Error: EXCEPTION_IN_PAGE_ERROR\n", stderr);
            break;
        case EXCEPTION_INT_DIVIDE_BY_ZERO:
            fputs("Error: EXCEPTION_INT_DIVIDE_BY_ZERO\n", stderr);
            break;
        case EXCEPTION_INT_OVERFLOW:
            fputs("Error: EXCEPTION_INT_OVERFLOW\n", stderr);
            break;
        case EXCEPTION_INVALID_DISPOSITION:
            fputs("Error: EXCEPTION_INVALID_DISPOSITION\n", stderr);
            break;
        case EXCEPTION_NONCONTINUABLE_EXCEPTION:
            fputs("Error: EXCEPTION_NONCONTINUABLE_EXCEPTION\n", stderr);
            break;
        case EXCEPTION_PRIV_INSTRUCTION:
            fputs("Error: EXCEPTION_PRIV_INSTRUCTION\n", stderr);
            break;
        case EXCEPTION_SINGLE_STEP:
            fputs("Error: EXCEPTION_SINGLE_STEP\n", stderr);
            break;
        case EXCEPTION_STACK_OVERFLOW:
            fputs("Error: EXCEPTION_STACK_OVERFLOW\n", stderr);
            break;
        default:
            fputs("Error: Unrecognized Exception\n", stderr);
            break;
        }
        fflush(stderr);
        /* If this is a stack overflow then we can't walk the stack, so just show
          where the error happened */
        if (EXCEPTION_STACK_OVERFLOW != ExceptionInfo->ExceptionRecord->ExceptionCode)
        {
            ErrorPrintStack(ExceptionInfo->ContextRecord);
        }
        else
        {
            addr2line(g_mini_program_name, (void*)ExceptionInfo->ContextRecord->Eip);
        }

        return EXCEPTION_EXECUTE_HANDLER;
    }

    static void set_signal_handler()
    {
        SetUnhandledExceptionFilter(windows_exception_handler);
    }

    /* Resolve symbol name and source location given the path to the executable
       and an address
       Fonte https://spin.atomicobject.com/2013/01/13/exceptions-stack-traces-c/
       */
    static int addr2line(char const * const program_name, void const * const addr)
    {
        char addr2line_cmd[512] = {0};

        /* have addr2line map the address to the relent line in the code */

        sprintf(addr2line_cmd,"addr2line -f -p -e %.256s %p", program_name, addr);

        /* This will print a nicely formatted string specifying the
           function and source line of the address */
        return system(addr2line_cmd);
    }
#elif SYSTEM == LINUX
    void ErrorPrintStack()
    {
        void *array[10];
        size_t size;
        char **strings;
        size_t i;

        size = backtrace (array, 10);
        strings = backtrace_symbols (array, size);

        //printf ("Obtained %zd stack frames.\n", size);

        for (i = 1; i < size; i++)
            printf ("%s\n", strings[i]);

        free (strings);
    }
#endif
