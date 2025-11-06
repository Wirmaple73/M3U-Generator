#pragma once
#include <string>
#include <filesystem>
#include <algorithm>
#include <cwctype>
#include <fstream>

class Utils
{
public:
	static inline void ToLower(std::wstring& s) noexcept
	{
		// No built-in ToLower function for strings? Thanks C++ :)
		std::transform(s.begin(), s.end(), s.begin(), [](const wchar_t c)
		{
			return towlower(c);
		});
	}

	static inline void WriteUtf8StringToFilestream(std::ofstream& stream, const std::u8string& s) noexcept
	{
		stream.write(reinterpret_cast<const char*>(s.c_str()), s.length());
	}
};
