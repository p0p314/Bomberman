#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
class Ray 
{
    public :
        enum orientation{center = 0, up,right,down,left};
        Ray(sf::Texture, orientation orientation);
       
        void updateRect(int posOnTexture);
        void updatePosition(sf::Sprite);
        bool contains(sf::Vector2f);

        bool getVisibility();
        void setVisibility(bool);

        sf::FloatRect getRect();
        sf::Sprite getSprite() const;

        sf::Sprite _sprite;
    private : 
        sf::Texture _texture;
        float _width = 50, 
              _height =50;
        bool _visible = false;
        orientation _orientation; 

};