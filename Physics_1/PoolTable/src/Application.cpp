
#include "PhysicsApplication.h"

int main(int argc, char** argv)
{
	PhysicsApplication application;

	Debugger::print = false;

	application.InitializeWindow(1366,728);

	application.Render();

	return -1;
}
