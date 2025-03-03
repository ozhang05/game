#include "Textbox.h"

#include <iostream>
#include <functional>

#include <SFML/Graphics.hpp>

Textbox::Textbox(const sf::FloatRect& rect, const sf::Font& font, std::string buttonName)
    : Interactable(font),
        linkedString(nullptr) {
    onHoverMethod = nullptr;
    onClickMethod = nullptr;

    text = "";
    editBoundingBox(rect);
    getText().setString(buttonName);
}
Textbox::Textbox(const sf::FloatRect& rect, const sf::Font& font, std::string *linkString)
    : Interactable(font),
        linkedString(linkString) {
    onHoverMethod = nullptr;
    onClickMethod = nullptr;

    text = "";
    editBoundingBox(rect);
    // getText().setString(buttonName);
}
Textbox::~Textbox() {
    std::cout << "Destroying Textbox " << (std::string) getText().getString() << " Textbox.cpp\n";
}

/* ************************************************************************** */

void Textbox::render(sf::RenderWindow& window) {
    // window.draw(getRenderBoxBorder());
    window.draw(getRenderBox());
    window.draw(getText());
}

/* ************************************************************************** */

void Textbox::updateRenderBoxLocation(const sf::Vector2f& viewCenter,
    const sf::Vector2f& viewSize) {

    sf::FloatRect boundingBox = getBoundingBox();

    sf::Vector2f size = {
        boundingBox.size.x * viewSize.x, boundingBox.size.y * viewSize.y
    };
    sf::Vector2f position = {
        boundingBox.position.x * viewSize.x - size.x / 2.f,
        boundingBox.position.y * viewSize.y - size.y / 2.f
    };

    sf::Vector2f viewOrigin = viewCenter - (viewSize / 2.f);
    sf::Vector2f borderOffset = size * 0.05f;

    getRenderBoxBorder().setSize(size * 1.1f);
    getRenderBoxBorder().setPosition(viewOrigin + position - borderOffset);
    getRenderBox().setSize(size);
    getRenderBox().setPosition(viewOrigin + position); // Scale position

    sf::FloatRect textBounds = getText().getLocalBounds();
    getText().setOrigin({textBounds.position.x + textBounds.size.x / 2,
        textBounds.position.y + textBounds.size.y / 2});

    // Adjust text position to keep it centered
    getText().setPosition(viewOrigin + position + sf::Vector2f(size.x / 2.f, size.y / 2.f));

    getRenderBoxBorder().setFillColor(sf::Color::Black);
    getRenderBox().setFillColor(sf::Color(128, 128, 128));
}

void Textbox::updateScreenSize(const sf::Vector2f& pos, const sf::Vector2f& size) {
    boundingBox.position = pos;
    boundingBox.size = size;
}

/* ************************************************************************** */

Interactable::Type Textbox::getType() {
    return Interactable::Type::TEXTBOX;
}

/* ************************************************************************** */

void Textbox::onHover() {
    getRenderBox().setFillColor(sf::Color(211, 211, 211));

    if (!onHoverMethod) {
        // std::cerr << "ERROR NO HOVER METHOD TEXTBOX.CPP\n";
        return;
    }

    onHoverMethod();
}

void Textbox::onHoverExit() {
    getRenderBox().setFillColor(sf::Color(128, 128, 128));
}

void Textbox::onClick() {
    if (!onClickMethod) {
        std::cerr << "ERROR NO CLICK METHOD TEXTBOX.CPP\n";
        return;
    }
    
    onClickMethod();
}

/* ************************************************************************** */

void Textbox::handleTextInput(const char i) {
    std::cout << "input char " << (int) i << "\n";
    if (i == 10) return;                // enter key case
    if (i == 8) {                       // delete case
        if (text.empty()) return;       // do it here to avoid += delete in empty case

        if (linkedString) {
            linkedString->pop_back();
        } else {
            text.pop_back();
        }
    } else {
        if (linkedString) {
            (*linkedString) += (i);
        } else {
            text += i;
        }
    }

    if (linkedString) {
        getText().setString(*linkedString);
    } else {
        getText().setString(text);
    }

    resizeText();
}

void Textbox::setOnHover(std::function<void()> method) {
    onHoverMethod = method;
}

void Textbox::setOnClick(std::function<void()> method) {
    onClickMethod = method;
}

/* ************************************************************************** */

void Textbox::resizeText() {
    // this function works by interpreting already set values to copy formula
    // and set position of text based off updateRenderBoxLocation
    // tell me this isn't beautiful man 1-31-2025 I'm about to get high out of my mind
    sf::Vector2f size = getRenderBox().getSize();
    sf::Vector2f viewOriginAndPos = getRenderBox().getPosition();
    sf::FloatRect textBounds = getText().getLocalBounds();

    getText().setOrigin({textBounds.position.x + textBounds.size.x / 2,
        textBounds.position.y + textBounds.size.y / 2});
    getText().setPosition(viewOriginAndPos + sf::Vector2f(size.x / 2.f, size.y / 2.f));
}
