
#ifndef MONDE
#define MONDE

#include "TileMap.h"
#include <vector>
#include "../elements/bombe/Bombe.hpp"
#include <SFML/Graphics.hpp>


class Monde
{
    sf::Texture mur, grass, caisse, brick;
    sf::Vector2i playerPos;
    // sf::Vector2i enemyPos;
    void setUpInitialState();
    // void setUpEnemyPositions();
    void setUpTiles();

public:
    bool isColision(Element elm);
    void isDestruction(Bombe bombe);

    std::vector<std::vector<TileMap *>> tiles;
    int gridLength;
    Monde();
};

#endif