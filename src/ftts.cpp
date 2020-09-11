#include "ftts.h"
#define LIBICONV_STATIC
#define UTF8PROC_STATIC
#include <unordered_map>
#include <cassert>
#include <uchardet.h>
#include <iconv.h>
#include <utf8proc.h>

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
		char*			charset_;
		uchardet*		ud_;

	public:
		Processor() : charset_(nullptr)
		{
			ud_ = uchardet_new();
			if (charset_ != nullptr)
				free(charset_);
		}

		virtual ~Processor()
		{
			uchardet_delete(ud_);
		}

		const char* Detect(const char* text, size_t length)
		{
			if (charset_ != nullptr)
				free(charset_);

			uchardet_reset(ud_);
			int ret = uchardet_handle_data(ud_, text, length);
			uchardet_data_end(ud_);
			charset_ = (char*)uchardet_get_charset(ud_);
			return charset_;
		}

		void ConvertUTF8(std::string& out, const char* text, const char* enc = nullptr)
		{
			size_t length = std::strlen(text);
			if( enc == nullptr)
				enc = Detect(text, length);

			// to unicode
			size_t hash = std::hash<const char*>{}(enc);
			iconv_t cd(nullptr);
			auto it = encodings_.find(hash);
			if (it == encodings_.end())
			{
				cd = iconv_open("utf-8", enc);
				encodings_.emplace(std::make_pair(hash, cd));
			}

			size_t buf_size(256);
			out.clear();
			out.resize(buf_size);

			char* src_ptr = const_cast<char*>(text);
			size_t src_size = length;
			size_t pos = 0;
			while (0 < src_size)
			{
				char* dst_ptr = &out[pos];
				size_t dst_size = buf_size;

				size_t res = ::iconv(cd, &src_ptr, &src_size, &dst_ptr, &dst_size);
				if (res == (size_t)-1)
				{
					if (errno == E2BIG)
					{
						// ignore
					}
					else
					{
						fprintf(stderr, "Failed Convert Encoding: {}", errno);
					}
				}
				size_t length = buf_size - dst_size;
				pos += length;
				buf_size -= (length - src_size);
				out.append(dst_ptr);
				out.resize(pos + src_size);			// fit

				assert(src_size == 0);
			}
		}
	};

	class JSProcessor : public Processor
	{
	private:
		std::string pad_;
		std::string eos_;
		std::string punctuation_;
		std::string cleaner_;

	public:
		JSProcessor() 
			: pad_("pad")
			, eos_("eos")
		{
			int32_t seq(0);

			// for katakana
			int32_t cp = 0x30A0;
			for (; cp <= 0x30FF; cp++, seq++)
				symbols_.emplace(std::make_pair(cp, seq));
		}

		void ToSeq(const char* text, std::vector<int32_t>& seq) override
		{
			std::string out;
			ConvertUTF8(out, text);

			utf8proc_int32_t cp = 0;
			const utf8proc_uint8_t* pstring = reinterpret_cast<const utf8proc_uint8_t*>(&out.front());
			const utf8proc_uint8_t* pend = reinterpret_cast<const utf8proc_uint8_t*>(pstring + out.size());
			utf8proc_ssize_t size = pend - pstring;
			utf8proc_ssize_t n(0);
			while ((n = utf8proc_iterate(pstring, size, &cp)) > 0)
			{
				seq.emplace_back(cp);
				pstring += n;
				size -= n;
			}

			//30DE
			int temp = 0;
		}
	};


	IProcessor::Ptr IProcessor::Create(const char* processor)
	{
		return std::make_shared<JSProcessor>();
	}
}