#pragma once
#include "Component.h"
#include "Mesh.h"

class MeshRenderer: public Component{
    friend GameObject;

    MeshRenderer(GameObject* gm): Component(gm){}
public:
    Mesh* mesh;

    static Symbol Type(){
        return Symbol("Component.MeshRenderer");
    }
    
    Symbol InstanceType() override{
        return Symbol("Component.MeshRenderer");
    }

    ComponentManager* GetManager(){
        return nullptr;
    }
};