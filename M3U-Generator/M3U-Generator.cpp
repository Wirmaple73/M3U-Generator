// This was supposed to be a 1-hour project in C#, not a 4-day one... I'm C++phobic forever.
// How do y'all manage to stay sane in C++?

#include "CommandLineArgs.h"
#include "Utils.h"
#include "MediaFormatChecker.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <ranges>
#include <sstream>

namespace fs = std::filesystem;

static auto args = CommandLineArgs();

static inline fs::path GetActualExecutablePath(const std::wstring& path) noexcept
{
    try
    {
        return fs::canonical(path).parent_path();
    }
    catch (const fs::filesystem_error&)
    {
        return fs::current_path();
    }
}

static std::unordered_set<std::wstring> GetUnorderedSetFromArgument(const std::wstring& argName) noexcept
{
    std::optional<std::wstring> optArg = args.GetArgumentValue(argName);
    std::unordered_set<std::wstring> set;

    if (optArg.has_value())
    {
        std::wstringstream ss(optArg.value());
        std::wstring token;

        while (std::getline(ss, token, L','))
        {
            if (!token.empty())
            {
                Utils::ToLower(token);
                set.emplace(token);
            }
        }
    }

    return set;
}

static MediaFormatChecker InitMediaFormatChecker() noexcept
{
    MediaFormatChecker::AllowedFormats allowed;

    if (args.DoesArgumentExist(CommandLineArgs::AUDIO_ONLY))
        allowed = MediaFormatChecker::AllowedFormats::Audio;
    else if (args.DoesArgumentExist(CommandLineArgs::VIDEO_ONLY))
        allowed = MediaFormatChecker::AllowedFormats::Video;
    else if (args.DoesArgumentExist(CommandLineArgs::NO_DEFAULT_FORMATS))
        allowed = MediaFormatChecker::AllowedFormats::None;
    else
        allowed = MediaFormatChecker::AllowedFormats::Both;

    const bool allowNoException = args.DoesArgumentExist(CommandLineArgs::ALLOW_NO_EXTENSION);

    const auto inclusions = GetUnorderedSetFromArgument(CommandLineArgs::INCLUSIONS);
    const auto exclusions = GetUnorderedSetFromArgument(CommandLineArgs::EXCLUSIONS);

    return MediaFormatChecker(inclusions, exclusions, allowed, allowNoException);
}

static inline void DisplayPathError(const std::wstring& path, const std::string& message) noexcept
{
    std::wcerr << L"An error occurred while processing " << path;
    std::cerr << ": " << message << "\n";
}

static void WaitForEnter(const bool isSuccessful = true, const std::string& message = "") noexcept
{
    if (message != "")
    {
        if (isSuccessful)
            std::cout << message << "\n\n";
        else
            std::cerr << message << "\n\n";
    }

    if (!args.DoesArgumentExist(CommandLineArgs::NO_PAUSE))
    {
        std::cout << "Press Enter to continue...";
        std::cin.get();
    }
}

int wmain(int argc, wchar_t* argv[])
{
    args = CommandLineArgs(argv, argc);

    if (argc < 2)
    {
        WaitForEnter(false, CommandLineArgs::USAGE_HELP);
        return 1;
    }

    const std::wstring outputFilename = args.GetArgumentValue(CommandLineArgs::OUTPUT_PATH).value_or(L"Output.m3u8");
    const fs::path outputPath = GetActualExecutablePath(argv[0]) / outputFilename;

    std::error_code ec;
    fs::create_directories(outputPath.parent_path(), ec);

    if (ec)
    {
        DisplayPathError(outputPath.parent_path().wstring(), ec.message());
        return 2;
    }

    std::ofstream outputFile;

    if (args.DoesArgumentExist(CommandLineArgs::OVERWRITE))
        outputFile = std::ofstream(outputPath, std::ios::out | std::ios::trunc | std::ios::binary);
    else
        outputFile = std::ofstream(outputPath, std::ios::app | std::ios::binary);

    if (!outputFile.is_open())
    {
        WaitForEnter(false, "Could not open the output file. Please ensure it's not in use by another process and not read-only.");
        return 3;
    }

    outputFile.imbue(std::locale("en_US.UTF-8"));

    if (!args.DoesArgumentExist(CommandLineArgs::NO_BOM))
        outputFile.write("\xEF\xBB\xBF", 3);  // Write the BOM

    MediaFormatChecker checker = InitMediaFormatChecker();
    bool anyErrorOccurred = false;

    auto start = std::chrono::steady_clock::now();

    for (int i = args.GetFirstRegularArgumentIndex(); i < argc; i++)
    {
        fs::path path(argv[i]);

        if (fs::is_regular_file(path, ec) && checker.IsFormatAllowed(path.extension().wstring()))
        {
            Utils::WriteUtf8StringToFilestream(outputFile, path.u8string() + u8"\r\n");
            continue;
        }
        
        if (fs::is_directory(path, ec))
        {
            for (const fs::directory_entry& entry : fs::recursive_directory_iterator(path, fs::directory_options::skip_permission_denied, ec))
            {
                if (fs::is_regular_file(entry, ec) && checker.IsFormatAllowed(entry.path().extension().wstring()))
                {
                    Utils::WriteUtf8StringToFilestream(outputFile, entry.path().u8string() + u8"\r\n");
                    continue;
                }

                if (ec)
                {
                    DisplayPathError(entry.path().wstring(), ec.message());
                    ec.clear();

                    anyErrorOccurred = true;
                }
            }

            continue;
        }
        
        if (ec)
        {
            DisplayPathError(path.wstring(), ec.message());
            ec.clear();

            anyErrorOccurred = true;
        }
    }

    outputFile.close();

    auto end = std::chrono::steady_clock::now();
    std::cout << std::format("Processed all files in {:.5f} seconds.\n", std::chrono::duration<double>(end - start).count());

    WaitForEnter();
    return anyErrorOccurred ? 4 : 0;
}
