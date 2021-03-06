#pragma once
#include <stdio.h>
#include <windows.h>
#define  MAXPLUGINSALLOWED 10
#ifdef __cplusplus

extern "C"
{
#endif

	typedef void(*PluginExecute)();
	typedef bool(*PluginInfo)(LPSTR, DWORD, DWORD*);
	static HINSTANCE Plugins[MAXPLUGINSALLOWED];
	static int pluginsCount;

	__declspec(dllexport) void LoadPlugins();
	__declspec(dllexport) void UnloadPlugins();
	__declspec(dllexport) void ShowInfo();
	__declspec(dllexport) void RunPlugins();
	__declspec(dllexport) void Exit();

#ifdef __cplusplus
};
#endif