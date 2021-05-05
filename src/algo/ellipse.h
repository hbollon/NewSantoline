#ifndef ALGO_ELLIPSE_H
#define ALGO_ELLIPSE_H

#include "Vecteur.h"
#include "Utils.h"

class AEllipse
{

    Vector3D m_wind;
    Vector3D m_slope;
    Vector3D m_windSlope;

    double m_aspect;
    double m_slope_value;

    double m_exentricite;
    double m_rateOfSpread;

    double m_temperature;
    double m_water_level;

    Point2D m_top_left;
    Point2D m_plan;

public:
    AEllipse(Vector3D wind, Vector3D slope, Vector3D windSlope, double aspect, double slope_value, double exentricite, double rateOfSpread,
             double temp, double swl, Point2D top_left, const Point2D plan);
    AEllipse(Vector3D wind, Vector3D slope, Vector3D windSlope, double aspect, double slope_value, double exentricite, double rateOfSpread,
             Point2D top_left, const Point2D plan);
    AEllipse();

    /**
     * Renvoi le vecteur vent
     * @return le vecteur vent
     */
    Vector3D get_wind() const;

    /**
     * Renvoi le vecteur pente
     * @return le vecteur pente
     */
    Vector3D get_slope() const;

    /**
     * Renvoi l'exposition de la pente
     * @return phi, l'aspect, ce qui correspond à l'exposition de la pente
     */
    double get_aspect() const;

    /**
     * Renvoi le denivele de la pente
     * @return alpha, le denivele de la pente
     */
    double get_slope_value() const;

    /**
     * Retourne la norme du vecteur pente, tronqué à 10.
     * @return la norme du vecteur pente ( formule de Finney, 2004 )
     */
    double get_slope_norm() const;

    /**
     * Calcul le temps de propagation du feu
     * @param a le point à partir duquel on part
     * @param b le point vers lequel on va
     * @return le temps de propagation entre les points A et B
     */
    double time(const Point2D &a, const Point2D &b) const;

    /**
     * Fait la somme du vecteur vent et pente
     * @return la somme du vecteur vent et du vecteur pente
     */
    Vector3D get_c() const;

    /**
     * Donne la direction de l'axe principale du feu (direction de l'ellipse)
     * @return la direction de l'axe de propagation du feu
     */
    Vector3D get_rc() const;

    /**
     * Calcul le ratio longueur-largeur (longueur/largeur)
     *  @return le ration longueur-largeur
     */
    double get_lb() const;

    /**
     * Renvoie l'excentricite de l'ellipse
     * @return epsilon, l'excentricité de l'ellipse
     */
    double get_e() const;

    /**
     * Calcule la longueur de l'axe majeur du feu
     * @return la longueur de l'axe de propagation du feu
     */
    double get_rc_norm() const;

    /**
     * Calcul la longueur de l'axe principal en fonction d'un angle donné
     * @param cosTheta Angle pour lequel on souhaite calculer la longueur de l'axe
     * @return la longueur de l'axe de propagation du feu le long d'un axe d'un
     * certain angle
     */
    double get_rc_norm(double cosTheta) const;

    /**
     * Calcule le cos de l'ange entre l'axe ab et le vecteur de propagation de feu
     * de la cellule.
     * @param ab l'axe à partir duquel on souhaite calculer l'angle avec l'axe de
     * propagation de feu
     * @return cos de l'angle désiré
     */
    double get_cos_theta(Vector3D ab) const;

    /**
     * Permet de calculer la hauteur d'un point en fonction de la pente et de
     * l'aspect
     * @param point en deux dimension pour lequel on souhaite calculer sa hauteur
     * @return le même points avec une troisième dimension
     */
    Point3D get_3d(const Point2D &p) const;

    Point2D get_topleft() const;
};

#endif //ALGO_ELLIPSE_H
