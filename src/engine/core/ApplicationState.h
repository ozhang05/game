#ifndef APPLICATION_STATE_H
#define APPLICATION_STATE_H

#include "InputEnums.h"

#include <SFML/System/Vector2.hpp>

struct GameState {
    enum class Scene { MAP, BATTLE };
    enum class Control { GAMEPLAY, MENU, TEXTBOX };

    Scene scene = Scene::MAP;
    Control control = Control::GAMEPLAY;
};

struct ApplicationState {
    ApplicationState() {

    }
    bool quitGame = false;
    InputMode mode;
    GameState state;

    bool debugMode = true;  // TODO: change this back to false

    // game relevant variables
    bool updateMainViewZoom = false;                // used to tell app to update zoom
    float mainViewZoom = 1.f;

    // mover relevant variables
    bool updateMainViewLocation = false;
    sf::Vector2f mainViewLocation = {0, 0};
    
    // game specific variables
    // HACK: should be abstracted out. does map maker need these?
    bool transitionMap = false;
    int newMapId = 0;
    int newMapEntrance = 0;

    bool startBattle = false;
    sf::Vector2i lastPlayerLocation;

};

#endif /* APPLICATION_STATE_H */
