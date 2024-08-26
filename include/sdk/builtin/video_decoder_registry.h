/*
	SPDX-FileCopyrightText: 2023 CVEDIA LTD

	SPDX-License-Identifier: LicenseRef-CVEDIA
*/
#pragma once 
#include <unordered_set>

#include "moduleimpl.h"
#include "interface/video_decoder.h"

namespace cvedia {
	namespace rt {
		class VideoDecoderRegistry final {
		public:
			static void initialize();
			static void reset();
				
			//! \cond
			EXPORT static void registerCodecHandler(std::string const& deviceId, Codec const& codec, module::createVideoDecoderHandler func);
			//! \endcond

			static std::unique_ptr<iface::VideoDecoder> create(std::string const& deviceId, Codec const& codec);

			EXPORT NODISCARD static std::map<std::string, std::unordered_set<Codec>> getSupportedDecoders();
		private:
			struct CodecHandler {
				std::string deviceId;
				module::createVideoDecoderHandler func;
			};

			//! \cond
			static std::map<Codec, std::vector<CodecHandler>> codecHandlers_;
			//! \endcond
		};
	}
}

