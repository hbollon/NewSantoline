/**
 * \file pipe.h
 * \brief Pipe windows.
 * \author Antoine Porcheron-Roche (antoine.pr@live.fr)
 * \version 0.2
 * \date 16 juillet 2018
 *
 * Code qui permet de représenté les tubes nommés windows avec le paradigme
 * objet. Les classes utilise le patron de conception état.
 * Le code ici présent fait office de serveur, et ne permet pas de faire 
 * un client simple en l'état. (Les modifications à faire serais
 * assez trivial pour obtenir un client simple.)
 */
#ifndef __PIPE__
#define __PIPE__

#include <Windows.h>

#include <string>

namespace Tube {

	/**
	 * Effectue la conversion d'un tableau de quatre octets vers des entiers.
	 * @param byteArray Le tableau à convertir en entier.
	 * @return L'entier obtenu à partir du tableau.
	 */
	int byteArrayToInt(char* byteArray);

	/**
	 * Représente les différents état possible pour le tube nommé.
	 */
	enum State {
		Deconnecte,
		Connecte
	};

	/* Il y a des attributs circulaires, sans cette prédéclaration 
	 * des classes, le code ne compile pas.
	 */
	class TubeEtat;
	class TubeMultiplexe;
	
	/**
	 * Tube nommé multiplexé, ce qui signifie qu'il peut effectué la lecture
	 * et l'écriture dans un seul et même tube.
	 */
	class TubeMultiplexe{
	public:
		/**
		 * Constructeur par défaut des tubes.
		 * @param pipename le nom du tube nommé.
		 */
		TubeMultiplexe(std::wstring pipename);

		/**
		 * Attend une connexion au tube nommé, pour pouvoir effectuer les
		 * opérations de lecture et d'écriture de manière fiable.
		 */
		void connect();

		/**
		 * Permet de savoir si le tube est connecté avec un client ou non.
		 *  @return Vrai si le tube est connecté à un client, faux sinon.
		 */
		bool is_connected();

		/**
		 * Effectue la lecture dans le tube nommé. Peut renvoyer une erreur
		 * DisconnectedException si le tube n'est pas connecté à un client.
		 */
		char* read();

		/**
		* Effectue lalecture dans le tube nommé.
		* Encapsule la fonction read() de manière à ne pas avoir à gerer la
		* mémoire.
		*/
		std::string read_str();


		/**
		 * Effectue l'écriture dans le tube nommé. Peut renvoyer une erreur
		 * DisconnectedException si le tube n'est pas connecté à un client.
		 * @param message chaine de charactères à écrire dans le tube.
		 */
		void write(std::string message);

		/**
		 * Effectue l'écriture dans le tube nommé. Peut renvoyer une erreur
		 * DisconnectedException si le tube n'est pas connecté à un client.
		 * @param message entier à écrire dans le tube.
		 */
		void write(int message);

		/**
		 * Effectue l'écriture dans le tube nommé. Peut renvoyer une erreur
		 * DisconnectedException si le tube n'est pas connecté à un client.
		 * @param message charactère à écrire dans le tube.
		 */
		void write(char message);

		/**
		 * Effectue l'écriture dans le tube nommé. Peut renvoyer une erreur
		 * DisconnectedException si le tube n'est pas connecté à un client.
		 * @param message pointeur sur l'objet que l'on veut écrire.
		 * @param size taille de l'objet à écrire, en octet.
		 */
		void write(LPCVOID message, int size);

		/**
		 * Effectue la déconnexion du tube nommé avec le client.
		 */
		void close();

		/**
		 * Permet d'obtenir l'état courant du tube nommé.
		 * @return L'état du tube nommé. Peut être connecté ou deconnecté.
		 */
		State get_state();

		/**
		 * Permet de récupérer le tube nommé.
		 * @return le tube nommé.
		 */
		HANDLE pipe();

		/**
		 * Permet de modifier l'état du tube nommé.
		 * @param state le nouvel état du tube nommé.
		 */
		void set_state(TubeEtat * state);

	protected:

		/**
		* Permet d'obtenir le chemin d'accès du tube nommé.
		* @return Le chemin d'accès du tube nommé.
		*/
		std::wstring get_pipepath();

		/**
		* L'état courant du tube nommé.
		*/
		TubeEtat * m_etat;

		/**
		* Le tube nommé.
		*/
		HANDLE m_pipe;

		/**
		* Le nom du tube nommé.
		*/
		std::wstring m_pipename;
	};

	/**
	 * Classe mère des états possible du tube nommé.
	 */
	class TubeEtat {
	public:
		/**
		 * Constructeur des états du tube.
		 * @param tube Pointeur vers le tube nommé concerné.
		 */
		TubeEtat(TubeMultiplexe* tube);

		/**
		* Attend une connexion au tube nommé, pour pouvoir effectuer les
		* opérations de lecture et d'écriture de manière fiable.
		*/
		virtual void connect() = 0;

		/**
		 * Permet de savoir si le tube est connecté avec un client ou non.
		 *  @return Vrai si le tube est connecté à un client, faux sinon.
		 */
		virtual bool is_connected() = 0;

		/**
		 * Effectue la lecture dans le tube nommé. Peut renvoyer une erreur
		 * DisconnectedException si le tube n'est pas connecté à un client.
		 */
		virtual char* read() = 0;

		/**
		 * Effectue l'écriture dans le tube nommé. Peut renvoyer une erreur
		 * DisconnectedException si le tube n'est pas connecté à un client.
		 * @param message pointeur sur l'objet que l'on veut écrire.
		 * @param size taille de l'objet à écrire, en octet.
		 */
		virtual void write(LPCVOID message, int size) = 0;

		/**
		 * Effectue la déconnexion du tube nommé avec le client.
		 */
		virtual void close() = 0;

		/**
		 * Permet d'obtenir l'état courant du tube nommé.
		 * @return L'état du tube nommé. Peut être connecté ou deconnecté.
		 */
		virtual State get_state() = 0;
	protected:

		/**
		 * Pointeur vers le tube géré par les états.
		 */
		TubeMultiplexe * m_tube;
	};

	class TubeEtatDeconnecte : public TubeEtat {
	public:
		TubeEtatDeconnecte(TubeMultiplexe * tube);

		/**
		 * Attend une connexion au tube nommé, pour pouvoir effectuer les
		 * opérations de lecture et d'écriture de manière fiable.
		 */
		void connect();

		/**
		 * Permet de savoir si le tube est connecté avec un client ou non.
		 *  @return Vrai si le tube est connecté à un client, faux sinon.
		 */
		bool is_connected();

		/**
		 * Effectue la lecture dans le tube nommé. Peut renvoyer une erreur
		 * DisconnectedException si le tube n'est pas connecté à un client.
		 */
		char* read();

		/**
		 * Effectue l'écriture dans le tube nommé. Peut renvoyer une erreur
		 * DisconnectedException si le tube n'est pas connecté à un client.
		 * @param message pointeur sur l'objet que l'on veut écrire.
		 * @param size taille de l'objet à écrire, en octet.
		 */
		void write(LPCVOID message, int size);

		/**
		 * Effectue la déconnexion du tube nommé avec le client.
		 */
		void close();

		/**
		 * Permet d'obtenir l'état courant du tube nommé.
		 * @return L'état du tube nommé. Peut être connecté ou deconnecté.
		 */
		State get_state();
	};

	class TubeEtatConnecte : public TubeEtat {
	public:
		TubeEtatConnecte(TubeMultiplexe* tube);

		/**
		 * Attend une connexion au tube nommé, pour pouvoir effectuer les
		 * opérations de lecture et d'écriture de manière fiable.
		 */
		void connect();

		/**
		 * Permet de savoir si le tube est connecté avec un client ou non.
		 *  @return Vrai si le tube est connecté à un client, faux sinon.
		 */
		bool is_connected();

		/**
		 * Effectue la lecture dans le tube nommé. Peut renvoyer une erreur
		 * DisconnectedException si le tube n'est pas connecté à un client.
		 * @return Un pointeur vers la chaine lue. Il faut penser à la désallouer.
		 */
		char* read();

		/**
		 * Effectue l'écriture dans le tube nommé. Peut renvoyer une erreur
		 * DisconnectedException si le tube n'est pas connecté à un client.
		 * @param message pointeur sur l'objet que l'on veut écrire.
		 * @param size taille de l'objet à écrire, en octet.
		 */
		void write(LPCVOID message, int size);

		/**
		 * Effectue la déconnexion du tube nommé avec le client.
		 */
		void close();

		/**
		 * Permet d'obtenir l'état courant du tube nommé.
		 * @return L'état du tube nommé. Peut être connecté ou deconnecté.
		 */
		State get_state();
	};
};
#endif //__PIPE__