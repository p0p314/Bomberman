#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "carte/Monde.h"
#include "elements/personnage/Personnage.hpp"
#include "server/Server.hpp"

#include <iostream>
#include <mutex>
#include <thread>
class Personnage;
class Server;
class Player;
class Partie
{
    private: 
        sf::RenderWindow * _window;
        std::string _codeGame;
        Server * _server;
        Monde *_level; 
        Player * _player;
        std::mutex * _mutex;
        std::vector<Personnage*> _characterList;

        bool _allowingMovement = false,
            _exit;

    public: 
        Partie(sf::RenderWindow * window);
        Partie(sf::RenderWindow * window, Player * creator);
        Partie(sf::RenderWindow * window, Player * joiner, sf::IpAddress server);
        std::vector<Personnage*> getCharacterList();
        Monde * getLevel();
        void startServer();
       //~Partie();
        int Run(); 
        void HandleEvents(sf::Event);
        void Update(float dt);
        void Draw();

};