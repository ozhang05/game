#ifndef TRAINER_H
#define TRAINER_H

#include <string>
#include <vector>

#include <SFML/Graphics/Sprite.hpp>

class Pokemon;
class TextureManager;

class Trainer {
public:
    Trainer(int _id, std::string _name, TextureManager& tm);
    ~Trainer();

    // relating to pokemon
    int getPokemonCount();
    std::string getActivePokemonTexture();
    sf::Sprite& getActivePokemonSprite();

private:
    int id;
    std::string name;
    Pokemon *active;

    std::vector<Pokemon *> pokemon;


};

#endif // TRAINER_H
