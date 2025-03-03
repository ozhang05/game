#include "Menu.h"

#include <iostream>

#include "MouseEvent.h"
#include "Interactable.h"
#include "Textbox.h"
#include "Frame.h"

Menu::Menu(const sf::Font& gf)
    : mainFont(gf) {
    updateInteractableLocations = true;
    updateFrameLocations = true;
    selectedIndex = 0;
    currentMouseHover = -1;
}

Menu::~Menu() {
    for (Interactable *i : interactables) {
        delete i;
    }
    for (Frame *i : frames) {
        delete i;
    }
}
/* ************************************************************************** */

void Menu::updateMenuInteractableLocations(const sf::Vector2f& viewCenter,
    const sf::Vector2f& viewSize) {

    for (Interactable *i : interactables) {
        i->updateRenderBoxLocation(viewCenter, viewSize);
    }
}

void Menu::updateMenuFrameLocations(const sf::Vector2f& viewCenter,
    const sf::Vector2f& viewSize) {
   
    for (Frame *i : frames) {
        i->updateRenderBoxLocation(viewCenter, viewSize);
    }
}

void Menu::queueInteractableUpdate() {
    updateInteractableLocations = true;
}
void Menu::queueFrameUpdate() {
    updateFrameLocations = true;
}

/* ************************************************************************** */

void Menu::handleMouseEvent(const MouseEvent& mouseEvent) {
    if (currentMouseHover != -1) {
        interactables.at(currentMouseHover)->onHoverExit();
        currentMouseHover = -1;
    }
    int j = 0;

    for (Interactable *i : interactables) {
        sf::FloatRect iRect = i->getRenderBoxLocation().getGlobalBounds();

        if (iRect.contains(mouseEvent.location)) {
            if (mouseEvent.left) {
                selectedIndex = j;
                i->onHover();
                i->onClick();
                break;
            } else {
                i->onHover();
                currentMouseHover = j;
            }
        }
        // if (i->getRenderBoxLocation())
        j++;
    }
}

/* ************************************************************************** */

const std::string& Menu::getName() {
    return name;
}
void Menu::setName(const std::string &name) {
    this->name = name;
}
Interactable *Menu::getInteractable() const {
    return interactables.at(selectedIndex);
}

/* ************************************************************************** */

MenuCommand Menu::queryKeyInput(const sf::Event::KeyPressed& e) {
    auto it = inputMap.find(e.scancode);
    if (it == inputMap.end()) return MenuCommand::NONE;

    return it->second;
}

bool Menu::queryTextInput(const char32_t c) {
    if (interactables.at(selectedIndex)->getType() == Interactable::Type::TEXTBOX) {
        Textbox *text = (Textbox *) interactables.at(selectedIndex);
        text->handleTextInput(c);
        return true;
    }
    return false;
}
