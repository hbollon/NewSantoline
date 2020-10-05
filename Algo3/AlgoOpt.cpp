//
// Created by Skaldr on 7/11/2019.
//
#include "json.hpp"
using json = nlohmann::json;
#include <algorithm>
#include <fstream>
#include "math.h"
#include "AlgoOpt.hpp"
#include "EllipseOpt.hpp"

using namespace std;

char NUM_DELIMITER = '_';

float det(Vector2D v1, Vector2D v2){
    return v1.x()*v2.y() - v1.y() * v2.x();
}

bool AlgoOpt::saisieEstSensHoraire(vector<Point2D> listE)
{
    double somme = 0.0;
    int taille = listE.size();
    Vector2D Op;
    Vector2D ukp1;

    for(int i = 0;i < taille;i++)
    {
        Op = Vector2D(listE.at(i).x(),listE.at(i).y());
        ukp1 = Vector2D(listE.at((i+1)%taille).x()-listE.at((i)%taille).x(),listE.at((i+1)%taille).y()-listE.at((i)%taille).y());

        somme += Op.determinant(ukp1);

    }
    return somme < 0;


}

vector<Point2D> AlgoOpt::inverseSensList(vector<Point2D> listE)
{
    vector<Point2D> listTemp;

    for(int i = listE.size()-1;i>=0;i--)
    {
        listTemp.push_back(listE.at(i));
    }
    return listTemp;

}

vector<Point3D> AlgoOpt::propagation(vector<Point2D> listE, json m, json p, ofstream& sortie, ofstream& infos_algo, ofstream& etat_final_liste_sommets){

    /*Ces trois listes servent à tester que l'id du point actuel correpond bien au suivant du point preceddent et au precedent du point suivant*/
    vector<string> id;
    vector<string> suivant;
    vector<string> precedent;
    double ancientmin;

    json infos_json = nullptr;
    double tempsPropagation = timeToDouble(p["duree"]);

    int compt;

/*********************************************************************************************
 *                              ZONE DE "POINT DE COLLE"                                     *
 *********************************************************************************************/
        initialiserEllipse(m, sortie, p["largeur"], p["hauteur"], p["waterReserve"], p["temperature"], tempsPropagation);
        if (saisieEstSensHoraire(listE)) {
            cout << "Sens horaire !" << endl;
            listE = inverseSensList(listE);
        }
        for (auto &i : listE) {
            i = coordinateToCoordinateCalc(i);
        }
/*********************************************************************************************
 *                              FIN DE "POINT DE COLLE"                                      *
 *********************************************************************************************/

/*********************************************************************************************
 *                                      ALGORITHME                                           *
 *********************************************************************************************/
    initListff(listE);
    initialisationListff = false;
    compt = 0;
    vector<Point3D> r;
    cout << "############################################################################" << endl;
    cout << "temps propagation ===> " << tempsPropagation << " secondes." << endl;
    cout << "############################################################################" << endl;

    int nmax = listff.size();
    int nmaxnewn;
    int nmaxnew;
    ancientmin = listff.at(aPropager).t;

    int iterationAffichage = 802222;
    double tfinal = (tempsPropagation / 3600.0);

    while(listff.at(aPropager).t <= tfinal) {
        if(compt == iterationAffichage) affichage = true;

        if(ancientmin > listff.at(aPropager).t) {
            cout << "Le temps min precedent est plus grand que le temps min actuel !" << endl;
            cout <<"Ancien tmin : "<<ancientmin<<", tmin : "<<listff.at(aPropager).t<<endl;
        }

        ancientmin = listff.at(aPropager).t;
        cout << "----" << compt << "----" << endl;
        if(affichage) cout << "aPropager : " << aPropager << endl;
        if(affichage) cout << "getId() : " << listff.at(aPropager).getId() << endl;
        if(affichage) cout << "avant t : " << listff.at(aPropager).t << " temps final : " << (tempsPropagation / 3600.0) << endl;

        uneIteration(aPropager);

        nmaxnewn = listff.size();
        nmaxnew = nmax;

        while (nmaxnewn > nmaxnew && (nmaxnewn - nmaxnew <= 20)) {
            for (int k = nmaxnew; k < nmaxnewn; k++) {
                rajoutePoints(to_string(k));
            }
            nmaxnew = nmaxnewn;
            nmaxnewn = listff.size();
        }

        nmaxnew = listff.size();
        for (int k = nmax ; k < nmaxnew; k++) {
            actualiseSommet(to_string(k));
        }
        nmax = nmaxnew;
        if(affichage) cout << "apres t : " << listff.at(aPropager).t << " temps final : " << (tempsPropagation / 3600.0) << endl;

        compt++;
    }
    cout << "-------------------------------------------------------" << endl;
    cout << "t : " <<listff.at(aPropager).t << "  temps : " << tempsPropagation / 3600.0 <<endl;


    int nb_points_parc = 1;
    double tempMin = listff.at(aPropager).t;
    PointOpt premier = listff.at(aPropager);
    cout<<"Premier boucle final : "<< premier.getId()<<endl;
    cout<<"Temp min : "<<tempMin<<endl;
    string suiv = premier.suivant;
    PointOpt temp;
    temp = listff.at(suiv);

    /*gestion du premier point*/
    //cout << "getId() : "<<premier.getId()<<", precedent : "<<premier.precedent<<", suivant : "<<premier.suivant<<", ancetre : "<<premier.ancetre<<", Creation : "<<premier.creation<<endl;
    vector<vector<VitesseOpt>> lvp = (premier.listeVitesse);
    Point2D pp = premier.coordonne;
    id.push_back(premier.getId());
    precedent.push_back(premier.precedent);
    suivant.push_back(premier.suivant);
    double t = premier.t;
    for(auto it2:lvp){
        Vector2D v = it2.begin()->vecteur();
        pp = pp + (((tempMin-t)*v));
        Point2D p2d = indextoCoordinate(pp);
        r.push_back(Point3D(p2d.x() ,p2d.y(),0));
    }

    while(nb_points_parc < listff.size() && temp.getId() != premier.getId() ){
        //cout << "getId() : "<<temp.getId()<<", precedent : "<<temp.precedent<<", suivant : "<<temp.suivant<<", ancetre : "<<temp.ancetre<<", Creation : "<<temp.creation<<endl;
        vector<vector<VitesseOpt>> lv = (temp.listeVitesse);
        Point2D pcoord = temp.coordonne;
        double ttemp = temp.t;
        id.push_back(temp.getId());
        precedent.push_back(temp.precedent);
        suivant.push_back(temp.suivant);
        for(auto it2:lv){
            Vector2D v = it2.begin()->vecteur();
            pcoord = pcoord + (((tempMin-ttemp)*v));
            Point2D p2d = indextoCoordinate(pcoord);
            r.push_back(Point3D(p2d.x() ,p2d.y(),0));
        }

        suiv = temp.suivant;
        temp = listff.at(suiv);
        nb_points_parc++;
    }
    cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<endl;
    /*sert tester que l'id du point actuel correpond bien au suivant du point preceddent et au precedent du point suivant*/
    for(int position = 0; position < id.size(); position++)
    if(position == 0)
    {
        if (id.at(position) != precedent.at((position + 1) % precedent.size()) ||
            id.at(position) != suivant.at( suivant.size() - 1)) {
            cout << "Erreur a la position : " << position << ", id : " << id.at(position) << ", suivant : "
                 << suivant.at(suivant.size() - 1)
                 << ", precedent : " << precedent.at((position + 1) % precedent.size()) << endl;
        }

    } else{
        {
            if (id.at(position) != precedent.at((position + 1) % precedent.size()) ||
                id.at(position) != suivant.at((position - 1) % suivant.size())) {
                cout << "Erreur a la position : " << position << ", id : " << id.at(position) << ", suivant : "
                     << suivant.at((position - 1) % suivant.size())
                     << ", precedent : " << precedent.at((position + 1) % precedent.size()) << endl;
            }
        }
    }
    cout <<"Taille de id : "<< id.size()<<", taille de suivant : "<< suivant.size() <<", taille de precedent : " <<precedent.size()<<endl;
    cout << "Taille listff fin " << listff.size()<<", nb_points_parc : "<<nb_points_parc << endl;


    if(nb_points_parc>=listff.size()){
        cout<<"Erreur, nb_points_parc >= listff.size"<<endl;
    }

    sauvegarder_liste_sommets(etat_final_liste_sommets);

    cout<<endl;
    if (infos_algo) {
        infos_json.push_back({
                                     {"algorithme", 3},
                                     {"temps", listff.at(aPropager).t},
                                     {"nb_iterations", compt},
                                     {"nb_points", nb_points_parc}
                             });

        infos_algo << infos_json;
        cout << "#######################################################" << endl;
        cout << "Les infos de l'algo ont ete sauvegardes." << endl;
    } else {
        cout << "#######################################################" << endl;
        cout << "[ERROR] Les infos de l'algo n'ont pas pu etre sauvegardes." << endl;
    }


    return r;
}

double AlgoOpt::azero(Vector2D tau, VitesseOpt Vit){

    Point2D ij = Vit.coordonne();
    if(affichage) cout<<"Debut azero"<<endl;
    if(affichage) cout <<"tau : "<<tau<<endl;
    if(affichage) cout <<"vit : " <<Vit.coordonne()<<endl;

    EllipseOpt ellipse = carteEllipse[ij.x()][ij.y()];
    if(affichage) cout<<"ellipse : "<<ellipse<<endl;
    Vector2D eperp = Vector2D(-ellipse.vecteur().y(), ellipse.vecteur().x());
    Vector2D V = Vit.vecteur();
    Vector2D W = V + Vector2D(ellipse.coordonne().x(), ellipse.coordonne().y());
    Vector2D VE = Vector2D(ellipse.vecteur().dot(W) , eperp.dot(W)  );
    Vector2D AW = Vector2D( VE.x()/(ellipse.a() * ellipse.a()) , VE.y()/(ellipse.b() * ellipse.b()) );
    Vector2D AW_Bis = AW.x() * ellipse.vecteur() + AW.y() * eperp;
    double aux1 = AW_Bis.dot(tau) ;
    double aux2 = AW_Bis.dot(V);
    double a0 = aux1/aux2;
    if(affichage) cout << "aux1 : "<<aux1<<", aux2 : "<<aux2<<endl;
    if(affichage) cout <<"Fin azero : " << a0 << endl;
    return a0;
}
//verif
VitesseOpt AlgoOpt::vitessea0(double a0, Vector2D tau, Point2D ij){
    if(affichage) cout<< "Debut vitessea0"<<endl;
    Vector2D tff;
    Vector2D v;
    double normTFF;
    double INDIC;

    EllipseOpt ellipse = carteEllipse[ij.x()][ij.y()];
    if(affichage) cout << "ellipse : "<<ellipse << endl;
    tau = tau.normalized();

    Vector2D tauperp = Vector2D(-tau.y(), tau.x());
    Vector2D eperp = Vector2D(-ellipse.vecteur().y(), ellipse.vecteur().x());

    Vector2D w0 = Vector2D(tau.x() + a0* ellipse.coordonne().x(),
                           tau.y() + a0* ellipse.coordonne().y());
    double w01 = w0.dot(ellipse.vecteur());
    double w02 = w0.dot(eperp);

    Vector2D aw0 = (w01/(ellipse.a()*ellipse.a()))*ellipse.vecteur()
            + (w02/(ellipse.b()*ellipse.b()))*eperp;


    double w0aw0 = w0.dot(aw0);
    double delta = w0aw0-(a0*a0);

    if(affichage) cout << "delta : "<<delta << endl;
    if(delta >= seuil){
        Vector2D w1 = Vector2D(-aw0.y(),aw0.x());
        double test = w1.dot(tauperp);
        if(test < 0){
            test = 1.0;
        }
        else{
            test = -1.0;
        }
        double aux1 = a0/w0aw0;
        double aux2 = ellipse.a()*ellipse.b()*sqrt(delta)/w0aw0;
        Vector2D vtemp = Vector2D ( -ellipse.coordonne().x(),-ellipse.coordonne().y());
        v = vtemp+aux1*w0+test*aux2*w1;
        tff = aux1*w1 - (test*aux2/(ellipse.a()*ellipse.a()*ellipse.b()*ellipse.b()))*w0;
        if( det(v, tff)  <= 0.0){
            tff = Vector2D(-tff.x(), -tff.y());
        }
        tff=tff.normalized();
        INDIC = 0.0;
    }
    else if(abs(delta) <= seuil)
    {
        Vector2D w1 = Vector2D(-aw0.y(),aw0.x());
        double aux1 = a0/w0aw0;
        Vector2D vtemp = Vector2D ( -ellipse.coordonne().x(),-ellipse.coordonne().y());
        v = vtemp+aux1*w0;
        tff = aux1*w1;
        if( det(v, tff)  <= 0.0){
            tff = Vector2D(-tff.x(), -tff.y());
        }
        tff=tff.normalized();
        INDIC = 0.0;
    }
    else{
        v = Vector2D(0,0);
        tff = Vector2D(0,0);
        INDIC = 10;
    }
    if(affichage) cout << "Delta " << delta << endl;
    if(affichage) cout << "Fin vitessea0"<<endl;
    return {v,tff,ij,INDIC};
}

VitesseOpt AlgoOpt::vitesseMax(Vector2D direction, Point2D ij){
    EllipseOpt ellipse = carteEllipse[ij.x()][ij.y()];
    if(direction.norm()<=seuil){
        cout << "erreur dans vitessemax, vd=0" << std::endl;
        exit (EXIT_FAILURE);
    }
    direction = direction.normalized();
    Point2D ijd = Point2D(ellipse.vecteur().dot(direction),
            det(ellipse.vecteur(),direction));
    Point2D ijc = Point2D(ellipse.vecteur().dot(ellipse.coordonne()),
            det(ellipse.vecteur(),ellipse.coordonne()));

    double avdvd = (ijd.x()*ijd.x())/ (ellipse.a()*ellipse.a())+
            (ijd.y()*ijd.y())/ (ellipse.b()*ellipse.b());
    double avdc = (ijd.x()*ijc.x())/ (ellipse.a()*ellipse.a())+
                  (ijd.y()*ijc.y())/ (ellipse.b()*ellipse.b());
    double acc = (ijc.x()*ijc.x())/(ellipse.a()*ellipse.a())+
            (ijc.y()*ijc.y())/ (ellipse.b()*ellipse.b());
    double delta = sqrt((avdc*avdc) - avdvd*(acc-1));

    double aux1 = -avdc + delta;
    aux1 = aux1/avdvd;

    Vector2D v = aux1*direction;

    double aux3 = (ijd.x() * aux1 + ijc.x())/(ellipse.a()*ellipse.a());
    double aux4 = (ijd.y() * aux1 + ijc.y())/(ellipse.b()*ellipse.b());

    Vector2D tff = Vector2D(-aux3 * ellipse.vecteur().y() - aux4 * ellipse.vecteur().x(),
                            aux3 * ellipse.vecteur().x() - aux4 * ellipse.vecteur().y());

    double normtff = tff.norm();

    if(det(v,tff) <= 0.0){
        tff = Vector2D(-tff.x(), -tff.y());

    }

    tff = tff.normalized();

    double INDIC = 0.0;

    return VitesseOpt(v,tff,ij,INDIC);
}
//verif
vector<vector<VitesseOpt>> AlgoOpt::vitesseRajoutes(Point2D ij, Vector2D v0, Vector2D v1, int kmax){
    if(affichage) cout << "Debut vitesseRajoutes" << endl;
    EllipseOpt e = carteEllipse[ij.x()][ij.y()];
    if(affichage) cout << "ellipse : "<< e <<  endl;
    Vector2D vv0 = vitesseMax(v0,ij).vecteur();
    Vector2D vv1 = vitesseMax(v1,ij).vecteur();

    Vector2D v00 = Vector2D(vv0.dot(e.vecteur()), det(e.vecteur(),vv0));
    Vector2D v11 = Vector2D(vv1.dot(e.vecteur()), det(e.vecteur(),vv1));
    Vector2D ce = Vector2D(e.vecteur().dot(e.coordonne()),det(e.vecteur(),e.coordonne()));
    double theta0 = atan2((v00.y()+ce.y())/e.b(),(v00.x()+ce.x())/e.a());
    double thetaf = atan2((v11.y()+ce.y())/e.b(),(v11.x()+ce.x())/e.a());

    if(thetaf < theta0-seuil){
        thetaf = thetaf+2*M_PI;
    }
    if(affichage) cout << "thethaf : "<<thetaf<<", thetha0 : "<<theta0<<endl;
    double t1 = atan2(-e.vecteur().x()/e.a(),-e.vecteur().y()/e.b());

    if(t1<thetaf-seuil){
        t1+=2*M_PI;
    }
    double t2 = atan2(-e.vecteur().y()/e.a(),-e.vecteur().x()/e.b());

    if(t2<theta0-seuil){
        t2+=2*M_PI;
    }
    double t3 = atan2(e.vecteur().x()/e.a(), e.vecteur().y()/e.b());
    if(t3<theta0-seuil){
        t3+=2*M_PI;
    }
    double t4 = atan2(e.vecteur().y()/e.a(), - e.vecteur().x()/e.b());
    if(t4<theta0-seuil){
        t4+=2*M_PI;
    }

    std::vector<double> listheta;

    listheta.push_back(t1);
    listheta.push_back(t2);
    listheta.push_back(t3);
    listheta.push_back(t4);

    double KMAX = floor((thetaf-theta0)*kmax/(2*M_PI))+1;

    for (double k = 0.0; k <= KMAX; k += 1.0){

        listheta.push_back(theta0 + (thetaf - theta0) * k/KMAX);
    }

    std::vector<double> listhetab;

    listhetab.reserve(listheta.size());
    for (double & it : listheta){

        listhetab.push_back( round(100000 * it) );
    }

    std::sort(listhetab.begin(), listhetab.end());
    std::unique(listhetab.begin(), listhetab.end());

    std::sort(listheta.begin(), listheta.end());
    std::vector<double>::iterator it_theta = listheta.begin();
    vector<vector<VitesseOpt>> listvn;
    for (int k = 0; k < listhetab.size(); k += 1){

        double t = listhetab.at(k)/100000;
        if(t <= thetaf){

            double v1 = -e.coordonne().x() + e.a()*cos(t)*e.vecteur().x() - e.b()*sin(t)*e.vecteur().y();
            double v2 = -e.coordonne().y() + e.a()*cos(t)*e.vecteur().y() + e.b()*sin(t)*e.vecteur().x();
            double v3 = -e.a() * sin(t) * e.vecteur().x() - e.b()*cos(t)*e.vecteur().y();
            double v4 = -e.a() * sin(t) * e.vecteur().y() + e.b()*cos(t)*e.vecteur().x();
            double normTFF = sqrt((v3*v3) + (v4*v4));
            v3 = v3/normTFF;
            v4 = v4/normTFF;

            VitesseOpt v = VitesseOpt(Vector2D(v1,v2), Vector2D(v3,v4),ij, 0.0);
            vector<VitesseOpt> temp;
            temp.push_back(v);
            listvn.push_back(temp);
        }
    }
    if(affichage) cout << "Fin vitesseRajoute "<<endl;
    return listvn;

}

pair<double,vector<vector<VitesseOpt>>> AlgoOpt::vitesseChocOuPas(VitesseOpt vip, VitesseOpt vis){
    if(affichage) cout << "Debut vitesseChocOuPas"<< endl;
    std::pair<double,double> p;
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
    Vector2D vtemp = ijp-ijs;
    double test = vtemp.norm() + indics + indicp;

    if (test > seuil){
        cout << "erreur dans VIP ou VIS dans vitessechocoupas test = " <<test<< std::endl;
        exit (EXIT_FAILURE);
    }

    test = det(tp,ts);
    vectorTemp= vp-vs;

    if(vectorTemp.norm() < seuil){
        indic = 2.0;
        vector<VitesseOpt> temp;
        temp.push_back(vip);
        vitc.push_back(temp);
    }
    else if (test < -seuil){
        alpha = (det(vs-vp,ts))/test;
        Vector2D vc = vp + (alpha*tp);

        VitesseOpt vitesseTemp = VitesseOpt(vc, Vector2D(0,0),ijp,1.0);
        vector<VitesseOpt> temp;

        temp.push_back(vitesseTemp);
        temp.push_back(vip);
        temp.push_back(vis);
        vitc.push_back(temp);
        indic=1.0;
    }

    else{

        Vector2D vip_bis = vip.vecteur();
        Vector2D vis_bis = vis.vecteur();
        listvn = vitesseRajoutes(ijp, vip_bis, vis_bis, kmax);
        vector<VitesseOpt> temp;

        temp.push_back(vip);
        vitc.push_back(temp);


        if(listvn.size()>1){
            for (auto it = (listvn.begin()+1) ; it != (listvn.end()-1); ++it){
                vitc.push_back(*it);

            }
        }


        vector<VitesseOpt> temp2;
        temp2.push_back(vis);
        vitc.push_back(temp2);
        indic = 0.0;
    }
    if(affichage) cout << "Fin vitesseChocOuPas indic = "<<indic << endl;
    return std::make_pair(indic,vitc);
}

pair<Point3D, Vector2D> AlgoOpt::mnplusun(Point3D point, VitesseOpt vitesse){
    if(affichage) cout << "Debut mnplusun" << endl;
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

    if(v.x()>seuil){
        m1 = ij.x()+1;
        dt1 = (m1-p.x())/v.x();
        tauperp1 = 1;
    }
    else if ( v.x() < -seuil){
        m1 = ij.x();
        dt1 = (m1-p.x())/v.x();
        tauperp1 = -1;
    }
    else{
        v.x()=0;
        dt1=1000000000;
        tauperp1=0;
    }

    if(v.y()> seuil){
        m2 = ij.y()+1;
        dt2 = (m2-p.y())/v.y();
        tauperp2 = 1;
    }
    else if ( v.y() < -seuil){
        m2 = ij.y();
        dt2 = (m2-p.y())/v.y();
        tauperp2 = -1;
    }
    else{
        v.y()=0;
        dt2=1000000000;
        tauperp2=0;
    }
    if(dt1 < dt2){
        pointRetour = Vector3D(m1, p.y()+dt1*v.y(), point.z()+dt1);
        vectorRetour = Vector2D(1,0);
    }
    else if ( dt1 > dt2){
        pointRetour = Vector3D(p.x()+dt2*v.x(), m2, point.z()+dt2);
        vectorRetour = Vector2D(0,1);
    }
    else if(dt1==1000000000){
        pointRetour = point;
        vectorRetour = Vector2D(0,0);
    }
    else{
        pointRetour = Vector3D(p.x()+dt2*v.x(), m2,point.z() + dt2 );
        vectorRetour = Vector2D(1,1);
    }
    vectorRetour = Vector2D(vectorRetour.x()*tauperp1,
            vectorRetour.y()*tauperp2);

    if(affichage) cout << "Fin de mnplusun"<<endl;
    return(std::make_pair(pointRetour,vectorRetour));

}
//verif
int AlgoOpt::supprimerCroisement(PointOpt mtp, std::string ind){
    if(affichage) cout << "SupprimerCroisement" << endl;
    listeChoc = vector<Choc>();
    int indic = 0;
    PointOpt mt = listff.at(ind);
    Point2D mm = mt.coordonne;
    Point2D mp = mtp.coordonne;
    double tmin = mt.t;
    double tp = mtp.t;
    Vector2D vtemp = Vector2D(mm-mp);

    if(vtemp.norm()<=seuil){/*Cas sommet*/
        cout<<"Dans supprimeCroisement mtp = mm IND="<<ind<<std::endl;
        return 1;/*TODO attention*/
        exit (EXIT_FAILURE);
    }

    Vector2D v = mtp.listeVitesse.begin()->begin()->vecteur();

    vector<string> indasuppr;
    vector<string> listNum;


    Point2D ij = (mtp.listeVitesse.begin())->begin()->coordonne();
    string num1 = numero(ij);
    listNum.push_back(num1);

    if(mtp.indice == "2"){
        listNum.push_back(numero(((mtp.listeVitesse.begin())->begin()+1)->coordonne()));
    }

    for (auto num:listNum){

        auto iterator = find(listeCellulesBrulees.begin(),listeCellulesBrulees.end(),num);
        if(iterator != listeCellulesBrulees.end()){
            for(int i = 0; i < listePointsDansCellule.at(num).size(); i++){
                PointOpt it = listff.at(listePointsDansCellule.at(num).at(i));

                Point2D mc = it.coordonne;
                string indoc = it.ancetre;
                Vector2D temp = (mc-mm);
                if( temp.norm() > seuil && ind != indoc){
                    VitesseOpt vitc = *(it.listeVitesse.begin())->begin();
                    Vector2D vc = vitc.vecteur();
                    double tc = it.t;
                    Vector2D dm = mm - mc;
                    double aux = det(vc,v);
                    if (abs(aux)> seuil ){
                        double test = tmin + det(dm,vc)/aux;
                        // test -> Temps au moment de l'intersection de (m->mvt) avec la trajectoire issue de mc en parcourant (m->mvt).
                        if(tp>=test && test >= tmin){//Verifie si on se trouve dans la cellule
                            double testc = tc + det(dm,v)/aux;
                            // testc -> Temps au moment de l'intersection de (m->mvt) en parcourant la trajectoire issue de mc.
                            Point2D mcr = mc +(testc - tc)*vc;//Point d'intersection des trajectoires
                            temp = mcr - mm;
                            if( temp.norm() > seuil && testc >= 0 ){
                                if(test+seuil<testc && tc >= tmin){
                                    if(affichage) cout<<"On supprime :"+it.getId()<<endl;
                                    indasuppr.push_back(it.getId());
                                }
                                else if (test>testc+seuil){
                                    return 1;
                                }
                                else if( abs(testc-test)<= seuil){
                                    if(mtp.indice=="2"){//On insere pas les sommets
                                        /*TODO supprimer quand on va traiter les sommets*/
                                        indasuppr.push_back(it.getId());
                                    }
                                    else if ( it.indice == "2"){
                                        /*TODO faire attention, gestion de sommets etrange*/
                                        return 1;
                                    }
                                    else{
                                        if(listeCellulesActivesSousContour.count(it.getId())>0){
                                            listeChoc.emplace_back(num,it.getId(),mcr,test);
                                            indic = 2;
                                        }
                                        else{
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
    ite = std::unique (indasuppr.begin(), indasuppr.end());

    indasuppr.resize( std::distance(indasuppr.begin(),ite) );
    if(affichage) cout << "Indasuppr taille : " << indasuppr.size() << endl;
    if(!indasuppr.empty()){
        for(auto &it:indasuppr){
            if(affichage) cout << "Suppression de : " << it << endl;
            supprime(it);
            for(unsigned int i=0; i<listeChoc.size(); i++){
                if (listeChoc[i].indic == it){

                    listeChoc.erase(listeChoc.begin() + i);
                }
            }
        }
    }

    return indic;
}


string AlgoOpt::getMinlisteCellulesBrulees()
{
    string min = "0";

    for(int i = 0;i<listeCellulesBrulees.size();i++)
    {
        if(listeCellulesActivesSousContour.at(listeCellulesBrulees.at(i)) < listeCellulesActivesSousContour.at(min))
        {
            min = listeCellulesBrulees.at(i);
        }
    }

    return min;


}



int AlgoOpt::propage(string ind, vector<vector<VitesseOpt>> vits){
    if(affichage) cout << "Debut de propage ind : " << ind<< endl;
    PointOpt mt = listff.at(ind);
    int compt = 1;
    int indic = 0;

    if(affichage) {
        cout << "Debut affichage des vitesses donnees en parametre de propage." << endl;
        for (auto &vit:vits) {
            cout << "vit.size() : " << vit.size() << endl;
            for (int i = 0; i < vit.size(); i++) {
                cout << fixed << " vitesse : " << vit.at(i) << endl;
            }
        }
        cout << "fin affichage des vitesses." << endl;
        cout << fixed << "mt : " << mt.coordonne << endl;
        cout << fixed << "mt tau : " << mt.tau << endl;
    }
    for (auto &vit:vits){
        Point3D temp = Point3D(mt.coordonne.x(),mt.coordonne.y(),mt.t);
        pair<Point3D,Vector2D> mnp = mnplusun(temp,*vit.begin());

        string indm;
        if(vit.size() == 1)
        {
            indm = "0";
        } else if(vit.size() == 2)
        {
            indm = "2";
        }
        else if(vit.size() == 3)
        {
            indm = "1";
        }else{
            cout << "Erreur sur vit.size() dans propage, vit.size = " << vit.size() << endl;
            exit(EXIT_FAILURE);
        }

        PointOpt mvt = PointOpt(mnp.first,mnp.second,indm, &compteurId,"Point de propage");
        if(affichage) {
            cout << "Point fait apres mnplusun" << endl;
            cout << "ind : " << ind << endl;
            cout << "mvt.tauperp : " << mvt.tau << endl;
            cout << fixed << " temp mvt : " << mvt.t << endl;
            cout << fixed << " mvt coordonnees: " << mvt.coordonne << endl;
            cout << "mvt.indice " << mvt.indice << endl;
            cout << "mvt.getid " << mvt.getId() << endl;
            cout << "mvt.ancetre " << mvt.ancetre << endl;
            cout << "mvt.suivant " << mvt.suivant << endl;
            cout << "mvt.precedent " << mvt.precedent << endl;
            cout << "mvt.vit.size : " << vit.size() << endl;
        }

        mvt.addVitesse(vit);


        int indics = supprimerCroisement(mvt, ind);

        bool test = testAreteBrulee(mvt);
        if(affichage) cout << "resultat testarete : " << test << endl;
        if (!test) {



            if(affichage) cout << "Valeur de indics apres supprimeCroisement : " << indics << endl;

            if (indics == 0 && compt == 1) {
                insere(mvt, ind, false);
                compt++;
                indic = 1;
            } else if (indics == 0 && compt > 1) {
                int indc = listff.size() - 1;
                insere(mvt, to_string(indc), true);
            } else if (indics == 1) {
                if (affichage) cout << "on insere pas mvt" << endl;
                compteurId--;/*temporaire*/
            } else if (indics == 2) {
                if (affichage) cout << "il y a un choc" << endl;
                compteurId--;/*temporaire*/
                return indic;
            } else {
                cout << "erreur 1 dans propage indic :" << indic << endl;
                exit(EXIT_FAILURE);
            }
        } else
        {
            compteurId--;
            cout << "On est sur une arete brulee." << endl;
        }
    }
    if(affichage) cout << "Fin de propage" << endl;
    return indic;
}

string AlgoOpt::numero (Point2D ij){
    return to_string(ij.x())+"_"+to_string(ij.y());
}

void AlgoOpt::supprime(string ind) {
    if(affichage) cout <<"Dans supprime ind : "<<ind<<endl;
    if (listeCellulesActivesSousContour.count(ind) != 0) {
        listeCellulesActivesSousContour.erase(ind);
        string minInd = listeCellulesActivesSousContour.begin()->first;
        for (auto &it:listeCellulesActivesSousContour) {
            if (it.second < listeCellulesActivesSousContour.at(minInd)) {
                minInd = it.first;
            }
        }
        aPropager = minInd;
        /*if(affichage) {
            cout << "listff.at((listff.at(ind).precedent)).suivant = (listff.at(ind).suivant);" << endl;
            cout << "(listff.at(ind).precedent) : " << (listff.at(ind).precedent) << endl;
            cout << "listff.at((listff.at(ind).precedent)).suivant : " << listff.at((listff.at(ind).precedent)).suivant
                 << endl;
            cout << "(listff.at(ind).suivant) : " << (listff.at(ind).suivant) << endl;
            cout << "listff.at((listff.at(ind).suivant)).precedent = (listff.at(ind).precedent);" << endl;
            cout << "(listff.at(ind).suivant) : " << (listff.at(ind).suivant) << endl;
            cout << "listff.at((listff.at(ind).suivant)).precedent : " << listff.at((listff.at(ind).suivant)).precedent
                 << endl;
            cout << "(listff.at(ind).precedent) : " << (listff.at(ind).precedent) << endl;
        }*/
        listff.at((listff.at(ind).precedent)).suivant = (listff.at(ind).suivant);
        listff.at((listff.at(ind).suivant)).precedent = (listff.at(ind).precedent);
        /*if(affichage) {
            cout << "listff.at((listff.at(ind).precedent)).suivant = (listff.at(ind).suivant);" << endl;
            cout << "(listff.at(ind).precedent) : " << (listff.at(ind).precedent) << endl;
            cout << "listff.at((listff.at(ind).precedent)).suivant : " << listff.at((listff.at(ind).precedent)).suivant
                 << endl;
            cout << "(listff.at(ind).suivant) : " << (listff.at(ind).suivant) << endl;
            cout << "listff.at((listff.at(ind).suivant)).precedent = (listff.at(ind).precedent);" << endl;
            cout << "(listff.at(ind).suivant) : " << (listff.at(ind).suivant) << endl;
            cout << "listff.at((listff.at(ind).suivant)).precedent : " << listff.at((listff.at(ind).suivant)).precedent
                 << endl;
            cout << "(listff.at(ind).precedent) : " << (listff.at(ind).precedent) << endl;
        }*/
    } else{
        if(affichage) cout << "Le point d'indice "<<ind<<" a deja ete supprime."<<endl;
    }
}
//bof
void AlgoOpt::insere(PointOpt point, string ind, bool indic){

    if(affichage) cout << "Indice : "<<point.indice<<", t : "<<point.t<<endl;

    int nmax = listff.size();
    if(!indic){
        /*if(affichage) {
            cout << "point.precedent : " << point.precedent << endl;
            cout << "point.suivant : " << point.suivant << endl;
            cout << "point.ancetre : " << point.ancetre << endl;
        }*/
        point.precedent=listff.at(ind).precedent;
        point.suivant=listff.at(ind).suivant;
        point.ancetre=ind;
        /*if(affichage) {
            cout << "point.precedent : " << point.precedent << endl;
            cout << "point.suivant : " << point.suivant << endl;
            cout << "point.ancetre : " << point.ancetre << endl;
            cout << "Listff.size : " << listff.size() << ", nmax : " << nmax << endl;
        }*/
        listff.insert(make_pair(to_string(nmax), point));
        /*if(affichage) {
            cout << "listff.at(ind).precedent : " << listff.at(ind).precedent << endl;
            cout << "listff.at(listff.at(ind).precedent).suivant : " << listff.at(listff.at(ind).precedent).suivant
                 << endl;
            cout << "listff.at(ind).suivant : " << listff.at(ind).suivant << endl;
            cout << "listff.at(listff.at(ind).suivant).precedent : " << listff.at(listff.at(ind).suivant).precedent
                 << endl;
        }*/
        listff.at(listff.at(ind).precedent).suivant = to_string(nmax);
        listff.at(listff.at(ind).suivant).precedent = to_string(nmax);
        /*if(affichage) {
            cout << "listff.at(ind).precedent : " << listff.at(ind).precedent << endl;
            cout << "listff.at(listff.at(ind).precedent).suivant : " << listff.at(listff.at(ind).precedent).suivant
                 << endl;
            cout << "listff.at(ind).suivant : " << listff.at(ind).suivant << endl;
            cout << "listff.at(listff.at(ind).suivant).precedent : " << listff.at(listff.at(ind).suivant).precedent
                 << endl;
        }*/
    }
    else{
        /*if(affichage) {
            cout << "point.precedent : " << point.precedent << endl;
            cout << "point.suivant : " << point.suivant << endl;
            cout << "point.ancetre : " << point.ancetre << endl;
        }*/
        point.precedent = ind;
        point.suivant=listff.at(ind).suivant;
        point.ancetre = listff.at(ind).ancetre;
        /*if(affichage) {
            cout << "point.precedent : " << point.precedent << endl;
            cout << "point.suivant : " << point.suivant << endl;
            cout << "point.ancetre : " << point.ancetre << endl;
            cout << "Listff.size : " << listff.size() << ", nmax : " << nmax << endl;
        }*/
        listff.insert(make_pair(to_string(nmax), point));
        /*if(affichage) {
            cout << "Listff.size : " << listff.size() << endl;
            cout << "listff.at(to_string(nmax)).suivant : " << listff.at(to_string(nmax)).suivant << endl;
            cout << "listff.at(listff.at(to_string(nmax)).suivant).precedent : "
                 << listff.at(listff.at(to_string(nmax)).suivant).precedent << endl;
            cout << "listff.at(ind).suivant : " << listff.at(ind).suivant << endl;
        }*/
        listff.at(ind).suivant = to_string(nmax);
        listff.at(listff.at(to_string(nmax)).suivant).precedent = to_string(nmax);
        /*if(affichage) {
            cout << "listff.at(to_string(nmax)).suivant : " << listff.at(to_string(nmax)).suivant << endl;
            cout << "listff.at(listff.at(to_string(nmax)).suivant).precedent : "
                 << listff.at(listff.at(to_string(nmax)).suivant).precedent << endl;
            cout << "listff.at(ind).suivant : " << listff.at(ind).suivant << endl;
        }*/
        if(point.ancetre != "0"){
            //cette condition permet au programme de s'executer mais n'est pas certaine et sert sans doute a traiter les chocs
            if(affichage) cout << "On passe dans la condition pas certaine."<<endl;
            listff.at(listff.at(ind).ancetre).precedent = to_string(nmax);
        }
    }
    Point2D ij;
    string num;
    vector<string> listNum;

    if( point.indice == "2"){
        ij = point.listeVitesse.begin()->begin()->coordonne();
        num = numero(ij);
        listNum.push_back(num);
        ij = ((point.listeVitesse.begin())->begin()+1)->coordonne();
        num = numero(ij);
        listNum.push_back(num);
    }
    else{
        ij = point.listeVitesse.begin()->begin()->coordonne();
        num = numero(ij);
        listNum.push_back(num);
    }
    map<string, int> test;

    for(auto &num2: listNum){
        if(count(listeCellulesBrulees.begin(),listeCellulesBrulees.end(),num2)==0){
            listeCellulesBrulees.push_back(num2);
            vector<string> temp;
            temp.push_back(point.getId());
            listePointsDansCellule.insert(make_pair(num2, temp));
        }
        else{
            listePointsDansCellule.at(num2).push_back(point.getId());
        }
    }

    if(!indic){
        listeCellulesActivesSousContour.erase(ind);
    }
    listeCellulesActivesSousContour.insert(make_pair(to_string(nmax),point.t));



    //!!! Les instructions suivantes ont un gros besoin d'optimisation
    string minInd = listeCellulesActivesSousContour.begin()->first;
    for(auto &it:listeCellulesActivesSousContour){
        if(it.second<listeCellulesActivesSousContour.at(minInd)){
            minInd=it.first;
        }
    }
    aPropager = minInd;
    if(affichage) {
        cout << "ind: " << ind << " nouveau point " << listff.at(to_string(listff.size() - 1)).getId() << " suivant "
             << listff.at(listff.at(to_string(listff.size() - 1)).suivant).getId() << " precedent "
             << listff.at(listff.at(to_string(listff.size() - 1)).precedent).getId() << endl;


        cout << "Fin de insere" << endl;
    }

}
//verif
void AlgoOpt::uneIteration(string ind){
    if(affichage) cout << "Debut UneIteration." << endl;
    PointOpt mvt = listff.at(ind);
    Point2D mm = mvt.coordonne;
    vector<vector<VitesseOpt>> vits = mvt.listeVitesse;
    bool indic;
    if(Vector2D(mm-Vector2D(round(mm.x()), round(mm.y()))).norm() <0){
        supprime(ind);
    }
    if(affichage) {
        cout << "mvt.indice : " << mvt.indice << endl;
        cout << "mvt.coordonne : " << mvt.coordonne << endl;
        cout << "mvt.tau : " << mvt.tau << endl;
        cout << "mvt.ancetre : " << mvt.ancetre << endl;
        cout << "ind : " << ind << endl;
    }

    supprimePointTropProche(ind,0.05);
    if(mvt.indice=="0"){
        pair<bool,vector<vector<VitesseOpt>>> tempp = vitesseNew1(vits,mvt.tau);
        vits=tempp.second;
        indic = tempp.first;

    }
    else if (mvt.indice=="1"){
        pair<bool,vector<vector<VitesseOpt>>> tempp = vitesseChocNew(vits,mvt.tau);
        vits = tempp.second;
        indic = tempp.first;
    }
    else if(mvt.indice=="2"){
        //pair<bool,vector<vector<VitesseOpt>>> tempp = vitesseSommetNew(vits,mvt.tau);
        //vits = tempp.second;
        //indic = tempp.first;/*TODO attention*/
        indic = false;
    }
    else if(mvt.indice=="3"){
        vits = mvt.listeVitesse;
        indic = true;
    }
    else{
        cout<<"erreur dans iteration: indice incorrect " <<mvt.indice << "." <<endl;
        exit (EXIT_FAILURE);
    }
    int indic2 = 0;
    if(affichage) cout <<"valeur de indic dans uneIteration : "<<indic << endl;
    if(!indic)
    {
        supprime(ind);
    }
    else
    {
        indic2 = propage(ind, vits);
        if(affichage) cout <<"valeur de indic2 dans uneIteration : " << indic2 <<endl;
    }

    if(indic2!=1){
        supprime(ind);
    }
    if(affichage) {
        cout << "Fin uneIteration, prochain pts Apropager : " << aPropager << endl;
        cout << endl;
        cout << endl;
        cout << endl;
        cout << endl;
    }

}

VitesseOpt AlgoOpt::vitesse0(Point2D p, Point2D s){
    return VitesseOpt();
}

vector<vector<VitesseOpt>> AlgoOpt::vitesseBord2(VitesseOpt vit, Point2D ijn, string cote){
    if(vit.indice()!=0){
        cout<<"VIT n'est pas une vitesse optimale dans vitessebord2"<<endl;
        exit (EXIT_FAILURE);
    }
    Point2D ij = vit.coordonne();
    Vector2D tauperp =ijn-ij;
    Vector2D tau = Vector2D (-tauperp.y(), tauperp.x());
    vector<vector<VitesseOpt>> vits;
    vector<vector<VitesseOpt>> vitb;
    vector<vector<VitesseOpt>> listv;
    vector<vector<VitesseOpt>> result;


    if(cote == "p"){
        tau = Vector2D(-tau.x(),-tau.y());
    }
    Vector2D taun = Vector2D(-tau.x(),-tau.y());
    Vector2D ta = vit.tau();
    double aux = det(tau,ta);
    int signe = -1;
    if(aux>=0){
        signe =1;
    }
    Vector2D vi = vit.vecteur();
    double auxv = tauperp.dot(vi);
    if(abs(aux>0)){
        VitesseOpt vitbn = vitesseMax(signe*tau,ijn);
        if(auxv < 0){
            VitesseOpt vitbo = vitesseMax(signe*tau, ij);
            Vector2D vtemp = (vitbn.vecteur()-vitbo.vecteur());
            double test = signe * vtemp.dot(tau);
            if(test>0){
                VitesseOpt v = vitesse(vitbn, ij);
                vector<VitesseOpt> temp1;
                temp1.push_back(vitbn);
                vits.push_back(temp1);
                vector<VitesseOpt> temp2;
                temp1.push_back(v);
                vits.push_back(temp2);

            }
            else if (test <0){
                VitesseOpt v = vitesse(vitbo,ijn);
                vector<VitesseOpt> vitb;
                vitb.push_back(vitbo);
                vitb.push_back(v);
            }
            else{
                VitesseOpt v = vitesse(vitbn,ij);
                vector<VitesseOpt> temp1;
                temp1.push_back(vitbn);
                vits.push_back(temp1);
                vector<VitesseOpt> temp2;
                temp1.push_back(v);
                vits.push_back(temp2);
            }

        }
        else{
            double auxx =det(vi,ta)/aux;
            Vector2D vita = auxx*tau;
            vector<VitesseOpt> temp;
            temp.push_back(vit);
            listv.push_back(temp);
            VitesseOpt vb = VitesseOpt(vita,ta,vit.coordonne(),listv.size());
            if(auxx<0){
                VitesseOpt vbn = vitesseMax(taun,ijn);
                double test = -(vbn.vecteur()-vita).dot(tau);
                if (test>=0){
                    VitesseOpt v = vitesse(vbn,ij);
                    vector<VitesseOpt> temp1;
                    temp1.push_back(vbn);
                    vits.push_back(temp1);
                    vector<VitesseOpt> temp2;
                    temp2.push_back(v);
                    vits.push_back(temp2);                }
                else{
                    VitesseOpt v = vitesse(vbn,ijn);
                    vector<VitesseOpt> temp2;
                    temp2.push_back(v);
                    vits.push_back(temp2);                }
            }
            else{
                double test = -(vitbn.vecteur()-vita).dot(tau);
                if(test>=0){
                    VitesseOpt v = vitesse(vitbn,ij);
                    vector<VitesseOpt> vitb;
                    vitb.push_back(vitbn);
                    vitb.push_back(v);
                    if(cote =="p"){
                        vector<vector<VitesseOpt>> vitc = vitesseChocOuPas(vit,v).second;
                        vector<vector<VitesseOpt>> listvn = vitesseRajoutes(ijn,tau,tauperp,kmax);
                        vector<VitesseOpt> temp1;
                        temp1.push_back(v);
                        listvn.insert(listvn.begin(), temp1);
                        vector<VitesseOpt> temp2;
                        temp2.push_back(vitbn);
                        listvn.insert(listvn.begin(),temp2);
                        vits.insert(vits.begin(),vitc.begin(),vitc.end());
                        vits.insert(vits.begin(),listvn.begin(),vitc.end());
                    }
                    else{
                        vector<vector<VitesseOpt>> vitc = vitesseChocOuPas(v,vit).second;
                        vector<vector<VitesseOpt>> listvn = vitesseRajoutes(ijn,tauperp,tau,kmax);
                        vector<VitesseOpt> temp1;
                        temp1.push_back(v);
                        listvn.insert(listvn.begin(),temp1);
                        vector<VitesseOpt> temp2;
                        temp2.push_back(vitbn);
                        listvn.insert(listvn.begin(),temp2);
                        vits.insert(vits.begin(),listvn.begin(),vitc.end());
                        vits.insert(vits.begin(),vitc.begin(),vitc.end());
                    }

                }
                else{
                    VitesseOpt v = vitesse(vb,ijn);
                    vector<VitesseOpt> vitb;
                    vitb.push_back(vb);
                    vitb.push_back(v);
                    vector<VitesseOpt> vits;
                    vits.push_back(v);
                }

            }
        }
    }
    else{
        VitesseOpt v = vitesse0(ij,ijn);
        vector<VitesseOpt> vits;
        vits.push_back(v);
        vector<VitesseOpt> vitb;

    }
//    result.push_back(vits);
//    result.push_back(vitb);
//    result.push_back(listv);

    return result;
}

void AlgoOpt::vitesseBord(vector<VitesseOpt> vitsp, vector<VitesseOpt> vitss){
//    vector<VitesseOpt> listv;
//    vector<VitesseOpt> vits;
//    VitesseOpt vp = *vitsp.end();
//    VitesseOpt vs = *vitss.begin();
//    if(vs.indice()!=0){
//        cout<<"erreur dans vitesseBord indice vs != 0"<<endl;
//        exit (EXIT_FAILURE);
//
//    }
//    if(vp.indice()!=0){
//        cout<<"erreur dans vitesseBord indice vp != 0"<<endl;
//        exit (EXIT_FAILURE);
//
//    }
//    Point2D ijp = vp.coordonne();
//    Point2D ijs = vs.coordonne();
//    Vector2D tauperp =ijs-ijp;
//    Vector2D tau = Vector2D(tauperp.y(), -tauperp.x());
//    Vector2D ta = vp.tau();
//    string typ;
//    string tys;
//    double aux =det(tau,ta);
//    Vector2D vi = vp.vecteur();
//    double auxp = vi.dot(tauperp);
//    double auxv = tauperp.dot(vi);
//    VitesseOpt vnbs = VitesseOpt();
//    VitesseOpt vnbp = VitesseOpt();
//
//    if(abs(aux)>0){
//        if(auxv<0){
//            VitesseOpt vnbp = vitesseMax(tau, ijp);
//            typ = "n";
//        }
//        else{
//            double auxx = det(vi,ta)/aux;
//            Vector2D vita = auxx*tau;
//            listv.push_back(vp);
//            VitesseOpt vnbp = VitesseOpt(vita,ta,vp.coordonne(),listv.size());
//            if(auxx<0){
//                VitesseOpt vps = vitesse(vnbp, ijs);
//                typ = "bbpp";
//            }
//            else{
//                typ = "n";
//            }
//        }
//    }
//    else{
//        cout<<"le front provenant de P est parallele au bord"<<endl;
//        typ="bbppp";
//        if(auxv<0){
//            VitesseOpt vps = vitesse0(ijs, ijp);
//
//        }
//        else{
//            VitesseOpt vps = vitesse0(ijp,ijs);
//        }
//    }
//
//    ta =vs.tau();
//    aux = det(tau,ta);
//    vi = vs.vecteur();
//    auxv = Vector2D(-tauperp.x(),-tauperp.y()).dot(vi);
//    if(abs(aux)>0){
//        if(auxv<0){
//            VitesseOpt vnbs = vitesseMax(tau,ijs);
//            tys = "n";
//        }
//        else{
//            double auxx = det(vi,ta)/aux;
//            Vector2D vita = auxx*tau;
//            listv.push_back(vs);
//            VitesseOpt vnbs = VitesseOpt(vita,ta,vs.coordonne(),listv.size());
//            if(auxx<0){
//                VitesseOpt vsp= vitesse(vnbs,ijp);
//                tys = "bbps";
//            }
//            else{
//                tys="n";
//            }
//        }
//    }
//    else{
//        cout<<"le front provenant de S est parallele au bord"<<endl;
//        tys = "bbpsp";
//        if(auxv < 0){
//            VitesseOpt vsp = vitesse0(ijp,ijs);
//        }
//        else{
//            VitesseOpt vsp = vitesse0(ijs,ijp);
//        }
//
//    }
//
//    if( typ == "n" && tys =="n"){
//        double test = vnbs.vecteur().norm() - vnbp.vecteur().norm();
//        if(test>0){
//            VitesseOpt vr = vitesse(vnbs,ijp);
//            vector<VitesseOpt> vnb;
//            vnb.push_back(vnbs);
//            vnb.push_back(vr);
//            pair<double,vector<VitesseOpt>> mypair = vitesseChocOuPas(vp,vr);
//            vector<VitesseOpt> vitc = mypair.second;
//            if(tau.dot(vitc.begin()->vecteur())>=0){
//                Vector2D vit = vitss.begin()->vecteur();
//                if(det(vnbs.vecteur(),vit)>0){
//                    vits.insert(vits.begin(),vitc.begin(),vitc.end());
//                    vits.insert(vits.begin(),vnb.begin(),vnb.end());
//                    vits.insert(vits.begin(),vitss.begin(),vitss.end());
//                }
//                else{
//                    vitss.erase(vitss.begin());
//                    vits.insert(vits.begin(),vitc.begin(),vitc.end());
//                    vits.insert(vits.begin(),vnb.begin(),vnb.end());
//                    vits.insert(vits.begin(),vitss.begin(),vitss.end());
//                }
//            }
//            else{
//                vitesseBord2();
//                vits=;
//            }
//        }
//        else if(test< 0){
//            VitesseOpt vr= vitesse(vnbp,ijs);
//            vector<VitesseOpt> vnb;
//            vnb.push_back(vnbp);
//            vnb.push_back(vr);
//            pair<double,vector<VitesseOpt>> mypair = vitesseChocOuPas(vr,vs);
//            vector<VitesseOpt> vitc = mypair.second;
//            if(tau.dot(vitc.begin()->vecteur())){
//                Vector2D vit = vitsp.end()->vecteur();
//                if(det(vnbp.vecteur(),vit)<0){
//                    vits.insert(vits.begin(),vitc.begin(),vitc.end());
//                    vits.insert(vits.begin(),vnb.begin(),vnb.end());
//                    vits.insert(vits.begin(),vitss.begin(),vitss.end());
//
//
//                }
//                else{
//                    vitessebord2;
//                    vits=;
//                }
//            }
//            else{
//                vitessebord2;
//                vits = vitrr;
//            }
//
//        }
//        else {
//            VitesseOpt vr = vitesse(vnbp,ijs);
//            vnb = ;
//            pair<double,vector<VitesseOpt>> mypair = vitesseChocOuPas(vr,vs);
//            vector<VitesseOpt> vitc = mypair.second;
//            if(tau.dot(vitc.begin()->vecteur())>=0){
//                Vector2D vit = vitsp.end()->vecteur();
//                if(det(vnbp.vecteur(),vit)<0){
//                    vits =;
//                }
//                else{
//                    vitsp.erase(vitsp.end());
//                    vits =;
//                }
//            }
//            else{
//                vitessebord2;
//                vits = vitrs;
//            }
//        }
//    }
//    else if(typ != "n" && tys =="n"){
//        vitc =vitesseChocOuPas(vs,vps).second;
//        vits = vitc;
//    }
//    else if(typ=="n" && tys != "n"){
//        vitc = vitesseChocOuPas(vsp, vp);
//        vits = vitc;
//    }
//    else if(typ == "bbppp" && tys=="bbpsp"){
//        Vector2D vtemp = (vsp-vps);
//        if(vtemp.norm()==0){
//            if((vsp.coordonne()-ijs)==0){
//                vits = vitss;
//            }
//            else{
//                vits = vitsp;
//            }
//        }
//        else{
//            cout<<"dans vitessebord, il faut supprimer le point"<<endl;
//        }
//    }
//    else{
//        cout<<"tys: "<<tys<<" typ: "<<typ<<endl;
//    }

}

//void AlgoOpt::chocDansCellule(PointOpt mvt){
//    PointOpt ancetre = *mvt.ancetre;
//    PointOpt *c = &listff.at(listeChoc.begin()->indic);
//    vector<vector<VitesseOpt>> vits = mvt.listeVitesse;
//    vector<vector<VitesseOpt>> vitsc = c->listeVitesse;
//    vector<vector<VitesseOpt>> vitc;
//    PointOpt precedent = PointOpt(&compteurId);
//    PointOpt suivant = PointOpt(&compteurId);
//    int indchoc;
//    Point3D mc = Point3D();
//    VitesseOpt v = *vits.begin()->begin();
//    VitesseOpt vc = *vitsc.begin()->begin();
//    Vector2D t = v.tau();
//    Vector2D tc = vc.tau();
//    double test = det(t,tc);
//
//    if(test<0){
//        if(vits.size()==3){
//            v = *(vits.begin()+1)->begin();
//        }
//        if (vitsc.size()==3){
//            vc = *(vitsc.begin()+1)->begin();
//        }
//        vitc = vitesseChocOuPas(v,vc).second;
//        PointOpt precedent = *ancetre.precedent;
//        PointOpt suivant = *c->suivant;
//
//    }
//    else{
//        PointOpt precedent = *c->precedent;
//        if(vits.size()==3){
//            v = *(vits.begin()+2)->begin();
//        }
//        if(vitsc.size()==3){
//            vc = *(vits.begin()+1)->begin();
//        }
//        vitc = vitesseChocOuPas(vc,v).second;
//        precedent = *(c->precedent);
//        PointOpt suivant = * (ancetre.suivant);
//
//    }
//    VitesseOpt vitemp = *vitc.begin()->begin();
//    Vector2D vetemp =  (vitemp.vecteur());
//    vetemp = Vector2D(-vetemp.x(), -vetemp.y());
//    vitemp.vecteur() = vetemp;
//    pair<Point3D, Vector2D> mnp = mnplusun(mc,vitemp);
//    bool test2 = false;
//    Point2D ijc = vitc.begin()->begin()->coordonne() + mnp.second;
//    string num = numero(ijc);
//    int nbre = listeCellulesActives.count(num);
//    if(nbre == 1){
//        vector<PointOpt *> tabcel = listeCellulesActives.at(num);
//        for (auto &ind:tabcel){
//            if(ind->indice=="1"){
//                Point2D mvttt = ind->coordonne;
//                Point2D ptemp = Point2D(mnp.first.x(), mnp.first.y());
//                Vector2D vtemp =(mvttt-ptemp);
//                if (vtemp.norm()==0){
//                    PointOpt * indchoc = ind;
//                    test2 = true;
//                }
//            }
//
//        }
//    }
//    if(!test2){
//        mnp.first.z()=mc.z() -(mnp.first.z()-mc.z());
//        mvt = PointOpt(mnp.first,Vector2D(mnp.second.x(),mnp.second.y()),"3", &compteurId);
//        mvt.precedent=&precedent;
//        mvt.suivant=&suivant;
//        insere(mvt,"p",false);
//        listff.end()->second.precedent= &precedent;
//        listff.end()->second.suivant= &suivant;
//        indchoc = listff.size();
//    }
//    suivant.precedent=&listff.end()->second;
//    precedent.suivant=&listff.end()->second;
//    supprimePlus("a changer", "a changer");
//
//}

vector<Point2D> AlgoOpt::raffine(vector<Point2D> listPoint, double crit){
    Point2D m = *listPoint.begin();
    vector<Point2D> result;
    listPoint.push_back(m);
    for(vector<Point2D>::const_iterator it = listPoint.begin()+1; it != listPoint.end(); ++it){
        Vector2D  vtemp =(*it)-m;
        
        double dist = vtemp.norm();
        if(dist > seuil){/*Attention val abs*/
            double test = floor((dist)/crit);
            for(int j = 1;j<test+1;j++){
                Point2D mk = m+(j-1)*Vector2D(*it-m)/(test+1);
                Point2D mki = Point2D(round(mk.x()),round(mk.y()));
                /*On ne veut pas tomber pile sur un cote d'une case */
                if(((mk.x()-mki.x()) * (mk.y()-mki.y()))==0){/*Si on tombe sur une ligne on decale d'un centieme*/
                    mk=mk+ 0.01*Vector2D(Vector2D(*it-m)/(test+1));
                }
                result.push_back(mk);
            }
            m = *it;
        }
    }
    return result;
}


void AlgoOpt::initListff(vector<Point2D> listPointE) {
    if(affichage) cout << "debut initlistff "<< endl;

    listePointsDansCellule =  map<string,vector<string>>();
    vector<PointOpt> listDef;


    vector<Point2D> listPoint = raffine(listPointE,0.5);

    for(vector<Point2D>::const_iterator it = listPoint.begin(); it != listPoint.end(); ++it){

        Point2D pTemp = Point2D(it->x(), it->y());
        if(pTemp.x() - floor(pTemp.x()) < seuil)
        {
            cout << "pTemp.x() : " << pTemp.x() << endl;
            pTemp.x() = pTemp.x() + 0.001;
            cout << "pTemp.x() : " << pTemp.x() << endl;
        }
        if(pTemp.y() - floor(pTemp.y()) < seuil)
        {
            cout << "pTemp.y() : " << pTemp.y() << endl;
            pTemp.y() = pTemp.y() + 0.001;
            cout << "pTemp.y() : " << pTemp.y() << endl;
        }
        PointOpt p = PointOpt(pTemp,0,Vector2D(0,0),"0", &compteurId,"Point de initListff");
        p.ancetre = p.getId();
        listDef.push_back(p);
    }


    for(int i =1; i<listDef.size();i++){
        listDef.at(i).precedent = listDef.at(i - 1).getId();
    }
    listDef.at(0).precedent = listDef.at(listDef.size() -1).getId();
    listDef.at(0).suivant = listDef.at(1).getId();

    for(int i =0; i<listDef.size()-1;i++){
        listDef.at(i).suivant = listDef.at(i+1).getId();
    }
    listDef.at(listDef.size()-1).precedent = listDef.at(listDef.size()-2).getId();
    listDef.at(listDef.size()-1).suivant = listDef.at(0).getId();

    vector<VitesseOpt> listvit;
    int i = 0;

    PointOpt chercher;
    for(auto &it: listDef){
        Point2D m =it.coordonne;
        chercher = findPts(listDef,it.precedent);
        Point2D mp = chercher.coordonne;
        chercher = findPts(listDef,it.suivant);
        Point2D ms = chercher.coordonne;
        Point2D ds = ms-m;
        Point2D dp = m-mp;
        if(Vector2D(ds).norm()!=0){
            Point2D ij = Point2D(floor(m.x()),floor(m.y()));
            VitesseOpt vp = vitessea0(0, dp, ij);
            VitesseOpt vs = vitessea0(0, ds, ij);
            vector<VitesseOpt> temp1;
            temp1.push_back(vp);
            vector<VitesseOpt> temp2;
            temp2.push_back(vs);
            it.listeVitesse.push_back(temp1);
            it.listeVitesse.push_back(temp2);
        }
    i++;
    }

    for(auto &it:listDef){
        vitesseChocOuPas(*(it.listeVitesse.begin())->begin(),*(it.listeVitesse.begin()+1)->begin()).second;
        it.listeVitesse=vitesseChocOuPas(*(it.listeVitesse.begin())->begin(),*(it.listeVitesse.begin()+1)->begin()).second;
        listff.insert(make_pair(it.getId(),it));
    }

    i = 0;
    for(i; i < listPoint.size(); i++) {

        bool indic = propage(to_string(i),listff.at(to_string(i)).listeVitesse);

        if(!indic){
            supprime(to_string(i));
        }

    }

    string ind0 = aPropager;
    string ind = listff.at(ind0).suivant;

    while(ind != ind0)
    {

        actualiseSommet(ind);
        ind = listff.at(ind).suivant;
    }

    actualiseSommet(ind0);

    ind = listff.at(ind0).suivant;
    while(ind != ind0) {
        initialiserSommetContourDepart(listff.at(listff.at(ind).ancetre));
        ind = listff.at(ind).suivant;
    }initialiserSommetContourDepart(listff.at(listff.at(ind0).ancetre));


  }

VitesseOpt AlgoOpt::vitesse(VitesseOpt vb, Point2D ij){

    EllipseOpt ellipse = carteEllipse[ij.x()][ij.y()];

    Vector2D vv = vb.vecteur();
    Point2D ijo = vb.coordonne();
    Vector2D tauperp =ij - ijo;
    if(tauperp.norm() != 0){
        cout<<"Erreur dans vitesse, tauperp.norm() != 0"<<endl;
        exit (EXIT_FAILURE);
    }
    Vector2D tau = Vector2D(tauperp.y(),-tauperp.x());
    double a0 = 1 / (vv.dot(tau));
    if(tauperp.norm()!=1){
        cout<<"Erreur dans vitesse, tauperp.norm() != 1"<<endl;
        exit (EXIT_FAILURE);
    }
    return vitessea0(-a0,Vector2D(-tau.x(),-tau.y()),ij);
}

pair<bool,vector<vector<VitesseOpt>>> AlgoOpt::vitesseChocNew(vector<vector<VitesseOpt>> vits, Vector2D tauperp){
    if(affichage) {
        cout << "Debut vitesseChocNew" << endl;
        cout << "Taille : " << vits.size() << endl;
        cout << "Taille : " << vits.begin()->size() << endl;
    }
    if(vits.begin()->size()!=3){
        cout << "Taille : " << vits.size() << endl;
        cout << "Taille : " << vits.begin()->size() << endl;
        cout<<"erreur 1 dans vitesseCHocNew"<<endl;
        exit (EXIT_FAILURE);
    }
    else if(tauperp.x()*tauperp.y() != 0){
        cout<<"erreur 2 dans vitesseCHocNew"<<endl;
        exit (EXIT_FAILURE);
    }
    VitesseOpt vc = *vits.begin()->begin();
    Point2D ij = vc.coordonne();
    Point2D ijn = ij+tauperp;
    VitesseOpt vp = *(vits.begin()->begin()+1);
    VitesseOpt vs = *(vits.begin()->begin()+2);
    Vector2D tau = Vector2D(-tauperp.y(), tauperp.x());
    double a0s = azero(tau, vs);
    double a0p = azero(tau, vp);
    VitesseOpt vns = vitessea0(a0s,tau,ijn);
    VitesseOpt vnp = vitessea0(a0p,tau,ijn);
    if(vns.indice()==0 && vnp.indice()==0){
        pair<double,vector<vector<VitesseOpt>>> chocOupas = vitesseChocOuPas(vnp, vns);
        if(chocOupas.first!=1){
            return make_pair(false, chocOupas.second);
        }
        else{
            return make_pair(true, chocOupas.second);

        }
    }
    else{
        return make_pair(false,vits);
    }
}

pair<bool,vector<vector<VitesseOpt>>> AlgoOpt::vitesseNew1(vector<vector<VitesseOpt>> vits, Vector2D tauperp){
    if(affichage) cout << "Debut vitesseNew1" << endl;
    VitesseOpt vit = *vits.begin()->begin();
    Point2D ij = vit.coordonne();
    if(affichage) cout << "ij : "<<ij << endl;
    Vector2D tau = Vector2D(-tauperp.y(), tauperp.x());
    if(affichage) cout << "tauperp : "<<tauperp << endl;
    if(affichage) cout << "tau : "<<tau << endl;
    Point2D ijn = ij+tauperp;
    if(affichage) cout << "ijn : "<<ijn << endl;
    double a0 = azero(tau, vit);
    if(affichage) cout << "a0 : "<<a0 << endl;

    VitesseOpt v = vitessea0(a0,tau,ijn);
    if(affichage) {
        if (v.indice() == 10) {
            v.affiche();
        }
    }

    vector<vector<VitesseOpt>> result;
    if(v.indice()==0){
        vector<VitesseOpt> temp;
        temp.push_back(v);
        result.push_back(temp);
        if(affichage) cout << "Sortie de vitesseNew1 via v.indice == 0 "<<endl;
        return make_pair(true, result);
    }
    else{
        if(a0>seuil) {
            VitesseOpt vb = vitesseMax(tau, ijn);
            double b0 = 1 / vb.vecteur().norm();
            Vector2D taun = Vector2D(-tau.x(), -tau.y());
            VitesseOpt vr = vitessea0(-b0, taun, ij);
            pair<double, vector<vector<VitesseOpt>>> chocOupas = vitesseChocOuPas(vr, vit);
            vector<vector<VitesseOpt>> listvn = vitesseRajoutes(ijn, tauperp, tau, 20);
            bool tailleValide;
            if (listvn.size() >= 3) {
                listvn.erase(listvn.begin());
                listvn.erase(listvn.end());
                tailleValide = true;
            } else {
                tailleValide = false;
            }
            vector<VitesseOpt> temp1;
            temp1.push_back(vb);
            temp1.push_back(vr);
            vector<vector<VitesseOpt>> temp2;
            temp2.push_back(temp1);
            if (tailleValide) {
                listvn.insert(listvn.end(), temp2.begin(), temp2.end());
                listvn.insert(listvn.end(), chocOupas.second.begin(), chocOupas.second.end());
                if(affichage) cout << "fin vitessenew1 > seuil" << endl;
                return make_pair(true,listvn);
            }else{
                temp2.insert(temp2.end(),chocOupas.second.begin(),chocOupas.second.end());
                if(affichage) cout << "fin vitessenew1 > seuil" << endl;
                return make_pair(true,temp2);
            }

        }
        else if(a0<-seuil){
            Vector2D taun = Vector2D(-tau.x(),-tau.y());
            VitesseOpt vb = vitesseMax(taun,ijn);
            double b0 = 1/vb.vecteur().norm();
            VitesseOpt vr = vitessea0(b0, taun, ij);
            pair<double, vector<vector<VitesseOpt>>> chocOupas  = vitesseChocOuPas(vit,vr);
            if(affichage) cout << "double de chocOuPas : " << chocOupas.first << " Taille second elt : " << chocOupas.second.at(0).size() << endl;
            if(affichage) cout << "taun : " << taun << ", tauperp : "<<tauperp << endl;
            vector<vector<VitesseOpt>> listvn = vitesseRajoutes(ijn,taun,tauperp,20);
            if(affichage) cout << "taille listvn : " << listvn.size() << endl;
            bool tailleValide;
            if(listvn.size() >= 3) {
                listvn.erase(listvn.begin());
                listvn.erase(listvn.end());
                tailleValide = true;
            }else{
                tailleValide = false;
            }
            if(affichage) cout << "taille listvn : " << listvn.size() << endl;
            vector<VitesseOpt> temp1;
            temp1.push_back(vb);
            temp1.push_back(vr);
            vector<vector<VitesseOpt>> temp2;
            temp2.push_back(temp1);
            if(affichage) cout << "Taille chocOupas : " << chocOupas.second.size() << endl;
            chocOupas.second.insert(chocOupas.second.end(),temp2.begin(),temp2.end());
            if(affichage) cout << "Taille chocOupas : " << chocOupas.second.size() << endl;
            if(tailleValide) {
                chocOupas.second.insert(chocOupas.second.end(), listvn.begin(), listvn.end());
            }
            if(affichage) cout << "Taille chocOupas : " << chocOupas.second.size() << endl;
            if(affichage) cout << "fin vitessenew1 < seuil, taille de la liste de retour : "<< chocOupas.second.size()<< endl;
            return make_pair(true,chocOupas.second);
        }
        else{
            if(affichage) cout<<"erreur dans vitesseNew1 : a0=0"<<endl;
            exit (EXIT_FAILURE);
        }
    }
}

pair<bool,vector<vector<VitesseOpt>>> AlgoOpt::vitesseSommetNew(vector<vector<VitesseOpt>> vits, Vector2D tauperp){
    if(vits.size()!=2){
        cout<<"erreur 1 dans vitessseSommetNew"<<endl;
        exit (EXIT_FAILURE);
    }
    Vector2D vb = vits.begin()->begin()->vecteur();
    Point2D ijb = vits.begin()->begin()->coordonne();
    Vector2D vr = (vits.begin()+1)->begin()->vecteur();
    Point2D ijr = (vits.begin()+1)->begin()->coordonne();
    Vector2D tau = ijb - ijr;
    Point2D ijpo;
    Point2D ijso;
    VitesseOpt vso;
    VitesseOpt vpo;

    if(det(vb,tau)<=0){
        tau = Vector2D(-tau.x(), -tau.y());
        vpo = *vits.begin()->begin();
        vso = *(vits.begin()+1)->begin();
        ijpo = ijb;
        ijso = ijr;
    }
    else{
        vso = *vits.begin()->begin();
        vpo = *(vits.begin()+1)->begin();
        ijpo = ijr;
        ijso = ijb;
    }
    tauperp = Vector2D(tau.y(),-tau.x());
    Point2D ijpn=ijpo+tauperp;
    Point2D ijsn=ijso+tauperp;

    //A finir
}

Point2D AlgoOpt::coordinateToIndex(const Point2D &p) {
    return {floor((p.x()-xmin)/tailleCellule), floor((p.y()-ymin)/tailleCellule)};
}

Point2D AlgoOpt::coordinateToCoordinateCalc(const Point2D &p) {
    return {(p.x()-xmin)/tailleCellule, (p.y()-ymin)/tailleCellule};
}
Point2D AlgoOpt::indextoCoordinate(const Point2D &p) const {
    return {tailleCellule*p.x() + xmin, tailleCellule*p.y() + ymin};
}

void AlgoOpt::initialiserEllipse(json j, ofstream& sortie, int largeur, int hauteur, int water, int temp, double tempsPropa){

    json jsonVent = nullptr;

    cout<<"init!"<<endl;
    int largeurCarteEllipse = largeur/tailleCellule;
    int hauteurCarteEllipse = hauteur/tailleCellule;
    carteEllipse = vector<vector<EllipseOpt >> (largeurCarteEllipse);

    for(long i = 0 ; i <  largeurCarteEllipse; i++){
        try
        {
            carteEllipse.at(i).resize(hauteurCarteEllipse);
        }
        catch (std::bad_alloc& ba)
        {
            std::cout << "erreur init map" << std::endl;
            std::cout<< " i = "<<i<<std::endl;
        }

    }
    double xmintemp = -1;
    double ymintemp = -1;

    for (const auto &it : j) {

        double tempx  = it["x"];
        double tempy  = it["y"];
        if (tempx < xmintemp || xmintemp == -1)
            xmintemp = tempx;
        if (tempy < ymintemp || ymintemp == -1)
            ymintemp = tempy;
    }


    xmin = xmintemp;
    ymin = ymintemp;


    for (const auto &it : j) {

        double slope = it["slope"];
        double aspect = it["aspect"];

        json jSlopeVector = it["slope_vector"];
        Vector3D windSlope = Vector3D(it["windSlope"][0],it["windSlope"][1],it["windSlope"][2]);
        Point2D mapCoordinate = Point2D(it["x"], it["y"]);
        Point2D index = coordinateToIndex(Point2D(mapCoordinate.x(), mapCoordinate.y()));


        float lb = 1 + 0.0012 * pow((2.237 * windSlope.norm()), 2.154);
        float epsilon = sqrt(1 - (1 / (lb*lb)));

        double rateOfSpread = 180 * exp(0.06 * temp) * tanh(((100-water)*1.0)/150.0)
                * (1+2*(0.8483+tanh(((windSlope.norm()*3.6)/30.0)-1.25)));

        double windslopenorme = windSlope.norm();
        double ratio = rateOfSpread / windslopenorme;

        Vector3D rateOfSpreadVect = Vector3D((windSlope.x()*ratio)/tailleCellule,(windSlope.y()*ratio)/tailleCellule,(windSlope.z()*ratio)/tailleCellule);

        EllipseOpt e = EllipseOpt(mapCoordinate, rateOfSpreadVect,epsilon,aspect,slope);

        jsonVent.push_back({{"cellule_coords", mapCoordinate.toPair()},
                            {"index", index.toPair()},
                            {"igni_point_coords", e.coordonne().toPair()},
                            {"great_axis_dir", e.vecteur().toPair()},
                            {"great_axis", e.a()},
                            {"small_axis", e.b()}});


        carteEllipse[(int)index.x()].at((int)index.y()) = e;
    }
    sortie << jsonVent << endl;
}

void AlgoOpt:: affiche(std::vector<vector<VitesseOpt>> v){

    int i =0;
    for(auto &it:v){
        std::cout<<"###### Vitesse: "<<i<<" ######"<<std::endl;
        it.begin()->affiche();
        i++;
    }
}

void AlgoOpt:: affiche(std::pair<double,std::vector<vector<VitesseOpt>>> v){
    std::cout<<"List Indic: "<<v.first<<std::endl;
    affiche(v.second);

}

void AlgoOpt:: affiche(std::pair<Point3D,Vector2D> v){
    std::cout<<"Point: "<<v.first;
    std::cout<<"Vecteur: "<<v.second<<std::endl;

}

PointOpt
AlgoOpt::findPts(vector<PointOpt> listDef, string indice)
{
    for(auto & i : listDef)
    {

        if(i.getId() == indice)
        {
            return i;
        }

    }
    return nullptr;
}

void
AlgoOpt::actualiseSommet(string ind)
{
    if(affichage) cout << "Dans actualise sommet"<<endl;
    /*ind est l'indice d'un point*/
    /*Fonction qui actualise les temps d'ignition des sommets dans la prochaine cellule avec des vitessesmax issues de ind*/
    /*Recuperation ds informations du point*/
    PointOpt mt = listff.at(ind);//Le point
    Point2D ijo = (listff.at(ind).listeVitesse.begin())->begin()->coordonne();
    /*ijo = IJorigin --> Coordonnees de la case origin du point*/
    Point2D ijn = ijo + mt.tau;
    /*ijn = IJnew --> Coordonnees de la case arrive du point*/
    Point2D m = mt.coordonne;//Les coordonnees du point
    double t = mt.t;//Le temps d'ignition du point
    Vector2D dm  = m - Point2D(round(m.x()),round(m.y()));//dm sert a indiquer si le point est un sommet ou non
    bool indic;

    if(dm.norm() <= seuil)//mt est un sommet
    {
        Point2D ij = Point2D(round(mt.coordonne.x()),round(mt.coordonne.y()));
        double tnew = mt.t;//nouveau temps
        string num = numero(ij);

        /*Debut suspect*/
        if(mt.suivant == "5")//Erreur a corriger (pts suivant)
        {
            (listff.at(ind).listeVitesse.begin())->begin()->setIndice(3);
            indic = false;

        }
        /*Fin suspect*/
        else{
            indic = true;/*Gestion des sommets plus tard*/
        }

        if(listeSommets.count(num) == 0)
        {
            //Si le sommet n'est pas present on l'insere
            listeSommets.insert(make_pair(num,SommetCase(num,tnew,ind,indic,0,0)));

        } else
        {
            double tOld = listeSommets.at(num).tIgnitionSommet;
            if(tnew<=tOld)
            {
                /*Si le sommet est deja present et que le tnew est inferieur au told, on ecrase la valeur avec un nouveau sommet*/
                listeSommets.erase(num);/*A confirmer*/
                listeSommets.insert(make_pair(num,SommetCase(num,tnew,ind,indic,0,0)));
            }

        }
    } else
    {
        vector<Point2D> listDIJ;/*Liste des coordonnees*/
        listDIJ.emplace_back(Point2D(0,0));
        listDIJ.emplace_back(Point2D(1,0));
        listDIJ.emplace_back(Point2D(1,1));
        listDIJ.emplace_back(Point2D(0,1));

        /*On va parcourir les quatres sommets d'une case*/
        for(Point2D dij : listDIJ)
        {
            Point2D ij = ijn + dij;//Sommet qu'on est en train de parcourir
            indic = false;
            Vector2D dir = ij - m;//Vecteur entre l'endroit d'intersection entre les cases et le sommet actuel
            VitesseOpt vit = vitesseMax(dir,ijn);
            double dt = dir.norm() / vit.vecteur().norm();
            double tnew = t+dt;//Temps d'ignition du sommet ij par le point mt
            string num = numero(ij);//Indice de la cellule

            if(listeSommets.count(num) == 0)//Si le numero ne fait pas deja parti de la list
            {
                //On insere
                listeSommets.insert(make_pair(num,SommetCase(num,tnew,ind,indic,dij)));

            } else
            {
                double tMaxOld = listeSommets.at(num).tIgnitionSommet;//SommetCase.t a l'endroit ou se trouve deja le num (loc)
                bool indicOld = listeSommets.at(num).indic;//Pareil mais pour l'indic
                if(tnew <= tMaxOld && indicOld != 1)//Si le nouveau temps est plus tot on modif
                {

                    listeSommets.erase(num);
                    listeSommets.insert(make_pair(num,SommetCase(num,tnew,ind,indic,dij)));

                }
                else if(tnew <= tMaxOld && indicOld == 1)
                {

                    listeSommets.erase(num);
                    listeSommets.insert(make_pair(num,SommetCase(num,tnew,ind,indic,dij)));

                }

                //Si le tnew est plus grand on ne fait rien

            }
        }
    }
}

void
AlgoOpt::rajoutePoint(string indice)
{
    if(affichage) cout << "Dans rajoute Point indice : "<<indice<<endl;
    double tmin = listff.at(aPropager).t;

    PointOpt mt = listff.at(indice);
    Point2D m = mt.coordonne;
    double t = mt.t;
    vector<vector<VitesseOpt>> vitm = mt.listeVitesse;
    vector<VitesseOpt> vit = vitm.at(0);
    string inds = mt.suivant;
    PointOpt mts = listff.at(inds);

    if((mts.ancetre != mt.ancetre) || ((atoi(mts.indice.c_str()) + atoi(mt.indice.c_str())) != 3))
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
        if(ts >= t)
        {
            ms = ms + (t - ts)*vs;
            dm = ms - m;
            mm = m;
        } else
        {
            mm = m +(ts-t)*v;
            ms = mts.coordonne;
            dm = ms - mm;
        }
        double testS;

        Vector2D ijMoinsijs = ij - ijs;
        if(ijMoinsijs.norm() != 0)
        {
            testS = dm.norm();
        } else
        {
            testS = (abs(v.determinant(dm)) / v.norm()) + (abs(vs.determinant(dm)) / vs.norm());
        }
        if(testS >= crit)
        {
            mm = m + (tmin - t)*v;
            ms = mts.coordonne + (tmin - ts)*vs;
            Point2D mis = (ms+mm)/2;
            ij = Point2D(round(mis.x()),round(mis.y()));
            if((mis.x()-ij.x())*(mis.y() - ij.y()) == 0)
            {
                if(affichage) cout << "MIS est sur le bord" << endl;
                mis =((ms-mm)*2)/3;
            }

            ij = Point2D(floor(mis.x()),floor(mis.y()));

            Vector2D dir = ms - mm;
            double testSS = dir.norm();

            if(testSS >= (crit/5))
            {
                VitesseOpt vitmis = vitessea0(0,dir,ij);
                Point3D misAvecT = Point3D(mis.x(),mis.y(),tmin);
                pair<Point3D, Vector2D> mnp = mnplusun(misAvecT,vitmis);
                Point2D postmnplusun = Point2D(mnp.first.x(),mnp.first.y());
                PointOpt mvt = PointOpt(postmnplusun,mnp.first.z(),mnp.second,"0","0","0","0",vitmis,&compteurId,"Point de rajoutePoint");
                bool test = testAreteBrulee(mvt);
                if(affichage) cout << "resultat testAreteBrulee : " << test << endl;
                if(!test)
                {
                    int indics = supprimeCroisementRaj(mvt,tmin,indice);

                    if(indics == 0)
                    {
                        insere(mvt,indice,true);
                        if(affichage) cout << "On a rajoute "<< listff.size() - 1 << endl;
                    }else{
                        if(affichage) cout << "indics !=0 bla" <<endl;
                        compteurId --;
                    }

                }else{
                    compteurId --;
                }
            }
        }

    }


}

void AlgoOpt::rajoutePoints(string indice) {
    PointOpt mt = listff.at(indice);
    rajoutePoint(indice);
    string indp = mt.precedent;
    rajoutePoint(indp);

}

bool AlgoOpt::testAreteBrulee(PointOpt point) {
    if(affichage) cout << "Dans testAreteBrulee" << endl;

    if(listeSommets.empty() || initialisationListff)
        return false;

    PointOpt m = point;
    Point2D ij = Point2D(floor(m.coordonne.x()), floor(m.coordonne.y()));
    string num = numero(ij);
    double tmin = listff.at(aPropager).t;

    if(affichage) cout << "listeSommets.count(num) : "<< listeSommets.count(num)  <<endl;
    if(affichage) cout << "num : "<< num  <<endl;
    if(listeSommets.count(num) == 0) {
        return false;
    } else {
        Vector2D dm = m.coordonne - ij;//Distance entre le point mvt et le sommet ij de la cellule
        if (dm.norm() > seuil) {
            Point2D ij1 = ij + dm/dm.norm();// / dm.norm();//Ij1 correspond au deuxieme sommet de l'arete sur laquelle le point mvt est
            ij1 = Point2D(round(ij1.x()), round(ij1.y()));
            string num1 = numero(ij1);

            if(affichage) cout << "listeSommets.count(num1) : "<< listeSommets.count(num1)  <<endl;
            if (listeSommets.count(num1) == 0) {//Ici le deuxieme sommet n'a pas encore brule
                return false;
            } else {//Ici les deux sommets sont ou vont brules
                string ind = listeSommets.at(num).indicePointAllume;//On va chercher l'indice du point dans la cellule
                string ind1 = listeSommets.at(num1).indicePointAllume;
                double tmax = listeSommets.at(num).tIgnitionSommet;
                double tmax1 = listeSommets.at(num1).tIgnitionSommet;
                double tm = m.t;

                //if(affichage) cout <<"tmin : "<< tmin << ", tmax : "<<tmax << ", tmax1 : "<<tmax1 <<", tm : "<<tm<<endl;
                if (tmax <= tmin && tmax1 <= tmin) {//Ici on test si l'insertion va se faire sur une arete brule
                    return true;
                }

                Vector2D dij = listeSommets.at(num).pts;
                Vector2D dij1 = listeSommets.at(num1).pts;
                Vector2D ij1moinsij = ij1 - ij;

                //if(affichage) cout <<"dij.x() : "<< dij.x() << endl;
                //if(affichage) cout <<"dij.y() : "<< dij.y() << endl;
                //if(affichage) cout <<"(dij1.x() + dij1.y()) : "<< (dij1.x() + dij1.y()) << endl;
                //if(affichage) cout <<"dij1.determinant(ij1moinsij) : "<< dij1.determinant(ij1moinsij) << endl;
                //if(affichage) cout <<"(dij.x() == 0 && dij.y() == 0 && (dij1.x() + dij1.y()) == 1 && dij1.determinant(ij1moinsij) == 0) : "<< (dij.x() == 0 && dij.y() == 0 && (dij1.x() + dij1.y()) == 1 && dij1.determinant(ij1moinsij) == 0) << endl;
                if (dij.x() == 0 && dij.y() == 0 && (dij1.x() + dij1.y()) == 1 && dij1.determinant(ij1moinsij) == 0) {
                    //if(affichage) cout <<"tm : "<< tm<<", tmax : "<<tmax<<", tmax1 : "<<tmax1 << endl;
                    //if(affichage) cout <<"(tmax - tm) * (tm - tmax1) : "<< (tmax - tm) * (tm - tmax1)<< endl;
                    if ((tmax - tm) * (tm - tmax1) <= 0 && tm >= tmax) {
                        return true;
                    } else {
                        return false;
                    }
                } else {
                    return false;
                }
            }
        } else {
            string ind = listeSommets.at(num).indicePointAllume;
            double tmax = listeSommets.at(num).tIgnitionSommet;
            double tm = m.t;

            if (tm > tmax) {
                if(affichage) cout << listff.at(ind).getId() << " déjà brulé par " << ij << " sommet " << m.getId() << " m " << endl;
                return true;
            } else {
                return false;
            }
        }
    }
}


void AlgoOpt::sauvegarde_liste_vitesses(ofstream& contour_final_detail) const {
    json liste_points = nullptr;
    json vitesses = nullptr;

    Point2D convertedCoord;
    PointOpt premier = listff.at(aPropager);
    double tempsMin = premier.t;
    string suiv = aPropager;
    PointOpt point = listff.at(suiv);

    int nb_points_parc = 0;

    while(nb_points_parc < listff.size() && (nb_points_parc == 0 || listff.at(suiv).getId() != premier.getId() ))

    {
        convertedCoord = point.coordonne;
        for(auto& row: point.listeVitesse){
            for(auto& vitesse: row){

                convertedCoord = convertedCoord + (tempsMin - point.t) * vitesse.vecteur();
                //cout << fixed << convertedCoord << endl;
                vitesses.push_back({
                                           {"indice", vitesse.indice()},
                                           {"id", suiv},
                                           {"coordonnees", indextoCoordinate(convertedCoord).toPair()},
                                           {"coordonnees_calcul", point.coordonne.toPair()},
                                           {"tau", vitesse.tau().toPair()},
                                           {"vecteur", vitesse.vecteur().toPair()}
                                   });
            }
            liste_points.push_back({
                                           {"indice", point.indice},
                                           {"id",point.getId()},
                                           {"coordonnees", indextoCoordinate(point.coordonne).toPair()},
                                           {"coordonnees_calcul", point.coordonne.toPair()},
                                           {"temps", point.t},
                                           {"tau", point.tau.toPair()},
                                           {"ancetre", point.ancetre},
                                           {"precedent", point.precedent},
                                           {"suivant", point.suivant},
                                           {"creation",point.creation},
                                           {"vitesses", vitesses}
                                   });
            vitesses = nullptr;
        }
        suiv = point.suivant;
        point = listff.at(suiv);
        nb_points_parc++;
    }

    if (contour_final_detail) {
        contour_final_detail << liste_points;
        cout << "#######################################################" << endl;
        cout << "La liste de points a ete sauvegardee." << endl;
    } else {
        cout << "#######################################################" << endl;
        cout << "[ERROR] La liste de points n'a pas pu etre sauvegardee." << endl;
    }
}



int AlgoOpt::supprimeCroisementRaj(PointOpt mvt, double tmin, string ind) {
    if(affichage) cout << "Debut supprimeCroisementRaj"<<endl;
    listeChoc = vector<Choc>();

    int indic = 0;
    Point2D mm = mvt.coordonne;
    double tp = mvt.t;
    vector<VitesseOpt> vit = *mvt.listeVitesse.begin();
    Vector2D v = (vit.begin())->vecteur();
    vector<string> indasuppr;
    Point2D ij = vit.begin()->coordonne();
    string num = numero(ij);

    auto iterator = find(listeCellulesBrulees.begin(),listeCellulesBrulees.end(),num);

    if(iterator != listeCellulesBrulees.end())
    {
        for(int i = 0; i < listePointsDansCellule.at(num).size(); i++)
        {

            PointOpt mtc = listff.at(listePointsDansCellule.at(num).at(i));
            Point2D mc = mtc.coordonne;
            vector<VitesseOpt> vitc = *mtc.listeVitesse.begin();
            Vector2D vc = vitc.begin()->vecteur();
            double tc = mtc.t;
            Vector2D dm = mm - mc;
            double aux = vc.determinant(v);
            if(abs(aux) > seuil)
            {
                double tesT = tmin + (dm.determinant(vc)/aux);
                if(tp >= tesT && tesT>=tmin)
                {
                    double tesTC = tc + (dm.determinant(v)/aux);
                    Point2D mcr = mc+((tesTC-tc)*vc);
                    Vector2D mcrMoinsMM = mcr - mm;
                    if(mcrMoinsMM.norm() >= seuil && tesTC>=0)
                    {
                        if(tesT+seuil < tesTC && tc >= tmin)
                        {
                            indasuppr.push_back(mtc.getId());
                        }else if(tesT> tesTC+seuil)
                        {
                            return 1;
                        }else if(abs(tesTC-tesT)<=seuil)
                        {
                            return 1;
                        }
                    }
                }
            }
        }
        std::vector<string>::iterator ite;
        ite = unique(indasuppr.begin(),indasuppr.end());
        indasuppr.resize( std::distance(indasuppr.begin(),ite) );

        if(!indasuppr.empty())
        {
            for(auto &it:indasuppr)
            {
                if(it != ind) {
                    if(affichage) cout << "Suppression de : " << it << endl;
                    supprime(it);
                    for (unsigned int i = 0; i < listeChoc.size(); i++) {
                        if (listeChoc[i].indic == it) {
                            listeChoc.erase(listeChoc.begin() + i);
                        }
                    }
                }else{/*TODO chercher une autre maniere de faire car ça peut provoquer des problemes*/
                    if(affichage) cout << "On ne supprime pas le point d'indice : "<<ind<<" car c'est le point qui a ete donne en parametre" << endl;
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
    Vector2D mds = ms + (t - ts)*vs.vecteur() - m;
    Vector2D mdp = mp + (t - tp)*vp.vecteur() - m;

    if(mds.norm() < crit)
    {
        if (affichage) cout << "mds.norm() < crit, on va supprimer : "<<inds<<endl;
        cout << "mds.norm() < crit, on va supprimer : "<<inds<<endl;
        supprime(inds);
    }
    if(mdp.norm() < crit)
    {
        if (affichage) cout << "mdp.norm() < crit, on va supprimer : "<<indp<<endl;
        cout << "mdp.norm() < crit, on va supprimer : "<<indp<<endl;
        supprime(indp);
    }
}


void AlgoOpt::sauvegarder_liste_sommets(ofstream &etat_final_liste_sommets) const {
    cout <<"Nombre de sommets dans la liste : "<<listeSommets.size()<<endl;

    json liste_sommets_a_sauvegarder = nullptr;
    pair<double, double> coordonnees;
    if (etat_final_liste_sommets) {
        for (auto const& sommet : listeSommets) {
            coordonnees = numToCoordinate(sommet.second.numSommet);
            liste_sommets_a_sauvegarder.push_back({{"coordonnees", indextoCoordinate(
                    Point2D(coordonnees.first, coordonnees.second))
                                                                           .toPair()},
                                                   {"coordonnees_calcul", Point2D(coordonnees.first, coordonnees.second).toPair()},
                                                   {"indice_allume",sommet.second.indicePointAllume},
                                                   {"t_ignition", sommet.second.tIgnitionSommet}});
        }
        etat_final_liste_sommets << liste_sommets_a_sauvegarder;
        cout << "#######################################################" << endl;
        cout << "L'etat de la liste de sommets a ete sauvegarde." << endl;
    } else {
        cout << "#######################################################" << endl;
        cout << "[ERROR] L'etat de la liste de sommets n'a pas pu etre sauvegarde." << endl;
    }
}

pair<double, double> AlgoOpt::numToCoordinate(const string& num) const {

    string buffer;
    double x, y;
    for(char it : num) {
        if (it == NUM_DELIMITER) {
            x = stod(buffer);
            buffer = "";
        } else {
            buffer += it;
        }
    }
    y = stod(buffer);
    return make_pair(x, y);
}


void AlgoOpt::initialiserSommetContourDepart(PointOpt mvt) {


    vector<Point2D> listDIJ;/*Liste des coordonnees*/
    listDIJ.emplace_back(Point2D(0, 0));
    listDIJ.emplace_back(Point2D(1, 0));
    listDIJ.emplace_back(Point2D(1, 1));
    listDIJ.emplace_back(Point2D(0, 1));

    Point2D m = mvt.coordonne;
    string ind = mvt.getId();
    Point2D ijo = Point2D(floor(m.x()), floor(m.y()));

    /*On va parcourir les quatres sommets d'une case*/
    for (Point2D dij : listDIJ) {
        Point2D ij = ijo + dij;//Sommet qu'on est en train de parcourir
        string num = numero(ij);//Indice de la cellule

        if (listeSommets.count(num) == 0)//Si le numero ne fait pas deja parti de la liste
        {
            //On insere
            listeSommets.insert(make_pair(num, SommetCase(num, 0, ind, false, dij)));/*indice??*/
        }

    }
}






