#include "Pokemon.h"

#include <iostream>

#include "TextureManager.h"

std::string idToTextureName[] = {
    "Player",
    "tree1"
};

Pokemon::Pokemon(int _id, TextureManager& tm)
    : id(_id),
        textureName(idToTextureName[id]),
        sprite(tm.getTexture(textureName)) {

    std::cout << "texturename " << textureName << "\n";

}

const std::string& Pokemon::getTextureName() const {
    return textureName;
}

sf::Sprite& Pokemon::getSprite() {
    return sprite;
}
