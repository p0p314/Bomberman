#include "Player.hpp"

Player::Player(){
    _client = new sf::TcpSocket();
    _client->setBlocking(false);
    _localAdress = sf::IpAddress::getLocalAddress();
   
}



void Player::setIp(sf::IpAddress remoteIp)
{
    _localAdress = remoteIp;
}
sf::IpAddress Player::getPublicAdress()
{
    return _localAdress;                //!Travailler avec publicAdress quand plusieurs joueurs
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

void Player::getPacket()
{
    sf::Packet packet;
    if(_client->receive(packet) == sf::Socket::Done){
        packet >> _charNameFromPacket >>_dirFromPacket;
    } else {
        _charNameFromPacket = "none";
        _dirFromPacket = 10;
    }
}

bool Player::joinAGame()
{
    if(_client->connect(sf::IpAddress("127.0.0.1"), 2000) != sf::Socket::Done)
    {  
        std::cout << "player : connexion echouee" <<  std::endl;
        return false;
    }
    return true;
}

std::string Player::getCharNameFromPacket()
{
    return _charNameFromPacket;
}

sf::Uint16 Player::getDirFromPacket()
{
    return _dirFromPacket;
}

void Player::addEvent(std::string charName, sf::Uint16 type)
{
    sf::Packet packet;
    
    packet << charName << type;
    if(_client->getRemoteAddress() != sf::IpAddress::None)
    {
       // std::cout<<"Player :Envoi du paquet  "<<std::endl;
        sf::Socket::Status status = _client->send(packet);
        if(status != sf::Socket::Done)
            std::cout<<"Player : erreur pendant l'envoi du paquet "<< _client->getRemoteAddress() <<std::endl;
    } else std::cout<<"Player : Connexion au serveur perdue pour "<< _client->getRemoteAddress() <<std::endl;
    
}