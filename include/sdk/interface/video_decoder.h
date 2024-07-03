#pragma once

#include <opencv2/core/mat.hpp>
#include "builtin/moduleimpl.h"
#include "rterror.h"

namespace cvedia {
	namespace rt {
		enum Codec : size_t {
			H264,
			HEVC,
			MPEG2,
			MPEG4,
			MJPEG,
			VP8,
			VP9,
			VC1,
			AV1
		};

		inline std::string codecToName(Codec codec)
		{
			switch (codec)
			{
			case Codec::H264:
				return "H264";
			case Codec::HEVC:
				return "HEVC";
			case Codec::MPEG2:
				return "MPEG2";
			case Codec::MPEG4:
				return "MPEG4";
			case Codec::MJPEG:
				return "MJPEG";
			case Codec::VP8:
				return "VP8";
			case Codec::VP9:
				return "VP9";
			case Codec::VC1:
				return "VC1";
			case Codec::AV1:
				return "AV1";
			default:
				return "Unknown";
			}
		}

		inline Codec nameToCodec(std::string const& name)
		{
			if (name == "H264")
			{
				return Codec::H264;
			}
			else if (name == "HEVC")
			{
				return Codec::HEVC;
			}
			else if (name == "MPEG2")
			{
				return Codec::MPEG2;
			}
			else if (name == "MPEG4")
			{
				return Codec::MPEG4;
			}
			else if (name == "MJPEG")
			{
				return Codec::MJPEG;
			}
			else if (name == "VP8")
			{
				return Codec::VP8;
			}
			else if (name == "VP9")
			{
				return Codec::VP9;
			}
			else if (name == "VC1")
			{
				return Codec::VC1;
			}
			else if (name == "AV1")
			{
				return Codec::AV1;
			}
			else
			{
				throw std::runtime_error("Unknown codec");
			}
		}

		struct Frame {
			cv::Mat image;
		};

		namespace iface {
			class VideoDecoder : public ModuleImpl
			{
			public:
				explicit VideoDecoder(std::string const& moduleName) : ModuleImpl(moduleName) {}
				virtual ~VideoDecoder() override = default;

				virtual expected<std::vector<Frame>> decodeFrame(unsigned char const* data, size_t dataSize) = 0;
			};
		}
	}
}
