#pragma once
#include <memory>

namespace ftts
{
	class IProcessor
	{
	public:
		using Ptr = std::shared_ptr<IProcessor>;
		virtual ~IProcessor() = default;

		static Ptr Create(const char* processor = nullptr);
	};
}