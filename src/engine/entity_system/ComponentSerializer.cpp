#include "ComponentSerializer.h"

#include <iostream>

#include "Component.h"

ComponentSerializer::ComponentSerializer() {
    componentOutline["PositionComponent"] = { {"x", "int"}, {"y", "int"} };
    componentOutline["MovementComponent"] = {};
    componentOutline["PlayerMovementComponent"] = {};
    componentOutline["PatrolMovementComponent"] = {};
    componentOutline["AIMovementComponent"] = {};
    componentOutline["CollisionComponent"] = {
        {"centerX", "float"},
        {"centerY", "float"},
        {"width", "float"},
        {"height", "float"}
    };
    componentOutline["RenderableComponent"] = {"texture", "string"};
}

json ComponentSerializer::encodeComponent(Component *component, const std::string& componentName) {
    // TODO: change this function to work with the new componentOutline frameowrk
    // if (componentOutline.find(componentName) == componentOutline.end()) {
    //     std::cerr << "COMPONENT OUTLINE NOT FOUND COMPONENTSERIALIZER.CPP\n";
    //     return {};
    // }
    //
    // json outline = componentOutline[componentName];
    json result;

    // for (auto& [fieldName, fieldType] : outline.items()) {
    //     // result[fieldName] = component->
    //     (void) fieldName;
    //     (void) fieldType;
    // }
    
    // return result;
    if (componentName == "PositionComponent") {
        PositionComponent *pos = (PositionComponent *) component;
        result["x"] = pos->getX();
        result["y"] = pos->getY();
    } else if (componentName == "MovementComponent") {
        exit(EXIT_FAILURE);
    } else if (componentName == "PlayerMovementComponent") {
        exit(EXIT_FAILURE);
    } else if (componentName == "PatrolMovementComponent") {
        exit(EXIT_FAILURE);
    } else if (componentName == "AIMovementComponent") {
        exit(EXIT_FAILURE);
    } else if (componentName == "CollisionComponent") {
        CollisionComponent *col = (CollisionComponent *) component;
        result["centerX"] = col->getCenter().x;
        result["centerY"] = col->getCenter().y;
        result["width"] = col->getSize().x;
        result["height"] = col->getSize().y;
    } else if (componentName == "RenderableComponent") {
        RenderableComponent *ren = (RenderableComponent *) component;
        result["texture"] = ren->getImageName();
    } else if (componentName == "newone") {
        
    } else if (componentName == "newtwo") {
        
    } else {
        std::cerr << "ERROR: CREATING INVALID COMPONENT COMPONENTFACTORY.H";
        exit(EXIT_FAILURE);
    }

    return result;
}

json ComponentSerializer::getComponentOutline(const std::string& name) {
    if (componentOutline.find(name) == componentOutline.end()) {
        std::cerr << "ERROR COMPONENT NOT FOUND COMPONENTSERIALIZER.CPP\n";
        exit(EXIT_FAILURE);
    }
    return componentOutline[name];
}
