#ifndef STARTMENU_H
#define STARTMENU_H

#include "Menu.h"

struct ApplicationState;

class StartMenu : public Menu {
    public:
        StartMenu(ApplicationState& appState, const sf::Font& givenFont);
        ~StartMenu();

        void render(sf::RenderWindow& window) override;
        void handleEvent(MenuCommand menuCommand) override;

        void onEnter() override;
        void onExit() override;

    private:
        int selectedIndex;

        ApplicationState& applicationState;

        void debug();
};

#endif /* STARTMENU_H */
