//#include "src/Graphics/Graphics.h"
#include "main.hpp"
#include "game/Graphics/Graphics.h"
#include "game/Partie.hpp"


int main() {
    
    sf::RenderWindow * window  = new RenderWindow();
    ContextSettings settings;
    settings.antialiasingLevel = 16;
    window->create(sf::VideoMode(600, 600), "BOMBERMAN", sf::Style::Default, settings);
    window->setFramerateLimit(30);
    
    
    Partie partie(window); //!Faire appel à menu à la place de la partie
    partie.Run();
   
}





