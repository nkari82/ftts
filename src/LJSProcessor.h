#pragma once

class PhonetisaurusScript;

namespace ftts
{
	class LJSProcessor : public Processor
	{
	private:
		std::shared_ptr<PhonetisaurusScript> script_;

	public:
		LJSProcessor(const char* args);

		virtual ~LJSProcessor();

		void ToSeq(const char* text, std::vector<int32_t>& seq, const char* enc) override;
	};
};