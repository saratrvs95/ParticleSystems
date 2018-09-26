#pragma once
#include <chrono>

namespace Time
{
	using Clock = std::chrono::high_resolution_clock;
	using TimePoint = Clock::time_point;
	using Duration = Clock::duration;

	class ElapsedTime
	{
	public:
		ElapsedTime(float maxTimeStep = 0.03333f);
		float GetElapsedTime() const;

	private:
		float mMaxTimeStep;
		mutable TimePoint mPrevious;

	};
}

