#include "Server.hpp"
#include <iostream>
Server::Server() : _selector()
{
    if (_listener.listen(2000, sf::IpAddress::getLocalAddress()) != sf::Socket::Done)
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
    
}

void Server::run()
{
    //! Boucle permanente
    while(_serverOpen)
    {
        std::cout<< "Ecoute sur le port " << _listener.getLocalPort() <<std::endl;
        initData();
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
                                std::cout<< "connexion de "<< client->getRemoteAddress() << std::endl;

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
                                auto status = client->receive(packet);
                                if(status == sf::Socket::Done)
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
                                                returnToMenu();
                                                continue;
                                            }

                                        }
                                        else
                                        {
                                             //! Taille de paquet incorrecte
                                            packetError(client, size, _packetTypeError);  
                                            returnToMenu();                                 
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
                                } else if(status == sf::Socket::Done) std::cout << "deco"<<std::endl;
                                else if(status ==sf::Socket::Error) std::cout << "erreur" <<std::endl;
                                else
                                {
                                    sf::Uint8 cleanBuffer;
                                    packet >> cleanBuffer;
                                    std::cout  << "code " << static_cast<int>(status)<< std::endl;
                                    std::cout <<"\tcontient " << cleanBuffer;
                                } 
                            } 
                            else
                            {
                                std::cout  << "connexion impossible" << std::endl;
                                std::cerr;
                                delete client;
                            } 
                        }              
                        checkPacketFromPlayers(); 
                        if(_playerList->empty()) returnToMenu();                   
                    }
                }                  
            }
            while(!_endOfGame)
            {
                checkPacketFromPlayers(); 
                if(_forcedExit == static_cast<sf::Uint8>(0))
                    if(!_playerList->empty())
                    {
                        sf::Packet packet;
                        packet.clear();
                        packet << quiteGame;
                        std::cout <<"\t liste non vide"<< std::endl;
                        for(std::pair  player : *_playerList)
                        {
                            player.first->getSocket()->send(packet);
                            std::cout <<"\tnotif envoyee "<< std::endl;
                        }
                    }
                if(_playerList->empty()) returnToMenu();        
            }
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
        if(debug)std::cout << " check pour le client  : " << client.second << std::endl << "\t ";
        switch (status)
        {
            case sf::Socket::Done :
                if(debug)std::cout << "paquet recu de : " << clientSocket->getRemoteAddress() << " --> ";
                if(packet >> _packetType)
                {
                    if(_packetType == quiteGame){
                        if(debug)std::cout << " quiteGame : ";
                        clientDisconnect(client, it, packet); 
                        if(it == _playerList->end()) return;                       
                    }

                    else if(_packetType == action )
                    {
                        if(debug)std::cout << "action de " << static_cast<int>(client.second) << " avec id : " << static_cast<int>(_idSender) << std::endl;
                        packet >> _idSender;
                        packet >> _actionType;

                        packet.clear();
                        if(client.second == _idSender)
                        {
                            packet << _packetType << _idSender << _actionType; //!!!!Ajouter identification du joueur
                            for(std::pair  player : *_playerList)
                                player.first->getSocket()->send(packet);
                            if(debug)std::cout << "action partagee a tous les joueurs" << std::endl;
                        }
                    } 
                    else if(_packetType == listReady )
                    {
                        _cptListReady++;
                        if(debug)std::cout << "liste prete | " << _cptListReady << " prete(s)" << std::endl;
                        if(_cptListReady == _maxPlayers)
                        {
                            sf::Packet packet;
                            packet << startGame;
                            sf::sleep(sf::milliseconds(500));
                            if(debug)std::cout << "Tous le monde est pret, debut dans 3 secondes" <<std::endl;
                            for(auto & player : *_playerList)
                                player.first->getSocket()->send(packet);

                        }  
                    } else if(debug) std::cout<< static_cast<int>(_packetType) << std::endl;
                            
                }
                break;

            case sf::Socket::Disconnected : 
                if(debug)std::cout << " disconnected : " << std::endl;
                clientDisconnect(client, it, packet);
                if(it == _playerList->end()) return;    
                break;
                
            case sf::Socket::Error : 
                if(debug)std::cout <<"erreur avec le client : " << static_cast<int>(client.second) <<std::endl;
                std::cerr;
                break;

            default:
                break;
        }
       
        it++;    
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
    _playerList->push_back(std::pair(new Player(client, client->getRemoteAddress()),_idPlayer));
    _selector.add(*client);
    
    packet << numberOfPlayer << static_cast<sf::Uint8>(_playerList->size()) << _idPlayer;
    std::cout << "id joueur " << static_cast<int>(_idPlayer) << std::endl;
    _idPlayer = _idPlayer + 1;

    sf::sleep(sf::milliseconds(1000));
    for(std::pair  player : *_playerList)
        if(player.first->getSocket()->send(packet) == sf::Socket::Done)
            std::cout << "Nombre de joueurs envoye " << std::endl;
    client->setBlocking(false);                               
        
    
   
}

void Server::clientDisconnect(std::pair<Player*,sf::Uint8> client, 
                              std::vector<std::pair<Player *, sf::Uint8>>::iterator it,
                              sf::Packet packet)
{
    sf::TcpSocket * clientSocket = client.first->getSocket();
    std::cout <<"\tdeconnexion de " << clientSocket->getRemoteAddress()<< std::endl;
    
    packet >> _forcedExit;
    std::cout <<"\tsortie forcee ?  " << static_cast<int>(_forcedExit)<< std::endl;
    
    clientSocket->disconnect();
    it = _playerList->erase(it); 
    if(it == _playerList->end()) return;                                                      
    _selector.remove(*clientSocket);

    //! Lorsqu'un joueur se deconnecte, il force les autres à quitter la partie
    
    
}

void Server::returnToMenu()
{
    _inLobby = false;
    _endOfGame = true;   
}

void Server::initData()
{
    _playerList->clear();
    _selector.clear();
    _inLobby = true;
    _endOfGame = false;
    _maxPlayers = 1;
    _cptListReady = 0;
    _idPlayer = 0;
    _selector.add(_listener);
    _forcedExit = 2;
}
int main(int argc, char const *argv[])
{
    Server * server = new Server();
    server->run();
    std::cout<<"fin" <<std::endl;
    return 0;
}


std::vector<std::pair<Player*,sf::Uint8>>* Server::getPlayers()
{
    return _playerList;
}


