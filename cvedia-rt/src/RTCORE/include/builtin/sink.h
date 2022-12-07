/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <shared_mutex>

#include "defines.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class Instance;
		}

		class Sink {
		public:
			EXPORT Sink(bool isInput);

			EXPORT pCValue read();
			EXPORT void write(std::string key, std::string type, pCValue data, std::string displayName);
			EXPORT void flush(iface::Instance* inst);
			EXPORT void clear();
		private:
			double flushCount_ = 0;
			bool isInput_ = false;
			cmap data_;
			mutable __shared_mutex_class sinkMut_;
		};
	}
}