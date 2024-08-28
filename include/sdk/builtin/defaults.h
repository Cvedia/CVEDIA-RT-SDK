#pragma once

// This is a header where we define the default values for the configuration 
// of various system components (that are likely to be referenced from other components)
// for convenience and consistency

namespace cvedia
{
namespace rt
{
namespace defaults
{
	namespace solution_manager
	{
		constexpr bool persistent = true;
		constexpr bool metadataMode = true;
		constexpr bool statisticsMode = true;
		constexpr bool diagnosticsMode = false;
		constexpr bool debugMode = false;
		constexpr int frameRateLimit = 0;
		constexpr bool autoStart = false;
		constexpr bool autoRestart = true;
	} // namespace solution_manager

} // namespace defaults
} // namespace rt
} // namespace cvedia