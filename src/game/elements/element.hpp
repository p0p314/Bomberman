#ifndef __ELEMENT__
#define __ELEMENT__

#include<SFML/Graphics.hpp>

class Element 
{
    protected : 
        sf::Texture _texture;
        sf::Sprite _sprite; 
    
    public : 
        Element() ; 
    
        sf::Vector2f getPosition();
        void setPosition(int x, int y );
        void setPosition(sf::Vector2f nouvellePosition);
        void deplacer(sf::Vector2f nouvellePosition);
    
    
        sf::Sprite&  getSprite();
        sf::Texture& getTexture();
    

};

#endif