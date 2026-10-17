#pragma once

#include <string>

class FileSystem
{
public:
	typedef enum { NAMEHEREUserApplicationData, NAMEHEREUserApplicationDataLow, NAMEHERECommonApplicationData, NAMEHEREUserPrograms, NAMEHERECommonPrograms, NAMEHEREProgramFiles, Desktop, RoamingAppData, AppData } FolderType;
	static std::wstring getSpecialFolder(FolderType folder, bool create, const char* subDirectory = 0, bool appendNAMEHERE = true);
	static bool IsFileExists(const TCHAR* name);
	static bool IsFolderExists(const TCHAR* name);
};
