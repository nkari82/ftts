#include "ftts.h"

using namespace ftts;

int main()
{
	IProcessor::Ptr ptr = IProcessor::Create("-rD:/Github/unidic-mecab-2.1.2/mecabrc");
	std::vector<int32_t> seq;
	ptr->ToSeq("™ﬁ™ø‘‘ﬁ—™Œ™Ë™¶™ÀÁÈ”ﬁŸ•Ë›™»˚º™–™Ï™ÎÒ´È©™ Ÿ•Ë›™ŒÒÈ‰Á™À€’™µ™Ï™Î™≥™»™‚“˝™§°£", seq);
	//ptr->ToSeq("æ»≥Á«œººø‰", seq);
	
	
	return 0;
}