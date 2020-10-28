//
// Created by Skaldr on 7/11/2019.
//

#ifndef ALGO3_ALGOOPT_HPP
#define ALGO3_ALGOOPT_HPP


#include <vector>
#include <map>
#include <string>
#include "PointOpt.hpp"
#include "Choc.hpp"
#include "EllipseOpt.hpp"
#include "json.hpp"
#include "CoinCase.h"

using json = nlohmann::json;

using  namespace std;
class AlgoOpt {

private:
    double seuil=0.0000001;
    double crit = 0.8;
    std::map<std::string,PointOpt> listff;

    /*Listes auxiliaires*/
    vector<string> listeCellulesBrulees;/*listaux(1)*///Liste des cellules brulees, les cellules qui ont ete active + celles qui sont activent en ce moment
    map<string,vector<string>>  listePointsDansCellule;/*listaux(2)*/
    std::map<std::string, double> listePointsActifs;/*listaux(3)*///Liste des cellules active actuellement
    string aPropager;/*ListAux(4)*/
    std::vector<Choc> listeChoc;/*listaux(5)*/

    map<string,CoinCase> listeCoins;/*LIstaux(7)*/

    int kmax = 20;
    int compteurId =0;
    vector<vector<EllipseOpt >> carteEllipse;
    int tailleCellule=25;
    int xmin;
    int ymin;


    bool affichage = false;//Bool  qui sert a activer l'affichage

public:
    /*Fonctions principales*/
    std::vector<Point3D> propagation(std::vector<Point2D> listE, json m, json p, ofstream& sortie, ofstream& infos_algo, ofstream& etat_final_liste_sommets);
    void initListff(std::vector<Point2D> listPointE);
    void initialiserEllipse(json j, ofstream& sortie, int largeur, int hauteur, int water, int temp);
    void uneIteration(std::string num);
    int propage(string ind, vector<vector<VitesseOpt>> vits);
    bool chocDansCellule(PointOpt mvt, string ind);

    /*Fonctions d'insertion de point*/
    void rajoutePoint(string indice);
    void rajoutePoints(string indice);
    void insere(PointOpt point, std::string ind, bool indic);
    std::pair<Point3D, Vector2D> mnplusun(Point3D point, VitesseOpt vitesse);
    vector<Point2D> raffine(vector<Point2D> listPoint, double crit);


    /*Fonctions de suppression de point*/
    int supprimeCroisementRaj(PointOpt mvt,double tmin, string ind);
    void supprimePointTropProche(string ind, double crit);
    int supprimerCroisement(PointOpt point, std::string ind);
    void supprime(std::string ind);
    void supprimePlus(std::string ind, std::string num);

    /*Fonction de gestion des vitesses*/
    double azero(Vector2D tau, VitesseOpt Vit);
    VitesseOpt vitessea0(double a0, Vector2D tau, Point2D coordonne);
    VitesseOpt vitesseMax(Vector2D direction, Point2D coordonne);
    std::vector<std::vector<VitesseOpt>> vitesseRajoutes(Point2D coordonne, Vector2D v0, Vector2D v1, int kmax);
    std::pair<double,std::vector<std::vector<VitesseOpt>>> vitesseChocOuPas(VitesseOpt vip, VitesseOpt vis);
    VitesseOpt vitesse0(Point2D p, Point2D s);
    pair<bool,vector<vector<VitesseOpt>>> vitesseChocNew(vector<vector<VitesseOpt>> vits, Vector2D tauperp);
    pair<bool,vector<vector<VitesseOpt>>> vitesseNew1(vector<vector<VitesseOpt>> vits, Vector2D tauperp);
    pair<bool,vector<vector<VitesseOpt>>> vitesseSommetNew(vector<vector<VitesseOpt>> vits, Vector2D tauperp);
    VitesseOpt vitesse(VitesseOpt vb, Point2D ij);

    /*Fonction de gestion des bordures*/
    bool testBordureBrulee(PointOpt point);
    void actualiseCoin(string ind);
    void initialiserCoinContourDepart(PointOpt mvt);

    /*Fonctions de gestion*/
    bool saisieEstSensHoraire(vector<Point2D>);
    vector<Point2D> inverseSensList(vector<Point2D>);
    Point2D coordinateToIndex(const Point2D &p);
    Point2D coordinateToCoordinateCalc(const Point2D &p);
    Point2D indextoCoordinate(const Point2D &p) const;
    std::string numero (Point2D ij);
    void setXminYmin(json carteVent);

    /*Fonctions pour l'application de test*/
    void sauvegarde_liste_vitesses(ofstream& contour_final_detail) const;
    void sauvegarder_liste_sommets(ofstream &etat_final_liste_sommets) const;
    pair<double, double> numToCoordinate(const string& basicString) const;



    /*Fonctions d'affichage*/
    void affiche(std::vector<vector<VitesseOpt>> v);
    void affiche(std::pair<double,std::vector<vector<VitesseOpt>>> v);


    /*Autre*/
    string getMinlisteCellulesBrulees();
    PointOpt findPts(vector<PointOpt>,string);
    float det(Vector2D v1, Vector2D v2);
    void triDesChocs();

};


#endif //ALGO3_ALGOOPT_HPP
