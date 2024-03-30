#include "Player.hpp"

Player::Player(){
    _client = new sf::TcpSocket();
    //_client->setBlocking(false);
    
   
}

Player::Player(sf::TcpSocket * socket, sf::IpAddress IPadress){
    _client = socket;
    _remoteIP = IPadress;
    
   
}

Player::~Player(){}


sf::IpAddress Player::getPublicIP()
{
    return _remoteIP;                //!Travailler avec publicAdress quand plusieurs joueurs
}

void Player::setID(sf::Uint8 id)
{
    std::cout << id << std::endl;
    _ID = id;
}
sf::Uint8 Player::getID()
{
    return _ID;
}
bool Player::isOwner()
{
    std::cout << _remoteIP << " | " << sf::IpAddress::getPublicAddress() <<std::endl;
    
    if(_remoteIP ==  sf::IpAddress::getPublicAddress() ) return true; 
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

        if(_client->connect(sf::IpAddress("127.0.0.1"), 2000) == sf::Socket::Done)
        {  
            std::cout << "player : connexion reussi" <<  std::endl;
            return true;
        } else std::cout << "player : connexion echouee" <<  std::endl;

        return false;
    
}

sf::Uint8 Player::getSenderOfPacket()
{
    return _senderOfPacket;
}

sf::Uint8 Player::getDirFromPacket()
{
    return _dirFromPacket;
}

void Player::signalArrival(sf::Uint8 playerType, sf::Uint8 maxPlayers)
{
    sf::Packet  packet;
    sf::Uint8 newPlayer = 0;
    packet <<  newPlayer << playerType << maxPlayers;
    if(_client->send(packet) == sf::Socket::Done)
    {
        std::cout << "Arrivee signalee" <<  std::endl;
    }
    else 
    {
    
        std::cout << "signal arrivee echoue" << std::endl; 
    }
    
}
void Player::listReady(){}
void Player::quiteGame()
{
    sf::Packet  packet;
    sf::Uint8 quiteGame = 6;
    packet << quiteGame;
    if(_client->send(packet) == sf::Socket::Done)
    {
        std::cout << "paquet quiteGame envoye" << std::endl;
    }
    else 
    {
        std::cout << "paquet quiteGame echoue" << std::endl; 
    }
}

void Player::action(sf::Uint8 typeOfAction)
{
    sf::Packet packet;
    packet << static_cast<sf::Uint8>(5) <<typeOfAction;
    if(_client->send(packet) == sf::Socket::Done)
    {
        std::cout << "paquet action envoye" << std::endl;
    }
    else 
    {
        std::cout << "paquet action echoue" << std::endl; 
    }

    
}