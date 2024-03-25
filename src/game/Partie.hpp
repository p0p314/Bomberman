#pragma once

#include <SFML/Graphics.hpp>
#include "carte/Monde.h"
#include "elements/personnage/Personnage.hpp"
#include <iostream>
class Personnage;

class Partie
{
    private: 
        sf::RenderWindow * _window;
        std::string _codeGame;
        Monde *_level; 

        std::vector<Personnage*> _characterList;
        //std::vector<std::pair<Personnage*,Bombe*>> _bombList;
        bool _exit;
        bool _exitM;

    public: 
        Partie(sf::RenderWindow * window);
       //~Partie();
        int Run(); 
        void HandleEvents(sf::Event, bool AllowingMovement);
        void Update(float dt);
        void Draw();

};