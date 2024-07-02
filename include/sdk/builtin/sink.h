/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <shared_mutex>

#include "defines.h"
#include "cvaluefwd.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class Instance;
		}

		class Sink {
		public:
			EXPORT Sink(bool isInput);

			EXPORT pCValue read();
			EXPORT void write(std::string const& key, std::string const& type, pCValue data, std::string const& displayName);
			EXPORT void flush(iface::Instance* inst, bool withCounter = true);
			EXPORT void clear();
		private:
			double flushCount_ = 0;
			bool isInput_ = false;
			cmap data_;
			mutable shared_mutex_class sinkMut_;
		};
	}
}