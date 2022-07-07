#include "primary_app.hpp"
#include "prim_exception.hpp"
#include "logger.hpp"
#include <stdexcept>
#include <iostream>

int main(int argc, char* argv[]) 
{
	try
	{
		prim::PrimaryApp app(argv[0]);
		app.init();
		app.run();
		return 0;
	}
	catch(const prim::Exception& e)
	{
		std::cerr << e.what() << '\n';
		Logger::log(e.what());
		return 1;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		Logger::log(e.what());
		return 1;
	}
	catch(...)
	{
		std::cerr << "Unknown exception was caught!" << '\n';
		Logger::log("Unknown exception was caught!");
		return 1;
	}
	
}