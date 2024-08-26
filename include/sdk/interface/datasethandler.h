/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once
#include "cvaluefwd.h"
#include "rterror.h"

namespace cvedia {
	namespace rt {
		namespace iface {

			class EXPORT_CLASS DatasetHandler
			{
			public:
				DatasetHandler() = default;
				virtual ~DatasetHandler() = default;
				DatasetHandler(std::string const& /*path*/) {};

				DatasetHandler(DatasetHandler const&) = delete;
				DatasetHandler& operator =(DatasetHandler const&) = delete;

				virtual expected<std::vector<pCValue>> readDataset(int limit) = 0;
				virtual expected<void> writeDataset(std::vector<pCValue> data) = 0;
			};
		}
	}
}
