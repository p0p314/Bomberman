#include "Server.hpp"
#include <iostream>
Server::Server() : _selector()
{
    if (_listener.listen(2000, sf::IpAddress("127.0.0.1")) != sf::Socket::Done)
    {
        std::cout << "Impossible de creer le serveur" <<std::endl;
        exit(1);
    }
   
    _listener.setBlocking(false); //! Verifier si les attentes se font bien pour reception de paquet client
    _playerList = new std::vector<std::pair<Player*,sf::Uint8>>();
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
        _selector.add(_listener);
        _endOfGame = false;
        _inLobby = true;
        //!Boucle de jeu
        while(!_endOfGame)
        {   
            std::cout<< "Cree une nouvelle partie "<<std::endl;
            while(_inLobby)
            {
                std::cout<< "Attend joueur dans lobby, " << _playerList->size()<<" present(s), "<< static_cast<int>(_maxPlayers)<< " attendus"<<std::endl;
                
                if(_playerList->size() == static_cast<int>(_maxPlayers))
                {
                    sf::Packet packet;
                    std::cout << "Envoi paquet pour creation de liste" <<std::endl;
                    packet << allPlayersPresent << static_cast<sf::Uint8>(_playerList->size());
                    for(std::pair  player : *_playerList)
                        player.first->getSocket()->send(packet);
                    _inLobby = false;
                }

                if(_inLobby)
                {

                    if(_selector.wait())
                    {                            
                        std::cout<< "listener est pret" << std::endl;

                        if(_selector.isReady(_listener))
                        {
                            sf::TcpSocket * client = new sf::TcpSocket();
                            sf::Packet packet;
                            std::cout <<"listener ready" << std::endl;

                            if(_listener.accept(*client) == sf::Socket::Done){
                                std::cout<< "connexion, ok" << std::endl;
                                if(!_playerList->empty())
                                {
                                    if(_playerList->size() >= _maxPlayers)
                                    {
                                        client->disconnect();
                                        delete client;
                                        continue;
                                    }
                                }


                                std::cout<< "Connecte, attend paquet" << std::endl;
                                if(client->receive(packet) == sf::Socket::Done)
                                {
                                    //! Quand la partie est crée - ajout de l'host
                                    if(_playerList->empty())
                                    {                          
                                        std::cout<< "attend paquet de l'host" << std::endl;
                                        //! Verification du type de paquet
                                        if(packet >> _packetType >> _playerType >> _maxPlayers)
                                        {                
                                            if(_packetType == newPlayer && _playerType == host && _maxPlayers >= _minPlayers )
                                            {
                                                newClient(client);
                                                continue;
                                            }                        
                                            else
                                            {
                                                //! Verification de la cohérence des données
                                                packetError(client, contents, _packetContentError);                                    
                                                continue;
                                            }

                                        }
                                        else
                                        {
                                             //! Taille de paquet incorrecte
                                            packetError(client, size, _packetTypeError);                                   
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
                                                newClient(client);
                                                packet.clear();
                                                continue;
                                            } 
                                            else
                                            {
                                                packetError(client,contents, _packetContentError);
                                                continue;  
                                            }
                                        }
                                        else
                                        {
                                            packetError(client,size, _packetTypeError);
                                            continue;
                                        }  
                                    }
                                }
                            } 
                            else delete client;

                        }              
                        checkPacketFromPlayers();                    
                    }
                }

                
                
            }

            while(!_endOfGame)
            {
                std::cout<<"Dans la partie " << std::endl;
                if(_selector.wait())
                {    
                    std::cout<<"Quelqu'un est pret a recevoir" <<std::endl;                        
                    if(_selector.isReady(_listener))
                    {
                        checkPacketFromPlayers();
                    }
                }
                sf::sleep(sf::seconds(1));
            }
            //!Gerer les evenements de SYNC et de partie
        }
    }
}

void Server::checkPacketFromPlayers()
{
    for(auto it = _playerList->begin(); it != _playerList->end();)
    {
        sf::Packet packet;
        std::pair client = *it;
        sf::TcpSocket * clientSocket = client.first->getSocket();
        sf::Socket::Status status = clientSocket->receive(packet);
        switch (status)
        {
            case sf::Socket::Done :
                if(packet >> _packetType)
                {
                    if(_packetType == quiteGame)
                        clientDisconnect(client, it); 

                    else if(_packetType == action )
                    {
                        packet >> _actionType;
                        
                        packet.clear();
                        packet << _packetType << _actionType; //!!!!Ajouter identification du joueur
                        for(std::pair  player : *_playerList)
                            player.first->getSocket()->send(packet);
                    } 
                    else if(_packetType == listReady )
                    {
                        _cptListReady++;
                        if(_cptListReady == _maxPlayers)
                        {
                            sf::Packet packet;
                            packet << startGame;
                            sf::sleep(sf::milliseconds(500));
                            for(auto & player : *_playerList)
                                player.first->getSocket()->send(packet);
                        }
                    }
                            

                }
                break;

            case sf::Socket::Disconnected : 
                clientDisconnect(client, it);
                break;
                
            case sf::Socket::Error : 
                std::cout <<"erreur avec les client" << client.second <<std::endl;
                break;
            default:
                break;
        }
       
                   
            
    }  
}
void Server::packetError(sf::TcpSocket * client, Server::errorCode code, std::string message)
{
    sf::Packet packet;
    packet << error << code << message;

    std::cout<< message << std::endl;
    
    client->send(packet);
    sf::sleep(sf::milliseconds(300));
    client->disconnect();
    
    delete client;
}


void Server::newClient(sf::TcpSocket *client)
{
    sf::Packet packet;
    
    std::cout << "nouveau client : " << client->getRemoteAddress() << std::endl;
    _playerList->push_back(std::pair(new Player(client, client->getRemoteAddress()),_playerType));
    _selector.add(*client);
    packet << numberOfPlayer << static_cast<sf::Uint8>(_playerList->size());
    //sf::sleep(sf::milliseconds(1000));
    for(std::pair  player : *_playerList)
        if(player.first->getSocket()->send(packet) == sf::Socket::Done)
            std::cout << "Nombre de joueurs envoye " << std::endl;
    client->setBlocking(false);                               
        
    
   
}

void Server::clientDisconnect(std::pair<Player*,sf::Uint8> client, std::vector<std::pair<Player *, sf::Uint8>>::iterator it)
{
    sf::TcpSocket * clientSocket = client.first->getSocket();
    sf::Packet packet;
    std::cout <<"deconnexion de " << clientSocket->getRemoteAddress()<< std::endl;
    clientSocket->disconnect();
    packet.clear();
    packet << quiteGame << client.second;
    if(client.second == _idHost)
    {
        for(std::pair  player : *_playerList){
            player.first->getSocket()->send(packet);
            player.first->getSocket()->disconnect();
        }
        _playerList->clear();
        _selector.clear();
        _inLobby = false;
        _endOfGame = true;
        _maxPlayers = 1;
        _cptListReady = 0;
    } 
    else
    {
        for(std::pair  player : *_playerList)
            player.first->getSocket()->send(packet);
        clientSocket->disconnect();
        _playerList->erase(it);                                                       
        _selector.remove(*clientSocket);
        _cptListReady--;
    }
}
int main(int argc, char const *argv[])
{
    Server * server = new Server();
    server->run();
    return 0;
}


std::vector<std::pair<Player*,sf::Uint8>>* Server::getPlayers()
{
    return _playerList;
}


