%include "arrays_csharp.i"
%include <typemaps.i>

%apply int *OUTPUT { int* }
%apply float *OUTPUT { float* }
%apply float INPUT[] { float* coords }
%apply double INPUT[] { double* color }
%apply int INPUT[] { int* classes }
%apply unsigned char INPUT[] { void const* buffer }

%typemap(imtype, out="IntPtr") char* "string"
%typemap(cstype) void* "IntPtr"
%typemap(imtype) void* "IntPtr"
%typemap(csin) void* "$csinput"

%typemap(csout, excode=SWIGEXCODE) char* {
    IntPtr tempStringPtr = $imcall;$excode
    if (tempStringPtr == IntPtr.Zero) return null;

    string result = Marshal.PtrToStringAnsi(tempStringPtr);
    Core.FreeString(tempStringPtr);
    return result;
}

%typemap(csin, 
pre="    IntPtr tmp_$csinput = IntPtr.Zero;",
post="      $csinput = Marshal.PtrToStringAnsi(tmp_$csinput);\n      Core.FreeString(tmp_$csinput);") char **OUTPUT "out tmp_$csinput"

%typemap(cstype) char **OUTPUT "out string"
%typemap(imtype) char **OUTPUT "out IntPtr"

%apply char **OUTPUT { char ** };
