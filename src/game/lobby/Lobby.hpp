#pragma once

#include <SFML/Graphics.hpp>
#include "../server/Server.hpp"
#include <iostream>

class Server;

class Lobby 
{
    public : 
        Lobby(sf::RenderWindow * window, Server * server);
        bool Run(),
             getExitToMenu();

    private : 
        bool _exit,
             _exitToMenu = true;

        sf::RenderWindow * _window;
        int _windowWidth,
            _windowHeight;
        Server * _server;

        std::vector<sf::CircleShape> _loadingPoints;
       
        float _radiusLoadingPoint = 20;
        int _loadingBarPosY = 150,
            _spacementBetweenPoints = 10,
            _numberOfLoadingPoints = 4,
            _maxWidthLoadingBar = (_radiusLoadingPoint*2*_numberOfLoadingPoints) + (_numberOfLoadingPoints * _spacementBetweenPoints) ;
    
        void Draw();
        void Update(int dt);
        void HandleEvent(sf::Event);
       
};