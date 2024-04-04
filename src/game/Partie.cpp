#include "Partie.hpp"

/*
 * window : fenetre de jeu
 * player : Joueur à rejoindre ou créer une partie
 ! typeOfPlayer : host( =0 par defaut) ou guest(=1)
 * Cette méthode permet de créer ou de rejoindre une partie
*/
Partie::Partie(sf::RenderWindow * window, Player * player, sf::Uint8 typeOfPlayer) : _window(window), _player(player)
{
        
    
    std::cout<< "creation partie" << std::endl;
    if(_player->joinAGame())
        if(!_player->signalArrival(typeOfPlayer,static_cast<sf::Uint8>(2)))  
            returnToMenu();
        else 
        {  
            checkRecievedPacket();
           
            std::cout <<"Nombre de joueur pour la partie : " << static_cast<int>(_numberOfPlayer) << std::endl;;
    
            _level = new Monde();
            _level->initialisation();
            createCharacters();
           
            _gameInfo = new Gui();
            sf::Font * font = new sf::Font();
            if(!font->loadFromFile("assets/fonts/LiberationMono-Bold.ttf"))
            {
                std::cerr << "Impossible de charger la font BondiBraveDemoRegular " <<std::endl;
                std::exit(1);
            }
            _gameInfo->init(font, window->getSize().x, window->getSize().y);
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

void Partie::createCharacters()
{
    if(_characterList.size() != _numberOfPlayer)
    {
        Personnage::skin skins[] = {Personnage::titi,Personnage ::toto};
        for (int j = 0; j < _numberOfPlayer; j++)
        {
            Personnage *p = new Personnage(_player,_level, skins[j]);
            _characterList.push_back(p);
        }
    }
}

int Partie::Run() // Méthode appelé par le menu lorsque le joueur rejoint une partie.
{
    sf::Music music;
    sf::Event event;
    sf::Clock clock;
    std::cout << "dans run " << std::endl;

    if (!music.openFromFile("assets/music/cruising-down-8bit-lane-159615.ogg"))
        return -1;
    music.setVolume(30);
    music.setLoop(true);
    music.play();

    if (!_exit && _characterList.size() == _numberOfPlayer)
        _player->listReady();

    while (!_exit )
    {
        float dt = clock.getElapsedTime().asSeconds();
        HandleEvents(event, dt);
        checkRecievedPacket(dt);     
        clock.restart();

        Draw();
    }
    std::cout << _player->numberOfActionSent << " || " << _player->numberOfActionRecieved << std::endl;
    
    _player->exitGame(_forcedExit);

    return _exitToMenu;
}

void Partie::returnToMenu()
{
    _exit = true;
    _exitToMenu = true;
}

void Partie::checkRecievedPacket(float dt)
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
                    std::cout << "Un joueur a quiitte la partie : Fin de la partie"<< std::endl;
                    _forcedExit = true;
                }
                _exit = true;
                _exitToMenu = true;
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
                        if(charchater->getSkin() == static_cast<int>(_player->getID()))
                            _player->numberOfActionRecieved++;
                        return;
                    }                   
                    
                 
            }else  std::cout <<"Type de paquet non pris en charge" << std::endl;
        } else std::cout <<"Erreur format du paquet" << std::endl;
    } 
    else Update(dt);
    
}


void Partie::HandleEvents(sf::Event event, float dt)
{
    while (_window->pollEvent(event)) {
          
            if(event.type == sf::Event::Closed ){
               _exit = true;
               _exitToMenu = false;
               break;
            } 
            
            if(_synchronised && _window->hasFocus())
                for(Personnage * charchater : _characterList) 
                    if(charchater->getSkin() == static_cast<int>(_player->getID()))
                        if(_player->getDebouncer().update(dt)) 
                            charchater->actions(event, dt);                     //!Utiliser player->character à la place
                           
                        

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Delete))
            {
                _exit = true;
                _exitToMenu = true;
            }
    }

    if(!_synchronised && !_startingGame)
    {
        sf::Packet packet;
        if(_player->getSocket()->receive(packet) == sf::Socket::Done)
        {    
            if(packet >> _startGame )
                if(_startGame == static_cast<int>(4))
                    _startingGame = true;

        }
    }

}

void Partie::Update(float dt)
{
 
    if(_startingGame)
    {
        _startingGameCounter += dt;
        
        if(_gameCounter > -1)
        {
            if(_startingGameCounter >= _timeToChangeCount)
            {
                std::cout <<  _gameCounter-- <<std::endl;
                _startingGameCounter = 0;
            }
            //!Afficher le compteur sur l'ecran de jeu
        } 
        else
        {
            std::cout << "jouer" <<std::endl;
            _synchronised = true;
            _startingGame = false;
        }
    }

    for (Personnage *character : _characterList)
    {
        character->Update(dt);
        if(character->getLives() == 0) _endofGame = true;
    }
    
    if(_endofGame) _synchronised = false;
    
    _gameInfo->updateStats(&_characterList);
    
      
    

    if(_startingGame)
    {
        _gameInfo->updateStats(_gameCounter);
    }
    
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

    _window->draw(*_gameInfo);
    
    if(_startingGame) _window->draw(*_gameInfo);
    
    _window->display();
}