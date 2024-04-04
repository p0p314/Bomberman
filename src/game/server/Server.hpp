#pragma once

#include <SFML/Network.hpp>
#include "../Partie.hpp"
#include "../player/Player.hpp"

#include <mutex>

class Partie; // Déclaration anticipée de la classe Partie
class Player; // Déclaration anticipée de la classe Player

class Server
{
public:
    // Énumérations pour différents signaux, types de joueur et codes d'erreur
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

    Server();                           // Constructeur
    ~Server();                          // Destructeur
    bool waitingPLayer(Partie *partie); // Attente de joueurs
    sf::Uint16 getID();                 // Récupère l'ID
    sf::Uint16 getDirection();          // Récupère la direction
    sf::TcpListener getListener();      // Récupère l'écouteur TCP
    void getPacket();                   // Récupère le paquet

    std::vector<std::pair<Player *, sf::Uint8>> *getPlayers(); // Récupère la liste des joueurs

    void run();                                                        // Fonction principale d'exécution du serveur
    void close();                                                      // Ferme le serveur
    void listen(std::vector<Player *> *playerList, std::mutex *mutex); // Fonction d'écoute du serveur
    void shareToAllOthers(sf::Packet packet, Player *player);          // Partage avec tous les autres

    friend sf::Packet &operator>>(sf::Packet &packet, Server &server); // Surcharge de l'opérateur >>

private:
    bool debug = false;        // Mode debug
    sf::TcpListener _listener; // Objet écoutant les connexions

    sf::Uint8 _idHost = 0; // ID de l'hôte

    sf::SocketSelector _selector;                             // Sélecteur de socket
    std::vector<std::pair<Player *, sf::Uint8>> *_playerList; // Liste des joueurs

    sf::Uint8 _idPlayer = 0, // ID du joueur
        _idSender,           // ID de l'envoyeur
        _playerType = 0,     // Type de joueur
        _maxPlayers = 1,     // Nombre maximum de joueurs
        _minPlayers = 2,     // Nombre minimum de joueurs
        _packetType,         // Type de paquet
        _actionType,         // Type d'action
        _forcedExit;         // Sortie forcée

    std::string _packetMessage,                                    // Message du paquet
        _packetContentError = "Erreur dans les donnees du paquet", // Erreur de contenu de paquet
        _packetTypeError = "Le type de paquet n'est pas reconnu";  // Erreur de type de paquet

    int _cptListReady = 0; // Compteur pour la liste prête

    bool _serverOpen = true, // Serveur ouvert
        _endOfGame = false,  // Fin de partie
        _inLobby = true;     // Dans le lobby

    void initData(); // Initialise les données

    void waitingForPlayers();                                // Attente des joueurs
    void newPlayerTryingToJoin();                            // Nouveau joueur essayant de se joindre
    void refuseNewPlayer(sf::TcpSocket *client);             // Refus d'un nouveau joueur
    void acceptNewPlayer(sf::TcpSocket *client);             // Acceptation d'un nouveau joueur
    void hasHost(sf::TcpSocket *client, sf::Packet packet);  // L'hôte est présent
    void hasGuest(sf::TcpSocket *client, sf::Packet packet); // L'invité est présent
    void startPlaying();                                     // Début du jeu
    void clientDisconnect(std::pair<Player *, sf::Uint8> client,
                          std::vector<std::pair<Player *, sf::Uint8>>::iterator it,
                          sf::Packet packet); // Déconnexion d'un client
    void DisconnectOthers();                  // Déconnexion de tous les autres
    void returnToMenu();                      // Retour au menu
    void newClient(sf::TcpSocket *client);    // Nouveau client
    void packetError(sf::TcpSocket * client, Server::errorCode, std::string message);
    void checkPacketFromPlayers(); // Vérification des paquets des joueurs
};