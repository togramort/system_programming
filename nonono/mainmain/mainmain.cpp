#include <stdio.h>
#include <iostream> 
#include <windows.h>
#include "..\sthnotlib\notlib.h"

#pragma comment (lib, "sthnotlib.lib")
//–≈—”–—Õ€≈ ‘¿…À€ - ÀŒ ¿À»«¿÷»ﬂ
int main()
{
	bool menu, choosing;
	int choice;
	menu = true;
	while (menu)
	{
		std::cout << "not awesome menu:\n"
			<< "(1)  upload plugins\n"
			<< "(2)  unload plugins\n"
			<< "(3)  get list of plugins\n"
			<< "(4)  get full info about every plugin\n"
			<< "(5)  use the f- plugin\n"
			<< "so what will you choose? enter just a single number please\n";

		choosing = true;

		while (choosing)
		{
			printf("%s", "and the number is... ");
			scanf_s("%d", &choice);

			switch (choice)
			{
			case 1:
				LoadPlugins();
				choosing = false;
				break;
			case 2:
				UnloadPlugins();
				choosing = false;
				break;
			case 3:
				//ShowInfo();
				choosing = false;
				break;
			case 4:
				ShowInfo();
				choosing = false;
				break;
			case 5:
				RunPlugins();
				choosing = false;
				break;
			case 6:
				Exit();
				choosing = false;
				menu = false;
				break;
			default:
				printf("%s\n\n", "ok and what did you expect huh?\n");
				break;
			}
		}
	}

	system("pause>>NUL");
	return 0;
}