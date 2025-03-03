#include <string>
#include <iostream>
#include <stdlib.h>

#include "Component.h"
#include "Constants.h"

std::string Component::getName() const {
    return name;
}
void Component::setName(std::string newName) {
    name = newName;
}

/* ************************************************************************** */

PositionComponent::PositionComponent(float nx, float ny) {
    position.x = nx;
    position.y = ny;
    setName("PositionComponent");
}
float PositionComponent::getX() const {
    return position.x;
}
float PositionComponent::getY() const {
    return position.y;
}
void PositionComponent::setX(float n) {
    position.x = n;
}
void PositionComponent::setY(float n) {
    position.y = n;
}
void PositionComponent::incX(float n) {
    position.x += n;
}
void PositionComponent::incY(float n) {
    position.y += n;
}
const sf::Vector2f& PositionComponent::getPosition() const {
    return position;
}
sf::Vector2f PositionComponent::getPixelPosition() const {
    return position * TILE_SIZE;
}

void PositionComponent::move(sf::Vector2f movement) {
    position += movement;
}

/* ************************************************************************** */

MovementComponent::MovementComponent(float accel, float max, float nx, float ny) {
    velocity.x = nx;
    velocity.y = ny;

    acceleration = accel;
    maxSpeed = max;
    setName("MovementComponent");
}
void MovementComponent::updateDirection(float nx, float ny) {
    velocity.x = nx;
    velocity.y = ny;
}
void MovementComponent::update(sf::Vector2f normalizedDirection) {
    // direction is a normalized vector containing direction info
    if (normalizedDirection.length() - 1 > 0.001f && normalizedDirection.length() != 0) {
        // direction has not been normalized
        // std::cerr << "ERROR DIRECTION HAS NOT BEEN NORMALIZED COMPONENT.CPP\n";
        // normalizedDirection = normalizedDirection.normalized();
    }

    velocity.x += normalizedDirection.x * acceleration;
    velocity.y += normalizedDirection.y * acceleration;

    // deceleration step
    if (normalizedDirection.x == 0) {
        if (velocity.x > 0) {
            velocity.x -= acceleration;
            if (velocity.x < 0) velocity.x = 0;
        } else if (velocity.x < 0) {
            velocity.x += acceleration;
            if (velocity.x > 0) velocity.x = 0;
        }
    }

    if (normalizedDirection.y == 0) {
        if (velocity.y > 0) {
            velocity.y -= acceleration;
            if (velocity.y < 0) velocity.y = 0;
        } else if (velocity.y < 0) {
            velocity.y += acceleration;
            if (velocity.y > 0) velocity.y = 0;
        }
    }

    // limiting velocity step
    float speedSquared = velocity.lengthSquared();
    if (speedSquared > (maxSpeed * maxSpeed)) {
        float scale = maxSpeed / std::sqrt(speedSquared);

        velocity.x *= scale;
        velocity.y *= scale;
    }
}
const sf::Vector2f& MovementComponent::getMovement() {
    return velocity;
}

/* ************************************************************************** */

PlayerMovementComponent::PlayerMovementComponent (bool nw, bool na, bool ns, bool nd) {
    w = nw;
    a = na;
    s = ns;
    d = nd;
    setName("PlayerMovementComponent");
}
bool PlayerMovementComponent::getW() {
    return w;
}
bool PlayerMovementComponent::getA() {
    return a;
}
bool PlayerMovementComponent::getS() {
    return s;
}
bool PlayerMovementComponent::getD() {
    return d;
}
void PlayerMovementComponent::setW(bool n) {
    w = n;
}
void PlayerMovementComponent::setA(bool n) {
    a = n;
}
void PlayerMovementComponent::setS(bool n) {
    s = n;
}
void PlayerMovementComponent::setD(bool n) {
    d = n;
}
void PlayerMovementComponent::resetAll() {
    w = false;
    a = false;
    s = false;
    d = false;
}

/* ************************************************************************** */

PathfindingComponent::PathfindingComponent(std::vector<sf::Vector2i> *tp) {
    tilePath = tp;
    tilePathIndex = 0;
}

PathfindingComponent::~PathfindingComponent() {
    if (tilePath != nullptr) delete tilePath;
}

const sf::Vector2i& PathfindingComponent::consultTilePath(const sf::Vector2f& p) {
    // TODO: change to account for different rounding method (to nearest tile)
    if (tilePath == nullptr) {
        std::cerr << "ERROR CONSULTING NULL TILEPATH COMPONENT.CPP\n";
        exit(EXIT_FAILURE);
    }
    sf::Vector2i pos;
    pos.x = std::round(p.x);
    pos.y = std::round(p.y);

    if (tilePathIndex < (int) (*tilePath).size() - 1
        && ((*tilePath)[tilePathIndex] - pos).lengthSquared() == 0.f) {
        // std::cout << "incrementing tilepath\n";
        // std::cout << "evidence: " << pos.x << " " << pos.y << ". true " << (*tilePath)[tilePathIndex].x << " " << (*tilePath)[tilePathIndex].y << ". prev: " << tilePathIndex << "\n";
        tilePathIndex++;
        std::cout << "tilePathIndex " << tilePathIndex << "\n";
    }

    // if (tilePathIndex < (int) (*tilePath).size() - 1
    //     && (pos.x == (*tilePath)[tilePathIndex].x && pos.y == (*tilePath)[tilePathIndex].y)) {
    //     // std::cout << "incrementing tilepath\n";
    //     // std::cout << "evidence: " << pos.x << " " << pos.y << ". true " << (*tilePath)[tilePathIndex].x << " " << (*tilePath)[tilePathIndex].y << ". prev: " << tilePathIndex << "\n";
    //     tilePathIndex++;
    //     std::cout << "tilePathIndex " << tilePathIndex << "\n";
    // }
    return (*tilePath)[tilePathIndex];
}

std::vector<sf::Vector2i> *PathfindingComponent::getTilePath() const {
    return tilePath;
}

void PathfindingComponent::setTilePath(std::vector<sf::Vector2i> *tp) {
    if (tilePath) delete tilePath;
    if (!tp) std::cerr << "ERROR SETTING NULL TILEPATH\n";
    tilePathIndex = 0;
    tilePath = tp;
}

/* ************************************************************************** */
FollowingComponent::FollowingComponent(int id) 
    : followId(id) {
    
    setName("FollowingComponent");
}

FollowingComponent::~FollowingComponent() {

}

int FollowingComponent::getFollowId() {
    return followId;
}

bool FollowingComponent::checkIfStuck(const sf::Vector2f& pos) {
    if ((previousPos - pos).length() <= 0.0001) {
        stuckTimer += 0.001;
    } else {
        stuckTimer = 0;
    }
    previousPos = pos;
    return stuckTimer >= 1;
}
/* ************************************************************************** */

PatrolMovementComponent::PatrolMovementComponent(sf::Vector2f initialPos, float radius)
    : radius(radius),
        initialPosition(initialPos) {
    
    (void) initialPos;
    currentDestination = {-1, -1};
    setName("PatrolMovementComponent");
}

void PatrolMovementComponent::generateDestination(const sf::Vector2f& pos, const sf::Vector2i& mapSize) {
    // random number [-0.5, 0.5]
    float random = ((float) rand() / RAND_MAX) - 0.5f;
    float randX = initialPosition.x + (random * radius);
    random = ((float) rand() / RAND_MAX) - 0.5f;
    float randY = initialPosition.x + (random * radius);

    currentDestination.x = randX += pos.x;
    currentDestination.y = randY += pos.y;

    if (currentDestination.x < 0 || currentDestination.y < 0) generateDestination(pos, mapSize);
    if (currentDestination.x > mapSize.x || currentDestination.y > mapSize.y) generateDestination(pos, mapSize);

    stuckTimer = 0;
}
const sf::Vector2f& PatrolMovementComponent::getDestination() const {
    return currentDestination;
}
bool PatrolMovementComponent::checkIfStuck(const sf::Vector2f& pos) {
    // TODO: ????? FUCKED IT UP
    if ((previousPos - pos).length() <= 0.001) {
        stuckTimer += 0.001;
    } else {
        stuckTimer = 0;
    }
    previousPos = pos;
    return stuckTimer >= 1;
}
/* ************************************************************************** */

AIMovementComponent::AIMovementComponent(float cl)
    : cycleLength(cl) {

}

void AIMovementComponent::update(float deltaTime) {
    currentTime += deltaTime;
    if (currentTime >= cycleLength) {
        currentTime -= cycleLength;
    }
}

float AIMovementComponent::getCycleLength() {
    return cycleLength;
}
/* ************************************************************************** */

// centerX, centerY are center of collision box in relative coordinates
// width and height are in pixels
// collision box is based in map coordinates, multiply by TILE_SIZE to get true coords
// CollisionComponent::CollisionComponent(float wp, float hp, float pX, float pY, bool collidable) {
//     size.x = wp;
//     size.y = hp;
//
//     collide = collidable;
//
//     // if ((center.x - (size.x / 2.f)) / TILE_SIZE > 0) {
//     //     collisionBox.position.x = pX + ((center.x - (size.x / 2)) / TILE_SIZE);
//     // }
//     // if ((center.y - (size.y / 2.f)) / TILE_SIZE > 0) {
//     //     collisionBox.position.y = pY + ((center.y - (size.y / 2)) / TILE_SIZE);
//     // }
//
//
//     collisionBox.position.x = pX + ((center.x - (size.x / 2)) / TILE_SIZE);
//     collisionBox.position.y = pY + ((center.y - (size.y / 2)) / TILE_SIZE);
//     (void) pX;
//     (void) pY;
//
//     collisionBox.size.x = size.x / TILE_SIZE;
//     collisionBox.size.y = size.y / TILE_SIZE;
//
//     setName("CollisionComponent");
// }
CollisionComponent::CollisionComponent(int width, int height, bool collidable)
    : size(width / TILE_SIZE, height / TILE_SIZE),
        collisionBox({0, 0}, size) {

    collide = collidable;
    setName("CollisionComponent");
}
sf::FloatRect CollisionComponent::getCollisionBox() const {
    return collisionBox;
}
void CollisionComponent::updateCollisionBox(const PositionComponent& pos) const {
    collisionBox.position = pos.getPosition();
}
bool CollisionComponent::isCollidable() const {
    return collide;
}

const sf::Vector2f& CollisionComponent::getCenter() {
    return center;
}
const sf::Vector2f& CollisionComponent::getSize() {
    return size;
}

/* ************************************************************************** */

RenderableComponent::RenderableComponent(const sf::Texture& texture, const std::string& name, sf::Vector2f pos, sf::Vector2f off)
    : imageName(name),
        sprite(texture),
        offsets(off) {
    this->texture = texture;
    sprite.setTexture(texture);
    sprite.setPosition((pos * TILE_SIZE - offsets / 2.f));
    setName("RenderableComponent");
}
sf::Sprite RenderableComponent::getSprite() {
    return sprite;
}
void RenderableComponent::updatePos(const PositionComponent& pos) {
    sprite.setPosition((pos.getPosition() * TILE_SIZE - (offsets / 2.f)));
}
const std::string& RenderableComponent::getImageName() {
    return imageName;
}
