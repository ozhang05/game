#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include "Menu.h"

struct ApplicationState;

class PauseMenu : public Menu {
    public:
        PauseMenu(ApplicationState& appState, const sf::Font& givenFont);
        ~PauseMenu();

        void render(sf::RenderWindow& window) override;
        void handleEvent(MenuCommand command) override;

        void onEnter() override;
        void onExit() override;

    private:

        ApplicationState& applicationState;

        void resumeGame();
        void zoomIn();
        void zoomOut();
        void debugToggle();
        void toggleBattle();
        void quitGame();
};

#endif /* PAUSEMENU_H */
