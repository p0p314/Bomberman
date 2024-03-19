
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
        TileMap(sf::Texture texture, float posX, float posY, bool passable, bool destructible);
        
        bool setUpSprite();

        sf::Sprite &getSprite();

        bool isTraversable();
        void setTraversable(bool traversable);
        bool isDestructible();
        void setDestructible(bool destructible);
        bool containsCharacter(Element elm);
        bool intersects(sf::Sprite);
        bool intersects(sf::FloatRect);

};

#endif