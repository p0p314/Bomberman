#include "element.hpp"
#include <iostream>


Element::Element(){

}


void Element::setPosition(int x, int y)
{
    _sprite.setPosition(x,y);
    
}

void Element::setPosition(sf::Vector2f nouvellePosition)
{
    _sprite.setPosition(nouvellePosition);
  
}

void Element::deplacer(sf::Vector2f position)
{
    _sprite.move(position);
    
}


sf::Vector2f Element::getPosition(){
    return _sprite.getPosition();
}

sf::Sprite& Element::getSprite() {
    return _sprite;
}

sf::Texture& Element::getTexture(){
    return _texture;
}