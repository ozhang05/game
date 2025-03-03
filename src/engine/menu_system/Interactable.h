#ifndef INTERACTABLE_H
#define INTERACTABLE_H

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class Interactable {
    public:
        enum class Type {
            NONE,
            BUTTON,
            TEXTBOX
        };

        Interactable(const sf::Font& font)
            : interactableText(font) {
            
        }
        virtual ~Interactable() = default;

        // rendering and updating
        virtual void render(sf::RenderWindow& window) = 0;
        virtual void updateRenderBoxLocation(const sf::Vector2f& viewCenter,
            const sf::Vector2f& viewSize) = 0;
        virtual void updateScreenSize(const sf::Vector2f& pos, const sf::Vector2f& size) = 0;
        const sf::RectangleShape& getRenderBoxLocation() {
            return renderBox;
        }

        // debug related
        std::string getName() {
            return interactableText.getString();
        }
        virtual Type getType() = 0;

        // player interaction functions
        virtual void onHover() = 0;
        virtual void onHoverExit() = 0;
        virtual void onClick() = 0;
    protected:
        bool isEnabled() {
            return enabled;
        }
        void setEnabled(bool e) {
            enabled = e;
        }

        // change these functions around later, and all dependent code
        sf::Text& getText() {
            return interactableText;
        }
        sf::FloatRect getBoundingBox() {
            return boundingBox;
        }
        void editBoundingBox(const sf::FloatRect& rect) {
            boundingBox = rect;
        }
        sf::RectangleShape& getRenderBox() {
            return renderBox;
        }
        sf::RectangleShape& getRenderBoxBorder() {
            return renderBoxBorder;
        }
    protected:
        bool enabled;

        sf::Text interactableText;      // name for interactable (displayed on screen)
        sf::FloatRect boundingBox;      // bounding box for interactable
        sf::RectangleShape renderBox;   // box to be rendered
        sf::RectangleShape renderBoxBorder;
};

#endif /* INTERACTABLE_H */
