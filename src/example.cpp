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

	//ptr->ToSeq("ª³ªÈªâª¤¡£", seq, "SHIFT_JIS");
	//ptr->ToSeq("ª³ªÈªâª¤¡£", seq, "WINDOWS-1251");
	//ptr->ToSeq("ªŞª¿ÔÔŞÑªÎªèª¦ªËçéÓŞÙ¥èİªÈû¼ªĞªìªëñ«é©ªÊÙ¥èİªÎñéäçªËÛÕªµªìªëª³ªÈªâÒıª¤¡£", seq, "CP949");
	//ptr->ToSeq("¾È³çÇÏ¼¼¿ä.", seq);
	ptr->ToSeq("ªŞª¿ÔÔŞÑªÎªèª¦ªËçéÓŞÙ¥èİªÈû¼ªĞªìªëñ«é©ªÊÙ¥èİªÎñéäçªËÛÕªµªìªëª³ªÈªâÒıª¤¡£", seq);

	return 0;
}