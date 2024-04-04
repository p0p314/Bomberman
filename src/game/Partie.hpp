#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/System.hpp>
#include "carte/Monde.h"
#include "elements/personnage/Personnage.hpp"
#include "server/Server.hpp"
#include "lobby/Lobby.hpp"
#include "GUI/Gui.hpp"
#include <iostream>
#include <mutex>
#include <thread>

// Déclarations préliminaires des classes utilisées pour éviter les dépendances cycliques
class Personnage;
class Server;
class Player;
class Lobby;
class Gui;

class Partie
{
private:
    sf::Uint8 _numberOfPlayer = 0, // Nombre de joueurs dans la partie
        _idPlayer,                 // Identifiant du joueur local
        _idSender,                 // Identifiant de l'émetteur du paquet réseau
        _action,                   // Action effectuée par un personnage
        _typeOfPacket,             // Type de paquet réseau reçu
        _playerDisconnected,       // Identifiant du joueur déconnecté
        _startGame;                // Indicateur de démarrage du jeu

    bool _synchronised = false, // Indique si le jeu est synchronisé
        _forcedExit = false;    // Indique si la sortie du jeu est forcée

    std::string _errorMessage; // Message d'erreur

    sf::RenderWindow *_window; // Fenêtre de jeu
    Monde *_level;             // Monde (carte) du jeu
    Player *_player;           // Joueur local
    Lobby *_lobby;             // Lobby du jeu

    std::vector<Personnage *> _characterList; // Liste des personnages du jeu

    bool _exit = false,     // Indique si la partie est terminée
        _exitToMenu = true, // Indique si le joueur retourne au menu principal
        _startingGame,      // Indique si le jeu est en train de démarrer
        _endofGame = false; // Indique si la partie est terminée

    float _startingGameCounter = 0.f, // Compteur pour le démarrage du jeu
        _timeToChangeCount = 1.f;     // Temps entre chaque changement de compte à rebours

    int _gameCounter = 3; // Compte à rebours avant le début du jeu

    Gui *_gameInfo; // Interface utilisateur du jeu

    void createCharacters();                // Crée les personnages du jeu
    void returnToMenu();                    // Retourne au menu principal
    void checkRecievedPacket(float dt = 0); // Vérifie les paquets réseau reçus

    void HandleEvents(sf::Event, float dt = 0); // Gère les événements du jeu
    void Update(float dt);                      // Met à jour l'état du jeu
    void Draw();                                // Dessine le jeu

public:
    // Constructeur et destructeur
    Partie() = delete;                                                            // Empêche la création d'un objet Partie sans arguments
    Partie(sf::RenderWindow *window, Player *joiner, sf::Uint8 typeOfplayer = 0); // Constructeur de la partie
    ~Partie();                                                                    // Destructeur de la partie

    std::vector<Personnage *> getCharacterList(); // Renvoie la liste des personnages
    Monde *getLevel();                            // Renvoie le monde (carte) du jeu
    int Run();                                    // Exécute la partie principale du jeu
};
