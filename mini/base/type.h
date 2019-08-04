/*
 * Type.h
 *
 *  Created on: 8 de nov de 2018
 *      Author: danilo
 */

#ifndef MINI_BASE_TYPE_H
#define MINI_BASE_TYPE_H

typedef long int Size;
typedef long int Lenght;
typedef long int Counter;
typedef long int Position;
typedef long int Identifier;

typedef void *Pointer;

typedef char *PChar;

typedef char *CString;

#define LongException -1L
#define BooleanException -1

#ifdef BOOL
    typedef BOOL Bool;
#else
    typedef int Bool;
#endif

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

#ifndef FAIL
    #define FAIL 1
#endif

#ifndef OK
    #define OK 0
#endif

typedef int Status;

#ifndef DEBUG
    #define Private static
#else
    #define Private
#endif

#endif /* MINI_BASE_TYPE_H */
