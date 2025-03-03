#ifndef APPLICATION_H
#define APPLICATION_H

#include <vector>

#include "ApplicationState.h"
#include "Map.h"
#include "Battle.h"
#include "InputHandler.h"
#include "TextureManager.h"
#include "WindowContainer.h"
#include "ComponentFactory.h"
#include "ComponentSerializer.h"
#include "EntityManager.h"
#include "MovementSystem.h"
#include "MenuManager.h"
#include "sol/sol.hpp"

class Application {
public:
    Application(InputMode im);
    ~Application();
protected:
    // holds various things about the application like debug status, zoom, etc.
    ApplicationState applicationState;

    InputHandler inputHandler;          // must specify mapmaker or game on initialization
    TextureManager textureManager = TextureManager();
    WindowContainer windowContainer;

    ComponentFactory componentFactory = ComponentFactory(textureManager);
    ComponentSerializer componentSerializer = ComponentSerializer();
    EntityManager entityManager = EntityManager(componentFactory);
    MovementSystem movementSystem;

    MenuManager menuManager;            // initialized with the application

    //sf variables and methods
    sf::Clock clock;
    sf::Font mainFont;
    sf::Text fps;
    sf::Text mousePos;

    // fps specific data
    double previousFrameTime;
    int fpsIndex;
    float fpsSum;
    float fpsList[FPS_MAX_SAMPLES];

    Map playerMap;
    Battle battle;

    void renderScene();
    void updateApplication();
    void updateMainViewZoom();
    void updateMainViewLocation();

    //debug functions
    void updateFPS();
    void updateMousePos();

    //initialization functions
    virtual void run() = 0;
};

#endif /* APPLICATION_H */
