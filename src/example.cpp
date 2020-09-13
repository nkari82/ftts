#include "ftts.h"

using namespace ftts;

int main()
{
	IProcessor::Ptr ptr = IProcessor::Create("-rD:/Github/unidic-mecab-2.1.2/mecabrc");
	std::vector<int32_t> seq;
	ptr->ToSeq("ªŞª¿ÔÔŞÑªÎªèª¦ªËçéÓŞÙ¥èİªÈû¼ªĞªìªëñ«é©ªÊÙ¥èİªÎñéäçªËÛÕªµªìªëª³ªÈªâÒıª¤¡£", seq, "CP949" /*current codepage*/);

	return 0;
}