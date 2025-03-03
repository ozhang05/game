#ifndef TILE_EFFECT_H
#define TILE_EFFECT_H

enum class TileEffect {
    None,           // No effect
    Slippery,       // Makes the player slip
    SlowDown,       // Slows down the player
    TallGrass       // Actual game coding begins here!
};

TileEffect intToTileEffect(int effectValue);

float tileEffectMovementModifier(TileEffect e);

#endif // TILE_EFFECT_H
