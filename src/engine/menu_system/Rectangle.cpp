#include "Rectangle.h"

#include <iostream>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>

Rectangle::Rectangle(const sf::Vector2f& nPos, const sf::Vector2f& nSize, const sf::Color& color)
    : Frame(nPos, nSize) {
    this->pos = nPos;
    this->size = nSize;

    shape = new sf::RectangleShape(size);
    shape->setPosition(pos);
    shape->setFillColor(color);
}
Rectangle::~Rectangle() {

}

void Rectangle::render(sf::RenderWindow& window) {
    window.draw(*shape);
}
void Rectangle::updateRenderBoxLocation(const sf::Vector2f& viewCenter,
    const sf::Vector2f& viewSize) {
    sf::Vector2f rectangleViewSize = {
        size.x * viewSize.x, size.y * viewSize.y
    };
    sf::Vector2f rectangleViewPos = {
        (viewCenter.x - (viewSize.x / 2.f)) + pos.x * viewSize.x,
        (viewCenter.y - (viewSize.y / 2.f)) + pos.y * viewSize.y
    };

    if (pos.x == 0) {
        rectangleViewPos.x = viewCenter.x - (viewSize.x / 2.f);
    }
    if (pos.y == 0) {
        rectangleViewPos.y = viewCenter.y - (viewSize.y / 2.f);
    }
    ((sf::RectangleShape *) shape)->setSize(rectangleViewSize);
    shape->setPosition(rectangleViewPos);
}
