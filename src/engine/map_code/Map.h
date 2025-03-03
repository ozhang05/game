#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
struct Tile;
class TextureManager;
struct WindowContainer;

class Map : public sf::Drawable, public sf::Transformable {
    public:
        std::string name;
        // exit is where in the map you will spawn in
        Map();
        Map(int environment, int entrance, TextureManager& tm);
        Map(const std::string& inputJson, TextureManager& tm);
        ~Map();

        // updating map
        void update(int environment, int entrance);
        void update(const std::string& inputJson);
        void setFirstEntityId(int newId);

        // checking player movement
        bool collides(const sf::FloatRect& collisionBox) const;
        Tile *isMapTransition(const sf::FloatRect& collisionBox) const;

        // map getting functions
        Tile *getTile(sf::Vector2i pos) const;
        const sf::Vector2i& getMapSize() const;
        const sf::Vector2i& getEntranceLocation() const;
        const json& getAllEntities() const;
        sf::Vector2i getEntityUnloadingInformation() const; 

        // map visual updating function
        void updateTilemap();
        void updateTile(int x, int y);
        void updateBackground(const sf::Vector2f& center, const sf::Vector2f& size);

        // test function
        void renderDebugHighlights(WindowContainer& wc) const;
        void renderTileDebugHighlight(WindowContainer& wc, const sf::Vector2i& pos, sf::Color color) const;
    // TODO: remove below funcs
        void renderCollisionTileHighlights(WindowContainer& wc) const;
        void renderTransitionTileHighlights(WindowContainer& wc) const;
        void renderTileEffectHighlights(WindowContainer& wc) const;
        
    private:
        int mapId = 0;

        //mapHeight and mapX are the max size of the map
        //entrance location is the location of the entrance the player arrived at
        sf::Vector2i mapSize;
        sf::Vector2i entranceLocation;

        //tileMap holds the map as ints corresponding to tiles
        Tile *tileMap;

        //sf data structures used to render
        sf::VertexArray mapVertices;
        sf::Texture mapTileset;
        sf::VertexArray backgroundVertices;
        sf::Texture backgroundTexture;

        //used while initializing
        json entities;                      // passed to entity manager
        int firstEntityId;                  // used to unload entities
        TextureManager& textureManager;     // used to initialize map textures

        //load loads the map using the given environment
        //readTiles loads a set of tiles for the given location and end
        bool load(int environment, int entrance);
        bool createMap(const std::string& data);

        //inherited render function
        void draw(sf::RenderTarget &target, sf::RenderStates states) const;     
};

#endif /* MAP_H */
