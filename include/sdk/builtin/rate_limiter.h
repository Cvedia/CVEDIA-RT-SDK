#pragma once
#include <chrono>
#include <deque>
#include <vector>

#include "defines.h"

class RateLimiter {
public:
    EXPORT RateLimiter();

	EXPORT void setTargetFrameRate(float targetFrameRate);
	EXPORT bool shouldProcess(std::chrono::milliseconds timestamp);
    EXPORT float getActualFPS() const;
private:
    std::chrono::milliseconds curExpectedFrameTime_ = {};
    std::chrono::milliseconds frameInterval_ = {};
    std::chrono::milliseconds lastTime_ = {};

    std::deque<double> fpsHistory_;
    size_t fpsHistorySize_ = 30;

	std::deque<std::chrono::milliseconds> timestamps_;
};
