#include <iostream>
#include <fstream>
#include <iomanip>
#include "json.hpp"
#include "Algo.h"
using json = nlohmann::json;

int main(int argc, char *argv[])
{

    //    argc = 4;
    //    argv[1]="C:\\Users\\Skaldr\\Desktop\\NewSantolineCedric\\SantolinePyQT5\\parametreAlgo.json";
    //    argv[2]="C:\\Users\\Skaldr\\Desktop\\NewSantolineCedric\\SantolinePyQT5\\maps\\map.json";
    //    argv[3]="C:\\Users\\Skaldr\\Desktop\\NewSantolineCedric\\SantolinePyQT5\\resultatSimulation.json";

    if (argc != 4)
    {
        cerr << "le nombre d'arguments en entree n'est pas valide" << endl
             << "usage: algo.exe [parametreAlgo.json] [carteVent.json] [fichierSortie.json]" << endl;
    }
    json parametreAlgo, carteVent;

    // On récupère les json spécifiés par argv[1] et argv[3]
    std::ifstream inputParametreAlgo(argv[1]), inputCarteVent(argv[2]);
    inputParametreAlgo >> parametreAlgo;
    inputCarteVent >> carteVent;
    // faire la simulation
    if (parametreAlgo["algorithm"] == "double")
    {
        Algo algo(parametreAlgo, carteVent);
        json resultat1 = algo.simulation("2");
        json resultat2 = algo.simulation("1");
        string adress = "C:\\Users\\hbollon\\NewSantoline_test2\\newsantoline\\data\\communication\\resultatSimulation2.json";
        std::ofstream outputResultatSimulation(argv[3]);
        outputResultatSimulation << std::setw(4) << resultat1 << std::endl;
        cout << "simulation done, " << argv[3] << " filled!" << endl;
        std::ofstream outputResultatSimulation2(adress);
        outputResultatSimulation2 << std::setw(4) << resultat2 << std::endl;
        cout << "simulation done, " << adress << " filled!" << endl;
    }
    else
    {
        Algo algo(parametreAlgo, carteVent);
        json resultat = algo.simulation();
        // On écrit le résultat dans le fichier spécifié par argv[3]
        std::ofstream outputResultatSimulation(argv[3]);
        outputResultatSimulation << std::setw(4) << resultat << std::endl;
        cout << "simulation done, " << argv[3] << " filled!" << endl;
    }

    return 0;
}