%feature("autodoc", "3");
%feature("doxygen:ignore:transferfull");

%{
/* Includes the header in the wrapper code */
#include "core/core_capi.h"
#include "securt/securt_capi.h"
#include "api/solutions_capi.h"
#include "api/system_capi.h"
#define SWIG_FILE_WITH_INIT
#include <numpy/arrayobject.h>
%}

%include "numpy.i"

// Initialize numpy array functionality
%init %{
  import_array();
%}

#define EXPORT

%typemap(in) (float const* coords, int numPoints) {
    if (!PyList_Check($input)) {
        PyErr_SetString(PyExc_TypeError, "Expected a list of [x,y] lists for coords");
        return NULL;
    }
    $2 = PyList_Size($input) * 2;  // Since every inner list has 2 floats
    $1 = (float *) malloc(sizeof(float) * $2);
    if (!$1) {
        PyErr_NoMemory();
        return NULL;
    }
    for (int i = 0; i < PyList_Size($input); i++) {
        PyObject *listObj = PyList_GetItem($input, i);
        if (!PyList_Check(listObj) || PyList_Size(listObj) != 2) {
            free($1);
            PyErr_SetString(PyExc_TypeError, "Each item must be a list of two floats [x,y]");
            return NULL;
        }

        PyObject *xObj = PyList_GetItem(listObj, 0);
        PyObject *yObj = PyList_GetItem(listObj, 1);
        if (!PyFloat_Check(xObj) || !PyFloat_Check(yObj)) {
            free($1);
            PyErr_SetString(PyExc_TypeError, "Inner list items must be floats");
            return NULL;
        }

        $1[2*i] = (float) PyFloat_AsDouble(xObj);      // x-coordinate
        $1[2*i + 1] = (float) PyFloat_AsDouble(yObj);  // y-coordinate
    }
}

%typemap(in) (double const* color) {
    if (!PyList_Check($input) || PyList_Size($input) != 3) {
		PyErr_SetString(PyExc_TypeError, "Expected a list of 3 floats for color");
		return NULL;
	}
	$1 = (double *) malloc(sizeof(double) * 3);
	if (!$1) {
		PyErr_NoMemory();
		return NULL;
	}
	for (int i = 0; i < 3; i++) {
		PyObject *o = PyList_GetItem($input, i);
		if (!PyFloat_Check(o)) {
			free($1);
			PyErr_SetString(PyExc_TypeError, "List items must be floats");
			return NULL;
		}
		$1[i] = PyFloat_AsDouble(o);
	}
}

%typemap(in) (int const* classes, int classesSize) {
    if (!PyList_Check($input)) {
        PyErr_SetString(PyExc_TypeError, "Expected a list of ints for classes");
        return NULL;
    }
    $2 = PyList_Size($input);
    $1 = (int *) malloc(sizeof(int) * $2);
    if (!$1) {
        PyErr_NoMemory();
        return NULL;
    }
    for (int i = 0; i < $2; i++) {
        PyObject *o = PyList_GetItem($input, i);
        if (!PyLong_Check(o)) {
            free($1);
            PyErr_SetString(PyExc_TypeError, "List items must be ints");
            return NULL;
        }
        $1[i] = (int) PyLong_AsLong(o);
    }
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
%include "securt/securt_capi.h"
%include "api/solutions_capi.h"
%include "api/system_capi.h"
