#ifndef TILE_H
#define TILE_H

#include "TileEffect.h"

struct Tile {
    int tileId;
    bool isTraversable;

    //contains information about what map this exit leads to
    bool isTransition = false;
    int transitionId = -1;
    int entranceId = -1;

    TileEffect effect;
};

#endif /* TILE_H */