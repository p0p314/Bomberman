#pragma once

#include "../elements/personnage/Personnage.hpp"
#include "../server/Server.hpp"
#include <SFML/Graphics.hpp>

class Player
{
   private :
    sf::TcpSocket * _client;
    sf::IpAddress _remoteIP;
    sf::Uint16 _ID;
   
    
    sf::Uint16 _dirFromPacket,
               _senderOfPacket;
    public : 
        Player();
        Player(sf::TcpSocket * clientSocket, sf::IpAddress);
        ~Player();
        bool joinAGame();
        sf::IpAddress getPublicIP();
        void setIp(sf::IpAddress);
        bool isOwner();
        void addEvent(sf::Uint16 id, sf::Uint16 type);

        void getPacket();
        sf::Uint16 getSenderOfPacket();
        sf::Uint16 getDirFromPacket();
        
        sf::Uint16 getID();
        void setID(sf::Uint16);

        sf::TcpSocket * getSocket() ;
        void setSocket(sf::TcpSocket*);
};