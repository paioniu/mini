/*
 * OnError.h
 *
 *  Created on: 8 de nov de 2018
 *      Author: danilo
 */

#ifndef MINI_ERROR_H
#define MINI_ERROR_H

#include "type.h"

extern int      g_errorlog;
extern int      g_errorstatus;
extern int      g_errorprintstack;
extern CString  g_errorbasedir;

#define DISABLE_ERRORLOG() g_errorlog = 0

#define ENABLE_ERRORLOG() g_errorlog = 1

#define TOGGLE_ERRORLOG() g_errorlog = !g_errorlog

#define DISABLE_ERRORPRINTSTACK() g_errorprintstack = 0

#define ENABLE_ERRORPRINTSTACK() g_errorprintstack = 1

#define TOGGLE_ERRORPRINTSTACK() g_errorprintstack = !g_errorprintstack

#define RESET_ERRORSTATUS() g_errorstatus = 0

#define GET_ERRORSTATUS() g_errorstatus

#define SHIELD( COND, MSG, AFTER ) if( ! ( COND ) ) { if(g_errorprintstack && !g_errorstatus) ErrorPrintStack(  ); \
				if( g_errorlog ) printf("Error: %s\n", MSG); \
				g_errorstatus = 1; \
				AFTER }

#define SHIELD_EXIT( COND, MSG ) if( ! ( COND ) ) { ErrorPrintStack(  ); \
				if( g_errorlog ) printf("Fatal Error: %s\n\nExiting...", MSG); \
				exit(1); }

#define SHIELD_ABORT( COND, MSG ) if( ! ( COND ) ) { ErrorPrintStack(  ); \
				if( g_errorlog ) printf("Umanageable Fatal Error: %s\n\nAborting...", MSG); \
				abort(); }

#define CHECK_PARAM( COND, NUMBER, RET ) if( ! ( COND ) ) { if(g_errorprintstack && !g_errorstatus) ErrorPrintStack(  ); \
				if( g_errorlog ) printf("Parametro %d está errado.\n", NUMBER); \
				g_errorstatus = 1; \
				return RET; }

void    ErrorInit(int argc, char **argv);

void    ErrorPrintStack();

#endif /* MINI_ERROR_H */
