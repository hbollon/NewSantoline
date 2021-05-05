// Pète moi le soleil !

#include "../libs/json.hpp"
using json = nlohmann::json;
#include <algorithm>
#include <fstream>
#include "math.h"
#include "AlgoOpt.hpp"
#include "EllipseOpt.hpp"
#include "ConvexHull.hpp"
using namespace std;

/*********************************************************************************************
 *                              Fonctions principales                                        *
 *********************************************************************************************/
vector<Point3D> AlgoOpt::propagation(vector<Point2D> listE, json cartevent, json parametreAlgo, ofstream &sortie, ofstream &infos_algo, ofstream &etat_final_liste_sommets)
{
    /*Ces trois listes servent à tester que l'id du point actuel correpond bien au suivant du point preceddent et au precedent du point suivant*/
    vector<string> id;
    vector<string> suivant;
    vector<string> precedent;
    double ancientmin;

    json infos_json = nullptr;
    double tempsPropagation = timeToDouble(parametreAlgo["duree"]);

    /*********************************************************************************************
 *                              ZONE DE "POINT DE COLLE"                                     *
 *********************************************************************************************/
    initialiserEllipse(cartevent, sortie, parametreAlgo["largeur"], parametreAlgo["hauteur"], parametreAlgo["waterReserve"], parametreAlgo["temperature"]);

    //Si la saisie du contour initial est fait dans le sens horaire
    if (saisieEstSensHoraire(listE))
    {
        //On inverse la liste pour faire comme si c'etait rentré dans le sens contraire de l'horaire
        listE = inverseSensList(listE);
    }
    for (auto &i : listE)
    {
        i = coordinateToCoordinateCalc(i);
    }
    /*********************************************************************************************
 *                              FIN DE "POINT DE COLLE"                                      *
 *********************************************************************************************/

    /*********************************************************************************************
 *                                      ALGORITHME                                           *
 *********************************************************************************************/
    initListff(listE);

    int compt = 0;
    vector<Point3D> r;

    int nmax = listff.size();
    int nmaxnewn;
    int nmaxnew;
    ancientmin = listff.at(aPropager).t;

    int iterationAffichage = 999999;
    double tfinal = (tempsPropagation / 3600.0);
    while (listff.at(aPropager).t <= tfinal)
    {
        if (compt == iterationAffichage)
            affichage = true;

        ancientmin = listff.at(aPropager).t;
        if (affichage)
            cout << "APropager : " << aPropager << endl;

        uneIteration(aPropager);

        nmaxnewn = listff.size();
        nmaxnew = nmax;

        while (nmaxnewn > nmaxnew && (nmaxnewn - nmaxnew <= 20))
        {
            for (int k = nmaxnew; k < nmaxnewn; k++)
            {
                rajoutePoints(to_string(k));
            }
            nmaxnew = nmaxnewn;
            nmaxnewn = listff.size();
        }

        nmaxnew = listff.size();
        for (int k = nmax; k < nmaxnew; k++)
        {
            actualiseCoin(to_string(k));
        }
        nmax = nmaxnew;
        compt++;
    }

    cout << "t : " << listff.at(aPropager).t << "  temps : " << tempsPropagation / 3600.0 << endl;

    int nb_points_parc = 1;
    double tempMin = listff.at(aPropager).t;
    PointOpt premier = listff.at(aPropager);
    cout << "Premier boucle final : " << premier.getId() << endl;
    cout << "Coordonnees de aPropager : " << premier.coordonne << endl;
    cout << "Temp min : " << tempMin << endl;
    string suiv = premier.suivant;
    PointOpt temp;
    temp = listff.at(suiv);

    /*gestion du premier point*/
    vector<vector<VitesseOpt>> lvp = (premier.listeVitesse);
    Point2D pp = premier.coordonne;
    id.push_back(premier.getId());
    precedent.push_back(premier.precedent);
    suivant.push_back(premier.suivant);
    double t = premier.t;
    for (auto it2 : lvp)
    {
        Vector2D v = it2.begin()->vecteur();
        pp = pp + (((tempMin - t) * v));
        Point2D p2d = indextoCoordinate(pp);
        r.push_back(Point3D(p2d.x(), p2d.y(), 0));
    }

    while (nb_points_parc < listff.size() && temp.getId() != premier.getId())
    {
        vector<vector<VitesseOpt>> lv = (temp.listeVitesse);
        Point2D pcoord = temp.coordonne;
        double ttemp = temp.t;
        id.push_back(temp.getId());
        precedent.push_back(temp.precedent);
        suivant.push_back(temp.suivant);
        for (auto it2 : lv)
        {
            Vector2D v = it2.begin()->vecteur();
            pcoord = pcoord + (((tempMin - ttemp) * v));
            Point2D p2d = indextoCoordinate(pcoord);
            r.push_back(Point3D(p2d.x(), p2d.y(), 0));
        }
        suiv = temp.suivant;
        temp = listff.at(suiv);
        nb_points_parc++;
    }

    /*sert tester que l'id du point actuel correpond bien au suivant du point preceddent et au precedent du point suivant*/
    for (int position = 0; position < id.size(); position++)
        if (position == 0)
        {
            if (id.at(position) != precedent.at((position + 1) % precedent.size()) ||
                id.at(position) != suivant.at(suivant.size() - 1))
            {
                cout << "Erreur a la position : " << position << ", id : " << id.at(position) << ", suivant : "
                     << suivant.at(suivant.size() - 1)
                     << ", precedent : " << precedent.at((position + 1) % precedent.size()) << endl;
            }
        }
        else
        {
            {
                if (id.at(position) != precedent.at((position + 1) % precedent.size()) ||
                    id.at(position) != suivant.at((position - 1) % suivant.size()))
                {
                    cout << "Erreur a la position : " << position << ", id : " << id.at(position) << ", suivant : "
                         << suivant.at((position - 1) % suivant.size())
                         << ", precedent : " << precedent.at((position + 1) % precedent.size()) << endl;
                }
            }
        }

    if (nb_points_parc >= listff.size())
    {
        cout << "Erreur, nb_points_parc >= listff.size" << endl;
    }

    sauvegarder_liste_sommets(etat_final_liste_sommets);

    cout << endl;
    if (infos_algo)
    {
        infos_json.push_back({{"algorithme", 3},
                              {"temps", listff.at(aPropager).t},
                              {"nb_iterations", compt},
                              {"nb_points", nb_points_parc}});
        infos_algo << infos_json;
        cout << "Les infos de l'algo ont ete sauvegardes." << endl;
    }
    else
    {
        cout << "[ERROR] Les infos de l'algo n'ont pas pu etre sauvegardes." << endl;
    }

    // Hull
    // 1 -> Convex Hull
    // 2 -> Concave Hull
    // Check folder's readme for more informations
    int courbeEnveloppe = parametreAlgo["courbeEnveloppe"];
    if (courbeEnveloppe == 1)
    {
        export3dPointsTo2dFile(r);
        int retCode = system("..\\src\\algo\\concave.exe points.txt -out points2.txt");
        r = import3dPointsTo2dFile();
    }
    else if (courbeEnveloppe == 2)
    {
        r = makeConvexHull(r);
    }
    else
    {
        std::cerr << "Error : bad hull method" << std::endl;
    }

    return r;
}

void AlgoOpt::uneIteration(string ind)
{
    PointOpt mvt = listff.at(ind);
    Point2D mm = mvt.coordonne;
    vector<vector<VitesseOpt>> vits = mvt.listeVitesse;
    bool indic;
    if (Vector2D(mm - Vector2D(round(mm.x()), round(mm.y()))).norm() < 0)
    {
        supprime(ind);
    }
    if (affichage)
    {
        cout << "mvt.indice : " << mvt.indice << endl;
        cout << "mvt.coordonne : " << mvt.coordonne << endl;
        cout << "mvt.tau : " << mvt.tau << endl;
        cout << "mvt.ancetre : " << mvt.ancetre << endl;
    }

    supprimePointTropProche(ind, 0.05);

    if (mvt.indice == "0")
    {
        pair<bool, vector<vector<VitesseOpt>>> tempp = vitesseNew1(vits, mvt.tau);
        vits = tempp.second;
        indic = tempp.first;
    }
    else if (mvt.indice == "1")
    {
        pair<bool, vector<vector<VitesseOpt>>> tempp = vitesseChocNew(vits, mvt.tau);
        vits = tempp.second;
        indic = tempp.first;
    }
    else if (mvt.indice == "2")
    {
        //pair<bool,vector<vector<VitesseOpt>>> tempp = vitesseSommetNew(vits,mvt.tau);
        //vits = tempp.second;
        //indic = tempp.first;/*TODO attention*/
        indic = false;
    }
    else if (mvt.indice == "3")
    {
        vits = mvt.listeVitesse;
        indic = true;
    }
    else
    {
        cout << "erreur dans iteration: indice incorrect " << mvt.indice << "." << endl;
        exit(EXIT_FAILURE);
    }
    int indic2 = 0;
    if (affichage)
        cout << "valeur de indic dans uneIteration : " << indic << endl;
    if (!indic)
    {
        supprime(ind);
    }
    else
    {
        indic2 = propage(ind, vits);
        if (affichage)
            cout << "valeur de indic2 dans uneIteration : " << indic2 << endl;
    }

    if (indic2 != 1)
    {
        supprime(ind);
    }
    if (affichage)
    {
        cout << "Fin uneIteration, prochain pts Apropager : " << aPropager << endl;
        cout << endl;
        cout << endl;
        cout << endl;
        cout << endl;
    }
}

int AlgoOpt::propage(string ind, vector<vector<VitesseOpt>> vits)
{
    if (affichage)
        cout << "Debut de propage ind : " << ind << endl;
    PointOpt mt = listff.at(ind);
    int compt = 1;
    int indic = 0;

    if (affichage)
    {
        cout << "Debut affichage des vitesses donnees en parametre de propage." << endl;
        for (auto &vit : vits)
        {
            cout << "vit.size() : " << vit.size() << endl;
            for (int i = 0; i < vit.size(); i++)
            {
                cout << fixed << " vitesse : " << vit.at(i) << endl;
            }
        }
        cout << "fin affichage des vitesses." << endl;
        cout << fixed << "mt : " << mt.coordonne << endl;
        cout << fixed << "mt tau : " << mt.tau << endl;
    }
    for (auto &vit : vits)
    {
        Point3D temp = Point3D(mt.coordonne.x(), mt.coordonne.y(), mt.t);
        pair<Point3D, Vector2D> mnp = mnplusun(temp, *vit.begin());

        string indm;
        if (vit.size() == 1)
        {
            indm = "0"; //Cas normal
        }
        else if (vit.size() == 2)
        {
            indm = "2"; //Cas du coin
        }
        else if (vit.size() == 3)
        {
            indm = "1"; //Cas du choc
        }
        else
        {
            cout << "Erreur sur vit.size() dans propage, vit.size = " << vit.size() << endl;
            exit(EXIT_FAILURE);
        }

        PointOpt mvt = PointOpt(mnp.first, mnp.second, indm, &compteurId, "Point de propage");

        mvt.addVitesse(vit);

        int indics = supprimerCroisement(mvt, ind);

        bool test = testBordureBrulee(mvt);

        if (!test)
        {
            if (indics == 0 && compt == 1)
            {
                insere(mvt, ind, false);
                compt++;
                indic = 1;
            }
            else if (indics == 0 && compt > 1)
            {
                int indc = listff.size() - 1;
                insere(mvt, to_string(indc), true);
            }
            else if (indics == 1)
            {
                if (affichage)
                    cout << "on insere pas mvt" << endl;
                compteurId--; /*temporaire*/
            }
            else if (indics == 2)
            {
                if (affichage)
                    cout << "il y a un choc" << endl;
                string indc;
                compteurId--;
                if (compt == 1)
                {
                    indc = ind;
                }
                else
                {
                    indc = to_string(listff.size() - 1);
                }
                indic = chocDansCellule(mvt, indc);

                return indic;
            }
            else
            {
                cout << "erreur 1 dans propage indic :" << indic << endl;
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            compteurId--;
            if (affichage)
                cout << "On est sur une Bordure brulee." << endl;
        }
    }
    if (affichage)
        cout << "Fin de propage" << endl;
    return indic;
}

void AlgoOpt::initialiserEllipse(json cartevent, ofstream &sortie, int largeur, int hauteur, int water, int temp)
{
    /*Initialise la carte des ellipses 3d pour toutes les cellules*/
    /*Les coordonnées d'une cellule correspondent au coin en bas à gauche d'une cellule*/
    json jsonVent = nullptr;
    setXminYmin(cartevent);

    int largeurCarteEllipse = largeur / tailleCellule;
    int hauteurCarteEllipse = hauteur / tailleCellule;
    carteEllipse = vector<vector<EllipseOpt>>(largeurCarteEllipse);

    for (int i = 0; i < largeurCarteEllipse; i++)
    {
        try
        {
            carteEllipse.at(i).resize(hauteurCarteEllipse);
        }
        catch (std::bad_alloc &ba)
        {
            std::cout << "erreur init map" << std::endl;
            std::cout << " i = " << i << std::endl;
        }
    }
    for (const auto &it : cartevent)
    {
        double slope = it["slope"];
        double aspect = it["aspect"];
        json jSlopeVector = it["slope_vector"];
        Vector3D windSlope = Vector3D(it["windSlope"][0], it["windSlope"][1], it["windSlope"][2]);
        Point2D mapCoordinate = Point2D(it["x"], it["y"]);
        Point2D index = coordinateToIndex(mapCoordinate);

        float lb = 1 + 0.0012 * pow((2.237 * windSlope.norm()), 2.154);
        float epsilon = sqrt(1 - (1 / (lb * lb)));

        double rateOfSpread = 180 * exp(0.06 * temp) * tanh(((100 - water) * 1.0) / 150.0) * (1 + 2 * (0.8483 + tanh(((windSlope.norm() * 3.6) / 30.0) - 1.25)));

        double windslopenorme = windSlope.norm();
        double ratio = rateOfSpread / windslopenorme;

        Vector3D rateOfSpreadVect = Vector3D((windSlope.x() * ratio) / tailleCellule, (windSlope.y() * ratio) / tailleCellule, (windSlope.z() * ratio) / tailleCellule);

        EllipseOpt e = EllipseOpt(mapCoordinate, rateOfSpreadVect, epsilon, aspect, slope);

        jsonVent.push_back({{"cellule_coords", mapCoordinate.toPair()},
                            {"index", index.toPair()},
                            {"igni_point_coords", e.coordonne().toPair()},
                            {"great_axis_dir", e.vecteur().toPair()},
                            {"great_axis", e.a()},
                            {"small_axis", e.b()}});

        if ((int)index.x() < carteEllipse.size() && (int)index.y() < carteEllipse[(int)index.x()].size())
            carteEllipse[(int)index.x()][(int)index.y()] = e;
    }
    sortie << jsonVent << endl;
    cout << "Carte des ellipses initialisee !" << endl;
}

void AlgoOpt::initListff(const vector<Point2D>& listPointE)
{
    listePointsDansCellule = map<string, vector<string>>();
    vector<PointOpt> listDef;
    vector<Point2D> listPoint = raffine(listPointE, 0.5);
    //On créé une liste de point circulaire :
    //l(0) => ancetre = l(0)
    //        precedent = l(dernier)
    //        suivant = l(1)
    //        id = l(0)
    //l(1) => ancetre = l(1)
    //        precedent = l(0)
    //        suivant = l(2)
    //        id = l(1)
    // [...]
    //ajout de l'ancetre
    for (vector<Point2D>::const_iterator it = listPoint.begin(); it != listPoint.end(); ++it)
    {
        Point2D pTemp = Point2D(it->x(), it->y());
        if (pTemp.x() - floor(pTemp.x()) < seuil)
        {
            pTemp.x() = pTemp.x() + 0.001;
        }
        if (pTemp.y() - floor(pTemp.y()) < seuil)
        {
            pTemp.y() = pTemp.y() + 0.001;
        }
        PointOpt p = PointOpt(pTemp, 0, Vector2D(0, 0), "0", &compteurId, "Point de initListff");
        p.ancetre = p.getId();
        //ajout du pointOpt a la liste
        listDef.push_back(p);
    }
    //ajout du précédent
    for (int i = 1; i < listDef.size(); i++)
    {
        listDef.at(i).precedent = listDef.at(i - 1).getId();
    }
    //ajout du suivant
    for (int i = 0; i < listDef.size() - 1; i++)
    {
        listDef.at(i).suivant = listDef.at(i + 1).getId();
    }
    //gestion du premier element de la liste
    listDef.at(0).precedent = listDef.at(listDef.size() - 1).getId();
    listDef.at(0).suivant = listDef.at(1).getId();
    //gestion du dernier element de la liste
    listDef.at(listDef.size() - 1).precedent = listDef.at(listDef.size() - 2).getId();
    listDef.at(listDef.size() - 1).suivant = listDef.at(0).getId();

    //ajout la liste des vitesses pour chaque point
    for (auto &it : listDef)
    {
        Point2D m = it.coordonne;
        Point2D mp = findPointOptByIndice(listDef, it.precedent).coordonne;
        Point2D ms = findPointOptByIndice(listDef, it.suivant).coordonne;
        Point2D ds = ms - m;
        Point2D dp = m - mp;
        if (Vector2D(ds).norm() != 0)
        {
            Point2D ij = Point2D(floor(m.x()), floor(m.y()));
            VitesseOpt vp = vitessea0(0, dp, ij);
            VitesseOpt vs = vitessea0(0, ds, ij);
            vector<VitesseOpt> temp1;
            temp1.push_back(vp);
            vector<VitesseOpt> temp2;
            temp2.push_back(vs);
            it.listeVitesse.push_back(temp1);
            it.listeVitesse.push_back(temp2);
        }
    }

    for (auto &it : listDef)
    {
        it.listeVitesse = vitesseChocOuPas(*(it.listeVitesse.begin())->begin(), *(it.listeVitesse.begin() + 1)->begin()).second;
        listff.insert(make_pair(it.getId(), it));
    }
    for (int i = 0; i < listPoint.size(); i++)
    {
        if (!propage(to_string(i), listff.at(to_string(i)).listeVitesse))
        {
            string indp = listff.at(to_string(i)).precedent;
            string inds = listff.at(to_string(i)).suivant;
            listff.at(indp).suivant = inds;
            listff.at(inds).precedent = indp;
        }
    }

    string ind0 = aPropager;
    string ind = listff.at(ind0).suivant;

    while (ind != ind0)
    {
        actualiseCoin(ind);
        ind = listff.at(ind).suivant;
    }
    actualiseCoin(ind0);

    ind = listff.at(ind0).suivant;
    while (ind != ind0)
    {
        initialiserCoinContourDepart(listff.at(listff.at(ind).ancetre));
        ind = listff.at(ind).suivant;
    }
    initialiserCoinContourDepart(listff.at(listff.at(ind0).ancetre));
}

bool AlgoOpt::chocDansCellule(PointOpt mvt, string ind)
{
    cout << "ChocDansCellule mvt.getid() : " << mvt.getId() << ", ind : " << ind << endl;
    bool INDIC = true;
    string INDP, INDS;
    string INDCHOC;
    PointOpt MVT;

    triDesChocs();
    Choc choc = *listeChoc.begin();
    string INDC = choc.indic;
    vector<VitesseOpt> VITS = *mvt.listeVitesse.begin();
    vector<VitesseOpt> VITSC = *listff.at(INDC).listeVitesse.begin();
    Point3D MC = Point3D(choc.point.x(), choc.point.y(), choc.temps);

    VitesseOpt v = *VITS.begin();
    VitesseOpt vc = *VITSC.begin();
    pair<double, std::vector<std::vector<VitesseOpt>>> VITC;
    double test = v.vecteur().determinant(vc.vecteur());

    if (test < 0)
    {
        if (VITS.size() == 3)
        {
            v = VITS.at(1);
        }
        if (VITSC.size() == 3)
        {
            vc = VITSC.at(2);
        }
        VITC = vitesseChocOuPas(v, vc);
        INDP = listff.at(ind).precedent;
        INDS = listff.at(INDC).suivant;
    }
    else
    {
        INDP = listff.at(INDC).precedent;
        if (VITS.size() == 3)
        {
            v = VITS.at(2);
        }
        if (VITSC.size() == 3)
        {
            vc = VITSC.at(1);
        }
        VITC = vitesseChocOuPas(vc, v);
        INDS = listff.at(ind).suivant;
    }

    VitesseOpt vp = *VITC.second.begin()->begin();
    VitesseOpt vm = VitesseOpt(Vector2D(-vp.vecteur().x(), -vp.vecteur().y()), Vector2D(-vp.tau().x(), -vp.tau().y()), vp.coordonne(), vp.indice());
    pair<Point3D, Vector2D> MNP = mnplusun(MC, vm);

    bool TEST = false;
    Point2D ij = VITC.second.begin()->begin()->coordonne();
    Point2D ijc = ij + MNP.second;

    vector<string> listnum;
    string nb = numero(ij);
    listnum.push_back(nb);
    nb = numero(ijc);
    listnum.push_back(nb);

    for (auto num : listnum)
    {
        auto iterator = find(listeCellulesBrulees.begin(), listeCellulesBrulees.end(), num);
        if (iterator != listeCellulesBrulees.end())
        {
            vector<string> tabIndice1Num = listePointsDansCellule.at(num);
            for (int indz = 0; indz < tabIndice1Num.size(); indz++)
            {
                PointOpt mvtt = listff.at(tabIndice1Num.at(indz));
                Vector2D mvttt = mvtt.coordonne;
                if (mvttt.norm() <= seuil)
                {
                    TEST = true;
                    INDCHOC = tabIndice1Num.at(indz);
                }
            }
        }
    }

    MNP.first.z() = MC.z() - (MNP.first.z() - MC.z());
    MNP.second = Vector2D(-MNP.second.x(), -MNP.second.y());

    if (!TEST)
    {
        MVT = PointOpt(Point2D(MNP.first.x(), MNP.first.y()), MNP.first.z(),
                       MNP.second, "0", INDP, INDS, "3", VITC.second, &compteurId, "Point de chocDansCellule");
    }
    else
    {
        if (listff.at(INDC).ancetre == INDCHOC && listff.at(INDC).indice == "1")
        {
            INDIC = false;
            if (affichage)
                cout << "On sort de chocDansCellule, INDIC : " << INDIC << endl;
            return INDIC;
        }
        else if (mvt.indice == "1" && mvt.ancetre == INDCHOC)
        {
            supprime(INDC);
            if (affichage)
                cout << "On sort de chocDansCellule, INDIC : " << INDIC << endl;
            return INDIC;
        }
        MVT = PointOpt(Point2D(MNP.first.x(), MNP.first.y()), MNP.first.z(),
                       MNP.second, INDCHOC, INDP, INDS, "3", VITC.second, &compteurId, "Point de chocDansCellule");
    }

    insere(MVT, ind, false);

    listff.at(to_string(listff.size() - 1)).precedent = INDP;
    listff.at(to_string(listff.size() - 1)).suivant = INDS;
    INDCHOC = to_string(listff.size() - 1);

    listff.at(INDS).precedent = INDCHOC;
    listff.at(INDP).suivant = INDCHOC;

    listff.at(ind).suivant = INDS;
    listff.at(ind).precedent = INDCHOC;
    if (affichage)
        cout << "ind : " << ind << ", INDS : " << INDS << ", INDP : " << INDP << ", INDCHOC : " << INDCHOC << ", listff.size() : " << listff.size() << endl;

    supprimePlus(INDC, choc.num);
    if (affichage)
        cout << "On sort de chocDansCellule, INDIC : " << INDIC << endl;

    return INDIC;
}

/*********************************************************************************************
 *                              Fonctions d'insertion de point                               *
 *********************************************************************************************/

//fonction qui renvoit une liste de point contour plus complete à partir des quelques points initiaux
vector<Point2D> AlgoOpt::raffine(vector<Point2D> listPoint, double crit)
{
    //On recupère le premier point posé par l'utilisateur et on le met dans m
    Point2D m = *listPoint.begin();
    //On créé une liste de points qui sera notre résultat
    vector<Point2D> result;
    //On copie le premier point dans la dernière case du vector
    listPoint.push_back(m);
    //On parcours la liste de point sans passer par le premier
    for (vector<Point2D>::const_iterator it = listPoint.begin() + 1; it != listPoint.end(); ++it)
    {
        //On créé un vecteur temporaire
        Vector2D vtemp = (*it) - m;
        //on calcule la norme (distance) du vecteur temporaire
        double dist = vtemp.norm();
        if (dist > seuil)
        { /*Attention val abs*/
            double test = floor((dist) / crit);
            for (int j = 1; j < test + 1; j++)
            {
                Point2D mk = m + (j - 1) * Vector2D(*it - m) / (test + 1);
                Point2D mki = Point2D(round(mk.x()), round(mk.y()));
                //On ne veut pas tomber pile sur un cote d'une case
                //Si on tombe sur une ligne on decale d'un centieme
                if (((mk.x() - mki.x()) * (mk.y() - mki.y())) == 0)
                {
                    mk = mk + 0.01 * Vector2D(Vector2D(*it - m) / (test + 1));
                }
                result.push_back(mk);
            }
            m = *it;
        }
    }
    return result;
}

pair<Point3D, Vector2D> AlgoOpt::mnplusun(const Point3D& point, const VitesseOpt& vitesse)
{
    if (affichage)
        cout << "Debut mnplusun" << endl;
    Vector2D v = vitesse.vecteur();
    Point2D ij = vitesse.coordonne();
    double m1;
    double dt1;
    double tauperp1;
    Point3D p = point;

    double m2;
    double dt2;
    double tauperp2;

    Point3D pointRetour;
    Vector2D vectorRetour;

    if (v.x() > seuil)
    {
        m1 = ij.x() + 1;
        dt1 = (m1 - p.x()) / v.x();
        tauperp1 = 1;
    }
    else if (v.x() < -seuil)
    {
        m1 = ij.x();
        dt1 = (m1 - p.x()) / v.x();
        tauperp1 = -1;
    }
    else
    {
        v.x() = 0;
        dt1 = 1000000000;
        tauperp1 = 0;
    }

    if (v.y() > seuil)
    {
        m2 = ij.y() + 1;
        dt2 = (m2 - p.y()) / v.y();
        tauperp2 = 1;
    }
    else if (v.y() < -seuil)
    {
        m2 = ij.y();
        dt2 = (m2 - p.y()) / v.y();
        tauperp2 = -1;
    }
    else
    {
        v.y() = 0;
        dt2 = 1000000000;
        tauperp2 = 0;
    }
    if (dt1 < dt2)
    {
        pointRetour = Vector3D(m1, p.y() + dt1 * v.y(), point.z() + dt1);
        vectorRetour = Vector2D(1, 0);
    }
    else if (dt1 > dt2)
    {
        pointRetour = Vector3D(p.x() + dt2 * v.x(), m2, point.z() + dt2);
        vectorRetour = Vector2D(0, 1);
    }
    else if (dt1 == 1000000000)
    {
        pointRetour = point;
        vectorRetour = Vector2D(0, 0);
    }
    else
    {
        pointRetour = Vector3D(p.x() + dt2 * v.x(), m2, point.z() + dt2);
        vectorRetour = Vector2D(1, 1);
    }
    vectorRetour = Vector2D(vectorRetour.x() * tauperp1,
                            vectorRetour.y() * tauperp2);
    return (std::make_pair(pointRetour, vectorRetour));
}

void AlgoOpt::insere(PointOpt point, string ind, bool indic)
{
    int nmax = listff.size();
    if (!indic)
    {
        point.precedent = listff.at(ind).precedent;
        point.suivant = listff.at(ind).suivant;
        point.ancetre = ind;
        listff.insert(make_pair(to_string(nmax), point));
        listff.at(listff.at(ind).precedent).suivant = to_string(nmax);
        listff.at(listff.at(ind).suivant).precedent = to_string(nmax);
    }
    else
    {
        point.precedent = ind;
        point.suivant = listff.at(ind).suivant;
        point.ancetre = listff.at(ind).ancetre;
        listff.insert(make_pair(to_string(nmax), point));
        listff.at(ind).suivant = to_string(nmax);
        listff.at(listff.at(to_string(nmax)).suivant).precedent = to_string(nmax);
        if (point.ancetre != "0")
        {
            //cette condition permet au programme de s'executer mais n'est pas certaine et sert sans doute a traiter les chocs
            listff.at(listff.at(ind).ancetre).precedent = to_string(nmax);
        }
    }
    Point2D ij;
    string num;
    vector<string> listNum;
    if (point.indice == "2")
    {
        ij = point.listeVitesse.begin()->begin()->coordonne();
        num = numero(ij);
        listNum.push_back(num);
        ij = ((point.listeVitesse.begin())->begin() + 1)->coordonne();
        num = numero(ij);
        listNum.push_back(num);
    }
    else
    {
        ij = point.listeVitesse.begin()->begin()->coordonne();
        num = numero(ij);
        listNum.push_back(num);
    }
    map<string, int> test;

    for (auto &num2 : listNum)
    {
        if (count(listeCellulesBrulees.begin(), listeCellulesBrulees.end(), num2) == 0)
        {
            listeCellulesBrulees.push_back(num2);
            vector<string> temp;
            temp.push_back(point.getId());
            listePointsDansCellule.insert(make_pair(num2, temp));
        }
        else
        {
            listePointsDansCellule.at(num2).push_back(point.getId());
        }
    }

    if (!indic)
    {
        listePointsActifs.erase(ind);
    }
    listePointsActifs.insert(make_pair(to_string(nmax), point.t));

    //!!! Les instructions suivantes ont un gros besoin d'optimisation
    string minInd = listePointsActifs.begin()->first;
    for (auto &it : listePointsActifs)
    {
        if (it.second < listePointsActifs.at(minInd))
        {
            minInd = it.first;
        }
    }
    aPropager = minInd;
}

void AlgoOpt::rajoutePoints(string indice)
{
    PointOpt mt = listff.at(indice);
    rajoutePoint(indice);
    string indp = mt.precedent;
    rajoutePoint(indp);
}

void AlgoOpt::rajoutePoint(string indice)
{
    if (affichage)
        cout << "Dans rajoute Point indice : " << indice << endl;
    double tmin = listff.at(aPropager).t;

    PointOpt mt = listff.at(indice);
    Point2D m = mt.coordonne;
    double t = mt.t;
    vector<vector<VitesseOpt>> vitm = mt.listeVitesse;
    vector<VitesseOpt> vit = vitm.at(0);
    string inds = mt.suivant;
    PointOpt mts = listff.at(inds);

    if ((mts.ancetre != mt.ancetre) || ((atoi(mts.indice.c_str()) + atoi(mt.indice.c_str())) != 3))
    {
        vector<vector<VitesseOpt>> vitmts = mts.listeVitesse;
        vector<VitesseOpt> vits = vitmts.at(0);
        double ts = mts.t;
        Point2D ms = mts.coordonne;
        Vector2D vs = vits.at(0).vecteur();
        Vector2D v = vit.at(0).vecteur();

        Point2D ij = vit.at(0).coordonne();
        Point2D ijs = vits.at(0).coordonne();

        Vector2D dm;
        Point2D mm;
        if (ts >= t)
        {
            ms = ms + (t - ts) * vs;
            dm = ms - m;
            mm = m;
        }
        else
        {
            mm = m + (ts - t) * v;
            ms = mts.coordonne;
            dm = ms - mm;
        }
        double testS;

        Vector2D ijMoinsijs = ij - ijs;
        if (ijMoinsijs.norm() != 0)
        {
            testS = dm.norm();
        }
        else
        {
            testS = (abs(v.determinant(dm)) / v.norm()) + (abs(vs.determinant(dm)) / vs.norm());
        }
        if (testS >= crit)
        {
            mm = m + (tmin - t) * v;
            if (affichage)
                cout << tmin << " " << t << endl;
            ms = mts.coordonne + (tmin - ts) * vs;
            Point2D mis = (ms + mm) / 2;
            ij = Point2D(round(mis.x()), round(mis.y()));
            if ((mis.x() - ij.x()) * (mis.y() - ij.y()) == 0)
            {
                mis = ((ms + mm) * 2) / 3;
            }

            ij = Point2D(floor(mis.x()), floor(mis.y()));

            Vector2D dir = ms - mm;
            double testSS = dir.norm();

            if (testSS >= (crit / 5))
            {
                VitesseOpt vitmis = vitessea0(0, dir, ij);
                Point3D misAvecT = Point3D(mis.x(), mis.y(), tmin);
                pair<Point3D, Vector2D> mnp = mnplusun(misAvecT, vitmis);
                Point2D postmnplusun = Point2D(mnp.first.x(), mnp.first.y());
                PointOpt mvt = PointOpt(postmnplusun, mnp.first.z(), mnp.second, "0", "0", "0", "0", vitmis, &compteurId, "Point de rajoutePoint");
                bool test = testBordureBrulee(mvt);
                if (!test)
                {
                    int indics = supprimeCroisementRaj(mvt, tmin, indice);
                    if (indics == 0)
                    {
                        insere(mvt, indice, true);
                    }
                    else
                    {
                        compteurId--;
                    }
                }
                else
                {
                    compteurId--;
                }
            }
        }
    }
}

/*********************************************************************************************
 *                              Fonctions de suppression de point                            *
 *********************************************************************************************/
void AlgoOpt::supprime(string ind)
{
    if (listePointsActifs.count(ind) != 0)
    {
        listePointsActifs.erase(ind);
        string minInd = listePointsActifs.begin()->first;
        for (auto &it : listePointsActifs)
        {
            if (it.second < listePointsActifs.at(minInd))
            {
                minInd = it.first;
            }
        }
        aPropager = minInd;
        listff.at((listff.at(ind).precedent)).suivant = (listff.at(ind).suivant);
        listff.at((listff.at(ind).suivant)).precedent = (listff.at(ind).precedent);
    }
}

void AlgoOpt::supprimePlus(std::string ind, std::string num)
{
    if (listePointsActifs.count(ind) != 0)
    {
        listePointsActifs.erase(ind);
        string minInd = listePointsActifs.begin()->first;
        for (auto &it : listePointsActifs)
        {
            if (it.second < listePointsActifs.at(minInd))
            {
                minInd = it.first;
            }
        }
        aPropager = minInd;
    }
    if (listePointsDansCellule.count(num) != 0)
    {
        vector<string> tab = listePointsDansCellule.at(num);
        vector<int> aSuppr;
        for (int i = 0; i < tab.size(); i++)
        {
            if (tab.at(i) == ind)
            {
                tab.erase(tab.begin() + i);
                listePointsDansCellule.at(num) = tab;
                i = tab.size();
            }
        }
    }
}

int AlgoOpt::supprimerCroisement(PointOpt mtp, std::string ind)
{
    if (affichage)
        cout << "SupprimerCroisement" << endl;
    listeChoc = vector<Choc>();
    int indic = 0;
    PointOpt mt = listff.at(ind);
    if (affichage)
        cout << "ind : " << ind << " listff.size() " << listff.size() << endl;
    Point2D mm = mt.coordonne;
    Point2D mp = mtp.coordonne;
    double tmin = mt.t;
    double tp = mtp.t;
    Vector2D vtemp = Vector2D(mm - mp);

    if (vtemp.norm() <= seuil)
    { /*Cas sommet*/
        cout << "Dans supprimeCroisement mtp = mm IND=" << ind << std::endl;
        return 1; /*TODO attention*/
        exit(EXIT_FAILURE);
    }

    Vector2D v = mtp.listeVitesse.begin()->begin()->vecteur();

    vector<string> indasuppr;
    vector<string> listNum;

    Point2D ij = (mtp.listeVitesse.begin())->begin()->coordonne();
    string num1 = numero(ij);
    listNum.push_back(num1);

    if (mtp.indice == "2")
    {
        listNum.push_back(numero(((mtp.listeVitesse.begin())->begin() + 1)->coordonne()));
    }

    for (auto num : listNum)
    {

        auto iterator = find(listeCellulesBrulees.begin(), listeCellulesBrulees.end(), num);
        if (iterator != listeCellulesBrulees.end())
        {
            if (affichage)
                cout << "num est present : " << num << endl;
            for (int i = 0; i < listePointsDansCellule.at(num).size(); i++)
            {
                PointOpt it = listff.at(listePointsDansCellule.at(num).at(i));

                Point2D mc = it.coordonne;
                string indoc = it.ancetre;
                Vector2D temp = (mc - mm);
                if (temp.norm() > seuil && ind != indoc)
                {
                    VitesseOpt vitc = *(it.listeVitesse.begin())->begin();
                    Vector2D vc = vitc.vecteur();
                    double tc = it.t;
                    Vector2D dm = mm - mc;
                    double aux = det(vc, v);
                    if (abs(aux) > seuil)
                    {
                        double test = tmin + det(dm, vc) / aux;
                        // test -> Temps au moment de l'intersection de (m->mvt) avec la trajectoire issue de mc en parcourant (m->mvt).
                        if (tp >= test && test >= tmin)
                        { //Verifie si on se trouve dans la cellule
                            double testc = tc + det(dm, v) / aux;
                            // testc -> Temps au moment de l'intersection de (m->mvt) en parcourant la trajectoire issue de mc.
                            Point2D mcr = mc + (testc - tc) * vc; //Point d'intersection des trajectoires
                            temp = mcr - mm;
                            if (temp.norm() > seuil && testc >= 0)
                            {
                                if (test + seuil < testc && tc >= tmin)
                                {
                                    if (affichage)
                                        cout << "On supprime :" + it.getId() << endl;
                                    indasuppr.push_back(it.getId());
                                }
                                else if (test > testc + seuil)
                                {
                                    return 1;
                                }
                                else if (abs(testc - test) <= seuil)
                                {
                                    if (mtp.indice == "2")
                                    { //On insere pas les sommets
                                        /*TODO supprimer quand on va traiter les sommets*/
                                        indasuppr.push_back(it.getId());
                                    }
                                    else if (it.indice == "2")
                                    {
                                        /*TODO faire attention, gestion de sommets etrange*/
                                        return 1;
                                    }
                                    else
                                    {
                                        if (listePointsActifs.count(it.getId()) > 0)
                                        {
                                            listeChoc.emplace_back(num, it.getId(), mcr, test);
                                            indic = 2;
                                        }
                                        else
                                        {
                                            return 1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    std::vector<string>::iterator ite;
    ite = std::unique(indasuppr.begin(), indasuppr.end());

    indasuppr.resize(std::distance(indasuppr.begin(), ite));
    if (affichage)
        cout << "Indasuppr taille : " << indasuppr.size() << endl;
    if (!indasuppr.empty())
    {
        for (auto &it : indasuppr)
        {
            if (affichage)
                cout << "Suppression de : " << it << endl;
            supprime(it);
            for (unsigned int i = 0; i < listeChoc.size(); i++)
            {
                if (listeChoc[i].indic == it)
                {

                    listeChoc.erase(listeChoc.begin() + i);
                }
            }
        }
    }

    return indic;
}

int AlgoOpt::supprimeCroisementRaj(const PointOpt& mvt, const double& tmin, const string& ind)
{
    if (affichage)
        cout << "Debut supprimeCroisementRaj" << endl;
    listeChoc = vector<Choc>();

    int indic = 0;
    Point2D mm = mvt.coordonne;
    double tp = mvt.t;
    vector<VitesseOpt> vit = *mvt.listeVitesse.begin();
    Vector2D v = (vit.begin())->vecteur();
    vector<string> indasuppr;
    Point2D ij = vit.begin()->coordonne();
    string num = numero(ij);

    auto iterator = find(listeCellulesBrulees.begin(), listeCellulesBrulees.end(), num);

    if (iterator != listeCellulesBrulees.end())
    {
        for (int i = 0; i < listePointsDansCellule.at(num).size(); i++)
        {

            PointOpt mtc = listff.at(listePointsDansCellule.at(num).at(i));
            Point2D mc = mtc.coordonne;
            vector<VitesseOpt> vitc = *mtc.listeVitesse.begin();
            Vector2D vc = vitc.begin()->vecteur();
            double tc = mtc.t;
            Vector2D dm = mm - mc;
            double aux = vc.determinant(v);
            if (abs(aux) > seuil)
            {
                double tesT = tmin + (dm.determinant(vc) / aux);
                if (tp >= tesT && tesT >= tmin)
                {
                    double tesTC = tc + (dm.determinant(v) / aux);
                    Point2D mcr = mc + ((tesTC - tc) * vc);
                    Vector2D mcrMoinsMM = mcr - mm;
                    if (mcrMoinsMM.norm() >= seuil && tesTC >= 0)
                    {
                        if (tesT + seuil < tesTC && tc >= tmin)
                        {
                            indasuppr.push_back(mtc.getId());
                        }
                        else if (tesT > tesTC + seuil)
                        {
                            return 1;
                        }
                        else if (abs(tesTC - tesT) <= seuil)
                        {
                            return 1;
                        }
                    }
                }
            }
        }
        std::vector<string>::iterator ite;
        ite = unique(indasuppr.begin(), indasuppr.end());
        indasuppr.resize(std::distance(indasuppr.begin(), ite));

        if (affichage)
            cout << "Indasuppr taille : " << indasuppr.size() << endl;
        if (!indasuppr.empty())
        {
            for (auto &it : indasuppr)
            {
                if (it != ind)
                {
                    if (affichage)
                        cout << "Suppression de : " << it << endl;
                    supprime(it);
                    for (unsigned int i = 0; i < listeChoc.size(); i++)
                    {
                        if (listeChoc[i].indic == it)
                        {
                            listeChoc.erase(listeChoc.begin() + i);
                        }
                    }
                }
                else
                { /*TODO chercher une autre maniere de faire car ça peut provoquer des problemes*/
                    if (affichage)
                        cout << "On ne supprime pas le point d'indice : " << ind << " car c'est le point qui a ete donne en parametre" << endl;
                }
            }
        }
    }

    return indic;
}

void AlgoOpt::supprimePointTropProche(string ind, double crit)
{
    string inds = listff.at(ind).suivant;
    string indp = listff.at(ind).precedent;
    double ts = listff.at(inds).t;
    double tp = listff.at(indp).t;
    double t = listff.at(ind).t;
    Point2D ms = listff.at(inds).coordonne;
    Point2D mp = listff.at(indp).coordonne;
    Point2D m = listff.at(ind).coordonne;
    VitesseOpt vs = *listff.at(inds).listeVitesse.begin()->begin();
    VitesseOpt vp = *listff.at(indp).listeVitesse.begin()->begin();
    Vector2D mds = ms + (t - ts) * vs.vecteur() - m;
    Vector2D mdp = mp + (t - tp) * vp.vecteur() - m;

    if (mds.norm() < crit)
    {
        if (affichage)
            cout << "mds.norm() < crit, on va supprimer : " << inds << endl;
        supprime(inds);
    }
    if (mdp.norm() < crit)
    {
        if (affichage)
            cout << "mdp.norm() < crit, on va supprimer : " << indp << endl;
        supprime(indp);
    }
}

/*********************************************************************************************
 *                              Fonction de gestion des vitesses                             *
 *********************************************************************************************/
double AlgoOpt::azero(const Vector2D& tau, const VitesseOpt& Vit)
{
    /*Observations faites concernant la fonction azero*/
    /*azero a l'air de faire un synthese d'elements existants dans la cellule sortante*/
    /*vit correspond a la vitesse du point qui vient de finir sa propagation dans la cellule sortante*/
    /*retourne une synthse d'elements de la cellule sous forme d'un double*/

    Point2D ij = Vit.coordonne();
    if (affichage)
        cout << "Debut azero" << endl;
    if (affichage)
        cout << "tau : " << tau << endl;
    if (affichage)
        cout << "vit : " << Vit.coordonne() << endl;
    EllipseOpt ellipse = carteEllipse[ij.x()][ij.y()]; /*Ellipse de la cellule sortante*/

    if (affichage)
        cout << "ellipse : " << ellipse << endl;
    Vector2D eperp = Vector2D(-ellipse.vecteur().y(), ellipse.vecteur().x());
    Vector2D V = Vit.vecteur();
    Vector2D W = V + Vector2D(ellipse.coordonne().x(), ellipse.coordonne().y());
    Vector2D VE = Vector2D(ellipse.vecteur().dot(W), eperp.dot(W));
    Vector2D AW = Vector2D(VE.x() / (ellipse.a() * ellipse.a()), VE.y() / (ellipse.b() * ellipse.b()));
    Vector2D AW_Bis = AW.x() * ellipse.vecteur() + AW.y() * eperp;
    double aux1 = AW_Bis.dot(tau);
    double aux2 = AW_Bis.dot(V);
    double a0 = aux1 / aux2;
    if (affichage)
        cout << "aux1 : " << aux1 << ", aux2 : " << aux2 << endl;
    if (affichage)
        cout << "Fin azero : " << a0 << endl;
    return a0;
}

VitesseOpt AlgoOpt::vitessea0(double a0, Vector2D tau, Point2D ijn)
{
    /*Commentaires de Pierre Baras : */
    // calcule la vitesse V et la tangente au front de flamme au sein d'une cellule IJ
    // a0 coefficient d'entrée dans la cellule,
    // pour obtenir la vitesse correspondant à un front de flamme de tangente tau, faire a0=0,
    // tau vecteur tangent au front de flamme (ou à la face d'entrée dans la cellule si a0 n'est pas égal à 0),
    // ATTENTION, IL DOIT ETRE DANS LE SENS DES AIGUILLES D'UNE MONTRE.

    /*Observations faites concernant la fonction vitessea0*/
    /*a0 correspond a une synthese des differents elements de la cellule sortante*/
    /*Quand il s'agit d'un "nouveau point" (point sur le contour initial ou point creer), on force la a0 a 0*/
    /*La fonction nous donne la vitesse dans la cellule entrante*/
    /*Les coordonnees ijn correspondent aux coordonnees de la cellule entrante*/
    Vector2D tff;
    Vector2D v;
    double INDIC;

    //Ellipse de la nouvelle cellule
    EllipseOpt ellipse = carteEllipse[ijn.x()][ijn.y()];
    tau = tau.normalized();

    Vector2D tauperp = Vector2D(-tau.y(), tau.x());
    Vector2D eperp = Vector2D(-ellipse.vecteur().y(), ellipse.vecteur().x());

    Vector2D w0 = Vector2D(tau.x() + a0 * ellipse.coordonne().x(),
                           tau.y() + a0 * ellipse.coordonne().y());
    double w01 = w0.dot(ellipse.vecteur());
    double w02 = w0.dot(eperp);

    Vector2D aw0 = (w01 / (ellipse.a() * ellipse.a())) * ellipse.vecteur() + (w02 / (ellipse.b() * ellipse.b())) * eperp;

    double w0aw0 = w0.dot(aw0);
    double delta = w0aw0 - (a0 * a0);

    if (delta >= seuil)
    {
        Vector2D w1 = Vector2D(-aw0.y(), aw0.x());
        double test = w1.dot(tauperp);
        if (test < 0)
        {
            test = 1.0;
        }
        else
        {
            test = -1.0;
        }
        double aux1 = a0 / w0aw0;
        double aux2 = ellipse.a() * ellipse.b() * sqrt(delta) / w0aw0;
        Vector2D vtemp = Vector2D(-ellipse.coordonne().x(), -ellipse.coordonne().y());
        v = vtemp + aux1 * w0 + test * aux2 * w1;
        tff = aux1 * w1 - (test * aux2 / (ellipse.a() * ellipse.a() * ellipse.b() * ellipse.b())) * w0;
        if (det(v, tff) <= 0.0)
        {
            tff = Vector2D(-tff.x(), -tff.y());
        }
        tff = tff.normalized();
        INDIC = 0.0;
    }
    else if (abs(delta) <= seuil)
    {
        Vector2D w1 = Vector2D(-aw0.y(), aw0.x());
        double aux1 = a0 / w0aw0;
        Vector2D vtemp = Vector2D(-ellipse.coordonne().x(), -ellipse.coordonne().y());
        v = vtemp + aux1 * w0;
        tff = aux1 * w1;
        if (det(v, tff) <= 0.0)
        {
            tff = Vector2D(-tff.x(), -tff.y());
        }
        tff = tff.normalized();
        INDIC = 0.0;
    }
    else
    {
        v = Vector2D(0, 0);
        tff = Vector2D(0, 0);
        INDIC = 10;
    }
    return {v, tff, ijn, INDIC};
}

VitesseOpt AlgoOpt::vitesseMax(Vector2D direction, Point2D ij)
{
    EllipseOpt ellipse = carteEllipse[ij.x()][ij.y()];
    if (direction.norm() <= seuil)
    {
        cout << "erreur dans vitessemax, vd=0" << std::endl;
        exit(EXIT_FAILURE);
    }
    direction = direction.normalized();
    Point2D ijd = Point2D(ellipse.vecteur().dot(direction),
                          det(ellipse.vecteur(), direction));
    Point2D ijc = Point2D(ellipse.vecteur().dot(ellipse.coordonne()),
                          det(ellipse.vecteur(), ellipse.coordonne()));

    double avdvd = (ijd.x() * ijd.x()) / (ellipse.a() * ellipse.a()) +
                   (ijd.y() * ijd.y()) / (ellipse.b() * ellipse.b());
    double avdc = (ijd.x() * ijc.x()) / (ellipse.a() * ellipse.a()) +
                  (ijd.y() * ijc.y()) / (ellipse.b() * ellipse.b());
    double acc = (ijc.x() * ijc.x()) / (ellipse.a() * ellipse.a()) +
                 (ijc.y() * ijc.y()) / (ellipse.b() * ellipse.b());
    double delta = sqrt((avdc * avdc) - avdvd * (acc - 1));

    double aux1 = -avdc + delta;
    aux1 = aux1 / avdvd;

    Vector2D v = aux1 * direction;

    double aux3 = (ijd.x() * aux1 + ijc.x()) / (ellipse.a() * ellipse.a());
    double aux4 = (ijd.y() * aux1 + ijc.y()) / (ellipse.b() * ellipse.b());

    Vector2D tff = Vector2D(-aux3 * ellipse.vecteur().y() - aux4 * ellipse.vecteur().x(),
                            aux3 * ellipse.vecteur().x() - aux4 * ellipse.vecteur().y());

    double normtff = tff.norm();

    if (det(v, tff) <= 0.0)
    {
        tff = Vector2D(-tff.x(), -tff.y());
    }

    tff = tff.normalized();

    double INDIC = 0.0;

    return VitesseOpt(v, tff, ij, INDIC);
}

vector<vector<VitesseOpt>> AlgoOpt::vitesseRajoutes(Point2D ij, Vector2D v0, Vector2D v1, int kmax)
{
    if (affichage)
        cout << "Debut vitesseRajoutes" << endl;
    EllipseOpt e = carteEllipse[ij.x()][ij.y()];
    if (affichage)
        cout << "ellipse : " << e << endl;
    Vector2D vv0 = vitesseMax(v0, ij).vecteur();
    Vector2D vv1 = vitesseMax(v1, ij).vecteur();

    Vector2D v00 = Vector2D(vv0.dot(e.vecteur()), det(e.vecteur(), vv0));
    Vector2D v11 = Vector2D(vv1.dot(e.vecteur()), det(e.vecteur(), vv1));
    Vector2D ce = Vector2D(e.vecteur().dot(e.coordonne()), det(e.vecteur(), e.coordonne()));
    double theta0 = atan2((v00.y() + ce.y()) / e.b(), (v00.x() + ce.x()) / e.a());
    double thetaf = atan2((v11.y() + ce.y()) / e.b(), (v11.x() + ce.x()) / e.a());

    if (thetaf < theta0 - seuil)
    {
        thetaf = thetaf + 2 * M_PI;
    }
    if (affichage)
        cout << "thethaf : " << thetaf << ", thetha0 : " << theta0 << endl;
    double t1 = atan2(-e.vecteur().x() / e.a(), -e.vecteur().y() / e.b());

    if (t1 < thetaf - seuil)
    {
        t1 += 2 * M_PI;
    }
    double t2 = atan2(-e.vecteur().y() / e.a(), -e.vecteur().x() / e.b());

    if (t2 < theta0 - seuil)
    {
        t2 += 2 * M_PI;
    }
    double t3 = atan2(e.vecteur().x() / e.a(), e.vecteur().y() / e.b());
    if (t3 < theta0 - seuil)
    {
        t3 += 2 * M_PI;
    }
    double t4 = atan2(e.vecteur().y() / e.a(), -e.vecteur().x() / e.b());
    if (t4 < theta0 - seuil)
    {
        t4 += 2 * M_PI;
    }

    std::vector<double> listheta;

    listheta.push_back(t1);
    listheta.push_back(t2);
    listheta.push_back(t3);
    listheta.push_back(t4);

    double KMAX = floor((thetaf - theta0) * kmax / (2 * M_PI)) + 1;

    for (double k = 0.0; k <= KMAX; k += 1.0)
    {
        listheta.push_back(theta0 + (thetaf - theta0) * k / KMAX);
    }

    std::vector<double> listhetab;

    listhetab.reserve(listheta.size());
    for (double &it : listheta)
    {
        listhetab.push_back(round(100000 * it));
    }

    //std::sort(listhetab.begin(), listhetab.end());
    //std::unique(listhetab.begin(), listhetab.end());

    std::sort(listheta.begin(), listheta.end());
    std::vector<double>::iterator it_theta = listheta.begin();
    vector<vector<VitesseOpt>> listvn;
    for (int k = 0; k < listhetab.size(); k += 1)
    {
        double t = listhetab.at(k) / 100000;
        if (t <= thetaf)
        {
            double v1 = -e.coordonne().x() + e.a() * cos(t) * e.vecteur().x() - e.b() * sin(t) * e.vecteur().y();
            double v2 = -e.coordonne().y() + e.a() * cos(t) * e.vecteur().y() + e.b() * sin(t) * e.vecteur().x();
            double v3 = -e.a() * sin(t) * e.vecteur().x() - e.b() * cos(t) * e.vecteur().y();
            double v4 = -e.a() * sin(t) * e.vecteur().y() + e.b() * cos(t) * e.vecteur().x();
            double normTFF = sqrt((v3 * v3) + (v4 * v4));
            v3 = v3 / normTFF;
            v4 = v4 / normTFF;

            VitesseOpt v = VitesseOpt(Vector2D(v1, v2), Vector2D(v3, v4), ij, 0.0);
            vector<VitesseOpt> temp;
            temp.push_back(v);
            listvn.push_back(temp);
        }
    }
    if (affichage)
        cout << "Fin vitesseRajoute " << endl;
    return listvn;
}

pair<double, vector<vector<VitesseOpt>>> AlgoOpt::vitesseChocOuPas(VitesseOpt vip, VitesseOpt vis)
{
    // VIP vitesse précédente
    // VIS vitesse suivante
    // VITC = liste de vitesse
    // INDIC = 0 si pas de choc, INDIC =  1 si c'est un choc, INDIC = 2 si VIP=VIS
    // on n'a pas besoin de l'ellipse car VIP et VIS contiennent les directions conjuquées (tangentes au front)),
    if (affichage)
        cout << "Debut vitesseChocOuPas" << endl;
    Vector2D vp = vip.vecteur();
    Vector2D tp = vip.tau();
    Vector2D vs = vis.vecteur();
    Vector2D ts = vis.tau();
    Point2D ijp = vip.coordonne();
    Point2D ijs = vis.coordonne();
    double indicp = vip.indice();
    double indics = vis.indice();
    int kmax = 50;
    Vector2D vectorTemp;
    float indic;
    float alpha;
    vector<vector<VitesseOpt>> listvn;
    vector<vector<VitesseOpt>> vitc;
    Vector2D vtemp = ijp - ijs;
    double test = vtemp.norm() + indics + indicp;

    if (test > seuil)
    {
        cout << "erreur dans VIP ou VIS dans vitessechocoupas test = " << test << std::endl;
        exit(EXIT_FAILURE);
    }
    test = det(tp, ts);
    vectorTemp = vp - vs;
    if (vectorTemp.norm() < seuil)
    {
        indic = 2.0;
        vector<VitesseOpt> temp;
        temp.push_back(vip);
        vitc.push_back(temp);
    }
    else if (test < -seuil)
    {
        alpha = (det(vs - vp, ts)) / test;
        Vector2D vc = vp + (alpha * tp);
        VitesseOpt vitesseTemp = VitesseOpt(vc, Vector2D(0, 0), ijp, 1.0);
        vector<VitesseOpt> temp;
        temp.push_back(vitesseTemp);
        temp.push_back(vip);
        temp.push_back(vis);
        vitc.push_back(temp);
        indic = 1.0;
    }
    else
    {
        Vector2D vip_bis = vip.vecteur();
        Vector2D vis_bis = vis.vecteur();
        listvn = vitesseRajoutes(ijp, vip_bis, vis_bis, kmax);
        vector<VitesseOpt> temp;
        temp.push_back(vip);
        vitc.push_back(temp);

        if (listvn.size() > 1)
        {
            for (auto it = (listvn.begin() + 1); it != (listvn.end() - 1); ++it)
            {
                vitc.push_back(*it);
            }
        }
        vector<VitesseOpt> temp2;
        temp2.push_back(vis);
        vitc.push_back(temp2);
        indic = 0.0;
    }
    if (affichage)
        cout << "Fin vitesseChocOuPas indic = " << indic << endl;
    return std::make_pair(indic, vitc);
}

VitesseOpt AlgoOpt::vitesse0(Point2D p, Point2D s)
{
    return VitesseOpt();
}

VitesseOpt AlgoOpt::vitesse(const VitesseOpt& vb, const Point2D& ij)
{

    EllipseOpt ellipse = carteEllipse[ij.x()][ij.y()];
    Vector2D vv = vb.vecteur();
    Point2D ijo = vb.coordonne();
    Vector2D tauperp = ij - ijo;
    if (tauperp.norm() != 0)
    {
        cout << "Erreur dans vitesse, tauperp.norm() != 0" << endl;
        exit(EXIT_FAILURE);
    }
    Vector2D tau = Vector2D(tauperp.y(), -tauperp.x());
    double a0 = 1 / (vv.dot(tau));
    if (tauperp.norm() != 1)
    {
        cout << "Erreur dans vitesse, tauperp.norm() != 1" << endl;
        exit(EXIT_FAILURE);
    }
    return vitessea0(-a0, Vector2D(-tau.x(), -tau.y()), ij);
}

pair<bool, vector<vector<VitesseOpt>>> AlgoOpt::vitesseChocNew(vector<vector<VitesseOpt>> vits, Vector2D tauperp)
{
    if (affichage)
    {
        cout << "Debut vitesseChocNew" << endl;
        cout << "Taille : " << vits.begin()->size() << endl;
    }
    if (vits.begin()->size() != 3)
    {
        cout << "Taille : " << vits.size() << endl;
        cout << "Taille : " << vits.begin()->size() << endl;
        cout << "erreur 1 dans vitesseCHocNew" << endl;
        exit(EXIT_FAILURE);
    }
    else if (tauperp.x() * tauperp.y() != 0)
    {
        cout << "erreur 2 dans vitesseCHocNew" << endl;
        exit(EXIT_FAILURE);
    }
    VitesseOpt vc = *vits.begin()->begin();
    Point2D ij = vc.coordonne();
    Point2D ijn = ij + tauperp;
    VitesseOpt vp = *(vits.begin()->begin() + 1);
    VitesseOpt vs = *(vits.begin()->begin() + 2);
    Vector2D tau = Vector2D(-tauperp.y(), tauperp.x());
    double a0s = azero(tau, vs);
    double a0p = azero(tau, vp);
    VitesseOpt vns = vitessea0(a0s, tau, ijn);
    VitesseOpt vnp = vitessea0(a0p, tau, ijn);
    if (vns.indice() == 0 && vnp.indice() == 0)
    {
        pair<double, vector<vector<VitesseOpt>>> chocOupas = vitesseChocOuPas(vnp, vns);
        if (chocOupas.first != 1)
        {
            return make_pair(false, chocOupas.second);
        }
        else
        {
            return make_pair(true, chocOupas.second);
        }
    }
    else
    {
        return make_pair(false, vits);
    }
}

pair<bool, vector<vector<VitesseOpt>>> AlgoOpt::vitesseNew1(vector<vector<VitesseOpt>> vits, Vector2D tauperp)
{
    /*tauperp, vecteur perpendiculaire à la bordure dans le sens de propagation*/
    if (affichage)
        cout << "Debut vitesseNew1, la liste des vitesses et le tau appartiennent au point qui est a propager" << endl;
    VitesseOpt vit = *vits.begin()->begin();
    Point2D ij = vit.coordonne();
    /*ij -> coordonnées de la cellule d'origine*/
    if (affichage)
        cout << "ij : " << ij << endl;
    Vector2D tau = Vector2D(-tauperp.y(), tauperp.x());
    /*tau -> vecteur perpendiculaire au tauperp*/
    if (affichage)
        cout << "tauperp : " << tauperp << endl;
    if (affichage)
        cout << "tau : " << tau << endl;
    Point2D ijn = ij + tauperp;
    /*Coordonnées de la nouvelle cellule*/
    if (affichage)
        cout << "ijn : " << ijn << endl;
    double a0 = azero(tau, vit);
    if (affichage)
        cout << "a0 : " << a0 << endl;

    VitesseOpt v = vitessea0(a0, tau, ijn);
    if (affichage)
    {
        if (v.indice() == 10)
        {
            v.affiche();
        }
    }

    vector<vector<VitesseOpt>> result;
    if (v.indice() == 0)
    {
        vector<VitesseOpt> temp;
        temp.push_back(v);
        result.push_back(temp);
        if (affichage)
            cout << "Sortie de vitesseNew1 via v.indice == 0 " << endl;
        return make_pair(true, result);
    }
    else
    {
        if (a0 > seuil)
        {
            VitesseOpt vb = vitesseMax(tau, ijn);
            double b0 = 1 / vb.vecteur().norm();
            Vector2D taun = Vector2D(-tau.x(), -tau.y());
            VitesseOpt vr = vitessea0(-b0, taun, ij);
            pair<double, vector<vector<VitesseOpt>>> chocOupas = vitesseChocOuPas(vr, vit);
            vector<vector<VitesseOpt>> listvn = vitesseRajoutes(ijn, tauperp, tau, 20);
            bool tailleValide;
            if (listvn.size() >= 3)
            {
                listvn.erase(listvn.begin());
                listvn.erase(listvn.end());
                tailleValide = true;
            }
            else
            {
                tailleValide = false;
            }
            vector<VitesseOpt> temp1;
            temp1.push_back(vb);
            temp1.push_back(vr);
            vector<vector<VitesseOpt>> temp2;
            temp2.push_back(temp1);
            if (tailleValide)
            {
                listvn.insert(listvn.end(), temp2.begin(), temp2.end());
                listvn.insert(listvn.end(), chocOupas.second.begin(), chocOupas.second.end());
                if (affichage)
                    cout << "fin vitessenew1 > seuil" << endl;
                return make_pair(true, listvn);
            }
            else
            {
                temp2.insert(temp2.end(), chocOupas.second.begin(), chocOupas.second.end());
                if (affichage)
                    cout << "fin vitessenew1 > seuil" << endl;
                return make_pair(true, temp2);
            }
        }
        else if (a0 < -seuil)
        {
            Vector2D taun = Vector2D(-tau.x(), -tau.y());
            VitesseOpt vb = vitesseMax(taun, ijn);
            double b0 = 1 / vb.vecteur().norm();
            VitesseOpt vr = vitessea0(b0, taun, ij);
            pair<double, vector<vector<VitesseOpt>>> chocOupas = vitesseChocOuPas(vit, vr);
            if (affichage)
                cout << "double de chocOuPas : " << chocOupas.first << " Taille second elt : " << chocOupas.second.at(0).size() << endl;
            if (affichage)
                cout << "taun : " << taun << ", tauperp : " << tauperp << endl;
            vector<vector<VitesseOpt>> listvn = vitesseRajoutes(ijn, taun, tauperp, 20);
            if (affichage)
                cout << "taille listvn : " << listvn.size() << endl;
            bool tailleValide;
            if (listvn.size() >= 3)
            {
                listvn.erase(listvn.begin());
                listvn.erase(listvn.end());
                tailleValide = true;
            }
            else
            {
                tailleValide = false;
            }
            if (affichage)
                cout << "taille listvn : " << listvn.size() << endl;
            vector<VitesseOpt> temp1;
            temp1.push_back(vb);
            temp1.push_back(vr);
            vector<vector<VitesseOpt>> temp2;
            temp2.push_back(temp1);
            if (affichage)
                cout << "Taille chocOupas : " << chocOupas.second.size() << endl;
            chocOupas.second.insert(chocOupas.second.end(), temp2.begin(), temp2.end());
            if (affichage)
                cout << "Taille chocOupas : " << chocOupas.second.size() << endl;
            if (tailleValide)
            {
                chocOupas.second.insert(chocOupas.second.end(), listvn.begin(), listvn.end());
            }
            if (affichage)
                cout << "Taille chocOupas : " << chocOupas.second.size() << endl;
            if (affichage)
                cout << "fin vitessenew1 < seuil, taille de la liste de retour : " << chocOupas.second.size() << endl;
            return make_pair(true, chocOupas.second);
        }
        else
        {
            if (affichage)
                cout << "erreur dans vitesseNew1 : a0=0" << endl;
            exit(EXIT_FAILURE);
        }
    }
}

/*********************************************************************************************
 *                              Fonction de gestion des bordures                             *
 *********************************************************************************************/
bool AlgoOpt::testBordureBrulee(const PointOpt& point)
{
    if (affichage)
        cout << "Dans testBordureBrulee" << endl;

    if (listeCoins.empty())
        return false;

    PointOpt m = point;
    Point2D ij = Point2D(floor(m.coordonne.x()), floor(m.coordonne.y()));
    string num = numero(ij);

    double tmin = listff.at(aPropager).t;

    if (listeCoins.count(num) == 0)
    {
        return false;
    }
    else
    {

        Vector2D dm = m.coordonne - ij; //Distance entre le point mvt et le sommet ij de la cellule
        if (dm.norm() > seuil)
        {
            Point2D ij1 = ij + dm / dm.norm(); // / dm.norm();//Ij1 correspond au deuxieme sommet de la Bordure sur laquelle le point mvt est
            ij1 = Point2D(round(ij1.x()), round(ij1.y()));
            string num1 = numero(ij1);

            if (affichage)
                cout << "listeSommets.count(num1) : " << listeCoins.count(num1) << endl;
            if (listeCoins.count(num1) == 0)
            { //Ici le deuxieme sommet n'a pas encore brule

                return false;
            }
            else
            { //Ici les deux sommets sont ou vont brules

                string ind = listeCoins.at(num).indicePointAllume; //On va chercher l'indice du point dans la cellule
                string ind1 = listeCoins.at(num1).indicePointAllume;
                double tmax = listeCoins.at(num).tIgnitionCoin;
                double tmax1 = listeCoins.at(num1).tIgnitionCoin;
                double tm = m.t;

                if (affichage)
                    cout << "tmax : " << tmax << ", tmax1 : " << tmax1 << ", tmin : " << tmin << endl;
                if (tmax <= tmin && tmax1 <= tmin)
                { //Ici on test si l'insertion va se faire sur une Bordure brule
                    return true;
                }
                /* Attention, la condition suivante a été ajoutée pour corriger un bug
                 * Cet ajout sert à tester si les deux bout de la Bordure auront brulé avant que le point arrive sur la Bordure*/
                if (tmax <= tm && tmax1 <= tm)
                {
                    return true;
                }

                Vector2D dij = listeCoins.at(num).pts;
                Vector2D dij1 = listeCoins.at(num1).pts;
                Vector2D ij1moinsij = ij1 - ij;

                if (dij.x() == 0 && dij.y() == 0 && (dij1.x() + dij1.y()) == 1 && dij1.determinant(ij1moinsij) == 0)
                {
                    if ((tmax - tm) * (tm - tmax1) <= 0 && tm >= tmax)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }
        }
        else
        {
            string ind = listeCoins.at(num).indicePointAllume;
            double tmax = listeCoins.at(num).tIgnitionCoin;
            double tm = m.t;

            if (tm > tmax)
            {
                if (affichage)
                    cout << listff.at(ind).getId() << " déjà brulé par " << ij << " sommet " << m.getId() << " m " << endl;
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}

void AlgoOpt::actualiseCoin(string ind)
{
    if (affichage)
        cout << "Dans actualise sommet" << endl;
    /*ind est l'indice d'un point*/
    /*Fonction qui actualise les temps d'ignition des sommets dans la prochaine cellule avec des vitessesmax issues de ind*/
    /*Recuperation ds informations du point*/
    PointOpt mt = listff.at(ind); //Le point
    Point2D ijo = (listff.at(ind).listeVitesse.begin())->begin()->coordonne();
    /*ijo = IJorigin --> Coordonnees de la case origin du point*/
    Point2D ijn = ijo + mt.tau;
    /*ijn = IJnew --> Coordonnees de la case arrive du point*/
    Point2D m = mt.coordonne;                              //Les coordonnees du point
    double t = mt.t;                                       //Le temps d'ignition du point
    Vector2D dm = m - Point2D(round(m.x()), round(m.y())); //dm sert a indiquer si le point est un sommet ou non
    bool indic;

    if (dm.norm() <= seuil) //mt est un sommet
    {
        Point2D ij = Point2D(round(mt.coordonne.x()), round(mt.coordonne.y()));
        double tnew = mt.t; //nouveau temps
        string num = numero(ij);

        /*Debut suspect*/
        if (mt.suivant == "5") //Erreur a corriger (pts suivant)
        {
            (listff.at(ind).listeVitesse.begin())->begin()->setIndice(3);
            indic = false;
        }
        /*Fin suspect*/
        else
        {
            indic = true; /*Gestion des sommets plus tard*/
        }

        if (listeCoins.count(num) == 0)
        {
            //Si le sommet n'est pas present on l'insere
            listeCoins.insert(make_pair(num, CoinCase(num, tnew, ind, indic, 0, 0)));
        }
        else
        {
            double tOld = listeCoins.at(num).tIgnitionCoin;
            if (tnew <= tOld)
            {
                /*Si le sommet est deja present et que le tnew est inferieur au told, on ecrase la valeur avec un nouveau sommet*/
                listeCoins.erase(num);
                CoinCase sc = CoinCase(num, tnew, ind, indic, 0, 0);
                listeCoins.insert(make_pair(num, sc));
            }
        }
    }
    else
    {
        vector<Point2D> listDIJ; /*Liste des coordonnees*/
        listDIJ.emplace_back(Point2D(0, 0));
        listDIJ.emplace_back(Point2D(1, 0));
        listDIJ.emplace_back(Point2D(1, 1));
        listDIJ.emplace_back(Point2D(0, 1));

        /*On va parcourir les quatres sommets d'une case*/
        for (Point2D dij : listDIJ)
        {
            Point2D ij = ijn + dij; //Sommet qu'on est en train de parcourir
            indic = false;
            Vector2D dir = ij - m; //Vecteur entre l'endroit d'intersection entre les cases et le sommet actuel
            VitesseOpt vit = vitesseMax(dir, ijn);
            double dt = dir.norm() / vit.vecteur().norm();
            double tnew = t + dt;    //Temps d'ignition du sommet ij par le point mt
            string num = numero(ij); //Indice de la cellule

            if (listeCoins.count(num) == 0) //Si le numero ne fait pas deja parti de la list
            {
                //On insere
                listeCoins.insert(make_pair(num, CoinCase(num, tnew, ind, indic, dij)));
            }
            else
            {
                double tMaxOld = listeCoins.at(num).tIgnitionCoin; //SommetCase.t a l'endroit ou se trouve deja le num (loc)
                bool indicOld = listeCoins.at(num).indic;          //Pareil mais pour l'indic
                if (tnew <= tMaxOld && indicOld != 1)              //Si le nouveau temps est plus tot on modif
                {
                    listeCoins.erase(num);
                    CoinCase sc = CoinCase(num, tnew, ind, indic, dij);
                    listeCoins.insert(make_pair(num, sc));
                }
                else if (tnew <= tMaxOld && indicOld == 1)
                {
                    listeCoins.erase(num);
                    CoinCase sc = CoinCase(num, tnew, ind, indic, dij);
                    listeCoins.insert(make_pair(num, sc));
                }
                //Si le tnew est plus grand on ne fait rien
            }
        }
    }
}

void AlgoOpt::initialiserCoinContourDepart(PointOpt mvt)
{

    vector<Point2D> listDIJ; /*Liste des coordonnees*/
    listDIJ.emplace_back(Point2D(0, 0));
    listDIJ.emplace_back(Point2D(1, 0));
    listDIJ.emplace_back(Point2D(1, 1));
    listDIJ.emplace_back(Point2D(0, 1));

    Point2D m = mvt.coordonne;
    string ind = mvt.getId();
    Point2D ijo = Point2D(floor(m.x()), floor(m.y()));

    /*On va parcourir les quatres sommets d'une case*/
    for (Point2D dij : listDIJ)
    {
        Point2D ij = ijo + dij;  //Sommet qu'on est en train de parcourir
        string num = numero(ij); //Indice de la cellule

        if (listeCoins.count(num) == 0) //Si le numero ne fait pas deja parti de la liste
        {
            //On insere
            listeCoins.insert(make_pair(num, CoinCase(num, 0, ind, false, dij)));
        }
    }
}

/*********************************************************************************************
 *                              Fonctions de gestion                                         *
 *********************************************************************************************/
bool AlgoOpt::saisieEstSensHoraire(vector<Point2D> listE)
{
    double somme = 0.0;
    int taille = listE.size();
    Vector2D Op;
    Vector2D ukp1;
    for (int i = 0; i < taille; i++)
    {
        Op = Vector2D(listE.at(i).x(), listE.at(i).y());
        ukp1 = Vector2D(listE.at((i + 1) % taille).x() - listE.at((i) % taille).x(), listE.at((i + 1) % taille).y() - listE.at((i) % taille).y());
        somme += Op.determinant(ukp1);
    }
    return somme < 0;
}

vector<Point2D> AlgoOpt::inverseSensList(vector<Point2D> listE)
{
    vector<Point2D> listTemp;
    for (int i = listE.size() - 1; i >= 0; i--)
    {
        listTemp.push_back(listE.at(i));
    }
    return listTemp;
}

Point2D AlgoOpt::coordinateToIndex(const Point2D &p)
{
    return {floor((p.x() - xmin) / tailleCellule), floor((p.y() - ymin) / tailleCellule)};
}

Point2D AlgoOpt::coordinateToCoordinateCalc(const Point2D &p)
{
    return {(p.x() - xmin) / tailleCellule, (p.y() - ymin) / tailleCellule};
}

Point2D AlgoOpt::indextoCoordinate(const Point2D &p) const
{
    return {tailleCellule * p.x() + xmin, tailleCellule * p.y() + ymin};
}

// set the smallest x and y value of carteVent to xmin and ymin
void AlgoOpt::setXminYmin(json carteVent)
{
    double xmintemp = -1;
    double ymintemp = -1;
    //iterate carteVent to find the smallest x and y value
    for (const auto &it : carteVent)
    {
        if (it["x"] < xmintemp || xmintemp == -1)
            xmintemp = it["x"];
        if (it["y"] < ymintemp || ymintemp == -1)
            ymintemp = it["y"];
    }
    xmin = xmintemp;
    ymin = ymintemp;
}

string AlgoOpt::numero(Point2D ij)
{
    return to_string(ij.x()) + "_" + to_string(ij.y());
}

/*********************************************************************************************
 *                              Fonctions pour l'application de test                         *
 *********************************************************************************************/
void AlgoOpt::sauvegarde_liste_vitesses(ofstream &contour_final_detail) const
{
    json liste_points = nullptr;
    json vitesses = nullptr;

    Point2D convertedCoord;
    PointOpt premier = listff.at(aPropager);
    double tempsMin = premier.t;
    string suiv = aPropager;
    PointOpt point = listff.at(suiv);

    int nb_points_parc = 0;

    while (nb_points_parc < listff.size() && (nb_points_parc == 0 || listff.at(suiv).getId() != premier.getId()))

    {
        convertedCoord = point.coordonne;
        for (auto &row : point.listeVitesse)
        {
            for (auto &vitesse : row)
            {
                convertedCoord = convertedCoord + (tempsMin - point.t) * vitesse.vecteur();

                vitesses.push_back({{"indice", vitesse.indice()},
                                    {"id", suiv},
                                    {"coordonnees", indextoCoordinate(convertedCoord).toPair()},
                                    {"coordonnees_calcul", point.coordonne.toPair()},
                                    {"tau", vitesse.tau().toPair()},
                                    {"vecteur", vitesse.vecteur().toPair()}});
            }
            liste_points.push_back({{"indice", point.indice},
                                    {"id", point.getId()},
                                    {"coordonnees", indextoCoordinate(point.coordonne).toPair()},
                                    {"coordonnees_calcul", point.coordonne.toPair()},
                                    {"temps", point.t},
                                    {"tau", point.tau.toPair()},
                                    {"ancetre", point.ancetre},
                                    {"precedent", point.precedent},
                                    {"suivant", point.suivant},
                                    {"creation", point.creation},
                                    {"vitesses", vitesses}});
            vitesses = nullptr;
        }
        suiv = point.suivant;
        point = listff.at(suiv);
        nb_points_parc++;
    }

    if (contour_final_detail)
    {
        contour_final_detail << liste_points;
        cout << "#######################################################" << endl;
        cout << "La liste de points a ete sauvegardee." << endl;
    }
    else
    {
        cout << "#######################################################" << endl;
        cout << "[ERROR] La liste de points n'a pas pu etre sauvegardee." << endl;
    }
}

void AlgoOpt::sauvegarder_liste_sommets(ofstream &etat_final_liste_sommets) const
{
    cout << "Nombre de sommets dans la liste : " << listeCoins.size() << endl;

    json liste_sommets_a_sauvegarder = nullptr;
    pair<double, double> coordonnees;
    if (etat_final_liste_sommets)
    {
        for (auto const &sommet : listeCoins)
        {
            coordonnees = numToCoordinate(sommet.second.numCoin);
            liste_sommets_a_sauvegarder.push_back({{"coordonnees", indextoCoordinate(
                                                                       Point2D(coordonnees.first, coordonnees.second))
                                                                       .toPair()},
                                                   {"coordonnees_calcul", Point2D(coordonnees.first, coordonnees.second).toPair()},
                                                   {"indice_allume", sommet.second.indicePointAllume},
                                                   {"t_ignition", sommet.second.tIgnitionCoin}});
        }
        etat_final_liste_sommets << liste_sommets_a_sauvegarder;
        cout << "#######################################################" << endl;
        cout << "L'etat de la liste de sommets a ete sauvegarde." << endl;
    }
    else
    {
        cout << "#######################################################" << endl;
        cout << "[ERROR] L'etat de la liste de sommets n'a pas pu etre sauvegarde." << endl;
    }
}

pair<double, double> AlgoOpt::numToCoordinate(const string &num) const
{
    string buffer;
    double x, y;
    for (char it : num)
    {
        if (it == '_')
        {
            x = stod(buffer);
            buffer = "";
        }
        else
        {
            buffer += it;
        }
    }
    y = stod(buffer);
    return make_pair(x, y);
}

/*********************************************************************************************
 *                              Fonctions d'affichage                                        *
 *********************************************************************************************/
void AlgoOpt::affiche(std::vector<vector<VitesseOpt>> v)
{
    int i = 0;
    for (auto &it : v)
    {
        it.begin()->affiche();
        i++;
    }
}

void AlgoOpt::affiche(std::pair<double, std::vector<vector<VitesseOpt>>> v)
{
    affiche(v.second);
}

/*********************************************************************************************
 *                              Autre                                                        *
 *********************************************************************************************/
string AlgoOpt::getMinlisteCellulesBrulees()
{
    string min = "0";
    for (int i = 0; i < listeCellulesBrulees.size(); i++)
    {
        if (listePointsActifs.at(listeCellulesBrulees.at(i)) < listePointsActifs.at(min))
        {
            min = listeCellulesBrulees.at(i);
        }
    }
    return min;
}

float AlgoOpt::det(Vector2D v1, Vector2D v2)
{
    return v1.x() * v2.y() - v1.y() * v2.x();
}

//trouve un point avec son id
//retourne null si pas trouvé
PointOpt AlgoOpt::findPointOptByIndice(const vector<PointOpt>& listDef, const string& indice)
{
    for (auto &i : listDef)
    {
        if (i.getId() == indice)
        {
            return i;
        }
    }
    return nullptr;
}

void AlgoOpt::triDesChocs()
{
    Choc element;
    int j;
    for (int i = 1; i < listeChoc.size(); i++)
    {
        element = listeChoc.at(i);
        for (j = i; j > 0 && listeChoc.at(j - 1).temps > element.temps; j--)
        {
            listeChoc.at(j) = listeChoc.at(j - 1);
        }
        listeChoc.at(j) = element;
    }
}
