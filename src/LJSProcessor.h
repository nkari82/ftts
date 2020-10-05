#pragma once

class PhonetisaurusScript;

namespace ftts
{
	// Copyright  https://github.com/ZDisket
	class LJSProcessor : public Processor
	{
	private:
		std::shared_ptr<PhonetisaurusScript> script_;

	public:
		LJSProcessor(const char* args);

		virtual ~LJSProcessor();

		void ToSeq(const char* text, std::vector<int32_t>& seq, const char* enc) override;

	private:
		std::string IntToStr(int32_t number);
		void ExpandNumbers(std::vector<std::string>& OutTokens, const std::vector<std::string>& SpaceTokens);
		void Tokenize(std::vector<std::string>& OutTokens, const std::string& InTxt);
	};
};