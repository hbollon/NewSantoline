/**
 * \file cells_state.h
 * \brief Représente les états possible d'un jeu de cellules.
 * \author Antoine Porcheron-Roche - antoine.pr@live.fr
 * \version 0.1
 * \date 21 Juillet 2017
 * 
 * Fichier qui contient les différents états pour un ensemble de cellules par rapport
 * au pattern état.
 */

#ifndef __CELLS_STATE_CONVERTER__
#define __CELLS_STATE_CONVERTER__

#include "cells.h"

namespace converter {
	// Inclusion circulaire. 
	class Cells;

	/** Classe mère des états d'un ensemble de cellules. */
	class CellsState {
	public:
		/**
		 * Constructeur.
		 * @param cells Le jeu de cellules qui est géré par cet état.
		 */
		CellsState(Cells* cells);

		/** Destructeur par défaut. */
		~CellsState();

		/* Getter */

		/**
		 * Getter de la largeur.
		 * @return la largeur.
		 */
		virtual unsigned int width() = 0;

		/**
		 * Getter de la hauteur.
		 * @return la hauteur.
		 */
		virtual unsigned int height() = 0;

		/**
		 * Getter de la largeur d'une cellule.
		 * @return la hauteur d'une cellule.
		 */
		virtual unsigned int cell_width() = 0;

		/**
		 * Getter de la hauteur d'une cellule.
		 * @return la hauteur d'une cellule.
		 */
		virtual unsigned int cell_height() = 0;

		/**
		 * Permet de savoir si le jeu de cellules à été trié.
		 * @return Vrai si le jeu de cellule est trié, faux sinon.
		 */
		virtual bool sorted() = 0;

		/** Fonction qui trie le jeu de cellules. */
		virtual void sort() = 0;
	protected:

		/** Le jeu de cellule géré par cet état. */
		Cells * _cells;
	};

	/**
	 * Représente l'état de cellule trié. Hérite de CellsState.
	 */
	class CellsStateSorted : public CellsState {
	public:
		/**
		 * Constructeur.
		 * @param cells Le jeu de cellules qui est géré par cet état.
		 */
		CellsStateSorted(Cells* cells);

		/** Destructeur par défaut. */
		~CellsStateSorted();

		/**
		 * Getter de la largeur.
		 * @return la largeur.
		 */
		virtual unsigned int width();

		/**
		 * Getter de la hauteur.
		 * @return la hauteur.
		 */
		virtual unsigned int height();

		/**
		 * Getter de la largeur d'une cellule.
		 * @return la hauteur d'une cellule.
		 */
		virtual unsigned int cell_width();

		/**
		 * Getter de la hauteur d'une cellule.
		 * @return la hauteur d'une cellule.
		 */
		virtual unsigned int cell_height();

		/**
		 * Permet de savoir si le jeu de cellules à été trié.
		 * @return Vrai si le jeu de cellule est trié, faux sinon.
		 */
		virtual bool sorted();

		/** Fonction qui trie le jeu de cellules. */
		virtual void sort();
	};


	/**
	 * Classe qui représente l'état non trié pour un ensemble de cellule. Hérite de CellsState.
	 */
	class CellsStateUnsorted : public CellsState {
	public:

		/**
		 * Constructeur.
		 * @param cells Le jeu de cellules qui est géré par cet état.
		 */
		CellsStateUnsorted(Cells* cells);

		/** Destructeur par défaut. */
		~CellsStateUnsorted();

		/**
		 * Getter de la largeur.
		 * @return la largeur.
		 */
		virtual unsigned int width();

		/**
		 * Getter de la hauteur.
		 * @return la hauteur.
		 */
		virtual unsigned int height();

		/**
		 * Getter de la largeur d'une cellule.
		 * @return la hauteur d'une cellule.
		 */
		virtual unsigned int cell_width();

		/**
		 * Getter de la hauteur d'une cellule.
		 * @return la hauteur d'une cellule.
		 */
		virtual unsigned int cell_height();

		/**
		 * Permet de savoir si le jeu de cellules à été trié.
		 * @return Vrai si le jeu de cellule est trié, faux sinon.
		 */
		virtual bool sorted();

		/** Fonction qui trie le jeu de cellules. */
		virtual void sort();
	};

}

#endif // __CELLS_STATE_CONVERTER__