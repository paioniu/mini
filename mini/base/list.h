#ifndef MINI_BASE_LIST_H
#define MINI_BASE_LIST_H

#include "type.h"
#include "mem/manager.h"

typedef struct _node Node, *PNode;
typedef struct _List List, *PList;

struct _node {
    Pointer item;
    Bool	is_dynamic;
    PNode	prev;
    PNode	next;
};

struct _List {
    PNode	start;
    PNode	end;
    Counter count;
};

#define LIST_LOOP(LIST) { \
            Node *_NODE_ = (LIST)->start; \
            while(_NODE_) {

#define LIST_LOOP_END \
                _NODE_ = _NODE_->next; \
            } \
        }

typedef int (*f_node)(Node *, void *);

PList	ListAlloc(PMemmgr manager);

PList	ListAllocWithParent(PMemmgr manager, Dynamic parent);

#define	ListAdd(LIST, ITEM) ListAdd__backend(LIST, ITEM, FALSE)

#define	ListAddDyn(LIST, ITEM) ListAdd__backend(LIST, ITEM, TRUE)

PNode	ListAdd__backend(PList list, Pointer item, Bool is_dynamic);

PNode	ListGet(PList list, Pointer item);

PNode	ListGetAt(PList list, Position pos);

PNode	ListSearch(PList list, int (*comparator)(PNode, Pointer), Pointer data);

Status	ListRem(PList list, Pointer item);

Status	ListRemAll(PList list);

Bool	ListExistsNode(PList list, PNode node);

PNode	ListRemNode(PList list, PNode node);

/**
 * Executa uma acao para todos os nos
 *
 * Se a funcao de acao retornar um valor diferente de zero para algum dos nos,
 * a operacao será interrompida e esse no será retornado
 *
 * @param list      Lista criada com ListAlloc
 * @param action    Funcao de callback para ser executada em todos os nos
 * @param data      Dados a serem passados para o callback em cada chamada
 *
 * @return NULL em caso de sucesso. Se o callback retornar diferente de
 *                  zero, retorna o no correspondente
*/
PNode      ListForeach(PList list, int (*action)(PNode, Pointer), Pointer data);

#endif /* MINI_BASE_LIST_H */
