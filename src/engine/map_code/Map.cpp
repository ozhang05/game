#include "Map.h"

#include <string>
#include <iostream>
#include <fstream>
#include <cmath>

#include "Constants.h"
#include "Colors.h"
#include "Tile.h"
#include "TileEffect.h"
#include "TextureManager.h"
#include "WindowContainer.h"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

Map::Map(int environment, int entrance, TextureManager& tm)
    : textureManager(tm) {
    mapId = environment;

    if (!load(environment, entrance)) {
        std::cerr << "ERROR: ERROR LOADING MAP\n";
        std::cerr << environment << " " << entrance << "\n";
        exit(EXIT_FAILURE);
    }
}

Map::Map(const std::string& inputJson, TextureManager& tm)
    : textureManager(tm) {

    createMap(inputJson);
}

Map::~Map() {
    delete[] tileMap;
}

void Map::update(int environment, int entrance) {
    if (!load(environment, entrance)) {
        std::cerr << "ERROR: ERROR UPDATING MAP MAP.CPP\n";
        exit(EXIT_FAILURE);
    }
}
void Map::update(const std::string& inputJson) {
    if (!createMap(inputJson)) {
        std::cerr << "ERROR: ERROR CREATING MAP MAP.CPP\n";
        exit(EXIT_FAILURE);
    }
}

void Map::setFirstEntityId(int newId) {
    firstEntityId = newId;
}

bool Map::collides(const sf::FloatRect& collisionBox) const {
    int xVal = std::floor(collisionBox.position.x);
    int yVal = std::floor(collisionBox.position.y);

    int xEnd = std::floor(collisionBox.position.x + collisionBox.size.x);
    int yEnd = std::floor(collisionBox.position.y + collisionBox.size.y);

    if (xVal < 0 || xEnd >= mapSize.x || yVal < 0 || yEnd >= mapSize.y) {
        return true;
    }

    for (int i = yVal; i <= yEnd; i++) {
        for (int j = xVal; j <= xEnd; j++) {
            if (getTile({j, i})->isTraversable == false) {
                return true;
            }
        }
    }
    return false;
}

Tile *Map::isMapTransition(const sf::FloatRect &collisionBox) const {
    int xVal = std::floor(collisionBox.position.x);
    int yVal = std::floor(collisionBox.position.y);

    int xEnd = std::floor(collisionBox.position.x + collisionBox.size.x);
    int yEnd = std::floor(collisionBox.position.y + collisionBox.size.y);

    if (xVal < 0 || xEnd >= mapSize.x || yVal < 0 || yEnd >= mapSize.y) {
        return nullptr;
    }

    for (int i = yVal; i <= yEnd; i++) {
        for (int j = xVal; j <= xEnd; j++) {
            Tile *curr = getTile({j, i});
            if (curr->isTransition == true) {
                return curr;
            }
        }
    }
    return nullptr;
}

Tile *Map::getTile(sf::Vector2i pos) const {
    if (pos.y * mapSize.x + pos.x > mapSize.y * mapSize.x) return nullptr;
    if (pos.y * mapSize.x + pos.x < 0) return nullptr;
    return &tileMap[pos.y * mapSize.x + pos.x];
}

const sf::Vector2i& Map::getMapSize() const {
    return mapSize;
}

const sf::Vector2i& Map::getEntranceLocation() const {
    return entranceLocation;
}

const json& Map::getAllEntities() const {
    return entities;
}

sf::Vector2i Map::getEntityUnloadingInformation() const {
    return sf::Vector2i(firstEntityId, entities.size());
}

void Map::updateTilemap() {
    for (int i = 0; i < mapSize.y; i++) {
        for (int j = 0; j < mapSize.x; j++) {
            updateTile(j, i);
        }
    }
}

void Map::updateTile(int x, int y) {
    int tileNumber = getTile({x, y})->tileId;

    int tw = tileNumber % int(mapTileset.getSize().x / TILE_SIZE);
    int th = tileNumber / int(mapTileset.getSize().x / TILE_SIZE);

    sf::Vertex* triangle = &mapVertices[(y * mapSize.x + x) * 6];
    
    // First triangle (top-left, top-right, bottom-right)
    triangle[0].position = sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE);
    triangle[1].position = sf::Vector2f((x + 1) * TILE_SIZE, y * TILE_SIZE);
    triangle[2].position = sf::Vector2f((x + 1) * TILE_SIZE, (y + 1) * TILE_SIZE);

    triangle[0].texCoords = sf::Vector2f(tw * TILE_SIZE, th * TILE_SIZE);
    triangle[1].texCoords = sf::Vector2f((tw + 1) * TILE_SIZE, th * TILE_SIZE);
    triangle[2].texCoords = sf::Vector2f((tw + 1) * TILE_SIZE, (th + 1) * TILE_SIZE);

    // Second triangle (top-left, bottom-right, bottom-left)
    triangle = &mapVertices[(y * mapSize.x + x) * 6 + 3];

    triangle[0].position = sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE);
    triangle[1].position = sf::Vector2f((x + 1) * TILE_SIZE, (y + 1) * TILE_SIZE);
    triangle[2].position = sf::Vector2f(x * TILE_SIZE, (y + 1) * TILE_SIZE);

    triangle[0].texCoords = sf::Vector2f(tw * TILE_SIZE, th * TILE_SIZE);
    triangle[1].texCoords = sf::Vector2f((tw + 1) * TILE_SIZE, (th + 1) * TILE_SIZE);
    triangle[2].texCoords = sf::Vector2f(tw * TILE_SIZE, (th + 1) * TILE_SIZE);
}

void Map::updateBackground(const sf::Vector2f& center, const sf::Vector2f& size) {
    
    int screenX = center.x - (size.x / 2);
    int screenY = center.y - (size.y / 2);
    int screenWidth = size.x;
    int screenHeight = size.y;

    int textureX = backgroundTexture.getSize().x;
    int textureY = backgroundTexture.getSize().y;

    int textureXStart = screenX % textureX;
    int textureYStart = screenY % textureY;

    float repeatX = (float) screenWidth / textureX;
    float repeatY = (float) screenHeight / textureY;

    sf::Vertex* triangle = &backgroundVertices[0];

    // First triangle (top-left, top-right, bottom-right)
    triangle[0].position = sf::Vector2f(screenX, screenY);
    triangle[1].position = sf::Vector2f(screenX + screenWidth, screenY);
    triangle[2].position = sf::Vector2f(screenX + screenWidth, screenY + screenHeight);

    triangle[0].texCoords = sf::Vector2f(textureXStart, textureYStart);
    triangle[1].texCoords = sf::Vector2f(textureXStart + repeatX * textureX, textureYStart);
    triangle[2].texCoords = sf::Vector2f(textureXStart + repeatX * textureX, textureYStart + repeatY * textureY);

    // Second triangle (top-left, bottom-right, bottom-left)
    triangle[3].position = sf::Vector2f(screenX, screenY);
    triangle[4].position = sf::Vector2f(screenX + screenWidth, screenY + screenHeight);
    triangle[5].position = sf::Vector2f(screenX, screenY + screenHeight);

    triangle[3].texCoords = sf::Vector2f(textureXStart, textureYStart);
    triangle[4].texCoords = sf::Vector2f(textureXStart + repeatX * textureX, textureYStart + repeatY * textureY);
    triangle[5].texCoords = sf::Vector2f(textureXStart, textureYStart + repeatY * textureY);
}

void Map::renderDebugHighlights(WindowContainer& wc) const {
    for (int i = 0; i < mapSize.y; i++) {
        for (int j = 0; j < mapSize.x; j++) {
            Tile *curr = getTile({j, i});

            sf::Vector2i pos(j, i);

            if (curr->isTraversable != true) {
                renderTileDebugHighlight(wc, pos, mapCollisionTile);
            }
            if (curr->isTransition == true) {
                renderTileDebugHighlight(wc, pos, mapTransitionTile);
            }
            switch(curr->effect) {
                case TileEffect::None:
                    continue;
                case TileEffect::Slippery:
                    renderTileDebugHighlight(wc, pos, mapSlipperyTile);
                    break;
                case TileEffect::SlowDown:
                    renderTileDebugHighlight(wc, pos, sf::Color::Black);
                    break;
                case TileEffect::TallGrass:
                    renderTileDebugHighlight(wc, pos, sf::Color::Green);
                    break;
                default:
                    continue;
            }
        }
    }
}

void Map::renderTileDebugHighlight(WindowContainer& wc, const sf::Vector2i& pos, sf::Color color) const {
    wc.triangleArray.resize(wc.triangleArrayIndex + 6);

    int j = pos.x;
    int i = pos.y;

    wc.triangleArray[wc.triangleArrayIndex].position = sf::Vector2f{(float) j, (float) i} * TILE_SIZE;
    wc.triangleArray[wc.triangleArrayIndex].color = color;
    wc.triangleArrayIndex++;

    wc.triangleArray[wc.triangleArrayIndex].position = sf::Vector2f{(float) j, (float) i + 1} * TILE_SIZE;
    wc.triangleArray[wc.triangleArrayIndex].color = color;
    wc.triangleArrayIndex++;

    wc.triangleArray[wc.triangleArrayIndex].position = sf::Vector2f{(float) j + 1, (float) i + 1} * TILE_SIZE;
    wc.triangleArray[wc.triangleArrayIndex].color = color;
    wc.triangleArrayIndex++;

    wc.triangleArray[wc.triangleArrayIndex].position = sf::Vector2f{(float) j, (float) i} * TILE_SIZE;
    wc.triangleArray[wc.triangleArrayIndex].color = color;
    wc.triangleArrayIndex++;

    wc.triangleArray[wc.triangleArrayIndex].position = sf::Vector2f{(float) j + 1, (float) i} * TILE_SIZE;
    wc.triangleArray[wc.triangleArrayIndex].color = color;
    wc.triangleArrayIndex++;

    wc.triangleArray[wc.triangleArrayIndex].position = sf::Vector2f{(float) j + 1, (float) i + 1} * TILE_SIZE;
    wc.triangleArray[wc.triangleArrayIndex].color = color;
    wc.triangleArrayIndex++;
}

void Map::renderCollisionTileHighlights(WindowContainer& wc) const {
    for (int i = 0; i < mapSize.y; i++) {
        for (int j = 0; j < mapSize.x; j++) {
            if (getTile({j, i})->isTraversable == true) continue;

            wc.triangleArray.resize(wc.triangleArrayIndex + 6);

            wc.triangleArray[wc.triangleArrayIndex].position = sf::Vector2f{(float) j, (float) i} * TILE_SIZE;
            wc.triangleArray[wc.triangleArrayIndex].color = mapCollisionTile;
            wc.triangleArrayIndex++;

            wc.triangleArray[wc.triangleArrayIndex].position = sf::Vector2f{(float) j, (float) i + 1} * TILE_SIZE;
            wc.triangleArray[wc.triangleArrayIndex].color = mapCollisionTile;
            wc.triangleArrayIndex++;

            wc.triangleArray[wc.triangleArrayIndex].position = sf::Vector2f{(float) j + 1, (float) i + 1} * TILE_SIZE;
            wc.triangleArray[wc.triangleArrayIndex].color = mapCollisionTile;
            wc.triangleArrayIndex++;

            wc.triangleArray[wc.triangleArrayIndex].position = sf::Vector2f{(float) j, (float) i} * TILE_SIZE;
            wc.triangleArray[wc.triangleArrayIndex].color = mapCollisionTile;
            wc.triangleArrayIndex++;

            wc.triangleArray[wc.triangleArrayIndex].position = sf::Vector2f{(float) j + 1, (float) i} * TILE_SIZE;
            wc.triangleArray[wc.triangleArrayIndex].color = mapCollisionTile;
            wc.triangleArrayIndex++;

            wc.triangleArray[wc.triangleArrayIndex].position = sf::Vector2f{(float) j + 1, (float) i + 1} * TILE_SIZE;
            wc.triangleArray[wc.triangleArrayIndex].color = mapCollisionTile;
            wc.triangleArrayIndex++;
        }
    }
}

void Map::renderTransitionTileHighlights(WindowContainer& wc) const {
    for (int i = 0; i < mapSize.y; i++) {
        for (int j = 0; j < mapSize.x; j++) {
            if (getTile({j, i})->isTransition == false) continue;

            wc.triangleArray.resize(wc.triangleArrayIndex + 6);

            wc.triangleArray[wc.triangleArrayIndex].position = sf::Vector2f{(float) j, (float) i} * TILE_SIZE;
            wc.triangleArray[wc.triangleArrayIndex].color = mapTransitionTile;
            wc.triangleArrayIndex++;

            wc.triangleArray[wc.triangleArrayIndex].position = sf::Vector2f{(float) j, (float) i + 1} * TILE_SIZE;
            wc.triangleArray[wc.triangleArrayIndex].color = mapTransitionTile;
            wc.triangleArrayIndex++;

            wc.triangleArray[wc.triangleArrayIndex].position = sf::Vector2f{(float) j + 1, (float) i + 1} * TILE_SIZE;
            wc.triangleArray[wc.triangleArrayIndex].color = mapTransitionTile;
            wc.triangleArrayIndex++;

            wc.triangleArray[wc.triangleArrayIndex].position = sf::Vector2f{(float) j, (float) i} * TILE_SIZE;
            wc.triangleArray[wc.triangleArrayIndex].color = mapTransitionTile;
            wc.triangleArrayIndex++;

            wc.triangleArray[wc.triangleArrayIndex].position = sf::Vector2f{(float) j + 1, (float) i} * TILE_SIZE;
            wc.triangleArray[wc.triangleArrayIndex].color = mapTransitionTile;
            wc.triangleArrayIndex++;

            wc.triangleArray[wc.triangleArrayIndex].position = sf::Vector2f{(float) j + 1, (float) i + 1} * TILE_SIZE;
            wc.triangleArray[wc.triangleArrayIndex].color = mapTransitionTile;
            wc.triangleArrayIndex++;
        }
    }
}

void Map::renderTileEffectHighlights(WindowContainer& wc) const {
    (void) wc;

}

bool Map::load(int environment, int entrance) {
    std::cout << "../data/map_data/" + std::to_string(environment) + ".data\n";
    std::ifstream infile("../data/map_data/" + std::to_string(environment) + ".data");
    if(infile.fail()) {
        return false;
    }

    json data = json::parse(infile);
    mapSize.x = data["width"];
    mapSize.y = data["height"];
    std::string backgroundName = data["background"];
    std::string tilesetName = data["tileset"];

    tileMap = new Tile[mapSize.x * mapSize.y];

    for (int i = 0; i < mapSize.y; i++) {
        for (int j = 0; j < mapSize.x; j++) {
            Tile *curr = getTile({j, i});
            curr->tileId = data["map_tiles"][i][j];
            curr->isTraversable = (data["collision_tiles"][i][j] != 0);
            curr->effect = data["tile_effect"][i][j];
        }
    }

    entranceLocation = sf::Vector2i(data["entrance_tiles"][entrance]["x"], data["entrance_tiles"][entrance]["y"]);
    for (auto it = data["exit_tiles"].begin(); it != data["exit_tiles"].end(); it++) {
        int x = (*it)["x"];
        int y = (*it)["y"];
        
        Tile *currTile = getTile({x, y});
        currTile->isTransition = true;
        currTile->transitionId = (*it)["transitionId"];
        currTile->entranceId = (*it)["entranceId"];
    }

    entities = data["entities"];
    //initialize sfml rendering assets
    backgroundTexture = textureManager.getTexture(backgroundName);
    mapTileset = textureManager.getTexture(tilesetName);

    mapVertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    mapVertices.resize(mapSize.y * mapSize.x * 6);

    backgroundTexture.setRepeated(true);
    backgroundVertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    backgroundVertices.resize(6);

    updateTilemap();
    // updateBackground();

    return true;
}

bool Map::createMap(const std::string& input) {
    json data = json::parse(input);
    mapSize.x = data["width"];
    mapSize.y = data["height"];
    std::string tilesetName = data["tileset"];
    std::string backgroundName = data["background"];

    tileMap = new Tile[mapSize.x * mapSize.y];

    for (int i = 0; i < mapSize.y; i++) {
        for (int j = 0; j < mapSize.x; j++) {
            getTile({j, i})->tileId = data["map_tiles"][i][j];
            getTile({j, i})->isTraversable = (data["collision_tiles"][i][j] != 0);
        }
    }

    //initialize sfml rendering assets
    backgroundTexture = textureManager.getTexture(backgroundName);
    mapTileset = textureManager.getTexture(tilesetName);

    mapVertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    mapVertices.resize(mapSize.y * mapSize.x * 6);

    backgroundTexture.setRepeated(true);
    backgroundVertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    backgroundVertices.resize(6);

    updateTilemap();
    // updateBackground();

    return true;
}

void Map::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();
    states.texture = &mapTileset;

    sf::RenderStates backgroundStates = states;
    backgroundStates.texture = &backgroundTexture;

    target.draw(backgroundVertices, backgroundStates);
    target.draw(mapVertices, states);
}
