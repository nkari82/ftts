#include "ftts.h"
#define LIBICONV_STATIC
#define UTF8PROC_STATIC
#define MECAB_STATIC
#include <unordered_map>
#include <cassert>
#include <uchardet.h>
#include <iconv.h>
#include <utf8proc.h>
#include <mecab.h>
#include <cstring>
#include <list>
#include <algorithm>
#include <iostream>

namespace ftts
{
	static utf8proc_option_t NFKC = utf8proc_option_t(0
			|UTF8PROC_NULLTERM
			|UTF8PROC_STABLE
			|UTF8PROC_COMPOSE
			|UTF8PROC_COMPAT
			|UTF8PROC_CASEFOLD
			|UTF8PROC_IGNORE);

	// https://unicodelookup.com/
	using UString = std::basic_string<int32_t>;
	

	class Processor : public IProcessor
	{
	protected:
		using Symbols = std::unordered_map<size_t, int32_t>;
		Symbols		symbols_;

	private:
		using Encodings = std::unordered_map<size_t, iconv_t>;
		Encodings	encodings_;
		uchardet*		ud_;

	public:
		Processor()
		{
			ud_ = uchardet_new();
		}
		virtual ~Processor()
		{
			uchardet_delete(ud_);
			for (auto& pair : encodings_)
				iconv_close(pair.second);
			encodings_.clear();
		}

		void ToUnicode(UString& out, const char* text, const char* enc = nullptr)
		{
			size_t length = std::strlen(text);

			if( enc == nullptr)
				enc = Detect(text, length);

			size_t bufsize(256);
			std::string utf8;
			utf8.resize(bufsize);

			//std::stricmp()

			if (std::strlen(enc) == 0 || !std::strcmp(enc, "UTF-8"))
			{
				utf8 = text;
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
					char* outptr = &utf8[pos];
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
					utf8.append(outptr);
					utf8.resize(pos + insize);			// fit
					assert(insize == 0);
				}
			}

			utf8proc_int32_t cp = 0;
			const utf8proc_uint8_t* pstring = reinterpret_cast<const utf8proc_uint8_t*>(&utf8.front());
			utf8proc_ssize_t size = utf8.size();
			utf8proc_ssize_t n(0);
			while ((n = utf8proc_iterate(pstring, size, &cp)) > 0)
			{
				out.push_back(cp);
				pstring += n;
				size -= n;
			}
		}
		
		void Replace(UString& message, UString& pattern, UString& replace) 
		{
			UString::size_type pos = 0;
			UString::size_type offset = 0;
			while ((pos = message.find(pattern, offset)) != UString::npos)
			{
				message.replace(message.begin() + pos, message.begin() + pos + pattern.size(), replace);
				offset = pos + replace.size();
			}
		}
	
	private:
		const char* Detect(const char* text, size_t length)
		{
			uchardet_reset(ud_);
			int ret = uchardet_handle_data(ud_, text, length);
			uchardet_data_end(ud_);
			return (char*)uchardet_get_charset(ud_);
		}
	};

	class JSProcessor : public Processor
	{
	private:
		int32_t eos_;

		MeCab::Tagger* tagger_;
		std::vector<int32_t> punctuation_;
		std::vector<int32_t> cleaner_;
		std::vector<std::pair<UString, UString>> normalize_;
		
	public:
		JSProcessor(const char* args)
			: eos_(0xffffffff)
			, tagger_(MeCab::createTagger(args))
			, punctuation_{ 0x2e,0x2c,0x3001,0x3002,0xff01,0xff1f,0x21,0x3f }											// ".,、。！？!?"
			, cleaner_{ 0x20,0x3000,0x300c,0x300d,0x300e,0x300f,0x3f,0x3010,0x3011,0xff08,0xff09,0x28,0x29 }	// " 　「」『』・【】（）()"
			, normalize_{
				{{0x301C}, {0x30FC}},				// replace('〜', 'ー')
				{{0xFF5E}, {0x30FC}},				// replace('～', 'ー')
				{{0x2019}, {0x27}},				// replace("’", "'")
				{{0x201D}, {0x22}},				// replace('”', '"')
				{{0x201C}, {0x60, 0x60}},			// replace('“', '``')
				{{0x2D7}, {0x2D}},				// replace('˗', '-')
				{{0x58A}, {0x2D}},				// replace('֊', '-')
				{{0x2010}, {0x2D}},				// replace('‐', '-')
				{{0x2011}, {0x2D}},				// replace('‑', '-')
				{{0x2012}, {0x2D}},				// replace('‒', '-')
				{{0x2013}, {0x2D}},				// replace('–', '-')
				{{0x2043}, {0x2D}},				// replace('⁃', '-')
				{{0x207B}, {0x2D}},				// replace('⁻', '-')
				{{0x208B}, {0x2D}},				// replace('₋', '-')
				{{0x2212}, {0x2D}},				// replace('−', '-')
				{{0xFE63}, {0x30FC}},				// replace('﹣', 'ー')
				{{0xFF0D}, {0x30FC}},				// replace('－', 'ー')
				{{0x2014}, {0x30FC}},				// replace('—', 'ー')
				{{0x2015}, {0x30FC}},				// replace('―', 'ー')
				{{0x2501}, {0x30FC}},				// replace('━', 'ー')
				{{0x2500}, {0x30FC}},				// replace('─', 'ー')
				{{0x2C}, {0x3001}},				// replace(',', '、')
				{{0x2E}, {0x3002}},				// replace('.', '。')
				{{0xFF0C}, {0x3001}},				// replace('，', '、')
				{{0xFF0E}, {0x3002}},				// replace('．', '。')
				{{0x21}, {0xFF01}},				// replace('!', '！')
				{{0x3F}, {0xFF01}}}				// replace('?', '？	')
		{
			// pad 
			int32_t seq(1);

			for (size_t i = 0; i < punctuation_.size(); i++, seq++)
				symbols_.emplace(std::make_pair(punctuation_[i], seq));
			
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
			UString unicode;
			ToUnicode(unicode, text, enc);
			
			// cleaner
			for (auto& c : cleaner_)
				std::remove(unicode.begin(), unicode.end(), c);
			
			// normalize
			for (auto& pair : normalize_)
				Replace(unicode, pair.first, pair.second);

			size_t length = utf8proc_normalize_utf32((utf8proc_int32_t*)unicode.c_str(), unicode.length(), NFKC);
			unicode.resize(length);
		}
	};


	IProcessor::Ptr IProcessor::Create(const char* args, const char* processor)
	{
		return std::make_shared<JSProcessor>(args);
	}
}