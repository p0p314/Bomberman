#include "Server.hpp"
#include <iostream>
Server::Server() : _selector(), _playerList(new std::vector<Player*>())
{
    if(_listener.listen(2000, sf::IpAddress("192.168.1.2")) != sf::Socket::Done)
    {
        std::cout << "Impossible de creer le serveur" <<std::endl;
        exit(1);
    }
    _selector.add(_listener);
    _listener.setBlocking(false);
}

void Server::listen(std::vector<Player*>* playerList, std::mutex* mutex)
{
    
    _playerList = playerList;
    _mutex = mutex;
    std::cout<< "ecoute sur le port " << _listener.getLocalPort() <<std::endl;
    sf::Uint16 ID = 1;

    while ( _playerList->size() < 2)
    {
        std::cout<< "attente de connexion... " <<std::endl;
        if (_selector.wait())
        {
            std::cout<< "Nouvelle connexion detectee "  <<std::endl;

            sf::TcpSocket* socket = new sf::TcpSocket();
            if (_listener.accept(*socket) == sf::Socket::Done)
            {
                if(_playerList->size() > 2)
                {
                    socket->disconnect();
                    delete socket;
                    continue;
                }                
               
                Player* player = new Player();
                player->setSocket(socket);
                player->setID(ID++);
                player->setIp(socket->getRemoteAddress());
                std::unique_lock<std::mutex> lock(*_mutex);
                _playerList->push_back(player);
                _selector.add(*socket);
                 
                lock.unlock();
                std::cout << "Ajout reussi de " << socket->getRemoteAddress()<< std::endl;
            } else {
               // std::cout << "Erreur acceptation" << std::endl;
                std::cerr << "Erreur acceptation : " <<std::endl;
                delete socket;
            }
        }
    }
    while(true){
     getPacket();

    }                  //!Ajouter une dependance au nombre de joueur
}

std::vector<Player*>* Server::getPlayers()
{
    return _playerList;
}
sf::Uint16 Server::getID()
{
    return _ID;
}
       
sf::Uint16 Server::getDirection()
{
    return _dir;
}

void Server::shareToAllOthers(sf::Packet packet, Player * difuseur)
{
      std::cout << "serveur : attend"  << std::endl;
    if (_selector.wait(sf::microseconds(500))) {                 //!Si latence reduire la durée du wait (=) getPacket()
        for (auto it = _playerList->begin(); it != _playerList->end();) {
            Player* player = *it;
            
                sf::TcpSocket* socket = player->getSocket();
                std::cout << "serveur :Partage a :  "<< socket->getRemoteAddress()  << std::endl;
                if (_selector.isReady(*socket)) {
                    sf::Socket::Status status = socket->send(packet);
                    if (status == sf::Socket::Done) 
                        std::cout << "serveur : a envoye sur le socket de "<< socket->getRemoteAddress()  << std::endl;
                    else if(status == sf::Socket::Disconnected) {
                        // Le client s'est déconnecté
                       std::cout <<"share --->"<< socket->getRemoteAddress() << " c'est deconnecte" << std::endl;
                       _selector.remove(*socket);
                        delete player;
                        it = _playerList->erase(it);
                        continue;
                    } else {
                        std::cerr << "Erreur lors de l'envoi des données sur les sockets." << std::endl;
                    }
                }            
            ++it;
        }
    }
}

void Server::getPacket() {

    if (_selector.wait(sf::microseconds(500))) {
        for (auto it = _playerList->begin(); it != _playerList->end();) {
            Player* player = *it;
            sf::TcpSocket* socket = player->getSocket();
            //std::  cout << "serveur :err ";

            if (_selector.isReady(*socket)) {
                sf::Packet packet;
                
                sf::Socket::Status status = socket->receive(packet);
                
                if (status == sf::Socket::Done) {
                    if(packet >> _ID >> _dir){
                        std::cout << "serveur : packet recu de "<< _ID;
                        std::cout << " - option : " << _dir << std::endl;
                        shareToAllOthers(packet,player);
                    }
                } else if (status == sf::Socket::Disconnected) {
                    std::cout <<" getpacket() --> "<< socket->getRemoteAddress() << " c'est deconnecte" << std::endl;
                    _selector.remove(*socket);
                    delete player;
                    it = _playerList->erase(it);
                    continue;
                } else {
                    std::cerr << "Erreur lors de la reception des données sur le socket." << std::endl;
                }
            }
            
            ++it;
        }
    }
}




sf::Packet& operator >>(sf::Packet& packet, Server& server)
{
    return packet >> server._ID >> server._dir;
}