#pragma once
#include "MediaFormatChecker.h"
#include "Utils.h"
#include <algorithm>

MediaFormatChecker::MediaFormatChecker(
	const std::unordered_set<std::wstring>& inclusions,
	const std::unordered_set<std::wstring>& exclusions,
	const AllowedFormats allowedFormats,
	const bool allowNoException
) noexcept
{
	this->inclusions = inclusions;
	this->exclusions = exclusions;
	this->allowedFormats = allowedFormats;
	this->allowNoExtension = allowNoException;
}

bool MediaFormatChecker::IsFormatAllowed(std::wstring extension) const noexcept
{
	if (extension.length() < 2)  // No extension or something cursed
		return allowNoExtension;

	extension = extension.substr(1);  // Remove the initial dot (e.g. ".mp3" -> "mp3")
	Utils::ToLower(extension);

	if (exclusions.contains(extension))
		return false;

	if (inclusions.contains(extension))
		return true;

	switch (allowedFormats)
	{
		case AllowedFormats::Both:
			return AUDIO_FORMATS.contains(extension) || VIDEO_FORMATS.contains(extension);

		case AllowedFormats::Audio:
			return AUDIO_FORMATS.contains(extension);

		case AllowedFormats::Video:
			return VIDEO_FORMATS.contains(extension);

		default:
			return false;
	}
}
