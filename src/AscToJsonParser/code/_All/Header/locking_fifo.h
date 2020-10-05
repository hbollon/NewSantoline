/**
 * \file fifo.h
 * \brief Classe simple qui représente une file premier arrivé premier sorti (fifo)
 * avec les opérations basique d'un fifo.
 * \author Antoine Porcheron-Roche - antoine.pr@live.fr
 * \version 0.1
 * \date 03/07/2018
 */
 
#ifndef __LOCKING_FIFO__ 
#define __LOCKING_FIFO__

#include <vector>
#include <windows.h>

namespace File{
	/**
	 * File First In First Out. L'opération de pop est bloquante si la file est vide.
	 */
	template<class T>
	class LockingFIFO{
	public:
		
		/**
		 * Constructeur de base.
		 */
		LockingFIFO(){}
		
		/**
		 * Permet de récuperer le prochain élément.
		 * Ne fait pas la vérification pour la taille de la FIFO, et ne vérifie
		 * donc pas si elle est vide.
		 * @return L'élément récuperé.
		 */
		T pop()
		{
			T result = m_list[0];
			m_list.erase(m_list.begin());
			return result;
		}
		 
		/**
		 * Permet d'inserer un nouvel élément.
		 * @param element L'élément que l'on insère.
		 */
		void insert(T element)
		{
			m_list.push_back(element);
		}
		 
		/**
		 * Vide la file.
		 */
		void clear()
		{
			m_list.clear();
		}
		
		/**
		 * Permet de savoir si la file est vide ou non.
		 * @return Vrai si la file est vide, faux sinon.
		 */
		bool empty()
		{
			return m_list.empty();
		}
	protected:
	private:
		std::vector<T> m_list;
	};
}

#endif //__FIFO__