#ifndef BOMBERMAN_BOMBE_H
#define BOMBERMAN_BOMBE_H

#include "../element.hpp"


class Personnage; //! forward declaration, évite problème d'inclusion
class Monde;
class Bombe : public Element, public sf::Drawable
{
private :

    Personnage & _owner;

    bool _visibleBomb = false,
         _visibleFire = false,
         _exploded = false;
    
    int _posFireSpriteAnimation = 0,
        _posBombSpriteAnimation = 0,
        _fireSpriteWidth = 50,
        _fireSpriteHeight = 50;
     
    float _elapsedTime = 0.f,
          _elapsedTimeFireAnim = 0.f,
          _timeBeforeExplosion = 3.f,
          _timeEndOfExplosion = 5.f,
          _timeToChangeFrame = 0.0005f;

    int degats = 1,
        porteAttaque = 1;
    
    enum rayOrientation{Up, Right, Down, Left};

    sf::Texture _fireTexture;
    std::vector<sf::Sprite> _fireRays;

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
 
private : 
    int loadBomb(std::string);
    int loadFire();
    

    
public : 

    Bombe( Personnage& proprietaire);
    Bombe() = delete ; 
    
    bool plant(Monde *);

    void setVisibility(bool);
    void setFireVisibility(bool);
    void setExploded(bool);

    bool isVisible();
    bool isFireVisible();
    bool getIsExploded();
    
    bool flammesContains(Personnage perso);
    
    //void ajoutPowerUp(); //TODO: ajouter un powerup en param

    std::vector<sf::Sprite> getRays();
    sf::Sprite& getSPriteBombe() ;
    sf::Sprite& getSpriteFlammeCenter();
    sf::Sprite& getSpriteFlammeUp(); 
    sf::Sprite& getSpriteFlammeRight(); 
    sf::Sprite& getSpriteFlammeDown(); 
    sf::Sprite& getSpriteFlammeLeft();


  
    void Update(float dt);
    virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

};



#endif //BOMBERMAN_BOMBE_H
