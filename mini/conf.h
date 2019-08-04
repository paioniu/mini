#ifndef MZ_CONF_H
#define MZ_CONF_H

#include "base/type.h"
#include "file.h"
#include "base/mem/manager.h"
#include "base/list.h"
#include "string.h"
#include "dict.h"
#include "tokenset.h"

typedef List Conf;
typedef PList PConf;

#define ConfAlloc(MANAGER) ListAlloc(MANAGER)

Status	ConfLoad(PConf confs, CString filename);

Status  ConfFromString(PConf confs, CString cstr);

Status	ConfSave(PConf confs, CString filename);

Status 	ConfSet(PConf confs, CString conf, CString value);

#define ConfGet(CNFMAP, KEY) DictGet(CNFMAP, KEY)

#define ConfUnset(CNFMAP, KEY) DictUnset(CNFMAP, KEY)

#endif /* MZ_CONF_H */
