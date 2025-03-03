#ifndef BUTTON_H
#define BUTTON_H

#include <functional>

#include "Interactable.h"

#include <SFML/Graphics/RenderWindow.hpp>

class Button : public Interactable {
    public:
        // cX and cY = center position of button percentage based (0.5 = center)
        // width and height are percentage based off screen size
        Button(const sf::FloatRect& rect, const sf::Font& font, std::string buttonName = "default");
        ~Button();

        // visual related functions
        void render(sf::RenderWindow& window) override;
        void updateRenderBoxLocation(const sf::Vector2f& viewCenter,
            const sf::Vector2f& viewSize) override;
        void updateScreenSize(const sf::Vector2f& pos, const sf::Vector2f& size) override;

        // specialization functions
        Type getType() override;

        // menu manager functions
        void onHover() override;
        void onHoverExit() override;
        void onClick() override;

        // click and hover
        void setOnHover(std::function<void()> method);
        void setOnClick(std::function<void()> method);
    
    private:

        std::function<void()> onHoverMethod;
        std::function<void()> onClickMethod;
};

#endif /* BUTTON_H */
