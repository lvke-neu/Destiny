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

		void RandomUint(unsigned int min, unsigned int max, std::vector<unsigned int>& randoms)
		{
			if (randoms.empty())
				return;

			std::random_device rd;
			std::default_random_engine engine(rd());

			std::uniform_int_distribution<unsigned int> distribution(min, max);

			for (auto& val : randoms)
			{
				val = distribution(engine);
			}
		}
	}
}