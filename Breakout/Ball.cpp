#include "Ball.h"
#include "GameManager.h" // avoid cicular dependencies

Ball::Ball(sf::RenderWindow* window, float velocity, GameManager* gameManager)
    : _window(window), _velocity(velocity), _gameManager(gameManager),
    _timeWithPowerupEffect(0.f), _isFireBall(false), _isAlive(true), _direction({1,1})
{
    _sprite.setRadius(RADIUS);
    _sprite.setFillColor(sf::Color::Cyan);
    _sprite.setPosition(0, 300);

    _lastTrailPos = _sprite.getPosition() + sf::Vector2f(RADIUS, RADIUS);
    _trail.clear();
    _trail.push_back(_lastTrailPos);
}

Ball::~Ball()
{
}

void Ball::update(float dt)
{
    // check for powerup, tick down or correct
    if (_timeWithPowerupEffect > 0.f)
    {
        _timeWithPowerupEffect -= dt;
    }
    else
    {
        if (_velocity != VELOCITY)
            _velocity = VELOCITY;   // reset speed.
        else
        {
            setFireBall(0);    // disable fireball
            _sprite.setFillColor(sf::Color::Cyan);  // back to normal colour.
        }        
    }

    // Fireball effect
    if (_isFireBall)
    {
        // Flickering effect
        int flicker = rand() % 50 + 205; // Random value between 205 and 255
        _sprite.setFillColor(sf::Color(flicker, flicker / 2, 0)); // Orange flickering color
    }

    // Update position with a subtle floating-point error
    _sprite.move(_direction * _velocity * dt);

    addTrailPoint(_sprite.getPosition() + sf::Vector2f(RADIUS, RADIUS));

    // check bounds and bounce
    sf::Vector2f position = _sprite.getPosition();
    sf::Vector2u windowDimensions = _window->getSize();

    // bounce on walls
    if ((position.x >= windowDimensions.x - 2 * RADIUS && _direction.x > 0) || (position.x <= 0 && _direction.x < 0))
    {
        _direction.x *= -1;
    }

    // bounce on ceiling
    if (position.y <= 0 && _direction.y < 0)
    {
        _direction.y *= -1;
    }

    // lose life bounce
    if (position.y > windowDimensions.y)
    {
        _sprite.setPosition(0, 300);
        _direction = { 1, 1 };
        _gameManager->loseLife();

        _trail.clear();
        _lastTrailPos = _sprite.getPosition() + sf::Vector2f(RADIUS, RADIUS);
        _trail.push_back(_lastTrailPos);
    }

    // collision with paddle
    if (_sprite.getGlobalBounds().intersects(_gameManager->getPaddle()->getBounds()))
    {
        _direction.y *= -1; // Bounce vertically

        float paddlePositionProportion = (_sprite.getPosition().x - _gameManager->getPaddle()->getBounds().left) / _gameManager->getPaddle()->getBounds().width;
        _direction.x = paddlePositionProportion * 2.0f - 1.0f;

        // Adjust position to avoid getting stuck inside the paddle
        _sprite.setPosition(_sprite.getPosition().x, _gameManager->getPaddle()->getBounds().top - 2 * RADIUS);
    }

    // collision with bricks
    int collisionResponse = _gameManager->getBrickManager()->checkCollision(_sprite, _direction);
    if (_isFireBall) return; // no collisisons when in fireBall mode.
    if (collisionResponse == 1)
    {
        _direction.x *= -1; // Bounce horizontally
    }
    else if (collisionResponse == 2)
    {
        _direction.y *= -1; // Bounce vertically
    }
}

void Ball::addTrailPoint(const sf::Vector2f& p)
{
    sf::Vector2f d = p - _lastTrailPos;
    float dist2 = d.x * d.x + d.y * d.y;
    if (dist2 < TRAIL_MIN_DIST2) return;

    _trail.push_back(p);
    _lastTrailPos = p;

    // Trim oldest points
    if (_trail.size() > TRAIL_MAX_POINTS)
        _trail.pop_front();
}

void Ball::drawTrail()
{
    if (_trail.size() < 2) return;

    sf::VertexArray strip(sf::LineStrip, _trail.size());

    const float maxAlpha = 200.f;
    const std::size_t n = _trail.size();

    // Pick base colour depending on fireball state
    sf::Color baseColor;
    if (_isFireBall)
    {
        baseColor = sf::Color(255, 140, 0); 
    }
    else
    {
        baseColor = sf::Color::Cyan; 
    }

    for (std::size_t i = 0; i < n; ++i)
    {
        float t = static_cast<float>(i) / static_cast<float>(n - 1);
        sf::Uint8 a = static_cast<sf::Uint8>(t * maxAlpha);

        strip[i].position = _trail[i]; 
        strip[i].color = sf::Color(baseColor.r, baseColor.g, baseColor.b, a);
    }

    const int thickness = 2;
    _window->draw(strip);

    for (int offset = -thickness; offset <= thickness; ++offset)
    {
        if (offset == 0) continue;
        sf::VertexArray thickLine(sf::LineStrip, _trail.size());
        for (std::size_t i = 0; i < n; ++i)
        {
            thickLine[i].position = strip[i].position + sf::Vector2f(offset, 0.f);
            thickLine[i].color = strip[i].color;
        }
        _window->draw(thickLine);
    }
}

void Ball::render()
{
    drawTrail();
    _window->draw(_sprite);
}

void Ball::setVelocity(float coeff, float duration)
{
    _velocity = coeff * VELOCITY;
    _timeWithPowerupEffect = duration;
}

void Ball::setFireBall(float duration)
{
    if (duration) 
    {
        _isFireBall = true;
        _timeWithPowerupEffect = duration;        
        return;
    }
    _isFireBall = false;
    _timeWithPowerupEffect = 0.f;    
}
