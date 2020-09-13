#include "ftts.h"

using namespace ftts;

int main()
{
	IProcessor::Ptr ptr = IProcessor::Create("-rD:/Github/unidic-mecab-2.1.2/mecabrc");
	std::vector<int32_t> seq;
	ptr->ToSeq("�ު����ѪΪ誦������٥�ݪ����Ъ���驪�٥�ݪ�������ժ���몳�Ȫ�������", seq, "CP949" /*current codepage*/);

	return 0;
}