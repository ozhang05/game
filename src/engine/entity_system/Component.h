#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>

inline std::string ComponentNames[9] = {
    "PositionComponent",
    "MovementComponent",
    "PlayerMovementComponent",
    "PathfindingComponent",
    "FollowingComponent",
    "PatrolMovementComponent",
    "AIMovementComponent",
    "CollisionComponent",
    "RenderableComponent"
};

class Component {
public:
    virtual ~Component() = default;
    std::string getName() const;
protected:
    void setName(std::string newName);
private:
    std::string name;
};

class PositionComponent : public Component {
public:
    PositionComponent(float nx = 0.f, float ny = 0.f);

    float getX() const;
    float getY() const;
    void setX(float n);
    void setY(float n);
    void incX(float n);
    void incY(float n);
    const sf::Vector2f& getPosition() const;
    sf::Vector2f getPixelPosition() const;

    void move(sf::Vector2f movement);
private:
    sf::Vector2f position;
};

class MovementComponent : public Component {
public:
    MovementComponent(float accel = 0.f, float max = 1.f, float nx = 0.f, float ny = 0.f);

    void updateDirection(float nx, float ny);
    void update(sf::Vector2f normalizedDirection);

    const sf::Vector2f& getMovement();
private:
    sf::Vector2f velocity;
    float acceleration, maxSpeed;
};

class PlayerMovementComponent : public Component {
public:
    PlayerMovementComponent (bool nw = false, bool na = false, bool ns = false, bool nd = false);

    bool getW();
    bool getA();
    bool getS();
    bool getD();
    void setW(bool n);
    void setA(bool n);
    void setS(bool n);
    void setD(bool n);
    void resetAll();
private:
    bool w, a, s, d;
};

class PathfindingComponent : public Component {

public:
    PathfindingComponent(std::vector<sf::Vector2i> *tp = nullptr);
    ~PathfindingComponent();

    const sf::Vector2i& consultTilePath(const sf::Vector2f& p);
    std::vector<sf::Vector2i> *getTilePath() const;
    void setTilePath(std::vector<sf::Vector2i> *tp);
private:
    int tilePathIndex;
    std::vector<sf::Vector2i> *tilePath;
};

class FollowingComponent : public Component {
public:
    FollowingComponent(int id = 0);
    ~FollowingComponent();

    int getFollowId();
    bool checkIfStuck(const sf::Vector2f& pos);

private:
    int followId;
    float stuckTimer;
    sf::Vector2f previousPos;
};

class PatrolMovementComponent : public PathfindingComponent {
// class PatrolMovementComponent : public Component {
public:
    PatrolMovementComponent(sf::Vector2f initialPos, float radius = 1.f);

    // void generateDestination();
    void generateDestination(const sf::Vector2f& pos, const sf::Vector2i& mapSize);
    const sf::Vector2f& getDestination() const;
    bool checkIfStuck(const sf::Vector2f& pos);

private:
    float radius;
    float stuckTimer;

    sf::Vector2f initialPosition;
    sf::Vector2f previousPos;
    sf::Vector2f currentDestination;

};

class AIMovementComponent : public PathfindingComponent {
// class AIMovementComponent : public Component {
public:
    AIMovementComponent(float cl = 1.f);

    void update(float deltaTime);
    float getCycleLength();
private:
    float cycleLength, currentTime;
};

class CollisionComponent : public Component {
public:
    CollisionComponent(int width, int height, bool collidable = true);
    // CollisionComponent(
    //                    float pX = 0.f, float pY = 0.f,
    //                    bool collidable = true);
    sf::FloatRect getCollisionBox() const;
    void updateCollisionBox(const PositionComponent& pos) const;
    bool isCollidable() const;

    const sf::Vector2f& getCenter();
    const sf::Vector2f& getSize();
private:
    sf::Vector2f center, size;
    mutable sf::FloatRect collisionBox;
    bool collide;
};

class RenderableComponent : public Component {
public:
    RenderableComponent(const sf::Texture& texture, const std::string& name,
                        sf::Vector2f pos = {0, 0}, sf::Vector2f off = {0, 0});;

    sf::Sprite getSprite();
    void updatePos(const PositionComponent& pos);

    const std::string& getImageName();
    // void setPos(float x, float y);
    // void move(float x, float y);
private:
    //int animationTime;
    //std::vector<sf::Texture> textures;
    std::string imageName;

    sf::Texture texture;
    sf::Sprite sprite;
    sf::Vector2f offsets;
};

#endif /* COMPONENT_H */
