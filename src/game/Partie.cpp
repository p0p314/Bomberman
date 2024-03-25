#include "Partie.hpp"

Partie::Partie(sf::RenderWindow *window) : _window(window) // Le constructeur de cette classe initialise le niveau et les personnages.
{
    _level = new Monde();
    _level->initialisation();
    _characterList.push_back(new Personnage(_level, titi));
}

int Partie::Run() // Méthode appeler par le menu lorsque le joueur rejoint une partie.
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
        Draw(); // Fonction pour dessiner les personnages la carte.
    }

    return _exit;
}

void Partie::HandleEvents(sf::Event event, bool allowingMovement)
{
    while (_window->pollEvent(event)) {

        if (event.type == sf::Event::Closed)
            _exit = true;
        for (Personnage *character : _characterList) //! Ajouter l'autorisation de mouvement
            character->actions(event, allowingMovement);
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

    _window->clear();
    for (int i = 0; i < _level->getGridLength(); i++)
        for (int j = 0; j < _level->getGridLength(); j++)
            _window->draw(tiles[i][j]->getSprite());

    for (Personnage *character : _characterList)
        _window->draw(*character);

    _window->display();
}