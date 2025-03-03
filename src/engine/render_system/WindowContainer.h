#ifndef WINDOWCONTAINER_H
#define WINDOWCONTAINER_H

#include "Constants.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/VertexArray.hpp>

struct WindowContainer {
    WindowContainer(int width = DEFAULT_WINDOW_WIDTH, int height = DEFAULT_WINDOW_HEIGHT, std::string windowName = DEFAULT_WINDOW_NAME)
        : window(sf::VideoMode({uint(width), uint(height)}), windowName),
            view(sf::FloatRect({0, 0}, {float(width), float(height)})),
            triangleArray(sf::PrimitiveType::Triangles),
            lineArray(sf::PrimitiveType::Lines) {

        triangleArrayIndex = 0;
        lineArrayIndex = 0;

    };

    sf::RenderWindow window;
    sf::View view;
    sf::VertexArray triangleArray;
    sf::VertexArray lineArray;

    int triangleArrayIndex;
    int lineArrayIndex;
};

#endif /* WINDOWCONTAINER_H */
