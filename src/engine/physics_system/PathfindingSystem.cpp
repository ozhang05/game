#include "PathfindingSystem.h"

#include <vector>
#include <queue>
#include <unordered_set>
#include <iostream>

#include "Map.h"
#include "Tile.h"

std::vector<sf::Vector2i> neighbors = {
    {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    // {1, 1}, {-1, -1}, {1, -1}, {-1, 1}
};

/* ************************************************************************** */

PathfindingSystem::PathfindingSystem(const Map& mapLoc)
    : map(mapLoc) {

}

/* ************************************************************************** */

std::vector<sf::Vector2i> *PathfindingSystem::findPath(const sf::Vector2f& start, const sf::Vector2f& end) {
    std::priority_queue<Node *, std::vector<Node *>, greater> openList;
    std::unordered_set<sf::Vector2i, IntVectorHash> closedList;

    std::unordered_map<sf::Vector2i, Node *, IntVectorHash> openListCosts;

    sf::Vector2i initial;
    initial.x = std::round(start.x);
    initial.y = std::round(start.y);
    sf::Vector2i goal;
    goal.x = std::round(end.x);
    goal.y = std::round(end.y);

    Node *init = new Node;
    init->pos = initial;
    init->gCost = 0;
    init->hCost = heuristicFunc(start, end);
    init->parent = nullptr;
    openList.emplace(init);

    while (!openList.empty()) {
        Node *current = openList.top();
        openList.pop();

        if (current->pos == goal) {
            // path complete, do something???
            // std::cout << " tilepath for start and end " << start.x << " " << start.y << ", end: " << end.x << " " << end.y << "\n";
            std::cout << "generated path\n";
            return getTilepath(current);
        }

        closedList.emplace(current->pos);

        for (sf::Vector2i neighborTransform : neighbors) {
            sf::Vector2i neighborPos = current->pos + neighborTransform;

            // check to see if it has been seen
            if (closedList.count(neighborPos) == 1) continue;

            // float tentativeCost = current->gCost + getTileCost(neighborPos) + neighborTransform.lengthSquared();
            float tentativeCost = current->gCost + getTileCost(neighborPos) + 1;
            // TODO: account for map tile costs, and get rid of below
            Tile *neighborTile = map.getTile(neighborPos);
            if (neighborTile == nullptr) continue;
            if (!neighborTile->isTraversable) continue;
            

            Node *neighbor = new Node;
            neighbor->pos = neighborPos;
            neighbor->gCost = tentativeCost;
            neighbor->hCost = heuristicFunc(neighborPos, goal);
            neighbor->parent = current;
            // TODO: maybe need to add a check for complete a*

            if (openListCosts.find(neighborPos) != openListCosts.end()) {
                Node *old = openListCosts.at(neighborPos);

                if (old->gCost > tentativeCost) old->gCost = tentativeCost;
            } else {
                openListCosts[neighborPos] = neighbor;
                openList.emplace(neighbor);
            }
        }
    }
    // do something error
    return nullptr;
}

std::vector<sf::Vector2i> *PathfindingSystem::getTilepath(Node *n) {
    std::vector<sf::Vector2i> *returnPath = new std::vector<sf::Vector2i>;

    Node *curr = n;
    Node *temp = n;

    while (curr != nullptr) {
        returnPath->emplace_back(curr->pos);

        curr = curr->parent;

        delete temp;
        temp = curr;
    }

    if (returnPath->empty()) return nullptr;
    std::reverse(returnPath->begin(), returnPath->end());
    return returnPath;
}

/* ************************************************************************** */

float PathfindingSystem::getTileCost(const sf::Vector2i& mapLoc) {

    (void) mapLoc;
    return 1.f;
}

float PathfindingSystem::heuristicFunc(const sf::Vector2f& start, const sf::Vector2f& end) {
    sf::Vector2f temp = start - end;
    float dist = (temp).lengthSquared();

    return dist;
}

float PathfindingSystem::heuristicFunc(const sf::Vector2i& start, const sf::Vector2i& end) {
    sf::Vector2i temp = start - end;
    float dist = (temp).lengthSquared();
   return dist;
}
