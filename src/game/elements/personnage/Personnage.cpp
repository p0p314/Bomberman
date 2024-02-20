//
// Created by raximex on 28/01/24.
//

#include "Personnage.hpp"


Personnage::Personnage(apparence apparence) : skin(apparence), bombe(*this), anim(1,Down)
{
   
    if(skin == apparence::titi){
        if(!texture.loadFromFile("assets/bomber.png")) perror("texture bomber 'titi'");
        sprite.setTexture(texture);
        nom = titi;
        setPosition(100,200);  
    } 
    destructible = true;
    traversable = false;
  
}

Bombe& Personnage::getBombe()
{
    return bombe;
}

