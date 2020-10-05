/**
 * \file cell_saver.h
 * \brief Permet de faire la sauvegarde d'un jeu de cellules sous différents formats.
 * \author Antoine Porcheron-Roche - antoine.pr@live.fr
 * \version 0.1
 * \date 21 Juillet 2018
 * 
 * Permet de faire la sauvegarde d'un jeu de cellules sous différents formats, en 
 * utilisant le patron de conception visiteur.
 *
 */
#ifndef __CELL_SAVER_CONVERTER__
#define __CELL_SAVER_CONVERTER__

#include <string>
#include <vector>

#include "cell_visitor.h"
#include "cells.h"
#include "converter_cell.h"

namespace converter {
	/** Classe qui permet de représenter un visiteur de sauvegarde. */
	class SavingVisitor : public CellVisitor {
	public:

		/**
		 * Fonction qui permet d'interagir avec une cellule.
		 * @param cell La cellule avec laquel on interagi.
		 */
		virtual void visit_cell(Cell* cell) = 0;

		/**
		 * Fonction qui permet d'interagir avec un jeu de cellules.
		 * @param cell Le jeu de cellules avec lequel on veut interagir.
		 */
		virtual void visit_cells(Cells* cells);

		/**
		 * Permet de sauvegarder dans un fichier le jeu de cellules.
		 * @param filename Nom du fichier dans lequel on veut sauvegarder, sans extension.
		 */
		virtual void save_file(std::string filename) = 0;

		/**
		 * Opérateur d'écriture dans un stream.
		 * @param os Le stream dans lequel on écrit.
		 * @param Le sauveur que l'on veut écrire.
		 * @return Le stream après l'écriture du sauveur.
		 */
		friend std::ostream& operator<<(std::ostream& os, const SavingVisitor& saving);
	protected:

		/**
		 * L'ensemble des éléments que l'on veut sauvegarder, au format chaine de
		 * caractères.
		 */
		std::vector<std::string> _saved;
	};

	/** Sauvegardeur au format JSON. Hérite du visiteur de sauvegarde SavingVisitor. */
	class JSONSavingVisitor : public SavingVisitor {
	public:

		/**
		 * Fonction qui permet d'interagir avec une cellule.
		 * @param cell La cellule avec laquel on interagi.
		 */
		virtual void visit_cell(Cell* cell);

		/**
		 * Permet de sauvegarder dans un fichier le jeu de cellules au format JSON.
		 * @param filename Nom du fichier dans lequel on veut sauvegarder, sans extension.
		 */
		virtual void save_file(std::string filename);

		/**
		 * Permet de convertir une cellule en chaine de caractères représentant un JSON
		 * valide.
		 * @param cell La cellule à transformer en JSON.
		 * @return La chaine de caractères qui représente la cellule JSONifiée.
		 */
		virtual std::string jsonify_cell(const Cell& cell);

	};

	/** Sauvegardeur au format GeoJSON. Hérite du visiteur de sauvegarde SavingVisitor. */
	class GeoJSONSavingVisitor : public SavingVisitor {
	public:
		/**
		 * Fonction qui permet d'interagir avec une cellule.
		 * @param cell La cellule avec laquel on interagi.
		 */
		virtual void visit_cell(Cell* cell);

		/**
		 * Permet de sauvegarder dans un fichier le jeu de cellules au format GeoJSON.
		 * @param filename Nom du fichier dans lequel on veut sauvegarder, sans extension.
		 */
		virtual void save_file(std::string filename);

		/**
		* Permet de convertir une cellule en chaine de caractères représentant un GeoJSON
		* valide.
		* @param cell La cellule à transformer en GeoJSON.
		* @return La chaine de caractères qui représente la cellule GeoJSONifiée.
		*/
		virtual std::string geojsonify_cell(const Cell& cell);
	};

	/** Sauvegardeur au format Ascii. Hérite du visiteur de sauvegarde SavingVisitor. */
	class ASCSavingVisitor : public SavingVisitor {
	public:
		/**
		 * Fonction qui permet d'interagir avec une cellule.
		 * @param cell La cellule avec laquel on interagi.
		 */
		virtual void visit_cell(Cell* cell);

		/**
		 * Fonction qui permet d'interagir avec un jeu de cellules.
		 * @param cell Le jeu de cellules avec lequel on veut interagir.
		 */
		virtual void visit_cells(Cells* cells);

		/**
		 * Permet de sauvegarder dans un fichier le jeu de cellules au format GeoJSON.
		 * @param filename Nom du fichier dans lequel on veut sauvegarder, sans extension.
		 */
		virtual void save_file(std::string filename);
	private:

		/**
		 * Fait la mise à jour de la position courante, et met à jour les chaines de
		 * caractères si besoin.
		 */
		virtual void update_cols();

		/** Fonction qui fait la mise à jour des chaines de caractères si besoin. */
		virtual void update_saved_string();

		/**
		 * Permet d'obtenir l'entête d'un fichier ASC.
		 * @return L'entête du fichier ASC.
		 */
		virtual std::string header();

		/** Angle supérieur gauche de l'ensemble des points. */
		double _corner_x;
		double _corner_y;

		/** Nombre de lignes et de colonnes. */
		int _rows;
		int _cols;

		/** Dimension des cellules. */
		double _cellsize;

		/** Valeur associé à une absence de données (normalement pas utilisé.) */
		double _nodata;

		/** Position courante dans les colonnes du jeu de cellules. */
		int _current_cols;

		/** Vecteur de sauvegarde pour l'altitude et pour la vitesse du vent. */
		std::vector<std::string> _saved_alt;
		std::vector<std::string> _saved_vel;

		/** Chaine de caractères temporaire pour l'angle, la vitesse et l'altitude. */
		std::string _temporary_cols_ang;
		std::string _temporary_cols_vel;
		std::string _temporary_cols_alt;
	};
}

#endif //__CELL_SAVER_CONVERTER__