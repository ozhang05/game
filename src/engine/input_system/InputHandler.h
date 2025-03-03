#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "ApplicationState.h"
#include "WindowContainer.h"
#include "MouseEvent.h"

#include "SFML/Window/Event.hpp"

class Entity;
class EntityManager;
class MenuManager;

class InputHandler {
    public:
        // InputManager(InputMode i);
        InputHandler(ApplicationState& as, EntityManager& em, MenuManager& mm, WindowContainer& wc);
        ~InputHandler();

        bool setMover();
        void processInput(const std::optional<sf::Event>& e);

        const MouseEvent& getMouseEvent();

    private:
        ApplicationState& applicationState;
        EntityManager& entityManager;
        MenuManager& menuManager;
        WindowContainer& windowContainer;
        bool unloadedMap; // HACK: get rid of this, was used to make sure didn't unload menus immediately

        Entity *mover;          // mover entity (player or empty entity)
        MouseEvent mouseEvent;

        // important necessary functions
        void updateMouseEventLocation(const std::optional<sf::Event>& e);
        void changeState(GameState::Control control);

        // game state specific inputs
        void processGameplayInputs(const std::optional<sf::Event>& e);
        void processGameplayKeyPressed(const sf::Event::KeyPressed& e);
        void processGameplayKeyReleased(const sf::Event::KeyReleased& e);

        void processMenuInputs(const std::optional<sf::Event>& e);
        void processMenuKeyPressed(const sf::Event::KeyPressed& e);
        void processMenuMousePressed(const sf::Event::MouseButtonPressed& e);
        void processMenuMouseReleased(const sf::Event::MouseButtonReleased& e);
        void processMenuMouseMoved(const sf::Event::MouseMoved& e);

        void processTextboxInputs(const std::optional<sf::Event>& e);

        // application type specific inputs
        void gameplaySpecificInputs(const std::optional<sf::Event>& e);
        void mapmakerSpecificInputs(const std::optional<sf::Event>& e);

        void mapmakerSpecificKeyPressed(const sf::Event::KeyPressed& e);
        void mapmakerSpecificMousePressed(const sf::Event::MouseButtonPressed& e);
        void mapmakerSpecificMouseReleased(const sf::Event::MouseButtonReleased& e);
        void mapmakerSpecificMouseMoved(const sf::Event::MouseMoved& e);

};

#endif /* INPUTHANDLER_H */
