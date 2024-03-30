#pragma once

#include <SFML/Graphics.hpp>
#include "../server/Server.hpp"
#include "../player/Player.hpp"
#include <iostream>

class Player;

class Lobby 
{
    public : 
        Lobby(sf::RenderWindow * window, Player * player, sf::Uint8 &numberOfPlayer);
        bool Run(),
             getExitToMenu();

    private : 
        Player * _player;
        sf::RenderWindow * _window;
        
        sf::Packet _packet; 

        bool _exit,
             _exitToMenu = true;

        int _windowWidth,
            _windowHeight;

        std::vector<sf::CircleShape> _loadingPoints;
        
        sf::Uint8 _typeOfPacket,
                  &_numberOfPlayers;

        bool _allPlayersPresent = false;
        
        float _radiusLoadingPoint = 20;
        int _loadingBarPosY = 150,
            _spacementBetweenPoints = 10,
            _numberOfLoadingPoints = 4,
            _maxWidthLoadingBar = (_radiusLoadingPoint*2*_numberOfLoadingPoints) + (_numberOfLoadingPoints * _spacementBetweenPoints) ;
    
        void Draw();
        void Update(int dt);
        void HandleEvent(sf::Event);
       
};