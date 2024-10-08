# CVEDIA-RT SDK

This SDK contains a set of APIs to interact with CVEDIA-RT engine - a powerful, modular, and cross-platform AI Video Analytics inference engine designed to deliver high-performance video analysis across diverse environments. Whether you're building applications on Linux or Windows, CVEDIA-RT provides the tools necessary to integrate advanced AI capabilities into your systems with ease.

## Contents

CVEDIA-RT SDK offers a set of bindings for popular programming languages, including C++, C, C# and Python. This flexibility allows developers to seamlessly incorporate AI-driven video analytics into a wide range of applications, regardless of the underlying hardware or operating system.

In this package you can find the following:

- language bindings, allowing you to develop in the language you're most comfortable with
- a set of CVEDIA-RT header files, which allows you to interact directly with our AI engine and extend it with your own plugins
- powerful **Core API** which provides fundamental functions for interacting with the CVEDIA-RT engine, offering essential capabilities for managing video streams, processing analytics, and handling results
- **SecuRT API**, containing a set of functions to interact with our most advanced solution for security applications
- example code to show how to interact with CVEDIA-RT, helping you to quickly grasp how to integrate our engine with different systems

## Directory structure

- **include** directory contains C++ header files
- C++, C and Python examples are available in **samples\cli**
- **samples\Plugins** contains example CVEDIA-RT plugins
- **dotnet** folder contains C# bindings and demos
- **python** folder contains python tests for our SecuRT solution

## Requirements

### Windows

- an IDE of your choice
- Visual Studio 2019 / 2022 build tools
- git
- SWIG

### Linux

- GCC 8.4+              -- Ubuntu: `sudo apt install build-essential`
- CMAKE 3.23+           -- Ubuntu: Check how to install newest package here https://apt.kitware.com or use pip
- git                   -- Ubuntu: `sudo apt install git`
- swig                  -- Ubuntu: `sudo apt install swig`
- cpp rest [REST API]   -- Ubuntu: `sudo apt install libcpprest-dev`
- numpy [python API]    -- pip install numpy>=1.24.4,<1.26.0
- (optional for RTSP) gstreamer  -- Ubuntu: Check how to install here: https://gstreamer.freedesktop.org/documentation/installing/on-linux.html?gi-language=c#install-gstreamer-on-ubuntu-or-debian

### SDK Frameworks tested with
- Python: 3.10
- .NET: 6.0.414
    - Ubuntu: `sudo apt-get install -y dotnet-sdk-6.0`


## Quickstart

### Demo resources
http://bin.cvedia.com/videos/images_resources.zip
- `images` directory - Directory with images to feed in the C++ demo
- images.bin - Same images as in directory, but packed as raw frames in single file (C demo)

### Linux - Build

```bash
# INITIAL SETTINGS - Set according to your setup

RT_BIN_DIR="/opt/cvedia-rt"                                 # Path to CVEDIA-RT binaries
RT_SDK_DIR="${PWD}"                                         # Path to this sdk

SDK_BUILD_DIR="${PWD}/build"                                # Path to build directory
SDK_INSTALL_DIR="${PWD}/install"                            # Path to installation directory

# Python - When building python bindings
PYTHON_EXECUTABLE="/usr/bin/python3"                        # which python3
PYTHON_INCLUDE_DIR="/usr/include/python3.10"                # find /usr/include/ -maxdepth 1 -type d -name 'python3*' | head -n1
PYTHON_LIBRARY="/lib/x86_64-linux-gnu/libpython3.10.so.1.0" # ldconfig -vp | grep python3 | rev | cut -d' ' -f1 | rev | head -n1

# Create build directory
mkdir -p $SDK_BUILD_DIR
cd $SDK_BUILD_DIR

RT_LD_LIB_PATHS="${RT_BIN_DIR}:${RT_BIN_DIR}/lib"

# Prepare build - First line is base, following lines are optional
cmake "${RT_SDK_DIR}" -DCVEDIA_INSTALL_DIR="${RT_BIN_DIR}" -DCMAKE_INSTALL_PREFIX="${SDK_INSTALL_DIR}" \
    -DWITH_PUSHFRAMEDEMO_C_CLI=ON \
    -DWITH_PUSHFRAMEDEMO_CPP_CLI=ON \
    -DWITH_CORE_REST_CLIENT=ON \
    -DWITH_SECURT_REST_CLIENT=ON \
    -DWITH_ONNX_MODULE=OFF \
    -DWITH_SECURT_LIB=ON \
    -DWITH_SECURT_LIB_PYTHON=ON \
    -DWITH_SECURT_LIB_DOTNET=ON \
    -DWITH_RTSPDEMO_CLI=ON \
    -DPYTHON_BINDINGS_INSTALL_PATH=${SDK_INSTALL_DIR}/python/scripts/ \
    -DPython_EXECUTABLE:FILEPATH=${PYTHON_EXECUTABLE} \
    -DPython_INCLUDE_DIR:PATH=${PYTHON_INCLUDE_DIR} \
    -DPython_LIBRARY:FILE=${PYTHON_LIBRARY}

# Build
LD_LIBRARY_PATH="${RT_LD_LIB_PATHS}:${LD_LIBRARY_PATH}" cmake --build .

# Install (to install dir set above)
cmake --install .
```

### Linux - Test build

**BE SURE YOU HAVE ACTIVATED YOUR RT LICENSE**
Trial license: `rtcmd license -t`

**Download and unpack resources**
```bash
cd ${SDK_INSTALL_DIR}
wget http://bin.cvedia.com/videos/images_resources.zip
unzip images_resources.zip
```

**C DEMO**
```bash
cd ${SDK_INSTALL_DIR}
INPUT_IMAGE_BIN="${PWD}/images.bin"
LD_LIBRARY_PATH="${RT_LD_LIB_PATHS}:${LD_LIBRARY_PATH}" RT_SOLUTIONS_PATH="${RT_BIN_DIR}/solutions" IMAGE_BIN_CONTAINER="${INPUT_IMAGE_BIN}" ./pfdemo_c
```

**C++ DEMO**
```bash
cd ${SDK_INSTALL_DIR}
INPUT_IMAGES="${PWD}/images"
LD_LIBRARY_PATH="${RT_LD_LIB_PATHS}:${LD_LIBRARY_PATH}" ./pfdemo_cpp -i "${INPUT_IMAGES}" -s "${RT_BIN_DIR}/solutions" -v
```

**Python**
```bash
export CVEDIART_PATH=${RT_BIN_DIR}
cd ${SDK_INSTALL_DIR}/python/scripts
pip install -e .
# NOTE: sometimes you can use directly pytest instead of python3 -m pytest
LD_LIBRARY_PATH="${RT_LD_LIB_PATHS}:${LD_LIBRARY_PATH}" python3 -m pytest
```

**.NET**
```
cd ${RT_SDK_DIR}/dotnet/scripts
ln -s ${RT_BIN_DIR}/dotnet/scripts/bindings
ln -s ${RT_BIN_DIR}/dotnet/scripts/bindings/securtcsharp.so
dotnet build SecuRT_Test.sln -v d -c Release
cd ../UnitTests
export CVEDIART_HOME=${RT_BIN_DIR}
export SOLUTIONS_DIR=${RT_SOLUTIONS_PATH}
LD_LIBRARY_PATH="${RT_BIN_DIR}:${RT_BIN_DIR}/lib" dotnet test --no-build -c Release -l "console;verbosity=normal"
```

## Documentation

Full documentation is always available online at https://docs.cvedia.com

