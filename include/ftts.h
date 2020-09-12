#pragma once
#include <memory>
#include <vector>

namespace ftts
{
	class IProcessor
	{
	public:
		using Ptr = std::shared_ptr<IProcessor>;
		virtual ~IProcessor() = default;
		virtual void ToSeq(const char* text, std::vector<int32_t>& seq, const char* enc = nullptr) = 0;

		static Ptr Create(const char* processor = nullptr);
	};
}