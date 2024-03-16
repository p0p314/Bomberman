//
// Created by raximex on 28/01/24.
//

#ifndef __BOMBERMAN_PERSONNAGE__
#define __BOMBERMAN_PERSONNAGE__

#include "../element.hpp"
#include "apparence.hpp"
#include "../bombe/Bombe.hpp"
#include "../../carte/Monde.h"
class Personnage : public Element
{
private : 
    std::string nom;
    apparence skin; 
    int quantiteBombe = 2;
    int bouclier = 0;
    int viesRestantes = 3;
    bool estVivant = true;
    Bombe bombe;
    sf::Vector2i animMort;

public : 
    enum Dir{Up, Right, Down, Left};
    sf::Vector2i anim;
    Personnage() = delete;
    Personnage(apparence apparence); //TODO: définir init pos au chargement de la map
    Bombe& getBombe();
    
    void mourir(sf::RenderWindow&);
    bool vivant();
    void resurection();
};


#endif //BOMBERMAN_PERSONNAGE_H
