/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once

#include <memory>
#include <nlohmann/json_fwd.hpp>

#include "defines.h"
#include "interface/exporter.h"

namespace cvedia {
	namespace rt {
		class Process;

		class EXPORT_CLASS ExporterImpl : public iface::Exporter
		{
		public:
			EXPORT ExporterImpl();
			~ExporterImpl() override = default;

			ExporterImpl(ExporterImpl const&) = delete;
			ExporterImpl& operator =(ExporterImpl const&) = delete;

			EXPORT static std::unique_ptr<iface::Exporter> create();
			NODISCARD EXPORT expected<void> initialize(std::string const& initFile) override;

			EXPORT std::string getName() override;
			EXPORT iface::SchemaCapabilities getCapabilities() override;
			EXPORT expected<iface::SchemaCapabilities> getRequestedCapabilities() override;

			EXPORT std::map<std::string, std::shared_ptr<ConfigDescriptor>> getConfigDescriptors() override;

			NODISCARD EXPORT expected<void> writeData(nlohmann::json const& data) override;
			NODISCARD EXPORT expected<void> writeData(std::string const& data) override;
			EXPORT nlohmann::json getConfig();
			EXPORT std::map<std::string, std::string>& getEnvironment();
			EXPORT std::string getCommand();
		private:

			NODISCARD expected<iface::SchemaCapabilities> _parseCapabilities(CValue const& data) const;
			NODISCARD expected<void> _parseCommand(CValue const& data);
			NODISCARD expected<void> _parseConfigDescriptors(CValue const& data);
			NODISCARD expected<void> _parseEnvironmentVariables(CValue const& data);
			NODISCARD expected<void> _parseIntRange(pCValue const& data, std::shared_ptr<ConfigDescriptor> conf) const;
			NODISCARD expected<void> _parseFloatRange(pCValue const& data, std::shared_ptr<ConfigDescriptor> conf) const;
			NODISCARD expected<void> _parseTextField(pCValue const& data, std::shared_ptr<ConfigDescriptor> conf) const;
			NODISCARD expected<void> _parseCheckbox(pCValue const& data, std::shared_ptr<ConfigDescriptor> conf) const;
			NODISCARD expected<void> _parseCombobox(pCValue const& data, std::shared_ptr<ConfigDescriptor> conf) const;

			std::string name_;
			iface::SchemaCapabilities capabilities_;
			std::map<std::string, std::shared_ptr<ConfigDescriptor>> config_;
			std::map<std::string, std::string> environment_;
			std::string command_;
			std::shared_ptr<Process> process_;
		};
	}
}
