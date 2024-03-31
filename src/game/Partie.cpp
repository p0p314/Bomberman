#include "Partie.hpp"

/*
 * window : fenetre de jeu
 * player : Joueur à rejoindre ou créer une partie
 ! typeOfPlayer : host( =0 par defaut) ou guest(=1)
 * Cette méthode permet de créer ou de rejoindre une partie
*/
Partie::Partie(sf::RenderWindow * window, Player * player, sf::Uint8 typeOfPlayer) : _window(window), _player(player)
{
    if(_player->joinAGame())
        if(!_player->signalArrival(typeOfPlayer,static_cast<sf::Uint8>(2)))  
            returnToMenu();
        else 
        {  
            checkRecievedPacket();
           
            std::cout <<"Nombre de joueur pour la partie : " << static_cast<int>(_numberOfPlayer) << std::endl;;
    
            _level = new Monde();
            _level->initialisation();

             if (_characterList.size() != _numberOfPlayer)
            {
                Personnage::skin skins[] = {Personnage::titi,Personnage ::toto};
                for (int j = 0; j < _numberOfPlayer; j++)
                {
                    Personnage *p = new Personnage(_player,_level, skins[j]);
                    _characterList.push_back(p);
                }
            }
        }
    else returnToMenu();  
}


Partie::~Partie()
{
    delete _level;
    if(!_characterList.empty())
        for(Personnage * character : _characterList)
            delete character;
    delete _lobby;
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

    sf::Packet packet;
    packet << static_cast<sf::Uint8>(3);
    if(!_exit)
    {
        _player->getSocket()->setBlocking(true);
        if(_player->getSocket()->send(packet) == sf::Socket::Done)
            std::cout << "info liste prete envoye" << std::endl;
         else std::cout << "echec envoi info liste prete " << std::endl;
        _player->getSocket()->setBlocking(false);
    }
 
    packet.clear();  
    while (!_exit )
    { 
        synchronisation();
        if(_synchronised)
        {
            float dt = clock.getElapsedTime().asSeconds();
            HandleEvents(event, dt); 
            if(!checkRecievedPacket(dt));
                Update(dt);
            clock.restart();
        }
        Draw(); 
    }
    _player->quiteGame();

    return _exitToMenu;
}

void Partie::returnToMenu()
{
    _exit = true;
    _exitToMenu = true;
}

bool Partie::checkRecievedPacket(float dt)
{
    sf::Packet packet;
    if(_player->getSocket()->receive(packet) == sf::Socket::Done)
    {
        std::cout << "paquet recu ";
        if(packet >> _typeOfPacket)
        {
            std::cout << static_cast<int>(_typeOfPacket)<< " --> ";
            if(_typeOfPacket == static_cast<sf::Uint8>(1)) //accepté dans lobby
            {
                _player->getSocket()->setBlocking(false);
                packet >> _numberOfPlayer;
                packet >> _idPlayer;
                std::cout << "nombre de joueurs : "<< static_cast<int>(_numberOfPlayer) <<std::endl;
                
                _player->setID(_idPlayer);
                std::cout<< "id du joueur :"<<static_cast<int>(_player->getID()) << std::endl;
                _lobby = new Lobby(_window, _player, _numberOfPlayer);
                if((_exit = _lobby->Run()))
                {
                    _exitToMenu = _lobby->getExitToMenu();   
                    return false;
                }
            }  //Si deconnexion ou erreur
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
                    std::cout << "Un joueur a quiitte la partie : "<< std::endl;
                }
                _exit = true;
                _exitToMenu = true;
                return false;
            } 
            else if(_typeOfPacket == static_cast<sf::Uint8>(5)) //! Action de jeu
            {
                packet >> _idSender;
                packet >> _action;
                std::cout<< "idSender ::: " << static_cast<int>(_idSender) <<std::endl; 
                for(Personnage * charchater : _characterList)
                    if(charchater->getSkin() == static_cast<int>(_idSender))
                    {
                        charchater->Update(dt, static_cast<int>(_action));
                        return true;
                    }                   
                    
                 
            }else  std::cout <<"Type de paquet non pris en charge" << std::endl;
        } else std::cout <<"Erreur format du paquet" << std::endl;
    } else return false;
    return false;
}

void Partie::synchronisation()
{
    sf::Packet packet;
    if (!_synchronised)
    {
        if(_player->getSocket()->receive(packet) == sf::Socket::Done)
        {    
            if(packet >> _startGame )
                if(_startGame == static_cast<int>(4))
                {
                    sf::Clock synchronisation;
                    float dt = 0.f;  
                    while(synchronisation.getElapsedTime().asSeconds() < 3)
                    {
                        
                        
                    }//!Afficher le compteur sur l'ecran de jeu
                     std::cout << "jouer" << std::endl;
                    _synchronised = true;
                    _allowingMovement = true;
                }
        }
    }
}

void Partie::HandleEvents(sf::Event event, float dt)
{
    while (_window->pollEvent(event)) {
          
            if(event.type == sf::Event::Closed ){
               _exit = true;
               _exitToMenu = false;
               break;
            } 
            if(_synchronised)
                for(Personnage * charchater : _characterList) 
                    charchater->actions(event, _allowingMovement, dt);

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