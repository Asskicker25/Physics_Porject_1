
#include "PhysicsApplication.h"


int main(int argc, char** argv)
{
	PhysicsApplication application;

	Debugger::print = true;

	application.InitializeWindow(1366, 728);

	application.Render();

	return -1;
}

