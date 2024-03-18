
#ifndef TILEMAP
#define TILEMAP

#include <SFML/Graphics.hpp>
#include "../elements/element.hpp"
class TileMap 
{
public:
    bool _traversable, _destructible;
    sf::Vector2f pos;
    sf::Texture textureTile;
    sf::Sprite spriteTile;
    TileMap(sf::Texture texture, float posX, float posY, bool passable, bool destructible);
    bool setUpSprite();
    bool getFree();

    bool isColision(Element);
    sf::Sprite &getSprite();

    public : 
        bool isTraversable();
        void setTraversable(bool traversable);
        bool isDestructible();
        void setDestructible(bool destructible);
};
#endif