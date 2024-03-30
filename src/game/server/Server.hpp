#pragma once

#include <SFML/Network.hpp>
#include "../Partie.hpp"
#include "../player/Player.hpp"

#include <mutex>
class Partie;
class Player;

class Server
{
    public:
        enum Signal : sf::Uint8 
        {
            newPlayer = 0,
            numberOfPlayer,
            allPlayersPresent,
            listReady,
            startGame,
            action,
            quiteGame,
            error
        };

        enum playerType : sf::Uint8 
        {
            host = 0,
            guest
        };

        enum errorCode : sf::Uint8 
        {
            size = 0,
            contents,
        };

        enum typeOfAction : sf::Uint8
        {
            plant = 0,
            up,
            right,
            down,
            left
        };

        Server();
        ~Server();
        bool waitingPLayer(Partie * partie);
        sf::Uint16 getID();
        sf::Uint16 getDirection();
        sf::TcpListener  getListener();
        void getPacket();

        std::vector<std::pair<Player*,sf::Uint8>>* getPlayers();

        void run();
        void close();   
        void listen(std::vector<Player*>* playerList, std::mutex* mutex);
        void shareToAllOthers(sf::Packet packet, Player * player);
        friend sf::Packet& operator >>(sf::Packet& packet, Server& server);
        
private: 
    sf::TcpListener _listener;
   
    sf::Uint8 _idHost;
               
    sf::SocketSelector _selector;
    std::vector<std::pair<Player*,sf::Uint8>>* _playerList;
    std::mutex* _mutex;

    sf::Uint8 _playerType = 0,
              _maxPlayers = 1,
              _minPlayers = 2,
              _packetType,
              _actionType;

    std::string _packetMessage,
                _packetContentError = "Erreur dans les donnee du paquet",
                _packetTypeError = "Le type de paquet n'est pas reconnu";
    
    int _cptListReady = 0;

    bool _serverOpen = true,
         _endOfGame = false,
         _inLobby = true;

    void clientDisconnect(std::pair<Player*,sf::Uint8> client,
                         std::vector<std::pair<Player *, sf::Uint8>>::iterator it);
    
    void newClient(sf::TcpSocket* client);
    void packetError(sf::TcpSocket * client, Server::errorCode, std::string message);
    void checkPacketFromPlayers();
};