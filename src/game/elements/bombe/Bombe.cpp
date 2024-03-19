//
// Created by raximex on 28/01/24.
//

#include "Bombe.hpp"
#include <iostream>
#include "../personnage/Personnage.hpp"

Bombe::Bombe( Personnage& proprietaire) : _owner(proprietaire)
{
    if(!loadBomb("assets/bombes.png")){
        perror("chargement sprite bombe");
        return;
    }
  
    if(!loadFire()){
        perror("chargement sprite flammes");
    }

}

int Bombe::loadBomb(std::string lien){
    _texture.loadFromFile(lien);
    _sprite.setTexture(_texture);
    _sprite.setTextureRect(sf::IntRect(74,53,14,16));
    _sprite.setScale(sf::Vector2f(3.0f,3.0f));
    return 1;
}


std::vector<sf::Sprite> Bombe::getRays(){
    return _fireRays;
}

std::vector<Ray*> Bombe::get_Rays()
{
    return _fire_Rays;
}
void Bombe::Update(float dt)
{
    _elapsedTime += dt;
   // std::cout << "\t bombe -> visibilite : "<< _visibleBomb << std::endl;
   // std::cout << "\t bombe -> temps ecoule : "<< _elapsedTime << std::endl;

    //!Gerer animation bombe
    if(_visibleBomb && _elapsedTime >= _timeBeforeExplosion ){
        setFireVisibility(true);
         _elapsedTimeFireAnim += dt;
       // std::cout << "\t flammes visible"  << std::endl;
        if(_posFireSpriteAnimation <4 && _elapsedTimeFireAnim >= _timeToChangeFrame){

            _posFireSpriteAnimation++;
            for(Ray * ray : _fire_Rays)
                ray->updateRect(_posFireSpriteAnimation);
                /*_fireRays.at(i).setTextureRect(sf::IntRect(_posFireSpriteAnimation * _fireSpriteWidth,
                                                           i*_fireSpriteHeight,
                                                           _fireSpriteWidth,
                                                           _fireSpriteHeight));
                */
            _elapsedTimeFireAnim = 0.f;
        }

    }

    if(_visibleFire && _elapsedTime >= _timeEndOfExplosion){
       // std::cout << "\t Bombe explosée : invisible"  << std::endl;
        setVisibility(false);
        setFireVisibility(false);
        setExploded(true);
        _elapsedTime = 0;
        _posFireSpriteAnimation = 0;

    }
}

void Bombe::draw(sf::RenderTarget & target, sf::RenderStates states)  const 
{
    if(_visibleFire){
        for(Ray * ray : _fire_Rays)
            if(ray->getVisibility()) target.draw(ray->getSprite());

    } else target.draw(_sprite);

}

bool Bombe::isFireVisible()
{
    return _visibleFire;
}

int Bombe::loadFire(){   
    _fireTexture.loadFromFile("assets/fire_all.png");

    Ray::orientation orientations[] = { Ray::center, Ray::up, Ray::right, Ray::down, Ray::left };
    for(int i = 0; i < 5; i++){
        Ray * ray = new Ray(_fireTexture, orientations[i]);
        _fire_Rays.push_back(ray);

    }
    return 1;
}

bool Bombe::plant(Monde * monde)
{   
    std::vector<std::vector<TileMap*>>& tiles = monde->getTiles();

    for(int i = 0; i < monde->getGridLength() ; i++){
        for(int j = 0; j < monde->getGridLength(); j++)
            if(tiles[i][j]->containsCharacter(_owner) && tiles[i][j]->isTraversable()){

                _sprite.setPosition(tiles[i][j]->getSprite().getPosition().x ,
                                   tiles[i][j]->getSprite().getPosition().y  ); //!: Les position sont bizarre, a vérifier si changement de la taille du sprite de bombe
                for(Ray * ray : _fire_Rays)
                    ray->updatePosition(_sprite);
            
                return true;
            }
    }
    return false;
}

bool Bombe::fireContains(Personnage elm) 
{
    sf::Vector2f position{ elm.getPosition().x + elm.getTexture().getSize().x/2 , 
                           elm.getPosition().y + elm.getTexture().getSize().y/2 };
    
    for(Ray * ray : _fire_Rays)
        if(ray->contains(position)) return true;

    return false;
   
}



bool Bombe::isExploded()
{
    return _exploded;
}

void Bombe::setExploded(bool state)
{
    _exploded = state;
}


bool Bombe::isVisible()
{
    return _visibleBomb;
}
void Bombe::setVisibility(bool state){
    _visibleBomb = state;
}
void Bombe::setFireVisibility(bool state){
    _visibleFire = state;
}

