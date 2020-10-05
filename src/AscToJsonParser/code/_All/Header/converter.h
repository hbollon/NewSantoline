#ifndef __FACADE_CONVERTER__
#define __FACADE_CONVERTER__

#include <string>

namespace converter {

	/** Format de fichier supporté. */
	enum VALID_FORMAT {
		ASC,
		JSON,
		GeoJSON
	};

	/**
	 * Fonction du type énuméré qui permet d'obtenir le type énuméré au format
	 * chaine de caractères.
	 * @param[in] format Le format à convertir.
	 * @return Le format converti en chaine de caractères.
	 */
	std::string to_string(VALID_FORMAT format);

	/**
	 * Permet de faire la conversion d'une chaine de caractères vers
	 * un VALID_FORMAT.
	 * @param[in] format chaine de caractères à convertir.
	 * @return Résultat de la conversion, si celle ci c'est bien passée.
	 */
	VALID_FORMAT from_string(std::string format);

	/**
	 * Patron de conception Facade, permet de faire la facade de l'ensemble des
	 * classes mises en place pour faire la conversion.
	 */
	class Converter
	{
	public:
		/**
		 * Constructeur.
		 * @param[in] input_filename Le nom de fichier en entrée. Doit nécessairement avoir une extension.
		 * @param[in] output_filename Le nom de fichier en sortie. L'extension est rajouté par le code.
		 * @param[in] output_format Le format de sortie.
		 */
		Converter(std::string input_filename, std::string output_filename, std::string output_format);

		/**
		 * Constructeur.
		 * @param[in] input_filename Le nom de fichier en entrée. Doit nécessairement avoir une extension.
		 * @param[in] output_format Le format de sortie.
		 */
		Converter(std::string input_filename, std::string output_format);

		/**
		 * Constructeur.
		 * @param[in] input_filename Le nom de fichier en entrée. Doit nécessairement avoir une extension.
		 * @param[in] output_filename Le nom de fichier en sortie. L'extension est rajouté par le code.
		 * @param[in] output_format Le format de sortie.
		 */
		Converter(std::string input_filename, std::string output_filename, VALID_FORMAT output_format);

		/**
		 * Constructeur.
		 * @param[in] input_filename Le nom de fichier en entrée. Doit nécessairement avoir une extension.
		 * @param[in] output_format Le format de sortie.
		 */
		Converter(std::string input_filename, VALID_FORMAT output_format);

		/** Destructeur. */
		~Converter();

		/** Fonction qui fait appelle au différente méthode pour faire la conversion. */
		virtual void convert();
	private:

		/**
		 * Permet de récuperer l'extension du nom de fichier en entrée.
		 * @return L'extension du fichier en entrée.
		 */
		virtual std::string extension();

		/**
		 * Permet d'obtenir le nom du fichier en entrée sans l'extension.
		 * @return Le nom du fichier en entrée sans l'extension.
		 */
		virtual std::string filename();

		/** Nom du fichier en entrée, avec l'extension. */
		std::string _input_filename;

		/** Format du fichier en entrée. */
		VALID_FORMAT _input_format;

		/** Nom du fichier en sortie, sans extension. */
		std::string _output_filename;

		/** Format du fichier en sortie. */
		VALID_FORMAT _output_format;
	};
}
#endif // !__FACADE_CONVERTER__