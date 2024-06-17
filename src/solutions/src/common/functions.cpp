#include "common/functions.h"

#include <iomanip>
#include <ios>
#include <sstream>
#include <cstring>


std::string floatToString(float value, int precision) {
	std::stringstream ss;
	ss << std::fixed << std::setprecision(precision) << value;
	return ss.str();
}

/**
 * @brief Converts a given duration in seconds to a time string format.
 *
 * This function takes a duration represented in total seconds and converts it into a
 * human-friendly format of weeks, days, hours, minutes, and seconds. The resulting format
 * is a string, where each time component is represented by its first letter (w for weeks,
 * d for days, h for hours, m for minutes, and s for seconds) and a space between each component.
 * Only non-zero time components are included in the output string.
 *
 * The function works by repeatedly dividing the total seconds by the number of seconds in
 * each time component (week, day, hour, minute), and then taking the remainder. This allows
 * to break down the total seconds into individual time components.
 *
 * Example: secondsToTimeString(100000) returns "1w 1d 13h 46m 40s".
 *
 * @param totalSeconds The duration in total seconds to convert.
 * @return A string representation of the duration in a human-friendly format.
 */
std::string secondsToTimeString(int64_t totalSeconds) {
    std::stringstream ss;

    int64_t constexpr secondsInAMinute = 60;
    int64_t constexpr secondsInAnHour = 60 * secondsInAMinute;
    int64_t constexpr secondsInADay = 24 * secondsInAnHour;
    int64_t constexpr secondsInAWeek = 7 * secondsInADay;

    auto computeTime = [&](int64_t div) {
        int64_t const time = totalSeconds / div;
        totalSeconds %= div;
        return time;
    };

    int64_t weeks = computeTime(secondsInAWeek);
    int64_t days = computeTime(secondsInADay);
    int64_t hours = computeTime(secondsInAnHour);
    int64_t minutes = computeTime(secondsInAMinute);
    int64_t seconds = totalSeconds;

    if (weeks > 0) { ss << weeks << "w "; }
    if (days > 0) { ss << days << "d "; }
    if (hours > 0) { ss << hours << "h "; }
    if (minutes > 0) { ss << minutes << "m "; }

    ss << seconds << "s";

    return ss.str();
}

/**
 * @brief Function to adjust a given rectangle to match a specified aspect ratio, taking into account a scaling factor.
 *
 * The function takes a rectangle and modifies its dimensions to conform to the given aspect ratio,
 * while preserving the rectangle's center point. The dimensions are increased by a scale factor
 * before the aspect ratio adjustment. If after the scaling, the aspect ratio doesn't match the
 * desired one, the function increases the shorter dimension of the rectangle (width or height)
 * to reach the desired aspect ratio. It then ensures the rectangle lies entirely within the
 * image bounds by shifting and/or truncating it as needed.
 *
 * @param rect The original rectangle to be adjusted.
 * @param aspectRatio The desired aspect ratio to adjust the rectangle to.
 * @param scaleFactor The factor by which to scale the rectangle before adjusting the aspect ratio.
 * @param size The dimensions of the image or space within which the rectangle must fit.
 *
 * @return Returns a new rectangle that has been scaled, aspect ratio adjusted, and fitted within the given image size.
 */
cvedia::rt::Rect2f padToAspectRatio(cvedia::rt::Rect2f const& rect, float aspectRatio, float scaleFactor, cvedia::rt::Size const& size) {
	// Apply scaling
	float const scaledWidth = rect.width * scaleFactor;
	float const scaledHeight = rect.height * scaleFactor;

	cvedia::rt::Rect2f paddedRect(rect.x - (scaledWidth - rect.width) / 2.0f,
	                              rect.y - (scaledHeight - rect.height) / 2.0f,
	                              scaledWidth,
	                              scaledHeight);

	float const currentAspectRatio = paddedRect.width / paddedRect.height;

	if (currentAspectRatio > aspectRatio)
	{
		// Width is larger, increase height
		float const newHeight = paddedRect.width / aspectRatio;
		paddedRect.y -= (newHeight - paddedRect.height) / 2.0f;
		paddedRect.height = newHeight;
	}
	else if (currentAspectRatio < aspectRatio)
	{
		// Height is larger, increase width
		float const newWidth = paddedRect.height * aspectRatio;
		paddedRect.x -= (newWidth - paddedRect.width) / 2.0f;
		paddedRect.width = newWidth;
	}

	paddedRect.x = std::min(std::max(paddedRect.x, 0.0f), size.width - paddedRect.width);
	paddedRect.y = std::min(std::max(paddedRect.y, 0.0f), size.height - paddedRect.height);
	paddedRect.width = std::min(std::max(paddedRect.width, 0.0f), size.width - paddedRect.x);
	paddedRect.height = std::min(std::max(paddedRect.height, 0.0f), size.height - paddedRect.y);

	return paddedRect;
}

char* copyString(std::string const& str) {
#ifdef _WIN32
	return _strdup(str.c_str());
#else
	return strdup(str.c_str());
#endif
}
