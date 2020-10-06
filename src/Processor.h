#pragma once

namespace ftts
{
	// https://mothereff.in/utf-8
	// https://onlineutf8tools.com/convert-utf32-to-utf8
	// https://unicodelookup.com/

	class Processor : public IProcessor
	{
	private:
		using Encodings = std::unordered_map<size_t, iconv_t>;
		Encodings encodings_;

	public:
		Processor();
		virtual ~Processor();

		void ToUTF8(std::string& out, const char* text, const char* enc = nullptr);
		void Replace(std::string& message, std::string& pattern, std::string& replace);
	};
}
