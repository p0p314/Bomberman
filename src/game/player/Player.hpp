#pragma once

#include "../elements/personnage/Personnage.hpp"
#include "../server/Server.hpp"
#include <SFML/Graphics.hpp>

class Player
{
   private :
    sf::TcpSocket * _client;
    sf::IpAddress _localAdress;
   
    std::string _charNameFromPacket;
    sf::Uint16 _dirFromPacket;
    public : 
        Player();
        bool joinAGame();
        sf::IpAddress getPublicAdress();
        void setIp(sf::IpAddress);
        bool isOwner();
        void addEvent(std::string charName, sf::Uint16 type);

        void getPacket();
        std::string getCharNameFromPacket();
        sf::Uint16 getDirFromPacket();

        sf::TcpSocket * getSocket() ;
        void setSocket(sf::TcpSocket*);
};