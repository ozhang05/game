#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Frame.h"

class Rectangle : public Frame {
    public:
        Rectangle(const sf::Vector2f& pos, const sf::Vector2f& size, const sf::Color& color);
        ~Rectangle() override;
        
        void render(sf::RenderWindow& window) override;
        void updateRenderBoxLocation(const sf::Vector2f& viewCenter,
            const sf::Vector2f& viewSize) override;
};

#endif /* RECTANGLE_H */
