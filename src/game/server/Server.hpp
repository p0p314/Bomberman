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
    std::string _charName;
    sf::Uint8 _dir;
    sf::SocketSelector _selector;
    std::vector<Player*>* _playerList;
    std::mutex* _mutex;

    public:
        Server();
        bool waitingPLayer(Partie * partie);
        std::string getCharName();
        sf::Uint8 getDirection();
        sf::TcpListener  getListener();
        void getPacket();

        std::vector<Player*>* getPlayers();

            
        void listen(std::vector<Player*>* playerList, std::mutex* mutex);

        friend sf::Packet& operator >>(sf::Packet& packet, Server& server);
};