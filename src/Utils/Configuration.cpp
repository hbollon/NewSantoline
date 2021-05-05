#include "Configuration.h"

Configuration::~Configuration() = default;

nlohmann::json Configuration::fileToJson(string pathToFile)
{
    nlohmann::json parsedJson;
    try
    {
        //lecture
        std::ifstream i(pathToFile);
        i >> parsedJson;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Configuration.cpp : Erreur lors de la lecture du fichier : "
                  << e.what()
                  << std::endl;
    }
    return parsedJson;
}

bool Configuration::readPaths(string pathtoPaths)
{
    nlohmann::json json = fileToJson(std::move(pathtoPaths));
    this->qgis_path = json["qgis_path"];
    this->data_path = json["data_path"];
    this->windninja_path = json["windninja_path"];
    this->asctojsonparser_path = json["asctojsonparser_path"];
    this->wind_map_generation_path = json["wind_map_generation_path"];
    return true;
}

bool Configuration::readEpilobeParams(string pathToParams)
{
    this->epilobe_params = fileToJson(std::move(pathToParams));
}