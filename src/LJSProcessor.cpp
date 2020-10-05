#include "PCH.h"
#pragma warning(push, 0)
#include <include/PhonetisaurusScript.h>
#pragma warning(pop)


namespace ftts
{

	LJSProcessor::LJSProcessor(const char* args)
	{
		//script_ = std::make_shared<PhonetisaurusScript>(std::string(args));
	}

	LJSProcessor::~LJSProcessor()
	{
	}

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

	}

	std::string LJSProcessor::IntToStr(int32_t number)
	{
		const static std::vector<std::string> first14 = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "eleven", "twelve", "thirteen", "fourteen" };
		const static std::vector<std::string> prefixes = { "twen", "thir", "for", "fif", "six", "seven", "eigh", "nine" };

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
		const static std::string punctuation = ",.-;";
		const static std::string capitals = "QWERTYUIOPASDFGHJKLZXCVBNM";
		const static std::string lowercase = "qwertyuiopasdfghjklzxcvbnm";
		const static std::string misc = "'";

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