#include "../../pch.h"
#include "manager.h"
#include "list.h"
#include <string.h>

/*
	algoritmo, ok
	errorcheck, ok
	paramcheck, ok
	redundantcheckavoid, ok
	doc, TODO documentar
	test, TODO testar
*/
PMemmgr	MemInit(PMemmgr manager) {
    if( !manager ) {
        SHIELD( manager = malloc(sizeof(Memmgr)),
			"Malloc falhou",
            return NULL; )
        manager->allocated = TRUE;
    } else {
         manager->allocated = FALSE;
    }

    manager->pool		= NULL;
    manager->counter	= 0L;
	manager->scope		= 0L;

    return manager;
}

/*
	algoritmo, ok
	errorcheck, ok
	paramcheck, ok
	redundantcheckavoid, ok
	doc, TODO documentar
	test, TODO testar
*/
void	MemFinish(PMemmgr manager) {
    CHECK_PARAM( manager, 1, )

    if(manager->pool) {
        PMemlist cursor = (PMemlist)manager->pool;
        while( cursor )
			cursor = MemlistDel(cursor, NULL);
        manager->pool = NULL;
    }
    if(manager->allocated)
        free(manager);
    else {
		manager->pool		= NULL;
		manager->counter	= 0L;
		manager->scope		= 0L;
    }
}

/*
	algoritmo, ok
	errorcheck, ok
	paramcheck, ok
	redundantcheckavoid, ok
	doc, TODO documentar
	test, TODO testar
*/
Dynamic	MemAlloc(PMemmgr manager, Size size, CleaningCB cleaning) {
    PMeta     meta;
    PMemnode  node;

    CHECK_PARAM( manager,	1, NULL )
    CHECK_PARAM( manager->pool,	1, NULL )
    CHECK_PARAM( size,	2, NULL )

    SHIELD( node = MemlistAdd ( (PMemlist)manager->pool, ( meta = MemMetaAlloc( manager, size, cleaning ) ) ),
		"Falha em registrar o novo container de metadados",
        if(meta) MemMetaFree(meta, NULL);
        return NULL; )

    manager->pool = (Pointer)node;
    manager->counter++;

    return meta->dynamic;
}

Dynamic	MemAllocZero(PMemmgr manager, Size size, CleaningCB cleaning) {
    Dynamic ret;

    SHIELD( ret = MemAlloc( manager, size, cleaning),
           "Alocacao de objeto dinamico",
           return NULL; )

    memset( ret, 0, size );

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
Dynamic	MemAllocWithParent(PMemmgr manager, Dynamic parent, Size size, CleaningCB cleaning) {
	Dynamic ret;

    CHECK_PARAM( manager,	1, NULL )
    CHECK_PARAM( parent,    2, NULL )
    CHECK_PARAM( size,	    3, NULL )

	SHIELD( MemExists( manager, parent ),
		"O objeto pai deve estar registrado no mesmo manager",
		return NULL; )

	SHIELD( ret = MemAlloc( manager, size, cleaning ),
		"Falha de alocacao de objeto dinamico",
		return NULL; )

	GETMETA(ret)->parent = parent;

	return ret;
}

Dynamic	MemAllocZeroWithParent(PMemmgr manager, Dynamic parent, Size size, CleaningCB cleaning) {
    Dynamic ret;

    SHIELD( ret = MemAllocWithParent( manager, parent, size, cleaning),
           "Alocacao de objeto dinamico",
           return NULL; )

    memset( ret, 0, size );

    return ret;
}



Status	MemSetParent(PMemmgr manager, Dynamic dyn, Dynamic parent) {
    PMemnode node;

    CHECK_PARAM( manager,	1, FAIL )
    CHECK_PARAM( dyn,       2, FAIL )
    CHECK_PARAM( parent,    3, FAIL )

	SHIELD(node = MemlistGet(manager->pool, GETMETA(dyn)),
		"Tentativa de modificar objeto nao registrado",
		return FAIL; )

    node->meta->parent = parent;
    return OK;
}

Status	MemSetParentForAll(PMemmgr manager, Dynamic parent) {
    PMemnode cur;

    CHECK_PARAM( manager,	1, FAIL )
    CHECK_PARAM( parent,    2, FAIL )

    cur = (PMemnode)manager->pool;
    while( cur ) {
        cur->meta->parent = parent;
        cur = cur->next;
    }

    return OK;
}

/*
	algoritmo, ok
	errorcheck, ok
	paramcheck, ok
	redundantcheckavoid, ok
	doc, TODO documentar
	test, TODO testar
*/
Dynamic	MemRealloc(PMemmgr manager, Dynamic dyn, Size size) {
    PMeta meta;
    PMeta meta_reallocated;

    CHECK_PARAM( manager,	1, NULL )
    CHECK_PARAM( dyn,	2, NULL )
    CHECK_PARAM( size,	3, NULL )

    SHIELD( meta_reallocated = MemMetaRealloc( ( meta = GETMETA(dyn) ), size ),
		"Falha em realocar memoria",
		return NULL; )

    meta = meta_reallocated;

    return meta->dynamic;
}

/*
	algoritmo, ok
	errorcheck, ok
	paramcheck, ok
	redundantcheckavoid, ok
	doc, TODO documentar
	test, TODO testar
*/
Bool	MemExists(PMemmgr manager, Dynamic dyn) {
    CHECK_PARAM( manager,	1, BooleanException )
    CHECK_PARAM( manager->pool,	1, BooleanException )
    CHECK_PARAM( dyn,	2, BooleanException )

    if( MemlistGet( manager->pool, GETMETA(dyn) ) )
        return TRUE;

    return FALSE;
}

/*
	algoritmo, ok
	errorcheck, ok
	paramcheck, ok
	redundantcheckavoid, ok
	doc, TODO documentar
	test, TODO testar
*/
Size	MemSizeof(PMemmgr manager, Dynamic dyn) {
    PMeta meta;

    CHECK_PARAM( manager,	1, LongException )
    CHECK_PARAM( manager->pool,	1, LongException )
    CHECK_PARAM( dyn,	2, LongException )

    if( MemlistGet( manager->pool, ( meta = GETMETA(dyn) ) ))
        return meta->sizeInBytes;

    return 0L;
}


Dynamic	MemFree_backend(PMemmgr manager, PMemnode node, Dynamic parent, Bool doExecuteCallback, Pointer extra) {
	if (parent) {
		PMemnode cur = manager->pool;

		while (cur) {
			if (cur->meta->parent == parent) {
				MemFree_backend(manager, cur, cur->meta->dynamic, doExecuteCallback, extra);
			}
			cur = cur->next;
		}
	}

	if (node == (PMemnode)manager->pool)
		manager->pool = node->next;

	if (TRUE == doExecuteCallback) {
		MemlistDel(node, extra);
	} else {
		MemlistDelOnly(node);
	}

	manager->counter--;

	return NULL;
}

/*
	algoritmo, ok
	errorcheck, ok
	paramcheck, ok
	redundantcheckavoid, ok
	doc, TODO documentar
	test, TODO testar
*/
Dynamic	MemFreeOnly(PMemmgr manager, Dynamic dyn) {
    PMemnode node;

	CHECK_PARAM(manager, 1, NULL)
	CHECK_PARAM(manager->pool, 1, NULL)
	CHECK_PARAM(dyn, 2, NULL)

	SHIELD(node = MemlistGet(manager->pool, GETMETA(dyn)),
		"Tentativa de liberar memoria nao registrada",
		return NULL; )

    return MemFree_backend(manager, node, dyn, FALSE, NULL);
}

/*
	algoritmo, ok
	errorcheck, ok
	paramcheck, ok
	redundantcheckavoid, ok
	doc, TODO documentar
	test, TODO testar
*/
Dynamic	MemFree(PMemmgr manager, Dynamic dyn, Pointer extra) {
    PMemnode node;

	CHECK_PARAM(manager, 1, NULL)
	CHECK_PARAM(manager->pool, 1, NULL)
	CHECK_PARAM(dyn, 2, NULL)

	SHIELD(node = MemlistGet(manager->pool, GETMETA(dyn)),
		"Tentativa de liberar memoria nao registrada",
		return NULL; )

    return MemFree_backend(manager, node, dyn, TRUE, extra);
}

/*
	algoritmo, ok
	errorcheck, ok
	paramcheck, ok
	redundantcheckavoid, ok
	doc, TODO documentar
	test, TODO testar
*/
PMemmgr	MemManagerOf(Dynamic dyn) {
    CHECK_PARAM( dyn,	1, NULL )
    return GETMETA(dyn)->manager;
}

/*
	algoritmo, ok
	errorcheck, ok
	paramcheck, ok
	redundantcheckavoid, ok
	doc, TODO documentar
	test, TODO testar
*/
Status	MemSetCleaner(PMemmgr manager, Dynamic dyn, CleaningCB cleaning) {
    PMeta		meta;
    PMemnode	node;

	CHECK_PARAM(manager, 1, FAIL)
	CHECK_PARAM(manager->pool, 1, FAIL)
    CHECK_PARAM( dyn,	2, FAIL )

	meta = GETMETA(dyn);

    SHIELD( node = MemlistGet( manager->pool, meta ),
		"Tentativa de modificar metadados para memoria nao registrada",
        return FAIL; )

    meta->cleanerFunction = cleaning;

    return OK;
}

Status MemMergeManagers(PMemmgr dest, PMemmgr src)
{
	PMemnode srccur;
	PMemnode srclast = NULL;

	CHECK_PARAM(dest, 1, FAIL)
	CHECK_PARAM(dest->pool, 1, FAIL)
	CHECK_PARAM(src, 2, FAIL)
	CHECK_PARAM(src->pool, 2, FAIL)

	srccur = (PMemnode)src->pool;

	while (srccur) {
		srclast = srccur;
		srccur->meta->scope = dest->scope;
		srccur = srccur->next;
	}

	srclast->next = (PMemnode)dest->pool;
	dest->pool = src->pool;
	src->pool = NULL;
	src->counter = 0L;
	src->scope = 0L;

	return OK;
}

/*
	algoritmo, ok
	errorcheck, ok
	paramcheck, ok
	redundantcheckavoid, ok
	doc, TODO documentar
	test, TODO testar
*/
void	MemFreeScope(PMemmgr manager, Pointer extra) {
	PMemnode node;
	PMemnode toFree;

    CHECK_PARAM( manager,	1, )

	node = (PMemnode)manager->pool;
	while( node ) {
		toFree = node;
		node = node->next;
		SHIELD_EXIT(toFree->meta,
			"Metadados ausentes. No corrompido. Lista de objetos dinamicos comprometida" )
		if( toFree->meta->scope >= manager->scope ) {
			MemFree_backend(manager, toFree, toFree->meta->dynamic, TRUE, extra);
		}
	}
	--manager->scope;
}

void MemSurviveScope(PMemmgr manager, Dynamic dyn)
{
	CHECK_PARAM(manager, 1, )
	CHECK_PARAM(dyn, 2, )

	SHIELD(MemlistGet(manager->pool, GETMETA(dyn)),
		"Tentativa de modificar metadados para memoria nao registrada",
		return; )

	if (GETMETA(dyn)->scope > 0L)
		--GETMETA(dyn)->scope;
}

void MemSetToGlobalScope(PMemmgr manager, Dynamic dyn)
{
	CHECK_PARAM(manager, 1, )
	CHECK_PARAM(dyn, 2, )

	SHIELD(MemlistGet(manager->pool, GETMETA(dyn)),
		"Tentativa de modificar metadados para memoria nao registrada",
		return; )

	if (GETMETA(dyn)->scope > 0L)
		GETMETA(dyn)->scope = 0L;
}
