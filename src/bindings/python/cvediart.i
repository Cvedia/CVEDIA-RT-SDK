%module rt

%{
#define SWIG_FILE_WITH_INIT
#include <numpy/arrayobject.h>
%}

%include "numpy.i"

// Initialize numpy array functionality
%init %{
  import_array();
%}

%include rt_py.i
%include securt_py.i

%pythonbegin %{
import os
import sys

# Get the CVEDIART_PATH from environment
cvediart_home = os.getenv("CVEDIART_PATH", default="")
if cvediart_home == "":
	print("Please set the CVEDIART_PATH environment variable to the root of the CVEDIA RT installation")
	sys.exit(1)

def add_library_directory(path):
    if sys.platform == 'win32':
        os.add_dll_directory(path)  # For Windows, Python 3.8+
    elif sys.platform in ['linux', 'linux2', 'darwin']:  # For Linux and macOS
        current_ld_library_path = os.environ.get('LD_LIBRARY_PATH', '')
        new_ld_library_path = path + ':' + current_ld_library_path
        os.environ['LD_LIBRARY_PATH'] = new_ld_library_path

add_library_directory(cvediart_home)
%}

%pythoncode %{
_cvediartpy.system_set_rt_home_dir(cvediart_home)
%}