#pragma once

#include "api/logging.h"

namespace cvedia
{
namespace rt
{
namespace types
{
	// We specialize the following functions for each type for convenience and consistency

	// Some types have different representations in the API and in the SDK so we might need to specialize for different variants
	struct Default {};
	struct External {};

	template<typename T, typename Variant = Default>
	T getDefault()
	{
		LOGE << "getDefault not implemented for this type";
		return {};
	}

	template<typename T, typename Variant = Default>
	std::string toString(T const t)
	{
		LOGE << "toString not implemented for this type";
		return "";
	}

	template<typename T, typename Variant = Default>
	T fromString(std::string const& s)
	{
		LOGE << "fromString not implemented for this type";
		return "";
	}

	enum Severity
	{
		Empty = 1,
		Debug = 2,
		Info = 3,
		Warning = 4,
		Error = 5,
		Critical = 6
	};

	template<>
	inline Severity getDefault<Severity, Default>()
	{
		return Severity::Warning;
	}

	template<>
	inline Severity getDefault<Severity, External>()
	{
		return Severity::Warning;
	}

	template<>
	inline std::string toString<Severity, Default>(Severity const t)
	{
		switch (t)
		{
		case Severity::Empty:
			return "Empty";
		case Severity::Debug:
			return "Debug";
		case Severity::Info:
			return "Info";
		case Severity::Warning:
			return "Warning";
		case Severity::Error:
			return "Error";
		case Severity::Critical:
			return "Critical";
		default:
			LOGE << "toString() is not implemented for this enum value of Severity: " << static_cast<int>(t);
			return "";
		}
	}

	template<>
	inline std::string toString<Severity, External>(Severity const t)
	{
		return toString<Severity, Default>(t);
	}

	template<>
	inline Severity fromString<Severity, Default>(std::string const& s)
	{
		if (s == "Empty")
			return Severity::Empty;
		if (s == "Debug")
			return Severity::Debug;
		if (s == "Info")
			return Severity::Info;
		if (s == "Warning")
			return Severity::Warning;
		if (s == "Error")
			return Severity::Error;
		if (s == "Critical")
			return Severity::Critical;

		auto def = getDefault<Severity>();
		LOGE << "fromString() is not implemented for this string value of Severity: " << s << ", using default value: " << toString(def);
		return def;
	}

	template<>
	inline Severity fromString<Severity, External>(std::string const& s)
	{
		return fromString<Severity, Default>(s);
	}

	enum DetectorMode
	{
		SmartDetection = 1,
		Detection = 2,
	};

	template<>
	inline DetectorMode getDefault<DetectorMode, Default>()
	{
		return DetectorMode::SmartDetection;
	}

	template<>
	inline DetectorMode getDefault<DetectorMode, External>()
	{
		return DetectorMode::SmartDetection;
	}

	template<>
	inline std::string toString<DetectorMode, Default>(DetectorMode const t)
	{
		switch (t)
		{
		case DetectorMode::SmartDetection:
			return "MosaicInference";
		case DetectorMode::Detection:
			return "FullRegionInference";
		default:
			LOGE << "toString() is not implemented for this enum value of DetectorMode: " << static_cast<int>(t);
			return "";
		}
	}

	template<>
	inline std::string toString<DetectorMode, External>(DetectorMode const t)
	{
		switch (t)
		{
		case DetectorMode::SmartDetection:
			return "SmartDetection";
		case DetectorMode::Detection:
			return "Detection";
		default:
			LOGE << "toString() is not implemented for this enum value of DetectorMode: " << static_cast<int>(t);
			return "";
		}
	}

	template<>
	inline DetectorMode fromString<DetectorMode, Default>(std::string const& s)
	{
		if (s == "MosaicInference" || s == "SmartDetection")
			return DetectorMode::SmartDetection;
		if (s == "FullRegionInference" || s == "Detection")
			return DetectorMode::Detection;
		
		auto def = getDefault<DetectorMode>();
		LOGE << "fromString() is not implemented for this string value of DetectorMode: " << s << ", using default value: " << toString(def);
		return def;
	}

	template<>
	inline DetectorMode fromString<DetectorMode, External>(std::string const& s)
	{
		return fromString<DetectorMode, Default>(s);
	}

	enum Modality {
		RGB = 1,
		Thermal = 2
	};

	template<>
	inline Modality getDefault<Modality, Default>()
	{
		return Modality::RGB;
	}

	template<>
	inline Modality getDefault<Modality, External>()
	{
		return Modality::RGB;
	}

	template<>
	inline std::string toString<Modality, Default>(Modality const t)
	{
		switch (t)
		{
		case Modality::RGB:
			return "rgb";
		case Modality::Thermal:
			return "thermal";
		default:
			LOGE << "toString() is not implemented for this enum value of Modality: " << static_cast<int>(t);
			return "";
		}
	}

	template<>
	inline std::string toString<Modality, External>(Modality const t)
	{
		switch (t)
		{
		case Modality::RGB:
			return "RGB";
		case Modality::Thermal:
			return "Thermal";
		default:
			LOGE << "toString() is not implemented for this enum value of Modality: " << static_cast<int>(t);
			return "";
		}
	}

	template<>
	inline Modality fromString<Modality, Default>(std::string const& s)
	{
		if (s == "rgb" || s == "RGB")
			return Modality::RGB;
		if (s == "thermal" || s == "Thermal")
			return Modality::Thermal;
		
		auto def = getDefault<Modality>();
		LOGE << "fromString() is not implemented for this string value of Modality: " << s << ", using default value: " << toString(def);
		return def;
	}

	template<>
	inline Modality fromString<Modality, External>(std::string const& s)
	{
		return fromString<Modality, Default>(s);
	}

	enum Sensitivity
	{
		Low = 1,
		Medium = 2,
		High = 3,
	};

	template<>
	inline Sensitivity getDefault<Sensitivity, Default>()
	{
		return Sensitivity::Medium;
	}

	template<>
	inline Sensitivity getDefault<Sensitivity, External>()
	{
		return Sensitivity::Medium;
	}

	template<>
	inline std::string toString<Sensitivity, Default>(Sensitivity const t)
	{
		switch (t)
		{
		case Sensitivity::Low:
			return "Low";
		case Sensitivity::Medium:
			return "Medium";
		case Sensitivity::High:
			return "High";
		default:
			LOGE << "toString() is not implemented for this enum value of Sensitivity: " << static_cast<int>(t);
			return "";
		}
	}

	template<>
	inline std::string toString<Sensitivity, External>(Sensitivity const t)
	{
		return toString<Sensitivity, Default>(t);
	}

	template<>
	inline Sensitivity fromString<Sensitivity, Default>(std::string const& s)
	{
		if (s == "Low")
			return Sensitivity::Low;
		if (s == "Medium")
			return Sensitivity::Medium;
		if (s == "High")
			return Sensitivity::High;

		auto def = getDefault<Sensitivity>();
		LOGE << "fromString() is not implemented for this string value of Sensitivity: " << s << ", using default value: " << toString(def);
		return def;
	}

	template<>
	inline Sensitivity fromString<Sensitivity, External>(std::string const& s)
	{
		return fromString<Sensitivity, Default>(s);
	}

} // namespace types
} // namespace rt
} // namespace cvedia
