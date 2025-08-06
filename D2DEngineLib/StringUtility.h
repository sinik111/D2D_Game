#pragma once

#include <string>
#include <windows.h>
#include <vector>

namespace Util
{
	inline std::wstring MultiByteStringToWideCharString(const std::string& multiByteString)
	{
		if (multiByteString.empty())
		{
			return L"";
		}

		int count = MultiByteToWideChar(
			CP_UTF8,
			0,
			multiByteString.c_str(),
			(int)multiByteString.length(),
			nullptr,
			0
		);

		std::vector<wchar_t> wide_buf(count);

		MultiByteToWideChar(
			CP_UTF8,
			0,
			multiByteString.c_str(),
			(int)multiByteString.length(),
			wide_buf.data(),
			count
		);

		return std::wstring(wide_buf.data(), wide_buf.size());
	}

	inline std::string WideCharStringToMultiByteString(const std::wstring& wideCharString)
	{
		if (wideCharString.empty())
		{
			return "";
		}

		int count = WideCharToMultiByte(
			CP_UTF8,
			0,
			wideCharString.c_str(),
			(int)wideCharString.length(),
			nullptr,
			0,
			nullptr,
			nullptr
		);

		std::vector<char> utf8_buf(count);

		WideCharToMultiByte(
			CP_UTF8,
			0,
			wideCharString.c_str(),
			(int)wideCharString.length(),
			utf8_buf.data(),
			count,
			nullptr,
			nullptr
		);

		return std::string(utf8_buf.data(), utf8_buf.size());
	}
}