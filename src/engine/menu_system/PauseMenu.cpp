#include "PauseMenu.h"

#include <iostream>

#include "ApplicationState.h"
#include "Constants.h"
#include "Interactable.h"
#include "Button.h"

PauseMenu::PauseMenu(ApplicationState& appState, const sf::Font& font)
    : Menu(font),
        applicationState(appState) {

    background.setFillColor(sf::Color(0, 0, 0, 150));

    Button *b = new Button(sf::FloatRect({.5, .1}, {.7, .08}), font, "Resume");
    b->setOnClick(std::bind(&PauseMenu::resumeGame, this));
    interactables.emplace_back(b);

    b = new Button(sf::FloatRect({.5, .2}, {.7, .08}), font, "Zoom In");
    b->setOnClick(std::bind(&PauseMenu::zoomIn, this));
    interactables.emplace_back(b);

    b = new Button(sf::FloatRect({.5, .3}, {.7, .08}), font, "Zoom Out");
    b->setOnClick(std::bind(&PauseMenu::zoomOut, this));
    interactables.emplace_back(b);

    b = new Button(sf::FloatRect({.5, .4}, {.7, .08}), font, "Debug Mode");
    b->setOnClick(std::bind(&PauseMenu::debugToggle, this));
    interactables.emplace_back(b);

    b = new Button(sf::FloatRect({.5, .5}, {.7, .08}), font, "Trigger Battle");
    b->setOnClick(std::bind(&PauseMenu::toggleBattle, this));
    interactables.emplace_back(b);

    b = new Button(sf::FloatRect({.5, .6}, {.7, .08}), font, "Quit");
    b->setOnClick(std::bind(&PauseMenu::quitGame, this));
    interactables.emplace_back(b);


    inputMap.emplace(sf::Keyboard::Scancode::P, MenuCommand::CLOSE_MENU);
    inputMap.emplace(sf::Keyboard::Scancode::Escape, MenuCommand::CLOSE_MENU);
    inputMap.emplace(sf::Keyboard::Scancode::CapsLock, MenuCommand::CLOSE_MENU);
    inputMap.emplace(sf::Keyboard::Scancode::W, MenuCommand::NAVIGATE_UP);
    inputMap.emplace(sf::Keyboard::Scancode::A, MenuCommand::NAVIGATE_LEFT);
    inputMap.emplace(sf::Keyboard::Scancode::S, MenuCommand::NAVIGATE_DOWN);
    inputMap.emplace(sf::Keyboard::Scancode::D, MenuCommand::NAVIGATE_RIGHT);
    inputMap.emplace(sf::Keyboard::Scancode::Space, MenuCommand::SELECT_OPTION);
}

PauseMenu::~PauseMenu() {
    std::cout << "PauseMenu destructor\n";
}

void PauseMenu::render(sf::RenderWindow& window) {
    sf::View currentView = window.getView();
    sf::Vector2f viewSize = currentView.getSize();
    sf::Vector2f viewCenter = currentView.getCenter();

    sf::Vector2f topLeft(viewCenter.x - viewSize.x / 2, viewCenter.y - viewSize.y / 2);

    background.setSize(viewSize);
    background.setPosition(topLeft);
    window.draw(background);

    if (updateInteractableLocations) {
        std::cout << "updating interactables pausemenu.cpp\n";
        updateMenuInteractableLocations(viewCenter, viewSize);
        updateInteractableLocations = false;
    }
    for (int i = 0; i < (int) interactables.size(); i++) {
        interactables.at(i)->render(window);
    }
}
void PauseMenu::handleEvent(MenuCommand command) {
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

void PauseMenu::onEnter() {
    // pause the game
    updateInteractableLocations = true;
    updateFrameLocations = true;
}
void PauseMenu::onExit() {
    // unpause game
}

void PauseMenu::resumeGame() {
    applicationState.state.control = GameState::Control::GAMEPLAY;
}
void PauseMenu::zoomIn() {
    applicationState.mainViewZoom -= ZOOM_INCREMENT;
    applicationState.updateMainViewZoom = true;
}
void PauseMenu::zoomOut() {
    applicationState.mainViewZoom += ZOOM_INCREMENT;
    applicationState.updateMainViewZoom = true;
}
void PauseMenu::debugToggle() {
    applicationState.debugMode = !applicationState.debugMode;
}
void PauseMenu::toggleBattle() {
    if (applicationState.state.scene == GameState::Scene::MAP) {
        applicationState.state.scene = GameState::Scene::BATTLE;
    } else if (applicationState.state.scene == GameState::Scene::BATTLE) {
        applicationState.state.scene = GameState::Scene::MAP;
    }
}
void PauseMenu::quitGame() {
    applicationState.quitGame = true;
}
