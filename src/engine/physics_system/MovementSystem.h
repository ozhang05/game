#ifndef MOVEMENTSYSTEM_H 
#define MOVEMENTSYSTEM_H 

#include "EntityCollisionSystem.h"
#include "PathfindingSystem.h"
#include "WindowContainer.h"

#include <SFML/System/Vector2.hpp>

class EntityManager;
class Map;
class Entity;
class PositionComponent;
struct ApplicationState;
struct WindowContainer;
class PathfindingComponent;

class MovementSystem {
public:
    MovementSystem(EntityManager& em, const Map& mapLoc, ApplicationState& appState);

    // updating with regard to time
    void update(float timeElapsed);
    sf::Vector2f updateMoverEntity(Entity *e);
    sf::Vector2f updatePatrolEntity(Entity *e, PositionComponent *pos);
    sf::Vector2f updateFollowEntity(Entity *e, PositionComponent *pos);

    // special checks
    void applyTileEffect(Entity *e, sf::Vector2i& pos, sf::Vector2f& movement);
    void updateEntitySpritePos(Entity *e, const PositionComponent& pos);
    void checkMapTransition(Entity *e);

    void renderDebugHighlights(WindowContainer& wc);
    void renderPath(WindowContainer& wc, sf::Vector2f offset, PathfindingComponent& pat);
private:
    EntityManager& entityManager;
    const Map& map;

    EntityCollisionSystem entityCollisionSystem;
    PathfindingSystem pathfindingSystem;

    ApplicationState& applicationState;
};

#endif // MOVEMENTSYSTEM_H
