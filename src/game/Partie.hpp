#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "carte/Monde.h"
#include "elements/personnage/Personnage.hpp"
#include "server/Server.hpp"
#include "lobby/Lobby.hpp"
#include "GUI/Gui.hpp"
#include <iostream>
#include <mutex>
#include <thread>

class Personnage;
class Server;
class Player;
class Lobby;
class Gui;
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
                  
        
        bool _synchronised = false,
             _forcedExit = false;

        std::string _errorMessage;


        sf::RenderWindow * _window;
        Monde *_level; 
        Player * _player;
        Lobby * _lobby;
        
        std::vector<Personnage*> _characterList;
    
        bool _exit = false,
             _exitToMenu = true,
             _startingGame,
             _endofGame = false;

        float _startingGameCounter =0.f,
              _timeToChangeCount = 1.f;

        int _gameCounter = 3;

        Gui * _gameInfo;

        void createCharacters();
        void returnToMenu();
        bool checkRecievedPacket(float dt = 0);
        
        void HandleEvents(sf::Event, float dt =0);
        void Update(float dt);
        void Draw();

    public: 
        Partie() = delete;
        Partie(sf::RenderWindow * window, Player * joiner, sf::Uint8 typeOfplayer = 0);
        ~Partie();

        std::vector<Personnage*> getCharacterList();
        Monde * getLevel();
        int Run(); 
        

};