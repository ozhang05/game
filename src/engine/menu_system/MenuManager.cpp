#include "MenuManager.h"

#include <iostream>

#include "ApplicationState.h"
#include "Interactable.h"
#include "TextureManager.h"
#include "EntityManager.h"
#include "ComponentFactory.h"
#include "ComponentSerializer.h"
#include "PauseMenu.h"
#include "EntityMenu.h"
#include "BattleMenu.h"

MenuManager::MenuManager(ApplicationState& as, EntityManager& em, ComponentFactory& cf,
    ComponentSerializer& cs, TextureManager& tm, const sf::Font& f)
    : applicationState(as),
        entityManager(em),
        componentFactory(cf),
        componentSerializer(cs),
        textureManager(tm),
        mainFont(f) {
    activeMenu = nullptr;

}

MenuManager::~MenuManager() {
    if (activeMenu != nullptr) {
        unloadMenu();
    }
    for (auto pair: menus) {
        delete pair.second;
        std::cout << "Deleted Menu " << pair.first << "\n";
    }
}

/* ************************************************************************** */

bool MenuManager::menuLoaded() {
    return activeMenu != nullptr;
}

bool MenuManager::menuLoaded(const std::string& name) {
    if (!menuExists(name)) {
        return false;
    }
    return menus.at(name) == activeMenu;
}

void MenuManager::loadMenu(const std::string& name) {
    if (!menuExists(name)) {
        createMenu(name);
    }

    activeMenu = menus.at(name);
    activeMenu->onEnter();
}

void MenuManager::unloadMenu() {
    activeMenu->onExit();
    activeMenu = nullptr;
}

void MenuManager::setActiveMenu(const std::string& name) {
    if (!menuExists(name)) {
        std::cerr << "ERROR MENU NOT FOUND MENUMANAGER.CPP\n";
    }

    (void) textureManager;
    activeMenu = menus.at(name);
    activeMenu->onEnter();
}

/* ************************************************************************** */

void MenuManager::render(sf::RenderWindow& window) {
    if (activeMenu == nullptr) {
        return;
    }
    activeMenu->render(window);
}

void MenuManager::updateInteractableLocations(const sf::RenderWindow& window) {
    sf::View currentView = window.getView();
    sf::Vector2f viewSize = currentView.getSize();
    sf::Vector2f viewCenter = currentView.getCenter();

    std::cout << "menu manager update interactable locaitons\n";
    activeMenu->updateMenuInteractableLocations(viewSize, viewCenter);
}
void MenuManager::updateFrameLocations(const sf::RenderWindow& window) {
    sf::View currentView = window.getView();
    sf::Vector2f viewSize = currentView.getSize();
    sf::Vector2f viewCenter = currentView.getCenter();

    std::cout << "menu manager update frame locaitons\n";
    activeMenu->updateMenuFrameLocations(viewSize, viewCenter);
}

void MenuManager::queueInteractableLocationUpdate() {
    activeMenu->queueInteractableUpdate();
}
void MenuManager::queueFrameLocationUpdate() {
    activeMenu->queueFrameUpdate();
}

/* ************************************************************************** */

MenuCommand MenuManager::queryKeyInput(const sf::Event::KeyPressed& e) {
    if (activeMenu == nullptr) {
        std::cerr << "ERROR QUERYING NULL MENUMANAGER.CPP\n";
        exit(EXIT_FAILURE);
    }
    return activeMenu->queryKeyInput(e);
}
bool MenuManager::textboxInput(const char32_t& c) {
    // if seen new line then exit out of textbox state

    if (c == 10) applicationState.state.control = GameState::Control::MENU;
    std::cout << "received: " << c << "\n";
    return activeMenu->queryTextInput(c);
}
void MenuManager::processMouseEvent(const MouseEvent& em) {
    if (activeMenu == nullptr) return;
    activeMenu->handleMouseEvent(em);
}

void MenuManager::executeMenuCommand(MenuCommand menuCommand) {
    if (menuCommand == MenuCommand::SELECT_OPTION) {
        if (activeMenu->getInteractable()->getType() == Interactable::Type::TEXTBOX) {
            // if you selected a textbox
            applicationState.state.control = GameState::Control::TEXTBOX;
        }
    }
    activeMenu->handleEvent(menuCommand);
}

/* ************************************************************************** */

void MenuManager::createMenu(const std::string& name) {
    if (menuExists(name)) {
        std::cerr << "ERROR RECREATING MENU MENUMANAGER.CPP\n";
    }

    //create menu here
    // Menu *p;
    if (name == "Pause") {
        PauseMenu *p = new PauseMenu(applicationState, mainFont);
        menus.emplace(name, p);
    } else if (name == "Entity") {
        EntityMenu *e = new EntityMenu(entityManager, componentFactory, componentSerializer, mainFont);
        menus.emplace(name, e);
    } else if (name == "Battle") {
        BattleMenu *b = new BattleMenu(mainFont);
        menus.emplace(name, b);
    }
    // menus.emplace(name, p);
    std::cout << "Creating menu " << name << "\n";
}

void MenuManager::addMenu(Menu *menu) {
    menus.emplace(menu->getName(), menu);
}

/* ************************************************************************** */

bool MenuManager::menuExists(const std::string& name) {
    return menus.find(name) != menus.end();
}
