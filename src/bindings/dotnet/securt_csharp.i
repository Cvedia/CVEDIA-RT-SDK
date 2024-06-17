%feature("autodoc", "3");
%feature("doxygen:ignore:transferfull");
%module(swig_module_name) Securt
%{
/* Includes the header in the wrapper code */
#include "securt/securt_capi.h"
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

%rename("CreateSecurtInstance") securt_create_instance;
%rename("GetSecurtInstance") securt_get_instance;

// Remove the "securt_" prefix and capitalize all sections
%rename("%(regex:/^securt_(.)([^_]*)_?(.)?([^_]*)?_?(.)?([^_]*)?_?(.)?([^_]*)?_?(.)?([^_]*)?/\\U\\1\\L\\2\\U\\3\\L\\4\\U\\5\\L\\6\\U\\7\\L\\8/)s", %$isfunction) "";

/* Parse the header file to generate wrappers */
%include "securt/securt_capi.h"
