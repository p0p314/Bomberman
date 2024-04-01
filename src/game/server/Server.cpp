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
  
    while(_serverOpen)
    {
        std::cout<< "Ecoute sur le port " << _listener.getLocalPort() <<std::endl;
        
        initData();
        while(!_endOfGame)
        {   
            std::cout<< "Cree une nouvelle partie "<<std::endl;
            while(_inLobby)
            {
                std::cout<< "Attend joueur dans lobby, " << _playerList->size()<<" present(s), "<< static_cast<int>(_maxPlayers)<< " attendus"<<std::endl;
                
                if(_playerList->size() == static_cast<int>(_maxPlayers))
                {
                    startPlaying();
                    startPlaying();
                    continue;
                }   
                    
                waitingForPlayers();
                                  
            }
            while(!_endOfGame)
            {
                checkPacketFromPlayers(); 
                if(_forcedExit == static_cast<sf::Uint8>(0)) DisconnectOthers();
                if(_playerList->empty()) returnToMenu();        
            }
        }
    }
}

void Server::waitingForPlayers()
{
    if(_selector.wait())
    {                            
        std::cout<< "listener est pret" << std::endl;

        if(_selector.isReady(_listener))
        {
            newPlayerTryingToJoin();                        
            return;
        }
                      
        checkPacketFromPlayers(); 
        if(_forcedExit == static_cast<sf::Uint8>(0)) DisconnectOthers();
        if(_playerList->empty()) returnToMenu();                   
    }
}

void Server::newPlayerTryingToJoin()
{
    std::cout <<"listener ready" << std::endl;
    sf::TcpSocket * client = new sf::TcpSocket();
    sf::Socket::Status status = _listener.accept(*client);
    if(status == sf::Socket::Done){
        std::cout<< "connexion de "<< client->getRemoteAddress() << std::endl;

        if(!_playerList->empty() && _playerList->size() >= _maxPlayers)
        {
            refuseNewPlayer(client);
            return;
        }
                
        acceptNewPlayer(client);
        return;
        
    } else delete client;
}

void Server::refuseNewPlayer(sf::TcpSocket * client)
{
    
    std::cout << "deconnexion de " << client->getRemoteAddress() << std::endl;
    client->disconnect();
    delete client;
    return;

}

void Server::acceptNewPlayer(sf::TcpSocket * client)
{
    sf::Packet packet;
    
    std::cout<< "Connecte, attend paquet" << std::endl;
    auto status = client->receive(packet);
    if(status == sf::Socket::Done)
    {
        //! Quand la partie est crée - ajout de l'host
        if(_playerList->empty())
        { 
            hasHost(client, packet);                         
            return;              
        } 
        else if(!_playerList->empty())
        {
            hasGuest(client, packet);
            return;
        }
    } 
}

void Server::hasHost(sf::TcpSocket * client, sf::Packet packet)
{
    std::cout<< "paquet pour host recu" << std::endl;
    //! Verification du type de paquet
    if(packet >> _packetType >> _playerType >> _maxPlayers)
    {                
        if(_packetType == newPlayer && _playerType == host && _maxPlayers >= _minPlayers )
        {
            newClient(client);
            return;
        }                        
        else
        {
            //! Verification de la cohérence des données
            packetError(client, contents, _packetContentError);                                    
            returnToMenu();
            return;
        }
    }
    else
    {
         //! Taille de paquet incorrecte
        packetError(client, size, _packetTypeError);  
        returnToMenu();                                 
        return;
    }    
}

void Server::hasGuest(sf::TcpSocket * client, sf::Packet packet)
{
    std::cout<< "paquet pour gest recu" << std::endl;
    //! ajout des invités
    if(packet >> _packetType >> _playerType )
    {
        if(_packetType == newPlayer && _playerType == guest )
        {
            newClient(client);
            packet.clear();
            return;
        } 
        else
        {
            packetError(client,contents, _packetContentError);
            return;  
        }
    }
    else
    {
        packetError(client,size, _packetTypeError);
        return;
    }  
}

void Server::startPlaying()
{
    sf::Packet packet;
    std::cout << "Envoi paquet pour creation de liste" <<std::endl;
    packet << allPlayersPresent << static_cast<sf::Uint8>(_playerList->size());
    for(std::pair  player : *_playerList)
        player.first->getSocket()->send(packet);
    _inLobby = false;
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
                        packet >> _idSender;
                        packet >> _actionType;

                        std::cout << "action de " << static_cast<int>(client.second) << " avec id : " << static_cast<int>(_idSender) << std::endl;
                        
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

   
    
    
}

void Server::DisconnectOthers()
{

    if(!_playerList->empty())
    {
        sf::Packet packet;
        packet.clear();
        packet << quiteGame;
        for(std::pair  player : *_playerList)
        {
            player.first->getSocket()->send(packet);
            std::cout <<"\tnotif envoyee "<< std::endl;    
        }
        _forcedExit = 2;
    }
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


