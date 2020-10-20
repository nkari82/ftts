#include "PCH.h"
#pragma warning(push, 0)
#include <include/PhonetisaurusScript.h>
#pragma warning(pop)

// Copyright  https://github.com/ZDisket

namespace ftts
{
	const static std::string punctuation = ",.-;";
	const static std::string capitals = "QWERTYUIOPASDFGHJKLZXCVBNM";
	const static std::string lowercase = "qwertyuiopasdfghjklzxcvbnm";
	const static std::string misc = "'";

	const static std::vector<std::string> first14 = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen" };
	const static std::vector<std::string> prefixes = { "twen", "thir", "for", "fif", "six", "seven", "eigh", "nine" };


	const static std::unordered_map<std::string, int32_t> Phonemes =
	{ 
		{"AA",64}, {"AA0",65}, {"AA1",66}, {"AA2",67}, {"AE",68}, {"AE0",69}, {"AE1",70}, {"AE2",71}, {"AH",72}, {"AH0",73}, 
		{"AH1",74}, {"AH2",75}, {"AO",76}, {"AO0",77}, {"AO1",78}, {"AO2",79}, {"AW",80}, {"AW0",81}, {"AW1",82}, {"AW2",83},
		{"AY",84}, {"AY0",85}, {"AY1",86}, {"AY2",87}, {"B",88}, {"CH",89}, {"D",90}, {"DH",91}, {"EH",92}, {"EH0",93},
		{"EH1",94}, {"EH2",95}, {"ER",96}, {"ER0",97}, {"ER1",98}, {"ER2",99}, {"EY",100}, {"EY0",101}, {"EY1",102}, {"EY2",103},
		{"F",104}, {"G",105}, {"HH",106}, {"IH",107}, {"IH0",108}, {"IH1",109}, {"IH2",110}, {"IY",111}, {"IY0",112}, {"IY1",113},
		{"IY2",114}, {"JH",115}, {"K",116}, {"L",117}, {"M",118}, {"N",119}, {"NG",120}, {"OW",121}, {"OW0",122}, {"OW1",123},
		{"OW2",124}, {"OY",125}, {"OY0",126}, {"OY1",127}, {"OY2",128}, {"P",129}, {"R",130}, {"S",131}, {"SH",132}, {"T",133},
		{"TH",134}, {"UH",135}, {"UH0",136}, {"UH1",137}, {"UH2",138}, {"UW",139}, {"UW0",140}, {"UW1",141}, {"UW2",142}, {"V",143},
		{"W",144}, {"Y",145}, {"Z",146}, {"ZH",147}, {"SIL",148}, {"END",149}
	};

	LJSProcessor::LJSProcessor(const std::vector<std::string>& args)
	{
		if (args.empty()) return;

		script_ = std::make_shared<PhonetisaurusScript>(args[0]);
		sil_ = Phonemes.find("SIL")->second;
		eos_ = Phonemes.find("END")->second;
	}

	LJSProcessor::~LJSProcessor()
	{}

	void LJSProcessor::ToSeq(const char* text, std::vector<int32_t>& seq, const char* enc)
	{
		std::vector<std::string> Words;
		Tokenize(Words, text);
		
		if (script_ == nullptr) return;

		std::string Assemble = "";
		for (size_t w = 0; w < Words.size(); w++)
		{
			const std::string& Word = Words[w];

			if (Word == "SIL") 
			{
				Assemble.append(Word);
				Assemble.append(" ");
				continue;
			}

			std::vector<PathData> PhResults = script_->Phoneticize(Word, 1, 10000, 99.f, false, false, 0.99);
			for (const auto& padat : PhResults) 
			{
				for (const auto& uni : padat.Uniques) 
				{
					Assemble.append(script_->osyms_->Find(uni));
					Assemble.append(" ");
				}
			}
		}

		// Delete last space if there is
		if (Assemble.back() == ' ')
			Assemble.pop_back();

		// for test
		std::cout << Assemble << std::endl;

		// phonome to ids
		seq.reserve(Assemble.size());

		std::stringstream ss(Assemble);
		std::string Pho;
		while (std::getline(ss, Pho, ' '))
		{
			if (Pho.empty()) continue;

			int32_t id = 0;

			auto it = Phonemes.find(Pho);
			if (it != Phonemes.end())
				seq.emplace_back(it->second);
			else
				std::cerr << "WARNING: Unknown phoneme " << Pho << std::endl;
		}

		seq.emplace_back(eos_);
	}

	std::string LJSProcessor::IntToStr(int32_t number)
	{
		if (number < 0) return "minus " + IntToStr(-number);

		if (number <= 14)
			return first14.at(number);
		else if (number < 20)
			return prefixes.at(number - 12) + "teen";
		else if (number < 100) 
		{
			uint32_t remainder = number - (static_cast<int32_t>(number / 10) * 10);
			return prefixes.at(number / 10 - 2) + (0 != remainder ? "ty " + IntToStr(remainder) : "ty");
		}
		else if (number < 1000) 
		{
			uint32_t remainder = number - (static_cast<int32_t>(number / 100) * 100);
			return first14.at(number / 100) + (0 != remainder ? " hundred " + IntToStr(remainder) : " hundred");
		}
		else if (number < 1000000) 
		{
			uint32_t thousands = static_cast<int32_t>(number / 1000);
			uint32_t remainder = number - (thousands * 1000);
			return IntToStr(thousands) + (0 != remainder ? " thousand " + IntToStr(remainder) : " thousand");
		}
		else if (number < 1000000000) 
		{
			uint32_t millions = static_cast<int32_t>(number / 1000000);
			uint32_t remainder = number - (millions * 1000000);
			return IntToStr(millions) + (0 != remainder ? " million " + IntToStr(remainder) : " million");
		}
		throw std::out_of_range("inttostr() value too large");
	}

	void LJSProcessor::ExpandNumbers(std::vector<std::string>& OutTokens, const std::vector<std::string>& SpaceTokens)
	{
		OutTokens.reserve(SpaceTokens.size());
		
		for (auto& Token : SpaceTokens) 
		{
			char* p;
			long converted = std::strtol(Token.c_str(), &p, 10);
			if (*p) 
			{
				OutTokens.emplace_back(Token);
			}
			else 
			{
				if (converted > 1000000000)
					continue;

				std::vector<std::string> NumToks;
				std::stringstream ss(IntToStr((int32_t)converted));
				std::string col;
				
				while (std::getline(ss, col, ' '))
				{
					if (!col.empty())
						NumToks.emplace_back(col);
				}

				for (const auto& NumTok : NumToks)
					OutTokens.emplace_back(NumTok);
			}
		}
	}

	void LJSProcessor::Tokenize(std::vector<std::string>& OutTokens, const std::string& InTxt)
	{
		std::vector<std::string> DelimitedTokens;
		std::stringstream ss(InTxt);
		std::string col;

		while (std::getline(ss, col, ' ')) 
		{
			if(!col.empty())
				DelimitedTokens.push_back(col);
		}

		std::vector<std::string> Tokens;
		ExpandNumbers(Tokens, DelimitedTokens);

		OutTokens.reserve(Tokens.size());

		for (const auto& tok : Tokens)
		{
			std::string AppTok = "";
			for (size_t s = 0; s < tok.size(); s++)
			{
				if (lowercase.find(tok[s]) != std::string::npos) 
					AppTok += tok[s];

				size_t IdxInUpper = capitals.find(tok[s]);
				if (IdxInUpper != std::string::npos) 
					AppTok += lowercase[IdxInUpper];

				if (punctuation.find(tok[s]) != std::string::npos)
				{
					if (!AppTok.empty()) 
					{
						OutTokens.emplace_back(AppTok);
						AppTok = "";
					}
					OutTokens.emplace_back("SIL");
				}

				if (misc.find(tok[s]) != std::string::npos)
					AppTok += tok[s];
			}

			if (!AppTok.empty())
				OutTokens.emplace_back(AppTok);
		}

		if (!OutTokens.empty())
		{
			if (OutTokens.back() == "SIL")
				OutTokens.pop_back();
		}
	}
}