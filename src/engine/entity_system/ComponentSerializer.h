#ifndef COMPONENTSERIALIZER_H
#define COMPONENTSERIALIZER_H

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Component;

class ComponentSerializer {
    public:
        ComponentSerializer();

        // Factory method to create components based on the component name
        json encodeComponent(Component *component, const std::string& name);

        // method to get required data types to initialize component based off name
        json getComponentOutline(const std::string& name);
        
        // std::vector<std::string> 
    private:
        std::unordered_map<std::string, json> componentOutline;
};

#endif /* COMPONENTSERIALIZER_H */
