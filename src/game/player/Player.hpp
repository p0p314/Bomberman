#pragma once

#include "../elements/personnage/Personnage.hpp"
#include "../server/Server.hpp" 
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
        sf::Uint8 getDirFromPacket();
        
        sf::Uint8 getID();
        void setID(sf::Uint8);

        sf::TcpSocket * getSocket() ;
        void setSocket(sf::TcpSocket*);
        
        bool joinAGame();

        void signalArrival(sf::Uint8 playerType, sf::Uint8 maxPlayers = 0);
        void listReady();
        void action(sf::Uint8 typeOfAction);
        void quiteGame();

 private :
    sf::TcpSocket * _client;
    sf::IpAddress _remoteIP;
    sf::Uint8 _ID;
   
    
    sf::Uint8 _dirFromPacket,
               _senderOfPacket;
};