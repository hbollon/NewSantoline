#ifndef ALGO_WINDMAPDATA_H
#define ALGO_WINDMAPDATA_H

#include <vector>
#include "Point.h"
#include "Vecteur.h"
#include "json.hpp"
#include "ellipse.h"

class Data {
private:
    double _slope;
    Vector3D _slopeVector;
    double _aspect;
    Vector3D _wind;
    Vector3D _windSlopeVector;
    AEllipse _ellipse;

public:
    Data(double _slope, const Vector3D &_slopeVector, double _aspect, const Vector3D &_wind, const Vector3D &_windSlope, AEllipse _ellipse) :
                        _slope(_slope), _slopeVector(_slopeVector), _aspect(_aspect), _wind(_wind), _windSlopeVector(_windSlope), _ellipse(_ellipse) {}

    // accessseurs
    double get_slope() const {return _slope;}
    const Vector3D &get_slopeVector() const {return _slopeVector;}
    double get_aspect() const {return _aspect;}
    const Vector3D &get_wind() const {return _wind;}
    const Vector3D &get_windSlopeVector() const{return _windSlopeVector;}
    const AEllipse &get_ellipse() const {return _ellipse;}
};

class parsedWindMap{
private:
    std::vector<std::vector <Data*>*> _matrix;
    double _min_x;
    double _min_y;

    void setMinCoordinateOfWindMap(const nlohmann::json& carteVent);
    //void initMatrix(int _max_x, int _max_y);
public:

    void initialize(const nlohmann::json& carteVent, int water, double temp, double exentricite, double rateOfSpread, int width, int height);
    // renvoie l'index dans la matrice d'une coordonée de la carte de vents
    Point2D coordinateToIndex (const Point2D& p);
    // l'inverse
    Point2D indextoCoordinate (const Point2D& p);

    const std::vector<std::vector <Data*>*> &get_matrix() const {return _matrix;}
    double get_min_x() const {return _min_x;}
    double get_min_y() const {return _min_y;}

};

#endif //ALGO_WINDMAPDATA_H
