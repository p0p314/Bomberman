//
// Created by raximex on 28/01/24.
//

#include "Personnage.hpp"


Personnage::Personnage(Monde* level,apparence apparence) : skin(apparence), _bomb(*this), _posSpriteAnimation(1,Down)
{
    _level = level;
    if(skin == apparence::titi){
        if(!texture.loadFromFile("assets/bomber.png")) perror("texture bomber 'titi'");
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(_posSpriteAnimation.x*_spriteWidth,_posSpriteAnimation.y*_spriteHeight,
                                              _spriteWidth,_spriteHeight));
        sprite.setScale(sf::Vector2f(2.f,2.f));
        nom = titi;
    } 


    if(!_textureAtlas.loadFromFile("assets/bomberMort.png"))
        perror("Texture bomberman general");


    std::vector<std::vector<TileMap *>> tiles = _level->getTiles();
    setPosition(tiles[1][1]->getSprite().getPosition().x ,
                tiles[1][1]->getSprite().getPosition().y  ); 
    

    destructible = true;
    traversable = false;


  
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

void Personnage::dying(float dt)
{

    if(_dying)
    {
        _alive = false;
        sprite.setTexture(_textureAtlas);
        sprite.setTextureRect(sf::IntRect(_posSpriteDeathAnimation*29,0,29,25));
        if(dt < 1 ) _posSpriteDeathAnimation = 1;
        else if(_posSpriteDeathAnimation <=4) _posSpriteDeathAnimation++; 
        if(_posSpriteDeathAnimation == 5) _dying = false;
    }

   
}



void Personnage::respawn()
{
    viesRestantes--;
    _dying = false;
    _alive = true;
    getSprite().setTextureRect(sf::IntRect(_posSpriteAnimation.x*_spriteWidth,
                                            _posSpriteAnimation.y* _spriteHeight,
                                            _spriteWidth,_spriteHeight));
    sprite.setTexture(texture);
}

void Personnage::startDeath()
{
    _posSpriteDeathAnimation = 1;
    _dying = true;
}





void Personnage::actions(sf::Event event, bool allowingMovement)
{    
    if(allowingMovement){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Q) )
        {
            _moving = true;
        } else _moving = false;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)){
            _posSpriteAnimation.y = Up; 
            sprite.move(0,-_speed);
            if(_level->isColision(this))
                sprite.move(0,_speed);    
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){ 
            _posSpriteAnimation.y = Right; 
            sprite.move(_speed,0);
            if(_level->isColision(this)) 
                getSprite().move(-_speed,0);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){ 
            _posSpriteAnimation.y = Down; 
            sprite.move(0,_speed);
            if(_level->isColision(this)) 
                getSprite().move(0,-_speed);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){ 
            _posSpriteAnimation.y = Left; 
            sprite.move(-_speed,0);
            if(_level->isColision(this)) 
                getSprite().move(_speed,0);
        }   

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && _bombInHand) { 
            if(getBombe().plant(_level)){
                getBombe().setVisibility(true);
               _bombInHand = false;
               _level->getBombList().push_back(std::make_pair(this,&getBombe()));
            }   
        }
    }
}

void Personnage::Update(float dt)
{
    _elapsedTime += dt;
    _elaspsedTimeDeath += dt;

    //!Gestion animation de déplacement
    if(_elapsedTime > _timeToChangeFrame && _moving)
    {
        _posSpriteAnimation.x--;
        if(_posSpriteAnimation.x * _spriteWidth >= getTexture().getSize().x)
            _posSpriteAnimation.x = 2;
        
        sprite.setTextureRect(sf::IntRect(_posSpriteAnimation.x*_spriteWidth,_posSpriteAnimation.y*_spriteHeight,
                                          _spriteWidth,_spriteHeight));
        _elapsedTime = 0.f;
    } 

    //!Gestion bombe      
    if(!_bombInHand)
    {   
       // std::cout << "bombe posee " << std::endl;
        _bomb.Update(dt);
        if(_bomb.isFireVisible())
            _level->isDestroyed(_bomb);    
          
        if(_bomb.isExploded()){
           // std::cout << "Recuperation bombe " << std::endl;
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
        if(bomb.second->isFireVisible() && bomb.second->rayContains(*this) && _alive)
        {
            startDeath();
            _elaspsedTimeDeath = 0;
        }   
     

    if(_dying && _elaspsedTimeDeath < 1.5f)
        dying(_elaspsedTimeDeath);
    

    if(!_alive && _elaspsedTimeDeath >= 3.5f && _posSpriteDeathAnimation == 5)
    {
        std::vector<std::vector<TileMap *>> tiles = _level->getTiles();
        setPosition(tiles[1][1]->getSprite().getPosition().x,
                    tiles[1][1]->getSprite().getPosition().y - getSprite().getGlobalBounds().height/2);
        _posSpriteAnimation.y = Down;
        respawn();
       
    }
}

void Personnage::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    if(!_bombInHand)
        target.draw(_bomb);
    
    if(_alive || _dying)target.draw(sprite);
    
}