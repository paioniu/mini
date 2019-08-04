#include "pch.h"
#include "conf.h"

#include <string.h>

#include "file.h"
#include "base/mem/manager.h"
#include "base/list.h"
#include "string.h"
#include "dict.h"
#include "tokenset.h"

Status ConfLoad(PConf confs, CString filename) {
	FILE        *fileStream;
	String      fileContent = NULL;
	PMemmgr 	memmgr;

	CHECK_PARAM( confs, 1, FAIL )
	CHECK_PARAM( filename, 2, FAIL )

	memmgr = MemManagerOf(confs);

	SHIELD( fileStream = fopen(filename, "r+"),
			"Falha em abrir o arquivo para leitura",
			return FAIL; )

	SHIELD( StringLoadFile( memmgr, &fileContent, fileStream),
			"Falha em carregar o conteúdo do arquivo",
			fclose( fileStream );
			return FAIL; )

	SHIELD(  OK == ConfFromString( confs, fileContent ),
			"Falha em gerar tokenset o conteúdo do arquivo",
			MemRecycleOnly( memmgr, fileContent );
			fclose( fileStream );
			return FAIL; )

	MemRecycleOnly( memmgr, fileContent );
	fclose( fileStream );

	return OK;
}

Status ConfFromString(PConf confs, CString cstr) {
	String      key         = NULL,
                val         = NULL;
	PTokenset	tokenSet    = NULL;
	PMemmgr 	memmgr;
	PNode		_NODE_;

	CHECK_PARAM( confs, 1, FAIL )
	CHECK_PARAM( cstr, 2, FAIL )

	memmgr = MemManagerOf(confs);

	SHIELD(  tokenSet = TokensetAlloc( memmgr ),
			"Falha em gerar tokenset o conteúdo do arquivo",
			return FAIL; )

	SHIELD(  TokensetFromString( tokenSet, cstr, '\n'),
			"Falha em gerar tokenset o conteúdo do arquivo",
			MemRecycleOnly( memmgr, tokenSet );
			return FAIL; )

	_NODE_ = tokenSet->start;
	while(_NODE_) {
		SHIELD( StringScanf( memmgr, (String)_NODE_->item, "%=%", &key, &val),
				"Falha em extrair chave e valor a partir do token",
				MemRecycleOnly( memmgr, tokenSet );
				return FAIL; )

		SHIELD( OK==ConfSet( confs, key, val ),
				"Falha em adicionar configuracao",
				MemRecycleOnly( memmgr, val);
				MemRecycleOnly( memmgr, key);
				MemRecycleOnly( memmgr, tokenSet );
				return FAIL; )

		MemRecycleOnly( memmgr, key);
		_NODE_ = _NODE_->next;
	}
	MemRecycleOnly( memmgr, tokenSet );

	return OK;
}

Status   ConfSave(PConf confs, CString filename) {
	FILE	*fileStream  = NULL;
	String 	tempFilename;
	int 	res;
	PNode	_NODE_;

	CHECK_PARAM( confs, 1, FAIL )
	CHECK_PARAM( filename, 2, FAIL )

	SHIELD( tempFilename = StringAlloc(MemManagerOf(confs), strlen(filename) + 5),
			"Gerando nome de arquivo temporario",
			return FAIL; )

	sprintf(tempFilename, "~%s", filename);

	SHIELD( fileStream = fopen(tempFilename, "w"),
			"Abrindo arquivo temporario para gravacao",
			MemRecycleOnly( MemManagerOf( confs ), tempFilename );
			return FAIL; )

	_NODE_ = confs->start;
	while( _NODE_ ) {
		res = fprintf( fileStream,
				 "%s=%s\n",
				 ((PDictEntry)_NODE_->item)->key,
				 (CString)((PDictEntry)_NODE_->item)->value);
		SHIELD( res > -1,
				"Falha em gravar entrada no arquivo",
				fclose(fileStream);
				remove(tempFilename);
				MemRecycleOnly(MemManagerOf( confs ), tempFilename);
				return FAIL; )
		_NODE_ = _NODE_->next;
	}

	fclose(fileStream);
	res = rename(tempFilename, filename);
	SHIELD( res==0,
			"Falha em renomear arquivo temporario",
			remove(tempFilename);
			MemRecycleOnly(MemManagerOf( confs ), tempFilename);
			return FAIL; )

	MemRecycleOnly(MemManagerOf( confs ), tempFilename);

	return OK;
}

Status ConfSet(PConf confs, CString conf, CString value) {
    String      lvalue;

	CHECK_PARAM( confs, 1, FAIL )
	CHECK_PARAM( conf,  2, FAIL )
	CHECK_PARAM( value, 3, FAIL )

    SHIELD( DictSetDyn( confs, conf, lvalue = StringDuplicate( MemManagerOf( confs ), value ) ),
           "Adicao de valor entrada ao dicionario",
           if( lvalue ) MemFreeOnly( MemManagerOf( confs ), lvalue );
           return FAIL; )

	return OK;
}
