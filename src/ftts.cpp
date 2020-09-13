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

namespace ftts
{
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

		void ToUnicode(std::vector<int32_t>& out, const char* text, const char* enc = nullptr)
		{
			size_t length = std::strlen(text);

			if( enc == nullptr)
				enc = Detect(text, length);

			size_t bufsize(256);
			std::string utf8;
			utf8.resize(bufsize);

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
							fprintf(stderr, "Failed Convert Encoding: %d", errno);
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
				out.emplace_back(cp);
				pstring += n;
				size -= n;
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
		MeCab::Tagger* tagger_;

	public:
		JSProcessor(const char* args) 
			: tagger_(MeCab::createTagger(args))
		{
			int32_t seq(0);
			seq++; // pad

			// for katakana  ( U+30A0..U+30FF (96 code points) )
			int32_t cp = 0x30A0;
			for (; cp <= 0x30FF; cp++, seq++)
				symbols_.emplace(std::make_pair(cp, seq));

			// for hiragana ( U+3040..U+309F )
		}

		~JSProcessor()
		{
			if (tagger_ != nullptr)
				MeCab::deleteTagger(tagger_);
		}

		void ToSeq(const char* text, std::vector<int32_t>& seq, const char* enc) override
		{
			std::vector<int32_t> unicode;
			unicode.reserve(256);
			ToUnicode(unicode, text, enc);

			int temp = 0;
		}
	};


	IProcessor::Ptr IProcessor::Create(const char* args, const char* processor)
	{
		return std::make_shared<JSProcessor>(args);
	}
}