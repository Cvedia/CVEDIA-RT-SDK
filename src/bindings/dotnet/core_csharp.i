%feature("autodoc", "3");
%feature("doxygen:ignore:transferfull");
%module(swig_module_name) Core
%{
/* Includes the header in the wrapper code */
#include "core/core_capi.h"
%}

%pragma(csharp) moduleimports=%{
using global::System;
using global::System.Runtime.InteropServices;
%}

%pragma(csharp) imclassimports=%{
using global::System;
using global::System.Runtime.InteropServices;
%}

%include "types.i"

// Disable the EXPORT define because SWIG complains about __declspec(dllexport) being used in the header files
#define EXPORT

// Remove the "core_" prefix and capitalize all sections
%rename("%(regex:/^core_(.)([^_]*)_?(.)?([^_]*)?_?(.)?([^_]*)?_?(.)?([^_]*)?_?(.)?([^_]*)?/\\U\\1\\L\\2\\U\\3\\L\\4\\U\\5\\L\\6\\U\\7\\L\\8/)s", %$isfunction) "";

/* Parse the header file to generate wrappers */
%include "core/core_capi.h"