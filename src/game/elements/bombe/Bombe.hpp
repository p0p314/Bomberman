#ifndef BOMBERMAN_BOMBE_H
#define BOMBERMAN_BOMBE_H

#include "../element.hpp" // Inclut la classe Element
#include "Ray.h"          // Inclut la classe Ray

class Personnage; // Déclaration anticipée de la classe Personnage pour éviter les problèmes d'inclusion
class Monde;

// Classe représentant une bombe dans le jeu
class Bombe : public Element, public sf::Drawable
{
private:
    Personnage &_owner; // Référence vers le propriétaire de la bombe

    bool _visibleBomb = false, // Indique si la bombe est visible
        _visibleFire = false,  // Indique si les flammes de la bombe sont visibles
        _exploded = false;     // Indique si la bombe a explosé

    int _posFireSpriteAnimation = 0, // Position de l'animation des flammes de la bombe
        _posBombSpriteAnimation = 0, // Position de l'animation de la bombe
        _fireSpriteWidth = 50,       // Largeur du sprite des flammes
        _fireSpriteHeight = 50;      // Hauteur du sprite des flammes

    float _elapsedTime = 0.f,         // Temps écoulé depuis le début de la bombe
        _elapsedTimeFireAnim = 0.f,   // Temps écoulé depuis le début de l'animation des flammes
        _timeBeforeExplosion = 3.f,   // Temps avant l'explosion de la bombe
        _timeEndOfExplosion = 5.f,    // Temps de fin de l'explosion de la bombe
        _timeToChangeFrame = 0.0005f; // Temps pour changer de frame dans l'animation des flammes

    int degats = 1,       // Dégâts infligés par la bombe
        porteAttaque = 1; // Portée de l'attaque de la bombe

    sf::Texture _fireTexture;          // Texture des flammes de la bombe
    std::vector<sf::Sprite> _fireRays; // Vecteur de sprites représentant les flammes de la bombe
    std::vector<Ray *> _fire_Rays;     // Vecteur de pointeurs vers les rayons des flammes de la bombe

private:
    int loadBomb(std::string); // Charge le sprite de la bombe
    int loadFire();            // Charge le sprite des flammes

public:
    // Constructeur de la bombe
    Bombe(Personnage &proprietaire);

    // Constructeur par défaut supprimé pour empêcher la création d'objets Bombe sans paramètres
    Bombe() = delete;

    // Destructeur de la bombe
    ~Bombe();

    // Plante la bombe dans le monde du jeu
    bool plant(Monde *);

    // Définit la visibilité de la bombe
    void setVisibility(bool);

    // Définit la visibilité des flammes de la bombe
    void setFireVisibility(bool);

    // Définit si la bombe a explosé
    void setExploded(bool);

    // Vérifie si la bombe est visible
    bool isVisible();

    // Vérifie si les flammes de la bombe sont visibles
    bool isFireVisible();

    // Vérifie si la bombe a explosé
    bool isExploded();

    // Vérifie si les flammes de la bombe contiennent un personnage
    bool fireContains(Personnage);

    // Vérifie si l'expansion des flammes de la bombe est possible sur un sprite donné
    bool fireExpansion(sf::Sprite) const;

    // Obtient les sprites des flammes de la bombe
    std::vector<sf::Sprite> getRays();

    // Obtient les rayons des flammes de la bombe
    std::vector<Ray*> get_Rays();

    // Met à jour l'état de la bombe
    void Update(float dt);

    // Dessine la bombe et ses flammes
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

#endif //BOMBERMAN_BOMBE_H
