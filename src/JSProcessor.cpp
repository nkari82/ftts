#include "PCH.h"

namespace ftts
{
	// https://apps.nolanlawson.com/japanese-name-converter/#?q=Ki
	static std::vector<std::pair<std::string, std::string>> e2k =
	{
		{"Ki", "\xE3\x82\xAD"},		// キ
		{"jung", "\xE3\x82\xB8\xE3\x83\xA5\xE3\x83\xB3\xE3\x82\xB0"}, // ジュング
		{"Kim", "\xE3\x82\xAD\xE3\x83\xA0"}, // キム
		{"taek", "\xE3\x82\xBF\xE3\x82\xA8\xE3\x82\xAF"}, // タエク
		{"woo", "\xE3\x82\xA6\xE3\x83\xBC"}, // ウー
		{"WiFi", "\xE3\x82\xA6\xE3\x82\xA3\xE3\x83\x95\xE3\x82\xA3"}, // ウィフィ
		{"coffee", "\xE3\x82\xB3\xE3\x83\x95\xE3\x82\xA3\xE3\x83\xBC"},		// コフィー
		{"land", "\xE3\x83\xA9\xE3\x83\xB3\xE3\x83\x89"},	// ランド 
		{"Whats", "\xE3\x82\xA6\xE3\x82\xA9\xE3\x83\x84"}, // ウォツ
		{"App", "\xE3\x82\xA8\xE3\x83\xBC\xE3\x83\x83\xE3\x83\x83\xE3\x83\x97\x20"}, // エーッップ 
		{"IPTIME", "\xE3\x82\xA4\xE3\x83\x97\xE3\x83\x86\xE3\x82\xA3\xE3\x83\x9F\xE3\x83\xBC"}, // イプティミー
		{"PARASITE", "\xE3\x83\x91\xE3\x83\xA9\xE3\x82\xB7\xE3\x83\x88"}, // パラシト
	};

	static std::vector <std::pair<std::string, std::string>> h2k =
	{
		{"\xE3\x81\x80", "\xE3\x82\xA0"},	// ぀
		{"\xE3\x81\x81", "\xE3\x82\xA1"}, 	// ぁ
		{"\xE3\x81\x82", "\xE3\x82\xA2"}, 	// あ
		{"\xE3\x81\x83", "\xE3\x82\xA3"}, 	// ぃ
		{"\xE3\x81\x84", "\xE3\x82\xA4"}, 	// い
		{"\xE3\x81\x85", "\xE3\x82\xA5"}, 	// ぅ
		{"\xE3\x81\x86", "\xE3\x82\xA6"}, 	// う
		{"\xE3\x81\x87", "\xE3\x82\xA7"}, 	// ぇ
		{"\xE3\x81\x88", "\xE3\x82\xA8"}, 	// え
		{"\xE3\x81\x89", "\xE3\x82\xA9"}, 	// ぉ
		{"\xE3\x81\x8a", "\xE3\x82\xAa"}, 	// お
		{"\xE3\x81\x8b", "\xE3\x82\xAb"}, 	// か
		{"\xE3\x81\x8c", "\xE3\x82\xAc"}, 	// が
		{"\xE3\x81\x8d", "\xE3\x82\xAd"}, 	// き
		{"\xE3\x81\x8e", "\xE3\x82\xAe"}, 	// ぎ
		{"\xE3\x81\x8f", "\xE3\x82\xAf"}, 	// く
		{"\xE3\x81\x90", "\xE3\x82\xB0"},
		{"\xE3\x81\x91", "\xE3\x82\xB1"},
		{"\xE3\x81\x92", "\xE3\x82\xB2"},
		{"\xE3\x81\x93", "\xE3\x82\xB3"},
		{"\xE3\x81\x94", "\xE3\x82\xB4"},
		{"\xE3\x81\x95", "\xE3\x82\xB5"},
		{"\xE3\x81\x96", "\xE3\x82\xB6"},
		{"\xE3\x81\x97", "\xE3\x82\xB7"},
		{"\xE3\x81\x98", "\xE3\x82\xB8"},
		{"\xE3\x81\x99", "\xE3\x82\xB9"},
		{"\xE3\x81\x9A", "\xE3\x82\xBa"},
		{"\xE3\x81\x9B", "\xE3\x82\xBb"},	// せ
		{"\xE3\x81\x9C", "\xE3\x82\xBC"},
		{"\xE3\x81\x9D", "\xE3\x82\xBd"},
		{"\xE3\x81\x9E", "\xE3\x82\xBE"},
		{"\xE3\x81\x9F", "\xE3\x82\xBF"},		// た
		{"\xE3\x81\xA0", "\xE3\x83\x80"},
		{"\xE3\x81\xA1", "\xE3\x83\x81"},
		{"\xE3\x81\xA2", "\xE3\x83\x82"},
		{"\xE3\x81\xA3", "\xE3\x83\x83"},
		{"\xE3\x81\xA4", "\xE3\x83\x84"},
		{"\xE3\x81\xA5", "\xE3\x83\x85"},
		{"\xE3\x81\xA6", "\xE3\x83\x86"},
		{"\xE3\x81\xA7", "\xE3\x83\x87"},
		{"\xE3\x81\xA8", "\xE3\x83\x88"},
		{"\xE3\x81\xA9", "\xE3\x83\x89"},
		{"\xE3\x81\xAa", "\xE3\x83\x8A"},
		{"\xE3\x81\xAb", "\xE3\x83\x8B"},
		{"\xE3\x81\xAc", "\xE3\x83\x8C"},
		{"\xE3\x81\xAd", "\xE3\x83\x8D"},
		{"\xE3\x81\xAe", "\xE3\x83\x8E"},
		{"\xE3\x81\xAf", "\xE3\x83\x8F"}, 	// は
		{"\xE3\x81\xB0", "\xE3\x83\x90"},	// ば
		{"\xE3\x81\xB1", "\xE3\x83\x91"},
		{"\xE3\x81\xB2", "\xE3\x83\x92"},
		{"\xE3\x81\xB3", "\xE3\x83\x93"},
		{"\xE3\x81\xB4", "\xE3\x83\x94"},
		{"\xE3\x81\xB5", "\xE3\x83\x95"},
		{"\xE3\x81\xB6", "\xE3\x83\x96"},
		{"\xE3\x81\xB7", "\xE3\x83\x97"},
		{"\xE3\x81\xB8", "\xE3\x83\x98"},
		{"\xE3\x81\xB9", "\xE3\x83\x99"},
		{"\xE3\x81\xBa", "\xE3\x83\x9a"},
		{"\xE3\x81\xBb", "\xE3\x83\x9b"},
		{"\xE3\x81\xBc", "\xE3\x83\x9c"},
		{"\xE3\x81\xBd", "\xE3\x83\x9d"},
		{"\xE3\x81\xBe", "\xE3\x83\x9e"},
		{"\xE3\x81\xBf", "\xE3\x83\x9F"}, 		// み
		{"\xE3\x82\x80", "\xE3\x83\xA0"},
		{"\xE3\x82\x81", "\xE3\x83\xA1"},
		{"\xE3\x82\x82", "\xE3\x83\xA2"},
		{"\xE3\x82\x83", "\xE3\x83\xA3"},
		{"\xE3\x82\x84", "\xE3\x83\xA4"},
		{"\xE3\x82\x85", "\xE3\x83\xA5"},
		{"\xE3\x82\x86", "\xE3\x83\xA6"},
		{"\xE3\x82\x87", "\xE3\x83\xA7"},
		{"\xE3\x82\x88", "\xE3\x83\xA8"},
		{"\xE3\x82\x89", "\xE3\x83\xA9"},
		{"\xE3\x82\x8a", "\xE3\x83\xAa"},
		{"\xE3\x82\x8b", "\xE3\x83\xAb"},
		{"\xE3\x82\x8c", "\xE3\x83\xAc"},
		{"\xE3\x82\x8d", "\xE3\x83\xAd"},
		{"\xE3\x82\x8e", "\xE3\x83\xAe"},
		{"\xE3\x82\x8f", "\xE3\x83\xAf"}, 	// わ
		{"\xE3\x82\x90", "\xE3\x83\xB0"},
		{"\xE3\x82\x91", "\xE3\x83\xB1"},
		{"\xE3\x82\x92", "\xE3\x83\xB2"},
		{"\xE3\x82\x93", "\xE3\x83\xB3"},
		{"\xE3\x82\x94", "\xE3\x83\xB4"},
		{"\xE3\x82\x95", "\xE3\x83\xB5"},
		{"\xE3\x82\x96", "\xE3\x83\xB6"},
		{"\xE3\x82\x97", "\xE3\x83\xB7"},
		{"\xE3\x82\x98", "\xE3\x83\xB8"},
		{"\xE3\x82\x99", "\xE3\x83\xB9"},
		{"\xE3\x82\x9a", "\xE3\x83\xBa"},
		{"\xE3\x82\x9b", "\xE3\x83\xBb"},
		{"\xE3\x82\x9c", "\xE3\x83\xBC"},
		{"\xE3\x82\x9d", "\xE3\x83\xBD"},
		{"\xE3\x82\x9e", "\xE3\x83\xBE"},
		{"\xE3\x82\x9f", "\xE3\x83\xBF"}		// ゟ
	};

	JSProcessor::JSProcessor(const std::vector<std::string>& args)
		: locale_("ja_JP.UTF-8")
		, eos_(0xffffffff)
		, tagger_(MeCab::createTagger(args[0].c_str()))
		, empty_("")
		, punctuation_
	{
		"\x2d", // "-"
		"\x2c", // ","
		"\xe3\x80\x81", // "、"
		"\xe3\x80\x82", // "。"
		"\xef\xbc\x81", // "！"
		"\xef\xbc\x9f", // "？"
		"\x21", // "!"
		"\x3f"  // "?"
	}
		, cleaner_
	{
		"\x28", // "("
		"\x29", // ")"
		"\x5B", // [
		"\x5D", // ]
		"\x2F", // /
		"\x22", // "
		"\xe3\x80\x8c", // "「"
		"\xe3\x80\x8d", // "」"
		"\xe3\x80\x8e", // "『"
		"\xe3\x80\x8f", // "』"
		"\xe3\x83\xbb", // "・"
		"\xe3\x80\x90", // "【"
		"\xe3\x80\x91", // "】"
		"\xef\xbc\x88", // "（"
		"\xef\xbc\x89" // "）"
	}
		, normalize_
	{
		{"\xe3\x80\x9c", "\xe3\x83\xbc"}, // replace('〜', 'ー')
		{"\xef\xbd\x9e", "\xe3\x83\xbc"}, // replace('～', 'ー')
		{"\xe2\x80\x99", "\x27"}, // replace("’", "'")
		{"\xe2\x80\x9d", "\x22"}, // replace('”', '"')
		{"\xe2\x80\x9c", "\x60\x60"}, // replace('“', '``')
		{"\xcb\x97", "\x2d"}, // replace('˗', '-')
		{"\xd6\x8a", "\x2d"}, // replace('֊', '-')
		{"\xe2\x80\x90", "\x2d"}, // replace('‐', '-')
		{"\xe2\x80\x91", "\x2d"}, // replace('‑', '-')
		{"\xe2\x80\x92", "\x2d"}, // replace('‒', '-')
		{"\xe2\x80\x93", "\x2d"}, // replace('–', '-')
		{"\xe2\x81\x83", "\x2d"}, // replace('⁃', '-')
		{"\xe2\x81\xbb", "\x2d"}, // replace('⁻', '-')
		{"\xe2\x82\x8b", "\x2d"}, // replace('₋', '-')
		{"\xe2\x88\x92", "\x2d"}, // replace('−', '-')
		{"\xef\xb9\xa3", "\xe3\x83\xbc"}, // replace('﹣', 'ー')
		{"\xef\xbc\x8d", "\xe3\x83\xbc"}, // replace('－', 'ー')
		{"\xe2\x80\x94", "\xe3\x83\xbc"}, // replace('—', 'ー')
		{"\xe2\x80\x95", "\xe3\x83\xbc"}, // replace('―', 'ー')
		{"\xe2\x94\x81", "\xe3\x83\xbc"}, // replace('━', 'ー')
		{"\xe2\x94\x80", "\xe3\x83\xbc"}, // replace('─', 'ー')
		{"\x2c", "\xe3\x80\x81"}, // replace(',', '、')
		{"\x2e", "\xe3\x80\x82"}, // replace('.', '。')
		{"\xef\xbc\x8c", "\xe3\x80\x81"}, // replace('，', '、')
		{"\xef\xbc\x8e", "\xE3\x80\x82"}, // replace('．', '。')
		{"\x21", "\xEF\xBC\x81"}, // replace('!', '！')
		{"\x3F", "\xEF\xBC\x9F"}	// replace('?', '？')
	}
	{
		// generate symbols (UTF32)

		// pad 
		int32_t seq(1);
		for (size_t i = 0; i < punctuation_.size(); i++)
		{
			const utf8proc_uint8_t* pstring = reinterpret_cast<const utf8proc_uint8_t*>(&punctuation_[i].front());
			utf8proc_ssize_t size = punctuation_[i].size();
			utf8proc_int32_t cp = 0;
			for (utf8proc_ssize_t n = 0; (n = utf8proc_iterate(pstring, size, &cp)) > 0; pstring += n, size -= n)
			{
				assert(symbols_.find(cp) == symbols_.end());
				symbols_.emplace(std::make_pair(cp, seq++));		// 8
			}
		}

		// https://jrgraphix.net/r/Unicode/30A0-30FF
		// katakana ( U+30A0..U+30FF ), hiragana ( U+3040..U+309F )
		for (int32_t cp = 0x30A0; cp < 0x30FF; cp++)
		{
			assert(symbols_.find(cp) == symbols_.end());
			symbols_.emplace(std::make_pair(cp, seq++));		// 95
		}

		// alphabet
		for (int32_t cp = 0x0041; cp < 0x007b; cp++)
		{
			if (cp > 0x5a && cp < 0x61) continue;
			assert(symbols_.find(cp) == symbols_.end());
			symbols_.emplace(std::make_pair(cp, seq++));		// 52
		}

		// numbers
		for (int32_t cp = 0x30; cp < 0x3a; cp++)
		{
			assert(symbols_.find(cp) == symbols_.end());
			symbols_.emplace(std::make_pair(cp, seq++));		// 10
		}

		// eos
		eos_ = seq;
	}

	JSProcessor::~JSProcessor()
	{
		if (tagger_ != nullptr)
			MeCab::deleteTagger(tagger_);
	}

	void JSProcessor::ToSeq(const char* text, std::vector<int32_t>& seq, const char* enc)
	{
		std::string utf8;
		ToUTF8(utf8, text, enc);

		// cleaner
		for (auto& c : cleaner_)
			Replace(utf8, c, empty_);

		std::cout << utf8 << std::endl;

		// normalize
		for (auto& pair : normalize_)
			Replace(utf8, pair.first, pair.second);

		utf8proc_uint8_t* buffer = utf8proc_NFKC((const utf8proc_uint8_t*)utf8.c_str());
		if (buffer != nullptr)
		{
			utf8.assign((const char*)buffer);
			free(buffer);
		}

		// tokenizer
		std::vector<std::string> tokens;
		std::string token;
		for (auto& c : utf8)
		{
			bool isspace = std::isspace(c, locale_);
			bool isalpha = std::isalpha(c, locale_);
			bool isdigit = std::isdigit(c, locale_);
			bool ispunct = std::ispunct(c, locale_);
			char p = token.empty() ? c : token.back();

			if ( (isalpha == !std::isalpha(p, locale_)) ||
				(isdigit == !std::isdigit(p, locale_)) ||
				(ispunct == !std::ispunct(p, locale_)) ||
				isspace ) // continues
			{
				if (token.empty())
				{
					if( !isspace ) 
						tokens.emplace_back(std::string{ c });
					continue;
				}
				else
				{
					tokens.emplace_back(token);
					token.clear();
				}

				if (isspace)
					continue;
			}

			token.push_back(c);
		}

		if( !token.empty() )
			tokens.emplace_back(token);

		utf8.clear();

		for (auto& token : tokens)
		{
			if (std::isalpha(token.front(), locale_))
			{
				for (auto& pair : e2k)
					Replace(token, pair.first, pair.second);
				utf8.append(token);
			}
			else
			{
				// pronunciation
				const char* result = tagger_->parse(token.c_str(), token.size());
				if (result)
				{
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
							utf8.append(column[1].empty() ? column[0] : column[1]);
					}
				}
				else
				{
					utf8.append(token);
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
}