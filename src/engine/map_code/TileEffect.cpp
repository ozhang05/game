#include "TileEffect.h"

#include <iostream>

TileEffect intToTileEffect(int effectValue) {
    switch (effectValue) {
        case 0:
            return TileEffect::None;
        case 1:
            return TileEffect::Slippery;
        case 2:
            return TileEffect::SlowDown;
        case 3:
            return TileEffect::TallGrass;
        default:
            std::cerr << "UNRECOGNIZED TILE EFFECT TILEEFFECT.H\n";
            return TileEffect::None;  // Default to None if invalid value
    }
}

float tileEffectMovementModifier(TileEffect e) {
    switch(e) {
        case TileEffect::None:
            return 1.f;
        case TileEffect::Slippery:
            return 1.5f;
        case TileEffect::SlowDown:
            return 0.5f;
        case TileEffect::TallGrass:
            return 0.90f;
        default:
            return 1.f;
    }
}
