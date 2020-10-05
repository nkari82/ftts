#include "PCH.h"

namespace ftts
{

	IProcessor::Ptr IProcessor::Create(const char* args, const char* processor)
	{
		return std::make_shared<JSProcessor>(args);
	}
}