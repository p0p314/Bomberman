#include "Personnage.hpp"


Personnage::Personnage(Player * player, Monde* level,skin apparence) : _player(player), _skin(apparence), _bomb(*this), _posSpriteAnimation(1,Down)
{
    _level = level;
    if(_skin == skin::titi)
    {
        _name = "titi";
        if (!_texture.loadFromFile("assets/Bomberman.png"))
            perror("texture bomber 'titi'");

        _sprite.setTexture(_texture);
        _sprite.setTextureRect(sf::IntRect(_posSpriteAnimation.x*_spriteWidth,_posSpriteAnimation.y*_spriteHeight,
                                              _spriteWidth,_spriteHeight));
        _sprite.setScale(sf::Vector2f(2.f,2.f));

        if(!_textureAtlas.loadFromFile("assets/bomberMort.png"))
            perror("Texture mort bomberman");

        std::vector<std::vector<TileMap *>> tiles = _level->getTiles();
        _spawnPos.x = tiles[1][1]->getSprite().getPosition().x;
        _spawnPos.y = tiles[1][1]->getSprite().getPosition().y;
        setPosition(_spawnPos); 
    } 

    if(_skin == skin::toto)
    {
        _name ="toto";
         if(!_texture.loadFromFile("assets/bomber.png")) perror("texture bomber 'toto'");
        
        _sprite.setTexture(_texture);
        _sprite.setTextureRect(sf::IntRect(_posSpriteAnimation.x*_spriteWidth,_posSpriteAnimation.y*_spriteHeight,
                                              _spriteWidth,_spriteHeight));
        _sprite.setScale(sf::Vector2f(2.f,2.f));

        if(!_textureAtlas.loadFromFile("assets/bomberMort.png"))
            perror("Texture mort bomberman");

        std::vector<std::vector<TileMap *>> tiles = _level->getTiles();
        _spawnPos.x = tiles[_level->getGridLength()-2][_level->getGridLength()-2]->getSprite().getPosition().x;
        _spawnPos.y =tiles[_level->getGridLength()-2][_level->getGridLength()-2]->getSprite().getPosition().y;
        setPosition(_spawnPos); 
    }

    
  
}

void Personnage::setPlayer(Player * player)
{
    _player = player;
}

sf::FloatRect Personnage::getCollisionZone()
{
    return _collisionZone;
}

void Personnage::updateCollisionZone()
{
    _collisionZone = sf::Rect<float>(getPosition().x,
                               getPosition().y + getSprite().getGlobalBounds().height - 15,
                               getSprite().getGlobalBounds().width,
                               15);
}

Bombe& Personnage::getBombe()
{
    return _bomb;
}

void Personnage::updateAnimation()
{
    _sprite.setTextureRect(sf::IntRect(_posSpriteAnimation.x*_spriteWidth,_posSpriteAnimation.y*_spriteHeight,
                                          _spriteWidth,_spriteHeight));
}

void Personnage::dying(float dt)
{

    if(_dying)
    {
        _sprite.setTexture(_textureAtlas);
        _sprite.setTextureRect(sf::IntRect(_posSpriteDeathAnimation*29,0,29,25));
        if(dt < 1 ) _posSpriteDeathAnimation = 1;
        else if(_posSpriteDeathAnimation <=4) _posSpriteDeathAnimation++; 
        if(_posSpriteDeathAnimation == 5) _dying = false;
    }

   
}

void Personnage::respawn()
{
    
    _dying = false;
    _alive = true;
    setPosition(_spawnPos);
    getSprite().setTextureRect(sf::IntRect(_posSpriteAnimation.x*_spriteWidth,
                                            _posSpriteAnimation.y* _spriteHeight,
                                            _spriteWidth,_spriteHeight));
    _sprite.setTexture(_texture);
}

void Personnage::startDeath()
{
    _lives--;
    _alive = false;
    _dying = true;
    _posSpriteDeathAnimation = 1;
}

void Personnage::move(sf::Vector2f movement)
{
    _sprite.move(movement);
    if(_level->isColision(this))
        _sprite.move(-movement);
}

void Personnage::actions(sf::Event event, float dt)
{   
        if(!_dying){  

            if((sf::Keyboard::isKeyPressed(sf::Keyboard::Z))) _player->action(sf::Uint8(1),dt);
        
            if((sf::Keyboard::isKeyPressed(sf::Keyboard::D)))_player->action(sf::Uint8(2),dt); 

         
            if((sf::Keyboard::isKeyPressed(sf::Keyboard::S)) ) _player->action(sf::Uint8(3),dt);

         
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) _player->action(sf::Uint8(4),dt);
           
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && _bombInHand) _player->action(sf::Uint8(0),dt);
            
        
        }
    
}


Personnage::skin Personnage::getSkin()
{
    return _skin;
}

void Personnage::Update(float dt, int dirFromPacket)
{
    _elapsedTime += dt;
    _elaspsedTimeDeath += dt;

    if(dirFromPacket == 0){
        if(getBombe().plant(_level))
        {
            _bombInHand = false;
            _level->getBombList().push_back(std::make_pair(this,&getBombe()));
        }
    }
    else if((dirFromPacket > 0 && dirFromPacket < 10) && !_dying ){
        float distance = _speed * dt;
        sf::Vector2f movement(0.0f, 0.0f);

        switch (dirFromPacket) {
            case 1:
                _posSpriteAnimation.y = Up;
                movement.y -= distance;
                break;
            case 2:
                _posSpriteAnimation.y = Right;
                movement.x += distance;
                break;
            case 3:
                _posSpriteAnimation.y = Down;
                movement.y += distance;
                break;
            case 4:
                _posSpriteAnimation.y = Left;
                movement.x -= distance;
                break;
            default:
                break;
        }

        _moving = true;
        move(movement);

    }  else _moving = false;
    
    dirFromPacket = 10; //!  = Aucune action 
    //!Gestion animation de déplacement
    if(_elapsedTime > _timeToChangeFrame)
    {
        if(_moving){
                _posSpriteAnimation.x--;
            if(_posSpriteAnimation.x * _spriteWidth >= getTexture().getSize().x)
                _posSpriteAnimation.x = 2;

            _elapsedTime = 0.f;
        } else  _posSpriteAnimation.x = 0;
        
        updateAnimation();
    } 

    //!Gestion bombe      
    if(!_bombInHand)
    {   
        std::cout << "bombe posee " << std::endl;
        _bomb.Update(dt);
        if(_bomb.isFireVisible())
            _level->isDestroyed(_bomb);    
          
        if(_bomb.isExploded()){
            std::cout << "Recuperation bombe " << std::endl;
            _bombInHand = true;
            _bomb.setExploded(false);
            auto posBombInList = std::find(_level->getBombList().begin(),  _level->getBombList().end(), 
                                 std::make_pair(this,&getBombe()));
       
            if(posBombInList != _level->getBombList().end())
                _level->getBombList().erase(posBombInList);

        }    
    }

    //! Gestion mort du personnage
    for(std::pair bomb :_level->getBombList() )
        if(bomb.second->isFireVisible() && bomb.second->fireContains(*this) && _alive)
        {
            startDeath(); 
            _elaspsedTimeDeath = 0;
        }   
     

    if(_dying && _elaspsedTimeDeath < 1.5f)
        dying(_elaspsedTimeDeath);
    

    if(!_alive && _elaspsedTimeDeath >= 3.5f && _posSpriteDeathAnimation == 5)
    {
        std::vector<std::vector<TileMap *>> tiles = _level->getTiles();
        _posSpriteAnimation.y = Down;
        respawn();
       
    }
}

void Personnage::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    if(!_bombInHand)
        target.draw(_bomb);
    
    if(_alive || _dying)target.draw(_sprite);
    
}

int Personnage::getLives()
{
    return _lives;
}

std::string Personnage::getName()
{
    return _name;
}
Player *  Personnage::getOwner()
{
    return _player;
}
