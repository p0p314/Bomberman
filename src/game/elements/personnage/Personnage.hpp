//
// Created by raximex on 28/01/24.
//

#ifndef __BOMBERMAN_PERSONNAGE__
#define __BOMBERMAN_PERSONNAGE__

#include "../element.hpp"
#include "apparence.hpp"
#include "../bombe/Bombe.hpp"
#include "../../carte/Monde.h"
#include <iostream>


/**
 * La classe Personnage a un pointeur vers Monde 
 * car chaque personnage existe dans un monde particulier. 
 * En utilisant un pointeur vers Monde, chaque personnage peut accéder 
 * à son monde et interagir avec les autres éléments du monde
*/
class Monde;

class Personnage : public Element, public sf::Drawable
{
private : 

    int _spriteWidth = 16,
        _spriteHeight = 24;

    Monde * _level;
    std::string nom;
    apparence skin; 
    int _speed = 2;
    bool _bombInHand = true;

    float _elapsedTime = 0.f,
          _elaspsedTimeDeath = 0.f, 
	      _timeToChangeFrame = 0.03f;

    int quantiteBombe = 2;
    int bouclier = 0;
    int viesRestantes = 3;
    bool _alive = true;
    bool _dying = false;
    Bombe  _bomb;       //! Faire un pointeur si probleme affichage
    int _posSpriteDeathAnimation = 1;
    sf::Texture textureAtlas;
    sf::Sprite _deathSprite;
public : 
    enum Dir{Up, Right, Down, Left};
    sf::Vector2i _posSpriteAnimation;
    Personnage() = delete;
    Personnage(Monde *monde, apparence apparence); //TODO: définir init pos au chargement de la map
    Bombe & getBombe();
    int getAnimMort();
    void mourir(float dt);
    bool vivant();
    bool isDying();
    void demarrerMort();
    void resurection();

    sf::Sprite& getSpriteMort();


    void actions(sf::Event);
    int getNumberBombLeft();
    bool isBombInHand();
    void bombCollied();

    void Update(float dt);
    virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
};



#endif //BOMBERMAN_PERSONNAGE_H
