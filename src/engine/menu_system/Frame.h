#ifndef FRAME_H
#define FRAME_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shape.hpp>

class Frame {
    public:
        Frame(const sf::Vector2f& nPos, const sf::Vector2f& nSize)
         : pos(nPos), size(nSize) {

        }
        virtual ~Frame() {
            delete shape;
        }

        // rendering and updating
        virtual void render(sf::RenderWindow& window) = 0;
        virtual void updateRenderBoxLocation(const sf::Vector2f& viewCenter,
            const sf::Vector2f& viewSize) = 0;
    
    protected:
        sf::Shape *shape;
        sf::Vector2f pos;  // as percentage of screen
        sf::Vector2f size;      // as percentage of screen
};

#endif /* FRAME_H */
