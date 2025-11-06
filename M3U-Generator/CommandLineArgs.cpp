#pragma once
#include <iostream>
#include <optional>
#include "CommandLineArgs.h"

CommandLineArgs::CommandLineArgs() noexcept { }

CommandLineArgs::CommandLineArgs(wchar_t* args[], int numPassedArgs) noexcept
{
	this->args = args;
	this->numPassedArgs = numPassedArgs;
}

int CommandLineArgs::GetFirstRegularArgumentIndex() const noexcept
{
	// Example:
	// args[0] = M3U-Generator.exe
	// args[1] = -o
	// args[2] = Output.m3u8
	// args[3] = --overwrite
	// args[4] = D:\a.wav
	// args[5] = D:\b.wav

	int index = 1;

	for (int i = 1; i < numPassedArgs; i++)
	{
		if (args[i][0] != L'-')
			continue;

		if (args[i][1] != L'-' && GetArgumentValue(args[i]).has_value())
			index += 2;
		else
			index++;
	}

	return index;
}

bool CommandLineArgs::DoesArgumentExist(const std::wstring& name) const noexcept
{
	for (int i = 1; i < numPassedArgs; i++)
	{
		if (args[i] == name)
			return true;
	}

	return false;
}

std::optional<std::wstring> CommandLineArgs::GetArgumentValue(const std::wstring& name) const noexcept
{
	for (int i = 1; i + 1 < numPassedArgs; i++)
	{
		if (args[i] == name && args[i + 1][0] != L'-')
			return args[i + 1];
	}

	return std::nullopt;
}
