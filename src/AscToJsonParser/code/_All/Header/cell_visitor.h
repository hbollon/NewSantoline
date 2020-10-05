/**
 * \file cell.h
 * \brief Représentation d'un visiteur d'une cellule.
 * \author Antoine Porcheron-Roche - antoine.pr@live.fr
 * \version 0.1
 * \date 20 Juillet 2018
 *
 * Comme j'explique dans la cellule, cette classe va servir à mettre en place le 
 * patron de conception visiteur. Dans cette classe en particulier, je défini 
 * la classe mère visiteuse, qui est une classe purement virtuel.
 *
 */
 
#ifndef __CELL_VISITOR_CONVERTER__
#define __CELL_VISITOR_CONVERTER__

#include "converter_cell.h"
#include "cells.h"

namespace converter {


	// Inclusion circulaire. 
	class Cells;
	class Cell;

	/** Classe visiteuse d'une cellule. */
	class CellVisitor {
	public:

		/**
		 * Fonction qui permet d'interagir avec un jeu de cellules.
		 * @param cell Le jeu de cellule avec lequel on interagi.
		 */
		virtual void visit_cells(Cells* cell) = 0;

		/**
		 * Fonction qui permet d'interagir avec une cellule.
		 * @param cell La cellule avec laquel on interagie.
		 */
		virtual void visit_cell(Cell* cell) = 0;
	};

}

#endif //__CELL_VISITOR_CONVERTER__