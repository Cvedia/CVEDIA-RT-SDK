# SPDX-FileCopyrightText: 2022 CVEDIA LTD
#
# SPDX-License-Identifier: Apache-2.0

if(WITH_ONNX_MODULE)
    subdirs("ONNX")
endif()

if (WITH_POST_CLASSIFIER_MODULE)
    subdirs("PostClassifier")
endif()
