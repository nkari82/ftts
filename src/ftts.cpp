#include "PCH.h"

namespace ftts
{

	IProcessor::Ptr IProcessor::Create(const Langauge lang, const char* args)
	{
		switch (lang)
		{
		case Langauge::JPN:
			return std::make_shared<JSProcessor>(args);
		case Langauge::ENG:
			return std::make_shared<LJSProcessor>(args);
		default:
			assert(false);
		}
		return nullptr;
	}
}