#ifndef MINI_MAP_H
#define MINI_MAP_H

#include "base/list.h"

typedef List Map;
typedef PList PMap;

typedef struct _MapEntry MapEntry, *PMapEntry;

struct _MapEntry {
    Pointer key;
    Pointer value;
    Bool    value_is_dynamic;
};

#define MapAlloc(MANAGER) ListAlloc(MANAGER)

#define MapSet(MAP, KEY, VALUE) MapSet__backend(MAP, KEY, VALUE, FALSE)

#define MapSetDyn(MAP, KEY, VALUE) MapSet__backend(MAP, KEY, VALUE, TRUE)

Pointer MapSet__backend(PMap map, Pointer key, Pointer value, Bool value_is_dyn);

Pointer MapGet(PMap map, Pointer key);

Status  MapUnSet(PMap map, Pointer key);

#endif // MINI_MAP_H
