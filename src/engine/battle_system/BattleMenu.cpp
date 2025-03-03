#include "BattleMenu.h"

#include <iostream>

#include "Constants.h"
#include "Button.h"
#include "Rectangle.h"

BattleMenu::BattleMenu(const sf::Font& font)
    : Menu(font) {

    background.setFillColor(sf::Color(0, 0, 0, 150));

    Rectangle *rect = new Rectangle({0, 0.70f}, {1.f, 0.35f}, sf::Color::Black);
    frames.emplace_back(rect);

    Button *b = new Button(sf::FloatRect({0.25, 0.8}, {0.425, 0.08}), font, "Fight");
    interactables.emplace_back(b);

    b = new Button(sf::FloatRect({0.25, 0.9}, {0.425, 0.08}), font, "Items");
    interactables.emplace_back(b);

    b = new Button(sf::FloatRect({0.75, 0.8}, {0.425, 0.08}), font, "Switch Pokemon");
    interactables.emplace_back(b);

    b = new Button(sf::FloatRect({0.75, 0.9}, {0.425, 0.08}), font, "Run Away!");
    interactables.emplace_back(b);

    // TODO: remove these once done creating battle things
    inputMap.emplace(sf::Keyboard::Scancode::B, MenuCommand::CLOSE_MENU);
    inputMap.emplace(sf::Keyboard::Scancode::Escape, MenuCommand::CLOSE_MENU);
    inputMap.emplace(sf::Keyboard::Scancode::CapsLock, MenuCommand::CLOSE_MENU);
    inputMap.emplace(sf::Keyboard::Scancode::W, MenuCommand::NAVIGATE_UP);
    inputMap.emplace(sf::Keyboard::Scancode::A, MenuCommand::NAVIGATE_LEFT);
    inputMap.emplace(sf::Keyboard::Scancode::S, MenuCommand::NAVIGATE_DOWN);
    inputMap.emplace(sf::Keyboard::Scancode::D, MenuCommand::NAVIGATE_RIGHT);
    inputMap.emplace(sf::Keyboard::Scancode::Space, MenuCommand::SELECT_OPTION);
}

BattleMenu::~BattleMenu() {
    std::cout << "PauseMenu destructor\n";
}

void BattleMenu::render(sf::RenderWindow& window) {
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
}
void BattleMenu::handleEvent(MenuCommand command) {
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
            break;
        default:
            break;
    }
}

void BattleMenu::onEnter() {
    // pause the game
    updateInteractableLocations = true;
    updateFrameLocations = true;
}
void BattleMenu::onExit() {
    // unpause game
}
