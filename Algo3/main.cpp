#include <iostream>
#include <fstream>
#include <iomanip>
#include "Vecteur.h"
#include "Point.h"
#include "EllipseOpt.hpp"
#include "AlgoOpt.hpp"
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

int main(int argc, char* argv[]) {

    // ORIGINE => pied de la cellule (x,y) donnés par le fichier carteVent.json.
    // penser à la conversion

    cout<<"Debut de programme, nombre d'arguments : "<< argc << endl;
    /* Arguments du programme */
    /* "[chemin]\data\communication\parametreAlgo.json" -> Json en entree qui contient tous les parametres pour l'execution de l'algo
     * "[chemin]\data\maps\map.json" -> json en entrre qui contient des informations de terrain de la zone
     * "[chemin]\data\communication\resultatSimulation.json" -> json de sortie qui contient les points du contour de feu final
     * "[chemin]\data\communication\sortie.json" -> json de sortie qui contient les informations de la carte de ellipses
     * "[chemin]\data\communication\points.json" -> json de sortie qui contient le detail des points
     * "[chemin]\data\communication\infos.json" -> json de sortie qui contient des informations sur la simulation
     * "[chemin]\data\communication\listeSommets.json" -> Json de sortie qui contient les informations sur les sommets qui ont ete traitees pendant l'execution de l'algo
     * */

    json parametreAlgo, carteVent;

    // On récupère les json spécifiés par argv[1] et argv[2]
    std::ifstream inputParametreAlgo(argv[1]),inputCarteVent(argv[2]);

    inputParametreAlgo >> parametreAlgo;
    inputCarteVent >> carteVent;

    std::vector<Point2D> listPointE;
    //On recupere le contour de feu initial
    json contourInitial = parametreAlgo["contourInitial"];

    //On transforme ce qu'on a recupere du json en objets utilisable pas l'algo
    for (const auto &it : contourInitial){
        listPointE.emplace_back(Point2D(it));
    }

    AlgoOpt algoOpt = AlgoOpt();
    //recuperation des jsons de sortie
    ofstream outputResultatSimulation(argv[3]);
    ofstream sortie(argv[4]);
    ofstream contour_final_detail(argv[5]);
    ofstream infos_algo(argv[6]);
    ofstream etat_final_liste_sommets(argv[7]);
    
    //On lance l'algorithme avec la fonction propagation, ce qui ressort de cette fonction est la liste des points du contour final.
    std::vector<Point3D> burned = algoOpt.propagation(listPointE,carteVent,parametreAlgo, sortie, infos_algo, etat_final_liste_sommets);

    json temp;
    /*On ajoute a un json les informations sur les points du contour final*/
    for (const auto &it : burned){
        temp.push_back(it.toTuple()); //[x,y,z] (z=date d'ignition)
    }
    json result;
    result.push_back(temp);

    //On ajoute au fichier json de sortie les informations de l'objet json
    outputResultatSimulation << std::setw(3) << result << std::endl;
    std::cout << "simulation done, " << argv[3] << " filled!" << std::endl;

    /*On sauvegarde les informations pour l'application de test*/
    algoOpt.sauvegarde_liste_vitesses(contour_final_detail);
   
    return 0;
}