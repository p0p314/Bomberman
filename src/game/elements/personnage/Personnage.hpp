//
// Created by raximex on 28/01/24.
//

#ifndef __BOMBERMAN_PERSONNAGE__
#define __BOMBERMAN_PERSONNAGE__

#include "../element.hpp"
#include "../bombe/Bombe.hpp"
#include "../../carte/Monde.h"
#include <iostream>
#include "../../server/Server.hpp"
#include "../../player/Player.hpp"

/**
 * La classe Personnage a un pointeur vers Monde 
 * car chaque personnage existe dans un monde particulier. 
 * En utilisant un pointeur vers Monde, chaque personnage peut accéder 
 * à son monde et interagir avec les autres éléments du monde
*/
class Monde;
class Player;
class Personnage : public Element, public sf::Drawable
{
public : 
   
    enum skin : int
    {
        titi = 0,
        toto
    };

    Personnage() = delete;
    Personnage(Player * player, Monde *monde, skin skin); //TODO: définir init pos au chargement de la map
    void setPlayer(Player *);
    Bombe & getBombe();
    
    skin getSkin();
    
    int getLives();
    void startDeath();
    void dying(float dt);
    void respawn();
   

    sf::FloatRect getCollisionZone();
    void updateCollisionZone();
    void actions(sf::Event, float dt);

    std::string getName();
    bool owner(sf::Uint8 id);
    Player * getOwner();
    void Update(float dt, int dirFromPaclet = 10);
    void updateAnimation();
    virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
    
private : 

    Monde * _level;
    skin _skin; 
    Bombe  _bomb;       
    Player * _player;
    std::string _name;
    
    enum Dir{Up = 0, Right, Down, Left};
    
    int _spriteWidth = 16,
        _spriteHeight = 24;

    int _speed = 320;
    bool _moving = false,
         _bombInHand = true,
         _alive = true,
         _dying = false;

    float _elapsedTime = 0.f,
          _elaspsedTimeDeath = 0.f, 
	      _timeToChangeFrame = 0.170f;

    
    

   
    int _posSpriteDeathAnimation = 1;
    sf::Vector2i _posSpriteAnimation;
    
    sf::Texture _textureAtlas;
    sf::Sprite _deathSprite;

    sf::Vector2f _spawnPos;
    sf::FloatRect _collisionZone;

    void move(sf::Vector2f movement);
    
    int quantiteBombe = 2,
        bouclier = 0,
        _lives = 3;

};



#endif //BOMBERMAN_PERSONNAGE_H
