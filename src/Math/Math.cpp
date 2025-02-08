#include "Math.h"
#include <random>

namespace Destiny
{
	namespace Math
	{
		float RandomFloat(float min, float max)
		{
			std::random_device rd;
			std::default_random_engine engine(rd());

			std::uniform_real_distribution<float> distribution(min, max);

			return distribution(engine);
		}
	}
}