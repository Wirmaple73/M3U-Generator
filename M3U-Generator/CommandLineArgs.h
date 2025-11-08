#pragma once
#include <filesystem>

class CommandLineArgs
{
private:
	int numPassedArgs;
	wchar_t** args;

public:
	static inline const std::string USAGE_HELP =
R"(USAGE:
  M3U-Generator.exe [options] <path1> [path2] [path3] ...

  Each path can be a file or a directory. Paths containing spaces must be wrapped in quotes.
  For your convenience, files and directories can also be drag-and-dropped onto the executable.
  * File paths can be relative to the current directory.


DESCRIPTION:
  Generates an M3U or M3U8 playlist by scanning the passed files and folders, including subfolders.
  By default, the program outputs a file named Output.m3u8 with BOM and CRLF for optimal compatiblity
  with Windows.


OPTIONS:
  -o <path>                 Specify the path to the output file. Defaults to Output.m3u8 if not set.
                            Wrap the path in double quotes if it contains spaces.
                            Example 1: -o "D:\Folder with space\My playlist.m3u8"
                            Example 2: -o Output.m3u

  -i <fmt1,fmt2,...>        Include these formats (comma-separated). Do not add space between formats.
                            Example 1: -i hevc
                            Example 2: -i ts,svi,raw

  -e <fmt1,fmt2,...>        Exclude these formats (comma-separated). Do not add space between formats.
                            Example 1: -e mp3
                            Example 2: -e avi,wav,flv,mp4

  --overwrite               Overwrite the output file instead of appending new entries.

  --audio-only              Only add audio files.

  --video-only              Only add video files.

  --allow-no-extension      Include files without an extension.

  --no-default-formats      Reject all formats except those included via -i.
                            See the NOTES section below for a list of default formats.

  --no-pause                Exit immediately when done instead of waiting for key press.


EXAMPLES:
  * Create an M3U8 playlist in the current directory (default):
    M3U-Generator.exe D:\Music

  * Create an M3U8 file in the current directory:
    M3U-Generator.exe -o Output.m3u8 D:\Music

  * Include only wav files:
    M3U-Generator.exe -i wav --no-default-formats D:\Music E:\File.wav

  * Include all video files including hevc and ts, but not avi:
    M3U-Generator.exe --video-only -i hevc,ts -e avi D:\Music "D:\My Movies"


NOTES:
  * If neither --audio-only nor --video-only is specified, both types are included by default.

  * Formats that are included by default (unless using --no-default-formats):
    - Audio formats:
      aac, ac3, aif, aiff, alac, amr, au, dts, flac, m4a, mid, midi, mka, mp3, oga, ogg, opus, pcm,
      ra, ram, wav, wma

    - Video formats:
      3g2, 3gp, avi, divx, f4v, flv, m2ts, m4v, mkv, mov, mp4, mpeg, mpg, mts, ogv, rm, rmvb, vob,
      webm, wmv, xvid


EXIT CODES:
  0: The playlist was generated successfully.
  1: The passed arguments are invalid or no file/folder was passed.
  2: The path to the output file could not be created.
  3: The output file could not be opened for writing.
  4: The playlist was generated, albeit with errors.)";

	static inline const std::wstring OUTPUT_PATH        = L"-o";
	static inline const std::wstring INCLUSIONS         = L"-i";
	static inline const std::wstring EXCLUSIONS         = L"-e";
	static inline const std::wstring OVERWRITE          = L"--overwrite";
	static inline const std::wstring AUDIO_ONLY         = L"--audio-only";
	static inline const std::wstring VIDEO_ONLY         = L"--video-only";
	static inline const std::wstring NO_DEFAULT_FORMATS = L"--no-default-formats";
	static inline const std::wstring ALLOW_NO_EXTENSION = L"--allow-no-extension";
	static inline const std::wstring NO_PAUSE           = L"--no-pause";

	CommandLineArgs() noexcept;
	CommandLineArgs(wchar_t* args[], int numArgs) noexcept;

	int GetFirstRegularArgumentIndex() const noexcept;
	bool DoesArgumentExist(const std::wstring& name) const noexcept;
	std::optional<std::wstring> GetArgumentValue(const std::wstring& name) const noexcept;
};
