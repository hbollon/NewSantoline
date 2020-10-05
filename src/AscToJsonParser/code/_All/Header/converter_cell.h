/**
 * \file cell.h
 * \brief Représentation d'une cellule.
 * \author Antoine Porcheron-Roche - antoine.pr@live.fr
 * \version 0.1
 * \date 20 Juillet 2018
 *
 * Cette classe permet de représenter une cellule avec tout ses informations, c'est 
 * à dire, sont vent (représenté avec un angle et une vitesse), mais aussi l'altitude,
 * et les coordonnées de la cellule. Cette classe peut être visiter (elle met en place
 * le patron de conception visiteur).
 */
 
#ifndef __CELL_CONVERTER__
#define __CELL_CONVERTER__

#include "cell_visitor.h"

#include <iostream>

namespace converter {
	// Inclusion circulaire.
	class CellVisitor;

	/** Class qui représente une cellule avec son vent et sa pente.*/
	class Cell {
	public:
		/** Constructeur par défaut. */
		Cell();

		/**
		 * Constructeur pour définir tout les attributs.
		 * @param[in] wind_direction La direction du vent, en degrés.
		 * @param[in] wind_speed La vitesse du vent, en mètres par secondes.
		 * @param[in] x Coordonnée X de la cellule.
		 * @param[in] y Coordonnée Y de la cellule.
		 * @param[in] altitude Altitude de la cellule, en mètres.
		 */
		Cell(double wind_direction, double wind_speed, double x, double y, double altitude);

		/** Destructeur par défaut. */
		virtual ~Cell();

		/* Getter et Setter */

		/**
		 * Getter de la direction du vent.
		 * @return La direction du vent.
		 */
		virtual double wind_direction() const;

		/**
		 * Setter de la direction du vent.
		 * @return La direction du vent.
		 */
		virtual double& wind_direction();

		/**
		 * Getter de la vitesse du vent.
		 * @return La vitesse du vent.
		 */
		virtual double wind_speed() const;

		/**
		 * Setter de la vitesse du vent.
		 * @return La vitesse du vent.
		 */
		virtual double& wind_speed();

		/**
		 * Getter de la coordonnée X de la cellule.
		 * @return La coordonnée X de la cellule.
		 */
		virtual double x() const;

		/**
		 * Setter de la coordonnée X de la cellule.
		 * @return La coordonnée X de la cellule.
		 */
		virtual double& x();

		/**
		 * Getter de la coordonnée Y de la cellule.
		 * @return La coordonnée Y de la cellule.
		 */
		virtual double y() const;

		/**
		 * Setter de la coordonnée Y de la cellule.
		 * @return La coordonnée Y de la cellule.
		 */
		virtual double& y();

		/**
		 * Getter de l'altitude de la cellule.
		 * @return L'altitude de la cellule.
		 */
		virtual double altitude() const;

		/**
		 * Setter de l'altitude de la cellule.
		 * @return L'altitude de la cellule.
		 */
		virtual double& altitude();

		/**
		 * Fonction qui permet à un visiteur d'interagir avec la cellule.
		 * Permet la mise en place du patron de conception visiteur.
		 * @param cell_visitor Le visiteur de la cellule.
		 */
		virtual void accept(CellVisitor& cell_visitor);

		/**
		 * Scribe par défaut de la cellule.
		 * @param os Le stream dans lequel on inscrit la cellule.
		 * @param La cellule à inscrire.
		 * @return Le stream après l'écriture de la cellule.
		 */
		friend std::ostream& operator<<(std::ostream& os, const Cell& c);
	private:

		/** Direction du vent et vitesse du vent. */
		double _wind_direction, _wind_speed;

		/** Coordonnées de la cellule. */
		double _x, _y;

		/** Altitude de la cellule. */
		double _altitude;
	};
}

#endif //__CELL_CONVERTER__