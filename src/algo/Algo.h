#ifndef ALGO_ALGO_H
#define ALGO_ALGO_H
#include <iostream>
#include <map>
#include <vector>
#include <math.h>
#include <time.h>
#include "json.hpp"
#include "Utils.h"
#include "Point.h"
#include "Vecteur.h"
#include "ellipse.h"
#include "geometry.h"
#include "WindMapData.h"
#include "ConvexHull.hpp"

using json = nlohmann::json;
using namespace std;

typedef vector<BurningPoint> ListeBurningPoint;
typedef vector<Point2D> ListePoint2D;

class Algo{
public:
    Algo(json parametreAlgo, const json &carteVent): carteVent(carteVent){
        // On instancie toute les données membres dans le constructeur
        int height;
        int width;
        try
        {
            contourInitial = parametreAlgo["contourInitial"];
            algorithm = parametreAlgo["algorithm"];
            listeObstacles = parametreAlgo["listeObstacle"];
            temperature = parametreAlgo["temperature"];
            waterReserve = parametreAlgo["waterReserve"];
            exentricite = parametreAlgo["exentricite"];
            rateOfSpread = parametreAlgo["rateOfSpread"];
            duration = timeToDouble(parametreAlgo["duree"]);
            intervalle = timeToDouble(parametreAlgo["intervalle"]);
            nbProcess = parametreAlgo["nbProcess"];
            dimensionCellule = parametreAlgo["dimension"];
            nbDivisionCellule = parametreAlgo["nbDivisionCellule"];
            arcAngle = parametreAlgo["angle"];
            segmentSize= parametreAlgo["segment"];
            segmentSize = 25/segmentSize;
            generation = parametreAlgo["generations"];
            coefficient = parametreAlgo["coef"];
            width = parametreAlgo["largeur"];
            height = parametreAlgo["hauteur"];

        }
        catch (string const& chaine)
        {
            cerr << chaine << endl;
        }
        // initialize carteVentData to optimize access to values
        carteVentData.initialize(carteVent,waterReserve,temperature,exentricite,rateOfSpread,width,height);    // carteVentData contient toute les cellules des la carte des vents
        // rangés dans une matrice (on a donc une fct qui transforme
        // des coordonées géographiques en index et vice-versa...)
        // chaque cellule contient les données de la carte des vents,
        // ainsi que l'ellipse associée à ces données (le calcul de toutes
        // les ellipses se fait lors de l'initialisation (donc ici) )
    }

    json simulation();
    json simulation(string type);

    void afficheProgression(const double &currentValue);

    // getters
    const json getCarteVent() const {return carteVent;}
    const json getContourInitial() const {return contourInitial;}
    const json getListeObstacles() const {return listeObstacles;}
    const string& getAlgorithm() const {return algorithm;}
    double getTemperature() const {return temperature;}
    int getWaterReserve() const {return waterReserve;}
    double getDuration() const {return duration;}
    int getNbProcess() const {return nbProcess;}
    double getDimension() const {return dimensionCellule;}
    int getNbDivisionCellule() const {return nbDivisionCellule;}
    const parsedWindMap &getCarteVentData() const {return carteVentData;}
    int get_generation() const {return generation;}
    double get_coef() const {return coefficient;}

private:
    json carteVent;
    json contourInitial;
    json listeObstacles;
    string algorithm;
    double temperature;
    int waterReserve;
    double arcAngle;
    double segmentSize;
    double duration;
    double intervalle;
    double exentricite;
    double rateOfSpread;
    int nbProcess;
    double dimensionCellule;
    int nbDivisionCellule;
    int generation;
    double coefficient;
    parsedWindMap carteVentData;
    double distanceP(Point3D p1, Point3D p2);
    double distanceP(Point2D p1, Point2D p2);

    bool pointOnLine(Point2D a,Point2D b,Point2D c);
    vector<Point3D> trier(vector<Point3D> list);
    vector<Point3D> trier2(vector<Point3D> list);


    json algoMaillageFixe();
    json algoMaillageVariable();
    ListePoint2D getPointsInitiauxFromJson(const json& contourInitial);
    vector<ListePoint2D> getObstacleFromJson(const json& listeSegment);

    ListeBurningPoint getBurningPointInitiaux(const ListePoint2D &pointsInitiaux, int dimention, int nbDivisionCellule);

    AEllipse getEllipseFromPoint(const Point2D& p);
    Point2D getCellFromPoint(const Point2D& p);
    vector<Point3D> get_contour(map<string,pair<Point3D, Point3D>> betterVicinity,vector<ListePoint2D>& listeObstacles, int t );
    void updateVicinity(map<string,pair<Point3D,Point3D>> *vicinity, map<string, Point3D> burning, vector<vector<Point2D>> listeObstacles,Point3D p,double segment_size);

    string associated_id(const Point2D& p, double segment_size, double cell_dimension);
    void arcs(const Point2D& p, AEllipse& e, vector<Point2D>& result);
    vector<Point2D> handleImpossible(vector<Point2D> points, Point2D point);
    vector<Point3D> get_neighbor(Point2D p);
    pair<const string, pair<Point3D,Point3D>>  get_min_key(map<string, pair<Point3D,Point3D>>& map);
    vector<vector<Point3D>> propagation(ListeBurningPoint& burningPointInitiaux, vector<ListePoint2D>& listeObstacles);

    ListeBurningPoint getBurningPointInitiauxFixe(const ListePoint2D &pointsInitiaux, int dimention, int nbDivisionCellule);

    vector<vector<Point3D>> propagationFixe(ListeBurningPoint& burningPointInitiaux, vector<ListePoint2D>& listeObstacles);
    void updateVicinityFixe(map<string,pair<Point3D,Point3D>> *vicinity, map<string, Point3D> burning,Point3D p,double segment_size);
    vector<Point3D> get_neighbor_Fixe(Point2D p);

    void arcs_Fixe(const Point2D& p, vector<Point2D>& result);

    Point2D getCentreFromPoint(const Point2D& p);
    Point2D getCoinHautGaucheFromPoint(const Point2D& p);
    Point2D getCoinHautDroitFromPoint(const Point2D& p);
    Point2D getCoinBasGaucheFromPoint(const Point2D& p);
    Point2D getCoinBasDroitFromPoint(const Point2D& p);

};
#endif //ALGO_ALGO_H
