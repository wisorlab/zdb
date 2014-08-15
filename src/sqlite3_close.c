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
 * In Matlab, this function takes the parameters
 *		:param db - the SQL database
 * 
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
	// the first parameter is a pointer designating the open DB
	mxHandle* handle = convertMatToHandle( prhs[0] );
	sqlite3* db = (sqlite3*)(handle->ptr);

	if ( db ) 
	{
		// free memory and return
		// mexUnlock();
		sqlite3_close(db);
	}
	else mexErrMsgTxt("Error accessing the database.");
}

// for Visual Studio
int main(void)
{
	return 0;
}