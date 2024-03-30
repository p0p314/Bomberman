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
                  _typeOfPacket,
                  _playerDisconnected;
        
        std::string _errorMessage;

        sf::RenderWindow * _window;
        std::string _codeGame;
        Server * _server;
        Monde *_level; 
        Player * _player;
        std::mutex * _mutex;
        std::vector<std::pair<Player*, Personnage*>> * _playerList;
        std::vector<Personnage*> _characterList;
        std::thread * _gameServer;
        Lobby * _lobby;
    
        bool _allowingMovement = false,
             _exit = false,
             _exitToMenu = true,
             _creator = false;

        void disconnectFromServer();
        void HandleEvents(sf::Event);
        void Update(float dt);
        void Draw();

    public: 
        Partie() = delete;
        Partie(sf::RenderWindow * window, Player * creator);
        Partie(sf::RenderWindow * window, Player * joiner, sf::IpAddress server);
        ~Partie();

        std::vector<Personnage*> getCharacterList();
        Monde * getLevel();
        void startServer();
        
        int Run(); 
        

};