#pragma once
#include <filesystem>

/*
* Command-line arguments:
*
* Argument				| Parameters	| Description							| Examples
* ----------------------------------------------------------------------------------------------------------
* -o					| Path			| Specify the path to the output file.	| -o D:\Folder\Playlist.m3u8
*						|				| Do not quote the path.				| -o Output.m3u8
* ----------------------------------------------------------------------------------------------------------
* -i					| fmt1,fmt2,...	| Include these formats.				| -i hevc
*						|				| 										| -i mod,raw,svi
* ----------------------------------------------------------------------------------------------------------
* -e					| fmt1,fmt2,...	| Exclude these formats.				| -e mp3
*						|				|										| -e avi,wav,flv,mp4
* ----------------------------------------------------------------------------------------------------------
* --overwrite			| None			| Overwrite the output file instead of	| --overwrite
*						|				| appending new entries.				|
* ----------------------------------------------------------------------------------------------------------
* --audio-only			| None			| Only add audio files.					| --audio-only
* ----------------------------------------------------------------------------------------------------------
* --video-only			| None			| Only add video files.					| --video-only
* ----------------------------------------------------------------------------------------------------------
* --no-default-formats	| None			| Reject everything but formats that	| --no-default-formats
*						|				| are specified via -i.					|
* ----------------------------------------------------------------------------------------------------------
* --allow-no-extension	| None			| Include files without an extension.	| --allow-no-extension
* ----------------------------------------------------------------------------------------------------------
* --no-pause			| None			| Don't pause when done and instantly	| --no-pause
*						|				| close the cmd window.					|
* ----------------------------------------------------------------------------------------------------------
*/

class CommandLineArgs
{
private:
	int numPassedArgs;
	wchar_t** args;

public:
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
