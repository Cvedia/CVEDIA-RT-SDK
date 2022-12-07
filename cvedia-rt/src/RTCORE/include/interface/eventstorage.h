/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include "defines.h"

namespace cvedia {
	namespace rt {
		namespace iface {
			enum EventActionTypeEnum
			{
				EventActionType_Disk = 0x01,
				EventActionType_MQTT = 0x02,
				EventActionType_REST = 0x03,
				EventActionType_ScreenMessagePopup = 0x04,
				EventActionType_ScreenNotification = 0x05,
				EventActionType_Sound = 0x06,
				EventActionType_Log = 0x07,
				EventActionType_SharedMemory = 0x08,
				//#if WITH_GPIO
				EventActionType_GPIO = 0xdd,
				//#endif
				EventActionType_None = 0xff
			};

			class EventStorage {
			public:
				virtual ~EventStorage() = default;

				virtual bool queryApplies(std::string const& query, pCValue evnt) = 0;

				virtual pCValue query(std::string const& query, int listSize = -1) = 0;

				virtual bool store(std::string const& descriptor, pCValue const eventValues, std::string const& type = "", std::string const& sourceDesc = "", cbuffer img = nullptr, std::string const& timestamp = "") = 0;

				virtual void setStorage(pCValue eventStorage) = 0;
			};
		}
	}
}
