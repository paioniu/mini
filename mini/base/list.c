#include "../pch.h"
#include "list.h"

#ifndef NULL
    #define NULL ((void*)0)
#endif

Private void _NodeCleaner(Dynamic dyn, Pointer extra) {
    PNode node = (PNode)dyn;

    if(node->is_dynamic && node->item)
        MemFreeOnly(MemManagerOf(node->item), node->item);
}

Private PNode _NodeAlloc(Memmgr *manager, PList list, PNode prev, PNode next, void *item) {
    PNode ret;

    SHIELD( ret = MemAllocWithParent(manager, list, sizeof(Node), _NodeCleaner ),
		"Malloc falhou",
        return NULL; )

    ret->prev = prev;
    if(prev)
        prev->next = ret;
    ret->next = next;
    if(next)
        next->prev = ret;

    ret->item = item;

    return ret;
}
/*
static void ListCleaner(Dynamic dyn, Pointer extra) {
    PList list = (PList)dyn;
    PNode cur = list->start;
    PNode tofree;

    while(cur) {
        tofree = cur;
        cur = cur->next;
        MemFree(MemManagerOf(list), tofree, NULL);
    }
}*/

PList	ListAlloc(PMemmgr manager) {
    PList ret;

    CHECK_PARAM( manager, 1, NULL )

    SHIELD( ret = MemAlloc( manager, sizeof(List), NULL ),
		"Alocacao da lista falhou",
		return NULL; )

    ret->count = 0L;
    ret->start = ret->end = NULL;

    return ret;
}

PList	ListAllocWithParent(PMemmgr manager, Dynamic parent) {
	PList ret;

	CHECK_PARAM(manager, 1, NULL)
	CHECK_PARAM(parent, 2, NULL)

	SHIELD(ret = MemAllocWithParent(manager, parent, sizeof(List), NULL),
		"Alocacao da lista falhou",
		return NULL; )

	ret->count = 0L;
	ret->start = ret->end = NULL;

	return ret;
}

PNode	ListAdd__backend(PList list, Pointer item, Bool is_dynamic) {
    PNode node;

    CHECK_PARAM( list, 1, NULL )
    CHECK_PARAM( is_dynamic != BooleanException, 1, NULL )


    SHIELD( node = _NodeAlloc(MemManagerOf(list), list, list->end, NULL, item),
		"Alocacao de novo no falhou",
        return NULL; )

    list->end = node;

    if(!list->start)
        list->start = node;

    list->count++;

    node->is_dynamic = is_dynamic;

    return node;
}

PNode	ListGet(PList list, Pointer item) {
    PNode cur;

	CHECK_PARAM( list, 1, NULL )

    cur = list->start;

    while(cur) {
        if(cur->item == item)
            break;
        cur = cur->next;
    }

	SHIELD( cur,
		"Item nao encontrado",
		return NULL; )

    return cur;
}

PNode	ListGetAt(PList list, Position pos) {
    PNode		cur;
    Position	l_pos;

	CHECK_PARAM( list, 1, NULL )

    cur = list->start;
	l_pos = 0L;

    while(cur) {
        if(l_pos == pos)
            break;
        l_pos++;
        cur = cur->next;
    }

	SHIELD( cur,
		"Item nao encontrado",
		return NULL; )

    return cur;
}

PNode	ListSearch(PList list, int (*comparator)(PNode, Pointer), Pointer data) {
    PNode cur;

	CHECK_PARAM( list, 1, NULL )
	CHECK_PARAM(comparator, 2, NULL)

    cur = list->start;

    while(cur) {
        if(comparator(cur, data))
            break;
        cur = cur->next;
    }

    return cur;
}

Status	ListRem(PList list, Pointer item) {
    PNode cur;

	CHECK_PARAM( list, 1, FAIL )

    cur = ListGet(list, item);
    SHIELD( cur,
		"Tentativa de remover item inexistente",
        return FAIL; )

    ListRemNode(list, cur);

    return OK;
}

Status	ListRemAll(PList list) {
    PNode cur;
    PNode toFree;

	CHECK_PARAM( list, 1, FAIL )

    cur = list->start;
    RESET_ERRORSTATUS();
    while(cur) {
        toFree = cur;
        cur = cur->next;
        MemFree(MemManagerOf(list), toFree, NULL);
        SHIELD( 0 == GET_ERRORSTATUS(),
            "Falha em liberar memoria",
            return FAIL; )
        list->count--;
    }

    list->start = list->end = NULL;
    SHIELD( list->count > 0L,
            "Lista aparentemente tinha mais nós do que o contador registra", return FAIL; )

    return OK;
}

Bool	ListExistsNode(PList list, PNode node) {
    PNode cur;

	CHECK_PARAM( list, 1, BooleanException )
	CHECK_PARAM(node, 2, BooleanException)

    cur = list->start;
    while(cur) {
        if(cur == node)
            return TRUE;
        cur = cur->next;
    }

    return FALSE;
}

PNode	ListRemNode(PList list, PNode node) {
    PNode prev;
    PNode next;

	CHECK_PARAM( list, 1, NULL )
	CHECK_PARAM( node, 2, NULL )

	SHIELD( TRUE == ListExistsNode(list, node),
		"Tentativa de remover no que nao faz parte da lista",
		return NULL; )

	prev = node->prev;
	next = node->next;

	if(node->is_dynamic)
        MemFree(MemManagerOf(node->item), node->item, NULL);

	RESET_ERRORSTATUS();
    MemFree(MemManagerOf(list), node, NULL);
	SHIELD( 0 == GET_ERRORSTATUS(),
		"Falha em liberar memoria",
		return NULL; )

    if(prev)
        prev->next = next;
    if(next)
        next->prev = prev;

    if(list->end == node)
        list->end = prev;
    if(list->start == node)
        list->start = next;

    list->count--;

    return next;
}

PNode      ListForeach(PList list, int (*action)(PNode, Pointer), Pointer data) {
    PNode cur;
    PNode next;

	CHECK_PARAM( list, 1, NULL )
	CHECK_PARAM( action, 2, NULL )

    cur = list->start;
    while(cur) {
		next = cur->next; /* Previne desvios no loop, uma vez que action pode alterar as conexões de cur */
        if(action(cur, data))
            return cur;
        cur = next;
    }

    return NULL;
}
