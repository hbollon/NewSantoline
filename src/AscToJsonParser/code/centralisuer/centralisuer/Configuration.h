#pragma once

#include <string>

struct Configs {
	std::string datas_path;
	std::string qgis_path;
	std::string windninja_path;

};

void parseLine(const std::string& line, Configs& confs);
void parseConf(const std::string& filename, Configs& confs);