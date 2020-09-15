#include "ftts.h"
#define LIBICONV_STATIC
#define UTF8PROC_STATIC
#define MECAB_STATIC
#include <unordered_map>
#include <cassert>
#include <iconv.h>
#include <utf8proc.h>
#include <mecab.h>
#include <string>
#include <cstring>
#include <list>
#include <algorithm>
#include <iostream>
#include <cctype>
#include <sstream>

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
		Processor()
		{}

		virtual ~Processor()
		{
			for (auto& pair : encodings_)
				iconv_close(pair.second);
			encodings_.clear();
		}

		void ToUTF8(std::string& out, const char* text, const char* enc = nullptr)
		{
			size_t length = std::strlen(text);

			size_t bufsize(256);
			out.resize(bufsize);

			const char* utf8 = "UTF-8";
			bool is_utf8 = std::equal(enc, enc+std::strlen(enc), utf8, utf8+std::strlen(utf8), [](char a, char b)
			{ 
				return std::tolower(a) == std::tolower(b);
			});

			if (std::strlen(enc) == 0 || is_utf8)
			{
				out = text;
			}
			else
			{
				// to utf8
				size_t hash = std::hash<const char*>{}(enc);
				iconv_t cd(nullptr);
				auto it = encodings_.find(hash);
				if (it == encodings_.end())
				{
					cd = iconv_open("UTF-8", enc);
					encodings_.emplace(std::make_pair(hash, cd));
				}
				else
				{
					cd = it->second;
				}

				char* inptr = const_cast<char*>(text);
				size_t insize = length;
				size_t pos = 0;
				while (0 < insize)
				{
					char* outptr = &out[pos];
					size_t outsize = bufsize;

					size_t res = ::iconv(cd, (const char**)&inptr, &insize, &outptr, &outsize);
					if (res == (size_t)-1)
					{
						if (errno == E2BIG)
						{
							// ignore
						}
						else
						{
							std::cerr << "Failed Convert Encoding: " << errno << std::endl;
						}
					}
					size_t length = bufsize - outsize;
					pos += length;
					bufsize -= (length - insize);
					out.append(outptr);
					out.resize(pos + insize);			// fit
					assert(insize == 0);
				}
			}
		}
		
		void Replace(std::string& message, std::string& pattern, std::string& replace) 
		{
			std::string::size_type pos = 0;
			std::string::size_type offset = 0;
			while ((pos = message.find(pattern, offset)) != std::string::npos)
			{
				message.replace(message.begin() + pos, message.begin() + pos + pattern.size(), replace);
				offset = pos + replace.size();
			}
		}
	};

#define b1(a) {(char)a}
#define b2(a, b) {(char)a, (char)b}
#define b3(a, b, c) {(char)a, (char)b, (char)c}
	class JSProcessor : public Processor
	{
	private:
		using Symbols = std::unordered_map<int32_t, int32_t>;
		int32_t eos_;
		Symbols symbols_;
		MeCab::Tagger* tagger_;
		std::string empty_;
		std::vector<std::string> punctuation_;
		std::vector<std::string> cleaner_;
		std::vector<std::pair<std::string, std::string>> normalize_;	
		
	public:
		JSProcessor(const char* args)
			: eos_(0xffffffff)
			, tagger_(MeCab::createTagger(args))
			, empty_("")
			, punctuation_
			{ 
				b1(0x2e), // "."
				b1(0x2c), // ","
				b3(0xe3,0x80,0x81), // "、"
				b3(0xe3,0x80,0x82), // "。"
				b3(0xef,0xbc,0x81), // "！"
				b3(0xef,0xbc,0x9f), // "？"
				b1(0x21), // "!"
				b1(0x3f) // "?"
			}	
			, cleaner_	
			{ 
				b1(0x20),  // " "
				b3(0xe3,0x80,0x80), // "　"
				b3(0xe3,0x80,0x8c), // "「"
				b3(0xe3,0x80,0x8d), // "」"
				b3(0xe3,0x80,0x8e), // "『"
				b3(0xe3,0x80,0x8f), // "』"
				b1(0x3f), // "・"
				b3(0xe3,0x80,0x90), // "【"
				b3(0xe3,0x80,0x91), // "】"
				b3(0xef,0xbc,0x88), // "（"
				b3(0xef,0xbc,0x89), // "）"
				b1(0x28), // "("
				b1(0x29) // ")"
			}
			, normalize_
			{
				{b3(0xe3,0x80,0x9c), b3(0xe3,0x83,0xbc)}, // replace('〜', 'ー')
				{b3(0xef,0xbd,0x9e), b3(0xe3,0x83,0xbc)}, // replace('～', 'ー')
				{b3(0xe2,0x80,0x99), b1(0x27)}, // replace("’", "'")
				{b3(0xe2,0x80,0x9d), b1(0x22)}, // replace('”', '"')
				{b3(0xe2,0x80,0x9c), b2(0x60,0x60)}, // replace('“', '``')
				{b2(0xcb,0x97), b1(0x2d)}, // replace('˗', '-')
				{b2(0xd6,0x8a), b1(0x2d)}, // replace('֊', '-')
				{b3(0xe2,0x80,0x90), b1(0x2d)}, // replace('‐', '-')
				{b3(0xe2,0x80,0x91), b1(0x2d)}, // replace('‑', '-')
				{b3(0xe2,0x80,0x92), b1(0x2d)}, // replace('‒', '-')
				{b3(0xe2,0x80,0x93), b1(0x2d)}, // replace('–', '-')
				{b3(0xe2,0x81,0x83), b1(0x2d)}, // replace('⁃', '-')
				{b3(0xe2,0x81,0xbb), b1(0x2d)}, // replace('⁻', '-')
				{b3(0xe2,0x82,0x8b), b1(0x2d)}, // replace('₋', '-')
				{b3(0xe2,0x88,0x92), b1(0x2d)}, // replace('−', '-')
				{b3(0xef,0xb9,0xa3), b3(0xe3,0x83,0xbc)}, // replace('﹣', 'ー')
				{b3(0xef,0xbc,0x8d), b3(0xe3,0x83,0xbc)}, // replace('－', 'ー')
				{b3(0xe2,0x80,0x94), b3(0xe3,0x83,0xbc)}, // replace('—', 'ー')
				{b3(0xe2,0x80,0x95), b3(0xe3,0x83,0xbc)}, // replace('―', 'ー')
				{b3(0xe2,0x94,0x81), b3(0xe3,0x83,0xbc)}, // replace('━', 'ー')
				{b3(0xe2,0x94,0x80), b3(0xe3,0x83,0xbc)}, // replace('─', 'ー')
				{b1(0x2c), b3(0xe3,0x80,0x81)}, // replace(',', '、')
				{b1(0x2e), b3(0xe3,0x80,0x82)}, // replace('.', '。')
				{b3(0xef,0xbc,0x8c), b3(0xe3,0x80,0x81)}, // replace('，', '、')
				{b3(0xef,0xbc,0x8e), b3(0xe3,0x80,0x82)}, // replace('．', '。')
				{b1(0x21), b3(0xef,0xbc,0x81)}, // replace('!', '！')
				{b1(0x3f), b3(0xef,0xbc,0x81)}	// replace('?', '？')
			}
		{
			// generate symbols (UTF32)

			// pad 
			int32_t seq(1);

			for (size_t i = 0; i < punctuation_.size(); i++, seq++)
			{
				const utf8proc_uint8_t* pstring = reinterpret_cast<const utf8proc_uint8_t*>(&punctuation_[i].front());
				utf8proc_ssize_t size = punctuation_[i].size();
				utf8proc_int32_t cp = 0;
				for (utf8proc_ssize_t n = 0; (n = utf8proc_iterate(pstring, size, &cp)) > 0; pstring += n, size -= n)
					symbols_.emplace(std::make_pair(cp, seq));
			}
			
			// katakana ( U+30A0..U+30FF ), hiragana ( U+3040..U+309F )
			for (int32_t cp = 0x30A0; cp <= 0x30FF; cp++, seq++)
				symbols_.emplace(std::make_pair(cp, seq));

			// alphabet
			for (int32_t cp = 0x0041; cp <= 0x007a; cp++, seq++)
				symbols_.emplace(std::make_pair(cp, seq));

			// numbers
			for (int32_t cp = 0x0030; cp <= 0x0039; cp++, seq++)
				symbols_.emplace(std::make_pair(cp, seq));

			// eos
			eos_ = seq;	
		}

		~JSProcessor()
		{
			if (tagger_ != nullptr)
				MeCab::deleteTagger(tagger_);
		}

		void ToSeq(const char* text, std::vector<int32_t>& seq, const char* enc) override
		{
			std::string utf8;
			ToUTF8(utf8, text, enc);
			
			// cleaner
			for (auto& c : cleaner_)
				Replace(utf8, c, empty_);
			
			// normalize
			for (auto& pair : normalize_)
				Replace(utf8, pair.first, pair.second);

			utf8proc_uint8_t* buffer = utf8proc_NFKC((const utf8proc_uint8_t*)utf8.c_str());
			utf8.assign((const char*)buffer);
			free(buffer);

			// pronunciation
			const char* result = tagger_->parse(utf8.c_str(), utf8.size());
			if (result)
			{
				utf8.clear();

				std::istringstream input(result);
				std::string line;

				while (std::getline(input, line))
				{
					std::istringstream row(line);
					std::string column[2];

					size_t count;
					for (count = 0; count < 2 && std::getline(row, column[count], '\t'); ++count) {}
					//std::cout << line << std::endl;

					if (count == 2)
					{
						// keep punctuation
						if (0)
						{
							bool found(false);
							for (auto& p : punctuation_)
							{
								if (!column[0].compare(p.c_str()))
								{
									found = true;
									break;
								}
							}
							utf8.append(found ? column[0] : column[1]);
						}
						else
						{
							utf8.append(column[1].empty() ? column[0] : column[1]);
						}
					}
				}
			}

			// for test
			std::cout << utf8 << std::endl;

			seq.clear();

			const utf8proc_uint8_t* pstring = reinterpret_cast<const utf8proc_uint8_t*>(utf8.c_str());
			utf8proc_ssize_t size = utf8.size();
			utf8proc_int32_t cp = 0;
			for (utf8proc_ssize_t n = 0; (n = utf8proc_iterate(pstring, size, &cp)) > 0; pstring += n, size -= n)
			{
				auto it = symbols_.find(cp);
				if (it == symbols_.end())
				{
					std::cerr << "That code is not in the symbol => " << cp << std::endl;
					continue;
				}
				seq.emplace_back(it->second);
			}

			seq.emplace_back(eos_);
		}
	};


	IProcessor::Ptr IProcessor::Create(const char* args, const char* processor)
	{
		return std::make_shared<JSProcessor>(args);
	}
}