#ifndef MINI_BASE_MEM_META_H
#define MINI_BASE_MEM_META_H

/**
 * Operações com metadados de objetos dinâmicos
 * 
 * Danilo Almeida
 */

#define GETMETA(PTR) (((PMeta )PTR) - 1)

typedef struct _Memmgr Memmgr, * PMemmgr;

typedef void *Dynamic;

typedef void (*CleaningCB)(Dynamic dyn, Pointer extra);

typedef struct _Meta {
	/** Objeto dinamico. Ponteiro para memoria dinamicamente alocada.
	Seu endereco e relativo ao endereco do objeto Meta */
    Dynamic		dynamic;
    Dynamic		parent;
	/** Gerenciador de memoria no qual o objeto dinamico esta registrado */
    PMemmgr		manager;
	/** Tamanho em bytes alocado para o objeto dinamico.
	Sera sempre o tamanho solicitado na alocacao */
    Size		sizeInBytes;
	/** Callback para limpeza antes da liberacao de memoria do objeto */
    CleaningCB  cleanerFunction;
	/** Escopo ao qual o objeto pertence */
	Position	scope;
} Meta, *PMeta;

/**
 * Aloca espaço em memória para os metadados e o objeto dinâmico
 * 
 * Aloca um objeto de metadados que contém um objeto dinâmico e seus
 * dados de gerenciamento de memória
 * 
 * @param manager 			Gerente de memória
 * @param sizeInBytes 		Tamanho em bytes que o objeto deverá ter.
 * @param cleanerFunction	Função de limpeza. Será chamada quando for 
 * 							solicitada a liberação de memória do objeto
 * 
 * @return O objeto de metadados alocado
 * 
 * Obs.:
 * 	NÃO checa parâmetros. 
 * 	sizeInBytes não reflete o tamanho total alocado 
 * 		pois não inclui o espaço ocupado pelos metadados
 */
PMeta	MemMetaAlloc(PMemmgr manager, Size sizeInBytes, CleaningCB  cleanerFunction);

PMeta	MemMetaRealloc(PMeta meta, Size sizeInBytes);

void	MemMetaFree(PMeta meta, Pointer extra);

void	MemMetaFreeOnly(PMeta meta);

#endif /* MINI_BASE_MEM_META_H */
