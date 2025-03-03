#include "EntityCollisionSystem.h"

#include <iostream>

#include "Component.h"
#include "Map.h"
#include "Entity.h"
#include "EntityManager.h"

EntityCollisionSystem::EntityCollisionSystem(EntityManager& em, const Map& mapLoc)
    : entityManager(em),
        map(mapLoc) {

}

/* ************************************************************************** */

void EntityCollisionSystem::collisionCheck(Entity *e, sf::Vector2f& movement) {
    CollisionComponent *col = (CollisionComponent *) e->getComponent("CollisionComponent");
    if (col == nullptr) return;
    if (!col->isCollidable()) return;

    if (movement.x != 0 || movement.y != 0) {
        mapCollisionCheck(*col, movement);
    }
    if (movement.x != 0 || movement.y != 0) {
        entityCollisionCheck(e->getId(), *col, movement);
    }
}

/* ************************************************************************** */

void EntityCollisionSystem::mapCollisionCheck(const CollisionComponent& col, sf::Vector2f& movement) {
    sf::FloatRect colBox = col.getCollisionBox();

    sf::FloatRect collisionX(colBox);
    sf::FloatRect collisionY(colBox);

    collisionX.position.x += movement.x;
    collisionY.position.y += movement.y;

    bool cancelX = map.collides(collisionX);
    bool cancelY = map.collides(collisionY);

    if (cancelX) {
        movement.x = 0;
    }
    if (cancelY) {
        movement.y = 0;
    }
}

void EntityCollisionSystem::entityCollisionCheck(int entityId, const CollisionComponent& col, sf::Vector2f& movement) {
    sf::FloatRect colBox = col.getCollisionBox();

    sf::FloatRect collisionX(colBox);
    sf::FloatRect collisionY(colBox);

    collisionX.position.x += movement.x;
    collisionY.position.y += movement.y;

    for (auto pair : entityManager.getEntities()) {
        if (!pair.second->hasComponent("CollisionComponent")) {
            continue;
        }
        if (pair.first == entityId) {
            continue;
        }

        CollisionComponent *otherCol = (CollisionComponent *) pair.second->getComponent("CollisionComponent");
        if (!otherCol->isCollidable()) continue;
        
        sf::FloatRect otherColBox = otherCol->getCollisionBox();

        std::optional<sf::FloatRect> resultX = collisionX.findIntersection(otherColBox);
        std::optional<sf::FloatRect> resultY = collisionY.findIntersection(otherColBox);

        if (resultX.has_value()) {
            movement.x = 0;
        }

        if (resultY.has_value()) {
            movement.y = 0;
        }
    }
}
