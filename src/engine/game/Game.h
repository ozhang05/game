#ifndef GAME_H
#define GAME_H

#include <string>

#include "Entity.h"
#include "Application.h"

#include <SFML/Graphics.hpp>

class PositionComponent;
class Entity;

class Game : public Application {
    public:
        Game();
        ~Game();
        
    private:
        Entity player = Entity("Player");

        //sfml variables
        sf::Clock deltaTime;
        
        //game functions
        void checkMapTransition();
        void transitionMap(int mapId, int entrance);
        void checkEncounter();
        void centerPlayerInTile(int x, int y);

        //player entity functions
        void updatePlayerPos(const PositionComponent& pos);

        //debug functions

        //game initialization functions
        bool startMenu();
        void initializePlayer();
        void run();

        //save data functions
        bool previousSave();
        void saveGame();
        void readSaveData();
        void resetSaveData();
};

#endif /* GAME_H */
