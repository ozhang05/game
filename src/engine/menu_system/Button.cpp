#include "Button.h"

#include <iostream>
#include <functional>

// #include "SFML/Graphics/Rect.hpp"
// #include "SFML/Graphics/RectangleShape.hpp"
// #include "SFML/Graphics/RenderWindow.hpp"
#include <SFML/Graphics.hpp>

Button::Button(const sf::FloatRect& rect, const sf::Font& font, std::string buttonName)
    : Interactable(font) {
    onHoverMethod = nullptr;
    onClickMethod = nullptr;

    editBoundingBox(rect);
    getText().setString(buttonName);
}
Button::~Button() {
    std::cout << "Destroying Button " << (std::string) getText().getString() << " Button.cpp\n";
}

/* ************************************************************************** */

void Button::render(sf::RenderWindow& window) {
    // window.draw(getRenderBoxBorder());
    window.draw(getRenderBox());
    window.draw(getText());
}

/* ************************************************************************** */

void Button::updateRenderBoxLocation(const sf::Vector2f& viewCenter,
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

    getText().setPosition(viewOrigin + position);

    sf::FloatRect textBounds = getText().getLocalBounds();
    getText().setOrigin({textBounds.position.x + textBounds.size.x / 2, textBounds.position.y + textBounds.size.y / 2});

    // Adjust text position to keep it centered
    getText().setPosition(viewOrigin + position + sf::Vector2f(size.x / 2.f, size.y / 2.f));

    getRenderBoxBorder().setFillColor(sf::Color::Black);
    getRenderBox().setFillColor(sf::Color(128, 128, 128));
}

void Button::updateScreenSize(const sf::Vector2f& pos, const sf::Vector2f& size) {
    boundingBox.position = pos;
    boundingBox.size = size;
}

/* ************************************************************************** */

Interactable::Type Button::getType() {
    return Interactable::Type::BUTTON;
}

/* ************************************************************************** */

void Button::onHover() {
    getRenderBox().setFillColor(sf::Color(211, 211, 211));

    if (!onHoverMethod) {
        // std::cerr << "ERROR NO HOVER METHOD BUTTON.CPP\n";
        return;
    }

    onHoverMethod();
}

void Button::onHoverExit() {
    getRenderBox().setFillColor(sf::Color(128, 128, 128));
}

void Button::onClick() {
    if (!onClickMethod) {
        std::cerr << "ERROR NO CLICK METHOD BUTTON.CPP\n";
        return;
    }
    
    onClickMethod();
}

void Button::setOnHover(std::function<void()> method) {
    onHoverMethod = method;
}

void Button::setOnClick(std::function<void()> method) {
    onClickMethod = method;
}
