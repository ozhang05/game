#include <iostream>
#include <string>

#include "map.h"

#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 800), "My window");

    Map testMap = Map(0, 0);

    //testMap.render(0, 0, 0);

    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::KeyPressed) {
                std::cout << event.key.code << " " << event.key.scancode << "\n";
            }
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) {
                window.close();
                break;
            }
        }

        window.draw(testMap);
        window.display();
    }

    return 0;
}