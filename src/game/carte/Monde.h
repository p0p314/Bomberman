
#ifndef MONDE
#define MONDE

#include "TileMap.h"
#include <vector>
#include "../elements/bombe/Bombe.hpp"
#include "../elements/personnage/Personnage.hpp"

#include <SFML/Graphics.hpp>

class Personnage;
class Monde
{
 private:   
    sf::Texture mur, grass, caisse, brick;
    int gridLength;
    
    void setUpInitialState();
    void setUpTiles();
    
    std::vector<std::pair<Personnage*,Bombe*>> _bombList;
    std::vector<std::vector<TileMap *>> tiles;

public:
    Monde();
    void initialisation();

    bool isColision(Personnage* elm);
    void isDestroyed(Bombe bombe); 

    int getGridLength();

    std::vector<std::vector<TileMap*>>& getTiles();
    std::vector<std::pair<Personnage*,Bombe*>>& getBombList();
};

#endif