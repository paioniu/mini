#include "../../pch.h"
#include "meta.h"
#include "manager.h"

/*
	algoritmo, ok
	errorcheck, ok
	paramcheck, ok
	redundantcheckavoid, ok
	doc, TODO documentar
	test, TODO testar
*/
PMeta MemMetaAlloc(PMemmgr manager, Size sizeInBytes, CleaningCB  cleanerFunction) {
    PMeta ret;
/*
	CHECK_PARAM( manager,		1, NULL )
    CHECK_PARAM( sizeInBytes,	2, NULL )
*/
	/* Aloca */
	SHIELD( ret = malloc(sizeof(Meta) + sizeInBytes),
		"Malloc falhou",
		return NULL; )

	/* Inicializa */
    ret->dynamic            = (void *)(ret + 1);
	ret->parent				= NULL;
    ret->manager            = manager;
    ret->sizeInBytes        = sizeInBytes;
    ret->cleanerFunction    = cleanerFunction;
	ret->scope				= manager->scope;

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
PMeta MemMetaRealloc(PMeta meta, Size sizeInBytes) {
    PMeta ret = NULL;
/*
    CHECK_PARAM( meta,			1, NULL )
    CHECK_PARAM( sizeInBytes,	2, NULL )
*/
	/* Realoca */
    SHIELD( ret = realloc(meta, sizeof(Meta) + sizeInBytes),
		"Realloc falhou",
        return NULL; )

	/* Inicializa parametros relativos */
    ret->dynamic        = (void *)(ret + 1);
    ret->sizeInBytes    = sizeInBytes;

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
void MemMetaFree(PMeta meta, Pointer extra) {
/*
    CHECK_PARAM( meta, 1, )
*/
    if(meta->cleanerFunction)
        meta->cleanerFunction(meta->dynamic, extra);
    free(meta);
}

/*
	algoritmo, ok
	errorcheck, ok
	paramcheck, ok
	redundantcheckavoid, ok
	doc, TODO documentar
	test, TODO testar
*/
void MemMetaFreeOnly(PMeta meta) {
/*
    CHECK_PARAM( meta, 1, )
*/
    free(meta);
}
