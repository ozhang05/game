#include "Editor.h"

#include <string>
#include <iostream>
#include <fstream>

#include "Application.h"
#include "Constants.h"
#include "Component.h"
#include "Map.h"
#include "Tile.h"
#include "Entity.h"

#include <SFML/Graphics.hpp>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

Editor::Editor()
    : Application(InputMode::MAPMAKER),
        tileDisplay(tileDisplayTexture) {
    if (!startMenu()) {
        exit(EXIT_SUCCESS);
    }

    initializeEditor();

    run();

    writeToFile("test");
}

void Editor::renderScene() {
    Application::renderScene();
    displayTileset();
}

void Editor::displayTileset() {
    sf::Vector2f windowSize = sf::Vector2f(windowContainer.window.getSize());
    sf::Vector2f textureSize = sf::Vector2f(tileDisplayTexture.getSize());
    float ratio = (MAPMAKER_TILESET_RATIO * windowSize.y) / textureSize.y;

    sf::FloatRect normalizedViewport(
        {(windowSize.x - (textureSize.x * ratio)) / windowSize.x, 1 - MAPMAKER_TILESET_RATIO},
        {(textureSize.x * ratio) / windowSize.x, MAPMAKER_TILESET_RATIO}
    );
    
    tileDisplayView.setViewport(normalizedViewport);
    windowContainer.window.setView(tileDisplayView);
    windowContainer.window.draw(tileDisplay);
    windowContainer.window.setView(windowContainer.view);
}

void Editor::handleMouseInput() {
    if (applicationState.state.control == GameState::Control::MENU) return;

    MouseEvent mouseEvent = inputHandler.getMouseEvent();
    MouseEventType eventType = mouseEvent.type;

    switch(eventType) {
        case MouseEventType::LEFTCLICK:
            if (!mouseEvent.left) break;
            handleTileLeftClick(mouseEvent.location);
            break;
        case MouseEventType::MIDDLECLICK:
            if (!mouseEvent.middle) break;
            handleTileMiddleClick(mouseEvent.location);
            break;
        case MouseEventType::RIGHTCLICK:
            if (!mouseEvent.right) break;
            handleTileRightClick(mouseEvent.location);
            break;
        case MouseEventType::MOVE:
            if (mouseEvent.left) handleTileLeftClick(mouseEvent.location);
            if (mouseEvent.middle) handleTileMiddleClick(mouseEvent.location);
            break;
        default:
            break;
    }
}

void Editor::handleTileLeftClick(const sf::Vector2f& loc) {
    sf::Vector2u textureSize = tileDisplayTexture.getSize();

    // pixelLoc is used to find tilesetLoc
    // tilesetLoc gives coordinates relative to top left of tileset
    sf::Vector2i pixelLoc = windowContainer.window.mapCoordsToPixel(loc, windowContainer.view);
    sf::Vector2f tilesetLoc = windowContainer.window.mapPixelToCoords(pixelLoc, tileDisplayView);

    float ratio = 1.f;
    float maxX = textureSize.x * ratio;
    float maxY = textureSize.y * ratio;
    
    // calculate everything based off TILE_SIZE because mapPixelToCoords gives world coordinates
    if (tilesetLoc.x >= 0 && tilesetLoc.y >= 0
        && tilesetLoc.x <= maxX && tilesetLoc.y <= maxY) {

        // selecting tile case

        int x = tilesetLoc.x / TILE_SIZE;
        int y = tilesetLoc.y / TILE_SIZE;

        selectedTile = y * tilesetWidth + x;
        std::cout << "selected tile " << selectedTile << "\n";
    } else if (inTilemap(loc)) {

        // placing tile case
        int tileX = loc.x / TILE_SIZE;
        int tileY = loc.y / TILE_SIZE;

        // skip previously selected tile
        // if (tileX == int(previousTile.x) && tileY == int(previousTile.y)) return;
        // previousTile.x = tileX;
        // previousTile.y = tileY;

        playerMap.getTile({tileX, tileY})->tileId = selectedTile;
        playerMap.updateTile(tileX, tileY);
    }
}

void Editor::handleTileMiddleClick(const sf::Vector2f& loc) {
    if (!inTilemap(loc)) {
        return;
    }
    int tileX = loc.x / TILE_SIZE;
    int tileY = loc.y / TILE_SIZE;
        
    if (tileX == int(previousTile.x) && tileY == int(previousTile.y)) return;
    previousTile.x = tileX;
    previousTile.y = tileY;

    // reverse the traversability when seeing middle click
    playerMap.getTile({tileX, tileY})->isTraversable = !playerMap.getTile({tileX, tileY})->isTraversable;
}

void Editor::handleTileRightClick(const sf::Vector2f& loc) {
    if (!inTilemap(loc)) {
        return;
    }
    int tileX = loc.x / TILE_SIZE;
    int tileY = loc.y / TILE_SIZE;

    // menuManager.loadMenu("Entity");
    // applicationState.state = InputState::MENU;

    (void) tileX;
    (void) tileY;
}

bool Editor::inTilemap(const sf::Vector2f& loc) {
    return loc.x >= 0 && loc.y >= 0 && loc.x <= mapWidth * TILE_SIZE && loc.y <= mapHeight * TILE_SIZE;
}

/* ************************************************************************** */

bool Editor::startMenu() {
    // std::cout << "Enter the mapId:\n";
    // std::cin >> mapId;
    // std::cout << "Enter width:\n";
    // std::cin >> mapWidth;
    // std::cout << "Enter heigh:\n";
    // std::cin >> mapHeight;
    // std::cout << "Enter name of tileset as follows (\"___.png\", located in assets folder):\n";
    // std::cin >> tilesetName;
    // std::cout << "Enter the background image as follows (\"___.png\")\n";
    // std::cin >> backgroundName;
    
    mapId = 2;
    mapWidth = 15;
    mapHeight = 10;
    tilesetName = "default_tileset";
    backgroundName = "background_tree";

    return true;
}

void Editor::initializeEditor() {
    json newMap;
    newMap["height"] = mapHeight;
    newMap["width"] = mapWidth;
    newMap["tileset"] = tilesetName;
    newMap["background"] = backgroundName;
    for (int i = 0; i < mapHeight; i++) {
        for (int j = 0; j < mapWidth; j++) {
            newMap["map_tiles"][i][j] = 0;
            newMap["collision_tiles"][i][j] = 1;
        }
    }
    newMap["entities"] = json::array();
    newMap["entrance_tiles"] = json::array();
    newMap["exit_tiles"] = json::array();

    playerMap.update(newMap.dump());

    mover.addComponent(new PositionComponent(0, 0));
    mover.addComponent(new PlayerMovementComponent);
    mover.addComponent(new MovementComponent(PLAYER_ACCELERATION, PLAYER_MAX_SPEED));
    entityManager.addEntity(&mover);

    tileDisplayTexture = textureManager.getTexture(tilesetName);
    sf::Vector2u tdtSize = tileDisplayTexture.getSize();
    tileDisplay.setTexture(tileDisplayTexture, true);
    tileDisplay.setPosition({0, 0});

    tilesetWidth = tdtSize.x / TILE_SIZE;
    tilesetHeight = tdtSize.y / TILE_SIZE;

    tileDisplayView.setSize(sf::Vector2f(tdtSize.x, tdtSize.y));
    tileDisplayView.setCenter({tdtSize.x / 2.0f, tdtSize.y / 2.0f});
}

void Editor::run() {
    windowContainer.window.setFramerateLimit(FPS_CAP);
     
    while (windowContainer.window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        
        while (const std::optional event = windowContainer.window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                windowContainer.window.close();
                break;
            }

            inputHandler.processInput(event);

            handleMouseInput();
        }

        movementSystem.update(0.01);

        // updateView();

        updateApplication();

        renderScene();

        windowContainer.window.display();
    }
    // int answer;
    // std::string filename;
    // std::cout << "would you like to save? 1 to save, 0 to delete\n";
    // std::cin >> answer;
    // if (answer == 0) {
    //     exit(EXIT_SUCCESS);
    // }
    // std::cout << "what should the file be called?\n";
    // std::cin >> filename;
    // if (writeToFile(filename)) {
    //     std::cout << "successfully written to buffer\n";
    // }
}

bool Editor::writeToFile(const std::string& fileName) {
    std::ofstream outfile("../data/buffer/" + fileName + ".data");
    if (outfile.fail()) {
        return false;
    }

    json output;
    output["height"] = mapHeight;
    output["width"] = mapWidth;
    output["tileset"] = tilesetName;
    output["background"] = backgroundName;

    for (int i = 0; i < mapHeight; i++) {
        for (int j = 0; j < mapWidth; j++) {
            output["map_tiles"][i][j] = playerMap.getTile({j, i})->tileId;
            output["collision_tiles"][i][j] = playerMap.getTile({j, i})->isTraversable;
        }
    }

    int i = 0;
    for (auto it : entityManager.getEntities()) {
        if (it.second == entityManager.getMover()) continue;

        output["entities"][i]["name"] = it.second->getName();

        for (std::string componentName : ComponentNames) {
            Component *component = it.second->getComponent(componentName);

            if (component == nullptr) continue;

            // encode component parts into this
            json cs = componentSerializer.encodeComponent(component, componentName);
            output["entities"][i]["components"][componentName] = cs;
        }
        i++;
    }
    
    //encode entities here
    //encode entrance tiles
    //encode exit tiles
    
    outfile << output;
    return false;
}
