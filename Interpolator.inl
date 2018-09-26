#pragma once
#include "Interpolator.h"

namespace ParticleSystem
{
	template< typename T >
	void Interpolator<T>::AddValue(float time, const T& value)
	{
		mValues[time] = value;
	}

	template<typename T>
	inline T Interpolator<T>::GetValue(float time) const
	{
		// Case 1. There are no elements
		if (mValues.size() < 1) return mDefaultValue;
		// Case 2. There is exactly one element
		if (mValues.size() == 1) return mValues.begin()->second;
		// Case 3. There are 2 or more elements

		// Search through the map, stop when we find values between time.
		ConstIterator start = mValues.begin();
		ConstIterator end = start;

		while (end != mValues.end())
		{
			if (end->first > time)
			{
				break;
			}
			start = end;
			++end;
		}

		if (end == mValues.end())
		{
			return start->second;
		}

		// Linear Interpolate between start and end
		float initialKey = start->first;
		float finalKey = end->first;

		float ratio = (time - initialKey) / (finalKey - initialKey);

		const T& initialValue = start->second;
		const T& finalValue = end->second;

		return initialValue * (1.0f - ratio) + (finalValue * ratio);
	}

	template<typename T>
	inline void Interpolator<T>::Clear()
	{
		mValues.clear();
	}
}