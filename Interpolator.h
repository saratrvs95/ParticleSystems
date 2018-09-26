#pragma once
#include <map>


namespace ParticleSystem
{
	template < typename T >
	class Interpolator
	{
	public:
		using Map = std::map< float, typename T >;
		using Iterator = typename Map::iterator;
		using ConstIterator = typename Map::const_iterator;

		Interpolator(const T& defaultValue = T())
			: mDefaultValue(defaultValue)
		{}

		// Add a value at time into the graph
		void AddValue(float time, const T& value);

		// Get the interpolated value at time.
		T GetValue(float time) const;

		void Clear();

	private:
		Map mValues;
		T mDefaultValue;
	};
}
#include "Interpolator.inl"