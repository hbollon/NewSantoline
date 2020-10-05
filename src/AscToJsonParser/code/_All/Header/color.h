#ifndef _COLOR_H_
#define _COLOR_H_

#include <iostream>
#include <vector>


/**
 * Definition de plsuieur couleur en tant que contantes
 */
#define ROUGE_FONCE Color(192, 57, 43)
#define ROUGE_CLAIR Color(231, 76, 60)
#define VIOLET_CLAIR Color(155, 89, 182)
#define VIOLET_FONCE Color(108, 52, 131)
#define BLEU_FONCE Color(36, 113, 163)
#define BLEU_CLAIR Color(52, 152, 219)
#define TURQUOISE_CLAIR Color(26, 188, 156)
#define TURQUOISE_FONCE Color(19, 141, 117)
#define JAUNE_CLAIR Color(241, 196, 15)
#define JAUNE_FONCE Color(243, 156, 18)
#define VERT_FONCE Color(39, 174, 96)
#define VERT_CLAIR Color(46, 204, 113)
#define ORANGE_CLAIR Color(230, 126, 34)
#define ORANGE_FONCE Color(211, 84, 0)
#define GRIS_CLAIR Color(149, 165, 166)
#define GRIS_FONCE Color(127, 140, 141)
#define NOIR Color(44, 62, 80)
#define BLANC Color(252, 252, 252)



/**
 * Représente une couleur en composante RGB comprise entre 0 et 255
 */
class Color {
    unsigned char m_r, m_g, m_b;

public:
    // CONSTRUCTOR

    /** Constructeur par défaut, avec comment couleur #ffffff (blanc) */
    Color();

    /**
     * Constructeur avec paramètre
     * @param red valeur de la composante rouge
     * @param green valeur de la composante verte
     * @param blue valeur de la composante bleu
     */
    Color(unsigned char red, unsigned char green, unsigned char blue);

    /**
     * Reference de la couleur rouge
     * @return reférence de la composante rouge
     */
    unsigned char &red();

    /**
     * Reference de la couleur verte
     * @return reférence de la composante verte
     */
    unsigned char &green();

    /**
     * Reference de la couleur bleu
     * @return reférence de la composante bleu
     */
    unsigned char &blue();

    /**
     * Valeur de la couleur rouge
     * @return valeur de la composante rouge
     */
    unsigned char red() const;

    /**
     * Valeur de la couleur verte
     * @return valeur de la composante verte
     */
    unsigned char green() const;

    /**
     * Valeur de la couleur bleu
     * @return valeur de la composante bleu
     */
    unsigned char blue() const;

    /**
     * Operateur d'addition pour des couleurs. Calcule modulaire, si une
     * composante dépasse 255 retourne à 0
     * @param c couleur avec laquel on va être additionné
     * @return la somme des couleurs
     */
    Color operator+(const Color &c) const;


    /**
    * Palette de couleurs
    */
    std::vector<Color> palette();


};

/**
 * Scribe d'une couleur.
 * @param os le flux de sortie
 * @param c la couleur que l'on souhaite écrire
 * @return le flux de sortie surlequel on a écrit
 */
std::ostream &operator<<(std::ostream &os, const Color &c);




#endif //_COLOR_H_
