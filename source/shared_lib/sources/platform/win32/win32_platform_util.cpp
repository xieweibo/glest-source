// This file is part of Glest <https://github.com/Glest>
//
// Copyright (C) 2018  The Glest team
//
// Glest is a fork of MegaGlest <https://megaglest.org/>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>

#include "platform_util.h"
#include "window.h"

#include <io.h>
#include <dbghelp.h>

#include <cassert>
#include <algorithm>
#include <string.h>
#include "SDL_syswm.h"
#include <iostream>
#include <stdexcept>
#include "leak_dumper.h"

using namespace Shared::Util;
using namespace std;

namespace Shared {
	namespace Platform {

		// =====================================================
		//	class PlatformExceptionHandler
		// =====================================================
		string PlatformExceptionHandler::application_binary = "";
		bool PlatformExceptionHandler::disableBacktrace = false;
		PlatformExceptionHandler *PlatformExceptionHandler::thisPointer = NULL;

		// Constructs object and convert lpaszString to Unicode
		LPWSTR Ansi2WideString(LPCSTR lpaszString) {
			LPWSTR lpwszString(NULL);

			if (lpaszString != NULL) {
				int nLen = ::lstrlenA(lpaszString) + 1;
				lpwszString = new WCHAR[nLen];
				if (lpwszString == NULL) {
					return lpwszString;
				}

				memset(lpwszString, 0, nLen * sizeof(WCHAR));

				if (::MultiByteToWideChar(CP_ACP, 0, lpaszString, nLen, lpwszString, nLen) == 0) {
					// Conversation failed
					return lpwszString;
				}
			} else {
				int nLen = 1;
				lpwszString = new WCHAR[nLen];
				if (lpwszString == NULL) {
					return lpwszString;
				}

				memset(lpwszString, 0, nLen * sizeof(WCHAR));
			}

			return lpwszString;
		}

		// Convert a wide Unicode string to an UTF8 string
		std::string utf8_encode(const std::wstring &wstr) {
			if (wstr.length() == 0) {
				std::string wstrTo;
				return wstrTo;
			}

			int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int) wstr.size(), NULL, 0, NULL, NULL);
			std::string strTo(size_needed, 0);
			WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int) wstr.size(), &strTo[0], size_needed, NULL, NULL);
			replaceAll(strTo, "/", "\\");
			replaceAll(strTo, "\\\\", "\\");
			updatePathClimbingParts(strTo);
			return strTo;
		}

		// Convert an UTF8 string to a wide Unicode String
		std::wstring utf8_decode(const std::string &str) {
			if (str.length() == 0) {
				std::wstring wstrTo;
				return wstrTo;
			}
			string friendly_path = str;
			replaceAll(friendly_path, "/", "\\");
			replaceAll(friendly_path, "\\\\", "\\");
			updatePathClimbingParts(friendly_path);
			int size_needed = MultiByteToWideChar(CP_UTF8, 0, &friendly_path[0], (int) friendly_path.size(), NULL, 0);
			std::wstring wstrTo(size_needed, 0);
			MultiByteToWideChar(CP_UTF8, 0, &friendly_path[0], (int) friendly_path.size(), &wstrTo[0], size_needed);
			return wstrTo;
		}

		/**
		* @param location The location of the registry key. For example "Software\\Bethesda Softworks\\Morrowind"
		* @param name the name of the registry key, for example "Installed Path"
		* @return the value of the key or an empty string if an error occured.
		*/
		std::string getRegKey(const std::string& location, const std::string& name) {
			HKEY key;
			CHAR value[1024];
			DWORD bufLen = 1024 * sizeof(CHAR);
			long ret;
			ret = RegOpenKeyExA(HKEY_LOCAL_MACHINE, location.c_str(), 0, KEY_QUERY_VALUE, &key);
			if (ret != ERROR_SUCCESS) {
				return std::string();
			}
			ret = RegQueryValueExA(key, name.c_str(), 0, 0, (LPBYTE) value, &bufLen);
			RegCloseKey(key);
			if ((ret != ERROR_SUCCESS) || (bufLen > 1024 * sizeof(TCHAR))) {
				return std::string();
			}
			string stringValue = value;
			size_t i = stringValue.length();
			while (i > 0 && stringValue[i - 1] == '\0') {
				--i;
			}
			return stringValue.substr(0, i);
		}

		LONG WINAPI PlatformExceptionHandler::handler(LPEXCEPTION_POINTERS pointers) {

			//printf("In [%s::%s Line: %d]\n",__FILE__,__FUNCTION__,__LINE__);

			LPWSTR wstr = Ansi2WideString(thisPointer->dumpFileName.c_str());
			HANDLE hFile = CreateFile(
				wstr,
				GENERIC_WRITE,
				FILE_SHARE_WRITE,
				NULL,
				CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				0);
			if (wstr) delete[] wstr;

			//printf("In [%s::%s Line: %d]\n",__FILE__,__FUNCTION__,__LINE__);

			MINIDUMP_EXCEPTION_INFORMATION lExceptionInformation;

			lExceptionInformation.ThreadId = GetCurrentThreadId();
			lExceptionInformation.ExceptionPointers = pointers;
			lExceptionInformation.ClientPointers = false;

#if !defined(__GNUC__)
			MiniDumpWriteDump(
				GetCurrentProcess(),
				GetCurrentProcessId(),
				hFile,
				MiniDumpNormal,
				&lExceptionInformation,
				NULL,
				NULL);

			//printf("In [%s::%s Line: %d]\n",__FILE__,__FUNCTION__,__LINE__);

			thisPointer->handle(pointers);
#endif
			//printf("In [%s::%s Line: %d]\n",__FILE__,__FUNCTION__,__LINE__);

			return EXCEPTION_EXECUTE_HANDLER;
		}

		void PlatformExceptionHandler::install(string dumpFileName) {
			thisPointer = this;
			this->dumpFileName = dumpFileName;
#if !defined(HAVE_GOOGLE_BREAKPAD)
			SetUnhandledExceptionFilter(handler);
#endif
		}

		string PlatformExceptionHandler::getStackTrace() {
			string result = "\nStack Trace:\n";
			if (PlatformExceptionHandler::disableBacktrace == true) {
				result += "disabled...";
				return result;
			}
#ifndef __MINGW32__

			CONTEXT context = { 0 };
			context.ContextFlags = CONTEXT_FULL;

			IMAGEHLP_SYMBOL *pSym = (IMAGEHLP_SYMBOL*)new BYTE[sizeof(IMAGEHLP_SYMBOL) + 256];
			pSym->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL);
			pSym->MaxNameLength = 256;

			IMAGEHLP_LINE line = { 0 };
			line.SizeOfStruct = sizeof(IMAGEHLP_LINE);

			IMAGEHLP_MODULE module = { 0 };
			module.SizeOfStruct = sizeof(IMAGEHLP_MODULE);

			HANDLE hProcess = GetCurrentProcess();
			HANDLE hThread = GetCurrentThread();
			if (GetThreadContext(hThread, &context)) {
				STACKFRAME stackframe = { 0 };
#if !defined(_WIN64)
				stackframe.AddrPC.Offset = context.Eip;
#else
				stackframe.AddrPC.Offset = context.Rip;
#endif
				stackframe.AddrPC.Mode = AddrModeFlat;
#if !defined(_WIN64)
				stackframe.AddrFrame.Offset = context.Ebp;
#else
				stackframe.AddrFrame.Offset = context.Rbp;
#endif
				stackframe.AddrFrame.Mode = AddrModeFlat;

				SymInitialize(hProcess, NULL, TRUE);
				BOOL fSuccess = TRUE;

				do {
#if !defined(_WIN64)
					fSuccess = StackWalk(IMAGE_FILE_MACHINE_I386,
#else
					fSuccess = StackWalk(IMAGE_FILE_MACHINE_AMD64,
#endif
						GetCurrentProcess(),
						GetCurrentThread(),
						&stackframe,
						&context,
						NULL,
						NULL,
						NULL,
						NULL);
#if !defined(_WIN64)
					DWORD dwDisplacement = 0;
#else
					DWORD64 dwDisplacement = 0;
#endif
					DWORD dwDisplacement2 = 0;
					SymGetSymFromAddr(hProcess, stackframe.AddrPC.Offset, &dwDisplacement, pSym);
					SymGetLineFromAddr(hProcess, stackframe.AddrPC.Offset, &dwDisplacement2, &line);
					SymGetModuleInfo(hProcess, stackframe.AddrPC.Offset, &module);

					// RetAddr Arg1 Arg2 Arg3 module!funtion FileName(line)+offset

					char szBuf[8096] = "";
					snprintf(szBuf, 8096, "%08lx %08lx %08lx %08lx %s!%s %s(%lu) %+ld\n",
						stackframe.AddrReturn.Offset,
						stackframe.Params[0],
						stackframe.Params[1],
						stackframe.Params[2],
						pSym->Name,
						module.ModuleName,
						line.FileName,
						line.LineNumber,
						dwDisplacement);
					result += szBuf;

				} while (fSuccess);

				SymCleanup(hProcess);
			}
#endif

#ifndef __MINGW32__
			delete[] pSym;
#endif
			return result;
		}

		game_runtime_error::game_runtime_error(const string& __arg, bool noStackTrace)
			: std::runtime_error(noStackTrace == false ? __arg + PlatformExceptionHandler::getStackTrace() : __arg), noStackTrace(noStackTrace) {
		}

		// =====================================================
		//	class Misc
		// =====================================================

		//void getFullscreenVideoInfo(int &colorBits,int &screenWidth,int &screenHeight) {
		//    // Get the current video hardware information
		//    //const SDL_VideoInfo* vidInfo = SDL_GetVideoInfo();
		//    //colorBits      = vidInfo->vfmt->BitsPerPixel;
		//    //screenWidth    = vidInfo->current_w;
		//
		///*
		//	//screenHeight   = vidInfo->current_h;
		//    int cx = GetSystemMetrics(SM_CXVIRTUALSCREEN);
		//    // height
		//    int cy = GetSystemMetrics(SM_CYVIRTUALSCREEN);
		//
		//	printf("cx = %d, cy = %d\n",cx,cy);
		//
		//	if(cx > screenWidth) {
		//		screenWidth  = cx;
		//		screenHeight = cy;
		//	}
		//*/
		//	int iMaxWidth	= -1;
		//	int iMaxHeight	= -1;
		//	int iMaxBits	= -1;
		//
		//	DEVMODE devMode;
		//	for (int i=0; EnumDisplaySettings(NULL, i, &devMode) ;i++){
		//
		//		//printf("devMode.dmPelsWidth = %d, devMode.dmPelsHeight = %d, devMode.dmBitsPerPel = %d\n",devMode.dmPelsWidth,devMode.dmPelsHeight,devMode.dmBitsPerPel);
		//
		//		if(devMode.dmPelsWidth > iMaxWidth) {
		//			iMaxWidth  = devMode.dmPelsWidth;
		//			iMaxHeight = devMode.dmPelsHeight;
		//			iMaxBits   = devMode.dmBitsPerPel;
		//			//devMode.dmDisplayFrequency=refreshFrequency;
		//		}
		//	}
		//	if(iMaxWidth > 0) {
		//		colorBits      = iMaxBits;
		//		screenWidth    = iMaxWidth;
		//		screenHeight   = iMaxHeight;
		//	}
		//}
		//
		//bool changeVideoMode(int resW, int resH, int colorBits, int refreshFrequency){
		//	DEVMODE devMode;
		//
		//	for (int i=0; EnumDisplaySettings(NULL, i, &devMode) ;i++){
		//		if (devMode.dmPelsWidth== resW &&
		//			devMode.dmPelsHeight== resH &&
		//			devMode.dmBitsPerPel== colorBits){
		//
		//			devMode.dmDisplayFrequency=refreshFrequency;
		//
		//			LONG result= ChangeDisplaySettings(&devMode, 0);
		//			if(result == DISP_CHANGE_SUCCESSFUL){
		//				return true;
		//			}
		//			else{
		//				return false;
		//			}
		//		}
		//	}
		//
		//	return false;
		//}
		//
		//void restoreVideoMode(bool exitingApp) {
		//	int dispChangeErr= ChangeDisplaySettings(NULL, 0);
		//	assert(dispChangeErr==DISP_CHANGE_SUCCESSFUL);
		//}

		void message(string message, bool isNonGraphicalModeEnabled, const string &writepath) {
			std::cerr << "******************************************************\n";
			std::cerr << "    " << message << "\n";
			std::cerr << "******************************************************\n";

			if (isNonGraphicalModeEnabled == false) {
				LPWSTR wstr = Ansi2WideString(message.c_str());
				MessageBox(NULL, wstr, L"Message", MB_OK | MB_SYSTEMMODAL);
				if (wstr) delete[] wstr;
			}
		}

		void exceptionMessage(const exception &excp) {
			string message, title;
			showCursor(true);

			message += "ERROR(S):\n\n";
			message += excp.what();

			title = "Error: Unhandled Exception";
			printf("Error detected with text: %s\n", message.c_str());

			LPWSTR wstr = Ansi2WideString(message.c_str());
			LPWSTR wstr1 = Ansi2WideString(title.c_str());
			MessageBox(NULL, wstr, wstr1, MB_ICONSTOP | MB_OK | MB_SYSTEMMODAL);
			if (wstr) delete[] wstr;
			if (wstr1) delete[] wstr1;
		}

		//int getScreenW(){
		//	return GetSystemMetrics(SM_CXSCREEN);
		//}

		//int getScreenH(){
		//	return GetSystemMetrics(SM_CYSCREEN);
		//}

		// This lets us set the SDL Window Icon in Windows
		// since its a console application
		HICON icon;

		void init_win32() {
			HINSTANCE handle = ::GetModuleHandle(NULL);
			icon = ::LoadIcon(handle, L"IDI_ICON1");

			SDL_SysWMinfo wminfo;
			SDL_VERSION(&wminfo.version)
				if (SDL_GetWindowWMInfo(Window::getSDLWindow(), &wminfo) != 1) {
					// error: wrong SDL version
				}

			HWND hwnd = wminfo.info.win.window;

#ifdef __MINGW32__
#define GCL_HICON -14
#endif

#ifndef __MINGW32__
			LONG iconPtr = (LONG) icon;

#if !defined(_WIN64)
			::SetClassLong(hwnd, GCL_HICON, iconPtr);
#else
			::SetClassLongPtr(hwnd, GCLP_HICON, (LONG_PTR) &iconPtr);
#endif
#endif

			ontop_win32(0, 0);
		}

		void ontop_win32(int width, int height) {
			SDL_SysWMinfo wminfo;
			SDL_VERSION(&wminfo.version)
				if (SDL_GetWindowWMInfo(Window::getSDLWindow(), &wminfo) != 1) {
					// error: wrong SDL version
				}

			HWND hwnd = wminfo.info.win.window;

			SetWindowLong(hwnd, GWL_EXSTYLE, 0);
			SetWindowLong(hwnd, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
			SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
			if (width > 0 && height > 0) {
				SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, width, height, SWP_SHOWWINDOW);
			}
		}

		void done_win32() {
			::DestroyIcon(icon);
		}

		void CheckPacketThrottling() {
			static bool alreadyChecked = false;
			if (alreadyChecked == true) {
				return;
			}
			alreadyChecked = true;
			//printf("Checking Windows Network Packet Throttle Setting...\n");
			//Open the registry key.
			wstring subKey = L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Multimedia\\SystemProfile";
			wstring Key = L"NetworkThrottlingIndex";
			HKEY keyHandle;
			DWORD dwDesiredThrottle = 0xffffffff;
			//LONG reg_result = RegCreateKeyEx(HKEY_LOCAL_MACHINE,subKey.c_str(),0, NULL, 0, KEY_ALL_ACCESS, NULL, &keyHandle, &dwDisposition);
			//LONG reg_result = RegOpenKeyEx(HKEY_LOCAL_MACHINE,subKey.c_str(),0, KEY_ALL_ACCESS, &keyHandle);
			LONG reg_result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, subKey.c_str(), 0, KEY_QUERY_VALUE, &keyHandle);

			if (reg_result != ERROR_SUCCESS) {
				//printf("\nError opening network throttle registry hive: %d\n",reg_result);
				return;
			}
			//Set the value.

			DWORD disableThrottle = 0;
			DWORD len = sizeof(DWORD);

			reg_result = RegQueryValueEx(keyHandle, Key.c_str(), 0, 0, (LPBYTE) &disableThrottle, &len);
			if (reg_result != ERROR_SUCCESS) {
				printf("\nError opening network throttle registry key: %d\n", reg_result);
			}

			if (disableThrottle != dwDesiredThrottle) {
				printf("\n***WARNING*** Windows network throttling is enabled, value: %d\n", disableThrottle);
				wprintf(L"Please set: HKEY_LOCAL_MACHINE\\%s\nKey: %s to the value: %X\n", subKey.c_str(), Key.c_str(), dwDesiredThrottle);

				//		disableThrottle = 0xffffffff;
				//		reg_result = RegSetValueEx(keyHandle, L"NetworkThrottlingIndex", 0, REG_DWORD, (LPBYTE) &disableThrottle, len);
				//		if(reg_result != ERROR_SUCCESS) {
				//			printf("Error setting network throttle registry key: %d [%s]\n",reg_result,getWindowsAPIError(reg_result).c_str());
				//		}
			}
			RegCloseKey(keyHandle);
		}


	}
} //end namespace
