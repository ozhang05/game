#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

// game window constants
const float TILE_SIZE = 32.f;
const int DEFAULT_WINDOW_WIDTH = 1600;
const int DEFAULT_WINDOW_HEIGHT = 900;
const std::string DEFAULT_WINDOW_NAME = "hi";

// game core constants
const int TICKS_PER_SECOND = 60;
const double TICK_TIME = 1.f / TICKS_PER_SECOND;

// game setting constants
const float ZOOM_INCREMENT = 0.1;
const float MAX_ZOOM = 1.5;
const float MIN_ZOOM = 0.5;
const int FPS_CAP = 120;
// const int FPS_CAP = INT_MAX;
const int FPS_MAX_SAMPLES = 500;

//transition between maps
const float TRANSITION_SPEED = 300.f;

//player constants
const float PLAYER_MAX_SPEED = 6.f;
const float PLAYER_ACCELERATION = 0.5;
const float PLAYER_WIDTH = 10.f;
const float PLAYER_HEIGHT = 21.f;

//mapmaker
const float MAPMAKER_TILESET_RATIO = 0.4;

#endif /* CONSTANTS_H */
