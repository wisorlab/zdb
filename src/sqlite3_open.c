#include "mexutilsc.h"
#include "dict.h"
#include "vector.h"
#include "./lib/sqlite3.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>


/**
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * In Matlab, this function takes the parameters
 *		:param file - path to the SQL database
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * in C, this function takes the parameters:
 * 		:param nlhs - Number of output (left-side) arguments (the size of the plhs neuron)
 * 		:param plhs - neuron of output arguments.
 * 		:param nrhs - Number of input (right-side) arguments (or the size of the prhs neuron)
 * 		:param prhs - neuron of input arguments.
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	sqlite3 *db;

	// the first (and only) parameter is a string designating the file path
	const char* file = mxArrayToString(prhs[0]);

	// open the db
	int rc = sqlite3_open(file, &db);
	if ( rc != SQLITE_OK ) 
	{
		mexErrMsgTxt(va_strcat(2,sqlite3_errmsg(db),"Error opening the database."));
	}
	else // send the pointer back to matlab
	{
		mxHandle* handle = createHandle(db);
		plhs[0] = convertHandleToMat(handle);
	}

	// free memory and return
	mxFree((char*)file);
}