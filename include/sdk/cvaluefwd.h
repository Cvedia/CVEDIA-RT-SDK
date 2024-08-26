/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>

typedef std::vector<std::string> STRVECT;

namespace cvedia {
	namespace rt {
		class CValue;
		namespace iface {
			class CVDictRoot;
			class CVDict;
			class Buffer;
		}

		typedef std::shared_ptr<CValue> pCValue;
		typedef std::map<std::string, pCValue> cmap;
		typedef std::vector<pCValue> cvec;
		typedef std::shared_ptr<iface::Buffer> cbuffer;
		typedef std::shared_ptr<std::vector<unsigned char>> cmem;
		typedef std::unique_ptr<iface::CVDictRoot> uCVDictRoot;
		typedef std::shared_ptr<iface::CVDict> pCVDict;
	}
}
