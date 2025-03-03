#include "Battle.h"

#include <iostream>

#include "Trainer.h"
#include "TextureManager.h"
#include "WindowContainer.h"

Battle::Battle(TextureManager& tm)
    : textureManager(tm) {

    // TODO: remove these below
    first = new Trainer(0, "first", tm);
    second = new Trainer(1, "second", tm);

}

Battle::~Battle() {
    delete first;
    delete second;
}

/* ************************************************************************** */


void Battle::render(WindowContainer& wc) {
    renderTrainer(wc, *first);
    renderTrainer(wc, *second);

    renderUI();
}


/* ************************************************************************** */


void Battle::renderTrainer(WindowContainer& wc, Trainer& t) {
    (void) wc;

    std::string textureName = t.getActivePokemonTexture();

    sf::Sprite& pokemonSprite = t.getActivePokemonSprite();
    sf::Vector2f viewCenter = wc.view.getCenter();

    pokemonSprite.setPosition(viewCenter);
    (void) textureManager;
    wc.window.draw(pokemonSprite);

    // wc.window.draw(pokemonTexture);

}

void Battle::renderUI() {

}
