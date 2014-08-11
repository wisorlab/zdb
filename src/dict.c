#include "dict.h"


/**
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * create and allocate space to a Dict
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
extern Dict* createDict( size_t length )
{
	Dict* d = (Dict*)malloc(sizeof(Dict));;
	d->keys = (char**)malloc(sizeof(char*) * length);
	d->values = (char**)malloc(sizeof(char*) * length);
	d->length = length;

	// make sure it worked!
	assert(d->keys != NULL && d->values != NULL);
	return d;
}

/**
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * free the memory associated with a Dict
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
extern void destroyDict( Dict* d )
{
	for (size_t i = 0; i < d->length; ++i)
	{
		free(d->keys[i]);
		free(d->values[i]);
	}
	free(d->keys);
	free(d->values);
	free(d);
}

/**
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * resize a Dictionary
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
extern void resizeDict( Dict* d, size_t length )
{
	// resize a dictionary
	d->keys = (char**)realloc( d->keys, sizeof(char*) * length);
	d->values = (char**)realloc( d->values, sizeof(char*) * length);
	d->length = length;
}


/**
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * set the key and value at the index 'i'
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
extern void dictSetKeyAndValue( Dict* d, size_t i, char* key, char* value )
{
	// set value
	d->values[i] = (char*)malloc(strlen(value));
	memcpy(d->values[i], value, strlen(value) * sizeof(char));

	// set key
	d->keys[i] = (char*)malloc(strlen(value));
	memcpy(d->keys[i], key, strlen(key) * sizeof(char));
}

