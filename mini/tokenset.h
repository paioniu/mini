#ifndef MINI_TOKENSET_H
#define MINI_TOKENSET_H

#include "string.h"
#include "base/list.h"

typedef List Tokenset;
typedef PList PTokenset;

#define TokensetAlloc(PMANAGER) ListAlloc(PMANAGER)

Status  TokensetAdd( PTokenset tset, CString cstr );

#define TokensetAddPre( TSET, STR ) ListAddDyn(TSET, STR)

#define TokensetRem( TSET, STR ) ListRem(TSET, STR)

Status  TokensetFromString( PTokenset tset, CString cstr, char sep );

String  TokensetToString( PTokenset tset, char sep );

#endif // MINI_TOKENSET_H
