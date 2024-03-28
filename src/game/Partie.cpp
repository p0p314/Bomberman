#include "Partie.hpp"

Partie::Partie(sf::RenderWindow * window, Player * creator) :  _window(window), _mutex(new std::mutex)
{
    int i;
    _player = creator;
    _creator = true;
   // _server = new Server();
    
    //startServer();          //!ajouter liste de joueurs
    
    _lobby = new Lobby(_window);
    if((_exit = _lobby->Run()))
    {
        _exitToMenu = _lobby->getExitToMenu();   
        return;
    }
  
    _level = new Monde();
    _level->initialisation();
    _player->joinAGame();
    
    //! waitinforPlayers(); //fonction avec une boucle while et de l'affichage dedans, comme ça le run de la partie ne se lance que quand le nombre de joueurs necessaire est atteint
}

Partie::Partie(sf::RenderWindow * window, Player * joiner, sf::IpAddress server) :  _window(window), _mutex(new std::mutex)
{
    _player = joiner;
    _level = new Monde();
    _level->initialisation();
    _player->joinAGame();
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

int Partie::Run() // Méthode appeler par le menu lorsque le joueur rejoint une partie.
{

    sf::Event event;
    sf::Clock clock;
    
  
       
        

    int i = 0;

    /*if (_characterList.size() != _server->getPlayers()->size())
    {
        std::vector<Player *> *players = _server->getPlayers();
        Personnage::skin skins[] = {Personnage ::toto, Personnage::titi};
        for (int j = 0; j < players->size(); j++)
        {
            Personnage *p = new Personnage(_player,_level, skins[j]);
            _characterList.push_back(p);
        }
        std::cout << "nb Personnage : " << _characterList.size();
        std::cout << ", nb Joueur : " << _server->getPlayers()->size() << std::endl;
        _allowingMovement = true;
    }*/

     std::cout << "dans run " << std::endl;
    while (!_exit )
    { 
  
  
        HandleEvents(event); 
        float dt = clock.getElapsedTime().asSeconds();
        Update(dt);
        clock.restart();
        Draw(); // Fonction pour dessiner les personnages la carte.
    }
    std::cout << "attente du join " << std::endl;
    /*if(_gameServer->joinable())
    {
        std::cout << "joignable " << std::endl;
        _gameServer->join();
    }
    std::cout << "join fait " << std::endl;*/
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