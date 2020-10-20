#include "ftts.h"
#include <clocale>

using namespace ftts;

int main()
{
#if 1
	std::setlocale(LC_ALL, "ja_JP.UTF-8");

	const 
	IProcessor::Ptr ptr = IProcessor::Create(ftts::Langauge::JPN, {"-rD:/Github/unidic-mecab-2.1.2/mecabrc"});
	std::vector<int32_t> seq;
	//ptr->ToSeq("また東寺のように五大明王と呼ばれる主要な明王の中央に配されることも多い。", seq, "CP949");
	ptr->ToSeq("こんにちは、'Ki-jung!'", seq, "CP949");
#else
	IProcessor::Ptr ptr = IProcessor::Create(ftts::Langauge::ENG, { "D:/Github/ftts/g2p.fst" });
	std::vector<int32_t> seq;
	ptr->ToSeq("example to example.", seq, "CP949");
#endif
	return 0;
}