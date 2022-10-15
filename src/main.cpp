#include "primary_app.hpp"


int main(int argc, char* argv[])
{
	prim::PrimaryApp app;

	app.useEditor = true;

	return app.run();
}