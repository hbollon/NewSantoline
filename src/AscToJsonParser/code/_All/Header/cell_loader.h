/**
 * \file cell_loader.h
 * \brief Permet de faire le chargement depuis différent format.
 * \author Antoine Porcheron-Roche - antoine.pr@live.fr
 * \version 0.1
 * \date 21 Juillet 2018
 * 
 * Permet de charger un jeu de cellules depuis différent format, en utilisant
 * le patron de conception visiteur.
 */

#ifndef __CELL_LOADER_CONVERTER__
#define __CELL_LOADER_CONVERTER__

#include "cell_visitor.h"
#include "json.hpp"

#include <fstream>

namespace converter {
	/** Visiteur mère des visiteur de chargement. Hérite de la classe CellVisitor. */
	class LoadingVisitor : public CellVisitor {
	public:

		/**
		 * Constructeur.
		 * @param filename Nom de fichier depuis lequel on charge les données.
		 */
		LoadingVisitor(std::string filename);

		/**
		 * Fonction qui permet d'interagir avec un jeu de cellules.
		 * @param cell Le jeu de cellules avec lequel on veut interagir.
		 */
		virtual void visit_cells(Cells* cells) = 0;

		/**
		 * Fonction qui permet d'interagir avec une cellule.
		 * @param cell La cellule avec laquel on interagi.
		 */
		virtual void visit_cell(Cell* cell) = 0;
	protected:

		/** Nom de fichier depuis lequel on charge les données. */
		std::string _filename;
	};

	/** Chargeur depuis le format ascii. Hérite de LoadingVisitor. */
	class AscLoader : public LoadingVisitor {
	public:

		/**
		 * Constructeur.
		 * @param filename Nom de fichier depuis lequel on charge les données.
		 */
		AscLoader(std::string filename);

		/** Destructeur par défaut. */
		~AscLoader();

		/**
		 * Fonction qui permet d'interagir avec un jeu de cellules.
		 * @param cell Le jeu de cellules avec lequel on veut interagir.
		 */
		virtual void visit_cells(Cells* cells);

		/**
		 * Fonction qui permet d'interagir avec une cellule.
		 * @param cell La cellule avec laquel on interagi.
		 */
		virtual void visit_cell(Cell* cell);
	protected:

		/** Permet d'effectuer le pré traitement des fichiers. */
		virtual void load();

		/** Permet de lire le nombre de lignes et de colonnes dans les fichiers. */
		virtual void get_rows_and_cols();

		/** Permet de lire l'angle défini dans les fichiers. */
		virtual void get_corner();

		/** Permet d'obtenir la dimension des cellules dans les fichiers. */
		virtual void get_dimension();

		/** Permet d'obtenir la valeur du NoData dans les fichiers. */
		virtual void get_no_data();

		/**
		 * Permet de mettre à jour les coordonnées de la cellule que
		 * l'on est en train de lire.
		 */
		virtual void update_current_position();

		/** Peremt d'obtenir la coordonnées X courante. */
		virtual double x();

		/** Permet d'obtenir la coordonnées Y courante. */
		virtual double y();

		/** Fichier de direction du vent. */
		std::ifstream _wind_direction;

		/** Fichier de vitesse du vent. */
		std::ifstream _wind_velocity;

		/** Fichier d'altimétrie. */
		std::ifstream _altitude;

	private:

		/** Nombre de lignes et de colonnes. */
		int _cols;
		int _rows;

		/** Ligne et colonne que l'on est en train de lire dans les fichiers. */
		int _current_cols;
		int _current_rows;

		/** Angle de départ décris dans les fichiers. */
		double _corner_x;
		double _corner_y;

		/** Dimension des cellules. */
		double _cell_dimension;

		/** Nodata décris dans les fichiers. */
		double _no_data;

	};

	/**
	 * Permet de faire le chargement depuis des fichiers JSON.
	 * Hérite de LoadingVisitor.
	 */
	class JSONLoader : public LoadingVisitor {
	public:

		/**
		 * Constructeur.
		 * @param filename Nom de fichier depuis lequel on charge les données.
		 */
		JSONLoader(std::string filename);

		/**
		 * Fonction qui permet d'interagir avec un jeu de cellules.
		 * @param cell Le jeu de cellules avec lequel on veut interagir.
		 */
		virtual void visit_cells(Cells* cells);

		/**
		 * Fonction qui permet d'interagir avec une cellule.
		 * @param cell La cellule avec laquel on interagi.
		 */
		virtual void visit_cell(Cell* cell);

	protected:

		/** JSON à traiter. */
		nlohmann::json _json;

		/** Cellule que l'on est en train de traité. */
		int _current_pos;
	};

	/**
	* Permet de faire le chargement depuis des fichiers GeoJSON.
	* Hérite de JSONLoader, car le GeoJSON est un format de JSON spécialisé.
	*/
	class GeoJSONLoader : public JSONLoader {
	public:

		/**
		 * Constructeur.
		 * @param filename Nom de fichier depuis lequel on charge les données.
		 */
		GeoJSONLoader(std::string filename);

		/**
		 * Fonction qui permet d'interagir avec un jeu de cellules.
		 * @param cell Le jeu de cellules avec lequel on veut interagir.
		 */
		virtual void visit_cells(Cells* cells);

		/**
		 * Fonction qui permet d'interagir avec une cellule.
		 * @param cell La cellule avec laquel on interagi.
		 */
		virtual void visit_cell(Cell* cell);
	};
}

#endif // __CELL_LOADER_CONVERTER__