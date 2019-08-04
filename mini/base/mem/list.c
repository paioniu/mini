#include "../../pch.h"
#include "list.h"

/*
	algoritmo, ok
	errorcheck, ok
	paramcheck, ok
	redundantcheckavoid, ok
	doc, TODO documentar
	test, TODO testar
*/
PMemlist MemlistAdd ( PMemlist list, PMeta meta ) {
    PMemlist ret = NULL;
/*
    CHECK_PARAM( list,	1, NULL )
*/
    CHECK_PARAM( meta,	2, NULL )

    SHIELD( ret = (PMemlist)malloc(sizeof(Memlist)),
		"Malloc falhou",
        return NULL; )

    ret->meta = meta;
    ret->prev = NULL;
    ret->next = list;
    if(list)
        list->prev = ret;

    return ret;
}

/*
	algoritmo, ok
	errorcheck, ok
	paramcheck, ok
	redundantcheckavoid, ok
	doc, TODO documentar
	test, TODO testar
*/
PMemlist MemlistDel(PMemnode node, Pointer extra) {
    PMemlist ret = NULL;
/*
    CHECK_PARAM( node, 1, NULL )
*/

	SHIELD( node->meta,
		"Metadados ausentes. No corrompido.",
		return NULL; )

    ret = node->next;
    if(node->next)
        node->next->prev = node->prev;
    if(node->prev)
        node->prev->next = node->next;

	RESET_ERRORSTATUS();
    MemMetaFree(node->meta, extra);
	SHIELD( GET_ERRORSTATUS() == 0,
		"Falha em liberar metadados",
		return NULL; )
    free(node);

    return ret;
}

/*
	algoritmo, ok
	errorcheck, ok
	paramcheck, ok
	redundantcheckavoid, ok
	doc, TODO documentar
	test, TODO testar
*/
PMemlist MemlistDelOnly(PMemlist node) {
    PMemlist ret;
/*
    CHECK_PARAM( node, 1, NULL )
*/

	SHIELD( node->meta,
		"Metadados ausentes. No corrompido.",
		return NULL; )

    ret = node->next;
    if(node->next)
        node->next->prev = node->prev;
    if(node->prev)
        node->prev->next = node->next;

    MemMetaFreeOnly(node->meta);
    free(node);

    return ret;
}

/*
	algoritmo, ok
	errorcheck, ok
	paramcheck, ok
	redundantcheckavoid, ok
	doc, TODO documentar
	test, TODO testar
*/
PMemlist MemlistGet(PMemlist list, PMeta meta) {
    PMemlist cur;
/*
    CHECK_PARAM( list,	1,	NULL )
    CHECK_PARAM( meta,	2,	NULL )
*/
    cur = list;
    while(cur) {
        if(cur->meta == meta)
            return cur;
        cur = cur->next;
    }

    return NULL;
}
