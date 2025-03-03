#include "EntityManager.h"

#include <iostream>
#include <unordered_map>
#include <vector>

#include "Entity.h"
#include "Component.h"
#include "ComponentFactory.h"
#include "Constants.h"
#include "WindowContainer.h"

EntityManager::EntityManager(ComponentFactory& componentFactory) 
    : componentFactory(componentFactory) {
    
    mover = nullptr;
    entityCount = 0;
}
EntityManager::~EntityManager() {
    for (const auto& e : entities) {
        if (e.first == 0) {
            continue;
        }
        std::cerr << "Deleted Entity " << e.first << "\n";
        Entity *ePtr = e.second;
        (void) ePtr;
        delete ePtr;
    }
}

//initialization functions
void EntityManager::init() {

}

/* ************************************************************************** */

void EntityManager::renderAll(sf::RenderWindow *window) {
    //sort by y value
    std::sort(renderableEntities.begin(), renderableEntities.end(), [](Entity* a, Entity* b) {
        RenderableComponent *sprA = (RenderableComponent *) (a->getComponent("RenderableComponent"));
        RenderableComponent *sprB = (RenderableComponent *) (b->getComponent("RenderableComponent"));

        sf::FloatRect boundsA = sprA->getSprite().getGlobalBounds();
        sf::FloatRect boundsB = sprB->getSprite().getGlobalBounds();

        float aYPos = boundsA.position.y + boundsA.size.y;
        float bYPos = boundsB.position.y + boundsB.size.y;

        return aYPos < bYPos;  // Sort by y position, lowest first
    });

    //render step
    for (Entity* ePtr : renderableEntities) {
        RenderableComponent *renderable = (RenderableComponent *) (ePtr->getComponent("RenderableComponent"));
        window->draw(renderable->getSprite());
    }
}

Entity *EntityManager::getMover() {
    return mover;
}
std::unordered_map<int, Entity *>& EntityManager::getEntities() {
    return entities;
}
int EntityManager::getEntityCount() {
    return entities.size();
}

/* ************************************************************************** */

void EntityManager::renderDebugHighlights(WindowContainer& wc) {
    for (const auto& pair : entities) {
        Entity *e = pair.second;
        if (!e) continue;
        if (!e->hasComponent("CollisionComponent")) {
            continue;
        }
        CollisionComponent *col = (CollisionComponent *) e->getComponent("CollisionComponent");
        if (!col->isCollidable()) {
            continue;
        }

        sf::FloatRect collisionBox = col->getCollisionBox();

        wc.lineArray.resize(wc.lineArrayIndex + 8);

        // left line
        // std::cout << "test " << collisionBox.position.x << ", " << collisionBox.position.y << ". size: " << collisionBox.size.x << ", " << collisionBox.size.y << "\n";
        wc.lineArray[wc.lineArrayIndex].position = collisionBox.position * TILE_SIZE;
        wc.lineArray[wc.lineArrayIndex].color = sf::Color::White;
        wc.lineArrayIndex++;

        wc.lineArray[wc.lineArrayIndex].position = sf::Vector2f{collisionBox.position.x, collisionBox.position.y + collisionBox.size.y} * TILE_SIZE;
        wc.lineArray[wc.lineArrayIndex].color = sf::Color::White;
        wc.lineArrayIndex++;

        // right line
        wc.lineArray[wc.lineArrayIndex].position = sf::Vector2f{collisionBox.position.x, collisionBox.position.y + collisionBox.size.y} * TILE_SIZE;
        wc.lineArray[wc.lineArrayIndex].color = sf::Color::White;
        wc.lineArrayIndex++;

        wc.lineArray[wc.lineArrayIndex].position = (collisionBox.position + collisionBox.size) * TILE_SIZE;
        wc.lineArray[wc.lineArrayIndex].color = sf::Color::White;
        wc.lineArrayIndex++;

        // right line
        wc.lineArray[wc.lineArrayIndex].position = (collisionBox.position + collisionBox.size) * TILE_SIZE;
        wc.lineArray[wc.lineArrayIndex].color = sf::Color::White;
        wc.lineArrayIndex++;

        wc.lineArray[wc.lineArrayIndex].position = sf::Vector2f{collisionBox.position.x + collisionBox.size.x, collisionBox.position.y} * TILE_SIZE;
        wc.lineArray[wc.lineArrayIndex].color = sf::Color::White;
        wc.lineArrayIndex++;

        // top line
        wc.lineArray[wc.lineArrayIndex].position = sf::Vector2f{collisionBox.position.x + collisionBox.size.x, collisionBox.position.y} * TILE_SIZE;
        wc.lineArray[wc.lineArrayIndex].color = sf::Color::White;
        wc.lineArrayIndex++;

        wc.lineArray[wc.lineArrayIndex].position = collisionBox.position * TILE_SIZE;
        wc.lineArray[wc.lineArrayIndex].color = sf::Color::White;
        wc.lineArrayIndex++;
    }
}

/* ************************************************************************** */

//entity functions
int EntityManager::addEntity(Entity *e) {
    if (e->hasComponent("PlayerMovementComponent")) {
        if (mover != nullptr) {
            std::cerr << "ERROR MULTIPLE PLAYER MOVERS ENTITYMANAGER.CPP\n";
        }
        mover = e;
    }
    // if (e->hasComponent("MovementComponent")) {
        // movableEntities.push_back(e);
        // std::cout << "adding moveableentitymanager\n";
    // }
    if (e->hasComponent("RenderableComponent")) {
        renderableEntities.push_back(e);
    }
    entityCount++;
    entities.emplace(e->getId(), e);
    createCollisionBoxes();
    return e->getId();
}
void EntityManager::removeEntity(int id) {
    Entity *e = entities.at(id);
    // if (e->hasComponent("MovementComponent")) {
    //     movableEntities.erase(
    //         std::remove(movableEntities.begin(), movableEntities.end(), e),
    //         movableEntities.end()
    //     );
    // }
    if (e->hasComponent("RenderableComponent")) {
        renderableEntities.erase(
            std::remove(renderableEntities.begin(), renderableEntities.end(), e),
            renderableEntities.end()
        );
    }
    entityCount--;
    entities.erase(id);
}
int EntityManager::loadAllEntities(const json& entityData) {
    int firstEntity = -1;
    for (json entity : entityData) {
        std::string name = entity["name"];
        Entity *newEntity = new Entity(name);

        if (entity.contains("components")) {
            json components = entity["components"];
            
            for (const auto& [componentName, componentData] : components.items()) {
                // Use the factory to create the component
                Component* component = componentFactory.createComponent(componentName, componentData);
                if (component) {
                    newEntity->addComponent(component);
                }
            }
        }

        if (firstEntity == -1) {
            firstEntity = addEntity(newEntity);
        } else {
            addEntity(newEntity);
        }
    }
    createCollisionBoxes();
    initializeRenderablePosition();

    return firstEntity;
}
void EntityManager::unloadMap(int firstEntity, int size) {
    if (firstEntity == -1) {
        return;
    }

    for (int i = firstEntity; i < firstEntity + size; i++) {
        removeEntity(i);
    }
}

Entity *EntityManager::getEntity(int id) {
    if (!entityExists(id)) {
        std::cerr << "ERROR, ACCESSING NONEXISTENT ENTITY ENTITYMANAGER.CPP\n";
        return nullptr;
    }
    return entities.at(id);
}

void EntityManager::createCollisionBoxes() {
    for (const auto& e : entities) {
        Entity *ePtr = e.second;
        CollisionComponent *col = (CollisionComponent *) ePtr->getComponent("CollisionComponent");
        if (col == nullptr) continue;
        PositionComponent *pos = (PositionComponent *) ePtr->getComponent("PositionComponent");
        if (pos == nullptr) continue;

        col->updateCollisionBox(*pos);
    }
}

void EntityManager::initializeRenderablePosition() {
    for (const auto& e : entities) {
        Entity *ePtr = e.second;
        if (!ePtr->hasComponent("RenderableComponent")) {
            continue;
        }
        RenderableComponent *ren = (RenderableComponent *) ePtr->getComponent("RenderableComponent");
        PositionComponent *pos = (PositionComponent *) ePtr->getComponent("PositionComponent");

        ren->updatePos(*pos);
    }
}

bool EntityManager::entityExists(int id) {
    return entities.find(id) != entities.end();
}
