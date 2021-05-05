#include "Epilobe.h"

int main(int argc, char *argv[])
{
    cout << "argc: " << argc << endl;
    if (argc < 6)
    {
        cout << "Epilobe attend 5 arguments : les chemins de paths.json, params.json et du fichier .tif associé au département,"
                "la direction et la vitesse du vent";
        return 1;
    }
    cout << 1 << endl;
    Configuration *config = Configuration::getInstance();
    cout << 2 << endl;
    config->readPaths(argv[1]);
    cout << 3 << endl;
    config->readEpilobeParams(argv[2]);
    cout << 4 << endl;
    Epilobe m_epilobe;
    cout << 5 << endl;
    m_epilobe.generate(argv[3]);
    cout << 6 << endl;
    string commande = config->asctojsonparser_path + "\\AscToJsonParser " + config->wind_map_generation_path + " " + argv[4] + " " + argv[5] + " " + config->data_path;
    cout << 7 << endl;
    system(commande.c_str());
    cout << 8 << endl;

    return 0;
}