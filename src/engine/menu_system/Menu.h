#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>

class Interactable;
class Frame;
struct MouseEvent;

enum class MenuCommand {
    NONE,
    CLOSE_MENU,
    NAVIGATE_UP,
    NAVIGATE_DOWN,
    NAVIGATE_LEFT,
    NAVIGATE_RIGHT,
    SELECT_OPTION
};

class Menu {
    public:
        Menu(const sf::Font& gf);
        virtual ~Menu();

        // visual related functions
        virtual void render(sf::RenderWindow& window) = 0;
        void updateMenuInteractableLocations(const sf::Vector2f& viewCenter,
            const sf::Vector2f& viewSize);
        void updateMenuFrameLocations(const sf::Vector2f& viewCenter,
            const sf::Vector2f& viewSize);

        void queueInteractableUpdate();
        void queueFrameUpdate();

        // menu usage related functions
        virtual void handleEvent(MenuCommand command) = 0;
        void handleMouseEvent(const MouseEvent& mouseEvent);
        
        virtual void onEnter() = 0;
        virtual void onExit() = 0;

        const std::string& getName();
        void setName(const std::string &name);
        Interactable *getInteractable() const;

        MenuCommand queryKeyInput(const sf::Event::KeyPressed& e);
        bool queryTextInput(const char32_t c);
    protected:
        // functionality members
        int selectedIndex;
        int currentMouseHover;
        std::vector<Interactable *> interactables;
        std::vector<Frame *> frames;
        std::unordered_map<sf::Keyboard::Scancode, MenuCommand> inputMap;
        std::string name;
        bool updateInteractableLocations;
        bool updateFrameLocations;

        // necessary for interactabes/frames
        const sf::Font& mainFont;

        //render members
        sf::FloatRect menuBox;          // holds position and size
        sf::RectangleShape background;  // solid or semi transparant background
};

#endif /* MENU_H */
