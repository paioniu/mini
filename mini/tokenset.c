#include "pch.h"
#include <string.h>
#include "tokenset.h"
#include "string.h"

Status  TokensetAdd( PTokenset tset, CString cstr ) {
    String str;

    CHECK_PARAM(tset, 1, FAIL)
    CHECK_PARAM(cstr, 2, FAIL)

    SHIELD( str = StringDuplicate( MemManagerOf(tset), cstr ),
           "Falha em duplicar string",
           return FAIL; )

    SHIELD( TokensetAddPre(tset, str),
           "Falha em adicionar string",
           return FAIL; )

    return OK;
}

Status  TokensetFromString( PTokenset tset, CString cstr, char sep ) {
    PMemmgr     memmgr;
    String      str;
    String      dtok;
    CString     tok;
    char        ssep[2];

    CHECK_PARAM(tset, 1, FAIL)
    CHECK_PARAM(cstr, 2, FAIL)
    CHECK_PARAM(sep,  3, FAIL)

    memmgr = MemManagerOf(tset);

    SHIELD( str = StringDuplicate(memmgr, cstr),
            "Falha em duplicar string de origem",
            ListRemAll(tset);
            return FAIL; )

    ssep[0] = sep;
    ssep[1] = 0;

    tok = strtok(str, ssep);
    while( tok ) {
        SHIELD( dtok = StringDuplicate(memmgr, tok),
               "Falha em duplicar token",
                ListRemAll(tset);
                return FAIL; )

        SHIELD( TokensetAddPre(tset, dtok),
               "Falha em adicionar duplicata de token",
                MemFreeOnly(memmgr, dtok);
                ListRemAll(tset);
                return FAIL; )
        MemSetParent(memmgr, dtok, tset);
        tok = strtok(NULL, ssep);
    }

    return OK;
}

String  TokensetToString( PTokenset tset, char sep ) {
    String  ret;
    PNode   cur;
    Size    totalSizeOfTokens = 0L;
    char    ssep[2];

    CHECK_PARAM(tset, 1, NULL)
    CHECK_PARAM(tset->count, 1, NULL)
    CHECK_PARAM(sep,  2, NULL)

    cur = tset->start;
    while(cur) {
        totalSizeOfTokens += strlen((CString)cur->item);
        cur = cur->next;
    }

    SHIELD( ret = StringAlloc(MemManagerOf(tset), totalSizeOfTokens + tset->count),
           "Alocacao de string destino",
           return NULL; )

    ssep[0] = sep;
    ssep[1] = 0;
    cur = tset->start;
    while(cur) {
        strcat(ret, (CString)cur->item);
        if(cur->next)
            strcat(ret, ssep);
        cur = cur->next;
    }

    return ret;
}

