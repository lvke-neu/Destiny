#pragma once

namespace Destiny
{
	class ParallelSystem
	{
	public:
		virtual ~ParallelSystem() = default;
	public:
		virtual void initialize() = 0;
		virtual void uninitialize() = 0;
		virtual void tick() = 0;
	};
}