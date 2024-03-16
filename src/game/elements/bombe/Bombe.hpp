#ifndef BOMBERMAN_BOMBE_H
#define BOMBERMAN_BOMBE_H

#include "../element.hpp"


class Personnage; //! forward declaration, évite problème d'inclusion
class Monde;
class Bombe : public Element
{
private :

    Personnage & proprietaire;
    sf::Sprite spriteFlammes; 

    bool visible = false; 
    bool flammeVisible = false;
 

    int loadBombe(std::string);
    int loadFlammes();
    int degats = 1;
    int porteAttaque = 1;
    int delaiExplosion = 3; //TODO utiliser heure depart, heure fin pour calcul    
    
    sf::Texture textureFlammeCenter;
    sf::Texture textureFlammeUp;
    sf::Texture textureFlammeRight;
    sf::Texture textureFlammeDown;
    sf::Texture textureFlammeLeft;

    sf::Sprite spriteFlammeCenter;
    sf::Sprite spriteFlammeUp;
    sf::Sprite spriteFlammeRight;
    sf::Sprite spriteFlammeDown;
    sf::Sprite spriteFlammeLeft;
 
    sf::Vector2i anim;
    
public : 

    Bombe( Personnage& proprietaire);
    Bombe() = delete ; 
    
    bool poser(Monde);
    void exploser(int,int,int,int);
    bool flammesContains(Personnage perso);
    
    void ajoutPowerUp(); //TODO: ajouter un powerup en param

    bool est_visible() const;
    bool est_visibleFlammes();
    void toggleVisibilite(bool);
    void toggleVisibiliteFlammes(bool);

    sf::Vector2i& getAnim();
    
    sf::Sprite& getSPriteBombe() ;
    sf::Sprite& getSpriteFlammeCenter();
    sf::Sprite& getSpriteFlammeUp(); 
    sf::Sprite& getSpriteFlammeRight(); 
    sf::Sprite& getSpriteFlammeDown(); 
    sf::Sprite& getSpriteFlammeLeft();

};



#endif //BOMBERMAN_BOMBE_H
