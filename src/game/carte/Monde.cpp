#include "Monde.h"
#include <iostream>
#include <SFML/Graphics.hpp>

Monde::Monde()
{}

Monde::~Monde()
{

}
void Monde::initialisation()
{
    gridLength = 12;
    _wall.loadFromFile("assets/mur.png");         //Faire une class/structure ? pour chaque composant
    _grass.loadFromFile("assets/grass.png");     //Destructible ? traversable ? directement dans le type 
    _box.loadFromFile("assets/terrain.png");  // de la tile
    _brick.loadFromFile("assets/brick.png");
    setUpInitialState();
}
void Monde::setUpInitialState()
{
    setUpTiles();
}

int Monde::getGridLength()
{
    return gridLength;
}

std::vector<std::vector<TileMap*>>& Monde::getTiles() {
    return _tiles;
}

std::vector<std::pair<Personnage*,Bombe*>> & Monde::getBombList()
{
    return _bombList;
}

bool Monde::isColision(Personnage* elm)
{
    for (int i = 0; i < gridLength; i++)
        for (int j = 0; j < gridLength; j++){
            elm->updateCollisionZone();
            if (_tiles[i][j]->intersects(elm->getCollisionZone()) && !_tiles[i][j]->isTraversable()) 
                return true;
        }

    return false;
}

void Monde::destroy(sf::Texture newTexture ,int posI, int posJ )
{
    _tiles[posI][posJ]->getSprite().setTexture(_grass);
    _tiles[posI][posJ]->setTraversable(true);
    _tiles[posI][posJ]->setDestructible(false);
}

void Monde::isDestroyed(Bombe bomb)
{
    std::cout << "\t destruction level"  << std::endl;
    for (int i = 0; i < gridLength; i++)
        for (int j = 0; j < gridLength; j++)
            for(Ray * ray : bomb.get_Rays()){
                //!Herbe
                if(_tiles[i][j]->intersects(ray->getRect()) && _tiles[i][j]->isTraversable() && !_tiles[i][j]->isDestructible()) {    
                    ray->setVisibility(true);
                //!Caisse
                } else if(_tiles[i][j]->intersects(ray->getRect()) && _tiles[i][j]->isDestructible()){
                    ray->setVisibility(true);
                    destroy(_grass,i,j);
                //!Bordure, brique    
                } else if(_tiles[i][j]->intersects(ray->getRect()) && !_tiles[i][j]->isDestructible() && !_tiles[i][j]->isTraversable())
                    if(ray->getVisibility()) ray->setVisibility(false); 
            }    

}

void Monde::setUpTiles()
{
    _tiles.clear();

    //std::vector<std::vector<TileMap *>> _tiles;

std::vector<TileMap *> firstRow;
firstRow.push_back(new TileMap(_wall, 0, 0, false, false));
firstRow.push_back(new TileMap(_wall, 50, 0, false, false));
firstRow.push_back(new TileMap(_wall, 100, 0, false, false));
firstRow.push_back(new TileMap(_wall, 150, 0, false, false));
firstRow.push_back(new TileMap(_wall, 200, 0, false, false));
firstRow.push_back(new TileMap(_wall, 250, 0, false, false));
firstRow.push_back(new TileMap(_wall, 300, 0, false, false));
firstRow.push_back(new TileMap(_wall, 350, 0, false, false));
firstRow.push_back(new TileMap(_wall, 400, 0, false, false));
firstRow.push_back(new TileMap(_wall, 450, 0, false, false));
firstRow.push_back(new TileMap(_wall, 500, 0, false, false));
firstRow.push_back(new TileMap(_wall, 550, 0, false, false));
_tiles.push_back(firstRow);

std::vector<TileMap *> secondRow;
secondRow.push_back(new TileMap(_wall, 0, 50, false, false));
secondRow.push_back(new TileMap(_grass, 50, 50, true, false));
secondRow.push_back(new TileMap(_brick, 100, 50, false, false));
secondRow.push_back(new TileMap(_grass, 150, 50, true, false));
secondRow.push_back(new TileMap(_brick, 200, 50, false, false));
secondRow.push_back(new TileMap(_grass, 250, 50, true, false));
secondRow.push_back(new TileMap(_box, 300, 50, false, true));
secondRow.push_back(new TileMap(_grass, 350, 50, true, false));
secondRow.push_back(new TileMap(_brick, 400, 50, false, false));
secondRow.push_back(new TileMap(_grass, 450, 50, true, false));
secondRow.push_back(new TileMap(_brick, 500, 50, false, false));
secondRow.push_back(new TileMap(_wall, 550, 50, false, false));
_tiles.push_back(secondRow);

std::vector<TileMap *> thirdRow;
thirdRow.push_back(new TileMap(_wall, 0, 100, false, false));
thirdRow.push_back(new TileMap(_grass, 50, 100, true, false));
thirdRow.push_back(new TileMap(_brick, 100, 100, false, false));
thirdRow.push_back(new TileMap(_grass, 150, 100, true, false));
thirdRow.push_back(new TileMap(_grass, 200, 100, true, false));
thirdRow.push_back(new TileMap(_brick, 250, 100, false, false));
thirdRow.push_back(new TileMap(_box, 300, 100, false, true));
thirdRow.push_back(new TileMap(_grass, 350, 100, true, false));
thirdRow.push_back(new TileMap(_brick, 400, 100, false, false));
thirdRow.push_back(new TileMap(_grass, 450, 100, true, false));
thirdRow.push_back(new TileMap(_grass, 500, 100, true, false));
thirdRow.push_back(new TileMap(_wall, 550, 100, false, false));
_tiles.push_back(thirdRow);

std::vector<TileMap *> fourthRow;
fourthRow.push_back(new TileMap(_wall, 0, 150, false, false));
fourthRow.push_back(new TileMap(_grass, 50, 150, true, false));
fourthRow.push_back(new TileMap(_grass, 100, 150, true, false));
fourthRow.push_back(new TileMap(_brick, 150, 150, false, false));
fourthRow.push_back(new TileMap(_grass, 200, 150, true, false));
fourthRow.push_back(new TileMap(_grass, 250, 150, true, false));
fourthRow.push_back(new TileMap(_box, 300, 150, false, true));
fourthRow.push_back(new TileMap(_grass, 350, 150, true, false));
fourthRow.push_back(new TileMap(_brick, 400, 150, false, false));
fourthRow.push_back(new TileMap(_grass, 450, 150, true, false));
fourthRow.push_back(new TileMap(_grass, 500, 150, true, false));
fourthRow.push_back(new TileMap(_wall, 550, 150, false, false));
_tiles.push_back(fourthRow);

std::vector<TileMap *> fiveRow;
fiveRow.push_back(new TileMap(_wall, 0, 200, false, false));
fiveRow.push_back(new TileMap(_grass, 50, 200, true, false));
fiveRow.push_back(new TileMap(_brick, 100, 200, false, false));
fiveRow.push_back(new TileMap(_grass, 150, 200, true, false));
fiveRow.push_back(new TileMap(_grass, 200, 200, true, false));
fiveRow.push_back(new TileMap(_brick, 250, 200, false, false));
fiveRow.push_back(new TileMap(_grass, 300, 200, true, false));
fiveRow.push_back(new TileMap(_grass, 350, 200, true, false));
fiveRow.push_back(new TileMap(_grass, 400, 200, true, false));
fiveRow.push_back(new TileMap(_grass, 450, 200, true, false));
fiveRow.push_back(new TileMap(_grass, 500, 200, true, false));
fiveRow.push_back(new TileMap(_wall, 550, 200, false, false));
_tiles.push_back(fiveRow);

std::vector<TileMap *> sixthRow;
sixthRow.push_back(new TileMap(_wall, 0, 250, false, false));
sixthRow.push_back(new TileMap(_grass, 50, 250, true, false));
sixthRow.push_back(new TileMap(_brick, 100, 250, false, false));
sixthRow.push_back(new TileMap(_grass, 150, 250, true, false));
sixthRow.push_back(new TileMap(_grass, 200, 250, true, false));
sixthRow.push_back(new TileMap(_brick, 250, 250, false, false));
sixthRow.push_back(new TileMap(_grass, 300, 250, true, false));
sixthRow.push_back(new TileMap(_grass, 350, 250, true, false));
sixthRow.push_back(new TileMap(_grass, 400, 250, true, false));
sixthRow.push_back(new TileMap(_grass, 450, 250, true, false));
sixthRow.push_back(new TileMap(_brick, 500, 250, false, false));
sixthRow.push_back(new TileMap(_wall, 550, 250, false, false));
_tiles.push_back(sixthRow);

std::vector<TileMap *> seventhRow;
seventhRow.push_back(new TileMap(_wall, 0, 300, false, false));
seventhRow.push_back(new TileMap(_grass, 50, 300, true, false));
seventhRow.push_back(new TileMap(_grass, 100, 300, true, false));
seventhRow.push_back(new TileMap(_brick, 150, 300, false, false));
seventhRow.push_back(new TileMap(_grass, 200, 300, true, false));
seventhRow.push_back(new TileMap(_grass, 250, 300, true, false));
seventhRow.push_back(new TileMap(_box, 300, 300, false, true));
seventhRow.push_back(new TileMap(_grass, 350, 300, true, false));
seventhRow.push_back(new TileMap(_grass, 400, 300, true, false));
seventhRow.push_back(new TileMap(_grass, 450, 300, true, false));
seventhRow.push_back(new TileMap(_grass, 500, 300, true, false));
seventhRow.push_back(new TileMap(_wall, 550, 300, false, false));
_tiles.push_back(seventhRow);

std::vector<TileMap *> eight;
eight.push_back(new TileMap(_wall, 0, 350, false, false));
eight.push_back(new TileMap(_grass, 50, 350, true, false));
eight.push_back(new TileMap(_grass, 100, 350, true, false));
eight.push_back(new TileMap(_grass, 150, 350, true, false));
eight.push_back(new TileMap(_grass, 200, 350, true, false));
eight.push_back(new TileMap(_grass, 250, 350, true, false));
eight.push_back(new TileMap(_brick, 300, 350, false, false));
eight.push_back(new TileMap(_grass, 350, 350, true, false));
eight.push_back(new TileMap(_grass, 400, 350, true, false));
eight.push_back(new TileMap(_brick, 450, 350, false, false));
eight.push_back(new TileMap(_grass, 500, 350, true, false));
eight.push_back(new TileMap(_wall, 550, 350, false, false));
_tiles.push_back(eight);

std::vector<TileMap *> nine;
nine.push_back(new TileMap(_wall, 0, 400, false, false));
nine.push_back(new TileMap(_grass, 50, 400, true, false));
nine.push_back(new TileMap(_grass, 100, 400, true, false));
nine.push_back(new TileMap(_brick, 150, 400, false, false));
nine.push_back(new TileMap(_grass, 200, 400, true, false));
nine.push_back(new TileMap(_grass, 250, 400, true, false));
nine.push_back(new TileMap(_grass, 300, 400, true, false));
nine.push_back(new TileMap(_grass, 350, 400, true, false));
nine.push_back(new TileMap(_brick, 400, 400, false, false));
nine.push_back(new TileMap(_grass, 450, 400, true, false));
nine.push_back(new TileMap(_grass, 500, 400, true, false));
nine.push_back(new TileMap(_wall, 550, 400, false, false));
_tiles.push_back(nine);

std::vector<TileMap *> ten;
ten.push_back(new TileMap(_wall, 0, 450, false, false));
ten.push_back(new TileMap(_grass, 50, 450, true, false));
ten.push_back(new TileMap(_grass, 100, 450, true, false));
ten.push_back(new TileMap(_brick, 150, 450, false, false));
ten.push_back(new TileMap(_grass, 200, 450, true, false));
ten.push_back(new TileMap(_grass, 250, 450, true, false));
ten.push_back(new TileMap(_grass, 300, 450, true, false));
ten.push_back(new TileMap(_grass, 350, 450, true, false));
ten.push_back(new TileMap(_brick, 400, 450, false, false));
ten.push_back(new TileMap(_grass, 450, 450, true, false));
ten.push_back(new TileMap(_grass, 500, 450, true, false));
ten.push_back(new TileMap(_wall, 550, 450, false, false));
_tiles.push_back(ten);

std::vector<TileMap *> eleven;
eleven.push_back(new TileMap(_wall, 0, 500, false, false));
eleven.push_back(new TileMap(_grass, 50, 500, true, false));
eleven.push_back(new TileMap(_grass, 100, 500, true, false));
eleven.push_back(new TileMap(_grass, 150, 500, true, false));
eleven.push_back(new TileMap(_brick, 200, 500, false, false));
eleven.push_back(new TileMap(_grass, 250, 500, true, false));
eleven.push_back(new TileMap(_grass, 300, 500, true, false));
eleven.push_back(new TileMap(_grass, 350, 500, true, false));
eleven.push_back(new TileMap(_grass, 400, 500, true, false));
eleven.push_back(new TileMap(_grass, 450, 500, true, false));
eleven.push_back(new TileMap(_grass, 500, 500, true, false));
eleven.push_back(new TileMap(_wall, 550, 500, false, false));
_tiles.push_back(eleven);

std::vector<TileMap *> twelwe;
twelwe.push_back(new TileMap(_wall, 0, 550, false, false));
twelwe.push_back(new TileMap(_wall, 50, 550, false, false));
twelwe.push_back(new TileMap(_wall, 100, 550, false, false));
twelwe.push_back(new TileMap(_wall, 150, 550, false, false));
twelwe.push_back(new TileMap(_wall, 200, 550, false, false));
twelwe.push_back(new TileMap(_wall, 250, 550, false, false));
twelwe.push_back(new TileMap(_wall, 300, 550, false, false));
twelwe.push_back(new TileMap(_wall, 350, 550, false, false));
twelwe.push_back(new TileMap(_wall, 400, 550, false, false));
twelwe.push_back(new TileMap(_wall, 450, 550, false, false));
twelwe.push_back(new TileMap(_wall, 500, 550, false, false));
twelwe.push_back(new TileMap(_wall, 550, 550, false, false));
_tiles.push_back(twelwe);
   /* std::vector<TileMap *> firstRow;
    firstRow.push_back(new TileMap(_wall, 0, 0, false, false));
    firstRow.push_back(new TileMap(_wall, 50, 0, false, false));
    firstRow.push_back(new TileMap(_wall, 100, 0, false, false));
    firstRow.push_back(new TileMap(_wall, 150, 0, false, false));
    firstRow.push_back(new TileMap(_wall, 200, 0, false, false));
    firstRow.push_back(new TileMap(_wall, 250, 0, false, false));
    firstRow.push_back(new TileMap(_wall, 300, 0, false, false));
    firstRow.push_back(new TileMap(_wall, 350, 0, false, false));
    firstRow.push_back(new TileMap(_wall, 400, 0, false, false));
    firstRow.push_back(new TileMap(_wall, 450, 0, false, false));
    firstRow.push_back(new TileMap(_wall, 500, 0, false, false));
    firstRow.push_back(new TileMap(_wall, 550, 0, false, false));
    _tiles.push_back(firstRow);

    std::vector<TileMap *> secondRow;
    secondRow.push_back(new TileMap(_wall, 0, 50, false, false));
    secondRow.push_back(new TileMap(_grass, 50, 50, true, false));
    secondRow.push_back(new TileMap(_grass, 100, 50, true, false));
    secondRow.push_back(new TileMap(_grass, 150, 50, true, false));
    secondRow.push_back(new TileMap(_grass, 200, 50, true, false));
    secondRow.push_back(new TileMap(_brick, 250, 50, false, false));
    secondRow.push_back(new TileMap(_box, 300, 50, false, true));
    secondRow.push_back(new TileMap(_grass, 350, 50, true, false));
    secondRow.push_back(new TileMap(_brick, 400, 50, false, false));
    secondRow.push_back(new TileMap(_grass, 450, 50, false, false));
    secondRow.push_back(new TileMap(_grass, 500, 50, true, false));
    secondRow.push_back(new TileMap(_wall, 550, 50, false, false));
    _tiles.push_back(secondRow);

    std::vector<TileMap *> thirdRow;
    thirdRow.push_back(new TileMap(_wall, 0, 100, false, false));
    thirdRow.push_back(new TileMap(_grass, 50, 100, true, false));
    thirdRow.push_back(new TileMap(_grass, 100, 100, true, false));
    thirdRow.push_back(new TileMap(_brick, 150, 100, false, false));
    thirdRow.push_back(new TileMap(_box, 200, 100, false, true));
    thirdRow.push_back(new TileMap(_brick, 250, 100, false, false));
    thirdRow.push_back(new TileMap(_box, 300, 100, false, true));
    thirdRow.push_back(new TileMap(_grass, 350, 100, true, true));
    thirdRow.push_back(new TileMap(_box, 400, 100, false, true));
    thirdRow.push_back(new TileMap(_brick, 450, 100, false, false));
    thirdRow.push_back(new TileMap(_grass, 500, 100, true, false));
    thirdRow.push_back(new TileMap(_wall, 550, 100, false, false));
    _tiles.push_back(thirdRow);

    std::vector<TileMap *>
        fourthRow;
    fourthRow.push_back(new TileMap(_wall, 0, 150, false, false));
    fourthRow.push_back(new TileMap(_grass, 50, 150, true, false));
    fourthRow.push_back(new TileMap(_grass, 100, 150, true, false));
    fourthRow.push_back(new TileMap(_brick, 150, 150, false, false));
    fourthRow.push_back(new TileMap(_grass, 200, 150, true, false));
    fourthRow.push_back(new TileMap(_grass, 250, 150, true, false));
    fourthRow.push_back(new TileMap(_box, 300, 150, false, true));
    fourthRow.push_back(new TileMap(_grass, 350, 150, true, false));
    fourthRow.push_back(new TileMap(_brick, 400, 150, false, false));
    fourthRow.push_back(new TileMap(_grass, 450, 150, true, false));
    fourthRow.push_back(new TileMap(_grass, 500, 150, true, false));
    fourthRow.push_back(new TileMap(_wall, 550, 150, false, false));
    _tiles.push_back(fourthRow);

    std::vector<TileMap *> fiveRow;
    fiveRow.push_back(new TileMap(_wall, 0, 200, false, false));
    fiveRow.push_back(new TileMap(_grass, 50, 200, true, false));
    fiveRow.push_back(new TileMap(_brick, 100, 200, false, false));
    fiveRow.push_back(new TileMap(_grass, 150, 200, true, false));
    fiveRow.push_back(new TileMap(_grass, 200, 200, true, false));
    fiveRow.push_back(new TileMap(_brick, 250, 200, false, false));
    fiveRow.push_back(new TileMap(_grass, 300, 200, true, false));
    fiveRow.push_back(new TileMap(_grass, 350, 200, true, false));
    fiveRow.push_back(new TileMap(_grass, 400, 200, true, false));
    fiveRow.push_back(new TileMap(_grass, 450, 200, true, false));
    fiveRow.push_back(new TileMap(_grass, 500, 200, true, false));
    fiveRow.push_back(new TileMap(_wall, 550, 200, false, false));
    _tiles.push_back(fiveRow);

    std::vector<TileMap *> sixthRow;
    sixthRow.push_back(new TileMap(_wall, 0, 250, false, false));
    sixthRow.push_back(new TileMap(_grass, 50, 250, true, false));
    sixthRow.push_back(new TileMap(_grass, 100, 250, true, false));
    sixthRow.push_back(new TileMap(_grass, 150, 250, true, false));
    sixthRow.push_back(new TileMap(_brick, 200, 250, false, false));
    sixthRow.push_back(new TileMap(_grass, 250, 250, true, false));
    sixthRow.push_back(new TileMap(_grass, 300, 250, true, false));
    sixthRow.push_back(new TileMap(_grass, 350, 250, true, false));
    sixthRow.push_back(new TileMap(_grass, 400, 250, true, false));
    sixthRow.push_back(new TileMap(_grass, 450, 250, true, false));
    sixthRow.push_back(new TileMap(_brick, 500, 250, false, false));
    sixthRow.push_back(new TileMap(_wall, 550, 250, false, false));
    _tiles.push_back(sixthRow);

    std::vector<TileMap *> seventhRow;
    seventhRow.push_back(new TileMap(_wall, 0, 300, false, false));
    seventhRow.push_back(new TileMap(_grass, 50, 300, true, false));
    seventhRow.push_back(new TileMap(_grass, 100, 300, true, false));
    seventhRow.push_back(new TileMap(_grass, 150, 300, true, false));
    seventhRow.push_back(new TileMap(_grass, 200, 300, true, false));
    seventhRow.push_back(new TileMap(_grass, 250, 300, true, false));
    seventhRow.push_back(new TileMap(_brick, 300, 300, false, false));
    seventhRow.push_back(new TileMap(_grass, 350, 300, true, false));
    seventhRow.push_back(new TileMap(_grass, 400, 300, true, false));
    seventhRow.push_back(new TileMap(_grass, 450, 300, true, false));
    seventhRow.push_back(new TileMap(_grass, 500, 300, true, false));
    seventhRow.push_back(new TileMap(_wall, 550, 300, false, false));
    _tiles.push_back(seventhRow);

    std::vector<TileMap *> eight;
    eight.push_back(new TileMap(_wall, 0, 350, false, false));
    eight.push_back(new TileMap(_grass, 50, 350, true, false));
    eight.push_back(new TileMap(_grass, 100, 350, true, false));
    eight.push_back(new TileMap(_grass, 150, 350, true, false));
    eight.push_back(new TileMap(_grass, 200, 350, true, false));
    eight.push_back(new TileMap(_grass, 250, 350, true, false));
    eight.push_back(new TileMap(_brick, 300, 350, false, false));
    eight.push_back(new TileMap(_grass, 350, 350, true, false));
    eight.push_back(new TileMap(_grass, 400, 350, true, false));
    eight.push_back(new TileMap(_brick, 450, 350, false, false));
    eight.push_back(new TileMap(_grass, 500, 350, true, false));
    eight.push_back(new TileMap(_wall, 550, 350, false, false));
    _tiles.push_back(eight);

    std::vector<TileMap *> nine;
    nine.push_back(new TileMap(_wall, 0, 400, false, false));
    nine.push_back(new TileMap(_grass, 50, 400, true, false));
    nine.push_back(new TileMap(_grass, 100, 400, true, false));
    nine.push_back(new TileMap(_brick, 150, 400, false, false));
    nine.push_back(new TileMap(_grass, 200, 400, true, false));
    nine.push_back(new TileMap(_grass, 250, 400, true, false));
    nine.push_back(new TileMap(_grass, 300, 400, true, false));
    nine.push_back(new TileMap(_grass, 350, 400, true, false));
    nine.push_back(new TileMap(_brick, 400, 400, false, false));
    nine.push_back(new TileMap(_grass, 450, 400, true, false));
    nine.push_back(new TileMap(_grass, 500, 400, true, false));
    nine.push_back(new TileMap(_wall, 550, 400, false, false));
    _tiles.push_back(nine);

    std::vector<TileMap *> ten;
    ten.push_back(new TileMap(_wall, 0, 450, false, false));
    ten.push_back(new TileMap(_grass, 50, 450, true, false));
    ten.push_back(new TileMap(_grass, 100, 450, true, false));
    ten.push_back(new TileMap(_brick, 150, 450, false, false));
    ten.push_back(new TileMap(_grass, 200, 450, true, false));
    ten.push_back(new TileMap(_grass, 250, 450, true, false));
    ten.push_back(new TileMap(_grass, 300, 450, true, false));
    ten.push_back(new TileMap(_grass, 350, 450, true, false));
    ten.push_back(new TileMap(_brick, 400, 450, false, false));
    ten.push_back(new TileMap(_grass, 450, 450, true, false));
    ten.push_back(new TileMap(_grass, 500, 450, true, false));
    ten.push_back(new TileMap(_wall, 550, 450, false, false));
    _tiles.push_back(ten);

    std::vector<TileMap *> eleven;
    eleven.push_back(new TileMap(_wall, 0, 500, false, false));
    eleven.push_back(new TileMap(_grass, 50, 500, true, false));
    eleven.push_back(new TileMap(_grass, 100, 500, true, false));
    eleven.push_back(new TileMap(_grass, 150, 500, true, false));
    eleven.push_back(new TileMap(_brick, 200, 500, false, false));
    eleven.push_back(new TileMap(_grass, 250, 500, true, false));
    eleven.push_back(new TileMap(_grass, 300, 500, true, false));
    eleven.push_back(new TileMap(_grass, 350, 500, true, false));
    eleven.push_back(new TileMap(_grass, 400, 500, true, false));
    eleven.push_back(new TileMap(_grass, 450, 500, true, false));
    eleven.push_back(new TileMap(_grass, 500, 500, true, false));
    eleven.push_back(new TileMap(_wall, 550, 500, false, false));
    _tiles.push_back(eleven);

    std::vector<TileMap *> twelwe;
    twelwe.push_back(new TileMap(_wall, 0, 550, false, false));
    twelwe.push_back(new TileMap(_wall, 50, 550, false, false));
    twelwe.push_back(new TileMap(_wall, 100, 550, false, false));
    twelwe.push_back(new TileMap(_wall, 150, 550, false, false));
    twelwe.push_back(new TileMap(_wall, 200, 550, false, false));
    twelwe.push_back(new TileMap(_wall, 250, 550, false, false));
    twelwe.push_back(new TileMap(_wall, 300, 550, false, false));
    twelwe.push_back(new TileMap(_wall, 350, 550, false, false));
    twelwe.push_back(new TileMap(_wall, 400, 550, false, false));
    twelwe.push_back(new TileMap(_wall, 450, 550, false, false));
    twelwe.push_back(new TileMap(_wall, 500, 550, false, false));
    twelwe.push_back(new TileMap(_wall, 550, 550, false, false));

    _tiles.push_back(twelwe);
    */
}