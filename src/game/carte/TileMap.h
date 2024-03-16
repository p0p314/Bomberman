
#ifndef TILEMAP
#define TILEMAP

#include <SFML/Graphics.hpp>
#include "../elements/element.hpp"
class TileMap 
{
public:
    bool isFree, isDestructible;
    sf::Vector2f pos;
    sf::Texture textureTile;
    sf::Sprite spriteTile;
    TileMap(sf::Texture, float, float, bool,bool);
    bool setUpSprite();
    bool getFree();

    bool isColision(Element);
    sf::Sprite &getSprite();
};
#endif