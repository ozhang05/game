#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <unordered_map>

class Component;

class Entity {
    public:
        Entity(std::string newName = "default");
        ~Entity();

        void addComponent(Component *c);
        bool hasComponent(std::string key);
        Component *getComponent(const std::string& key);
        
        //entity generation methods
        std::string getName();
        int getId() const;

    private:
        int id;
        static int nextId;
        std::string name;
        std::unordered_map<std::string, Component *> components;
};

#endif /* ENTITY_H */
