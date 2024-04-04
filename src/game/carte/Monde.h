
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
    sf::Texture _wall, _grass, _box, _brick;
    int gridLength;
    void setUpInitialState();
    void setUpTiles(); // Initialisation des cases de la carte
    std::vector<std::pair<Personnage*,Bombe*>> _bombList;
    std::vector<std::vector<TileMap *>> _tiles;

public:
    Monde();
    ~Monde();
    void initialisation();                                    // Initialisation des textures de la carte
    bool isColision(Personnage *elm);                         // Fonction Détection de la collision entre un élément et une case
    void isDestroyed(Bombe bombe);                            // Fonction mettant à jour les cases détruites
    void destroy(sf::Texture newTexture, int posI, int posJ); // Modifie la valeur de la case détruite. Elle devient Traversable.
    int getGridLength();

    std::vector<std::vector<TileMap*>>& getTiles();
    std::vector<std::pair<Personnage*,Bombe*>>& getBombList();
};

#endif