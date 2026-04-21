#include "App.hpp"
#include <exception>
#include <iostream>

int	main()
{
	try
	{
		Scop::App app;
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << '\n';
		return 1;
	}
	catch (...)
	{
		std::cerr << "Unknown error\n";
		return 1;
	}
	return 0;
}
