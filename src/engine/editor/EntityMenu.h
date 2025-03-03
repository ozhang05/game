#ifndef ENTITYMENU_H
#define ENTITYMENU_H

#include "Menu.h"

class EntityManager;
class ComponentFactory;
class ComponentSerializer;
class Entity;

class EntityMenu : public Menu {
    public:
        EntityMenu(EntityManager& em, ComponentFactory& cf, ComponentSerializer& cs, const sf::Font& f);
        ~EntityMenu();

        void render(sf::RenderWindow& window) override;
        void handleEvent(MenuCommand command) override;

        void onEnter() override;
        void onExit() override;

        void queryTextInput(const char32_t c);

    private:

        EntityManager& entityManager;
        ComponentFactory& componentFactory;
        ComponentSerializer& componentSerializer;

        // entitymenu specific vars
        int selectedEntity = 0;
        std::string createdComponent;
        // control right side entity panel
        // to change, must change frame size as well
        int eCount;
        int ePerRow;
        float ePadding;
        sf::Vector2f eButtonSize;
        

        // temp functions
        void renderEntities(sf::RenderWindow& window);
        void updateEntities();
        void addEntity(Entity *e);

        // interactable functions
        void createNewEntity();     // create new entity
        void createNewComponent(const std::string& name);   // create new component
        void deleteEntity();        // delete entity button
        void selectEntity();        // selecting entity (right panel)
};

#endif /* ENTITYMENU_H */
