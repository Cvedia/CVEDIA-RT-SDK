#pragma once

#include <string>

#include "defines.h"
#include "builtin/basetypes.h"

EXPORT std::string floatToString(float value, int precision);
EXPORT std::string secondsToTimeString(int64_t totalSeconds);
cvedia::rt::Rect2f padToAspectRatio(cvedia::rt::Rect2f const& rect, float aspectRatio, float scaleFactor, cvedia::rt::Size const& size);
char* copyString(std::string const& str);

EXPORT bool validateBool(int value);
EXPORT bool validateEnum(int value, int min, int max);
EXPORT bool validateDirection(int direction);
EXPORT bool validateNumberOfPoints(int numPoints);
EXPORT bool validateClasses(int const* classes, int const classesSize);
EXPORT bool validateMode(int mode);
EXPORT bool validateAreaEvent(int areaEvent);
EXPORT bool validateSensitivity(int sensitivity);
EXPORT bool validateModality(int modality);
EXPORT bool validatePerformanceProfile(int profile);
