#pragma once
#include <vector>

namespace Destiny
{
	namespace Math
	{
		static const float PI = 3.14159265358979323846f;
		static const float RAD2DEG = (180.f / PI);
		static const float DEG2RAD = (PI / 180.f);
		static const float FLT_THRESHOLD = 1.0e-5f;
		
		float RandomFloat(float min, float max);
		void RandomUint(unsigned int min, unsigned int max, std::vector<unsigned int>& randoms);

		template <typename T> T AlignPowerOfTwo(T value)
		{
			return value == 0 ? 0 : 1 << Log2(value);
		}
		__forceinline unsigned char Log2(unsigned int value)
		{
			unsigned long mssb; // most significant set bit
			unsigned long lssb; // least significant set bit

								// If perfect power of two (only one set bit), return index of bit.  Otherwise round up
								// fractional log by adding 1 to most signicant set bit's index.
			if (_BitScanReverse(&mssb, value) > 0 && _BitScanForward(&lssb, value) > 0)
				return unsigned char(mssb + (mssb == lssb ? 0 : 1));
			else
				return 0;
		}
	}
}