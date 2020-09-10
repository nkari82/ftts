#include "ftts.h"


namespace ftts
{
	class Processor : public IProcessor
	{

	};

	class JSProcessor : public Processor
	{

	};


	IProcessor::Ptr IProcessor::Create(const char* processor)
	{
		return std::make_shared<JSProcessor>();
	}
}