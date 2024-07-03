/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include <shared_mutex>
#include "defines.h"
#include "builtin/moduleimpl.h"
#include "builtin/basetypes.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class EXPORT_CLASS AmbaOut : public ModuleImpl
			{
			public:
				AmbaOut(std::string const& moduleName) : ModuleImpl(moduleName) {}
				virtual ~AmbaOut() = default;

				/**
				* Module API
				*/
				virtual bool connectSink() = 0;
				virtual bool connectSink(std::string const& sink, std::string const& target) = 0;
				virtual void eventHandler(pCValue val) = 0;
			};
		}
	}
}
