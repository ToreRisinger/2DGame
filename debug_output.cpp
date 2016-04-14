#include "debug_output.h"

#include <string>
#include <iostream>

void debug_output(std::string output_text)
{
	std::cout << "[DEBUG] " << output_text << std::endl;
}

void error_output(std::string output_text)
{
	std::cout << "[ERROR] " << output_text << std::endl;
}