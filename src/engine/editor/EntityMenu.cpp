#include "EntityMenu.h"

#include <iostream>

#include "Interactable.h"
#include "Button.h"
#include "Textbox.h"
#include "Frame.h"
#include "Rectangle.h"
#include "Entity.h"
#include "EntityManager.h"
#include "ComponentFactory.h"
#include "ComponentSerializer.h"
#include "Constants.h"

EntityMenu::EntityMenu(EntityManager& em, ComponentFactory& cf, ComponentSerializer& cs, const sf::Font& f)
    : Menu(f),
        entityManager(em),
        componentFactory(cf),
        componentSerializer(cs) {

    // background.setFillColor(sf::Color::Transparent);
    eCount = 0;

    ePerRow = 4;
    ePadding = 0.01;
    eButtonSize = sf::Vector2f(.05, .05);
    sf::Vector2f buttonSize(0.15, 0.05);

    Rectangle *rect = new Rectangle({0, 0}, {0.25, 1.f}, sf::Color::Black);
    frames.emplace_back(rect);

    rect = new Rectangle({0.75, 0}, {0.25, 1.f - MAPMAKER_TILESET_RATIO}, sf::Color::Black);
    frames.emplace_back(rect);

    Button *button = new Button({{0.125, 0.1}, buttonSize}, mainFont, "Create new entity");
    button->setOnClick(std::bind(&EntityMenu::createNewEntity, this));
    interactables.emplace_back(button);

    Textbox *textbox = new Textbox({{0.125, 0.2}, buttonSize}, mainFont, &createdComponent);
    interactables.emplace_back(textbox);

    button = new Button({{0.125, 0.9}, buttonSize}, mainFont, "Delete entity");
    button->setOnClick(std::bind(&EntityMenu::deleteEntity, this));
    interactables.emplace_back(button);

    // rect = new Rectangle({0, 0}, {0, 0}, sf::Color::Black);
    // frames.emplace_back(rect);
    //
    background.setFillColor(sf::Color(0, 0, 0, 150));


    inputMap.emplace(sf::Keyboard::Scancode::E, MenuCommand::CLOSE_MENU);
    inputMap.emplace(sf::Keyboard::Scancode::Escape, MenuCommand::CLOSE_MENU);
    inputMap.emplace(sf::Keyboard::Scancode::CapsLock, MenuCommand::CLOSE_MENU);

    inputMap.emplace(sf::Keyboard::Scancode::W, MenuCommand::NAVIGATE_UP);
    inputMap.emplace(sf::Keyboard::Scancode::A, MenuCommand::NAVIGATE_LEFT);
    inputMap.emplace(sf::Keyboard::Scancode::S, MenuCommand::NAVIGATE_DOWN);
    inputMap.emplace(sf::Keyboard::Scancode::D, MenuCommand::NAVIGATE_RIGHT);
    inputMap.emplace(sf::Keyboard::Scancode::Space, MenuCommand::SELECT_OPTION);
}

EntityMenu::~EntityMenu() {

}

/* ************************************************************************** */

void EntityMenu::render(sf::RenderWindow& window) {
    sf::View currentView = window.getView();
    sf::Vector2f viewSize = currentView.getSize();
    sf::Vector2f viewCenter = currentView.getCenter();
    // HACK: change this, and render system for pause menu
    // rendering should be largely done through the menu base class

    if (updateInteractableLocations) {
        std::cout << "updating interactables pausemenu.cpp\n";
        updateMenuInteractableLocations(viewCenter, viewSize);
        updateMenuFrameLocations(viewCenter, viewSize);
        updateInteractableLocations = false;
    }

    for (auto it : frames) {
        it->render(window);
    }
    for (auto it : interactables) {
        it->render(window);
    }

    renderEntities(window);
}

void EntityMenu::handleEvent(MenuCommand command) {
    switch (command) {
        case MenuCommand::NAVIGATE_DOWN:
        case MenuCommand::NAVIGATE_RIGHT:
                    // increment case
            interactables.at(selectedIndex)->onHoverExit();
            selectedIndex++;
            selectedIndex %= interactables.size();
            interactables.at(selectedIndex)->onHover();
            break;
        case MenuCommand::NAVIGATE_UP:
        case MenuCommand::NAVIGATE_LEFT:
            // decrement case
            interactables.at(selectedIndex)->onHoverExit();
            selectedIndex--;
            selectedIndex %= interactables.size();
            interactables.at(selectedIndex)->onHover();
            break;
        case MenuCommand::SELECT_OPTION:
            interactables.at(selectedIndex)->onClick();
            interactables.at(selectedIndex)->onHover();
            break;
        default:
            break;
    }
}

/* ************************************************************************** */

void EntityMenu::onEnter() {
    std::cout << "entity menu on enter\n";
    updateInteractableLocations = true;
    updateFrameLocations = true;
}
void EntityMenu::onExit() {
    std::cout << "entity menu on exit\n";
}
void EntityMenu::queryTextInput(const char32_t c) {
    std::cout << "hello " << c << "\n";
}

/* ************************************************************************** */

void EntityMenu::renderEntities(sf::RenderWindow& window) {
    (void) window;
    // for (auto it : entityManager.getEntities()) {
    //     if (it.second == entityManager.getMover()) continue;
    //     std::cout << "seen entity " << it.first << " " << it.second->getName() << "\n";
    // }
}

void EntityMenu::updateEntities() {
    // eCount = entityManager.getEntityCount() - 1;
    
    // delete any buttons for entities that no longer exist
    auto start = interactables.end() - eCount;
    for (auto it = start; it != interactables.end(); it++) {
        std::cerr << "interactable test " << (*it)->getName() << "\n";
        if (selectedEntity == std::stoi((*it)->getName())) {
            std::cout << "found button to delete\n";
            delete *it;
            it = interactables.erase(it);
            break;
        }
    }

    eCount = entityManager.getEntityCount() - 1;

    // reposition all entities
    int i = 0;
    for (auto it = start; it != interactables.end(); it++) {
        int x = i % ePerRow;
        int y = i / ePerRow;

        sf::Vector2f frameTopLeft(0.75, 0);
        sf::Vector2f buttonPos = frameTopLeft;
        buttonPos.x += (1 + x) * ePadding + x * eButtonSize.x + eButtonSize.x / 2;
        buttonPos.y += (1 + y) * ePadding + y * eButtonSize.y + eButtonSize.y / 2;
        
        (*it)->updateScreenSize(buttonPos, eButtonSize);
        i++;
    }
    updateInteractableLocations = true;
    updateFrameLocations = true;
}

void EntityMenu::addEntity(Entity *e) {
    // subtract one first to remove mover, second to zero index
    eCount = entityManager.getEntityCount() - 1;
    int x = (eCount - 1) % ePerRow;
    int y = (eCount - 1) / ePerRow;

    sf::Vector2f frameTopLeft(0.75, 0);
    sf::Vector2f buttonPos = frameTopLeft;
    buttonPos.x += (1 + x) * ePadding + x * eButtonSize.x + eButtonSize.x / 2;
    buttonPos.y += (1 + y) * ePadding + y * eButtonSize.y + eButtonSize.y / 2;

    Button *b = new Button({buttonPos, eButtonSize}, mainFont, std::to_string(e->getId()));
    b->setOnClick(std::bind(&EntityMenu::selectEntity, this));
    interactables.emplace_back(b);

    updateInteractableLocations = true;
    updateFrameLocations = true;
}

/* ************************************************************************** */

void EntityMenu::createNewEntity() {
    Entity *e = new Entity();
    entityManager.addEntity(e);
    if (rand() * 2 >= 1) {
        json pos;
        pos["x"] = rand();
        pos["y"] = rand();
        Component *c = componentFactory.createComponent("PositionComponent", pos);
        e->addComponent(c);
    }
    addEntity(e);
    std::cout << "test \"" << createdComponent << "\"\n";
}

void EntityMenu::createNewComponent(const std::string& name) {
    json reqs = componentSerializer.getComponentOutline(name);
    for (auto& [fieldName, fieldType] : reqs.items()) {
        std::cout << "seen req " << fieldName << " " << fieldType << "\n";
    }
}

void EntityMenu::deleteEntity() {
    if (selectedEntity == 0) return;

    entityManager.removeEntity(selectedEntity);
    updateEntities();
    selectedEntity = 0;
}

void EntityMenu::selectEntity() {
    std::cerr << "stoi here\n";
    std::cerr << "seelectedIndex " << selectedIndex << " " << interactables.at(selectedIndex)->getName() << "\n";
    selectedEntity = std::stoi(interactables.at(selectedIndex)->getName());
}
