#include "pch.h"
#include "notlib.h"

void LoadPlugins()
{
	pluginsCount = 0;
	//WIN32_FIND_DATA file;//описание файла
	//HANDLE hFile = FindFirstFile(L"Plugins\\*.dll", &file);//ищет каталог файла или подкаталог, название которого соответствует указанному имени файла

	//if (hFile == INVALID_HANDLE_VALUE)
	//{
	//	return;
	//}
	/*while (FindNextFile(hFile, &file))
	{
		if (pluginsCount == MAXPLUGINSALLOWED)
		{
			break;
		}
		char buf[256] = "Plugins\\";
		strcat_s(buf, 256, (char*)file.cFileName);
		Plugins[pluginsCount] = LoadLibrary((LPCWSTR)buf);
		++pluginsCount;
	}*/
	//FindClose(hFile);//закрывает успешный FindFirst (и FindNext) поиск файла

	Plugins[pluginsCount] = LoadLibrary(L"D:\\program etc\\projects2\\sp\\nonono\\mainmain\\Plugins\\notcoolplugin1.dll");
	++pluginsCount;
	Plugins[pluginsCount] = LoadLibrary(L"D:\\program etc\\projects2\\sp\\nonono\\mainmain\\Plugins\\notcoolplugin2.dll");
	++pluginsCount;
	printf("Plugins loaded: %d \n\n", pluginsCount);
}

void UnloadPlugins()
{
	if (pluginsCount == 0)
	{
		printf("%s\n\n", "There're no loaded plugins");
		return;
	}
	while (pluginsCount != 0)
	{
		FreeLibrary(Plugins[pluginsCount - 1]);//сообщит Windows, что данная программа DLL больше не использует
		Plugins[pluginsCount - 1] = NULL;
		--pluginsCount;
	}

	printf("%s\n\n", "All plugins were unloaded.");
}

void ShowInfo()
{
	if (pluginsCount == 0)
	{
		printf("%s\n\n", "There're no loaded plugins");
		return;
	}
	DWORD bufdword = 0;
	char buffer[512];
	PluginInfo PI;
	for (int i = 0; i < pluginsCount; ++i)
	{
		PI = (PluginInfo)GetProcAddress(Plugins[i], "GetDescription");
		PI(buffer, 512, &bufdword);

		printf("Plugin%d description: %s\n", i + 1, buffer);

		PI = (PluginInfo)GetProcAddress(Plugins[i], "GetAuthor");
		PI(buffer, 512, &bufdword);

		printf("Author: %s\n\n", buffer);
	}
}

void RunPlugins()
{
	if (pluginsCount == 0)
	{
		printf("%s\n\n", "There're no loaded plugins");
		return;
	}
	PluginExecute PE;
	for (int i = 0; i < pluginsCount; ++i)
	{
		PE = (PluginExecute)GetProcAddress(Plugins[i], "Execute");
		PE();
	}
}


void Exit()
{
	if (pluginsCount != 0)
	{
		while (pluginsCount != 0)
		{
			FreeLibrary(Plugins[pluginsCount - 1]);//сообщит Windows, что данная программа DLL больше не использует
			Plugins[pluginsCount - 1] = NULL;
			--pluginsCount;
		}

		printf("%s\n\n", "All plugins were unloaded.");
	}

	printf("%s", "Program finished...");
}