%feature("autodoc", "3");
%feature("doxygen:ignore:transferfull");
%module(swig_module_name) System
%{
/* Includes the header in the wrapper code */
#include "api/system_capi.h"
%}

%include "types.i"

// Disable the EXPORT define because SWIG complains about __declspec(dllexport) being used in the header files
#define EXPORT

// Remove the "system_" prefix and capitalize all sections
%rename("%(regex:/^system_(.)([^_]*)_?(.)?([^_]*)?_?(.)?([^_]*)?_?(.)?([^_]*)?_?(.)?([^_]*)?/\\U\\1\\L\\2\\U\\3\\L\\4\\U\\5\\L\\6\\U\\7\\L\\8/)s", %$isfunction) "";

/* Parse the header file to generate wrappers */
%include "api/system_capi.h"
