
#ifndef TILEMAP
#define TILEMAP

#include <SFML/Graphics.hpp>
#include "../elements/element.hpp"
class TileMap 
{

    private :    
        bool _traversable, _destructible;
        sf::Vector2f pos;
        sf::Texture textureTile;
        sf::Sprite spriteTile;
    public:
        TileMap(sf::Texture texture, float posX, float posY, bool passable, bool destructible); // Définir la case, sa position si elle est traversable ou destructible.

        bool setUpSprite();

        sf::Sprite &getSprite();

        bool isTraversable();                  // Retourne une variable _traversable pour savoir si le bloc en question est traversable pour le joueur
        void setTraversable(bool traversable); // Définit les blocs traversables ou non
        bool isDestructible();                 // Définit si le bloc est destructible
        void setDestructible(bool destructible);
        bool containsCharacter(Element elm); // Si la case désigné a un élèment Personnage ou bombe dans sa zone. Sert à détecter les collisions
        bool intersects(sf::Sprite);         // Gestion collision
        bool intersects(sf::FloatRect);      // Gestion collision
};

#endif