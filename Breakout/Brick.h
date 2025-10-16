#pragma once
#include <SFML/Graphics.hpp>

class Brick {
public:
    Brick(float x, float y, float width, float height);

    void update(float dt, float windowHeight);

    void render(sf::RenderWindow& window);

    void hit();

    sf::FloatRect getBounds() const;

    bool isBreaking() const { return _isBreaking; }

private:
    sf::RectangleShape _shape;
    bool _isDestroyed;
    bool _isBreaking = false;
    sf::Vector2f _vel{ 0.f, 0.f };
    float _spin = 0.f;
};