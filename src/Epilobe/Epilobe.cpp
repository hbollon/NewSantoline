#include "Epilobe.h"

Epilobe::~Epilobe() = default;

bool Epilobe::isConfigValid()
{
    /*
     * Dans Configuration.h on a initialisé ces 4 valeurs à "0" pour pouvoir savoir si notre objet Configuration
     * est valide (i.e avec readPaths(...) et readEpilobeParams(...) utilisés pour avoir tous nos chemins et paramètres
     * utiles à Epilobe)
     */
    return config->qgis_path != "0" && config->data_path != "0" && config->windninja_path != "0" &&
           config->epilobe_params["axeorigine"] != "0";
}

Epilobe::Epilobe() : m_axe("est"), m_direction(0), m_force(1), m_nb_process(1),
                     m_origin(0, 0), m_dimension(0, 0)
{
    try
    {
        this->config = Configuration::getInstance();
        if (isConfigValid())
        {
            readConfigParams();
        }
        else
        {
            throw NoConfigException("Pas d'instance de Configuration trouvée, créez-en une avant d'utiliser Epilobe.");
        }
    }
    catch (NoConfigException &e)
    {
        cout << e.getMessage();
    }
}

void Epilobe::readConfigParams()
{
    m_axe = config->epilobe_params["axeorigine"].get<std::string>();

    m_direction = config->epilobe_params["direction"].get<double>();

    m_force = config->epilobe_params["force"].get<double>();

    m_nb_process = config->epilobe_params["nbProcess"].get<int>();

    m_origin = std::pair<double, double>(config->epilobe_params["origine"][0].get<double>(),
                                         config->epilobe_params["origine"][1].get<double>());

    m_dimension = std::pair<double, double>(config->epilobe_params["dimension"][0].get<double>(),
                                            config->epilobe_params["dimension"][1].get<double>());
}

bool Epilobe::can_generate()
{
    return m_direction > 0 && m_force > 0 && m_nb_process > 0 &&
           m_dimension.first > 0 && m_dimension.second > 0 && m_origin.first > 0 &&
           m_origin.second > 0 && m_axe.length() > 0;
}

/*** Créé la subzone ***/
bool Epilobe::subzone(const string& tifPath)
{
    std::string raster_de_base = "\"" + tifPath + "\"";
    std::string raster_resultant = "\"" + config->data_path + "\\subzone.tif\"";
    double xmin, xmax, ymin, ymax;

    //m_origin = point au milieu :
    xmin = m_origin.first - (m_dimension.first / 2.0);
    ymin = m_origin.second - (m_dimension.second / 2.0);
    xmax = m_origin.first + (m_dimension.first / 2.0);
    ymax = m_origin.second + (m_dimension.second / 2.0);
    /*
    //m_origin = point en haut à gauche :
    xmin = m_origin.first;
    ymin = m_origin.second;
    xmax = m_origin.first + (m_dimension.first);
    ymax = m_origin.second + (m_dimension.second);
    */

    std::string command = "\"\"" + config->qgis_path + "\\bin\\gdalwarp.exe\" ";

    std::string parameters = "-overwrite -of gtiff -s_srs "
                             "\"+init=IGNF:LAMB93 +proj=lcc +lat_1=49 +lat_2=44 +lat_0=46.5 "
                             "+lon_0=3 +x_0=700000 +y_0=6600000 +ellps=WGS84 +datum=WGS84 "
                             "+towgs84=0.0,0.0,0.0,0.0,0.0,0.0,0.0 +units=m +no_defs\""
                             " -te " +
                             std::to_string(xmin) + ' ' + std::to_string(ymin) + ' ' + std::to_string(xmax) + ' ' + std::to_string(ymax) + ' ' + raster_de_base + ' ' + raster_resultant;

    std::wcout << (command + parameters).c_str() << std::endl;

    return system((command + parameters).c_str()) == 0;
}

/*** Transforme la subzone en subzone.asc ***/
bool Epilobe::alti_to_ascii()
{
    std::string command = "\"\"" + config->qgis_path + "\\bin\\gdal_translate.exe\" ";
    std::string parameters = "-of AAIGrid "
                             "\"" +
                             config->data_path + "\\subzone.tif\" "
                                                 "\"" +
                             config->data_path + "\\subzone.asc\"";

    std::wcout << (command + parameters).c_str() << std::endl;

    return system((command + parameters).c_str()) == 0;
}

/*** Créé la carte des vents à partir de subzone.asc (sortie : subzone_....._25m_{vel/ang}.asc ***/
bool Epilobe::windninja()
{
    std::string input_map = "\"" + config->data_path + "\\subzone.tif\"";
    std::string output_data = "\"" + config->data_path + "\\wind\\\"";
    std::string command = "\"\"" + config->windninja_path + "\\bin\\WindNinja_cli.exe\" ";

    std::string parameters = "--num_threads " + std::to_string(m_nb_process) +
                             " --elevation_file " + input_map +
                             " --initialization_method domainAverageInitialization "
                             "--input_speed " +
                             std::to_string(m_force) +
                             " --input_speed_units mps "
                             "--output_speed_units mps "
                             "--input_direction " +
                             std::to_string(m_direction) +
                             " --output_wind_height 0 "
                             "--units_output_wind_height m "
                             "--units_input_wind_height m "
                             "--input_wind_height 0 "
                             "--vegetation trees "
                             "--mesh_resolution 25 "
                             "--units_mesh_resolution m "
                             "--write_ascii_output true "
                             "--output_points_file " +
                             output_data + ' ';

    std::wcout << (command + parameters).c_str() << std::endl;

    return system((command + parameters).c_str()) == 0;
}

void Epilobe::generate(const string& tifPath)
{
    if (can_generate())
    {
        m_generated = subzone(tifPath) && alti_to_ascii() && windninja();
    }
    else
    {
        std::cout << "La carte des vents n'a pas pu être générée car les tous les paramètres ne sont pas bons...";
    }
}

bool Epilobe::is_generated()
{
    return m_generated;
}

double Epilobe::direction() const
{
    return m_direction;
}

double Epilobe::force() const
{
    return m_force;
}

std::pair<double, double> Epilobe::origin() const
{
    return m_origin;
}

std::pair<double, double> Epilobe::dimension() const
{
    return m_dimension;
}