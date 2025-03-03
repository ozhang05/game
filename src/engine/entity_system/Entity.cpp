#include "Entity.h"

#include <string>
#include <iostream>
#include <stdlib.h>

#include "Component.h"

int Entity::nextId = 0;

Entity::Entity(std::string newName) :
    id(nextId++),
    name(newName) {
        std::cout << "Initializing entity with name: " << newName << ", id: " << id << "\n";
}

Entity::~Entity() {
    for (std::string s: ComponentNames) {
        if (hasComponent(s)) {
            delete getComponent(s);
            std::cout << "Deleting Component " << s << "\n";
        }
    }
}

void Entity::addComponent(Component *c) {
    std::cout << "adding component " << c->getName() << " " << id << "\n";
    components.insert({c->getName(), c});
}

bool Entity::hasComponent(std::string key) {
    if (components.find(key) != components.end()) {
        return true;
    }
    return false;
}

Component *Entity::getComponent(const std::string& key) {
    if (!hasComponent(key)) {
        // std::cerr << "ERROR DID NOT CHECK COMPONENT HAVING KEY " << key << ", ENTITY.CPP\n";
        return nullptr;
    }
    return components.at(key);
}

std::string Entity::getName() {
    return name;
}

int Entity::getId() const {
    return id;
}
