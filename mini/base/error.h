/*
 * OnError.h
 *
 *  Created on: 8 de nov de 2018
 *      Author: danilo
 */

#ifndef MINI_ERROR_H
#define MINI_ERROR_H

#include "type.h"

#ifdef COMPILE_WITH_FULL_SHIELD
	#define SHIELD( COND, MSG, AFTER ) if( ! ( COND ) ) { if(ErrorGetPrintstackStatus() && !ErrorGetStatus()) ErrorPrintStack(  ); \
				if( ErrorGetLogStatus() ) printf("Error: %s\n", MSG); \
				ErrorSetStatus(1); \
				AFTER }

	#define SHIELD_EXIT( COND, MSG ) if( ! ( COND ) ) { ErrorPrintStack(  ); \
				if( ErrorGetLogStatus() ) printf("Fatal Error: %s\n\nExiting...", MSG); \
				exit(1); }

	#define SHIELD_ABORT( COND, MSG ) if( ! ( COND ) ) { ErrorPrintStack(  ); \
				if( ErrorGetLogStatus() ) printf("Umanageable Fatal Error: %s\n\nAborting...", MSG); \
				abort(); }
#elif defined COMPILE_WITH_NO_SHIELD
	#define SHIELD( COND, MSG, AFTER )

	#define SHIELD_EXIT( COND, MSG )

	#define SHIELD_ABORT( COND, MSG )
#else
	#define SHIELD( COND, MSG, AFTER ) if( ! ( COND ) ) { \
				AFTER }

	#define SHIELD_EXIT( COND, MSG ) if( ! ( COND ) ) { \
				exit(1); }

	#define SHIELD_ABORT( COND, MSG ) if( ! ( COND ) ) { \
				abort(); }
#endif

#ifdef COMPILE_WITH_PARAM_CHECKING
	#define CHECK_PARAM( COND, NUMBER, RET ) if( ! ( COND ) ) { if(ErrorGetPrintstackStatus() && !ErrorGetStatus()) ErrorPrintStack(  ); \
				if( ErrorGetLogStatus() ) printf("Parametro %d est� errado.\n", NUMBER); \
				ErrorSetStatus(1); \
				return RET; }
#else
	#define CHECK_PARAM( COND, NUMBER, RET )
#endif
/**
 * @brief Desativa as mensagens de erro
 *
 * @NoThreadSafe
 */
void 	ErrorDisableLog();
/**
 * @brief Ativa as mensagens de erro
 *
 * @NoThreadSafe
 */
void 	ErrorEnableLog();
/**
 * @brief Ativa/Desativa as mensagens de erro
 *
 * Se o flag estiver ativado, será desativado. Caso contrário, será ativado.
 * 
 * @NoThreadSafe
 */
void 	ErrorToggleLog();
/**
 * @brief Retorna o estado do flag das mensagens de erro
 *
 * @NoThreadSafe
 */
int 	ErrorGetLogStatus();
/**
 * @brief Desativa a exibição da pilha de chamadas
 *
 * @NoThreadSafe
 */
void 	ErrorDisablePrintstack();
/**
 * @brief Ativa a exibição da pilha de chamadas
 *
 * @NoThreadSafe
 */
void 	ErrorEnablePrintstack();
/**
 * @brief Ativa/Desativa a exibição da pilha de chamadas
 *
 * Se o flag estiver ativado, será desativado. Caso contrário, será ativado.
 * 
 * @NoThreadSafe
 */
void 	ErrorTogglePrintstack();
/**
 * @brief Reseta o estado da variável de erro
 * 
 * @NoThreadSafe
 */
void 	ErrorResetStatus();
/**
 * @brief Atribui um valor à variável de erro
 * 
 * @NoThreadSafe
 */
int 	ErrorSetStatus(int val);
/**
 * @brief Retorna o valor da variável de erro
 * 
 * @NoThreadSafe
 */
int 	ErrorGetStatus();
/**
 * @brief Retorna o valor do flag de exibição de pilha de funções
 * 
 * @NoThreadSafe
 */
int 	ErrorGetPrintstackStatus();
/**
 * @brief Inicializa o subsistema de erros
 * 
 * @NoThreadSafe
 */
void    ErrorInit(int argc, char **argv);
/**
 * @brief Exibe a pilha de chamadas a partir da função atual
 * 
 * @NoThreadSafe
 */
void    ErrorPrintStack();

#endif /* MINI_ERROR_H */
