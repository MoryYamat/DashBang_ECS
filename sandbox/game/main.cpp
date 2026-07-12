// entry point

#include <iostream>
#include "game/app.h"

int main()
{
	app::App app;

	int width = 1920;
	int height = 1080;

	if(!app.Init(width,height))
	{
		std::cerr << "app init failed \n"; 
	}
	else
	{
		app.Loop();
	}

	return 0;

}