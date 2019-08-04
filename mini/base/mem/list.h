#ifndef MINI_BASE_MEM_LIST_H
#define MINI_BASE_MEM_LIST_H

#include "../../pch.h"
#include "meta.h"

typedef struct _Memlist Memlist, Memnode, *PMemlist, *PMemnode;

struct _Memlist {
	PMeta			meta;
	PMemnode prev;
	PMemnode next;
};

PMemlist MemlistAdd (PMemlist list, PMeta meta);

PMemlist MemlistDel(PMemnode node, Pointer extra);

PMemlist MemlistDelOnly(PMemlist node);

PMemlist MemlistGet(PMemlist list, PMeta meta);

#endif /* MINI_BASE_MEM_LIST_H */
