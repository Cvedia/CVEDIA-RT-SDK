#pragma once

#include <string>

#include "defines.h"
#include "builtin/basetypes.h"

EXPORT std::string floatToString(float value, int precision);
EXPORT std::string secondsToTimeString(int64_t totalSeconds);
cvedia::rt::Rect2f padToAspectRatio(cvedia::rt::Rect2f const& rect, float aspectRatio, float scaleFactor, cvedia::rt::Size const& size);
char* copyString(std::string const& str);
