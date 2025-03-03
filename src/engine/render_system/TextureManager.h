#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <string>
#include <unordered_map>

#include <SFML/Graphics.hpp>

class TextureManager {
    public:
        TextureManager();
        ~TextureManager();

        //Texture functions
        const sf::Texture& getTexture(const std::string& filename);
        void unloadTexture(const std::string& filename);

        //Force unloads all textures
        void clear();

    private:
        struct Container {
            int count;
            sf::Texture texture;
        };
        std::unordered_map<std::string, Container> textures;

        bool textureExists(int id);    
};

#endif /* TEXTUREMANAGER_H */