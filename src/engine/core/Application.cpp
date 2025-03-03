#include "Application.h"

#include <iostream>

#include "Constants.h"
#include "Map.h"

Application::Application(InputMode im)
    : inputHandler(applicationState, entityManager, menuManager, windowContainer),
        windowContainer(),
        movementSystem(entityManager, playerMap, applicationState),
        menuManager(applicationState, entityManager, componentFactory, componentSerializer, textureManager, mainFont),
        fps(mainFont),
        mousePos(mainFont),
        playerMap(0, 0, textureManager),
        battle(textureManager) {

    applicationState.mode = im;
    if (!mainFont.openFromFile("../assets/fonts/pixel_font.ttf")) {
        std::cerr << "ERROR FAILURE TO OPEN FONT APPLICATION.CPP\n";
    }
}
Application::~Application() {
    // delete playerMap;
    // delete gameWindow;
    // delete mainView;
}

/* ************************************************************************** */

void Application::updateApplication() {
    if (applicationState.updateMainViewZoom) {
        updateMainViewZoom();
        applicationState.updateMainViewZoom = false;
    }
    if (applicationState.updateMainViewLocation) {
        updateMainViewLocation();
        applicationState.updateMainViewLocation = false;
    }
    if (applicationState.quitGame) {
        windowContainer.window.close();
    }
}

void Application::renderScene() {
    windowContainer.window.clear();

    switch(applicationState.state.scene) {
        case GameState::Scene::MAP:
            windowContainer.window.setView(windowContainer.view);
            playerMap.updateBackground(windowContainer.view.getCenter(), windowContainer.view.getSize());
            windowContainer.window.draw(playerMap);
            break;
        case GameState::Scene::BATTLE:
            battle.render(windowContainer);
            break;
        default:
            break;
    }

    // windowContainer.window.setView(windowContainer.view);
    // playerMap.updateBackground(windowContainer.view.getCenter(), windowContainer.view.getSize());
    // windowContainer.window.draw(playerMap);

    updateFPS();
    updateMousePos();

    entityManager.renderAll(&windowContainer.window);
    // if (applicationState.state.primary == InputState::Primary::MENU) {
    //     menuManager.render(windowContainer.window);
    // }
    if (applicationState.state.control == GameState::Control::MENU) {
        menuManager.render(windowContainer.window);
    }

    //debug step
    if (applicationState.debugMode) {
        playerMap.renderDebugHighlights(windowContainer);
        // playerMap.renderCollisionTileHighlights(windowContainer);
        // playerMap.renderTransitionTileHighlights(windowContainer);

        entityManager.renderDebugHighlights(windowContainer);
        movementSystem.renderDebugHighlights(windowContainer);
        windowContainer.window.draw(fps);
        windowContainer.window.draw(mousePos);
    }

    // TODO: make this better maybe;
    windowContainer.window.draw(windowContainer.triangleArray);
    windowContainer.triangleArrayIndex = 0;
    windowContainer.triangleArray.clear();

    windowContainer.window.draw(windowContainer.lineArray);
    windowContainer.lineArrayIndex = 0;
    windowContainer.lineArray.clear();
}

void Application::updateMainViewZoom() {
    if (applicationState.mainViewZoom > MAX_ZOOM) {
        applicationState.mainViewZoom = MAX_ZOOM;
    } else if (applicationState.mainViewZoom < MIN_ZOOM) {
        applicationState.mainViewZoom = MIN_ZOOM;
    }

    sf::Vector2f currentCenter = windowContainer.view.getCenter();
    windowContainer.view = windowContainer.window.getDefaultView();

    windowContainer.view.zoom(applicationState.mainViewZoom);
    windowContainer.view.setCenter(currentCenter);
}

void Application::updateMainViewLocation() {
    windowContainer.view.setCenter(applicationState.mainViewLocation);
}

/* ************************************************************************** */

void Application::updateFPS() {
    double time = clock.getElapsedTime().asSeconds() - previousFrameTime;
    previousFrameTime = clock.getElapsedTime().asSeconds();

    fpsSum += time;
    fpsSum -= fpsList[fpsIndex];
    fpsList[fpsIndex] = time;
    

    if (++fpsIndex == FPS_MAX_SAMPLES) {
        fpsIndex = 0;
    }
    float avgFPS = FPS_MAX_SAMPLES / fpsSum; 
    std::string temp = std::to_string(avgFPS);
    temp = temp.substr(0, temp.find(".") + 3);

    fps.setString(temp + " FPS");
    sf::Vector2f loc = windowContainer.view.getCenter();
    loc -= windowContainer.view.getSize() / 2.f;
    fps.setPosition(loc);
}

void Application::updateMousePos() {
    sf::Vector2f mouseLocation = inputHandler.getMouseEvent().location;

    std::string xLoc = std::to_string(mouseLocation.x);
    std::string yLoc = std::to_string(mouseLocation.y);

    mousePos.setString("(" + xLoc + ", " + yLoc + ")");
    sf::Vector2f loc = windowContainer.view.getCenter();
    loc -= windowContainer.view.getSize() / 2.f;
    loc.y += 15.f;
    mousePos.setPosition(loc);
}

/* ************************************************************************** */
