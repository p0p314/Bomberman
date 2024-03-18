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
    destructible = false;
    traversable = true;
}

int Bombe::loadBomb(std::string lien){
    texture.loadFromFile(lien);
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(74,53,14,16));
    sprite.setScale(sf::Vector2f(3.0f,3.0f));
    return 1;
}
std::vector<sf::Sprite> Bombe::getRays(){
    return _fireRays;
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
            for(int i =0; i < 5; i++)
                _fireRays.at(i).setTextureRect(sf::IntRect(_posFireSpriteAnimation * _fireSpriteWidth,
                                                           i*_fireSpriteHeight,
                                                           _fireSpriteWidth,
                                                           _fireSpriteHeight));
            _elapsedTimeFireAnim = 0.f;
        }

       /* spriteFlammeCenter.setTextureRect(sf::IntRect(_posFireSpriteAnimation * 35,0,35,35));
        spriteFlammeUp.setTextureRect(sf::IntRect(_posFireSpriteAnimation * 35,0,35,35));
        spriteFlammeRight.setTextureRect(sf::IntRect(_posFireSpriteAnimation * 35,0,35,35));
        spriteFlammeDown.setTextureRect(sf::IntRect(_posFireSpriteAnimation * 35,0,35,35));
        spriteFlammeLeft.setTextureRect(sf::IntRect(_posFireSpriteAnimation * 35,0,35,35));
        */
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
       /* target.draw(spriteFlammeCenter);
        target.draw(spriteFlammeUp);
        target.draw(spriteFlammeRight);
        target.draw(spriteFlammeDown);
        target.draw(spriteFlammeLeft);
       */ 

        for(sf::Sprite ray : _fireRays)
            target.draw(ray);
    } else target.draw(sprite);

}
bool Bombe::isFireVisible()
{
    return _visibleFire;
}
int Bombe::loadFire(){
   /* textureFlammeCenter.loadFromFile("assets/explosion_center.png");
    textureFlammeUp.loadFromFile("assets/explosion_up.png");
    textureFlammeRight.loadFromFile("assets/explosion_right.png");
    textureFlammeDown.loadFromFile("assets/explosion_down.png");
    textureFlammeLeft.loadFromFile("assets/explosion_left.png");
    */
   
    _fireTexture.loadFromFile("assets/fire_all.png");
    for(int i = 0; i < 5; i++){
        _fireRays.push_back(sf::Sprite(_fireTexture,
                           sf::IntRect(_posFireSpriteAnimation * _fireSpriteWidth,
                                        i * _fireSpriteHeight,
                                        _fireSpriteWidth,
                                        _fireSpriteHeight)));
    }

  /*  spriteFlammeCenter.setTexture(textureFlammeCenter);
    spriteFlammeUp.setTexture(textureFlammeUp);
    spriteFlammeRight.setTexture(textureFlammeRight);
    spriteFlammeDown.setTexture(textureFlammeDown);
    spriteFlammeLeft.setTexture(textureFlammeLeft);


    spriteFlammeCenter.setTextureRect(sf::IntRect(_posFireSpriteAnimation * 35,0,35,35));
    spriteFlammeUp.setTextureRect(sf::IntRect(_posFireSpriteAnimation * 35,0,35,35));
    spriteFlammeRight.setTextureRect(sf::IntRect(_posFireSpriteAnimation * 35,0,35,35));
    spriteFlammeDown.setTextureRect(sf::IntRect(_posFireSpriteAnimation * 35,0,35,35));
    spriteFlammeLeft.setTextureRect(sf::IntRect(_posFireSpriteAnimation * 35,0,35,35));
  */
    return 1;
}

bool Bombe::plant(Monde * monde)
{   
    std::vector<std::vector<TileMap*>>& tiles = monde->getTiles();

    for(int i = 0; i < monde->getGridLength() ; i++){
        for(int j = 0; j < monde->getGridLength(); j++)
            if(tiles[i][j]->getSprite().getGlobalBounds().contains(
                        _owner.getPosition().x + _owner.getSprite().getGlobalBounds().width/2,
                        _owner.getPosition().y + _owner.getSprite().getGlobalBounds().height) && tiles[i][j]->isTraversable()){

                sprite.setPosition(tiles[i][j]->getSprite().getPosition().x ,
                                   tiles[i][j]->getSprite().getPosition().y  ); //!: Les position sont bizarre, a vérifier si changement de la taille du sprite de bombe

               // spriteFlammeCenter.setPosition((int)sprite.getPosition().x, (int)sprite.getPosition().y);
               // spriteFlammeUp.setPosition((int)sprite.getPosition().x, (int)sprite.getPosition().y-35);
               // spriteFlammeRight.setPosition((int)sprite.getPosition().x+35, (int)sprite.getPosition().y);
               // spriteFlammeDown.setPosition((int)sprite.getPosition().x, (int)sprite.getPosition().y+35);
               // spriteFlammeLeft.setPosition((int)sprite.getPosition().x-35, (int)sprite.getPosition().y);
               
                _fireRays.at(0).setPosition((int)sprite.getPosition().x, (int)sprite.getPosition().y);
                _fireRays.at(1).setPosition((int)sprite.getPosition().x, (int)sprite.getPosition().y-_fireSpriteHeight);
                _fireRays.at(2).setPosition((int)sprite.getPosition().x+_fireSpriteHeight, (int)sprite.getPosition().y);
                _fireRays.at(3).setPosition((int)sprite.getPosition().x, (int)sprite.getPosition().y+_fireSpriteHeight);
                _fireRays.at(4).setPosition((int)sprite.getPosition().x-_fireSpriteWidth, (int)sprite.getPosition().y);

                return true;
            }
    }
    return false;
}

bool Bombe::flammesContains(Personnage elm) 
{
    sf::Vector2f position{elm.getPosition().x + elm.getTexture().getSize().x/2 , elm.getPosition().y + elm.getTexture().getSize().y/2};
    
   
    for(sf::Sprite ray : _fireRays){
        if(ray.getGlobalBounds().contains(position)) return true;
    }

    return false;
    /*if(spriteFlammeCenter.getGlobalBounds().contains(position) ||
        spriteFlammeUp.getGlobalBounds().contains(position) ||
        spriteFlammeRight.getGlobalBounds().contains(position) ||
        spriteFlammeDown.getGlobalBounds().contains(position) ||
        spriteFlammeLeft.getGlobalBounds().contains(position) ) return true;
    else return false;
    */
}

bool Bombe::getIsExploded()
{
    return _exploded;
}

void Bombe::setExploded(bool state)
{
    _exploded = state;
}
sf::Sprite& Bombe::getSPriteBombe(){
    return sprite;
}

sf::Sprite& Bombe::getSpriteFlammeCenter(){

    return spriteFlammeCenter;
}

sf::Sprite& Bombe::getSpriteFlammeUp(){

    return spriteFlammeUp;
}
sf::Sprite& Bombe::getSpriteFlammeRight(){

    return spriteFlammeRight;
}
sf::Sprite& Bombe::getSpriteFlammeDown(){

    return spriteFlammeDown;
}

sf::Sprite& Bombe::getSpriteFlammeLeft(){

    return spriteFlammeLeft;
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

