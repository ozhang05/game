#ifndef ENTITYCOLLISIONSYSTEM_H 
#define ENTITYCOLLISIONSYSTEM_H 

#include <SFML/System/Vector2.hpp>

class EntityManager;
class Map;
class Entity;
class PositionComponent;
class CollisionComponent;
struct ApplicationState;

class EntityCollisionSystem {
    public:
        EntityCollisionSystem(EntityManager& em, const Map& mapLoc);

        void collisionCheck(Entity *e, sf::Vector2f& movement);
    private:
        EntityManager& entityManager;
        const Map& map;

        void mapCollisionCheck(const CollisionComponent& col, sf::Vector2f& movement);
        void entityCollisionCheck(int entityId, const CollisionComponent& col, sf::Vector2f& movement);
};

#endif // ENTITYCOLLISIONSYSTEM_H
