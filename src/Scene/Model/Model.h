#pragma once
#include "Engine/Asset.h"

namespace Destiny
{
	class ModelLoader;
	class Model : public Asset
	{
	public:
		Model();
		virtual ~Model();
	public:
		static std::shared_ptr<ModelLoader> s_modelLoader;
		static std::shared_ptr<Model> Create(const char* path);
	};
}