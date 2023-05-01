#pragma once

// just use std::filesystem

//#define RUNNING_WINDOWS
//#include <iostream>
//#include <string>
//#include <algorithm>
//
//namespace Utils
//{
//#ifdef RUNNING_WINDOWS
//#define WIN64_LEAN_AND_MEAN
//#include <Windows.h>
//#include <Shlwapi.h>
//
//	inline std::string GetWorkingDirectory()
//	{
//		HMODULE hModule = GetModuleHandle(nullptr);
//		if (hModule)
//		{
//			char path[256];
//			GetModuleFileNameA(hModule, path, sizeof(path));
//			std::string intermediatePath = path;
//
//			PathRemoveFileSpecA(path);
//			strcat_s(path, "\\");
//
//			std::string resultPath = path;
//
//			return std::string(path);
//		}
//		return "";
//	}
//
//#elif defined RUNNING_LINUX
//#include <unistd.h>
//
//	inline std::string GetWorkingDirectory()
//	{
//		char cwd[1024];
//		if (getcwd(cwd, sizeof(cwd)))
//		{
//			return std::string(cwd) + std::string("/");
//		}
//		return "";
//	}
//
//#endif
//}