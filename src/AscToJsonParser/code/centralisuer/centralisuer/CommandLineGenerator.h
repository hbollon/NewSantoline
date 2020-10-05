//{TODO} : TEST
#pragma once

#include <string>
#include <vector>
#include <iostream>

std::string cli_builder(std::string command, std::vector<std::string> parameters)
{
	std::string cli_instruction = "\"" + command + " ";
	for (auto it = parameters.begin(); it != parameters.end(); ++it)
	{
		cli_instruction += *it + " ";
	}
	std::cout << cli_instruction << std::endl;
	return cli_instruction + "\"";
}

bool cli_call(std::string command, std::vector<std::string> parameters)
{
	return !system(cli_builder(command, parameters).c_str());
}
