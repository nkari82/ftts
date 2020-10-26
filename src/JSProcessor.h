#pragma once


namespace ftts
{
	class JSProcessor : public Processor
	{
	private:
		using Symbols = std::unordered_map<int32_t, int32_t>;
		std::locale locale_;
		int32_t eos_;
		Symbols symbols_;
		MeCab::Tagger* tagger_;
		std::string empty_;
		std::vector<std::string> punctuation_;
		std::vector<std::string> cleaner_;
		std::vector<std::pair<std::string, std::string>> normalize_;

	public:
		JSProcessor(const std::vector<std::string>& args);

		virtual ~JSProcessor();

		void ToSeq(const char* text, std::vector<int32_t>& seq, const char* enc) override;
	};
}