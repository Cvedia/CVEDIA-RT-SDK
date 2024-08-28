%feature("autodoc", "3");
%feature("doxygen:ignore:transferfull");

%{
/* Includes the header in the wrapper code */
#include "core/core_capi.h"
#define SWIG_FILE_WITH_INIT
#include <numpy/arrayobject.h>
%}

%include "numpy.i"

// Initialize numpy array functionality
%init %{
  import_array();
%}

#define EXPORT

// typemap for numpy array -> C buffer
%typemap(in) (void const* buffer, int const width, int const height) {
    if (!PyArray_Check($input) || !PyArray_ISCONTIGUOUS($input)) {
        PyErr_SetString(PyExc_TypeError, "Requires a contiguous numpy array");
        return NULL;
    }
    
    $2 = PyArray_DIMS($input)[1];  // width (assuming array shape is height x width)
    $3 = PyArray_DIMS($input)[0];  // height
    $1 = PyArray_DATA($input);
}

%typemap(in, numinputs=0) char **outJson {
  // Allocate memory for the pointer only
  $1 = (char **)malloc(sizeof(char*));
  *$1 = 0; // Initialize to null, C function will allocate memory
}

%typemap(argout) char **outJson {
  $result = Py_BuildValue("s", *$1);
  free(*$1); // Free the string allocated in C
}

%typemap(freearg) (float const* coords, int numPoints) {
    free($1);
}

%typemap(freearg) (int const* classes, int classesSize) {
    free($1);
}

%typemap(freearg) (double const* color) {
    free($1);
}

%typemap(freearg) char **outJson {
  free($1);
}

// Typemap for converting char * to Python string and then freeing it
%typemap(out) char* {
    if ($1) {
        $result = PyUnicode_FromString($1);  // Convert char * to Python string
        core_free_string($1);                // Free the original C string
    } else {
        $result = Py_None;
        Py_INCREF(Py_None);                  // If NULL, return None in Python
    }
}

/* Parse the header file to generate wrappers */
%include "core/core_capi.h"