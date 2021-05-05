#pragma once

#include <utility>
#include <string>
#include <fstream>
#include <iostream>
#include "json.hpp"

using namespace std;

struct Configuration
{ //classe singleton

private:
    Configuration() : data_path("0"), qgis_path("0"), windninja_path("0")
    {
        this->epilobe_params["axeorigine"] = "0";
    }

    nlohmann::json fileToJson(string pathToFile);

public:
    ~Configuration();

    Configuration(const Configuration &) = delete;

    Configuration &operator=(const Configuration &) = delete;

    static Configuration *getInstance()
    {
        static Configuration instance;
        return &instance;
    }

    std::string data_path;
    std::string qgis_path;
    std::string windninja_path;
    std::string asctojsonparser_path;
    std::string wind_map_generation_path;
    nlohmann::json epilobe_params;

    bool readPaths(string pathtoPaths);

    bool readEpilobeParams(string pathToParams);
};
