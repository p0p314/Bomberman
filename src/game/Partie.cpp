#include "Partie.hpp"
Partie::Partie(sf::RenderWindow * window) : _window(window), _mutex(new std::mutex)
{
  
    _player = new Player();
    _server = new Server();
    
    _level = new Monde();
    _level->initialisation();
    //_characterList.push_back(new Personnage(_level, titi));
}

void Partie::startServer()
{
    std::thread gameServer(&Server::listen, _server, _server->getPlayers(), _mutex);
    gameServer.detach();

}

std::vector<Personnage*> Partie::getCharacterList()
{
    return _characterList;
}

Monde * Partie::getLevel()
{
    return _level;
}
int Partie::Run()
{
    _exit = false;

    startServer();

    sf::Event event;
    sf::Clock clock;
    
    bool allowingMovement = false;
    int i = 0;
     
    

    while(!_exit){
        while(_server->getPlayers()->size() < 1)
        {  
            if(!_player->joinAGame()){
                if(i == 3) exit(1);        
                sf::sleep(sf::seconds(1.0));
            }
        }

        if(_characterList.size() != _server->getPlayers()->size())
        {   
            std::vector<Player*> * players = _server->getPlayers();
            for(int j = 0; j < players->size() ; j++){
                Personnage *p = new Personnage(_level,titi);
                p->setPlayer(_player);
                _characterList.push_back(p);
            }
            
            std::cout << _characterList.size() << std::endl;
            std::cout << _server->getPlayers()->size() << std::endl;
            allowingMovement = true;
        }
            
        
        HandleEvents(event,allowingMovement); //! Mettre variable allowingMovement
        float dt = clock.getElapsedTime().asSeconds();
        Update(dt);
        clock.restart();
        Draw();
    }

    return _exit;
}

void Partie::HandleEvents(sf::Event event,bool alLowingMovement)
{
    while (_window->pollEvent(event)) {
          
            if(event.type == sf::Event::Closed) _exit = true ;
            for(Personnage * charchater : _characterList) //!Ajouter l'autorisation de mouvement
                charchater->actions(event, alLowingMovement);
    }
}

void Partie::Update(float dt){
    for(Personnage * charchater : _characterList)
            charchater->Update(dt);
}

void Partie::Draw()
{
    std::vector<std::vector<TileMap*>> & tiles = _level->getTiles();

    _window->clear();

    for(int i = 0; i < _level->getGridLength(); i++)
        for(int j = 0; j < _level->getGridLength(); j++)
             _window->draw(tiles[i][j]->getSprite());
    
    for(Personnage * charchater : _characterList)
            _window->draw(*charchater);
            
    _window->display();
}