//#include "src/Graphics/Graphics.h"
#include "main.hpp"
#include "game/Graphics/Graphics.h"
#include "game/carte/Carte.cpp"

using namespace sf;

void inputHandlerJoueur(Event event, RenderWindow &window, Personnage& perso);
void loadJoueur(Texture &JoueurT, Sprite &JoueurS);
bool updateFPS = false;
Personnage perso1(titi);
sf::Vector2i anim = perso1.anim;
int P_WIDTH = 16,
    P_HEIGHT = 24;
sf::Clock clock_bombe;

bool clic_g = false;

int main() {
    ContextSettings settings;
    settings.antialiasingLevel = 8;
    RenderWindow window(sf::VideoMode(1600, 900), "BOMBERMAN", sf::Style::Default, settings);
    Carte map;

    int animFlammes = 1;
    
    
   /* const int lvl[] = {
        0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,
        24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50
        };
  
    if(!map.load("C:/Users/cedri/Documents/GitHub/BombermanBis/assets/Map.png",sf::Vector2u(32,32),lvl,16,8)){
        std::cout << "erreur" << endl;
    }*/
    
    sf::Clock clock;
    float fpsCount = 0,
          switchFPS = 100,
          fpsSpeed = 500;
    
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) window.close();

            inputHandlerJoueur(event,window,perso1);
            
        }


        if(updateFPS){
            if(clock.getElapsedTime().asMilliseconds() >=220)
            {
                anim.x--;
                if(anim.x * P_WIDTH >= perso1.getTexture().getSize().x) anim.x = 2;
                if(perso1.getBombe().est_visibleFlammes()){
                    animFlammes++;
                    if(animFlammes * 35 >= perso1.getBombe().getSpriteFlammeCenter().getTexture()->getSize().x) animFlammes = 1;
                    if(animFlammes * 35 >= perso1.getBombe().getSpriteFlammeDown().getTexture()->getSize().x) animFlammes = 1;
                }
                clock.restart();
            }
        }

        
        
        window.clear();

        //! Gestion affichage Bombe
        if(perso1.getBombe().est_visible() && clock_bombe.getElapsedTime().asSeconds() < 3 ){
            perso1.getBombe().getSPriteBombe().setScale(sf::Vector2f(3.0f,3.0f));
            window.draw(perso1.getBombe().getSprite());
        }
        if(perso1.getBombe().est_visible() && clock_bombe.getElapsedTime().asSeconds() >= 3 ){
                perso1.getBombe().toggleVisibiliteFlammes(true);
                //perso1.getBombe().getSpriteFlammeCenter().setTextureRect(sf::IntRect(animFlammes * 35,0,35,35));
                //perso1.getBombe().getSpriteFlammeDown().setTextureRect(sf::IntRect(animFlammes * 35,0,35,35));
                window.draw(perso1.getBombe().getSpriteFlammeCenter()); 
                window.draw(perso1.getBombe().getSpriteFlammeUp()); 
                window.draw(perso1.getBombe().getSpriteFlammeRight()); 
                window.draw(perso1.getBombe().getSpriteFlammeDown()); 
                window.draw(perso1.getBombe().getSpriteFlammeLeft());
        }
        if(perso1.getBombe().est_visible() && clock_bombe.getElapsedTime().asSeconds() >= 5){
            perso1.getBombe().toggleVisibilite(false);
            perso1.getBombe().toggleVisibiliteFlammes(false);

            clic_g = false;
        }
        //!----Gestion affichage joueur 1---------------------------------------------------------------------
        
        std::cout << "clock " << clock_bombe.getElapsedTime().asSeconds()<< endl;            
        perso1.getSprite().setTextureRect(sf::IntRect(anim.x*P_WIDTH,anim.y* P_HEIGHT,P_WIDTH,P_HEIGHT));
        
        perso1.getSprite().setScale(sf::Vector2f(3.0f,3.0f));
        window.draw(perso1.getSprite());
        
        window.display();
        updateFPS = false;
    }
}


void inputHandlerJoueur(Event event, RenderWindow &window, Personnage &perso)
{
    if(event.type == sf::Event::Closed){
        window.close();
    }
    sf::Vector2f  position = perso.getPosition();
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        updateFPS = true;
        anim.y = perso.Right; 
        perso.deplacer(sf::Vector2f (1,0));

    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)){
        updateFPS = true;
        anim.y = perso.Up;
        perso.deplacer(sf::Vector2f (0,-1));
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
        updateFPS = true;
        anim.y = perso.Down;
        perso.deplacer(sf::Vector2f (0,1));
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
        updateFPS = true;
        anim.y = perso.Left;
        perso.getSprite().move(sf::Vector2f (-1,0));
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !clic_g) { 
         perso.getBombe().poser();
         perso.getBombe().toggleVisibilite(true);
         clic_g = true;
         //std::cout << "fonc : etat bombe" << perso.getBombe().toggleVisibilite << endl;
         clock_bombe.restart();
   
    }
    
        //lancer la bombe sur la carte
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::End)){}
        //ouvrir la page de paramètres
}


