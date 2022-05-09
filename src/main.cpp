#include "primary_app.hpp"
#include <stdexcept>
#include <iostream>

int main() 
{
	try
	{
		PrimaryApp app;
		app.run();
		return 0;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}
	
}