#include "app/App.hpp"
#include "app/CliParser.hpp"

#include <exception>
#include <iostream>

int	main(int argc, char** argv)
{
	try
	{
		Scop::AppOptions options = Scop::CliParser::parse(argc, argv);
		Scop::App app(options);
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
