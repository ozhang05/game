#include "InputHandler.h"

#include <iostream>

#include "Entity.h"
#include "Component.h"
#include "EntityManager.h"
#include "InputEnums.h"
#include "MenuManager.h"

InputHandler::InputHandler(ApplicationState& as, EntityManager& em, MenuManager& mm, WindowContainer& wc)
    : applicationState(as), entityManager(em), menuManager(mm), windowContainer(wc) {
    mover = nullptr;
}
InputHandler::~InputHandler() {

}

/* ************************************************************************** */

bool InputHandler::setMover() {
    mover = entityManager.getMover();
    return mover != nullptr;
}

void InputHandler::processInput(const std::optional<sf::Event>& e) {

    if (e->is<sf::Event::Closed>()) {
        applicationState.quitGame = true;
        return;
    }

    // TODO: fix this when you implement mouse events for all game
    mouseEvent.type = MouseEventType::NONE;
    // mouseEvent.left = false;
    // mouseEvent.middle = false;
    // mouseEvent.right = false;
    updateMouseEventLocation(e);

    unloadedMap = false; // TODO: see comment in inputhandler.h

    switch(applicationState.state.control) {
        case GameState::Control::GAMEPLAY:
            processGameplayInputs(e);
            break;
        case GameState::Control::MENU:
            processMenuInputs(e);
            break;
        case GameState::Control::TEXTBOX:
            processTextboxInputs(e);
            break;
        default:
            break;
    }

    // for mode specific inputs
    switch(applicationState.mode) {
        case InputMode::GAMEPLAY:
            gameplaySpecificInputs(e);
            break;
        case InputMode::MAPMAKER:
            mapmakerSpecificInputs(e);
            break;
        default:
            break;
    }
}

/* ************************************************************************** */

const MouseEvent& InputHandler::getMouseEvent() {
    return mouseEvent;
}

/* ************************************************************************** */

void InputHandler::updateMouseEventLocation(const std::optional<sf::Event>& e) {
    sf::Vector2i loc;
    if (const auto *mouseButtonPressed = e->getIf<sf::Event::MouseButtonPressed>()) {
        loc = mouseButtonPressed->position;
    } else if (const auto *mouseButtonReleased = e->getIf<sf::Event::MouseButtonReleased>()) {
        loc = mouseButtonReleased->position;
    } else if (const auto *mouseMoved = e->getIf<sf::Event::MouseMoved>()) {
        loc = mouseMoved->position;
    }
    mouseEvent.location = windowContainer.window.mapPixelToCoords(loc, windowContainer.view);
}

void InputHandler::changeState(GameState::Control control) {
    // resets things that should be reset with state
    PlayerMovementComponent *pmov = (PlayerMovementComponent *) mover->getComponent("PlayerMovementComponent");
    pmov->resetAll();

    applicationState.state.control = control;
}

/* ************************************************************************** */

void InputHandler::processGameplayInputs(const std::optional<sf::Event>& e) {
    if (!setMover()) {
        std::cerr << "MOVER NOT SET INPUTMANAGER.CPP\n";
    }

    if (const auto *keyEvent = e->getIf<sf::Event::KeyPressed>()) {
        processGameplayKeyPressed(*keyEvent);
    } else if (const auto *keyEvent = e->getIf<sf::Event::KeyReleased>()) {
        processGameplayKeyReleased(*keyEvent);
    }
}

void InputHandler::processGameplayKeyPressed(const sf::Event::KeyPressed& e) {

    PlayerMovementComponent *camMover = (PlayerMovementComponent *) mover->getComponent("PlayerMovementComponent");

    switch (e.scancode) {
        // player movement
        case sf::Keyboard::Scancode::W:
            camMover->setW(true);
            break;
        case sf::Keyboard::Scancode::A:
            camMover->setA(true);
            break;
        case sf::Keyboard::Scancode::S:
            camMover->setS(true);
            break;
        case sf::Keyboard::Scancode::D:
            camMover->setD(true);
            break;

        // settings related
        case sf::Keyboard::Scancode::Hyphen:
            applicationState.mainViewZoom += ZOOM_INCREMENT;
            applicationState.updateMainViewZoom = true;
            break;
        case sf::Keyboard::Scancode::Equal:
            applicationState.mainViewZoom -= ZOOM_INCREMENT;
            applicationState.updateMainViewZoom = true;
            break;
        case sf::Keyboard::Scancode::P:
        case sf::Keyboard::Scancode::Escape:
            menuManager.loadMenu("Pause");
            changeState(GameState::Control::MENU);
            // pass input to menumanager
            break;
        // TODO: REMOVE DEBUG CASE
        case sf::Keyboard::Scancode::B:
            menuManager.loadMenu("Battle");
            changeState(GameState::Control::MENU);
            break;
        
        default:
            break;
    }
}

void InputHandler::processGameplayKeyReleased(const sf::Event::KeyReleased& e) {

    PlayerMovementComponent *camMover = (PlayerMovementComponent *) mover->getComponent("PlayerMovementComponent");

    switch (e.scancode) {
        // player movement
        case sf::Keyboard::Scancode::W:
            camMover->setW(false);
            break;
        case sf::Keyboard::Scancode::A:
            camMover->setA(false);
            break;
        case sf::Keyboard::Scancode::S:
            camMover->setS(false);
            break;
        case sf::Keyboard::Scancode::D:
            camMover->setD(false);
            break;
        default:
            break;
    }
}

/* ************************************************************************** */

void InputHandler::processMenuInputs(const std::optional<sf::Event>& e) {

    if (e->is<sf::Event::Resized>()) {
        // rerender interactables if menu size is changed
        menuManager.queueInteractableLocationUpdate();
        menuManager.queueFrameLocationUpdate();
    }

        if (const auto *keyPressed = e->getIf<sf::Event::KeyPressed>()) {
        std::cout << "processing key\n";
        processMenuKeyPressed(*keyPressed);
    }
    
    if (const auto *mousePressed = e->getIf<sf::Event::MouseButtonPressed>()) {
        processMenuMousePressed(*mousePressed);
    } else if (const auto *mouseReleased = e->getIf<sf::Event::MouseButtonReleased>()) {
        processMenuMouseReleased(*mouseReleased);
    } else if (const auto *mouseMoved = e->getIf<sf::Event::MouseMoved>()) {
        processMenuMouseMoved(*mouseMoved);
    }
}

void InputHandler::processMenuKeyPressed(const sf::Event::KeyPressed& e) {
    MenuCommand m = menuManager.queryKeyInput(e);
    
    switch (m) {
        case MenuCommand::CLOSE_MENU:
            menuManager.unloadMenu();
            changeState(GameState::Control::GAMEPLAY);
            unloadedMap = true;
            break;
        case MenuCommand::NAVIGATE_UP:
        case MenuCommand::NAVIGATE_DOWN:
        case MenuCommand::NAVIGATE_LEFT:
        case MenuCommand::NAVIGATE_RIGHT:
        case MenuCommand::SELECT_OPTION:
            menuManager.executeMenuCommand(m);
            break;
        case MenuCommand::NONE:
            break;
    }
}

void InputHandler::processMenuMousePressed(const sf::Event::MouseButtonPressed& e) {
    if (e.button == sf::Mouse::Button::Left) {
        mouseEvent.left = true;
    }
    menuManager.processMouseEvent(mouseEvent);

    // if we quit out of menu, the release event is not processed
    if (applicationState.state.control != GameState::Control::MENU) {
        mouseEvent.left = false;
    }
}

void InputHandler::processMenuMouseReleased(const sf::Event::MouseButtonReleased& e) {
    if (e.button == sf::Mouse::Button::Left) {
        mouseEvent.left = false;
    }
}

void InputHandler::processMenuMouseMoved(const sf::Event::MouseMoved& e) {
    (void) e;
    menuManager.processMouseEvent(mouseEvent);
}


/* ************************************************************************** */

void InputHandler::processTextboxInputs(const std::optional<sf::Event>& e) {
    // only try textbox if it is textbox mode
    std::cout << "textbox input received\n";
    if (const auto *textEntered = e->getIf<sf::Event::TextEntered>()) {
        // if text entered worked
        menuManager.textboxInput(textEntered->unicode);
    }
}

/* ************************************************************************** */

void InputHandler::gameplaySpecificInputs(const std::optional<sf::Event>& e) {
    (void) e;
    (void) entityManager;
    (void) menuManager;
    // std::cout << "hello game\n";
}
void InputHandler::mapmakerSpecificInputs(const std::optional<sf::Event>& e) {
    if (const auto *keyPressed = e->getIf<sf::Event::KeyPressed>()) {
        mapmakerSpecificKeyPressed(*keyPressed);
    } else if (const auto *mousePressed = e->getIf<sf::Event::MouseButtonPressed>()) {
        mapmakerSpecificMousePressed(*mousePressed);
    } else if (const auto *mouseReleased = e->getIf<sf::Event::MouseButtonReleased>()) {
        mapmakerSpecificMouseReleased(*mouseReleased);
    } else if (const auto *mouseMoved = e->getIf<sf::Event::MouseMoved>()) {
        mapmakerSpecificMouseMoved(*mouseMoved);
    }
}

void InputHandler::mapmakerSpecificKeyPressed(const sf::Event::KeyPressed& e) {
    if (e.scancode == sf::Keyboard::Scancode::E) {
        if (applicationState.state.control == GameState::Control::MENU) return;
        if (unloadedMap) return;
        menuManager.loadMenu("Entity");
        changeState(GameState::Control::MENU);
    }
}
void InputHandler::mapmakerSpecificMousePressed(const sf::Event::MouseButtonPressed& e) {
    if (e.button == sf::Mouse::Button::Left) {
        mouseEvent.left = true;
        mouseEvent.type = MouseEventType::LEFTCLICK;
    } else if (e.button == sf::Mouse::Button::Middle) {
        mouseEvent.middle = true;
        mouseEvent.type = MouseEventType::MIDDLECLICK;
    } else if (e.button == sf::Mouse::Button::Right) {
        mouseEvent.right = true;
        mouseEvent.type = MouseEventType::RIGHTCLICK;
    }
}
void InputHandler::mapmakerSpecificMouseReleased(const sf::Event::MouseButtonReleased& e) {
    if (e.button == sf::Mouse::Button::Left) {
        mouseEvent.left = false;
    } else if (e.button == sf::Mouse::Button::Middle) {
        mouseEvent.middle = false;
    } else if (e.button == sf::Mouse::Button::Right) {
        mouseEvent.right = false;
    }
}
void InputHandler::mapmakerSpecificMouseMoved(const sf::Event::MouseMoved& e) {
    (void) e;
    mouseEvent.type = MouseEventType::MOVE;
}
