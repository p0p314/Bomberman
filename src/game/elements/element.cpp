#include "element.hpp"
#include <iostream>


Element::Element(){

}


void Element::setPosition(int x, int y)
{
    sprite.setPosition(x,y);
    
}

void Element::setPosition(sf::Vector2f nouvellePosition)
{
    sprite.setPosition(nouvellePosition);
  
}

void Element::deplacer(sf::Vector2f position)
{
    sprite.move(position);
    
}


sf::Vector2f Element::getPosition(){
    return sprite.getPosition();
}

sf::Sprite& Element::getSprite() {
    return sprite;
}

sf::Texture& Element::getTexture(){
    return texture;
}