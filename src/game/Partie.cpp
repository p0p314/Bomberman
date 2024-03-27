#include "Partie.hpp"
Partie::Partie(sf::RenderWindow * window) : _window(window), _mutex(new std::mutex)
{
  
    _player = new Player();
    _server = new Server();
    
    _level = new Monde();
    _level->initialisation();
}
Partie::Partie(sf::RenderWindow * window, Player * creator) :  _window(window), _mutex(new std::mutex)
{
    int i;
    _player = creator;
    _server = new Server();
    _level = new Monde();
    _level->initialisation();
    startServer();
    while (_server->getPlayers()->size() < 2) //!!!!!! Passer à deux pour tester
    {
        if (_server->getPlayers()->size() < 1)
            if (!_player->joinAGame())
            {
                if (i == 3)
                    exit(1);
                sf::sleep(sf::seconds(1.0));
            }
        sf::sleep(sf::seconds(1.0));
    }

    if (_characterList.size() != _server->getPlayers()->size())
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
    }
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
    delete _player;
    delete _level;
    delete _server;
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

int Partie::Run() // Méthode appeler par le menu lorsque le joueur rejoint une partie.
{
    _exit = 0;

    sf::Event event;
    sf::Clock clock;
    
    int i = 0;

    while (_exit != 2)
    { //! <!> Les deux joueurs ne peuvent pas avoir le serveur lancé <!>
      //! Déplacer la première partie du code dans une fonction déstinée au créateur de la partie, faire une page de chargement avec ce code qui tourne en fond ?

        //!------------------------------------------------------------------------------------------------------------
            
        HandleEvents(event); 
        float dt = clock.getElapsedTime().asSeconds();
        Update(dt);
        clock.restart();
        Draw(); // Fonction pour dessiner les personnages la carte.
    }

    return _exit;
}

void Partie::HandleEvents(sf::Event event)
{
    while (_window->pollEvent(event)) {
          
            if(event.type == sf::Event::Closed){
                //_exit = 1;
                delete _server;
                delete _player;
                for (Personnage *charchater : _characterList)
                {
                    charchater->getOwner()->getSocket()->disconnect();
                }
                exit(1);
            } 
            for(Personnage * charchater : _characterList) 
                charchater->actions(event, _allowingMovement);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Delete))
            {
                _exit = 2;
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