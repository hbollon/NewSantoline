#include <algorithm>
#include <iomanip>
#include <fstream>
#include "Algo.h"


using namespace std;

json Algo::simulation()
{

    std::cout << "debut simulation!! \n";

    json resultat;

    try
    {

        if (Algo::getAlgorithm() == "2")
        {

            return algoMaillageVariable();
        }
        else if (Algo::getAlgorithm() == "1")
        {

            return algoMaillageFixe();
        }
        else
        {

            throw string("aucun algo disponipble ne s'appelle: " + Algo::getAlgorithm());
        }
    }

    catch (string const &chaine)
    {

        cerr << chaine << endl;
    }

    std::cout << "fin simulation \n";

    return resultat;
}

json Algo::simulation(string type)
{

    std::cout << "debut simulation \n";

    json resultat;

    try
    {

        if (type == "2")
        {

            return algoMaillageVariable();
        }
        else if (type == "1")
        {

            return algoMaillageFixe();
        }
        else
        {

            throw string("aucun algo disponipble ne s'appelle: " + Algo::getAlgorithm());
        }
    }

    catch (string const &chaine)
    {

        cerr << chaine << endl;
    }

    std::cout << "fin simulation \n";

    return resultat;
}

/*** Algorithmes:***/

/*
 * 1) On part des cellules emmetrices,
 * 2) On la découpe en plusieurs segments
 * 3) pour chaque burning point, on calcule l'éllispe correspondante dans la cellule associé
 * 4) pour chaque éllipse, on calcule les arcs et pour chaque arc, on regarde le burning  point associé sur le segment de la cellule
 *  (l'ensembles des burning point potentiels est appelé vicinity
 * 5) on fait bruler le point avec la date d'ignition la plus proche
 */
json Algo::algoMaillageVariable()
{

    json result;

    time_t debut;
    time(&debut);

    // initialisation
    ListePoint2D pointsInitiaux = getPointsInitiauxFromJson(this->contourInitial); //TODO éviter la duplication des données lors du return
    vector<vector<Point2D>> listeObstacles = getObstacleFromJson(this->listeObstacles);
    ListeBurningPoint burningPointInitiaux = getBurningPointInitiaux(pointsInitiaux, this->dimensionCellule, this->nbDivisionCellule); //TODO éviter la duplication des données lors du return
    vector<vector<Point3D>> burned = propagation(burningPointInitiaux, listeObstacles);                                                // renvoie une liste des points qui ont brulés (trié) avec z = date d'ignition

    // Jsonifie la liste des points dans result
    for (const auto &it2 : burned)
    {

        json temp;

        for (const auto &it : it2)
        {

            temp.push_back(it.toTuple()); //[x,y,z] (z=date d'ignition)
        }

        result.push_back(temp);
    }

    time_t fin;
    time(&fin);

    cout << endl
         << "temps simulation = " << difftime(fin, debut) << " secondes" << endl;

    return result;
}

json Algo::algoMaillageFixe()
{

    json result;

    time_t debut;
    time(&debut);

    // initialisation
    ListePoint2D pointsInitiaux = getPointsInitiauxFromJson(this->contourInitial); //TODO éviter la duplication des données lors du return
    vector<vector<Point2D>> listeObstacles = getObstacleFromJson(this->listeObstacles);
    ListeBurningPoint burningPointInitiaux = getBurningPointInitiauxFixe(pointsInitiaux, this->dimensionCellule, this->nbDivisionCellule); //TODO éviter la duplication des données lors du return
    vector<vector<Point3D>> burned = propagationFixe(burningPointInitiaux, listeObstacles);                                                // renvoie une liste des points qui ont brulés (trié) avec z = date d'ignition

    // Jsonifie la liste des points dans result
    for (const auto &it2 : burned)
    {

        json temp;

        for (const auto &it : it2)
        {

            temp.push_back(it.toTuple()); //[x,y,z] (z=date d'ignition)
        }

        result.push_back(temp);
    }

    time_t fin;
    time(&fin);

    cout << endl
         << "temps simulation = " << difftime(fin, debut) << " secondes" << endl;

    return result;
}

/*** Fonctions associées aux algorithmes ***/

ListePoint2D Algo::getPointsInitiauxFromJson(const json &contourInitial)
{

    ListePoint2D pointsInitiaux;

    for (const auto &it : contourInitial)
    {

        pointsInitiaux.emplace_back(Point2D(it));
    }

    return pointsInitiaux;
}

vector<vector<Point2D>> Algo::getObstacleFromJson(const json &listeSegment)
{

    vector<vector<Point2D>> liste;

    for (const auto &it : listeSegment)
    {

        vector<Point2D> listeTemp;

        for (const auto &it2 : it)
        {

            listeTemp.emplace_back(Point2D(it2));
        }

        liste.push_back(listeTemp);
    }

    return liste;
}

/* On détermine les burning points initiaux à partir du contour de feu
 *(voir documentation https://docs.google.com/document/d/1uxjVtTK7xihsBscGaRZe4vKXCgWgosstKbmFNS3vm8Y/edit?usp=sharing ) */
ListeBurningPoint Algo::getBurningPointInitiaux(const ListePoint2D &pointsInitiaux, int dimention, int nbDivisionCellule)
{

    ListeBurningPoint burningPointInitiaux;

    for (auto it = pointsInitiaux.begin(); it != pointsInitiaux.end(); ++it)
    { // On veut parcourir tous les segments du contour initial

        // création du segment
        Point2D p1 = *it;
        Point2D p2;

        if (it + 1 == pointsInitiaux.end())
        { // si on est sur le dernier point

            p2 = *pointsInitiaux.begin(); // on prend le premier point comme point suivant
        }
        else
        {

            p2 = *(it + 1);
        }

        Vector2D segmentContour = Vector2D(p2 - p1);
        double normeSegmentContour = segmentContour.norm(); // On stocke la norme (c'est plus opti)
        Vector2D segmentContourUnitaire = segmentContour / normeSegmentContour;

        // On veut définir un ensemble de burning points sur ce segment (un tous les "tailleCellule/nbDivisionCellule")
        double pas = (double)dimention / nbDivisionCellule;

        for (int i = 0; i * pas < normeSegmentContour; ++i)
        {

            Point2D newPoint = p1 + i * pas * segmentContourUnitaire;
            BurningPoint newBurningPoint = BurningPoint(newPoint, 0.0); // La date d'ignition est t=0.0 secondes
            burningPointInitiaux.push_back(newBurningPoint);
        }
    }

    return burningPointInitiaux;
}

ListeBurningPoint Algo::getBurningPointInitiauxFixe(const ListePoint2D &pointsInitiaux, int dimention, int nbDivisionCellule)
{

    ListeBurningPoint burningPointInitiaux;

    for (auto it = pointsInitiaux.begin(); it != pointsInitiaux.end(); ++it)
    { // On veut parcourir tous les segments du contour initial

        // création du segment
        Point2D p1 = *it;
        Point2D p2;

        if (it + 1 == pointsInitiaux.end())
        { // si on est sur le dernier point

            p2 = *pointsInitiaux.begin(); // on prend le premier point comme point suivant
        }
        else
        {

            p2 = *(it + 1);
        }

        Vector2D segmentContour = Vector2D(p2 - p1);
        double normeSegmentContour = segmentContour.norm(); // On stocke la norme (c'est plus opti)
        Vector2D segmentContourUnitaire = segmentContour / normeSegmentContour;

        // On veut définir un ensemble de burning points sur ce segment (un tous les "tailleCellule/nbDivisionCellule")
        double pas = (double)dimention / nbDivisionCellule;

        for (int i = 0; i * pas < normeSegmentContour; ++i)
        {

            Point2D newPoint = p1 + i * pas * segmentContourUnitaire;
            BurningPoint newBurningPoint = BurningPoint(newPoint, 0.0); // La date d'ignition est t=0.0 secondes
            burningPointInitiaux.push_back(newBurningPoint);
        }
    }

    return burningPointInitiaux;
}

bool onSegment(Point2D p, Point2D q, Point2D r)
{

    if (q.x() <= max(p.x(), r.x()) && q.x() >= min(p.x(), r.x()) &&
        q.y() <= max(p.y(), r.y()) && q.y() >= min(p.y(), r.y()))
    {

        return true;
    }

    return false;
}

int orientation(Point2D p, Point2D q, Point2D r)
{

    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/
    // for details of below formula.
    int val = (q.y() - p.y()) * (r.x() - q.x()) -
              (q.x() - p.x()) * (r.y() - q.y());

    if (val == 0)
        return 0; // colinear

    return (val > 0) ? 1 : 2; // clock or counterclock wise
}

bool doIntersect(Point2D p1, Point2D q1, Point2D p2, Point2D q2)
{

    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1))
        return true;

    // p1, q1 and q2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1))
        return true;

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2))
        return true;

    // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2))
        return true;

    return false; // Doesn't fall in any of the above cases
}

Point2D lineLineIntersection(Point2D A, Point2D B, Point2D C, Point2D D)
{

    // Line AB represented as a1x + b1y = c1
    double a1 = B.y() - A.y();
    double b1 = A.x() - B.x();
    double c1 = a1 * (A.x()) + b1 * (A.y());

    // Line CD represented as a2x + b2y = c2
    double a2 = D.y() - C.y();
    double b2 = C.x() - D.x();
    double c2 = a2 * (C.x()) + b2 * (C.y());

    double determinant = a1 * b2 - a2 * b1;

    if (determinant == 0)
    {

        // The lines are parallel. This is simplified
        // by returning a pair of FLT_MAX
        return Point2D(-9999, -9999);
    }
    else
    {

        double x = (b2 * c1 - b1 * c2) / determinant;
        double y = (a1 * c2 - a2 * c1) / determinant;
        return Point2D(x, y);
    }
}

/***Fonctions de mr.Porcheron***/

vector<vector<Point3D>> Algo::propagation(ListeBurningPoint &burningPointInitiaux, vector<vector<Point2D>> &listeObstacles)
{
    double segment_size = segmentSize;
    //burning stock les points en feu
    map<string, Point3D> burning;
    //betterVicinity stock des pairs de points : un point futur, et son ancêtre
    map<string, pair<Point3D, Point3D>> betterVicinity; //first-> ancêtre et second-> futur

    cout << "initialisation..." << endl;

    //On rempli la map "burning" avec la liste "burningPointInitiaux"
    for (ListeBurningPoint::const_iterator it = burningPointInitiaux.begin(); it != burningPointInitiaux.end(); ++it)
    {

        burning[associated_id(*it, segment_size, dimensionCellule)] = Point3D(it->x(), it->y(), 0);
    }

    vector<Point3D> voisins;
    vector<vector<Point3D>> arctab;

    // on détermine la premiere vicinity en parcourant l'enssemble des points "burning"
    for (map<string, Point3D>::const_iterator it = burning.begin(); it != burning.end(); ++it)
    {
        updateVicinity(&betterVicinity, burning, listeObstacles, it->second, segment_size);
    }

    //contour stock des tableaux de points qui corespondent chacun a un contour de feu
    // (il est possible qu'il y ai un ou plusieur contour comme l'utilisateur le desire
    // si il y en a plusieurs, il s'agit de contours intermediaires calcules a intervalle regulier)
    vector<vector<Point3D>> contour;

    //contourCourant stock le contour intermediaire courant
    vector<Point3D> contourCourant;

    vector<Point3D> intersectionObstacle;

    //On recupere le point a la date d'ignition la plus faible de la vicinity
    pair<string, pair<Point3D, Point3D>> pointAMettreEnFeu = get_min_key(betterVicinity);

    //durationInte est la prochaine date pour laquelle il faut calculer un contour,
    //on l'initialise avec la valeur intervalle qui est un attribut de la classe Algo
    //celle-ci est recuperee dans le fichier parametreAlgo.json au moment de l'instanciation de de Algo
    int durationInte = intervalle;
    int lastProgressValue = 0;

    //Tant que le prochain point a mettre en feu a une date d'ignition inferieur a la duree de la simulation on parcours la vicinity
    while (pointAMettreEnFeu.second.second.z() < duration)
    {
        //Si intervalle est superieur a 0, alors il y a des contours intermediaires a calculer
        if (intervalle > 0)
        {
            //Si la date d'ignition de "pointAMettreEnFeu" est superieur a "durationInte" alors il faut calculer un contour
            if (pointAMettreEnFeu.second.second.z() > durationInte)
            {
                contourCourant = get_contour(betterVicinity, listeObstacles, durationInte);
                contourCourant.insert(contourCourant.end(), intersectionObstacle.begin(), intersectionObstacle.end());
                contour.push_back(trier(contourCourant));
                vector<Point3D> contourCourant;
                durationInte += intervalle;
            }
        }

        string key = pointAMettreEnFeu.first;
        vector<Point3D> temp;
        temp.push_back(pointAMettreEnFeu.second.second);
        temp.push_back(pointAMettreEnFeu.second.first);
        arctab.push_back(temp);

        Point3D a = pointAMettreEnFeu.second.second;
        Point2D a0 = Point2D(pointAMettreEnFeu.second.first.x(), pointAMettreEnFeu.second.first.y());
        Point2D a1 = Point2D(a.x(), a.y());
        if ((int)a.z() % 10 == 0){
            int progressValue = int(round(a.z() / duration * 100));
            if(progressValue > lastProgressValue){
                lastProgressValue = progressValue;
                std::cout << progressValue << endl;
            }
        }
        bool intersection = false;

        for (vector<vector<Point2D>>::const_iterator it = listeObstacles.begin(); it != listeObstacles.end(); ++it)
        {
            Point2D p0 = it->at(0);
            for (vector<Point2D>::const_iterator it2 = it->begin(); it2 != it->end(); ++it2)
            {
                Point2D p1 = *it2;
                if (intersect(p0, p1, a0, a1))
                {
                    Point2D temp = intersectionPoint(a0, a1, p0, p1);
                    a = Point3D(temp.x(), temp.y(), a.z());
                    intersectionObstacle.push_back(a);
                    intersection = true;
                }
                p0 = p1;
            }
        }

        betterVicinity.erase(key);

        //Si la propagation n'a pas ete interompue par un obstacle, alors on actualise la vicinity comme la premiere fois
        if (!intersection)
        {
            burning[key] = a;
            updateVicinity(&betterVicinity, burning, listeObstacles, a, segment_size);
        }
        pointAMettreEnFeu = get_min_key(betterVicinity);
    }

    //#######################################################################################################################################
    //Enfin on ajoute le dernier contour
    vector<Point3D> finalContour = get_contour(betterVicinity, listeObstacles, duration);
    finalContour.insert(finalContour.end(), intersectionObstacle.begin(), intersectionObstacle.end());
    for (auto &p : contourCourant)
    {
        finalContour.push_back(p);
    }

    //######################################################################################################################

    for (auto &p : betterVicinity)
    {
        vector<Point3D> temp;
        temp.push_back(p.second.first);
        temp.push_back(p.second.second);
        arctab.push_back(temp);
    }

    json result;

    for (const auto &it2 : arctab)
    {

        json temp;

        for (const auto &it : it2)
        {

            temp.push_back(it.toTuple());
        }

        result.push_back(temp);
    }
 
    // Hull
    // 1 -> Concave Hull 
    // 2 -> Convex Hull
    // Check folder's readme for more informations
    if (courbeEnveloppe == 1) {
        export3dPointsTo2dFile(finalContour);
        int retCode = system("..\\src\\algo\\concave.exe points.txt -out points2.txt");
        finalContour = import3dPointsTo2dFile();
        contour.push_back(finalContour);
    } else if (courbeEnveloppe == 2) {
        contour.push_back(makeConvexHull(finalContour));
    } else {
        std::cerr << "Error : bad hull method" << std::endl;
        contour.push_back(finalContour);
    }

    return contour;
}

vector<vector<Point3D>> Algo::propagationFixe(ListeBurningPoint &burningPointInitiaux, vector<vector<Point2D>> &listeObstacles)
{

    double segment_size = segmentSize;
    //burning stock les points en feu
    map<string, Point3D> burning;
    //betterVicinity stock des pairs de points : un point futur, et son ancêtre
    map<string, pair<Point3D, Point3D>> betterVicinity; //first-> ancêtre et second-> futur

    cout << "initialisation fixe..." << endl;

    //On rempli la map "burning" avec la liste "burningPointInitiaux"
    for (ListeBurningPoint::const_iterator it = burningPointInitiaux.begin(); it != burningPointInitiaux.end(); ++it)
    {
        burning[associated_id(*it, segment_size, dimensionCellule)] = Point3D(it->x(), it->y(), 0);
    }
    vector<Point3D> voisins;
    // on détermine la premiere vicinity en parcourant l'enssemble des points "burning"
    for (map<string, Point3D>::const_iterator it = burning.begin(); it != burning.end(); ++it)
    {
        updateVicinityFixe(&betterVicinity, burning, it->second, segment_size);
    }

    int last = 0;

    //contour stock des tableaux de points qui corespondent chacun a un contour de feu
    // (il est possible qu'il y ai un ou plusieur contour comme l'utilisateur le desire
    // si il y en a plusieurs, il s'agit de contours intermediaires calcules a intervalle regulier)
    vector<vector<Point3D>> contour;

    //contourCourant stock le contour intermediaire courant
    vector<Point3D> contourCourant;

    //On recupere le point a la date d'ignition la plus faible de la vicinity
    pair<string, pair<Point3D, Point3D>> pointAMettreEnFeu = get_min_key(betterVicinity);

    //durationInte est la prochaine date pour laquelle il faut calculer un contour,
    //on l'initialise avec la valeur intervalle qui est un attribut de la classe Algo
    //celle-ci est recuperee dans le fichier parametreAlgo.json au moment de l'instanciation de de Algo
    int durationInte = intervalle;

    //Tant que le prochain point a mettre en feu a une date d'ignition inferieur a la duree de la simulation on parcours la vicinity
    while (pointAMettreEnFeu.second.second.z() < duration)
    {
        //Si intervalle est superieur a 0, alors il y a des contours intermediaires a calculer
        if (intervalle > 0)
        {
            //Si la date d'ignition de "pointAMettreEnFeu" est superieur a "durationInte" alors il faut calculer un contour
            if (pointAMettreEnFeu.second.second.z() > durationInte)
            {
                contourCourant = get_contour(betterVicinity, listeObstacles, durationInte);
                contour.push_back(trier(contourCourant));
                vector<Point3D> contourCourant;
                durationInte += intervalle;
            }
        }

        string key = pointAMettreEnFeu.first;
        pointAMettreEnFeu = get_min_key(betterVicinity);
        Point3D a = pointAMettreEnFeu.second.second;
        Point2D a0 = Point2D(betterVicinity[key].first.x(), betterVicinity[key].first.y());
        Point2D a1 = Point2D(a.x(), a.y());
        //Sert uniquement a afficher la bar de progression
        if ((int)a.z() % 10 == 0)
            afficheProgression(a.z());

        if (floor(a.z()) != last)
        {
            last = floor(a.z());
        }
        bool intersection = false;

        //On verifie dans cette boucle si le segment forme par le point a mettre en feu et son ancetre
        //coupe un segment obstacle, il faut alors que le point d'intercetion se trouve sur le contour
        //et que celui ci ne puisse pas etre propageant
        for (auto &obstacle : listeObstacles)
        {
            Point2D b0 = obstacle.at(0);
            for (auto &b1 : obstacle)
            {
                if (doIntersect(a0, a1, b0, b1))
                {
                    Point2D temp = lineLineIntersection(a0, a1, b0, b1);
                    a = Point3D(temp.x(), temp.y(), a.z());
                    contourCourant.push_back(a);
                    intersection = true;
                }

                b0 = b1;
            }
        }

        betterVicinity.erase(key);

        //Si la propagation n'a pas ete interompue par un obstacle, alors on actualise la vicinity comme la premiere fois
        if (!intersection)
        {
            burning[key] = a;
            updateVicinityFixe(&betterVicinity, burning, a, segment_size);
        }
    }
    //Enfin on ajoute le dernier contour
    contour.push_back(trier(get_contour(betterVicinity, listeObstacles, duration)));

    return contour;
}

vector<Point3D> Algo::get_contour(map<string, pair<Point3D, Point3D>> betterVicinity, vector<ListePoint2D> &listeObstacles, int t)
{
    vector<Point3D> contour;
    for (map<string, pair<Point3D, Point3D>>::const_iterator it = betterVicinity.begin(); it != betterVicinity.end(); ++it)
    {
        Point3D pointPrecedent = it->second.first;
        Point3D pointSuivant = it->second.second;
        Point2D pointPrecedent2D = Point2D(pointPrecedent.x(), pointPrecedent.y());
        double deltaT = pointSuivant.z() - pointPrecedent.z();
        double percent = (t - pointPrecedent.z()) / deltaT;
        if (percent >= 0.0)
        {
            double xfinal = pointPrecedent.x() * (1 - percent) + pointSuivant.x() * percent;
            double yfinal = pointPrecedent.y() * (1 - percent) + pointSuivant.y() * percent;
            Point3D pointFinal = Point3D(xfinal, yfinal, t);
            Point2D pointFinal2D = Point2D(pointFinal.x(), pointFinal.y());
            for (auto &obstacle : listeObstacles)
            {
                Point2D b0 = obstacle.at(0);
                for (auto &b1 : obstacle)
                {

                    if (doIntersect(pointPrecedent2D, pointFinal2D, b0, b1))
                    {
                        Point2D temp = lineLineIntersection(pointPrecedent2D, pointFinal2D, b0, b1);
                        pointFinal = Point3D(temp.x(), temp.y(), pointFinal.z());
                    }
                    b0 = b1;
                }
            }

            contour.push_back(pointFinal);
        }
    }
    return (contour);
}

vector<Point3D> Algo::trier(vector<Point3D> list){
    time_t debut;
    time(&debut);

    vector<Point3D> result;
    Point3D p0 = *list.begin();
    result.push_back(p0);
    list.erase(list.begin());
    vector<Point3D>::const_iterator temp;
    Point3D p1;
    int i =0;
    while(list.size()>1){
        temp=list.begin();
        p1=*temp;

        for(vector<Point3D>::const_iterator it =list.begin(); it != list.end(); ++it){
            if (distanceP(p0, *it) < distanceP(p0, *temp) ) {
                temp = it;
                p1=*it;

            }
        }
        if (distanceP(p0, p1) > 1 && distanceP(p0, p1) < 100){
            result.push_back(p1);
            p0=p1;
        }

        list.erase(temp);

    }
    time_t fin;
    time(&fin);

    return result;
}

double Algo::distanceP(Point3D p1, Point3D p2)
{
    double x = p1.x() - p2.x();
    double y = p1.y() - p2.y();
    return sqrt(x * x + y * y);
}

double Algo::distanceP(Point2D p1, Point2D p2)
{
    double x = p1.x() - p2.x();
    double y = p1.y() - p2.y();
    return sqrt(x * x + y * y);
}

void Algo::updateVicinity(map<string, pair<Point3D, Point3D>> *vicinity, map<string, Point3D> burning, vector<vector<Point2D>> listeObstacles, Point3D p, double segment_size)
{

    //On recupere un ensemble de points pouvant etre mis en feu par le point "p"
    vector<Point3D> voisins = get_neighbor(Point2D(p.x(), p.y()));
    Point2D p2D = Point2D(p.x(), p.y());
    //On parcours cette liste
    for (auto &voisin : voisins)
    {

        bool intersection = false;

        Point2D voisin2D = Point2D(voisin.x(), voisin.y());
        //Comme on range les points dans une "map", on calcul son identifiant
        for (auto &obstacle : listeObstacles)
        {
            Point2D b0 = obstacle.at(0);
            for (auto &b1 : obstacle)
            {

                if (doIntersect(p2D, voisin2D, b0, b1))
                {
                    intersection = true;
                }

                b0 = b1;
            }
        }
        string id = associated_id(Point2D(voisin.x(), voisin.y()), segment_size, dimensionCellule);
        if (burning.find(id) == burning.end())
        {
            // sert à définir un point à l'intérieur de la cellule
            // afin de choisir la bonne ellipse (temp n'appartient qu'a une seule cellule)
            Point3D temporary = (p + voisin) / 2.0;
            //On calcul la date d'ignition du point futur grace a l'ellipse corespondante et au point "burning"
            double time = getEllipseFromPoint(Point2D(temporary.x(), temporary.y()))
                              .time(Point2D(p.x(), p.y()),
                                    Point2D(voisin.x(), voisin.y()));

            if (vicinity->find(id) == vicinity->end()         //Si il n'y a pas de point pour l'identifiant id
                || time + p.z() < (*vicinity)[id].second.z()) //ou si le nouveau point calcule a une date d'ignition inferieur a celui qui se trouve a l'identifiant "id"
            {
                //Alors on ajoute a la vicinite le nouveau point calcule et son ancetre
                voisin.z() = time + p.z();
                std::pair<Point3D, Point3D> myPair = std::make_pair(p, voisin);
                if (intersection)
                {
                    id += 'a';
                }
                (*vicinity)[id] = myPair;
            }
        }
    }
}

void Algo::updateVicinityFixe(map<string, pair<Point3D, Point3D>> *vicinity, map<string, Point3D> burning, Point3D p, double segment_size)
{

    //On recupere un ensemble de points pouvant etre mis en feu par le point "p"
    vector<Point3D> voisins = get_neighbor_Fixe(Point2D(p.x(), p.y()));
    //std::cout<<voisins.size()<<std::endl;
    //On parcours cette liste
    for (auto &voisin : voisins)
    {
        //Comme on range les points dans une "map", on calcul son identifiant
        string id = associated_id(Point2D(voisin.x(), voisin.y()), segment_size, dimensionCellule);
        if (burning.find(id) == burning.end())
        {
            // sert à définir un point à l'intérieur de la cellule
            // afin de choisir la bonne ellipse (temp n'appartient qu'a une seule cellule)
            Point3D temporary = (p + voisin) / 2.0;

            //On calcul la date d'ignition du point futur grace a l'ellipse corespondante et au point "burning"
            double time = getEllipseFromPoint(Point2D(temporary.x(), temporary.y()))
                              .time(Point2D(p.x(), p.y()),
                                    Point2D(voisin.x(), voisin.y()));

            if (vicinity->find(id) == vicinity->end()         //Si il n'y a pas de point pour l'identifiant id
                || time + p.z() < (*vicinity)[id].second.z()) //ou si le nouveau point calcule a une date d'ignition inferieur a celui qui se trouve a l'identifiant "id"
            {
                //Alors on ajoute a la vicinite le nouveau point calcule et son ancetre
                voisin.z() = time + p.z();
                (*vicinity)[id].second = voisin;
                std::pair<Point3D, Point3D> myPair = std::make_pair(p, voisin);
                (*vicinity)[id] = myPair;
            }
        }
    }
}

vector<Point3D> Algo::get_neighbor(Point2D p)
// retourne tout les points touché par les arcs de l'ellipse(déterminée au point p) avec une date d'ignition = 200
{

    AEllipse a, b;
    if (fmod(p.x(), dimensionCellule) == 0)
    { // p est su un bord vertical de cellule
        // on calcule les ellipse associées aux 2 cellules adjacentes
        a = getEllipseFromPoint(p + Point2D(1, 0));
        b = getEllipseFromPoint(p - Point2D(1, 0));
    }
    else
    { // p est sur un bord horizontal de cellule  (/!\ ici on suppose donc que p ne peux pas etre à l'intérieur d'une cellule)
        // on calcule les ellipse associées aux 2 cellules adjacentes
        a = getEllipseFromPoint(p + Point2D(0, 1));
        b = getEllipseFromPoint(p - Point2D(0, 1));
    }

    // on calcule les points touchés par les arcs des ellipses (arc_a et arc_b)
    vector<Point2D> arcs_a;
    vector<Point2D> arcs_b;
    arcs(p, a, arcs_a);
    arcs(p, b, arcs_b); // jusqu'ici ça va, j'arrive à suivre...

    arcs_a.insert(arcs_a.end(), arcs_b.begin(), arcs_b.end()); // on ajoute aux points touchés par l'ellipse 'a' les pointeurs
    // arcs_a.end(), arcs_b.begin(), arcs_b.end()...
    // HEIN???
    //arcs_a = handleImpossible(arcs_a, p);   // on va dire que ça sert a ne garder que les points provenant de a
    // ou les points provenants de b

    vector<Point3D> result;
    for (vector<Point2D>::const_iterator it = arcs_a.begin(); it != arcs_a.end(); ++it)
    { // on initialise la date d'ignition de points trouvés à 200 ->??

        result.emplace_back(it->x(), it->y(), 200);
    }

    return result;
}

vector<Point3D> Algo::get_neighbor_Fixe(Point2D p)
{
    // retourne tout les points touché par les arcs de l'ellipse(déterminée au point p) avec une date d'ignition = 200

    vector<Point2D> arcs;

    arcs_Fixe(getCentreFromPoint(p), arcs);
    arcs_Fixe(getCoinBasGaucheFromPoint(p), arcs);
    arcs_Fixe(getCoinBasDroitFromPoint(p), arcs);
    arcs_Fixe(getCoinHautGaucheFromPoint(p), arcs);
    arcs_Fixe(getCoinHautDroitFromPoint(p), arcs);

    vector<Point3D> result;
    for (vector<Point2D>::const_iterator it = arcs.begin(); it != arcs.end(); ++it)
    { // on initialise la date d'ignition de points trouvés à 200 ->??

        result.emplace_back(it->x(), it->y(), 200);
    }

    return result;
}

void Algo::arcs_Fixe(const Point2D &p, vector<Point2D> &result)
{

    int nb_gen = get_generation();
    int dimension = getDimension();
    double gen;

    // Centre to Coin
    gen = (dimension * nb_gen) + (dimension / 2);
    for (double x = -gen; x <= gen; x += dimension)
    {
        for (double y = -gen; y <= gen; y += dimension)
        {
            result.push_back({p.x() + x, p.y() + y});
        }
    }

    // Centre to Centre
    gen = (25 * nb_gen);
    for (double x = -gen; x <= gen; x += dimension)
    {
        for (double y = -gen; y <= gen; y += dimension)
        {
            result.push_back({p.x() + x, p.y() + y});
        }
    }
}

string Algo::associated_id(const Point2D &p, double segment_size, double cell_dimension)
{
    string result = "";

    if (fmod(p.x(), cell_dimension) == 0)
    {
        result = to_string((int)p.x()) + '_' + to_string(int(p.y() - fmod(p.y(), segment_size) + segment_size / 2.0));
    }
    else if (fmod(p.y(), cell_dimension) == 0)
    {
        result = to_string(int(p.x() - fmod(p.x(), segment_size) + segment_size / 2.0)) + '_' + to_string((int)p.y());
    }
    else
    {
        result = 'I' + to_string(p.x()) + '_' + to_string(p.y());
    }
    return result;
}

bool Algo::pointOnLine(Point2D a, Point2D b, Point2D c)
{
    if (distanceP(a, b) + distanceP(b, c) == distanceP(a, b))
    {
        return true; // C is on the line.
    }

    return false;
}

void Algo::arcs(const Point2D &p, AEllipse &e, vector<Point2D> &result)
{

    Vector3D direction3D = e.get_c().normalized();
    Vector2D direction(direction3D.x(), direction3D.y());
    Point2D topleft = e.get_topleft();
    Point2D downright = topleft + Vector2D(dimensionCellule, dimensionCellule);

    for (int i = 0; i < 360; i += arcAngle)
    {
        Vector2D curr_dir = direction.rotate(i);
        Vector2D off = curr_dir * 0.00001;
        curr_dir = curr_dir * 36;

        if (intersect(p + off, p + curr_dir, topleft, topleft + Vector2D(25, 0)))
        {

            result.push_back(intersectionPoint(p + off, p + curr_dir, topleft, topleft + Vector2D(25, 0)));
        }
        else if (intersect(p + off, p + curr_dir, topleft, topleft + Vector2D(0, 25)))
        {

            result.push_back(intersectionPoint(p + off, p + curr_dir, topleft, topleft + Vector2D(0, 25)));
        }
        else if (intersect(p + off, p + curr_dir, downright + Vector2D(-25, 0), downright))
        {

            result.push_back(intersectionPoint(p + off, p + curr_dir, downright + Vector2D(-25, 0), downright));
        }
        else if (intersect(p + off, p + curr_dir, downright + Vector2D(0, -25), downright))
        {

            result.push_back(intersectionPoint(p + off, p + curr_dir, downright + Vector2D(0, -25), downright));
        }
    }
}

vector<Point2D> Algo::handleImpossible(vector<Point2D> points, Point2D point)
// déso mais j'arrive vraiment pas à comprendre cette fonction
{
    // pour l'intant, on ne considère que la première ligne d'obstacle
    json premierObtacle = this->listeObstacles.at(0);
    ListePoint2D obstacle;
    for (const auto &it : premierObtacle)
    {
        obstacle.emplace_back(Point2D(it));
    }

    vector<Point2D> result;
    bool cross;
    for (vector<Point2D>::const_iterator it = points.begin(); it != points.end(); ++it)
    {
        cross = false;
        int pos = 0;
        while (!cross && pos < ((obstacle.size()) - 1))
        {
            cross = intersect(point + (point - *it), *it, obstacle[pos], obstacle[pos + 1]);
            ++pos;
        }
        if (cross)
            result.push_back(intersectionPoint(point + (point - *it), *it, obstacle[pos - 1], obstacle[pos]));
        else
            result.push_back(*it);
    }
    return result;
}

inline pair<const string, pair<Point3D, Point3D>> Algo::get_min_key(map<string, pair<Point3D, Point3D>> &map)
{

    map.end();
    return *min_element(map.begin(), map.end(),
                        [](const pair<const string, pair<Point3D, Point3D>> &a, const pair<const string, pair<Point3D, Point3D>> &b) { return a.second.second.z() < b.second.second.z(); });
}

inline AEllipse Algo::getEllipseFromPoint(const Point2D &p)
{
    // retourne l'ellipse associée au point p (p est strictement à l'intérieur d'une cellule)
    /* code clair:
    Point2D celluleBase = getCellFromPoint(p); //retourne les coordonées de la cellule qui contient p
    Point2D index =  carteVentData.coordinateToIndex(celluleBase);
    return carteVentData.get_matrix()[(int)index.x()][(int)index.y()]->get_ellipse();
    */

    /*code optimisé:*/
    Point2D index = carteVentData.coordinateToIndex(getCellFromPoint(p));
    Data d = *carteVentData.get_matrix()[(int)index.x()]->at((int)index.y());
    return d.get_ellipse();
}

inline Point2D Algo::getCellFromPoint(const Point2D &p)
{
    // retourne les coordonées correspondantes au centre de la cellule qui contient p
    /* code clair
    double x = p.x() - fmod(p.x(),25) + ( 25. / 2 );
    double y = p.y() - fmod(p.y(),25) + ( 25. / 2 );
    return {x,y};
    */

    /* code optimisé */
    return {p.x() - fmod(p.x(), 25) + (25. / 2), p.y() - fmod(p.y(), 25) + (25. / 2)};
}

/*----------------------------------------- TEST ---------------------------------------------*/

inline Point2D Algo::getCentreFromPoint(const Point2D &p)
{
    // retourne les coordonées correspondantes au centre de la cellule qui contient p

    return {p.x() - fmod(p.x(), 25) + (25. / 2), p.y() - fmod(p.y(), 25) + (25. / 2)};
}

inline Point2D Algo::getCoinHautGaucheFromPoint(const Point2D &p)
{
    // retourne les coordonées correspondantes au coin en Haut à Gauche de la cellule qui contient p

    return {p.x() - fmod(p.x(), 25), p.y() - fmod(p.y(), 25) + 25};
}

inline Point2D Algo::getCoinHautDroitFromPoint(const Point2D &p)
{
    // retourne les coordonées correspondantes au coin en Haut à Droite de la cellule qui contient p

    return {p.x() - fmod(p.x(), 25) + 25, p.y() - fmod(p.y(), 25) + 25};
}

inline Point2D Algo::getCoinBasGaucheFromPoint(const Point2D &p)
{
    // retourne les coordonées correspondantes au coin en Bas à Gauche de la cellule qui contient p

    return {p.x() - fmod(p.x(), 25), p.y() - fmod(p.y(), 25)};
}

inline Point2D Algo::getCoinBasDroitFromPoint(const Point2D &p)
{
    // retourne les coordonées correspondantes au coin en Bas à Droite de la cellule qui contient p

    return {p.x() - fmod(p.x(), 25) + 25, p.y() - fmod(p.y(), 25)};
}

/*--------------------------------------------------------------------------------------------*/

void Algo::afficheProgression(const double &currentValue)
{
    progressBar(cout, currentValue, duration);
}
