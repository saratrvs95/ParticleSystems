#include "ElapsedTime.h"
#include <algorithm>

namespace Time
{
	ElapsedTime::ElapsedTime(float maxTimeStep)
		: mMaxTimeStep(maxTimeStep)
		, mPrevious(TimePoint())
	{}

	float ElapsedTime::GetElapsedTime() const
	{
		float DeltaTime = (std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - mPrevious)).count() / 1000.f;
		mPrevious = Clock::now();

		// Clamp to the max time step
		DeltaTime = std::min(DeltaTime, mMaxTimeStep);

		return DeltaTime;
	}
}