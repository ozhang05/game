#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <functional>

#include "Interactable.h"

#include <SFML/Graphics/RenderWindow.hpp>

class Textbox : public Interactable {
    public:
        // cX and cY = center position of button percentage based (0.5 = center)
        // width and height are percentage based off screen size
        Textbox(const sf::FloatRect& rect, const sf::Font& font, std::string buttonName = "default");
        Textbox(const sf::FloatRect& rect, const sf::Font& font, std::string *linkString);
        ~Textbox();

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

        // click and hover (input functions)
        void handleTextInput(const char i);
        void setOnHover(std::function<void()> method);
        void setOnClick(std::function<void()> method);
    
    private:

        void resizeText();

        std::string text;
        std::string *linkedString;
        // sf::Text renderText;
        std::function<void()> onHoverMethod;
        std::function<void()> onClickMethod;
};

#endif /* TEXTBOX_H */
