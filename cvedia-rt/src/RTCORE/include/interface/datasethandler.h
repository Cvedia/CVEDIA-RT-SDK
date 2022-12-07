/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include "defines.h"

namespace cvedia {
	namespace rt {
		namespace iface {

			class DatasetHandler
			{
			public:
				DatasetHandler() = default;
				virtual ~DatasetHandler() = default;
				DatasetHandler(std::string const& path) {};

				DatasetHandler(DatasetHandler const&) = delete;
				DatasetHandler& operator =(DatasetHandler const&) = delete;

				virtual expected<std::vector<pCValue>> readDataset(int limit) = 0;
				virtual expected<void> writeDataset(std::vector<pCValue> data) = 0;
			};
		}
	}
}
