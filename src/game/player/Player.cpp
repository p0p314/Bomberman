#include "Player.hpp"

Player::Player(){
    _client = new sf::TcpSocket();
    _client->setBlocking(false);
    _localAdress = sf::IpAddress::getLocalAddress();
   
}

Personnage * Player::getMyCharacter()
{
    return _myCharacter;
}

void Player::setIdType(idType type){
    _idType = type;
}

void Player::setIp(sf::IpAddress remoteIp)
{
    _localAdress = remoteIp;
}
sf::IpAddress Player::getPublicAdress()
{
    return _localAdress;
}

bool Player::isOwner()
{
   // std::cout << _publicAdress << " | " << sf::IpAddress::getLocalAddress();
    if(_localAdress ==  sf::IpAddress::getLocalAddress() ) return true;  //!Tester avec sf::IpAddress::getLocalAddress()
    else return false;
            
}

sf::TcpSocket * Player::getSocket() 
{
    return _client;
}

void Player::setSocket(sf::TcpSocket * socket)
{
    _client = socket;
}

bool Player::joinAGame()
{
    if(_client->connect(sf::IpAddress("127.0.0.1"), 2000) != sf::Socket::Done)
    {  
        std::cout << "player : connexion echouee" <<  std::endl;
        return false;
    }
    std::cout << "player : connexion reussi" <<  std::endl;
    return true;
}

void Player::addEvent(std::string charName, sf::Uint16 type)
{
    sf::Packet packet;
    
    packet << charName << type;
    if(_client->getRemoteAddress() != sf::IpAddress::None)
    {
        sf::Socket::Status status = _client->send(packet);
        if(status == sf::Socket::Done)
            std::cout<<"Envoye sur le port "<< _client->getRemotePort() <<std::endl;
    } else std::cout<<"Connexion au serveur perdue "<< _client->getLocalPort() <<std::endl;
    
}