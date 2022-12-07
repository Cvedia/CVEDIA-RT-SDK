/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once

#include "defines.h"

namespace cvedia {
	namespace rt {

		namespace internal {
			class VersionControl;
		}

		//enum BufferDataType {
		//	GRAY = CV_8U,	// 00
		//	BGR = CV_8UC3,	// 16
		//	BGRA = CV_8UC4  // 24
		//};


		struct BufferInit {
			std::shared_ptr<iface::Buffer> instance = nullptr;
		};

#ifdef _WIN32
		typedef void(__stdcall* _InstantiateBufferHandler)(BufferInit* init);
#else
		typedef void(*_InstantiateBufferHandler)(BufferInit* init);
#endif

		class BufferMgr {
		public:
			EXPORT static std::shared_ptr<iface::Buffer> getBufferHandler(std::string name);

			//! \cond
			EXPORT static void registerBufferHandler(std::string name, _InstantiateBufferHandler _bufferPtr);
			//! \endcond

		private:

			//! \cond
			static std::map<std::string, _InstantiateBufferHandler> bufferPtrs_;
			//! \endcond
		};

		EXPORT pCValue composeFrame(cbuffer image, cmap meta);

	}
}
