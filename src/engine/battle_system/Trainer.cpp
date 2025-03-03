#include "Trainer.h"

#include <iostream>

#include "Pokemon.h"

Trainer::Trainer(int _id, std::string _name, TextureManager& tm)
    : id(_id),
        name(_name) {

    // TODO: remove this when we properly create trainers
    std::cout << "trainer init\n";
    active = new Pokemon(id, tm);
}

Trainer::~Trainer() {
    delete active;
}

/* ************************************************************************** */

int Trainer::getPokemonCount() {
    return pokemon.size();
}

std::string Trainer::getActivePokemonTexture() {
    return active->getTextureName();
}

sf::Sprite& Trainer::getActivePokemonSprite() {
    return active->getSprite();
}
