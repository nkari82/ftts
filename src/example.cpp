#include "ftts.h"

using namespace ftts;

int main()
{
	IProcessor::Ptr ptr = IProcessor::Create();
	std::vector<int32_t> seq;
	ptr->ToSeq("ддоб", seq);
	return 0;
}