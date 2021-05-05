#include <iostream>
#include <fstream>
#include <iomanip>
#include "json.hpp"
#include "Algo.h"
using json = nlohmann::json;

#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

int main(int argc, char *argv[])
{
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
        string adress = "..\\data\\communication\\resultatSimulation2.json";
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