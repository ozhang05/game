#ifndef PATHFINDING_H 
#define PATHFINDING_H 

#include <cstddef>
#include <functional>

#include "TileEffect.h"

#include <SFML/System/Vector2.hpp>

class Map;
class Entity;
class PositionComponent;



struct Node {
    sf::Vector2i pos;
    float gCost, hCost;
    Node *parent;
    bool operator>(const Node& other) const {
        return (gCost + hCost) > (other.gCost + other.hCost);
    }
};

struct IntVectorHash {
    std::size_t operator()(const sf::Vector2i& v) const {
        std::size_t xHash = std::hash<int>()(v.x);
        std::size_t yHash = std::hash<int>()(v.y);
        return xHash ^ (yHash << 1);
    }
};

struct greater {
    bool operator()(const Node *a, const Node *b) const {
        return *a > *b;
    }
};

class PathfindingSystem {
public:
    PathfindingSystem(const Map& mapLoc);

    // main path finding functions
    std::vector<sf::Vector2i> *findPath(const sf::Vector2f& start, const sf::Vector2f& end);
    std::vector<sf::Vector2i> *getTilepath(Node *n);

    // helper functions
    float getTileCost(const sf::Vector2i& mapLoc);
    float heuristicFunc(const sf::Vector2f& start, const sf::Vector2f& end);
    float heuristicFunc(const sf::Vector2i& start, const sf::Vector2i& end);
private:
    const Map& map;
};

#endif // PATHFINDING_H
