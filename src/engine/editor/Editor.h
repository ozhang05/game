#ifndef GAME_H
#define GAME_H

#include <string>

#include "Application.h"
#include "Entity.h"

#include <SFML/Graphics.hpp>

class Map;

class Editor : public Application {
    public:
        Editor();
        
    private:
        // map variables used to write to file
        int mapId;
        int mapWidth, mapHeight;
        //this is by tiles
        int tilesetWidth, tilesetHeight;
        std::string tilesetName;
        std::string backgroundName;

        // functionality variables
        int selectedTile;               // tile selected to be placed in map
        sf::Vector2u previousTile;      // previously selected tile

        Entity mover = Entity("mover");
        
        sf::View tileDisplayView;
        sf::Texture tileDisplayTexture;
        sf::Sprite tileDisplay;
        sf::Clock deltaTime;

        //sfml variables
        void renderScene();
        void displayTileset();
        
        //editor functions
        void handleMouseInput();
        void handleTileLeftClick(const sf::Vector2f& loc);
        void handleTileMiddleClick(const sf::Vector2f& loc);
        void handleTileRightClick(const sf::Vector2f& loc);

        // editor helper functions
        bool inTilemap(const sf::Vector2f& loc);

        //debug functions

        //game initialization functions
        bool startMenu();
        void initializeEditor();
        void run();

        //save data functions
        bool writeToFile(const std::string& fileName);
};

#endif /* GAME_H */
