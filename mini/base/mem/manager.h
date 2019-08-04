#ifndef MINI_BASE_MEM_MANAGER_H
#define MINI_BASE_MEM_MANAGER_H

#include "../type.h"
#include "meta.h"

/**
 * manager.c
 *
 * Funcões para gerenciamento de memoria dinamicamente alocada
 * Danilo Almeida, paioniu@gmail.com, 30 de junho de 2019
 *
 * Copyright 2019 Danilo Almeida. All Rights Reserved.
 * See LICENCE file
*/

struct _Memmgr {
    Pointer		pool;
    Counter		counter;
    Bool		allocated;
    Position	scope;
};

/**
 * Inicializa uma variável Memmgr
 *
 * @param manager Variável a ser inicializada. Se NULL, será alocada com malloc
 *
 * @return Ponteiro para a variável;
*/
PMemmgr	MemInit(PMemmgr manager);

/**
 * Finaliza uma variável do tipo Memmgr
 *
 * Libera toda a memoria alocada por MemAlloc usando essa memoria
 *
 * @param manager Variável a ser finalizada. Se alocada com malloc, será
 *          liberada com free
 */
void	MemFinish(PMemmgr manager);

/**
 * Aloca memoria
 *
 * @param manager       Memoria inicializada com MemInit
 * @param size      Tamanho em bytes a ser alocado
 * @param cleaning  Funcao a ser usada para limpeza pré destruicao
 *
 * @return Um ponteiro para a memoria alocada
 */
Dynamic	MemAlloc(PMemmgr manager, Size size, CleaningCB cleaning);

Dynamic	MemAllocZero(PMemmgr manager, Size size, CleaningCB cleaning);

Dynamic	MemAllocWithParent(PMemmgr manager, Dynamic parent, Size size, CleaningCB cleaning);

Dynamic	MemAllocZeroWithParent(PMemmgr manager, Dynamic parent, Size size, CleaningCB cleaning);

Status	MemSetParent(PMemmgr manager, Dynamic dyn, Dynamic parent);

Status	MemSetParentForAll(PMemmgr manager, Dynamic parent);

/**
 * Realoca memoria
 *
 * @param manager       Memoria inicializada com MemInit
 * @param size      Tamanho em bytes a ser alocado
 * @param ptr       Endereco da memoria previamente alocada ou realocada
 *
 * @return Um ponteiro para a memoria realocada. Em caso de falha, o endereco original é retornado
 */
Dynamic	MemRealloc(PMemmgr manager, Dynamic dyn, Size size);

/**
 * Informa se o endereco dado é de memoria alocada e registrada no manager dado
 *
 * @param manager       Memoria inicializada com MemInit
 * @param ptr       Endereco da memoria previamente alocada ou realocada
 *
 * @return TRUE ou FALSE
 */
Bool	MemExists(PMemmgr manager, Dynamic dyn);

/**
 * Informa o tamanho em bytes do objeto alocado
 *
 * @param manager       Memoria inicializada com MemInit
 * @param ptr       Endereco da memoria previamente alocada ou realocada
 *
 * @return O tamanho em bytes
 */
Size	MemSizeof(PMemmgr manager, Dynamic dyn);

/**
 * Libera memoria
 *
 * @param manager       Memoria inicializada com MemInit
 * @param ptr       Item alocado com MemAlloc
 * @param extra     Dados a serem passados para a rotina de limpeza
 *                  informada como terceiro parametro de MemAlloc
 *
 * @return NULL sempre
 */
Dynamic	MemFree(PMemmgr manager, Dynamic dyn, Pointer extra);

#define MemRecycle(MANAGER, DYN, EXTRA) MemFree(MANAGER, DYN, EXTRA)

Dynamic	MemFreeOnly(PMemmgr manager, Dynamic dyn);

#define MemRecycleOnly(MANAGER, DYN) MemFreeOnly(MANAGER, DYN)
/**
 * Retorna o gerenciador de memoria de um objeto
 *
 * @param ptr Um objeto alocado previamente com MemAlloc
 * @return O gerenciador de memoria
 * @warning Nao há checagem para saber se o ponteiro é um objeto válido.
 *          Uma chamada subsequente a MemExists usando o gerenciador
 *          retornado pode servir de verificacao
 *
*/
PMemmgr	MemManagerOf(Dynamic dyn);

Status	MemSetCleaner(PMemmgr manager, Dynamic dyn, CleaningCB cleaning);

/**
 * Unifica dois managers
 *
 * Na operacao, todos os elementos de src sao inseridos logo antes do início da lista de
 * dest. A ordem dos itens de src é mantida. O escopo de cada objeto dinamico vindo de src
 * é alterado para o escopo atual de dest. A lista de src é esvaziada e seu escopo é
 * alterado para 0L.
 *
 * @param dest Manager de destino. Receberá os objetos
 * @param src Manager de origem. Cederá os objetos
 * @return FAIL ou OK
 */
Status	MemMergeManagers(PMemmgr dest, PMemmgr src);

/* Operacões de Escopo */
#define MemEnterScope(MEM) ((PMemmgr)MEM)->scope++

#define MemLeaveScope(MEM, EXTRA) if(((PMemmgr)MEM)->scope > 0L) MemFreeScope((PMemmgr)MEM, NULL)

#define MemLeaveScopeWithExtra(MEM, EXTRA) if(((PMemmgr)MEM)->scope > 0L) MemFreeScope((PMemmgr)MEM, (Pointer)EXTRA)

void	MemFreeScope(PMemmgr manager, Pointer extra);

void	MemSurviveScope(PMemmgr manager, Dynamic dyn);

void	MemSetToGlobalScope(PMemmgr manager, Dynamic dyn);

#endif /* MINI_BASE_MEM_MANAGER_H */
