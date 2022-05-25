#include "primary_app.hpp"
#include "prim_exception.hpp"
#include <stdexcept>
#include <iostream>

int main() 
{
	try
	{
		PrimaryApp app;
		app.init();
		app.run();
		return 0;
	}
	catch(const prim::Exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
	catch(...)
	{
		std::cerr << "Unknown exception was caught!" << '\n';
		return 1;
	}
	
}