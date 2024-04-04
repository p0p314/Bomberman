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
public:
    enum skin : int
    {
        titi = 0,
        toto
    };

    Personnage() = delete;                               // Constructeur par défaut supprimé
    Personnage(Player *player, Monde *monde, skin skin); // Constructeur avec paramètres

    void setPlayer(Player *);                                                            // Méthode pour définir le joueur associé
    Bombe &getBombe();                                                                   // Méthode pour obtenir une référence à la bombe
    skin getSkin();                                                                      // Méthode pour obtenir le skin du personnage
    int getLives();                                                                      // Méthode pour obtenir le nombre de vies
    void startDeath();                                                                   // Méthode pour démarrer l'animation de mort
    void dying(float dt);                                                                // Méthode pour l'animation de mort
    void respawn();                                                                      // Méthode pour la réapparition du personnage
    sf::FloatRect getCollisionZone();                                                    // Méthode pour obtenir la zone de collision
    void updateCollisionZone();                                                          // Méthode pour mettre à jour la zone de collision
    void actions(sf::Event, float dt);                                                   // Méthode pour gérer les actions avec les événements
    std::string getName();                                                               // Méthode pour obtenir le nom du personnage
    bool owner(sf::Uint8 id);                                                            // Méthode pour vérifier le propriétaire
    Player *getOwner();                                                                  // Méthode pour obtenir le propriétaire
    void Update(float dt, int dirFromPaclet = 10);                                       // Méthode de mise à jour
    void updateAnimation();                                                              // Méthode pour mettre à jour l'animation
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override; // Méthode pour dessiner

private:
    Monde *_level;     // Pointeur vers le monde
    skin _skin;        // Skin du personnage
    Bombe _bomb;       // Bombe associée au personnage
    Player *_player;   // Joueur associé
    std::string _name; // Nom du personnage

    enum Dir
    {
        Up = 0,
        Right,
        Down,
        Left
    }; // Énumération pour les directions

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

    int _posSpriteDeathAnimation = 1; // Position du sprite pour l'animation de mort
    sf::Vector2i _posSpriteAnimation; // Position du sprite pour l'animation

    sf::Texture _textureAtlas; // Texture pour l'atlas du personnage
    sf::Sprite _deathSprite;   // Sprite pour l'animation de mort

    sf::Vector2f _spawnPos;       // Position de réapparition
    sf::FloatRect _collisionZone; // Zone de collision

    void move(sf::Vector2f movement); // Méthode pour déplacer le personnage

    int quantiteBombe = 2, // Quantité de bombes initiale
        bouclier = 0,      // Bouclier
        _lives = 3;        // Nombre de vies
};

#endif
