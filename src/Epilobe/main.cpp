#include "Epilobe.h"

int main (int argc, char *argv[]) {
    std::cout<<"puuuuuuuteuh"<<std::endl;

    if(argc < 5 ){
        cout << "Epilobe attend 4 arguments : les chemins de paths.json et de params.json "
                "et la direction, la vitesse du vent";
        return 1;
    }
    cout<<1<<endl;
    Configuration *config = Configuration::getInstance();
    cout<<2<<endl;
    config->readPaths(argv[1]);
    cout<<3<<endl;
    config->readEpilobeParams(argv[2]);
    cout<<4<<endl;
    Epilobe m_epilobe;
    cout<<5<<endl;
    m_epilobe.generate();
    cout<<6<<endl;
    string commande = config->asctojsonparser_path + "\\AscToJsonParser " + config->wind_map_generation_path + " "
            + argv[3] + " " + argv[4] + " " + config->data_path;
    cout<<7<<endl;
    system(commande.c_str());
    cout<<8<<endl;

    return 0;
}