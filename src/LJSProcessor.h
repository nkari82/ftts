#pragma once

namespace ftts
{
	class LJSProcessor : public Processor
	{
	public:
		LJSProcessor(const char* args);

		virtual ~LJSProcessor();

		void ToSeq(const char* text, std::vector<int32_t>& seq, const char* enc) override;
	};
};