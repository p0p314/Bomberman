#include "Partie.hpp"

Partie::Partie(sf::RenderWindow * window) : _window(window)
{
    _level = new Monde();
    _level->initialisation();
    _characterList.push_back(new Personnage(_level, titi));
}

int Partie::Run()
{
    _exit = false;

    sf::Event event;
    sf::Clock clock;
    bool allowingMovement = false;

    while(!_exit){
        if(_characterList.size() > 1)
            allowingMovement = true;

        HandleEvents(event,true); //! Mettre variable allowingMovement
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