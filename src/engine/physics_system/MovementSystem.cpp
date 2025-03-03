#include "MovementSystem.h"

#include "EntityManager.h"
#include "Map.h"
#include "Entity.h"
#include "Component.h"
#include "Tile.h"
#include "ApplicationState.h"
#include "TileEffect.h"
#include "WindowContainer.h"
#include "Constants.h"

#include <iostream>

MovementSystem::MovementSystem(EntityManager& em, const Map& mapLoc, ApplicationState& appState)
    : entityManager(em),
        map(mapLoc),
        entityCollisionSystem(em, mapLoc),
        pathfindingSystem(mapLoc),
        applicationState(appState) {
}

void MovementSystem::update(float timeElapsed) {
    for (auto pair : entityManager.getEntities()) {
        if (!pair.second->hasComponent("MovementComponent")) {
            continue;
        }
        Entity *e = pair.second;
        PositionComponent *pos = (PositionComponent *) e->getComponent("PositionComponent");
        MovementComponent *mov = (MovementComponent *) e->getComponent("MovementComponent");

        // movementInput is a vector consisting of directional movementInput, normalized during next step
        sf::Vector2f movementInput = {0, 0};

        if (e == entityManager.getMover()) {
            // player case
            movementInput = updateMoverEntity(e);

        } else if (e->hasComponent("PatrolMovementComponent")) {
            // patrol case
            movementInput = updatePatrolEntity(e, pos);

        } else if (e->hasComponent("FollowingComponent")) {
            // following case
            movementInput = updateFollowEntity(e, pos);

        }
        // normalize movementInput vector
        if (movementInput.x != 0 || movementInput.y != 0) {
            // if zero vector normalization does not work
            movementInput = movementInput.normalized();
        }
        mov->update(movementInput);

        // do movement
        sf::Vector2f movement(mov->getMovement());
        movement *= timeElapsed;
        if (movement.x == 0 && movement.y == 0) {
            continue;
        }

        entityCollisionSystem.collisionCheck(e, movement);
        if (movement.x == 0 && movement.y == 0) {
            continue;
        }

        sf::Vector2i position = sf::Vector2i(std::round(pos->getX()), std::round(pos->getY()));
        applyTileEffect(e, position, movement);

        // actual movement is done
        pos->move(movement);

        updateEntitySpritePos(e, *pos);

        if (e == entityManager.getMover()) {
            checkMapTransition(e);
        }
    }
}

sf::Vector2f MovementSystem::updateMoverEntity(Entity *e) {
    PlayerMovementComponent *pMov = (PlayerMovementComponent *) e->getComponent("PlayerMovementComponent");
    sf::Vector2f result;

    if (pMov->getA()) {
        result.x--;
    }
    if (pMov->getD()) {
        result.x++;
    }
    if (pMov->getW()) {
        result.y--;
    }
    if (pMov->getS()) {
        result.y++;
    }

    return result;
}

sf::Vector2f MovementSystem::updatePatrolEntity(Entity *e, PositionComponent *pos) {
    sf::Vector2f result;

    CollisionComponent *col = (CollisionComponent *) e->getComponent("CollisionComponent");
    PatrolMovementComponent *pat = (PatrolMovementComponent *) e->getComponent("PatrolMovementComponent");
    // runs in new case, if stuck, or if arrived
    if (pat->getDestination() == sf::Vector2f(-1, -1)
        || pat->checkIfStuck(pos->getPosition())
        || (pat->getDestination() - pos->getPosition()).length() <= 0.05f) {

        // std::cout << "reason " << int(pat->checkIfStuck(pos->getPosition()));
        // std::cout << "\n";
        // std::cout << "updating destination\n";

        if ((pat->getDestination() - pos->getPosition()).length() <= 0.05f) std::cout << "reached destination\n";

        sf::Vector2f temp = pat->getDestination() - pos->getPosition();
        temp += col->getSize() / 2.f;
        std::cout << "distance from dest print " << temp.x << " " << temp.y << "\n";

        pat->generateDestination(pos->getPosition(), map.getMapSize());
        pat->setTilePath(nullptr);
    }

    // TODO: generating path on repeat should happen above, with generate destination
    if (pat->getTilePath() == nullptr) {
        std::vector<sf::Vector2i> *tilePath = pathfindingSystem.findPath(pos->getPosition(), pat->getDestination());
        if (!tilePath) {
            // std::cout << " no path found \n";
            return result;
        } else {
            pat->setTilePath(tilePath);
        }
    }

    if (pat->getTilePath() == nullptr) return result;
    sf::Vector2f dest = sf::Vector2f(pat->consultTilePath(pos->getPosition()));
    result = dest - pos->getPosition();

    if (col != nullptr) result += col->getSize() / 2.f;

    return result;
}

sf::Vector2f MovementSystem::updateFollowEntity(Entity *e, PositionComponent *pos) {
    sf::Vector2f result;
    FollowingComponent *fol = (FollowingComponent *) e->getComponent("FollowingComponent");

    Entity *entityToFollow = entityManager.getEntity(fol->getFollowId());

    PositionComponent *posFollow = (PositionComponent *) entityToFollow->getComponent("PositionComponent");

    result = posFollow->getPosition() - pos->getPosition();

    if (result.length() < 0.25f) return sf::Vector2f();
    if (result.length() < 5.0f) return result * 0.5f;

    return result;
}

/* ************************************************************************** */

void MovementSystem::applyTileEffect(Entity *e, sf::Vector2i& pos, sf::Vector2f& movement) {
    Tile *tile = map.getTile(pos);
    if (tile == nullptr) return;

    switch(tile->effect) {
        case TileEffect::None:
            return;
        case TileEffect::Slippery:
            movement *= tileEffectMovementModifier(tile->effect);
            break;
        case TileEffect::SlowDown:
            movement *= tileEffectMovementModifier(tile->effect);
            break;
        case TileEffect::TallGrass:
            movement *= tileEffectMovementModifier(tile->effect);
            if (e == entityManager.getMover()) {
                if (pos == applicationState.lastPlayerLocation) return;
                if (rand() % 100 != 0) return;

                applicationState.startBattle = true;
                applicationState.state.scene = GameState::Scene::BATTLE;
                PlayerMovementComponent *pmov = (PlayerMovementComponent *) e->getComponent("PlayerMovementComponent");
                pmov->resetAll();

                applicationState.lastPlayerLocation = pos;
            }
            break;
        default:
            break;
    }

}

void MovementSystem::updateEntitySpritePos(Entity *e, const PositionComponent& pos) {
    RenderableComponent *ren = (RenderableComponent *) e->getComponent("RenderableComponent");
    CollisionComponent *col = (CollisionComponent *) e->getComponent("CollisionComponent");

    // std::cout << "attempt update\n";
    if (ren != nullptr) {
    // std::cout << "do ren update\n";
        ren->updatePos(pos);
    }
    if (col != nullptr) {
        col->updateCollisionBox(pos);
    }

    if (e == entityManager.getMover()) {
        applicationState.updateMainViewLocation = true;
        applicationState.mainViewLocation = {pos.getX() * TILE_SIZE, pos.getY() * TILE_SIZE};
    }
}

void MovementSystem::checkMapTransition(Entity *e) {
    CollisionComponent *col = (CollisionComponent *) e->getComponent("CollisionComponent");
    if (col == nullptr) {
        return;
    }

    sf::FloatRect colBox = col->getCollisionBox();

    Tile *transitionTile = map.isMapTransition(colBox);
    if (transitionTile != nullptr) {
        int newMap = transitionTile->transitionId;
        int entrance = transitionTile->entranceId;

        applicationState.newMapId = newMap;
        applicationState.newMapEntrance = entrance;
        applicationState.transitionMap = true;
    }
}

/* ************************************************************************** */

void MovementSystem::renderDebugHighlights(WindowContainer& wc) {
    for (auto it : entityManager.getEntities()) {

        MovementComponent *mov = (MovementComponent *) it.second->getComponent("MovementComponent");
        if (mov == nullptr) continue;

        PositionComponent *pos = (PositionComponent *) it.second->getComponent("PositionComponent");
        if (pos == nullptr) continue;

        CollisionComponent *col = (CollisionComponent *) it.second->getComponent("CollisionComponent");
        if (col == nullptr) continue;

        PathfindingComponent *pat = (PathfindingComponent *) it.second->getComponent("PatrolMovementComponent");
        if (pat != nullptr) renderPath(wc, col->getCollisionBox().position, *pat);
        
        sf::Vector2f movVec = (TILE_SIZE * 0.25f) * mov->getMovement();
        sf::Vector2f posVec = TILE_SIZE * pos->getPosition();
        sf::Vector2f colSizeVec = col->getSize() * TILE_SIZE;

        if (movVec.lengthSquared() == 0) continue;

        sf::Vector2f startX = posVec;
        if (movVec.x > 0) {
            startX.x += colSizeVec.x;
        }

        sf::Vector2f startY = posVec;
        if (movVec.y > 0) {
            startY.y += colSizeVec.y;
        }


        // horizontal movement triangle
        if (movVec.x != 0) {
            wc.triangleArray.resize(wc.triangleArrayIndex + 3);

            wc.triangleArray[wc.triangleArrayIndex].position = startX;
            wc.triangleArray[wc.triangleArrayIndex].color = sf::Color::White;
            wc.triangleArrayIndex++;

            wc.triangleArray[wc.triangleArrayIndex].position = startX + sf::Vector2f{0, colSizeVec.y};
            wc.triangleArray[wc.triangleArrayIndex].color = sf::Color::White;
            wc.triangleArrayIndex++;

            wc.triangleArray[wc.triangleArrayIndex].position = startX + movVec + sf::Vector2f{colSizeVec.x / 2, startY.y - startX.y + colSizeVec.y / 2};
            wc.triangleArray[wc.triangleArrayIndex].color = sf::Color::White;
            wc.triangleArrayIndex++;
        }

        // vertical movement triangle
        if (movVec.y != 0) {
            wc.triangleArray.resize(wc.triangleArrayIndex + 3);

            wc.triangleArray[wc.triangleArrayIndex].position = startY;
            wc.triangleArray[wc.triangleArrayIndex].color = sf::Color::White;
            wc.triangleArrayIndex++;

            wc.triangleArray[wc.triangleArrayIndex].position = startY + sf::Vector2f{colSizeVec.x, 0};
            wc.triangleArray[wc.triangleArrayIndex].color = sf::Color::White;
            wc.triangleArrayIndex++;

            wc.triangleArray[wc.triangleArrayIndex].position = startY + movVec + sf::Vector2f{startX.x - startY.x + colSizeVec.x / 2, colSizeVec.y / 2};
            wc.triangleArray[wc.triangleArrayIndex].color = sf::Color::White;
            wc.triangleArrayIndex++;
        }
    }
}

void MovementSystem::renderPath(WindowContainer& wc, sf::Vector2f offset, PathfindingComponent& pat) {
    std::vector<sf::Vector2i> *path = pat.getTilePath();
    if (path == nullptr) return;

    (void) offset;
    offset = {0, 0};
    //TODO: wefwef
    //how to get rendering in correct spot idk man???
    for (int i = 0; i < (int) (*path).size() - 1; i++) {
        wc.lineArray.append({ (sf::Vector2f((*path).at(i)) + offset + sf::Vector2f{0.5, 0.5}) * TILE_SIZE, sf::Color::White });
        wc.lineArray.append({ (sf::Vector2f((*path).at(i + 1)) + offset + sf::Vector2f{0.5, 0.5}) * TILE_SIZE, sf::Color::White });

        wc.lineArrayIndex += 2;

    }
}
