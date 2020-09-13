#include "ftts.h"

using namespace ftts;

int main()
{
	IProcessor::Ptr ptr = IProcessor::Create();
	std::vector<int32_t> seq;
	// EUC-JP	X
	// SHIFT_JIS
	// ISO-2022-JP	X
	// WINDOWS-1251
	// WINDOWS-1252

	//ptr->ToSeq("���Ȫ⪤��", seq, "SHIFT_JIS");
	//ptr->ToSeq("���Ȫ⪤��", seq, "WINDOWS-1251");
	//ptr->ToSeq("�ު����ѪΪ誦������٥�ݪ����Ъ���驪�٥�ݪ�������ժ���몳�Ȫ�������", seq, "CP949");
	//ptr->ToSeq("�ȳ��ϼ���.", seq);
	ptr->ToSeq("�ު����ѪΪ誦������٥�ݪ����Ъ���驪�٥�ݪ�������ժ���몳�Ȫ�������", seq);

	return 0;
}