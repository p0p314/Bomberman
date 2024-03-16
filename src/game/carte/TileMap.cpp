#include "TileMap.h"
#include <iostream>

TileMap::TileMap(sf::Texture texture, float x, float y, bool passable, bool destructible)
{
    textureTile = texture;
    if (!setUpSprite())
    {
        return;
    }
    pos = sf::Vector2f(x, y);
    spriteTile.setPosition(pos);
    isFree = passable;
    isDestructible = destructible;
}

bool TileMap::setUpSprite()
{
    textureTile.setSmooth(true);
    spriteTile.setTexture(textureTile);
    spriteTile.setTextureRect(sf::IntRect(0, 0, 49, 49));
    return true;
}



sf::Sprite &TileMap::getSprite()
{
    return spriteTile;
}

bool TileMap::getFree()
{
    return isFree;
}