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
       
}

int Bombe::loadBombe(std::string lien){
    texture.loadFromFile(lien);
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(74,53,14,16));
    return 1;
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


    spriteFlammeCenter.setTextureRect(sf::IntRect(0,0,35,35));
    spriteFlammeUp.setTextureRect(sf::IntRect(0,0,35,35));
    spriteFlammeRight.setTextureRect(sf::IntRect(0,0,35,35));
    spriteFlammeDown.setTextureRect(sf::IntRect(0,0,35,35));
    spriteFlammeLeft.setTextureRect(sf::IntRect(0,0,35,35));

    return 1;
}
bool Bombe::poser(Monde monde)
{
    for(int i = 0; i < monde.gridLength ; i++){
        for(int j = 0; j < monde.gridLength; j++)
            if(monde.tiles[i][j]->getSprite().getGlobalBounds().contains(
                        proprietaire.getPosition().x + proprietaire.getSprite().getGlobalBounds().width/2,
                        proprietaire.getPosition().y + proprietaire.getSprite().getGlobalBounds().height) && monde.tiles[i][j]->isFree){

                sprite.setPosition(monde.tiles[i][j]->getSprite().getPosition().x ,
                                   monde.tiles[i][j]->getSprite().getPosition().y  ); //!: Les position sont bizarre, a vérifier si changement de la taille du sprite de bombe

                spriteFlammeCenter.setPosition((int)sprite.getPosition().x, (int)sprite.getPosition().y);
                spriteFlammeUp.setPosition((int)sprite.getPosition().x, (int)sprite.getPosition().y-35);
                spriteFlammeRight.setPosition((int)sprite.getPosition().x+35, (int)sprite.getPosition().y);
                spriteFlammeDown.setPosition((int)sprite.getPosition().x, (int)sprite.getPosition().y+35);
                spriteFlammeLeft.setPosition((int)sprite.getPosition().x-35, (int)sprite.getPosition().y);

                return true;
            }
    }
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

bool Bombe::est_visible() const{
    return visible;
}

bool Bombe::est_visibleFlammes(){
    return flammeVisible;
}

void Bombe::toggleVisibilite(bool etat){
    visible = etat;
}
void Bombe::toggleVisibiliteFlammes(bool etat){
    flammeVisible = etat;
}
sf::Vector2i& Bombe::getAnim(){
    return anim;
}
