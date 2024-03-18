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
        sprite.setScale(sf::Vector2f(2.f,2.f));
        nom = titi;
        std::vector<std::vector<TileMap *>> tiles = _level->getTiles();
        setPosition(tiles[1][1]->getSprite().getPosition().x ,
                    tiles[1][1]->getSprite().getPosition().y  );  

    } 

    if(!textureAtlas.loadFromFile("assets/bomberMort.png"))
        perror("Texture bomberman general");

    destructible = true;
    traversable = false;


  
}

sf::Sprite & Personnage::getSpriteMort(){
    return _deathSprite;
}

Bombe& Personnage::getBombe()
{
    return _bomb;
}

void Personnage::mourir(float dt)
{

    if(_dying)
    {
        _alive = false;
        sprite.setTexture(textureAtlas);
        sprite.setTextureRect(sf::IntRect(_posSpriteDeathAnimation*29,0,29,25));
        if(dt < 1 ) _posSpriteDeathAnimation = 1;
        else if(_posSpriteDeathAnimation <=4) _posSpriteDeathAnimation++;  
    }

   
}

int Personnage::getAnimMort()
{
    return _posSpriteDeathAnimation;
}
bool Personnage::vivant()
{
    return _alive;
}

void Personnage::resurection()
{
    viesRestantes--;
    _dying = false;
    _alive = true;
    getSprite().setTextureRect(sf::IntRect(_posSpriteAnimation.x*_spriteWidth,
                                            _posSpriteAnimation.y* _spriteHeight,
                                            _spriteWidth,_spriteHeight));
    sprite.setTexture(texture);
}

void Personnage::demarrerMort()
{
    _posSpriteDeathAnimation = 1;
    _dying = true;
}

bool Personnage::isDying()
{
    return _dying;
}

bool Personnage::isBombInHand()
{
    return _bombInHand;
}

void Personnage::bombCollied()
{

}

void Personnage::actions(sf::Event event)
{    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)){ 
        _posSpriteAnimation.y = Up; 
        sprite.move(0,-_speed);
        if(_level->isColision(this)){
            sprite.move(0,_speed);
        }
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
        if(getBombe().poser(_level)){
            getBombe().setVisibility(true);
           _bombInHand = false;
           _level->getBombList().push_back(std::make_pair(this,&getBombe()));
        }   
    }
    
}

void Personnage::Update(float dt)
{
    _elapsedTime += dt;
    _elaspsedTimeDeath += dt;
    //_elaspedTimeBomb += dt;
    if(_elapsedTime > _timeToChangeFrame)
    {
        _posSpriteAnimation.x--;
       // std::cout << _posSpriteAnimation << std::endl;
        if(_posSpriteAnimation.x * _spriteWidth >= getTexture().getSize().x){
            _posSpriteAnimation.x = 2;
            sprite.setTextureRect(sf::IntRect(_posSpriteAnimation.x*_spriteWidth,_posSpriteAnimation.y*_spriteHeight,
                                              _spriteWidth,_spriteHeight));

        } 
            _elapsedTime = 0.f;
    }       
    if(!_bombInHand)
    {   
       // std::cout << "bombe posee " << std::endl;
        _bomb.Update(dt);
        if(_bomb.isFireVisible())
            _level->isDestruction(_bomb);    
          
        if(_bomb.getIsExploded()){
           // std::cout << "Recuperation bombe " << std::endl;
            _bombInHand = true;
            _bomb.setExploded(false);
            auto pos = std::find(_level->getBombList().begin(), 
                                 _level->getBombList().end(), 
                                 std::make_pair(this,&getBombe()));
        
            if(pos != _level->getBombList().end())
                _level->getBombList().erase(pos);

        }    
    }

    for(std::pair bomb :_level->getBombList() )
    {
        if(bomb.second->isFireVisible() && bomb.second->flammesContains(*this) && _alive)
        {
            demarrerMort();
            _elaspsedTimeDeath = 0;
        }   
    } 

    if(_dying && _elaspsedTimeDeath < 1.5f)
    {
        std::cout << "ccc" << std::endl; 
        mourir(_elaspsedTimeDeath);
        if(_posSpriteDeathAnimation == 5) _dying = false;
    }

    if(!_alive && _elaspsedTimeDeath >= 3.5f && _posSpriteDeathAnimation == 5)
    {
        std::vector<std::vector<TileMap *>> tiles = _level->getTiles();
        setPosition(tiles[1][1]->getSprite().getPosition().x,
                    tiles[1][1]->getSprite().getPosition().y - getSprite().getGlobalBounds().height/2);
        _posSpriteAnimation.y = Down;
        resurection();
       
    }
}

void Personnage::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    if(!_bombInHand){
        target.draw(_bomb);
    }

    if(_alive || _dying)target.draw(sprite);

    
}