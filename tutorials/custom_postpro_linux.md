# Creating a custom post processing plugin

#### Requirements
- Ubuntu 20.04
- GCC 8.4+
- CMake 3.2+
- .deb release of CVEDIA-RT

If you're looking to run a new Neural Network architecture in CVEDIA-RT you'll most likely need to convert the output tensors into a usable format. We call this step the `post processing`. Post processing in RT is done through plugins that are dynamically loaded at Runtime. Each plugin generally handles one type of architecture, although multiple can be supported.

## Installing the CVEDIA-RT SDK

Before you can build a plugin you first need to download the [CVEDIA-RT SDK](https://github.com/Cvedia/CVEDIA-RT-SDK). Make sure to pick the version of the SDK matches that CVEDIA-RT on your system.

```
git clone https://github.com/Cvedia/CVEDIA-RT-SDK.git --recursive
cd CVEDIA-RT-SDK
```

Next you can build and install the bundled plugins by running:
```
mkdir build
cd build
cmake ../cvedia-rt/src -DCVEDIA_INSTALL_DIR=<path to RT install>
make -j4
make install
```

If all went well the last command should show an output similar to:
```
-- Installing: /opt/cvedia-rt/Plugins/postclassifier.so
-- Installing: /opt/cvedia-rt/Plugins/postyolov4.so
-- Installing: /opt/cvedia-rt/Plugins/postyolox.so
```

## Building your first post processing plugin

First we create the folders to store our custom plugin in:
```
mkdir cvedia-rt/src/Plugins/HelloWorld
mkdir cvedia-rt/src/Plugins/HelloWorld/src
```

Now create a file called `helloworld.cpp` in `cvedia-rt/src/Plugins/HelloWorld/src/`  with the following contents:
```
#include "api/inference.h"
#include "cvalue.h"
#include "plog/Init.h"

using namespace cvedia::rt;
using namespace cvedia::rt::internal;

extern "C" EXPORT void registerHandler();
extern "C" EXPORT void logInit(plog::Severity severity, plog::IAppender * appender);

void logInit(plog::Severity severity, plog::IAppender * appender)
{
	plog::init(severity, appender); // Initialize the shared library logger.
}

/*
* Return a simple key/value for testing
*/
static expected<pCValue> helloworld(InferenceContext& /*ctx*/, std::vector<xt::xarray<float>> & /*output*/, CValue */*modelConf*/) {
	cvec cv;
	cmap cm;
	
	cm["data"] = VAL("Hello world");
	cv.push_back(VAL(cm));

    return VAL(cv);
}

void registerHandler() {
	api::inference::registerPostprocessHandler("helloworld", helloworld);
}
```

Next we create a `CMakeLists.txt` in `cvedia-rt/src/Plugins/HelloWorld/` and fill it with the following:
```
message("Configuring HelloWorld plugin")

# Add source to this project's executable.
add_library (HelloWorld MODULE "src/helloworld.cpp")

set_target_properties(HelloWorld 
                        PROPERTIES 
                        PREFIX ""
                        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Plugins/$<0:>")

target_include_directories(HelloWorld
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries (HelloWorld PUBLIC RTCORE)

install(TARGETS HelloWorld DESTINATION ${CMAKE_INSTALL_PREFIX}/Plugins)
```

Finally we modify `cvedia-rt/src/Plugins/CMakeLists.txt` so we can add our custom plugin to the end of the build list:

```
# SPDX-FileCopyrightText: 2022 CVEDIA LTD
#
# SPDX-License-Identifier: Apache-2.0

if(WITH_MNN_MODULE)
    subdirs("MNN")
endif()

if (WITH_POST_CLASSIFIER_MODULE)
    subdirs("PostClassifier")
endif()

if (WITH_POST_YOLOV4_MODULE)
    subdirs("PostYoloV4")
endif()

if (WITH_POST_YOLOX_MODULE)
    subdirs("PostYoloX")
endif ()

# My hello world plugin
subdirs("HelloWorld")

```

Now we can change back to the `build` folder and repeat:
```
make -j4
make install
```

This time you should see your plugin being installed in the CVEDIA-RT runtime:
```
-- Installing: /opt/cvedia-rt/Plugins/HelloWorld.so
-- Installing: /opt/cvedia-rt/Plugins/postclassifier.so
-- Installing: /opt/cvedia-rt/Plugins/postyolov4.so
-- Installing: /opt/cvedia-rt/Plugins/postyolox.so
```

## Confirming your plugin is found

Since the plugin is already copied to the correct location we only have to run the following command:
```
aj@anbessa:/opt# listnndevices -v | grep hello
2022-12-07 15:03:30.277 INFO  [inferenceregistry.cpp@82] [Global] Registering postprocess handler helloworld
```

## Getting output from your plugin

The data your plugin returns can be retrieved in many ways (C++ API, Lua, CLI). Here I'll concentrate on using a CLI utility just to confirm everything works.

Because our plugin doesn't actually do anything useful at this point we can use any AI model just for testing purpose. If you have your own model you can use that, if not, use the following commands to install one of CVEDIA's default models:

```
modelforge -u openvino.CPU://pva_det/rgb_gs/nano_160x160/220421b -d
```
This downloads the model to `/opt/cvedia-rt/assets/models/pva_det/`. 

Next we have to replace the default post-processing plugin for this model with our `helloworld`.

Open `/opt/cvedia-rt/assets/models/pva_det/pva_det_rgb_gs_nano_160x160_220421b.xml.json` and change `postprocess` from `yolox` to `helloworld`.

When we `runinference` on any image or video now you should see the following output:
```
runinference -m openvino.CPU://pva_det/rgb_gs/nano_160x160/220421b -i <image.jpg>

[{"data":"Hello world","source":"<buffer>"}]
```

## Returning actual data

Transforming tensors into usable data can be quite challenging at times. We rely heavily on `xtensor` as it provides an interface that's highly similar to `numpy`, making it easier to port over Python implementations.

Your next step would be to go over the example plugins in the `cvedia-rt/src/Plugins/` folder. We've bundled some of the different `detector` architectures as well as a `classifier` implementation.

For any questions on the data formats or conventions feel free to hit us up on Discord: https://discord.gg/7RdNVkRAQW

## Performance of your plugin

As optimizations matter greatly when developing for Edge devices it's important to keep an eye on the performance of your plugin. Using our model benchmark you can see how much time is spent in your plugin and you can even compare that to other implementations.

This is how you would run a benchmark:
```
aj@anbessa:/opt/CVEDIA-RT-SDK# benchmark -u openvino.CPU://pva_det/rgb_gs/nano_160x160/220421b -i 3 -n 10000
Running 1 thread(s)
Loading 1 backend(s)
[1/1] openvino.CPU://pva_det/rgb_gs/nano_160x160/220421b
Shape: 1x1x160x160
Number of channels in this model might not be supported
           Time     Preprocess      Inference      Inference    Postprocess
                      cpu time        latency     throughput       cpu time
    -----------    -----------    -----------    -----------    -----------
       00:00:03    0.0771644ms      2.74361ms      348.767/s    0.0145339ms
       00:00:06    0.0756386ms      2.74811ms       348.55/s    0.0146319ms
       00:00:09      0.06791ms      2.47256ms      387.762/s    0.0118216ms
       00:00:12    0.0686548ms      2.47962ms      386.538/s     0.012338ms

```

The last column shows the performance of the `Hello World` plugin. You can run `benchmark` with `--help` to see many of the other options available.