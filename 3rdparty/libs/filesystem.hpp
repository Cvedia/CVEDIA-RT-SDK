#pragma once
#if (__cplusplus >= 201703L)
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <ghc_filesystem.hpp>
namespace fs = ghc::filesystem;
#endif
