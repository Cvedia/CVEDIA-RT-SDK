%feature("autodoc", "3");
%feature("doxygen:ignore:transferfull");

%{
/* Includes the header in the wrapper code */
#include "api/rt_capi.h"
#include "api/config_capi.h"
#include "core/core_capi.h"
%}

%include "numpy.i"

// Disable the EXPORT define because SWIG complains about __declspec(dllexport) being used in the header files
#define EXPORT

// typemap for numpy array -> C buffer
%typemap(in) (void const* buffer, int width, int height) {
    if (!PyArray_Check($input) || !PyArray_ISCONTIGUOUS($input)) {
        PyErr_SetString(PyExc_TypeError, "Requires a contiguous numpy array");
        return NULL;
    }
    
    $2 = PyArray_DIMS($input)[1];  // width (assuming array shape is height x width)
    $3 = PyArray_DIMS($input)[0];  // height
    $1 = PyArray_DATA($input);
}

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

%typemap(freearg) (float const* coords, int numPoints) {
    free($1);
}

%typemap(freearg) (int const* classes, int classesSize) {
    free($1);
}

// Typemap for converting char * to Python string and then freeing it
%typemap(out) char* {
    if ($1) {
        $result = PyUnicode_FromString($1);  // Convert char * to Python string
        core_free_string($1);              // Free the original C string
    } else {
        $result = Py_None;
        Py_INCREF(Py_None);                  // If NULL, return None in Python
    }
}

/* Parse the header file to generate wrappers */
%include "api/rt_capi.h"
%include "api/config_capi.h"
