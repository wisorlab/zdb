#ifndef DICT_H
#define DICT_H

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

/**
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * dynamically sized dictionary of C strings
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
typedef struct Dict
{

	char** keys;
	char** values;
	size_t length;

} Dict;

extern Dict* createDict( size_t );
extern void destroyDict( Dict* );
extern void resizeDict( Dict*, size_t );
extern void dictSetKeyAndValue( Dict*, size_t, char*, char* );

#endif