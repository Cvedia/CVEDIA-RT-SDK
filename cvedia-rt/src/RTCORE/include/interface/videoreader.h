/*
	SPDX-FileCopyrightText: 2022 CVEDIA LTD

	SPDX-License-Identifier: Apache-2.0
*/
#pragma once
#include "cvalue.h"
#include "defines.h"
#include "inputhandler.h"

namespace cvedia {
	namespace rt {
		namespace iface {

			class VideoReader : public InputHandler {
			public:
				VideoReader(std::string const& moduleName) : InputHandler(moduleName) {}

				virtual void setCurrentFrame(int frameNum) = 0;
				virtual int getFrameCount() = 0;				
				virtual bool canSeek() = 0;
				virtual void setRepeatMode(iface::InputRepeatMode mode) = 0;
				virtual iface::InputRepeatMode getRepeatMode() = 0;
				virtual expected<void> readConfig() = 0;
				virtual bool setSourceList(std::vector<std::string> const& source) = 0;
				virtual std::vector<std::string> getSourceList() = 0;
				virtual bool setSourceByIndex(int index) = 0;
				virtual int getSourceListIndex() = 0;
				virtual int getSourceListSize() = 0;
				virtual bool playlistReachedEnd() = 0;
				virtual bool fileReachedEnd() = 0;
				virtual bool isEnded() = 0;
				virtual bool setNextSource() = 0;
				virtual bool setPreviousSource() = 0;
				virtual std::string getSourceUri() = 0;
				virtual std::string getSourceDesc() = 0;
			};
		}
	}
}
