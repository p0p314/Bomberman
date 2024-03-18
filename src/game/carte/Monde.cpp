#include "Monde.h"
#include <iostream>
#include <SFML/Graphics.hpp>

Monde::Monde()
{}

void Monde::initialisation()
{
    gridLength = 12;
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
    return tiles;
}

std::vector<std::pair<Personnage*,Bombe*>> & Monde::getBombList()
{
    return _bombList;
}

bool Monde::isColision(Personnage* elm)
{
    for (int i = 0; i < gridLength; i++)
    {
        for (int j = 0; j < gridLength; j++)
        {
            sf::FloatRect zoneCapture(sf::Rect<float>(elm->getPosition().x,
                                                      elm->getPosition().y + elm->getSprite().getGlobalBounds().height - 15,
                                                      elm->getSprite().getGlobalBounds().width,
                                                      15));
            if (tiles[i][j]->getSprite().getGlobalBounds().intersects(zoneCapture) && !tiles[i][j]->isTraversable())
            {
                return true;
            }
        }
    }
    return false;
}

void Monde::isDestruction(Bombe bombe)
{
    for (int i = 0; i < gridLength; i++)
    {
        for (int j = 0; j < gridLength; j++)
        {
            for(sf::Sprite ray : bombe.getRays())
                if(tiles[i][j]->getSprite().getGlobalBounds().intersects(ray.getGlobalBounds()) && tiles[i][j]->isDestructible()){
            /*if ((tiles[i][j]->getSprite().getGlobalBounds().intersects(bombe.getSpriteFlammeDown().getGlobalBounds()) ||
                 tiles[i][j]->getSprite().getGlobalBounds().intersects(bombe.getSpriteFlammeLeft().getGlobalBounds()) ||
                 tiles[i][j]->getSprite().getGlobalBounds().intersects(bombe.getSpriteFlammeUp().getGlobalBounds()) ||
                 tiles[i][j]->getSprite().getGlobalBounds().intersects(bombe.getSpriteFlammeRight().getGlobalBounds())) &&
                 tiles[i][j]->isDestructible())
            {*/
                std::cout << "\t tiles detruite -> x = "<< i << ", j = " <<j  << std::endl;
                tiles[i][j]->getSprite().setTexture(grass);
                tiles[i][j]->setTraversable(true);
                tiles[i][j]->setDestructible(false);
            }
        }
    }

}

void Monde::setUpTiles()
{
    tiles.clear();

    mur.loadFromFile("assets/mur.png");         //Faire une class/structure ? pour chaque composant
    grass.loadFromFile("assets/grass.png");     //Destructible ? traversable ? directement dans le type 
    caisse.loadFromFile("assets/terrain.png");  // de la tile
    brick.loadFromFile("assets/brick.png");

    std::vector<TileMap *> firstRow;
    firstRow.push_back(new TileMap(mur, 0, 0, false, false));
    firstRow.push_back(new TileMap(mur, 50, 0, false, false));
    firstRow.push_back(new TileMap(mur, 100, 0, false, false));
    firstRow.push_back(new TileMap(mur, 150, 0, false, false));
    firstRow.push_back(new TileMap(mur, 200, 0, false, false));
    firstRow.push_back(new TileMap(mur, 250, 0, false, false));
    firstRow.push_back(new TileMap(mur, 300, 0, false, false));
    firstRow.push_back(new TileMap(mur, 350, 0, false, false));
    firstRow.push_back(new TileMap(mur, 400, 0, false, false));
    firstRow.push_back(new TileMap(mur, 450, 0, false, false));
    firstRow.push_back(new TileMap(mur, 500, 0, false, false));
    firstRow.push_back(new TileMap(mur, 550, 0, false, false));
    tiles.push_back(firstRow);

    std::vector<TileMap *> secondRow;
    secondRow.push_back(new TileMap(mur, 0, 50, false, false));
    secondRow.push_back(new TileMap(grass, 50, 50, true, false));
    secondRow.push_back(new TileMap(grass, 100, 50, true, false));
    secondRow.push_back(new TileMap(grass, 150, 50, true, false));
    secondRow.push_back(new TileMap(grass, 200, 50, true, false));
    secondRow.push_back(new TileMap(brick, 250, 50, false, false));
    secondRow.push_back(new TileMap(caisse, 300, 50, false, true));
    secondRow.push_back(new TileMap(grass, 350, 50, true, false));
    secondRow.push_back(new TileMap(brick, 400, 50, false, false));
    secondRow.push_back(new TileMap(grass, 450, 50, false, false));
    secondRow.push_back(new TileMap(grass, 500, 50, true, false));
    secondRow.push_back(new TileMap(mur, 550, 50, false, false));
    tiles.push_back(secondRow);

    std::vector<TileMap *> thirdRow;
    thirdRow.push_back(new TileMap(mur, 0, 100, false, false));
    thirdRow.push_back(new TileMap(grass, 50, 100, true, false));
    thirdRow.push_back(new TileMap(grass, 100, 100, true, false));
    thirdRow.push_back(new TileMap(brick, 150, 100, false, false));
    thirdRow.push_back(new TileMap(caisse, 200, 100, false, true));
    thirdRow.push_back(new TileMap(brick, 250, 100, false, false));
    thirdRow.push_back(new TileMap(caisse, 300, 100, false, true));
    thirdRow.push_back(new TileMap(grass, 350, 100, true, true));
    thirdRow.push_back(new TileMap(caisse, 400, 100, false, true));
    thirdRow.push_back(new TileMap(brick, 450, 100, false, false));
    thirdRow.push_back(new TileMap(grass, 500, 100, true, false));
    thirdRow.push_back(new TileMap(mur, 550, 100, false, false));
    tiles.push_back(thirdRow);

    std::vector<TileMap *>
        fourthRow;
    fourthRow.push_back(new TileMap(mur, 0, 150, false, false));
    fourthRow.push_back(new TileMap(grass, 50, 150, true, false));
    fourthRow.push_back(new TileMap(grass, 100, 150, true, false));
    fourthRow.push_back(new TileMap(brick, 150, 150, false, false));
    fourthRow.push_back(new TileMap(grass, 200, 150, true, false));
    fourthRow.push_back(new TileMap(grass, 250, 150, true, false));
    fourthRow.push_back(new TileMap(caisse, 300, 150, false, true));
    fourthRow.push_back(new TileMap(grass, 350, 150, true, false));
    fourthRow.push_back(new TileMap(brick, 400, 150, false, false));
    fourthRow.push_back(new TileMap(grass, 450, 150, true, false));
    fourthRow.push_back(new TileMap(grass, 500, 150, true, false));
    fourthRow.push_back(new TileMap(mur, 550, 150, false, false));
    tiles.push_back(fourthRow);

    std::vector<TileMap *> fiveRow;
    fiveRow.push_back(new TileMap(mur, 0, 200, false, false));
    fiveRow.push_back(new TileMap(grass, 50, 200, true, false));
    fiveRow.push_back(new TileMap(brick, 100, 200, false, false));
    fiveRow.push_back(new TileMap(grass, 150, 200, true, false));
    fiveRow.push_back(new TileMap(grass, 200, 200, true, false));
    fiveRow.push_back(new TileMap(brick, 250, 200, false, false));
    fiveRow.push_back(new TileMap(grass, 300, 200, true, false));
    fiveRow.push_back(new TileMap(grass, 350, 200, true, false));
    fiveRow.push_back(new TileMap(grass, 400, 200, true, false));
    fiveRow.push_back(new TileMap(grass, 450, 200, true, false));
    fiveRow.push_back(new TileMap(grass, 500, 200, true, false));
    fiveRow.push_back(new TileMap(mur, 550, 200, false, false));
    tiles.push_back(fiveRow);

    std::vector<TileMap *> sixthRow;
    sixthRow.push_back(new TileMap(mur, 0, 250, false, false));
    sixthRow.push_back(new TileMap(grass, 50, 250, true, false));
    sixthRow.push_back(new TileMap(grass, 100, 250, true, false));
    sixthRow.push_back(new TileMap(grass, 150, 250, true, false));
    sixthRow.push_back(new TileMap(brick, 200, 250, false, false));
    sixthRow.push_back(new TileMap(grass, 250, 250, true, false));
    sixthRow.push_back(new TileMap(grass, 300, 250, true, false));
    sixthRow.push_back(new TileMap(grass, 350, 250, true, false));
    sixthRow.push_back(new TileMap(grass, 400, 250, true, false));
    sixthRow.push_back(new TileMap(grass, 450, 250, true, false));
    sixthRow.push_back(new TileMap(brick, 500, 250, false, false));
    sixthRow.push_back(new TileMap(mur, 550, 250, false, false));
    tiles.push_back(sixthRow);

    std::vector<TileMap *> seventhRow;
    seventhRow.push_back(new TileMap(mur, 0, 300, false, false));
    seventhRow.push_back(new TileMap(grass, 50, 300, true, false));
    seventhRow.push_back(new TileMap(grass, 100, 300, true, false));
    seventhRow.push_back(new TileMap(grass, 150, 300, true, false));
    seventhRow.push_back(new TileMap(grass, 200, 300, true, false));
    seventhRow.push_back(new TileMap(grass, 250, 300, true, false));
    seventhRow.push_back(new TileMap(brick, 300, 300, false, false));
    seventhRow.push_back(new TileMap(grass, 350, 300, true, false));
    seventhRow.push_back(new TileMap(grass, 400, 300, true, false));
    seventhRow.push_back(new TileMap(grass, 450, 300, true, false));
    seventhRow.push_back(new TileMap(grass, 500, 300, true, false));
    seventhRow.push_back(new TileMap(mur, 550, 300, false, false));
    tiles.push_back(seventhRow);

    std::vector<TileMap *> eight;
    eight.push_back(new TileMap(mur, 0, 350, false, false));
    eight.push_back(new TileMap(grass, 50, 350, true, false));
    eight.push_back(new TileMap(grass, 100, 350, true, false));
    eight.push_back(new TileMap(grass, 150, 350, true, false));
    eight.push_back(new TileMap(grass, 200, 350, true, false));
    eight.push_back(new TileMap(grass, 250, 350, true, false));
    eight.push_back(new TileMap(brick, 300, 350, false, false));
    eight.push_back(new TileMap(grass, 350, 350, true, false));
    eight.push_back(new TileMap(grass, 400, 350, true, false));
    eight.push_back(new TileMap(brick, 450, 350, false, false));
    eight.push_back(new TileMap(grass, 500, 350, true, false));
    eight.push_back(new TileMap(mur, 550, 350, false, false));
    tiles.push_back(eight);

    std::vector<TileMap *> nine;
    nine.push_back(new TileMap(mur, 0, 400, false, false));
    nine.push_back(new TileMap(grass, 50, 400, true, false));
    nine.push_back(new TileMap(grass, 100, 400, true, false));
    nine.push_back(new TileMap(brick, 150, 400, false, false));
    nine.push_back(new TileMap(grass, 200, 400, true, false));
    nine.push_back(new TileMap(grass, 250, 400, true, false));
    nine.push_back(new TileMap(grass, 300, 400, true, false));
    nine.push_back(new TileMap(grass, 350, 400, true, false));
    nine.push_back(new TileMap(brick, 400, 400, false, false));
    nine.push_back(new TileMap(grass, 450, 400, true, false));
    nine.push_back(new TileMap(grass, 500, 400, true, false));
    nine.push_back(new TileMap(mur, 550, 400, false, false));
    tiles.push_back(nine);

    std::vector<TileMap *> ten;
    ten.push_back(new TileMap(mur, 0, 450, false, false));
    ten.push_back(new TileMap(grass, 50, 450, true, false));
    ten.push_back(new TileMap(grass, 100, 450, true, false));
    ten.push_back(new TileMap(brick, 150, 450, false, false));
    ten.push_back(new TileMap(grass, 200, 450, true, false));
    ten.push_back(new TileMap(grass, 250, 450, true, false));
    ten.push_back(new TileMap(grass, 300, 450, true, false));
    ten.push_back(new TileMap(grass, 350, 450, true, false));
    ten.push_back(new TileMap(brick, 400, 450, false, false));
    ten.push_back(new TileMap(grass, 450, 450, true, false));
    ten.push_back(new TileMap(grass, 500, 450, true, false));
    ten.push_back(new TileMap(mur, 550, 450, false, false));
    tiles.push_back(ten);

    std::vector<TileMap *> eleven;
    eleven.push_back(new TileMap(mur, 0, 500, false, false));
    eleven.push_back(new TileMap(grass, 50, 500, true, false));
    eleven.push_back(new TileMap(grass, 100, 500, true, false));
    eleven.push_back(new TileMap(grass, 150, 500, true, false));
    eleven.push_back(new TileMap(brick, 200, 500, false, false));
    eleven.push_back(new TileMap(grass, 250, 500, true, false));
    eleven.push_back(new TileMap(grass, 300, 500, true, false));
    eleven.push_back(new TileMap(grass, 350, 500, true, false));
    eleven.push_back(new TileMap(grass, 400, 500, true, false));
    eleven.push_back(new TileMap(grass, 450, 500, true, false));
    eleven.push_back(new TileMap(grass, 500, 500, true, false));
    eleven.push_back(new TileMap(mur, 550, 500, false, false));
    tiles.push_back(eleven);

    std::vector<TileMap *> twelwe;
    twelwe.push_back(new TileMap(mur, 0, 550, false, false));
    twelwe.push_back(new TileMap(mur, 50, 550, false, false));
    twelwe.push_back(new TileMap(mur, 100, 550, false, false));
    twelwe.push_back(new TileMap(mur, 150, 550, false, false));
    twelwe.push_back(new TileMap(mur, 200, 550, false, false));
    twelwe.push_back(new TileMap(mur, 250, 550, false, false));
    twelwe.push_back(new TileMap(mur, 300, 550, false, false));
    twelwe.push_back(new TileMap(mur, 350, 550, false, false));
    twelwe.push_back(new TileMap(mur, 400, 550, false, false));
    twelwe.push_back(new TileMap(mur, 450, 550, false, false));
    twelwe.push_back(new TileMap(mur, 500, 550, false, false));
    twelwe.push_back(new TileMap(mur, 550, 550, false, false));

    tiles.push_back(twelwe);
}