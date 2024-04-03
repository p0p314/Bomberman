#pragma once

#include "../elements/personnage/Personnage.hpp"
#include "../server/Server.hpp" 
#include "Debouncer.hpp"
#include <SFML/Graphics.hpp>

class Player
{
  
    public : 

        Player();
        Player(sf::TcpSocket * clientSocket, sf::IpAddress);
        ~Player();
        
        sf::IpAddress getPublicIP();
        
        bool isOwner();

        void getPacket();
        sf::Uint8 getSenderOfPacket();
        sf::Uint8 dirFromPacket();
        
        sf::Uint8 getID();
        void setID(sf::Uint8);

        sf::TcpSocket * getSocket() ;
        void setSocket(sf::TcpSocket*);
        
        bool joinAGame();
        bool signalArrival(sf::Uint8 playerType, sf::Uint8 maxPlayers = 0);
        void listReady();
        void action(sf::Uint8 typeOfAction, float dt);
        void exitGame(bool forcedExit);

        Debouncer & getDebouncer();
        int numberOfActionRecieved = 0;
        int  numberOfActionSent = 0;


 private :
    sf::TcpSocket * _client;
    sf::IpAddress _remoteIP;
    sf::Uint8 _ID;
   
   sf::Uint8 _numberOfPlayer = 0,
             _typeOfPacket,
             _playerDisconnected,
             _startGame;
    
    sf::Uint8 _dirFromPacket,
               _senderOfPacket;

    float _elapsedTimeKeyPressed = 0;
    float _timeBeforeNewAction = 0.250f;


    Debouncer _debouncer;
};