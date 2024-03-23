#pragma once

#include "../elements/personnage/Personnage.hpp"
#include "../server/Server.hpp"
#include <SFML/Graphics.hpp>

class Player
{
   private :
    sf::TcpSocket * _client;
    Personnage * _myCharacter;
    sf::IpAddress _localAdress;
    enum idType{PRIVATE , PUBLIC};
    idType _idType;

    public : 
        Personnage * getMyCharacter();
        Player();
        bool joinAGame();
        void setIdType(idType type);
        void setId(sf::String id);
        sf::IpAddress getPublicAdress();
        void setIp(sf::IpAddress);
        bool isOwner();
        void addEvent(std::string charName, sf::Uint16 type);

        sf::TcpSocket * getSocket() ;
        void setSocket(sf::TcpSocket*);
};