#include "Lobby.hpp"

Lobby::Lobby(sf::RenderWindow * window, Player *player, sf::Uint8 &numberOfPlayer) : 
    _window(window), _player(player), _numberOfPlayers(numberOfPlayer)
{
    _windowWidth = _window->getSize().x;
    _windowHeight =  _window->getSize().y;
    for(int i = 0; i < 4; i++){
        sf::CircleShape point(_radiusLoadingPoint);
        point.setFillColor(sf::Color::White);
        point.setPosition((_windowWidth/2 - _maxWidthLoadingBar/2) + i*(_radiusLoadingPoint*2+_spacementBetweenPoints), _windowHeight - _loadingBarPosY);
        _loadingPoints.push_back(point);
    }
}

bool Lobby::getExitToMenu()
{
    return _exitToMenu;
}
bool Lobby::Run()
{
    _exit = false;
   
    sf::Clock timer;
    sf::Event event;

    

    
    while (!_exit && !_allPlayersPresent ){
        int dt = timer.getElapsedTime().asSeconds();       
        HandleEvent(event);
        Update(dt);
        Draw();
        if(dt >= 4)timer.restart();
        
    } 
    return _exit;
}

void Lobby::Update(int dt)
{
     for(int i = 0; i < 4;  i++)
     {
        if(dt >= i  && dt < (i+1) ) _loadingPoints.at(i).setFillColor(sf::Color::Green);
        else  _loadingPoints.at(i).setFillColor(sf::Color::White);
     }


    if(_player->getSocket()->receive(_packet) == sf::Socket::Done)
    {
        std::cout << "paquet recu : ";
        if(_packet >> _typeOfPacket )
        {
            if(_typeOfPacket == static_cast<sf::Uint8>(1))
            {
                _packet >> _numberOfPlayers;
                std::cout << "nombre de joueurs en attente --> " << _numberOfPlayers << std::endl;
           
            } else if(_typeOfPacket == static_cast<sf::Uint8>(2) )
            {
                _allPlayersPresent = true;
                _packet >> _numberOfPlayers;
                std::cout << "Tout le monde present, nombre de joueurs -->" << static_cast<int>(_numberOfPlayers) << std::endl;
           
            } else std::cout << "packet non pris en charge"  << std::endl;
       
        } else std::cout << "erreur format du paquet --> " << static_cast<int>(_typeOfPacket)<<std::endl; 
    }

  
}
void Lobby::Draw()
{
    _window->clear();
    for(int i = 0; i < 4; i++)
        _window->draw(_loadingPoints.at(i));
    
    
    _window->display();

}

void Lobby::HandleEvent(sf::Event event)
{
     while (_window->pollEvent(event)) {
         if (event.type == sf::Event::Closed)
         {
            _exit = true;
            _exitToMenu = false;
            break;
         }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Delete))
            {
                _exit = true;
                _exitToMenu = true;
            }
    }
}