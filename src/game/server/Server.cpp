#include "Server.hpp"
#include <iostream>
Server::Server() : _selector()
{
    if (_listener.listen(2000, sf::IpAddress("127.0.0.1")) != sf::Socket::Done)
    {
        std::cout << "Impossible de creer le serveur" <<std::endl;
        exit(1);
    }
    _selector.add(_listener);
    _listener.setBlocking(false); //! Verifier si les attentes se font bien pour reception de paquet client
    
}

Server::~Server()
{
    if(!_playerList->empty())
        for(std::pair  pair : *_playerList)  
            delete pair.first;        
    
    delete _mutex;
}

void Server::run()
{
    //! Boucle permanente
    while(_serverOpen)
    {
        std::cout<< "Ecoute sur le port " << _listener.getLocalPort() <<std::endl;

        _endOfGame = false;
        _inLobby = true;
        //!Boucle de jeu
        while(!_endOfGame)
        {                
            std::cout<< "Cree une nouvelle partie "<<std::endl;
            while(_inLobby)
            {
                std::cout<< "Attend joueur dans lobby "<<std::endl;
                if(_selector.wait())
                {
                    if(_selector.isReady(_listener))
                    {
                        sf::TcpSocket * client = new sf::TcpSocket();
                        sf::Packet packet;

                        if(_listener.accept(*client) == sf::Socket::Done){
                            if(_playerList->size() >= _maxPlayers)
                            {
                                client->disconnect();
                                delete client;
                                continue;
                            }

                            if(client->receive(packet) == sf::Socket::Done)
                            {
                                //! Quand la partie est crée - ajout de l'host
                                if(_playerList->empty())
                                {
                                    //! Verification du type de paquet
                                    if(packet >> _packetType >> _playerType >> _maxPlayers)
                                    {   
                                        //! Verification de la cohérence des données
                                        if(_packetType == newPlayer && _playerType == host && _maxPlayers >= _minPlayers)
                                        {
                                            std::cout << "nouveau client : " << client->getRemoteAddress() << std::endl;
                                            _playerList->push_back(std::pair(new Player(client, client->getRemoteAddress()),_playerType));
                                            _selector.add(*client);
                                            packet.clear();
                                            packet << numberOfPlayer << static_cast<sf::Uint8>(_playerList->size());
                                            client->send(packet);
                                            client->setBlocking(false);                               
                                            continue;
                                        }
                                        else
                                        {
                                            packet.clear();
                                            packet << error << contents << _packetContentError;
                                            client->send(packet);
                                            sf::sleep(sf::milliseconds(200));
                                            client->disconnect();
                                            delete client;
                                            continue;
                                        }    
                                    }
                                    else
                                    {
                                        packet.clear();
                                        packet << error << size << _packetTypeError;
                                        client->send(packet);
                                        sf::sleep(sf::milliseconds(200));
                                        client->disconnect();
                                        delete client;
                                        continue;
                                    }                  
                                } 
                                else if(!_playerList->empty())
                                {
                                    //! ajout des invités
                                    if(packet >> _packetType >> _playerType )
                                    {
                                        if(_packetType == newPlayer && _playerType == guest )
                                        {
                                            std::cout << "nouveau client : " << client->getRemoteAddress() << std::endl;
                                            _playerList->push_back(std::pair(new Player(client, client->getRemoteAddress()),_playerType));
                                            _selector.add(*client);
                                            packet.clear();
                                            packet << numberOfPlayer << static_cast<sf::Uint8>(_playerList->size());
                                            client->send(packet);
                                            client->setBlocking(false);
                                            continue;
                                        } 
                                        else
                                        {
                                            packet.clear();
                                            packet << error << contents << _packetContentError;
                                            client->send(packet);
                                            sf::sleep(sf::milliseconds(200));
                                            client->disconnect();
                                            delete client;
                                            continue;  
                                        }
                                    }
                                    else
                                    {
                                        packet.clear();
                                        packet << error << size << _packetTypeError;
                                        client->send(packet);
                                        sf::sleep(sf::milliseconds(200));
                                        client->disconnect();
                                        delete client;
                                        continue;
                                    }  
                                }
                            }                
                        } else delete client;
                        //! Verification récéption de message d'un client
                        for(auto it = _playerList->begin(); it != _playerList->end();)
                        {
                            std::pair client = *it;
                            sf::TcpSocket * clientSocket = client.first->getSocket();
                            if(clientSocket->receive(packet) == sf::Socket::Done)
                            {
                                if(packet >> _packetType)
                                {
                                    if(_packetType == quiteGame)
                                    {
                                        packet.clear();
                                        packet << quiteGame << client.second;
                                        //! Envoi notification déconnexion d'un joueur
                                        for(std::pair  player : *_playerList)
                                            player.first->getSocket()->send(packet);

                                        //! Si le joueur à quitter = hote, déconnexion de tous les joueurs
                                        sf::sleep(sf::milliseconds(200));
                                        if(client.second == _idHost)
                                        {
                                            for(std::pair  player : *_playerList)
                                                player.first->getSocket()->disconnect();
                                            _playerList->clear();
                                            _inLobby = false;
                                            _endOfGame = true;
                                        } 
                                        else
                                        {
                                            clientSocket->disconnect();
                                            _playerList->erase(it);
                                        }
                                        

                                    }
                                }
                            }
                        }
                    
                        if(_playerList->size() == _maxPlayers)
                        {
                            sf::Packet packet;
                            std::cout << "Envoi paquet pour creation de liste" <<std::endl;
                            packet << allPlayersPresent << static_cast<sf::Uint8>(_playerList->size());
                            for(std::pair  player : *_playerList)
                                player.first->getSocket()->send(packet);
                            _inLobby = false;
                        }
                    }
                   
                }
                
            }
            //!Gerer les evenements de SYNC et de partie
        }
    }
}

int main(int argc, char const *argv[])
{
    Server * server = new Server();
    server->run();
    return 0;
}


/*void Server::close()
{
     std::cout << "suppression joueurs " << std::endl;
    if(!_playerList->empty())
     for(Player * player : *_playerList)
        player->getSocket()->disconnect();
    
    std::cout << "joueur supprimes" << std::endl;
    _serverOpen = false;    
}*/
std::vector<std::pair<Player*,sf::Uint8>>* Server::getPlayers()
{
    return _playerList;
}
/*
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
*/