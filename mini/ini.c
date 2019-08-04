#include "pch.h"
#include "ini.h"
#include "conf.h"

Status  IniLoad( PMemmgr memmgr, PPIni ini, CString filename ) {
	Status      ret             = OK;
	PMemmgr 	lmemmgr;
	PIni        lini            = NULL;
	FILE        *fileStream     = NULL;
	String      fileContent     = NULL;
	PTokenset	tokenSet        = NULL;
	PNode		_NODE_;
	String      sectionName     = NULL;
	String      sectionContent  = NULL;
    PConf       confs           = NULL;

	CHECK_PARAM( memmgr, 1, FAIL )
	CHECK_PARAM( ini, 2, FAIL )
	CHECK_PARAM( filename, 3, FAIL )

	SHIELD( lmemmgr = MemInit( NULL ),
			"Falha em alocar um manager", return FAIL; )

	SHIELD( lmemmgr = MemInit( NULL ),
			"Falha em alocar um manager", return FAIL; )

	SHIELD( fileStream = fopen(filename, "r+"),
			"Falha em abrir o arquivo para leitura", ret = FAIL; goto cleaning_IniLoad; )

	SHIELD( lini = IniAlloc( lmemmgr ),
			"Falha em alocar objeto Ini", ret = FAIL; goto cleaning_IniLoad; )

	SHIELD( StringLoadFile( lmemmgr, &fileContent, fileStream),
			"Falha em carregar o conteúdo do arquivo", ret = FAIL; goto cleaning_IniLoad; )
	SHIELD(  tokenSet = TokensetAlloc( lmemmgr ),
			"Falha em gerar tokenset o conteúdo do arquivo", ret = FAIL; goto cleaning_IniLoad; )
	SHIELD(  TokensetFromString( tokenSet, fileContent, '['),
			"Falha em gerar tokenset com o conteúdo do arquivo", ret = FAIL; goto cleaning_IniLoad; )

    /* Neste ponto, cada token é uma seção do arquivo ini */
	_NODE_ = tokenSet->start;
    if( !strchr((CString)_NODE_->item , ']' ) )
    {
        /* SECAO DEFAULT */
        SHIELD( confs = ConfAlloc(lmemmgr),
                "Falha em alocar objeto Conf", ret = FAIL; goto cleaning_IniLoad; )
        SHIELD( OK == ConfFromString( confs, (CString)_NODE_->item ),
               "Falha em extrair configuracoes do token", ret = FAIL; goto cleaning_IniLoad; )
        SHIELD( DictSetDyn(lini, "default", confs),
               "Falha em adicionar a secao default", ret = FAIL; goto cleaning_IniLoad; )
        _NODE_ = _NODE_->next;
    }
	while(_NODE_)
	{
        /* DEMAIS SECOES */
	    confs = NULL;
        SHIELD( StringScanf( lmemmgr, (CString)_NODE_->item, "%]%", &sectionName, &sectionContent ),
                "Falha em extrair secao do token", ret = FAIL; goto cleaning_IniLoad; )
        SHIELD( confs = ConfAlloc(lmemmgr),
                "Falha em alocar objeto Conf", ret = FAIL; goto cleaning_IniLoad; )
        SHIELD( OK == ConfFromString( confs, sectionContent ),
               "Falha em extrair configuracoes do token", ret = FAIL; goto cleaning_IniLoad; )
        SHIELD( DictSetDyn(lini, sectionName, confs),
               "Falha em adicionar uma secao", ret = FAIL; goto cleaning_IniLoad; )
        sectionContent = MemRecycleOnly( lmemmgr, sectionContent );
        sectionName = MemRecycleOnly( lmemmgr, sectionName );
        _NODE_ = _NODE_->next;
	}

	cleaning_IniLoad:
        if(tokenSet)
            MemRecycleOnly( lmemmgr, tokenSet );
        if(fileContent)
            MemRecycleOnly( lmemmgr, fileContent );
        if(fileStream)
            fclose( fileStream );

    if( ret == OK ) {
        MemSetParentForAll( lmemmgr, lini );
        MemMergeManagers( memmgr, lmemmgr );
        *ini = lini;
    }
    MemFinish( lmemmgr );

	return ret;
}

Status  IniSave( PIni ini, CString filename ) {
    return OK;
}

Status  IniSet( PIni ini, CString section, CString key, CString value ) {
    return OK;
}

CString IniGet( PIni ini, CString section, CString key ) {
    return NULL;
}

Status  IniUnset( PIni ini, CString section, CString key ) {
    return OK;
}
