#ifndef MINI_INI_H
#define MINI_INI_H

#include "base/list.h"
#include "dict.h"
#include "base/type.h"

typedef Dict Ini;
typedef PDict PIni;
typedef PIni *PPIni;

#define IniAlloc(MANAGER) ListAlloc( MANAGER )

Status  IniLoad( PMemmgr memmgr, PPIni ini, CString filename );

Status  IniSave( PIni ini, CString filename );

Status  IniSet( PIni ini, CString section, CString key, CString value );

CString IniGet( PIni ini, CString section, CString key );

Status  IniUnset( PIni ini, CString section, CString key );

#endif // MINI_INI_H
