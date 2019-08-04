#ifndef MINI_STRING_H
#define MINI_STRING_H

#include <stdarg.h>
#include "base/type.h"
#include "base/mem/manager.h"

typedef char *String, **PString;
typedef char  CharArray[];

/*                    Strings dinamicas                       */

String  StringAlloc     ( PMemmgr memmgr,   Size    size );

String  StringDuplicate ( PMemmgr memmgr,   CString str );

String  StringCopy      ( String *dest,     CString orig );

String  StringCopyF     ( String *dest,     CString format, ... );

String  StringConcat    ( String *dest,     CString orig );

String  StringConcatF   ( String *dest,     CString format, ... );

String  StringResize    ( String *dest,     Size newSize );

#define StringSizeOf(STR) ( ( MemSizeof(MemManagerOf(STR), STR) / sizeof(char) ) - 1L )

/*                    Utilidades para Strings                      */

int     StringCompareF          ( CString src,  CString format, ... );

CString StringTruncRevSkip      ( CString str, int sepchar, Counter skipcount );

CString StringTruncRev          ( CString str, int sepchar );

Size    StringCalcFormatSize    ( CString format, ... );

Size    StringVCalcFormatSize   ( CString format, va_list ap ) ;

CString StringTrimm             ( CString str, int c );

CString StringTrimmAll          ( CString str, CharArray all );

CString StringInvert            ( CString str );

/*                    Geracao de Strings                      */

String StringScanf ( PMemmgr memmgr, char* src, char *format, ... );

String StringLoadFile ( PMemmgr memmgr, String *str, FILE *fp );

String StringLoadFileSection ( PMemmgr memmgr, String *str, FILE *fp, int sep );

#endif // MINI_STRING_H
