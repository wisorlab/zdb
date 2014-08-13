/**
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Some mex utilites for C
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/

#ifndef MEXUTILSC_H
#define MEXUTILSC_H

#ifndef MATLAB_MEX_FILE
	#define MATLAB_MEX_FILE
#endif

#ifdef _MSC_VER
	#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdlib.h>
#include <stdint.h>

// for some reason char16_t fails to be defined in clang
// TODO test for osx here
#ifndef char16_t
	typedef uint16_t char16_t;
#endif
#include "mex.h"
#include <string.h>
#include <stdarg.h>

// verify the integrity of the pointer by checking its signature on recreation
#define CLASS_HANDLE_SIGNATURE 0xFF00F0A5

/**
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Handle allows persistent integrity of a C pointer between multiple functin calls
 * by cast a C pointer into a Matlab mxArray
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
typedef struct mxHandle
{
	void* ptr;
	uint32_t signature;

} mxHandle;

/**
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * create a Handle object from a C pointer
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
mxHandle* createHandle ( void* ptr )
{
	mxHandle* handle = (mxHandle*)malloc(sizeof(mxHandle));
	handle->ptr = ptr;
	handle->signature = CLASS_HANDLE_SIGNATURE;
	return handle;
}

/**
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * convert between a Handle object and an mxArray
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
static __inline mxArray* convertHandleToMat( mxHandle* handle )
{
	mxArray *mx = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	*((uintptr_t*)mxGetData(mx)) = (uintptr_t)(handle->ptr);
	return mx;
}

/**
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * convert between a Matlab mxArray and a Handle object
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
static __inline mxHandle* convertMatToHandle( const mxArray *mx )
{
    if (mxGetNumberOfElements(mx) != 1 || mxGetClassID(mx) != mxUINT64_CLASS || mxIsComplex(mx))
        mexErrMsgTxt("Input must be a real uint64 scalar.");
	struct mxHandle* handle = createHandle((void*)*((uintptr_t *)mxGetData(mx)));
    if (handle->signature != CLASS_HANDLE_SIGNATURE)
        mexErrMsgTxt("Handle not valid.");
    return handle;
}


/**
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* copy a chunk of memory of a specified size from the value pointed to by
* an mxArray into the memory addressed by 'out'
* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/
static __inline void memcpyToMxArray( const mxArray* mx, void* out, int size )
{
	memcpy( out, (void*)mxGetData(mx), (int)size );
}



/**
 * TODO this doesn't really belong with the MEX funtions
 *
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * concat any number of strings using variadic argument lists
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
static __inline char* va_strcat( const size_t argc, ... )
{
	va_list args;
	va_start(args, argc);
	va_list args2;
	va_copy(args2,args);

	size_t size = 1;
	char* format = malloc(argc * 2 * sizeof(char));

	// calculate the length of the output buffer by summing all the strings
	for (size_t i = 0, j = 0; i < argc; ++i, j = i * 2)
	{
		size += strlen(va_arg(args, char*));
		format[j] = '%'; format[j + 1] = 's';
	}

	// allocate a buffer of size
	char* buf = (char*)malloc(sizeof(char)* size);
	// vsnprintf_s(buf, size, size, format, args2);
	vsnprintf(buf,size,format,args2);
	va_end(args);


	return buf;
}

#endif