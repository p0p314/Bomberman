#include "Player.hpp"

Player::Player() : _debouncer(0.25f)
{
    _client = new sf::TcpSocket();
 }

Player::Player(sf::TcpSocket * socket, sf::IpAddress IPadress) : _debouncer(0.25f)
{
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
        _client->setBlocking(true);
        auto status = _client->connect(sf::IpAddress("176.143.129.142"), 2000,sf::Time(sf::seconds(2)));
        if( status == sf::Socket::Done)
        {  
            std::cout << "player : connexion reussi" <<  std::endl;
            return true;
        } 
        else if( status == sf::Socket::Error)
        {
            std::cout << "player : connexion echouee" <<  std::endl;
        
        }
        return false;
    
}

sf::Uint8 Player::getSenderOfPacket()
{
    return _senderOfPacket;
}

sf::Uint8 Player::dirFromPacket()
{
    return _dirFromPacket;
}



bool Player::signalArrival(sf::Uint8 playerType, sf::Uint8 maxPlayers)
{
    sf::Packet  packet;
    sf::Uint8 newPlayer = 0;
    packet <<  newPlayer << playerType << maxPlayers;
    if(_client->send(packet) == sf::Socket::Done)
    {
        std::cout << "Arrivee signalee" <<  std::endl;
        return true;
    }
    else 
    {
        std::cout << "signal arrivee echoue" << std::endl; 
        return false;
    }
    
}
void Player::listReady()
{
    sf::Packet packet;
    packet << static_cast<sf::Uint8>(3);
    
    _client->setBlocking(true);
   
    if(_client->send(packet) == sf::Socket::Done)
        std::cout << "info liste prete envoye" << std::endl;
    else std::cout << "echec envoi info liste prete " << std::endl;
    
    _client->setBlocking(false);
}
void Player::exitGame(bool forcedExit)
{
    sf::Packet  packet;
    sf::Uint8 exitGame = 6;
    packet << exitGame;
    if(forcedExit) packet << sf::Uint8(1);
    else packet << sf::Uint8(0);

    if(_client->send(packet) == sf::Socket::Done)
    {
        std::cout << "paquet quiteGame envoye" << std::endl;
    }
    else 
    {
        std::cout << "paquet quiteGame echoue" << std::endl; 
    }

    _client->disconnect();
}

void Player::action(sf::Uint8 typeOfAction, float dt)
{
  

    if(_debouncer.update(dt)){
   
        sf::Packet packet;
        sf::Uint8 actionPacket = 5;

        packet << actionPacket << _ID << typeOfAction;
        if(_client->send(packet) == sf::Socket::Done)
        {
            std::cout << "paquet action envoye, mon id "<< static_cast<int>(_ID) << std::endl;
            if(typeOfAction != sf::Uint8(0)) _debouncer.trigger();
            numberOfActionSent++;
        }
        else  std::cout << "paquet action echoue" << std::endl; 
          
    }

    
}