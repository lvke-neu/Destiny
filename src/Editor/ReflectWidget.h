#pragma once
#include <memory>

class QVBoxLayout;
namespace Destiny
{
	class Reflection;
	class ReflectWidget
	{
	public:
		static void Reflect(std::shared_ptr<Reflection> reflection, QVBoxLayout* layout);
	};
}