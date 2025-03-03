#ifndef POKEMON_H
#define POKEMON_H

#include <string>

#include <SFML/Graphics/Sprite.hpp>

class TextureManager;

class Pokemon {
public:
    Pokemon(int id, TextureManager& tm);

    const std::string& getTextureName() const;
    sf::Sprite& getSprite();

private:
    // Unit *units;
    int id;
    std::string textureName;

    sf::Sprite sprite;

};

#endif // POKEMON_H
