#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "carte/Monde.h"
#include "elements/personnage/Personnage.hpp"
#include "server/Server.hpp"
#include "lobby/Lobby.hpp"
#include <iostream>
#include <mutex>
#include <thread>

class Personnage;
class Server;
class Player;
class Lobby;
class Partie
{
    private: 
        sf::Uint8 _numberOfPlayer = 0,
                  _idPlayer,
                  _idSender,
                  _action,
                  _typeOfPacket,
                  _playerDisconnected,
                  _startGame;
        
        bool _synchronised;
        std::string _errorMessage;


        sf::RenderWindow * _window;
        Monde *_level; 
        Player * _player;
        Lobby * _lobby;
        
        std::vector<Personnage*> _characterList;
    
        bool _allowingMovement = false,
             _exit = false,
             _exitToMenu = true;

        void returnToMenu();
        void checkRecievedPacket(float dt = 0);
        void disconnectFromServer();
        void synchronisation();
        void HandleEvents(sf::Event);
        void Update(float dt);
        void Draw();

    public: 
        Partie() = delete;
        Partie(sf::RenderWindow * window, Player * joiner, sf::Uint8 typeOfplayer = 0);
        Partie(sf::RenderWindow * window, Player * joiner, sf::IpAddress server);
        ~Partie();

        std::vector<Personnage*> getCharacterList();
        Monde * getLevel();
        int Run(); 
        

};