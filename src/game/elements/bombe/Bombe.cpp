//
// Created by raximex on 28/01/24.
//

#include "Bombe.hpp"
#include <iostream>
#include "../personnage/Personnage.hpp"

Bombe::Bombe( Personnage& proprietaire) : proprietaire(proprietaire)
{
    if(!loadBombe("assets/bombes.png")){
        perror("chargement sprite bombe");
        return;
    }
  
    if(!loadFlammes()){
        perror("chargement sprite flammes");
    }
    destructible = false;
    traversable = true;
    timer.restart();
}

int Bombe::loadBombe(std::string lien){
    texture.loadFromFile(lien);
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(74,53,14,16));
    sprite.setScale(sf::Vector2f(3.0f,3.0f));
    return 1;
}

void Bombe::Update(float dt)
{
    _elapsedTime += dt;
   // std::cout << "\t bombe -> visibilite : "<< _visibleBomb << std::endl;
   // std::cout << "\t bombe -> temps ecoule : "<< _elapsedTime << std::endl;

    //!Gerer animation bombe
    if(_visibleBomb && _elapsedTime >= 3.0f && _posFireSpriteAnimation != 2){
        setFireVisibility(true);
       // std::cout << "\t flammes visible"  << std::endl;
        _posFireSpriteAnimation++;
        spriteFlammeCenter.setTextureRect(sf::IntRect(_posFireSpriteAnimation * 35,0,35,35));
        spriteFlammeUp.setTextureRect(sf::IntRect(_posFireSpriteAnimation * 35,0,35,35));
        spriteFlammeRight.setTextureRect(sf::IntRect(_posFireSpriteAnimation * 35,0,35,35));
        spriteFlammeDown.setTextureRect(sf::IntRect(_posFireSpriteAnimation * 35,0,35,35));
        spriteFlammeLeft.setTextureRect(sf::IntRect(_posFireSpriteAnimation * 35,0,35,35));
    }

    if(_visibleFire && _elapsedTime >= 5.0f){
       // std::cout << "\t Bombe explosée : invisible"  << std::endl;
        setVisibility(false);
        setFireVisibility(false);
        setExploded(true);
        _elapsedTime = 0;
        _posFireSpriteAnimation = 1;
    }
}

void Bombe::draw(sf::RenderTarget & target, sf::RenderStates states)  const 
{
    if(_visibleFire){
        target.draw(spriteFlammeCenter);
        target.draw(spriteFlammeUp);
        target.draw(spriteFlammeRight);
        target.draw(spriteFlammeDown);
        target.draw(spriteFlammeLeft);
    } else target.draw(sprite);

}
bool Bombe::isFireVisible()
{
    return _visibleFire;
}
int Bombe::loadFlammes(){
    textureFlammeCenter.loadFromFile("assets/explosion_center.png");
    textureFlammeUp.loadFromFile("assets/explosion_up.png");
    textureFlammeRight.loadFromFile("assets/explosion_right.png");
    textureFlammeDown.loadFromFile("assets/explosion_down.png");
    textureFlammeLeft.loadFromFile("assets/explosion_left.png");
    
    spriteFlammeCenter.setTexture(textureFlammeCenter);
    spriteFlammeUp.setTexture(textureFlammeUp);
    spriteFlammeRight.setTexture(textureFlammeRight);
    spriteFlammeDown.setTexture(textureFlammeDown);
    spriteFlammeLeft.setTexture(textureFlammeLeft);


    spriteFlammeCenter.setTextureRect(sf::IntRect(_posFireSpriteAnimation * 35,0,35,35));
    spriteFlammeUp.setTextureRect(sf::IntRect(_posFireSpriteAnimation * 35,0,35,35));
    spriteFlammeRight.setTextureRect(sf::IntRect(_posFireSpriteAnimation * 35,0,35,35));
    spriteFlammeDown.setTextureRect(sf::IntRect(_posFireSpriteAnimation * 35,0,35,35));
    spriteFlammeLeft.setTextureRect(sf::IntRect(_posFireSpriteAnimation * 35,0,35,35));

    return 1;
}

bool Bombe::poser(Monde * monde)
{   
    std::vector<std::vector<TileMap*>>& tiles = monde->getTiles();

    for(int i = 0; i < monde->getGridLength() ; i++){
        for(int j = 0; j < monde->getGridLength(); j++)
            if(tiles[i][j]->getSprite().getGlobalBounds().contains(
                        proprietaire.getPosition().x + proprietaire.getSprite().getGlobalBounds().width/2,
                        proprietaire.getPosition().y + proprietaire.getSprite().getGlobalBounds().height) && tiles[i][j]->isTraversable()){

                sprite.setPosition(tiles[i][j]->getSprite().getPosition().x ,
                                   tiles[i][j]->getSprite().getPosition().y  ); //!: Les position sont bizarre, a vérifier si changement de la taille du sprite de bombe

                spriteFlammeCenter.setPosition((int)sprite.getPosition().x, (int)sprite.getPosition().y);
                spriteFlammeUp.setPosition((int)sprite.getPosition().x, (int)sprite.getPosition().y-35);
                spriteFlammeRight.setPosition((int)sprite.getPosition().x+35, (int)sprite.getPosition().y);
                spriteFlammeDown.setPosition((int)sprite.getPosition().x, (int)sprite.getPosition().y+35);
                spriteFlammeLeft.setPosition((int)sprite.getPosition().x-35, (int)sprite.getPosition().y);

                return true;
            }
    }
    timer.restart();
    return false;
}

bool Bombe::flammesContains(Personnage elm) 
{
    sf::Vector2f position{elm.getPosition().x + elm.getTexture().getSize().x/2 , elm.getPosition().y + elm.getTexture().getSize().y/2};
    if(spriteFlammeCenter.getGlobalBounds().contains(position) ||
        spriteFlammeUp.getGlobalBounds().contains(position) ||
        spriteFlammeRight.getGlobalBounds().contains(position) ||
        spriteFlammeDown.getGlobalBounds().contains(position) ||
        spriteFlammeLeft.getGlobalBounds().contains(position) ) return true;
    else return false;
    
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
void Bombe::exploser(int up, int left, int down, int right){}


bool Bombe::isVisible()
{
    return _visibleBomb;
}
void Bombe::setVisibility(bool state){
    timer.restart();
    _visibleBomb = state;
}
void Bombe::setFireVisibility(bool state){
    _visibleFire = state;
}
sf::Vector2i& Bombe::getAnim(){
    return anim;
}
