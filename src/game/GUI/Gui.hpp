#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "../elements/personnage/Personnage.hpp"

class Gui : public sf::Drawable
{


    public : 
        Gui();
        ~Gui();

        void init(sf::Font *, int windowSizeX, int windowSizeY);
        void updateStats(std::vector<Personnage*> * characters);
    
        void updateStats(int startCounter);

    private : 
        bool _isStartingGame = true,
             _isOnPause = false,
             _isEndGame = false;

         
        sf::RenderWindow * _window;

        sf::Text * _characterOneLives,
                 * _characterTwoLives,
                 * _startCounter,
                 * _textOfEndGame;
        int _victoryCounter = 0;
        int _windowWidth, _windowHeight;
        sf::RectangleShape * _background;
    private :
        virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const;
};