#pragma once
#include <SFML/Graphics.hpp>
#include <deque>


class GameManager;  // forward declaration



class Ball {
public:
    Ball(sf::RenderWindow* window, float velocity, GameManager* gameManager);
    ~Ball();
    void update(float dt);
    void render();
    void setVelocity(float coeff, float duration);
    void setFireBall(float duration);

private:
    sf::CircleShape _sprite;
    sf::Vector2f _direction;
    sf::RenderWindow* _window;
    float _velocity;
    bool _isAlive;
    bool _isFireBall;
    float _timeWithPowerupEffect;

    std::deque<sf::Vector2f> _trail;   
    sf::Vector2f _lastTrailPos;        
    static constexpr std::size_t TRAIL_MAX_POINTS = 32; 
    static constexpr float TRAIL_MIN_DIST2 = 9.0f;      

    void addTrailPoint(const sf::Vector2f& p);
    void drawTrail();                   

    GameManager* _gameManager;  // Reference to the GameManager


    static constexpr float RADIUS = 10.0f;      
    static constexpr float VELOCITY = 350.0f;   // for reference.
};

