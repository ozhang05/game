#ifndef COMPONENTFACTORY_H
#define COMPONENTFACTORY_H

#include <string>

#include <nlohmann/json.hpp>

class Component;
class TextureManager;

class ComponentFactory {
    public:
        ComponentFactory(TextureManager& textureManager);

        // Factory method to create components based on the component name
        Component *createComponent(const std::string& componentName, const nlohmann::json& data);
    private:
        TextureManager& textureManager;
};

#endif // COMPONENTFACTORY_H
