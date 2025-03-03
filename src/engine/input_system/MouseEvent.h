#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H

#include "InputEnums.h"

#include <SFML/System/Vector2.hpp>

struct MouseEvent {
    // event information
    MouseEventType type;
    sf::Vector2f location;

    // mouse click information
    bool left = false;
    bool middle = false;
    bool right = false;
};

#endif /* MOUSEEVENT_H */
