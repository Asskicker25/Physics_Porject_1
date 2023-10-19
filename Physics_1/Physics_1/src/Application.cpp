
#include "PhysicsApplication.h"

int main(int argc, char** argv)
{
	PhysicsApplication application;

	application.InitializeWindow(800,600);

	application.Render();

	return -1;
}
