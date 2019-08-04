#include "pch.h"
#include "map.h"
#include "base/mem/manager.h"

Private int mentrycmp(PNode node, Pointer key) {
    PMapEntry entry = (PMapEntry)node->item;

    if(entry->key == key)
        return 1;
    return 0;
}

Pointer MapSet__backend(PMap map, Pointer key, Pointer value, Bool value_is_dyn) {
    PMapEntry entry;
    PNode node;

    CHECK_PARAM( map, 1, NULL )
    CHECK_PARAM( key, 2, NULL )

    node = ListSearch(map, mentrycmp, key);
    if(node) {
        entry = (PMapEntry)node->item;
    }
    else {
        SHIELD( entry = MemAllocWithParent(MemManagerOf(map), map, sizeof(MapEntry), NULL),
                "Alocacao de entrada para mapa",
                return NULL; )

        SHIELD( ListAddDyn(map, entry),
                "Falha em adicionar entrada",
                MemFreeOnly(MemManagerOf(map), entry);
                return NULL; )

        entry->key      = key;
    }
    entry->value            = value;
    entry->value_is_dynamic = value_is_dyn;

    return value;
}

Pointer MapGet(PMap map, Pointer key) {
    PNode node;

    CHECK_PARAM( map, 1, NULL )
    CHECK_PARAM( key, 2, NULL )

    node = ListSearch(map, mentrycmp, key);
    if(node)
        return ((PMapEntry)node->item)->value;

    return NULL;
}

Status  MapUnSet(PMap map, Pointer key) {
    PNode node;

    CHECK_PARAM( map, 1, FAIL )
    CHECK_PARAM( key, 2, FAIL )

    TOGGLE_ERRORLOG();
    ErrorTogglePrintstack();
    node = ListSearch(map, mentrycmp, key);
    if(node) {
        if(((PMapEntry)node->item)->value_is_dynamic)
            MemFree(MemManagerOf(((PMapEntry)node->item)->value), ((PMapEntry)node->item)->value, NULL);
        ListRemNode(map, node);
    }
    TOGGLE_ERRORLOG();
    ErrorTogglePrintstack();
    RESET_ERRORSTATUS();

    return OK;
}
