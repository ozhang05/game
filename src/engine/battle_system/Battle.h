#ifndef BATTLE_H
#define BATTLE_H

class Trainer;
class TextureManager;
struct WindowContainer;

class Battle {
public:
    Battle(TextureManager& tm);
    ~Battle();

    void render(WindowContainer& wc);

private:
    Trainer *first, *second;

    TextureManager& textureManager;

    void renderTrainer(WindowContainer& wc, Trainer& t);
    void renderUI();
};

#endif // BATTLE_H
