#include <stdlib.h>

// for some reason char16_t fails to be defined in clang
// TODO test for osx here
#ifndef char16_t
	typedef uint16_t char16_t;
#endif

#include "mex.h"
#include "dict.h"
#include "vector.h"
#include <sqlite3.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>

/**
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * convert a Dict into a Matlab mxArray
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
mxArray* dictToMxArray( Dict* d )
{
	mxArray* mx = mxCreateStructMatrix( 1, 1, d->length, (const char**)d->keys );

	for(size_t i = 0; i < d->length; ++i)
	{
		const char *keyvalues[] = { d->values[i] };
		mxArray* value = mxCreateCharMatrixFromStrings( 1, keyvalues );	
		mxSetField( mx, 0, d->keys[i], value );
	}

	return mx;
}


/**
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * SQL callback
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
static int callback(void* vec, int argc, char** argv, char** azColName)
{
	// create a dictionary to fit the SQL query results
	Dict* d = createDict(argc);

	for(size_t i = 0; i < argc; ++i)
		dictSetKeyAndValue( (Dict*)d, i, (char*)azColName[i], (char*)(argv[i] ? argv[i] : "NULL"));

	pushToVector((Vector*)vec,d);

	return 0;
}


/**
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * concat any number of strings using variadic argument lists
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
static char* va_strcat( size_t argc, ... )
{
	va_list args;
	va_start(args, argc);
	va_list args2;
	va_copy(args2,args);

	size_t size = 1;
	char format[argc*2];

	// calculate the length of the output buffer by summing all the strings
	for (size_t i = 0, j = 0; i < argc; ++i, j=i*2) 
	{
		size += strlen(va_arg( args, char*));
		format[j] = '%'; format[j+1] = 's';
	}

	// allocate a buffer of size
	char* buf = (char*)malloc( sizeof(char) * size );
	vsnprintf(buf,size,format,args2);
	va_end(args);
	return buf;
}


/**
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * in C, this function takes the parameters:
 * 		:param nlhs - Number of output (left-side) arguments (the size of the plhs neuron)
 * 		:param plhs - neuron of output arguments.
 * 		:param nrhs - Number of input (right-side) arguments (or the size of the prhs neuron)
 * 		:param prhs - neuron of input arguments.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void mexFunction ( int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc; // return code

	// the first parameter is a string designating the file path
	// the second parameter is the SQL query
	const char* file = mxArrayToString(prhs[0]);
	const char* sql = mxArrayToString(prhs[1]);

	// open the db
	rc = sqlite3_open(file, &db);
	if ( rc ) 
	{
		mexErrMsgTxt("Error opening the database.");
	}
	else
	{
		// vector of dictionaries
		Vector* vec = createVector( 0, DICT );

		// Execute SQL statement
		rc = sqlite3_exec(db, sql, callback, (void*)vec, &zErrMsg);
		if ( rc != SQLITE_OK ) mexErrMsgTxt(va_strcat(2,"SQL Error: ", zErrMsg ));

		// assign outputs to a cell array
		plhs[0] = mxCreateCellMatrix(1,vec->size);
		for (size_t i = 0; i < vec->size; ++i)
			mxSetCell(plhs[0],i,dictToMxArray((Dict*)getFromVector(vec,i)));
	}


	// free memory, close the db and return
	sqlite3_free(zErrMsg);
	mxFree((char*)file);
	mxFree((char*)sql);
	sqlite3_close(db);
}