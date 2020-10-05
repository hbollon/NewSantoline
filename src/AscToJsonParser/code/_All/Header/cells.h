/**
 * \file cells.h
 * \brief Représentation d'un ensemble de cellule.
 * \author Antoine Porcheron-Roche - antoine.pr@live.fr
 * \version 0.1
 * \date 20 Juillet 2018
 *
 * Classe qui représente un ensemble de cellule, en respectant le patron de conception
 * visiteur.
 * 
 */
#ifndef __CELLS_CONVERTER__
#define __CELLS_CONVERTER__

#include "converter_cell.h"
#include "cell_visitor.h"
#include "cells_state.h"

#include <vector>

namespace converter {
	// Inclusions circulaires.
	class CellsState;
	class Cell;
	class CellVisitor;

	/** Classe qui représente un ensemble de cellules. */
	class Cells {
	public:
		/** Constructeur par défaut. */
		Cells();

		/** Destructeur par défaut. */
		~Cells();

		/**
		 * Permet de redimensionner le jeu de cellules.
		 * @param size La nouvelle taille du jeu de cellules.
		 */
		virtual void resize(unsigned int size);

		/**
		 * Permet d'ajouter une cellule au jeu de cellules.
		 * @param cell La nouvelle cellule à ajouter.
		 */
		virtual void push_back(const Cell& cell);

		/**
		 * Fonction qui permet de mettre en place le patron de conception visiteur.
		 * Cette fonction permet au visiteur d'interagir avec le jeu de cellules.
		 * @param cell_visitor Le visiteur à faire intéragir avec le jeu de cellules.
		 */
		virtual void accept(CellVisitor& cell_visitor);

		/* Getter */
		/**
		 * Permet d'obtenir la largeur du jeu de cellules.
		 * @return La largeur du jeu de cellules.
		 */
		virtual unsigned int width();

		/**
		 * Permet d'obtenir la hauteur du jeu de cellules.
		 * @return La heuteur du jeu de cellules.
		 */
		virtual unsigned int height();

		/**
		 * Permet d'obtenir la largeur d'une cellule.
		 * @return La largeur d'une cellule.
		 */
		virtual unsigned int cell_width();

		/**
		 * Permet d'obtenir la hauteur d'une cellule.
		 * @return La hauteur d'une cellule.
		 */
		virtual unsigned int cell_height();

		/**
		 * Permet d'obtenir le nombre de cellules dans ce jeu de cellules.
		 * @return Le nombre de cellules dans ce jeu de cellules.
		 */
		virtual unsigned int size();

		/**
		 * Permet de trier le jeu de cellules comme une grille, avec son
		 * premier element étant la case supérieur gauche, le second la
		 * case directement à droite de la première, et ainsi de suite.
		 * Schéma :
		 * -------------
		 * | 1 | 2 | 3 |
		 * -------------
		 * | 4 | 5 | 6 |
		 * -------------
		 * |    ...    |
		 * -------------
		 */
		virtual void sort();

		/**
		 * Permet de trier le jeu de cellules comme une grille, avec son
		 * premier element étant la case supérieur gauche, le second la
		 * case directement à droite de la première, et ainsi de suite.
		 * Schéma :
		 * -------------
		 * | 1 | 2 | 3 |
		 * -------------
		 * | 4 | 5 | 6 |
		 * -------------
		 * |    ...    |
		 * -------------
		 */
		virtual void sort_call();

		/**
		 * Permet de savoir si le jeu de cellules a été trié ou non.
		 * @return Vrai si le jeu de cellule est trié, faux sinon.
		 */
		virtual bool sorted();

		/**
		 * Permet de définir l'état du jeu de cellules.
		 * @param state Le nouvel état du jeu de cellules.
		 */
		virtual void set_state(CellsState* state);

		/**
		 * Getter d'une cellule via les crochets.
		 * @param i La position de la cellule que l'on veut.
		 * @return La cellule que l'on veut.
		 */
		virtual Cell operator[](int i) const;

		/**
		 * Setter d'une cellule via les crochets.
		 * @param i La position de la cellule que l'on veut.
		 * @return La cellule que l'on veut modifier.
		 */
		virtual Cell& operator[](int i);
	private:

		/** Ensemble des cellules du jeu de cellules. */
		std::vector<Cell> _cells;

		/** Etat courant de l'ensemble des cellules. */
		CellsState* _state;
	};

}

#endif //__CELLS_CONVERTER__