#include "Brick.h"

namespace {
    constexpr float GRAVITY = 1200.f; 
}

Brick::Brick(float x, float y, float width, float height)
    : _isDestroyed(false)
{
    _shape.setPosition(x, y);
    _shape.setSize(sf::Vector2f(width, height));
    _shape.setFillColor(sf::Color::Red);
    _shape.setOrigin(width * 0.5f, height * 0.5f);
}

void Brick::update(float dt, float windowHeight)
{
    if (_isDestroyed) return;

    if (_isBreaking)
    {
        _vel.y += GRAVITY * dt;
        _shape.move(_vel * dt);
        _shape.rotate(_spin * dt);

        const float y = _shape.getPosition().y;
        if (y - _shape.getSize().y * 0.5f > windowHeight) 
        {
            _isDestroyed = true;
        }
    }
}

void Brick::render(sf::RenderWindow& window)
{
    if (!_isDestroyed) {
        window.draw(_shape);
    }
}

void Brick::hit()
{
    if (_isDestroyed || _isBreaking) return;

    _isBreaking = true;
    
    float kickY = -300.f;                                
    float driftX = static_cast<float>((rand() % 201) - 100); 

    _vel = { driftX, kickY };

    _spin = static_cast<float>((rand() % 241) - 120); 
}

sf::FloatRect Brick::getBounds() const
{
    return _shape.getGlobalBounds();
}
