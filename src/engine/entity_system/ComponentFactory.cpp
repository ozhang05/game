#include "ComponentFactory.h"

#include <iostream>

#include "Component.h"
#include "TextureManager.h"

ComponentFactory::ComponentFactory(TextureManager& textureManager)
    : textureManager(textureManager) {

}

Component *ComponentFactory::createComponent(const std::string& componentName, const nlohmann::json& data) {
    if (componentName == "PositionComponent") {
        return new PositionComponent(data["x"], data["y"]);
    } else if (componentName == "MovementComponent") {
        return new MovementComponent(data["acceleration"], data["velocity"]);
    } else if (componentName == "PlayerMovementComponent") {
        // return new PlayerMovementComponent();
    } else if (componentName == "FollowingComponent") {
        return new FollowingComponent(data["followId"]);
    } else if (componentName == "PatrolMovementComponent") {
        return new PatrolMovementComponent({data["x"], data["y"]}, data["radius"]);
    } else if (componentName == "CollisionComponent") {
        // return new CollisionComponent(data["centerX"], data["centerY"], data["width"], data["height"]); // TODO: wefwef
        return new CollisionComponent(data["width"], data["height"]);
    } else if (componentName == "RenderableComponent") {
        std::string textureName = data["texture"];
        return new RenderableComponent(textureManager.getTexture(textureName), textureName, sf::Vector2f{data["posX"], data["posY"]}, sf::Vector2f{data["offsetX"], data["offsetY"]});
    } else if (componentName == "newone") {
        
    } else if (componentName == "newtwo") {
        
    } else {
        std::cerr << "ERROR: CREATING INVALID COMPONENT COMPONENTFACTORY.H\n";
    }
    // Add more component types here as necessary
    std::cerr << "Not recognized: " << componentName << "\n";
    std::cerr << "ERROR: CREATING INVALID COMPONENT COMPONENTFACTORY.H\n";
    return nullptr;  // Return nullptr if the component type is not recognized
}
