#ifndef MINI_DICT_H
#define MINI_DICT_H

#include "base/list.h"
#include "string.h"

typedef List Dict;
typedef PList PDict;

typedef struct _DictEntry DictEntry, *PDictEntry;

struct _DictEntry {
    String  key;
    Pointer value;
    Bool    value_is_dynamic;
};

#define DictAlloc(MANAGER) ListAlloc(MANAGER)

#define DictSet(DICT, KEY, VALUE) DictSet__backend(DICT, KEY, VALUE, FALSE)

#define DictSetDyn(DICT, KEY, VALUE) DictSet__backend(DICT, KEY, VALUE, TRUE)

Pointer DictSet__backend(PDict dict, CString key, Pointer value, Bool value_is_dyn);

Pointer DictGet         (PDict dict, CString key);

Status  DictUnset       (PDict dict, Pointer key);

#endif // MINI_DICT_H
