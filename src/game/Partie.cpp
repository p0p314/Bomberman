#include "Partie.hpp"

Partie::Partie(sf::RenderWindow * window, Player * creator) :  _window(window), _mutex(new std::mutex)
{
    int i;
    _player = creator;
    _creator = true;
    _playerList = new std::vector<std::pair<Player*, Personnage*>>();
    
    if(_player->joinAGame())
        _player->signalArrival(static_cast<sf::Uint8>(0),static_cast<sf::Uint8>(2));
    else std::cout << "bug"<< std::endl;
    
    sf::Packet packet;
    if(_player->getSocket()->receive(packet) == sf::Socket::Done)
    {
        std::cout << "paquet recu : ";
        if(packet >> _typeOfPacket)
        {
            std::cout << static_cast<int>(_typeOfPacket)<< " --> ";
            if(_typeOfPacket == static_cast<sf::Uint8>(1))
            {
                _player->getSocket()->setBlocking(false);
                packet >> _numberOfPlayer;
                std::cout << static_cast<int>(_numberOfPlayer) <<std::endl;
                _lobby = new Lobby(_window, _player, _numberOfPlayer);
                if((_exit = _lobby->Run()))
                {
                    _exitToMenu = _lobby->getExitToMenu();   
                    return;
                }
            }
            else if(_typeOfPacket == static_cast<sf::Uint8>(6) || _typeOfPacket == static_cast<sf::Uint8>(7))
            {
                //!Afficher un message d'erreur au client
                if(_typeOfPacket == static_cast<sf::Uint8>(7))
                {
                    packet >> _errorMessage;
                    std::cout << "erreur : " << _errorMessage << std::endl;
                }
                else
                {
                    packet >> _playerDisconnected;
                    std::cout << "deconnexion : " << _playerDisconnected <<std::endl;
                }
                
                _exit = true;
                _exitToMenu = true;
            } 
            else  std::cout <<"Type de paquet non pris en charge" << std::endl;
        } else std::cout <<"Erreur format du paquet" << std::endl;
    } else std::cout <<"Serveur inaccessible" << std::endl;;
    
    
    std::cout <<"Nombre de joueur pour la partie : " << static_cast<int>(_numberOfPlayer) << std::endl;;
  
    _level = new Monde();
    _level->initialisation();
    
     if (_characterList.size() != _numberOfPlayer)
    {
    
        Personnage::skin skins[] = {Personnage ::toto, Personnage::titi};
        for (int j = 0; j < _numberOfPlayer; j++)
        {
            Personnage *p = new Personnage(_player,_level, skins[j]);
            _characterList.push_back(p);
        }
        _allowingMovement = true;
    }

}

Partie::Partie(sf::RenderWindow * window, Player * joiner, sf::IpAddress server) :  _window(window), _mutex(new std::mutex)
{
    _player = joiner;
     if(_player->joinAGame())
        _player->signalArrival(static_cast<sf::Uint8>(0));
    else std::cout << "bug"<< std::endl;
    
    sf::Packet packet;
    if(_player->getSocket()->receive(packet) == sf::Socket::Done)
    {
        std::cout << "paquet recu : ";
        if(packet >> _typeOfPacket)
        {
            std::cout << static_cast<int>(_typeOfPacket)<< " --> ";
            if(_typeOfPacket == static_cast<sf::Uint8>(1))
            {
                _player->getSocket()->setBlocking(false);
                packet >> _numberOfPlayer;
                std::cout << static_cast<int>(_numberOfPlayer) <<std::endl;
                _lobby = new Lobby(_window, _player, _numberOfPlayer);
                if((_exit = _lobby->Run()))
                {
                    _exitToMenu = _lobby->getExitToMenu();   
                    return;
                }
            }
            else if(_typeOfPacket == static_cast<sf::Uint8>(6) || _typeOfPacket == static_cast<sf::Uint8>(7))
            {
                //!Afficher un message d'erreur au client
                if(_typeOfPacket == static_cast<sf::Uint8>(7))
                {
                    packet >> _errorMessage;
                    std::cout << "erreur : " << _errorMessage << std::endl;
                }
                else
                {
                    packet >> _playerDisconnected;
                    std::cout << "deconnexion : " << _playerDisconnected <<std::endl;
                }
                
                _exit = true;
                _exitToMenu = true;
            } 
            else  std::cout <<"Type de paquet non pris en charge" << std::endl;
        } else std::cout <<"Erreur format du paquet" << std::endl;
    } else std::cout <<"Serveur inaccessible" << std::endl;;
    
    
    std::cout <<"Nombre de joueur pour la partie : " << static_cast<int>(_numberOfPlayer) << std::endl;;
  
    _level = new Monde();
    _level->initialisation();
    
     if (_characterList.size() != _numberOfPlayer)
    {
    
        Personnage::skin skins[] = {Personnage ::toto, Personnage::titi};
        for (int j = 0; j < _numberOfPlayer; j++)
        {
            Personnage *p = new Personnage(_player,_level, skins[j]);
            _characterList.push_back(p);
        }
        _allowingMovement = true;
    }
}

Partie::~Partie()
{
    delete _level;
    if(!_characterList.empty())
        for(Personnage * character : _characterList)
            delete character;
    delete _gameServer;
    delete _mutex;
    delete _lobby;
}

void Partie::startServer()
{
   // _gameServer = new std::thread(&Server::listen, _server, _server->getPlayers(), _mutex);

}

std::vector<Personnage*> Partie::getCharacterList()
{
    return _characterList;
}

Monde * Partie::getLevel()
{
    return _level;
}

int Partie::Run() // Méthode appelé par le menu lorsque le joueur rejoint une partie.
{

    sf::Event event;
    sf::Clock clock;
    
    std::cout << "dans run " << std::endl;
    while (!_exit )
    { 
  
  
        HandleEvents(event); 
        float dt = clock.getElapsedTime().asSeconds();
        Update(dt);
        clock.restart();
        Draw(); 
    }
    _player->quiteGame();

    return _exitToMenu;
}

void Partie::HandleEvents(sf::Event event)
{
    while (_window->pollEvent(event)) {
          
            if(event.type == sf::Event::Closed ){
                //_exit = 1;
                for (Personnage *charchater : _characterList)
                {
                    charchater->getOwner()->getSocket()->disconnect();
                }
               _exit = true;
               _exitToMenu = false;
               break;
            } 

            for(Personnage * charchater : _characterList) 
                charchater->actions(event, _allowingMovement);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Delete))
            {
                _exit = true;
                _exitToMenu = true;
            }
    }
}
void Partie::Update(float dt)
{
    for (Personnage *character : _characterList)
        character->Update(dt);
}

void Partie::Draw()
{
    std::vector<std::vector<TileMap*>> & tiles = _level->getTiles();

    _window->clear();

    for (int i = 0; i < _level->getGridLength(); i++)
        for (int j = 0; j < _level->getGridLength(); j++)
            _window->draw(tiles[i][j]->getSprite());

    for (Personnage *character : _characterList)
        _window->draw(*character);

    _window->display();
}