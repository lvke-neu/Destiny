#include "StructDefine.h"
#include <numeric> 
#include <random>
#include <chrono>

namespace Destiny
{
	//std::vector<AABB> GenerateTrulyRandomAABBs(unsigned int boxCount) {
	//	std::vector<AABB> aabbs;
	//	aabbs.resize(boxCount);

	//	auto now = std::chrono::high_resolution_clock::now();
	//	auto seed = now.time_since_epoch().count();
	//	std::mt19937_64 gen(seed);


	//	std::uniform_real_distribution<float> baseOffsetDist(-500.0f, 500.0f);
	//	float baseX = baseOffsetDist(gen);
	//	float baseY = baseOffsetDist(gen);
	//	float baseZ = baseOffsetDist(gen);

	//	std::uniform_real_distribution<float> scaleDist(100.0f, 1000.0f);
	//	float scale = scaleDist(gen);

	//	for (unsigned int i = 0; i < boxCount; ++i) {

	//		std::uniform_real_distribution<float> centerDist(-scale / 2, scale / 2);
	//		float centerX = baseX + centerDist(gen);
	//		float centerY = baseY + centerDist(gen);
	//		float centerZ = baseZ + centerDist(gen);

	//		std::uniform_real_distribution<float> sizeDist(1.0f, scale * 0.1f);
	//		float sizeX = sizeDist(gen);
	//		float sizeY = sizeDist(gen);
	//		float sizeZ = sizeDist(gen);


	//		AABB aabb;
	//		aabb.min = { centerX - sizeX / 2, centerY - sizeY / 2, centerZ - sizeZ / 2 };
	//		aabb.max = { centerX + sizeX / 2, centerY + sizeY / 2, centerZ + sizeZ / 2 };

	//		aabbs[i] = aabb;
	//	}

	//	return aabbs;
	//}

	std::vector<AABB> GenerateTrulyRandomAABBs(unsigned int boxCount) {
		std::vector<AABB> aabbs;
		aabbs.resize(boxCount);

		auto now = std::chrono::high_resolution_clock::now();
		auto seed = now.time_since_epoch().count();
		std::mt19937_64 gen(seed);

		float baseX = 0.0f;
		float baseY = 0.0f;
		float baseZ = 50.0f;

		std::uniform_real_distribution<float> sizeDist(1.0f, 5.0f);

		std::uniform_real_distribution<float> offsetDist(-20.0f, 20.0f);

		for (unsigned int i = 0; i < boxCount; ++i) {
			float centerX = baseX + offsetDist(gen);
			float centerY = baseY + offsetDist(gen);
			float centerZ = baseZ + offsetDist(gen);

			float sizeX = sizeDist(gen);
			float sizeY = sizeDist(gen);
			float sizeZ = sizeDist(gen);

			AABB aabb;
			aabb.min = { centerX - sizeX * 0.5f, centerY - sizeY * 0.5f, centerZ - sizeZ * 0.5f };
			aabb.max = { centerX + sizeX * 0.5f, centerY + sizeY * 0.5f, centerZ + sizeZ * 0.5f };

			aabbs[i] = aabb;
		}

		return aabbs;
	}
}