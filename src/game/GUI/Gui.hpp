#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "../elements/personnage/Personnage.hpp"

class Gui : public sf::Drawable
{
public:
    Gui();  // Constructeur par défaut
    ~Gui(); // Destructeur

    void init(sf::Font *, int windowSizeX, int windowSizeY); // Méthode pour initialiser l'interface graphique
    void updateStats(std::vector<Personnage *> *characters); // Méthode pour mettre à jour les statistiques des personnages

    void updateStats(int startCounter); // Méthode pour mettre à jour le compteur de démarrage du jeu

private:
    bool _isStartingGame = true, // Indique si le jeu démarre
        _isOnPause = false,      // Indique si le jeu est en pause
        _isEndGame = false;      // Indique si le jeu est terminé

    sf::RenderWindow *_window; // Pointeur vers la fenêtre de rendu

    sf::Text *_characterOneLives,
        *_characterTwoLives,
        *_startCounter,
        *_textOfEndGame;
    int _victoryCounter = 0;         // Compteur de victoires
    int _windowWidth, _windowHeight; // Largeur et hauteur de la fenêtre
    sf::RectangleShape *_background; // Forme rectangulaire pour le fond

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};