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
        void setPosition(sf::Vector2f nouvellePosition); // Changer la position du Sprite
        void deplacer(sf::Vector2f nouvellePosition);    // Déplacer l'élément

        sf::Sprite&  getSprite();
        sf::Texture& getTexture();
    

};

#endif