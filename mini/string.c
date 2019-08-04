#include "pch.h"
#include "string.h"
#include "file.h"

String StringAlloc(PMemmgr memmgr, Size size) {
    String string_ret;

    CHECK_PARAM( memmgr, 1, NULL )
    CHECK_PARAM( size, 2, NULL )

    SHIELD( string_ret = MemAlloc(memmgr, sizeof(char) * (++size), NULL),
           "Alocacao de memoria para string",
           return NULL; )

    return string_ret;
}

String StringDuplicate(PMemmgr memmgr, CString str) {
    String string_ret;

    CHECK_PARAM( memmgr, 1, NULL )
    CHECK_PARAM( str, 2, NULL )

    SHIELD( string_ret = StringAlloc( memmgr, strlen(str) ),
           "Alocacao de memoria para string",
           return NULL; )

    strcpy(string_ret, str);

    return string_ret;
}

String StringCopy(String *dest, CString orig) {
    Lenght origlen;

    CHECK_PARAM( dest, 1, NULL )
    CHECK_PARAM( *dest, 1, NULL )
    CHECK_PARAM( orig, 2, NULL )
    CHECK_PARAM( *orig, 2, NULL )

    origlen = strlen(orig);
    if(origlen > (long int)StringSizeOf(*dest)) {
        SHIELD( StringResize(dest, origlen),
               "Falha em redimensionar a string",
               return NULL; )
    }

    strcpy(*dest, orig);

    return *dest;
}

String StringCopyF(String *dest, CString format, ...){
    va_list ap;
    Size    neededSize;

    CHECK_PARAM( dest, 1, NULL )
    CHECK_PARAM( *dest, 1, NULL )
    CHECK_PARAM( format, 2, NULL )
    CHECK_PARAM( *format, 2, NULL )

    /* Calcula tamanho necessário */
    va_start ( ap, format );
    neededSize = StringVCalcFormatSize ( format, ap );
    va_end ( ap );

    /* Redimensiona se necessário */
    if ( neededSize > (long int)StringSizeOf ( *dest ) ) {
		SHIELD( StringResize(dest, neededSize),
            "Falha em redimensionar a string",
            return NULL; )
    }

    /* Imprime com formatacao no objeto String */
    va_start ( ap, format );
    vsprintf ( *dest, format, ap );
    va_end ( ap );

    return *dest;
}

String StringConcat(String *dest, CString orig) {
    Size neededSize;

    CHECK_PARAM( dest, 1, NULL )
    CHECK_PARAM( *dest, 1, NULL )
    CHECK_PARAM( orig, 2, NULL )
    CHECK_PARAM( *orig, 2, NULL )

    neededSize = strlen(*dest) + strlen(orig);

    if(neededSize > (long int)StringSizeOf(*dest)) {
		SHIELD( StringResize(dest, neededSize),
            "Falha em redimensionar a string",
            return NULL; )
    }

    strcat(*dest, orig);

    return *dest;
}

String StringResize(String *dest, Size newSize) {
    String string_ret;

    CHECK_PARAM( dest, 1, NULL )
    CHECK_PARAM( *dest, 1, NULL )
    CHECK_PARAM( newSize, 2, NULL )

    SHIELD( string_ret = MemRealloc(MemManagerOf(*dest), *dest, sizeof(char) * (newSize+1)),
           "Realocacao de memoria falhou",
           return NULL; )

    string_ret[newSize] = 0;

    *dest = string_ret;

    return string_ret;
}

String StringConcatF( String *dest,  CString format, ...) {
    va_list ap;
    Size    neededSize;
    Size    destLen;

    CHECK_PARAM( dest, 1, NULL )
    CHECK_PARAM( *dest, 1, NULL )
    CHECK_PARAM( format, 2, NULL )
    CHECK_PARAM( *format, 2, NULL )

    destLen = strlen(*dest);

    /* Calcula o tamanho necessario */
    va_start(ap, format);
    neededSize = destLen + StringVCalcFormatSize(format, ap);
    va_end(ap);

    /* Redimensiona se preciso */
    if(neededSize > (long int)StringSizeOf(*dest)) {
		SHIELD( StringResize(dest, neededSize),
            "Falha em redimensionar a string",
            return NULL; )
    }

    /* Concatena com formato */
    {
        char *copyTo = *dest;

        va_start(ap, format);
        vsprintf(&copyTo[destLen], format, ap);
        va_end(ap);
    }

    return *dest;
}

/*                    Utilidades para Strings                      */

CString StringTruncRev(CString str, int c) {
    return StringTruncRevSkip(str, c, 0);
}

CString StringTruncRevSkip( CString str, int sepchar, Counter skipcount ) {
    register unsigned int _INDEX_;

    CHECK_PARAM( str, 1, NULL )
    CHECK_PARAM( *str, 1, NULL )
    CHECK_PARAM( sepchar, 2, NULL )

    for( _INDEX_ = strlen(str) - 1; _INDEX_ > 0; --_INDEX_ ) {
        if( str[_INDEX_]==(char)sepchar ) {
            /* Se é para saltá-lo esta vez... */
            if( skipcount > 0 ) {
                --skipcount; /* Atualiza o contador de saltos */
            } else { /* .. senao é para saltar */
                /* Retorna a substring que inicia no
                 * caractere seguinte */
                return &str[_INDEX_+1];
            }
        }
    }

    return str;
}

Size StringCalcFormatSize(char *format, ...) {
    Size    size;
    va_list ap;

    CHECK_PARAM( format, 1, 0L )
    CHECK_PARAM( *format, 1, 0L )

    va_start(ap, format);

    size = StringVCalcFormatSize(format, ap);

    va_end(ap);

    return size;
}

CString StringTrimm(CString str, int c) {
    CString cur;
    CString dest;
    Size    times;
    register Position _INDEX_;

    CHECK_PARAM( str,   1, NULL )
    CHECK_PARAM( *str,  1, NULL )
    CHECK_PARAM( c,     2, NULL )

    cur = str;
    while(*cur && *cur==(char)c)
        cur++;

    if(!*cur) {
        *str=0;
        return str;
    }
    dest = str;
    while(*cur) {
        *dest = *cur;
        dest++;
        cur++;
    }
    *dest = *cur;
    times = strlen(str);
    cur = str;
    for( _INDEX_ = times - 1; _INDEX_ > 0; --_INDEX_ ) {
        if( cur[ _INDEX_ ] != (char)c ) {
            cur[ _INDEX_ + 1 ]=0;
            break;
        }
    }

    return str;
}

CString StringTrimmAll(CString str, CharArray all) {
    CString cur;
    CString dest;
    Size    times;
    register Position _INDEX_;

    CHECK_PARAM( str,   1, NULL )
    CHECK_PARAM( *str,  1, NULL )
    CHECK_PARAM( all,   2, NULL )
    CHECK_PARAM( *all,  2, NULL )

    cur = str;
    while(*cur && strchr(all, *cur)) {
        cur++;
    }
    if(!*cur) {
        *str=0;
        return str;
    }
    dest = str;
    while(*cur) {
        *dest = *cur;
        dest++;
        cur++;
    }
    *dest = *cur;
    times = strlen(str);
    cur = str;
    for( _INDEX_ = times - 1; _INDEX_ > 0; --_INDEX_ ) {
        if(!strchr(all, cur[_INDEX_])) {
            cur[_INDEX_+1]=0;
            break;
        }
    }

    return str;
}

int StringCompareF( CString src,  CString format, ...) {
    char diff;
    union {
        char c;
        char *s;
        int i;
        unsigned int u;
    } u;
    va_list ap;
    int res;
    char number[50];

    CHECK_PARAM( src,       1, -2 )
    CHECK_PARAM( *src,      1, -2 )
    CHECK_PARAM( format,    2, -2 )
    CHECK_PARAM( *format,   2, -2 )

    res = 0;
    va_start(ap, format);

    for(; *format; ++format, ++src)
    {
        if(*format=='%')
        {
            /* Processa par?metro conforme o fomato */
            ++format;
            switch(*format) {
            case 'c': {
                    u.c = va_arg(ap, int);
                    diff = *src - u.c;
                    if(diff) {
                        if(diff>0) {
                            res = 1;
                            goto retpoint;
                        } else {
                            res = -1;
                            goto retpoint;
                        }
                    }
                }
                break;
            case 's': {
                    Size len;
                    u.s = va_arg(ap, char*);
                    len = strlen(u.s);
                    if((long int)strlen(src)>=len) {
                        res = strncmp(src, u.s, len);
                        if(res) {
                            goto retpoint;
                        }
                        src+=len-1;
                    } else {
                        res = -1;
                        goto retpoint;
                    }
                }
                break;
            case 'd': {
                    Size len;
                    u.i = va_arg(ap, int);
                    sprintf(number, "%d", u.i);
                    len = strlen(number);
                    if((long int)strlen(src)>=len) {
                        res = strncmp(src, number, len);
                        if(res) {
                            goto retpoint;
                        }
                        src+=len-1;
                    } else {
                        res = -1;
                        goto retpoint;
                    }
                }
                break;
            case 'u': {
                    Size len;
                    u.u = va_arg(ap, unsigned int);
                    sprintf(number, "%u", u.u);
                    len = strlen(number);
                    if((long int)strlen(src)>=len) {
                        res = strncmp(src, number, len);
                        if(res) {
                            goto retpoint;
                        }
                        src+=len-1;
                    } else {
                        res = -1;
                        goto retpoint;
                    }
                }
                break;
            case 'x': {
                    Size len;
                    u.u = va_arg(ap, unsigned int);
                    sprintf(number, "%x", u.u);
                    len = strlen(number);
                    if((long int)strlen(src)>=len) {
                        res = strncmp(src, number, len);
                        if(res) {
                            goto retpoint;
                        }
                        src+=len-1;
                    } else {
                        res = -1;
                        goto retpoint;
                    }
                }
                break;
            case 'X': {
                    Size len;
                    u.u = va_arg(ap, unsigned int);
                    sprintf(number, "%X", u.u);
                    len = strlen(number);
                    if((long int)strlen(src)>=len) {
                        res = strncmp(src, number, len);
                        if(res) {
                            goto retpoint;
                        }
                        src+=len-1;
                    } else {
                        res = -1;
                        goto retpoint;
                    }
                }
                break;
            case '%':
                goto blackhole;
                break;
            default:
                break;
            }
        }
        else
        {
blackhole:
            diff = *src - *format;
            if(diff) {
                if(diff>0) {
                    res = 1;
                    goto retpoint;
                } else {
                    res = -1;
                    goto retpoint;
                }
            }
        }
    }

    retpoint:
    va_end(ap);

    return res;
}

Size StringVCalcFormatSize( CString format, va_list ap) {
    Size size;

    CHECK_PARAM( format,       1, 0L )
    CHECK_PARAM( *format,      1, 0L )

    size = 0;

    for(; *format; ++format)
    {
        if(*format=='%')
        {
            /* Processa par?metro conforme o fomato */
            ++format;
            if(*format=='%')
                goto blackhole;
            switch(*format)
            {
            case 'c':
                va_arg(ap, int);
                goto blackhole;
                break;
            case 's':
            {
                size += strlen(va_arg(ap, char*));
                break;
            }
            case 'd':
                va_arg(ap, int);
                size += 15;
                break;
            case 'f':
                va_arg(ap, double);
                size += 35;
                break;
            case 'u':
            case 'x':
            case 'X':
                va_arg(ap, unsigned int);
                size += 15;
                break;
            default:
                break;
            }
        }
        else
        {
            /* Adiciona o caractere */
blackhole:
            size++;
        }
    }

    return size;
}

CString     StringInvert( CString str ) {
	int     len;
	int     half;
	CString start;
	CString end;
	register Position _INDEX_;

    CHECK_PARAM( str,       1, NULL )
    CHECK_PARAM( *str,      1, NULL )

    len     = strlen(str);
    half    = len / 2;
    start   = str;
    end     = &str[len-1];
	for(_INDEX_ = 0L; _INDEX_ < half; _INDEX_++, ++start, --end) {
		char t;
		t = *end;
		*end = *start;
		*start = t;
	}

	return str;
}

/*                    Geracao de Strings                      */

/* Auxilia a reversao em caso de alocacões parciais */
Private void StringScanf_aux_Clean ( va_list ap, Counter  howmanyvars ) {
    PString     pstr;
    register    Position _INDEX_;

    for ( _INDEX_ = 0L; _INDEX_ < howmanyvars; _INDEX_++ ) {
        pstr = va_arg ( ap, PString );
        pstr = NULL;
    }
}

/* "name = Oulard"  "%=%" */
String StringScanf ( PMemmgr memmgr, CString src, CString format, ... ) {
    String  firstStr;
    CString cur;
    Counter howmanyvars;
    va_list ap;
    Memmgr local_manager;

    CHECK_PARAM( memmgr,    1, NULL )
    CHECK_PARAM( src,       2, NULL )
    CHECK_PARAM( *src,      2, NULL )
    CHECK_PARAM( format,    3, NULL )
    CHECK_PARAM( *format,   3, NULL )

    cur         = src;
    howmanyvars = 0;
    firstStr    = NULL;
    MemInit(&local_manager);

    va_start ( ap, format );

    while ( *format ) {
        if ( *format == '%' ) {
            PString newStr;
            CString end;
            Counter count;

            newStr = va_arg ( ap, PString );
            format++;
            if ( *format == '\0' ) {

                SHIELD( *newStr = StringDuplicate(&local_manager, cur),
                        "Duplicacao de substring",
                        va_end ( ap );
                        va_start ( ap, format );
                        StringScanf_aux_Clean ( ap, howmanyvars );
                        va_end ( ap );
                        MemFinish(&local_manager);
                        return NULL; )
                if(!firstStr)
                    firstStr = *newStr;
                howmanyvars++;
                break;
            }

            SHIELD( end = strchr ( cur, *format ),
                    "Formato nao corresponde",
                    va_end ( ap );
                    va_start ( ap, format );
                    StringScanf_aux_Clean ( ap, howmanyvars );
                    va_end ( ap );
                    MemFinish(&local_manager);
                    return NULL; )

            count = ( end - cur ) / sizeof ( char );

            SHIELD( *newStr = StringAlloc(&local_manager, count),
                    "Alocacao para duplicacao de substring",
                    va_end ( ap );
                    va_start ( ap, format );
                    StringScanf_aux_Clean ( ap, howmanyvars );
                    va_end ( ap );
                    MemFinish(&local_manager);
                    return NULL; )
            if(!firstStr)
                firstStr = *newStr;

            strncpy ( *newStr, cur, count );
            cur = end + 1;
            howmanyvars++;
        }
        else if ( *format == '*' ) {
            CString end;

            format++;
            if ( *format == '\0' ) {
                break;
            }
            SHIELD( end = strchr ( cur, *format ),
                    "Formato nao corresponde",
                    va_end ( ap );
                    va_start ( ap, format );
                    StringScanf_aux_Clean ( ap, howmanyvars );
                    va_end ( ap );
                    MemFinish(&local_manager);
                    return NULL; )
            cur = end + 1;
        }
        else {
            CString pos;

            pos = strchr ( cur, *format );
            if ( !pos )
                break;
            cur = pos+1;
        }
        format++;
    }

    va_end ( ap );

    MemMergeManagers( memmgr, &local_manager );

    return firstStr;
}

String StringLoadFile ( PMemmgr memmgr, PString pstr, FILE *fp ) {
    Size            fsize;
    unsigned int    fposbackup;

    CHECK_PARAM( memmgr,    1, NULL )
    CHECK_PARAM( pstr,      2, NULL )
    CHECK_PARAM( fp,        3, NULL )

    fsize      = FileTailSizeOf ( fp );
    fposbackup = ftell ( fp );

    SHIELD( *pstr = StringAlloc( memmgr, fsize ),
           "Falha em criar objeto conteiner String",
           return NULL; )

    SHIELD( fsize == (long int)fread ( *pstr, sizeof ( char ), fsize, fp ),
            "Falha em ler o arquivo",
            MemFreeOnly(memmgr, *pstr );
            fseek ( fp, fposbackup, SEEK_SET );
            return NULL; )

    ( *pstr ) [fsize] = '\0';

    return *pstr;
}

String StringLoadFileSection ( PMemmgr memmgr, PString pstr, FILE *fp, int charsep ) {
    Size            fsize;
    unsigned int    fposbackup;

    CHECK_PARAM( memmgr,    1, NULL )

    fsize = FileSectionSizeOf ( fp, charsep );

    SHIELD( *pstr = StringAlloc( memmgr, fsize ),
           "Falha em criar objeto conteiner String",
           return NULL; )

    fposbackup = ftell ( fp );
    SHIELD( fsize == (long int)fread ( *pstr, sizeof ( char ), fsize, fp ),
        "Falha em ler o arquivo",
        *pstr = MemFreeOnly(memmgr, *pstr );
        fseek ( fp, fposbackup, SEEK_SET );
        return NULL; )

    ( *pstr ) [fsize] = '\0';

    return *pstr;
}
