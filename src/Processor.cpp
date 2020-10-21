#include "PCH.h"

namespace ftts
{
	Processor::Processor()
	{}

	Processor::~Processor()
	{
		for (auto& pair : encodings_)
			iconv_close(pair.second);
		encodings_.clear();
	}

	void Processor::ToUTF8(std::string& out, const char* text, const char* enc /*= nullptr*/)
	{
		if (enc == nullptr)
		{
			out = text;
			return;
		}

		size_t length = std::strlen(text);
		size_t bufsize(256);
		out.resize(bufsize);

		const char* utf8 = "UTF-8";
		bool is_utf8 = std::equal(enc, enc + std::strlen(enc), utf8, utf8 + std::strlen(utf8), [](char a, char b)
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

	void Processor::Replace(std::string& message, std::string& pattern, std::string& replace)
	{
		std::string::size_type pos = 0;
		std::string::size_type offset = 0;
		while ((pos = message.find(pattern, offset)) != std::string::npos)
		{
			message.replace(message.begin() + pos, message.begin() + pos + pattern.size(), replace);
			offset = pos + replace.size();
		}
	}

}