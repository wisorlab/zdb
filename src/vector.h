#ifndef VECTOR_H
#define VECTOR_H

#include "dict.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>

typedef enum 
{
	INT16 = sizeof(int16_t),
	INT32 = sizeof(int32_t),
	INT64 = sizeof(int64_t),
	UINT16 = sizeof(uint16_t),
	UINT32 = sizeof(uint32_t),
	UINT64 = sizeof(uint64_t),
	DOUBLE = sizeof(double),
	FLOAT = sizeof(float),
	LONGDOUBLE = sizeof(long double),
	DICT = sizeof(Dict),

} typesize_t;

#define VECTOR_INDEX(n,i) ((void *) (n)->elems + ((n)->elem_size * (i)))

/**
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * A basic data structure for a dynamically typed and 
 * dynamically sized vector
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
typedef struct
{
	void* elems;
	size_t size;
	typesize_t elem_size;

} Vector;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// functions that act on vectors; methods, if you will
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern Vector* createVector( size_t, typesize_t );
extern void destroyVector( Vector* );
extern void* getFromVector( Vector*, size_t );
extern void setToVector( Vector*, size_t, void* );
extern void resizeVector( Vector*, size_t );
extern void initVectorFromVarargs( Vector*, size_t, ... );
extern void pushToVector ( Vector*, void* );

#endif