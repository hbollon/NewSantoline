/**
 * \file converter_exception.h
 * \brief Spécialisation d'exception
 * \author Antoine Porcheron-Roche - antoine.pr@live.fr
 * \version 0.1
 * \date 21 Juillet 2018
 *
 * Simple fichier qui contient des spécialisations d'exceptions pour avoirs des
 * erreurs plus spécifique et plus claire.
 * 
 */
#ifndef __IOFILE_EXCEPTION_CONVERTER__
#define __IOFILE_EXCEPTION_CONVERTER__

#include <exception>
#include <string>

namespace converter {
	/**
	 * Classe qui représente une erreur lorsqu'un problème à eu lieu concernant
	 * une entrée/sortie avec un fichier.
	 */
	class IOFileException : public std::exception {
	public:
		/**
		 * Constructeur avec un message
		 * @param message Le message à afficher en cas d'erreur.
		 */
		explicit IOFileException(const char* message) : _msg(message) {}
		/**
		 * Constructeur avec un message
		 * @param message Le message à afficher en cas d'erreur.
		 */
		explicit IOFileException(const std::string& message) : _msg(message) {}

		/** Destructeur par défaut. */
		virtual ~IOFileException() {}

		/**
		 * Spécialisation de la fonction "what", qui retourne le message d'erreur obtenu.
		 * @return Le message d'erreur obtenu.
		 */
		virtual const char* what() const {
			return _msg.c_str();
		}
	protected:
		/** Le message d'erreur. */
		std::string _msg;
	};

	/** Classe qui retourne une exception en cas de format non supporté. */
	class UnsuportedFormatException : public std::exception {
	public:
		/**
		* Constructeur avec un message
		* @param message Le message à afficher en cas d'erreur.
		*/
		explicit UnsuportedFormatException(const char* message) : _msg(message) {}
		/**
		* Constructeur avec un message
		* @param message Le message à afficher en cas d'erreur.
		*/
		explicit UnsuportedFormatException(const std::string& message) : _msg(message) {}

		/** Destructeur par défaut. */
		virtual ~UnsuportedFormatException() {}

		/**
		* Spécialisation de la fonction "what", qui retourne le message d'erreur obtenu.
		* @return Le message d'erreur obtenu.
		*/
		virtual const char* what() const {
			return _msg.c_str();
		}
	protected:
		/** Le message d'erreur. */
		std::string _msg;
	};

}

#endif //__IOFILE_EXCEPTION_CONVERTER__