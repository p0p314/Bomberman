#include "Player.hpp"

Player::Player(){
    _client = new sf::TcpSocket();
    _client->setBlocking(false);
    
   
}



void Player::setIp(sf::IpAddress remoteIp)
{
    _IP = remoteIp;
}
sf::IpAddress Player::getPublicIP()
{
    return _IP;                //!Travailler avec publicAdress quand plusieurs joueurs
}

void Player::setID(sf::Uint16 id)
{
    std::cout << id << std::endl;
    _ID = id;
}
sf::Uint16 Player::getID()
{
    return _ID;
}
bool Player::isOwner()
{
    std::cout << _IP << " | " << sf::IpAddress::getPublicAddress() <<std::endl;
    
    if(_IP ==  sf::IpAddress::getPublicAddress() ) return true; 
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
    //std::cout << "player : verifie reception" <<  std::endl;
    if(_client->receive(packet) == sf::Socket::Done){
        packet >> _senderOfPacket >>_dirFromPacket;
        std::cout << _senderOfPacket << " " << _dirFromPacket << std::endl;
    } else {
           // std::cout << "player : verifie reception" <<  std::endl;

        _senderOfPacket = 0;
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

sf::Uint16 Player::getSenderOfPacket()
{
    return _senderOfPacket;
}

sf::Uint16 Player::getDirFromPacket()
{
    return _dirFromPacket;
}

void Player::addEvent(sf::Uint16 id, sf::Uint16 type)
{
    sf::Packet packet;
    std::cout<<id <<" " << type<<std::endl;
    packet << id << type;
    if(_client->getRemoteAddress() != sf::IpAddress::None)
    {
       // std::cout<<"Player :Envoi du paquet  "<<std::endl;
        sf::Socket::Status status = _client->send(packet);
        if(status == sf::Socket::Done) 
            //std::cout<<"Player : envoi du paquet reussi "<< _client->getRemoteAddress() <<std::endl;
        if(status != sf::Socket::Done)
            std::cout<<"Player : erreur pendant l'envoi du paquet "<< _client->getRemoteAddress() <<std::endl;
    } else std::cout<<"Player : Connexion au serveur perdue pour "<< _client->getRemoteAddress() <<std::endl;
    
}