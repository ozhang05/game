#include "TextureManager.h"

#include <unordered_map>
#include <iostream>

TextureManager::TextureManager() {

}
TextureManager::~TextureManager() {

}

        //Texture functions
const sf::Texture& TextureManager::getTexture(const std::string& filename) {
    auto it = textures.find(filename);
    if (it != textures.end()) {
        it->second.count++;
        return it->second.texture;
    }

    std::cerr << "Loading " << filename << "\n"; 

    sf::Texture texture;

    if (!texture.loadFromFile("../assets/" + filename + ".png")) {
        std::cerr << "ERROR OPENING TEXTURE TEXTUREMANAGER.CPP\n";
    }

    Container temp = {1, texture};
    textures[filename] = temp;

    return textures[filename].texture; 
}

 
void TextureManager::unloadTexture(const std::string& filename) {
    auto it = textures.find(filename);
    if (it == textures.end()) {
        std::cerr << "ERROR UNLOADING TEXTURE THAT IS ALREADY UNLOADED\n";
        exit(EXIT_FAILURE);
    }

    it->second.count--;
    if (it->second.count == 0) {
        textures.erase(it);
    }
}

void TextureManager::clear() {

}
        
bool TextureManager::textureExists(int id) {
    (void) id;
    return true;
}