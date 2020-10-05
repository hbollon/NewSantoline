#include "stdafx.h"

#include "Configuration.h"

#include <string>
#include <fstream>
#include <iostream>

void parseLine(const std::string& line, Configs& confs)
{
	char sep = '=';
	if (line.length() > 0 && line[0] != '#') {
		int pos = 0;
		std::string word = "";
		while (line[pos] != sep && pos < line.length()) {
			word += line[pos];
			++pos;
		}
		if (line[pos] != sep) return;
		++pos;
		if (word.compare("datas_path") == 0)
		{
			confs.datas_path = line.substr(pos);
		}
		else if (word.compare("qgis_path") == 0)
		{
			confs.qgis_path = line.substr(pos);
		}
		else if (word.compare("windninja_path") == 0)
		{
			confs.windninja_path = line.substr(pos);
		}
		else
		{
			std::cerr << "something goes wrong while parsing config file. Check if config file is correct." << std::endl;
		}
	}
}

void parseConf(const std::string& filename, Configs& confs)
{
	system("cd");
	std::ifstream input_file(filename);
	std::string line;
	while (std::getline(input_file, line)) {
		parseLine(line, confs);
	}
	std::cout << "confs : " << confs.datas_path << " : " << confs.qgis_path << " : " << confs.windninja_path << std::endl;
}