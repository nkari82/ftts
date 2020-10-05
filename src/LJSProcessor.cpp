#include "PCH.h"
#include <include/PhonetisaurusScript.h> // eng

namespace ftts
{

	LJSProcessor::LJSProcessor(const char* args)
	{
		script_ = std::make_shared<PhonetisaurusScript>(std::string(args));
	}

	LJSProcessor::~LJSProcessor()
	{

	}

	void LJSProcessor::ToSeq(const char* text, std::vector<int32_t>& seq, const char* enc)
	{

	}
}