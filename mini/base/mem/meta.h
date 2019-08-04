#ifndef MINI_BASE_MEM_META_H
#define MINI_BASE_MEM_META_H

#define GETMETA(PTR) (((PMeta )PTR) - 1)

typedef struct _Memmgr Memmgr, * PMemmgr;

typedef void *Dynamic;

typedef void (*CleaningCB)(Dynamic dyn, Pointer extra);

typedef struct _Meta {
	/* Objeto dinamico. Ponteiro para memoria dinamicamente alocada.
	Seu endereco é relativo ao endereco do objeto Meta */
    Dynamic		dynamic;
    Dynamic		parent;
	/* Gerenciador de memoria no qual o objeto dinamico está registrado */
    PMemmgr		manager;
	/* Tamanho em bytes alocado para o objeto dinamico.
	Será sempre o tamanho solicitado na alocacao */
    Size		sizeInBytes;
	/* Callback para limpeza antes da liberacao de memoria do objeto */
    CleaningCB  cleanerFunction;
	/* Escopo ao qual o objeto pertence */
	Position	scope;
} Meta, *PMeta;

PMeta	MemMetaAlloc(PMemmgr manager, Size sizeInBytes, CleaningCB  cleanerFunction);

PMeta	MemMetaRealloc(PMeta meta, Size sizeInBytes);

void	MemMetaFree(PMeta meta, Pointer extra);

void	MemMetaFreeOnly(PMeta meta);

#endif /* MINI_BASE_MEM_META_H */
