#include "PCH.h"

namespace ftts
{

	IProcessor::Ptr IProcessor::Create(const char* args, const char* processor)
	{
		if (processor != nullptr)
		{
			if (std::strcmp(processor, "ljs") == 0)
				return std::make_shared<LJSProcessor>(args);
		}
		return std::make_shared<JSProcessor>(args);
	}
}