#ifndef BATTLEMENU_H
#define BATTLEMENU_H

#include "Menu.h"

class BattleMenu : public Menu {
public:
    BattleMenu(const sf::Font& givenFont);
    ~BattleMenu();

    void render(sf::RenderWindow& window) override;
    void handleEvent(MenuCommand command) override;

    void onEnter() override;
    void onExit() override;

private:
};

#endif /* BATTLEMENU_H */
