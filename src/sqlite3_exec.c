
#include "mexutils.h"
#include "dict.h"
#include "vector.h"
#include "./lib/sqlite3.h"
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

	mexPrintf((char*)vec);


	// create a dictionary to fit the SQL query results
	// Dict* d = createDict(argc);

	// for(int i = 0; i < argc; ++i)
	// 	dictSetKeyAndValue( (Dict*)d, i, (char*)azColName[i], (char*)(argv[i] ? argv[i] : "NULL"));

	// pushToVector((Vector*)vec,d);

	return 0;
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
	// the first parameter is a pointer to the open db
	mxHandle* handle = convertMatToHandle( prhs[0] );
	sqlite3* db = (sqlite3*)(handle->ptr);

	// the second parameter is the SQL query
	const char* sql = mxArrayToString(prhs[1]);

	// other constants
	char *zErrMsg = 0;
	int rc; // return code

	// open the db
	if ( db )
	{
		// vector of dictionaries
		// Vector* vec = createVector( 0, DICT );
		char* test = "hello world";

		// mexPrintf(sql);


		// Execute SQL statement
		rc = sqlite3_exec(db, sql, callback, (void*)test, &zErrMsg);
		// if ( rc != SQLITE_OK ) mexErrMsgTxt(va_strcat(2,"SQL Error: ", zErrMsg ? zErrMsg : "NULL" ));

		// // assign outputs to a cell array
		// plhs[0] = mxCreateCellMatrix(1,vec->size);
		// for (size_t i = 0; i < vec->size; ++i)
		// 	mxSetCell(plhs[0],i,dictToMxArray((Dict*)getFromVector(vec,i)));
	}
	else mexErrMsgTxt("Error accessing the database.");

	// free memory, close the db and return
	sqlite3_free(zErrMsg);
	mxFree((char*)sql);
}

// for Visual Studio
int main(void)
{
	return 0;
}
