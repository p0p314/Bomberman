#include "TileMap.h"
#include <iostream>

TileMap::TileMap(sf::Texture texture, float posX, float posY, bool passable, bool destructible)
{
    textureTile = texture;
    if (!setUpSprite())
    {
        return;
    }
    pos = sf::Vector2f(posX, posY);
    spriteTile.setPosition(pos);
    _traversable = passable;
    _destructible = destructible;
}

bool TileMap::setUpSprite()
{
    textureTile.setSmooth(true);
    spriteTile.setTexture(textureTile);
    spriteTile.setTextureRect(sf::IntRect(0, 0, 49, 49));
    return true;
}


bool TileMap::isDestructible()
{
    return _destructible;
}

void TileMap::setDestructible(bool destructible)
{
    _destructible = destructible;
}

bool TileMap::isTraversable()
{
    return _traversable;
}

void TileMap::setTraversable(bool traversable)
{
    _traversable = traversable;
}
sf::Sprite &TileMap::getSprite()
{
    return spriteTile;
}

bool TileMap::containsCharacter(Element  elm)
{
    return getSprite().getGlobalBounds().contains( 
        elm.getPosition().x + elm.getSprite().getGlobalBounds().width/2,
        elm.getPosition().y + elm.getSprite().getGlobalBounds().height);
   
}

bool TileMap::intersects(sf::Sprite elm)
{
    return getSprite().getGlobalBounds().intersects(elm.getGlobalBounds());
}


bool TileMap::intersects(sf::FloatRect rect)
{
    return getSprite().getGlobalBounds().intersects(rect);
}