#include "ftts.h"

using namespace ftts;

int main()
{
	IProcessor::Ptr ptr = IProcessor::Create("-rD:/Github/unidic-mecab-2.1.2/mecabrc");
	std::vector<int32_t> seq;
	ptr->ToSeq("また東寺のように五大明王と呼ばれる主要な明王の中央に配されることも多い。", seq);
	//ptr->ToSeq("안녕하세요", seq);
		
	return 0;
}