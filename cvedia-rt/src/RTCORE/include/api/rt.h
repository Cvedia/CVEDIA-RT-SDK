/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include "defines.h"

namespace cvedia {

	/**
		* \ingroup Managed
		* @{
		*/
		//! API for controlling the server and lifecycle management of runtime instances
	namespace rt {
		namespace api {
			NODISCARD EXPORT expected<void> initialize(bool enableLogging = true, bool enableConsoleLogging = true);
			EXPORT expected<void> shutdown();
			EXPORT expected<void> stopAllInstances();

			EXPORT bool isRunning();
		};
	}
}
