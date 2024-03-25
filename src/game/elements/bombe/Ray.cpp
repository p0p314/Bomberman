#include "Ray.h"

Ray::Ray(sf::Texture texture, orientation orientation)
{
    _texture = texture;
    _sprite.setTexture(_texture);
    _orientation = orientation;
    
    _sprite.setTextureRect(sf::IntRect(0 * _width,
                                       _orientation*_height,
                                       _width,
                                       _height));

    _width = _sprite.getGlobalBounds().width;
    _height = _sprite.getGlobalBounds().height;
}

void Ray::updateRect(int posOnTexture)
{
    _sprite.setTextureRect(sf::IntRect(posOnTexture * _width,
                                        _orientation*_height,
                                        _width,
                                        _height));
}

void Ray::updatePosition(sf::Sprite sprite)
{
    switch (_orientation)
    {
        case 0:
            _sprite.setPosition((int)sprite.getPosition().x, (int)sprite.getPosition().y);
            break;
        case 1:
            _sprite.setPosition((int)sprite.getPosition().x, (int)sprite.getPosition().y-_height);
            break;

        case 2:
            _sprite.setPosition((int)sprite.getPosition().x+_width, (int)sprite.getPosition().y);
            break;

        case 3:
            _sprite.setPosition((int)sprite.getPosition().x, (int)sprite.getPosition().y+_height);

            break;

        case 4:
            _sprite.setPosition((int)sprite.getPosition().x-_width, (int)sprite.getPosition().y);
            break;
    
        default:
            break;
    }
}

bool Ray::contains(sf::Vector2f position)
{
    return _sprite.getGlobalBounds().contains(position);
}

bool Ray::getVisibility()
{
    return _visible;
}

void Ray::setVisibility(bool etat)
{
    _visible = etat;
}

sf::FloatRect Ray::getRect()
{
    return _sprite.getGlobalBounds();
}



sf::Sprite Ray::getSprite() const
{
    return _sprite;
}