//{TODO} PATH TO EXECUTABLE
#include "stdafx.h"
#include "MessageEpilobe.h"
#include "CommandLineGenerator.h"

#include <iostream>

MessageEpilobe::MessageEpilobe() : m_axe("est"), m_direction(0), m_force(1), m_nb_process(1),
								   m_origin(0, 0), m_dimension(0, 0)
{
	m_message = MessageType::EPILOBE;
	parseConf("path.ini", m_configs);
}

MessageEpilobe::~MessageEpilobe()
{
}

bool MessageEpilobe::can_generate()
{
	return m_direction > 0 && m_force > 0 && m_nb_process > 0 && m_dimension.first > 0 && m_dimension.second > 0 && m_origin.first > 0 && m_origin.second > 0 && m_axe.length() > 0;
}

bool MessageEpilobe::subzone()
{
	std::string raster_de_base = m_configs.datas_path + "\\maps\\var.tif";
	std::string raster_resultant = m_configs.datas_path + "\\maps\\subzone.tif";
	double xmin, xmax, ymin, ymax;
	xmin = m_origin.first - (m_dimension.first / 2.0);
	ymin = m_origin.second - (m_dimension.second / 2.0);
	xmax = m_origin.first + (m_dimension.first / 2.0);
	ymax = m_origin.second + (m_dimension.second / 2.0);

	std::string commands = "\"" + m_configs.qgis_path + "\\bin\\gdalwarp.exe\"";
	std::vector<std::string> params;
	params.push_back("-overwrite");
	params.push_back("-of gtiff");
	params.push_back("-s_srs \"+init=IGNF:LAMB93 +proj=lcc +lat_1=49 +lat_2=44 +lat_0=46.5 +lon_0=3 +x_0=700000 +y_0=6600000 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs\"");
	params.push_back("-te " + std::to_string(xmin) + " " + std::to_string(ymin) + " " + std::to_string(xmax) + " " + std::to_string(ymax) + " ");
	params.push_back(raster_de_base);
	params.push_back(raster_resultant);
	return cli_call(commands, params);
}

bool MessageEpilobe::alti_to_ascii()
{
	std::string maps = "\"" + m_configs.datas_path + "\\maps\\subzone.tif\"";
	std::string output = "\"" + m_configs.datas_path + "\\datas\\subzone.asc\"";
	std::string command = "\"" + m_configs.qgis_path + "\\bin\\gdal_translate.exe\"";
	std::vector<std::string> parameters;
	parameters.push_back("-of AAIGrid " + maps + " " + output);
	return cli_call(command, parameters);
}

bool MessageEpilobe::windninja()
{
	std::string input_map = "\"" + m_configs.datas_path + "\\maps\\subzone.tif\"";
	std::string output_data = "\"" + m_configs.datas_path + "\\datas\"";
	std::string command = "\"" + m_configs.windninja_path + "\\bin\\WindNinja_cli.exe\"";
	std::vector<std::string> parameters;
	parameters.push_back("--num_threads " + std::to_string(m_nb_process));
	parameters.push_back("--elevation_file " + input_map);
	parameters.push_back("--initialization_method domainAverageInitialization");
	parameters.push_back("--input_speed " + std::to_string(m_force));
	parameters.push_back("--input_speed_units mps");
	parameters.push_back("--output_speed_units mps");
	parameters.push_back("--input_direction " + std::to_string(m_direction));
	parameters.push_back("--output_wind_height 0 ");
	parameters.push_back("--units_output_wind_height m");
	parameters.push_back("--units_input_wind_height m");
	parameters.push_back("--input_wind_height 0");
	parameters.push_back("--vegetation trees");
	parameters.push_back("--mesh_resolution 25");
	parameters.push_back("--units_mesh_resolution m");
	parameters.push_back("--write_ascii_output true");
	parameters.push_back("--output_points_file " + output_data);
	return cli_call(command, parameters);
}

void MessageEpilobe::generate()
{
	subzone();
	alti_to_ascii();
	windninja();
}

bool MessageEpilobe::is_generated()
{
	return m_generated;
}

double MessageEpilobe::direction() const
{
	return m_direction;
}

double MessageEpilobe::force() const
{
	return m_force;
}

std::pair<double, double> MessageEpilobe::origin() const
{
	return m_origin;
}

std::pair<double, double> MessageEpilobe::dimension() const
{
	return m_dimension;
}

void MessageEpilobe::handleMessage(nlohmann::json message)
{
	if (message["epilobe"].find("axeorigine") != message["epilobe"].end())
	{
		m_axe = message["epilobe"]["axeorigine"].get<std::string>();
	}

	if (message["epilobe"].find("direction") != message["epilobe"].end())
	{
		m_direction = message["epilobe"]["direction"].get<double>();
	}

	if (message["epilobe"].find("force") != message["epilobe"].end())
	{
		m_force = message["epilobe"]["force"].get<double>();
	}

	if (message["epilobe"].find("nbProcess") != message["epilobe"].end())
	{
		m_nb_process = message["epilobe"]["nbProcess"].get<int>();
	}
	if (message["epilobe"].find("origine") != message["epilobe"].end())
	{
		m_origin = std::pair<double, double>(message["epilobe"]["origine"][0].get<double>(), message["epilobe"]["origine"][1].get<double>());
	}
	if (message["epilobe"].find("dimension") != message["epilobe"].end())
	{
		m_dimension = std::pair<double, double>(message["epilobe"]["dimension"][0].get<double>(), message["epilobe"]["dimension"][1].get<double>());
	}
	generate();
}