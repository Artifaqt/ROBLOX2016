#include "StdAfx.h"
#include "FileSystem.h"
#include "format_string.h"
#include "VistaTools.h"

#include "ATLPath.h"


std::wstring FileSystem::getSpecialFolder(FolderType folder, bool create, const char* subDirectory, bool appendNAMEHERE)
{
	std::string namehereDir = "";
	if (appendNAMEHERE)
	{
		namehereDir = "\\NAMEHERE";
	}
		
	switch (folder)
	{
	case NAMEHEREUserPrograms:
	case NAMEHEREProgramFiles:
		// TODO: Nuke "(Test)" when we go to production
		//namehereDir += " (Test)";
		break;
	}
	
	VistaAPIs apis;
	if (apis.isVistaOrBetter())
	{
		CLSID folderID;
		switch (folder)
		{
		case AppData:
		case NAMEHEREUserApplicationData: // %LOCALAPPDATA% (%USERPROFILE%\AppData\Local)
			folderID = FOLDERID_LocalAppData;
			break;
		case NAMEHEREUserApplicationDataLow: // %USERPROFILE%\AppData\LocalLow
			folderID = FOLDERID_LocalAppDataLow;
			break;
		case NAMEHERECommonApplicationData: // %ALLUSERSPROFILE% (%ProgramData%, %SystemDrive%\ProgramData)
			folderID = FOLDERID_ProgramData;
			break;
		case NAMEHEREUserPrograms: // %APPDATA%\Microsoft\Windows\Start Menu\Programs
			folderID = FOLDERID_Programs;
			break;
		case NAMEHEREProgramFiles: // %ProgramFiles% (%SystemDrive%\Program Files)
			folderID = FOLDERID_ProgramFilesX86;
			break;
		case NAMEHERECommonPrograms: //%ALLUSERSPROFILE%\Microsoft\Windows\Start Menu\Programs	
			folderID = FOLDERID_CommonPrograms;
			break;
		case Desktop: // %PUBLIC%\Desktop
			folderID = FOLDERID_Desktop;
			break;
		case RoamingAppData: // %APPDATA% (%USERPROFILE%\AppData\Roaming)
			folderID = FOLDERID_RoamingAppData;
			break;
		default:
			ATLASSERT(false);
			break;
		}
		
		LPWSTR lppwstrPath = NULL;
		if (apis.SHGetKnownFolderPath(folderID, create ? KF_FLAG_CREATE : 0, NULL, &lppwstrPath)!=E_NOTIMPL)
		{
			CString s = lppwstrPath;

			s += namehereDir.c_str();
			s += "\\";
			if (create)
				if (!::CreateDirectory(s, NULL) && ::GetLastError()!=ERROR_ALREADY_EXISTS)
					throw std::runtime_error(format_string("Failed to create directory '%S', error=%d",s,::GetLastError()));

			if (subDirectory)
			{
				s += subDirectory;
				if (create)
					if (!::CreateDirectory(s, NULL) && ::GetLastError()!=ERROR_ALREADY_EXISTS)
						throw std::runtime_error(format_string("Failed to create subdirectory '%S', error=%d",s,::GetLastError()));
			}
			ATL::CPath path(s);
			path.AddBackslash();
			return std::wstring(path);
		}
	}

	DWORD flags = create ? CSIDL_FLAG_CREATE : 0;
	std::wstring s = convert_s2w(namehereDir);
	if (subDirectory)
		s += convert_s2w(subDirectory);

	int csidl;
	switch (folder)
	{
	case NAMEHEREUserApplicationData:
		csidl = CSIDL_LOCAL_APPDATA;
		break;
	case NAMEHEREUserApplicationDataLow:
		csidl = CSIDL_LOCAL_APPDATA;
		break;
	case NAMEHERECommonApplicationData:
		csidl = CSIDL_COMMON_APPDATA;
		break;
	case NAMEHEREUserPrograms:
		csidl = CSIDL_PROGRAMS;
		break;
	case NAMEHERECommonPrograms:
		csidl = CSIDL_COMMON_PROGRAMS;
		break;
	case NAMEHEREProgramFiles:
		csidl = CSIDL_PROGRAM_FILES;
		break;
	case Desktop:
		csidl = CSIDL_DESKTOP;
		break;
	case AppData:
		csidl = CSIDL_APPDATA;
		break;
	default:
		ATLASSERT(false);
		break;
	}

	wchar_t strPath[_MAX_PATH];
	HRESULT hr = SHGetFolderPathAndSubDirW(NULL, csidl | flags, NULL, SHGFP_TYPE_CURRENT, s.c_str(), strPath);	

	if (hr==S_OK)
	{
		CString s = strPath;

		ATL::CPath path(s);
		path.AddBackslash();
		return std::wstring(path);
	}
	else{
		throw std::runtime_error(format_string("Bad result from SHGetFolderPathAndSubDirW=%d",hr));
	}
}

bool FileSystem::IsFileExists(const TCHAR* name)
{
	HANDLE file = CreateFile(name, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	bool exists = false;
	if (file != INVALID_HANDLE_VALUE) 
	{
		exists = true;
		CloseHandle(file);
	}

	return exists;
}

bool FileSystem::IsFolderExists(const TCHAR* name)
{
	DWORD dwAttrib = GetFileAttributes(name);
	return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

