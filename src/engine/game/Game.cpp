#include "Game.h"

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>

#include "Application.h"
#include "InputHandler.h"
#include "EntityManager.h"
#include "MenuManager.h"
#include "Constants.h"
#include "Component.h"
#include "Map.h"
#include "Entity.h"

#include <SFML/Graphics.hpp>

Game::Game()
    : Application(InputMode::GAMEPLAY) {

    if (!startMenu()) {
        exit(EXIT_SUCCESS);
    }

    readSaveData();

    initializePlayer();

    run();
}

Game::~Game() {

}

void Game::checkMapTransition() {
    if (!applicationState.transitionMap) {
        return;
    }
    applicationState.transitionMap = false;

    transitionMap(applicationState.newMapId, applicationState.newMapEntrance);
}

void Game::transitionMap(int mapId, int entrance) {
    sf::Vector2f viewSize = windowContainer.view.getSize();
    sf::Vector2f viewCenter = windowContainer.view.getCenter();
    
    sf::RectangleShape highlight(viewSize);
    highlight.setPosition(viewCenter - (viewSize / 2.f));

    //fade to black
    float dark = 0;
    float timeElapsed;
    while (dark <= 255.f) {
        dark = std::clamp(dark, 0.f, 255.f);
        timeElapsed = deltaTime.restart().asSeconds();
        dark += timeElapsed * TRANSITION_SPEED;
        highlight.setFillColor(sf::Color(0, 0, 0, dark));

        renderScene();
        windowContainer.window.draw(highlight);
        windowContainer.window.display();
    }

    // unloading previous entities
    sf::Vector2i unloadInfo = playerMap.getEntityUnloadingInformation();
    // x = first entity Id, y = number of entities
    entityManager.unloadMap(unloadInfo.x, unloadInfo.y);

    // loading new map and associated entities
    playerMap.update(mapId, entrance);
    int newFirstEntityId = entityManager.loadAllEntities(playerMap.getAllEntities());
    playerMap.setFirstEntityId(newFirstEntityId);

    sf::Vector2i newLoc = playerMap.getEntranceLocation();
    centerPlayerInTile(newLoc.x, newLoc.y);

    viewSize = windowContainer.view.getSize();
    viewCenter = windowContainer.view.getCenter();
    
    highlight.setPosition(viewCenter - (viewSize / 2.f));

    //fade from black
    while (dark >= 0) {
        dark = std::clamp(dark, 0.f, 255.f);
        timeElapsed = deltaTime.restart().asSeconds();
        dark -= timeElapsed * TRANSITION_SPEED;
        highlight.setFillColor(sf::Color(0, 0, 0, dark));

        renderScene();
        windowContainer.window.draw(highlight);
        windowContainer.window.display();
    }
}

void Game::checkEncounter() {
    if (applicationState.state.scene == GameState::Scene::BATTLE) {
        // TODO: initialize a battle here   
        // want to add battleId?

    }
}

void Game::centerPlayerInTile(int x, int y) {
    PositionComponent *pos = (PositionComponent *) player.getComponent("PositionComponent");
    
    pos->setX(x + (TILE_SIZE - PLAYER_WIDTH) / (2.f * TILE_SIZE));
    pos->setY(y + (TILE_SIZE - PLAYER_HEIGHT) / (2.f * TILE_SIZE));

    updatePlayerPos(*pos);
}

void Game::updatePlayerPos(const PositionComponent& pos) {
    RenderableComponent *renderer = (RenderableComponent *) player.getComponent("RenderableComponent");
    CollisionComponent *col = (CollisionComponent *) player.getComponent("CollisionComponent");

    col->updateCollisionBox(pos);
    renderer->updatePos(pos);
    windowContainer.view.setCenter(sf::Vector2f(pos.getX() * TILE_SIZE + PLAYER_WIDTH / 2, pos.getY() * TILE_SIZE + PLAYER_HEIGHT / 2));
    (void) pos;
}

//returns true to start the game, returns false otherwise
bool Game::startMenu() {
    // char userQuery;
    // std::cout << "What would you like to do?\n";
    // std::cout << "C to continue\n";
    // std::cout << "N for new game\n";
    // std::cout << "Q to quit\n";
    // std::cin >> userQuery;
    // userQuery = std::tolower(userQuery);
    // if (userQuery == 'q') {
    //     std::cout << "Thanks for playing\n";
    //     return false;
    // } else if (userQuery == 'n') {
    //     if (previousSave()) {
    //         std::cout << "Are you sure? Save data cannot be recovered (y or n)\n";
    //         std::cin >> userQuery;
    //         userQuery = std::tolower(userQuery);
    //         if (userQuery == 'y') {
    //             resetSaveData();
    //             return true;
    //         }
    //         return false;
    //     }
    //     resetSaveData();
    //     return true;
    // } else if (userQuery == 'c') {
    //     if (previousSave()) {
    //         return true;
    //     } else {
    //         std::cerr << "NO SAVE DATA EXISTS\n";
    //         return false;
    //     }
    // }

    // return false;
    
    return true;
}

void Game::initializePlayer() {

    //read in player location
    float playerX = 10.f;
    float playerY = 10.f;
    player.addComponent(new PositionComponent(playerX, playerY));
    player.addComponent(new PlayerMovementComponent());
    player.addComponent(new MovementComponent(PLAYER_ACCELERATION, PLAYER_MAX_SPEED));
    player.addComponent(new CollisionComponent(PLAYER_WIDTH, PLAYER_HEIGHT, true));
    // player.addComponent(new CollisionComponent(PLAYER_WIDTH / 2, PLAYER_HEIGHT / 2, PLAYER_WIDTH, PLAYER_HEIGHT, playerX, playerY));
    player.addComponent(new RenderableComponent(textureManager.getTexture("player"), "player", {playerX, playerY}));

    entityManager.addEntity(&player);

    windowContainer.view.setCenter(sf::Vector2f(playerX * TILE_SIZE + PLAYER_WIDTH / 2, playerY * TILE_SIZE + PLAYER_HEIGHT / 2));
}

void Game::run() {
    // windowContainer.window.setFramerateLimit(FPS_CAP);
    double currentTime = clock.getElapsedTime().asSeconds();
    double accumilator = 0.0;

    while (windowContainer.window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        
        while (const std::optional event = windowContainer.window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                windowContainer.window.close();
                break;
            }

            inputHandler.processInput(event);
        }

        if (applicationState.state.control == GameState::Control::GAMEPLAY) {
            double newTime = clock.getElapsedTime().asSeconds();
            double frameTime = newTime - currentTime;
            currentTime = newTime;
            accumilator += frameTime;

            while (accumilator > TICK_TIME) {
                // actual updates
                movementSystem.update(TICK_TIME);

                accumilator -= TICK_TIME;
            }
        }

        // TODO: part of hack, get rid please :)
        checkMapTransition();
        // TODO: me too!
        checkEncounter();

        updateApplication();
        
        renderScene();
        
        windowContainer.window.display();
    }

    saveGame();
}

//returns true if there is a previous save file detected
bool Game::previousSave() {
    std::ifstream test = std::ifstream("../save.data");
    return test.good();
}

void Game::saveGame() {
    std::ofstream save = std::ofstream("../save.data");
}

//reads save data and initializes the scene accordingly
void Game::readSaveData() {
    std::ifstream save = std::ifstream("../save.data");
    // mapId is the id that we read from the save data

    // playerMap = Map(0, 0, textureManager);
    playerMap.update(0, 0);
    int loc = entityManager.loadAllEntities(playerMap.getAllEntities());

    playerMap.setFirstEntityId(loc);
}

void Game::resetSaveData() {
    if (previousSave()) {
        std::remove("../save.data");
    }
    
    std::ofstream save = std::ofstream("../save.data");
    //relating to game data
    save << "version = 0.1\n\n";

    //relating to player data
    save << "player:\n";
    save << "map = 0\n";
    save << "x = 0\n";
    save << "y = 0\n";
    //default items
    save << "items:\n";
    save << "items-end\n";
    //default quests
    save << "quests:\n";
    save << "quests-end\n";
    save << "player-end\n";
    return;
}
