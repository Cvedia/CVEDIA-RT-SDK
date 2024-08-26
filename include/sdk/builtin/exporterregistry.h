/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once

#include "defines.h"
#include "objectregistry.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			class Exporter;
		}

		class ExporterRegistry
		{
		public:
			EXPORT static void clear();
			NODISCARD EXPORT static expected<void> discoverExporters(std::string const& path);

			NODISCARD EXPORT static expected<std::shared_ptr<iface::Exporter>> getExporter(std::string const& exporterName);
			NODISCARD EXPORT static std::unordered_set< std::shared_ptr<iface::Exporter>> getExporters();
		private:
			NODISCARD static expected<void> _registerExporter(std::string const& exporterName, std::shared_ptr<iface::Exporter> exporter);
			NODISCARD static expected<void> _unregisterExporter(std::string const& exporterName);

			static ObjectRegistry<iface::Exporter> exporters_;
		};
	}
}
