#include "GraphicsApp.h"

int main()
{
	GraphicsApp* app = new GraphicsApp(2560, 1440, "Computer Graphics");

	app->run();

	delete app;

	return 0;
}