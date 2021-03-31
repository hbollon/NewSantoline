#include <fstream>
#include "WindMapData.h"

using json = nlohmann::json;
using  namespace std;

void parsedWindMap::initialize(const json& carteVent, int water, double temp, double exentricite, double rateOfSpread, int width, int height) {
    // resize _matrix bound to windMap size
    this->_matrix = std::vector<std::vector<Data*>*> (carteVent.size());
    int size = carteVent.size();

    for(long i = 0 ; i <  width; i++){
        try
        {
            std::vector<Data *> * vec = new std::vector<Data *>;
            vec->resize(height);
            this->_matrix.at(i)= vec;
        }
        catch (std::bad_alloc& ba)
        {
            std::cout << "erreur init map" << std::endl;
        }

    }
    setMinCoordinateOfWindMap(carteVent);

    // initialize fields in matrix
    for (const auto &it : carteVent) {
        double slope = it["slope"];
        double aspect = it["aspect"];
        json jSlopeVector = it["slope_vector"];
        Vector3D  slopeVector = Vector3D(jSlopeVector[0], jSlopeVector[1], jSlopeVector[2]);
        json jWind = it["wind"];
        Vector3D  wind = Vector3D(jWind[0], jWind[1], jWind[2]);
        Vector3D windSlope = Vector3D(it["windSlope"][0],it["windSlope"][1],it["windSlope"][2]);
        Point2D mapCoordinate = Point2D(it["x"], it["y"]);
        Point2D plan = Point2D(it["plan"][0],it["plan"][1]);
        AEllipse ellipse = AEllipse(wind, slopeVector, windSlope, aspect, slope,exentricite,rateOfSpread,temp,water, mapCoordinate,plan);
        Data* d  = new Data(slope, slopeVector, aspect, wind, windSlope, ellipse);
        Point2D index = coordinateToIndex(Point2D(mapCoordinate.x(), mapCoordinate.y()));


        this->_matrix[(int)index.x()]->at((int)index.y()) = d;
    }

    // reduce size of matrix if it's possible
    this->_matrix.shrink_to_fit();
}


void parsedWindMap::setMinCoordinateOfWindMap(const nlohmann::json &carteVent) {
    double xmin = -1;
    double ymin = -1;
    for (const auto &it : carteVent) {
        double tempx  = it["x"];
        double tempy  = it["y"];
        if (tempx < xmin || xmin == -1)
            xmin = tempx;
        if (tempy < ymin || ymin == -1)
            ymin = tempy;
    }
    this->_min_x = xmin;
    this->_min_y = ymin;
}

Point2D parsedWindMap::coordinateToIndex(const Point2D &p) {
    return {(p.x()-_min_x)/25, (p.y()-_min_y)/25};
}

Point2D parsedWindMap::indextoCoordinate(const Point2D &p) {
    return {25*p.x() + _min_x, 25*p.y() + _min_y};
}