#include "Server.hpp"
#include <iostream>
Server::Server() : _selector(), _playerList(new std::vector<Player*>())
{
    if(_listener.listen(2000, sf::IpAddress::Any) != sf::Socket::Done)
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
    while (_playerList->empty())
    {
        std::cout<< "attente de connexion... " <<std::endl;
        if (_selector.wait())
        {
            std::cout<< "Nouvelle connexion detectee "  <<std::endl;

            sf::TcpSocket* socket = new sf::TcpSocket();
            if (_listener.accept(*socket) == sf::Socket::Done)
            {
                
                Player* player = new Player();
                player->setSocket(socket);
               // player->setIp(socket->getRemoteAddress());
                std::unique_lock<std::mutex> lock(*_mutex);
                _playerList->push_back(player);
                _selector.add(*socket);
                lock.unlock();
                std::cout << "Ajout reussi de " << player->getPublicAdress()<< std::endl;
            } else {
                std::cout << "Erreur acceptation" << std::endl;
                delete socket;
            }
        }
    }
    while(true)
     getPacket();
}

std::vector<Player*>* Server::getPlayers()
{
    return _playerList;
}
std::string Server::getCharName()
{
    return _charName;
}
       
sf::Uint8 Server::getDirection()
{
    return _dir;
}


void Server::getPacket() {
    // Attendre que des données soient disponibles sur au moins un socket
    if (_selector.wait(sf::milliseconds(100))) {
        // Parcourir tous les joueurs connectés
        for (auto it = _playerList->begin(); it != _playerList->end();) {
            Player* player = *it;
            sf::TcpSocket* socket = player->getSocket();
            
            // Vérifier si le socket est prêt à être lu
            if (_selector.isReady(*socket)) {
                sf::Packet packet;
                
                // Recevoir les données du socket
                sf::Socket::Status status = socket->receive(packet);
                
                if (status == sf::Socket::Done) {
                    std::cout << "recu" <<std::endl;
                } else if (status == sf::Socket::Disconnected) {
                    // Le client s'est déconnecté
                    _selector.remove(*socket);
                    delete player;
                    it = _playerList->erase(it);
                    continue;
                } else {
                    // Erreur lors de la réception des données
                    std::cerr << "Erreur lors de la réception des données sur le socket." << std::endl;
                }
            }
            
            // Passer au joueur suivant
            ++it;
        }
    }
}




sf::Packet& operator >>(sf::Packet& packet, Server& server)
{
    return packet >> server._charName >> server._dir;
}