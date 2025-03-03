#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <unordered_map>
#include <vector>

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Entity;
class ComponentFactory;
struct WindowContainer;

class EntityManager {
    public:
        EntityManager(ComponentFactory& componentFactory);
        ~EntityManager();
        
        //initialization functions
        void init();

        //game functions
        void renderAll(sf::RenderWindow *window);
        Entity *getMover();
        std::unordered_map<int, Entity *>& getEntities();
        int getEntityCount();
        // std::vector<Entity *>& getMovableEntities();

        //debug functions
        void renderDebugHighlights(WindowContainer& wc);

        //entity functions
        int addEntity(Entity *e);
        void removeEntity(int id);
        int loadAllEntities(const json& entityData);
        void unloadMap(int firstEntity, int size);
        Entity *getEntity(int id);
        bool entityExists(int id);
    private:
        std::unordered_map<int, Entity *> entities;
        std::vector<Entity *> renderableEntities;
        // std::vector<Entity *> movableEntities;
        ComponentFactory& componentFactory;
        Entity *mover;              // holds entity used to move
        int entityCount;

        void createCollisionBoxes();
        void initializeRenderablePosition();
};

#endif /* ENTITYMANAGER_H */
