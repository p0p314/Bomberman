#include "Bombe.hpp"
#include <iostream>
#include "../personnage/Personnage.hpp"

// Constructeur de la bombe
Bombe::Bombe(Personnage &proprietaire) : _owner(proprietaire)
{
    // Chargement du sprite de la bombe
    if (!loadBomb("assets/bombes.png"))
    {
        perror("chargement sprite bombe");
        return;
    }

    // Chargement du sprite des flammes
    if (!loadFire())
    {
        perror("chargement sprite flammes");
    }
}

// Destructeur de la bombe
Bombe::~Bombe()
{
    
}

// Chargement du sprite de la bombe
int Bombe::loadBomb(std::string lien)
{
    _texture.loadFromFile(lien);
    _sprite.setTexture(_texture);
    _sprite.setTextureRect(sf::IntRect(74, 53, 14, 16));
    _sprite.setScale(sf::Vector2f(3.0f, 3.0f));
    return 1;
}

// Obtention des rayons de flammes de la bombe
std::vector<sf::Sprite> Bombe::getRays()
{
    return _fireRays;
}

// Obtention des rayons de flammes de la bombe
std::vector<Ray*> Bombe::get_Rays()
{
    return _fire_Rays;
}

// Mise à jour de l'état de la bombe
void Bombe::Update(float dt)
{
    _elapsedTime += dt;

    if (_visibleBomb && _elapsedTime >= _timeBeforeExplosion)
    {
        setFireVisibility(true);
        _elapsedTimeFireAnim += dt;

        if (_posFireSpriteAnimation < 4 && _elapsedTimeFireAnim >= _timeToChangeFrame)
        {
            _posFireSpriteAnimation++;
            for (Ray *ray : _fire_Rays)
                ray->updateRect(_posFireSpriteAnimation);
               
            _elapsedTimeFireAnim = 0.f;
        }
    }

    if (_visibleFire && _elapsedTime >= _timeEndOfExplosion)
    {
        setVisibility(false);
        setFireVisibility(false);
        setExploded(true);
        _elapsedTime = 0;
        _posFireSpriteAnimation = 0;
    }
}

// Dessine la bombe et ses flammes
void Bombe::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    if (_visibleFire)
    {
        for (Ray *ray : _fire_Rays)
            if (ray->getVisibility())
                target.draw(ray->getSprite());
    }
    else
    {
        target.draw(_sprite);
    }
}

// Vérifie si les flammes de la bombe sont visibles
bool Bombe::isFireVisible()
{
    return _visibleFire;
}

// Chargement des sprites des flammes
int Bombe::loadFire()
{
    _fireTexture.loadFromFile("assets/fire_all.png");
   
    Ray::orientation orientations[] = { Ray::center, Ray::up, Ray::right, Ray::down, Ray::left };
    for (int i = 0; i < 5; i++)
    {
        Ray *ray = new Ray(_fireTexture, orientations[i]);
        _fire_Rays.push_back(ray);
    }
    return 1;
}

// Plante la bombe dans le monde du jeu
bool Bombe::plant(Monde *monde)
{   
    std::vector<std::vector<TileMap*>>& tiles = monde->getTiles();

    for (int i = 0; i < monde->getGridLength(); i++)
    {
        for (int j = 0; j < monde->getGridLength(); j++)
            if (tiles[i][j]->containsCharacter(_owner) && tiles[i][j]->isTraversable())
            {
                _sprite.setPosition(tiles[i][j]->getSprite().getPosition().x,
                                    tiles[i][j]->getSprite().getPosition().y);
                for (Ray *ray : _fire_Rays)
                    ray->updatePosition(_sprite);
                setVisibility(true);
                return true;
            }
    }
    return false;
}

// Vérifie si les flammes de la bombe contiennent un personnage
bool Bombe::fireContains(Personnage elm)
{
    sf::Vector2f position{elm.getPosition().x + elm.getTexture().getSize().x / 2,
                          elm.getPosition().y + elm.getTexture().getSize().y / 2};

    for (Ray *ray : _fire_Rays)
        if (ray->contains(position))
            return true;

    return false;
}

// Vérifie si la bombe a explosé
bool Bombe::isExploded()
{
    return _exploded;
}

// Définit si la bombe est visible
bool Bombe::isVisible()
{
    return _visibleBomb;
}

// Définit la visibilité de la bombe
void Bombe::setVisibility(bool state)
{
    _visibleBomb = state;
}

// Définit la visibilité des flammes de la bombe
void Bombe::setFireVisibility(bool state)
{
    _visibleFire = state;
}

// Définit si la bombe a explosé
void Bombe::setExploded(bool state)
{
    _exploded = state;
}
