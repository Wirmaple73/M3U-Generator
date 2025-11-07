#pragma once
#include <unordered_set>
#include <string>

class MediaFormatChecker
{
public:
    enum class AllowedFormats
    {
        None, Audio, Video, Both
    };

    MediaFormatChecker(
        const std::unordered_set<std::wstring>& inclusions,
        const std::unordered_set<std::wstring>& exclusions,
        const AllowedFormats allowedFormats,
        const bool allowNoExtension
    ) noexcept;

    bool IsFormatAllowed(std::wstring extension) const noexcept;

private:
    const std::unordered_set<std::wstring> AUDIO_FORMATS = {
        L"mp3", L"wav", L"ogg", L"flac", L"m4a", L"wma", L"aac", L"oga",
        L"mid", L"midi", L"opus", L"alac", L"aiff", L"aif", L"amr",
        L"pcm", L"ra", L"ram", L"au", L"ac3", L"dts", L"mka"
    };

    const std::unordered_set<std::wstring> VIDEO_FORMATS = {
        L"mp4", L"avi", L"mkv", L"mov", L"wmv", L"flv", L"webm", L"mpeg",
        L"mpg", L"m4v", L"3gp", L"3g2", L"vob", L"ogv", L"xvid", L"mts",
        L"m2ts", L"divx", L"f4v", L"rm", L"rmvb"
    };

    std::unordered_set<std::wstring> inclusions;
    std::unordered_set<std::wstring> exclusions;

    AllowedFormats allowedFormats;
    bool allowNoExtension;
};
