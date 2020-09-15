﻿#include "ftts.h"
#include <clocale>

using namespace ftts;

int main()
{
	std::setlocale(LC_ALL,"ja_JP.UTF-8");

	IProcessor::Ptr ptr = IProcessor::Create("-rD:/Github/unidic-mecab-2.1.2/mecabrc");
	std::vector<int32_t> seq;
	ptr->ToSeq("また東寺のように五大明王と呼ばれる主要な明王の中央に配されることも多い。", seq, "CP949");
		
	return 0;
}