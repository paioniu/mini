#include "pch.h"
#include "dict.h"
#include "base/mem/manager.h"

Private int dentrycmp(PNode node, Pointer key) {
    PDictEntry entry = (PDictEntry)node->item;

    if(0==strcmp(entry->key, key))
        return 1;
    return 0;
}

Private PDictEntry DictEntryAlloc(PMemmgr memmgr, CString key, Pointer value, Bool value_is_dynamic) {
    PDictEntry entry;
    String lkey;

    SHIELD( lkey = StringDuplicate(memmgr, key),
            "Falha em adicionar entrada",
            return NULL; )

    SHIELD( entry = MemAlloc(memmgr, sizeof(DictEntry), NULL),
            "Alocacao de entrada para mapa",
            MemFreeOnly(memmgr, lkey);
            return NULL; )

    MemSetParent(memmgr, lkey, entry);

    entry->key = lkey;
    if( entry->value_is_dynamic )
        MemFree( MemManagerOf( entry->value ), entry->value, NULL );
    entry->value = value;
    entry->value_is_dynamic = value_is_dynamic;

    return entry;
}

Pointer DictSet__backend(PDict dict, CString key, Pointer value, Bool value_is_dyn) {
    PDictEntry entry;
    PNode node;

    CHECK_PARAM( dict, 1, NULL )
    CHECK_PARAM( key, 2, NULL )
    CHECK_PARAM( value_is_dyn != BooleanException, 4, NULL )

    node = ListSearch(dict, dentrycmp, key);
    if(node) {
        entry = (PDictEntry)node->item;
        if( entry->value_is_dynamic )
            MemFree( MemManagerOf( entry->value ), entry->value, NULL );
        entry->value = value;
        entry->value_is_dynamic = value_is_dyn;
    }
    else {
        entry = DictEntryAlloc(MemManagerOf(dict), key, value, value_is_dyn);
        MemSetParent(MemManagerOf(dict), entry, dict);

        SHIELD( ListAddDyn(dict, entry),
                "Falha em adicionar entrada",
                MemFree(MemManagerOf(dict), entry, NULL);
                return NULL; )
    }

    return value;
}

Pointer DictGet (PDict dict, CString key) {
    PNode node;

    CHECK_PARAM( dict, 1, NULL )
    CHECK_PARAM( key, 2, NULL )

    node = ListSearch(dict, dentrycmp, key);
    if(node)
        return ((PDictEntry)node->item)->value;

    return NULL;
}

Status  DictUnset (PDict dict, Pointer key) {
    PNode node;

    CHECK_PARAM( dict, 1, FAIL )
    CHECK_PARAM( key, 2, FAIL )

    TOGGLE_ERRORLOG();
    TOGGLE_ERRORPRINTSTACK();
    node = ListSearch(dict, dentrycmp, key);
    if(node) {
        if(((PDictEntry)node->item)->value_is_dynamic)
            MemFree(MemManagerOf(((PDictEntry)node->item)->value), ((PDictEntry)node->item)->value, NULL);
        ListRemNode(dict, node);
    }
    TOGGLE_ERRORLOG();
    TOGGLE_ERRORPRINTSTACK();
    RESET_ERRORSTATUS();

    return OK;
}
