#pragma once

#include <SFML/Network.hpp>
#include "../Partie.hpp"
#include "../player/Player.hpp"

#include <mutex>
class Partie;
class Player;

class Server
{
    private: 
    sf::TcpListener _listener;
   
    sf::Uint16 _dir,
               _ID;
    sf::SocketSelector _selector;
    std::vector<Player*>* _playerList;
    std::mutex* _mutex;

    bool _serverOpen = true;
    public:
        Server();
        ~Server();
        bool waitingPLayer(Partie * partie);
        sf::Uint16 getID();
        sf::Uint16 getDirection();
        sf::TcpListener  getListener();
        void getPacket();

        std::vector<Player*>* getPlayers();

        void close();   
        void listen(std::vector<Player*>* playerList, std::mutex* mutex);
        void shareToAllOthers(sf::Packet packet, Player * player);
        friend sf::Packet& operator >>(sf::Packet& packet, Server& server);
};