#pragma once
#include <memory>
#include <vector>

#if defined(_WIN32)
#if defined(ftts_EXPORTS)
#define FTTS_EXPORT_DLL __declspec(dllexport)
#else
#define FTTS_EXPORT_DLL __declspec(dllimport)
#endif
#else
#define FTTS_EXPORT_DLL
#endif

namespace ftts
{
	enum class Langauge
	{
		JPN,		// use mecab dictionary
		ENG		// use g2p dictionary
	};

	class FTTS_EXPORT_DLL IProcessor
	{
	public:
		using Ptr = std::shared_ptr<IProcessor>;
		virtual ~IProcessor() = default;
		virtual void ToSeq(const char* text, std::vector<int32_t>& seq, const char* enc = nullptr) = 0;

		static Ptr Create(const Langauge lang, const char* args);
	};
}