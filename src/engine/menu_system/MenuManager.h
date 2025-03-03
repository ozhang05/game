#ifndef MENUMANAGER_H
#define MENUMANAGER_H

#include <unordered_map>
#include <string>

#include "Menu.h"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

struct ApplicationState;
struct MouseEvent;
class TextureManager;
class EntityManager;
class ComponentFactory;
class ComponentSerializer;

class MenuManager {
    public:
        MenuManager(ApplicationState& as, EntityManager& em, ComponentFactory& cf,
            ComponentSerializer& cs, TextureManager& tm, const sf::Font& f);
        ~MenuManager();

        // general utility
        bool menuLoaded();
        bool menuLoaded(const std::string& name);
        void loadMenu(const std::string& name);
        void unloadMenu();
        void setActiveMenu(const std::string& name);

        // visual update functions
        void render(sf::RenderWindow& window);
        void updateInteractableLocations(const sf::RenderWindow& window);
        void updateFrameLocations(const sf::RenderWindow& window);
        void queueInteractableLocationUpdate();
        void queueFrameLocationUpdate();

        // input functions
        MenuCommand queryKeyInput(const sf::Event::KeyPressed& e);
        bool textboxInput(const char32_t& c);
        void processMouseEvent(const MouseEvent& em);

        void executeMenuCommand(MenuCommand menuCommand);
        
        //menu creation and management
        void createMenu(const std::string& name);
        void addMenu(Menu *menu);
    private:
        bool menuExists(const std::string& name);

        // data to edit
        ApplicationState& applicationState;
        EntityManager& entityManager;
        ComponentFactory& componentFactory;
        ComponentSerializer& componentSerializer;

        // menu storage
        std::unordered_map<std::string, Menu *> menus;
        Menu *activeMenu;

        // resources to use
        TextureManager& textureManager;
        const sf::Font& mainFont;
};

#endif /* MENUMANAGER_H */
