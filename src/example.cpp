#include "ftts.h"

using namespace ftts;

int main()
{
	IProcessor::Ptr ptr = IProcessor::Create();
	std::vector<int32_t> seq;
	ptr->ToSeq("����", seq);
	return 0;
}