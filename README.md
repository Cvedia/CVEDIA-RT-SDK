# CVEDIA-RT SDK

This SDK contains the base CVEDIA-RT headers to build plugins.

# Requirements:

## Windows

- Visual Studio 2019 / 2022

## Linux
- GCC 8.4+
- CMAKE 3.2+

# Quickstart

```
git clone https://github.com/Cvedia/CVEDIA-RT-SDK.git --recursive
cd CVEDIA-RT-SDK

mkdir build
cd build

cmake .. -DCVEDIA_INSTALL_DIR=<path to RT install>
make -j4
make install
```

# Full documentation

Please check the updated documentation at https://docs.cvedia.com
