#pragma once

#include "Symbol.h"
#include "String.h"

class Component;
class GameObject;
class Game;

class ComponentManager{
    friend Game;
    friend Component;
  protected:
    ComponentManager(){}
    virtual ~ComponentManager(){}
  public:
    virtual void Setup() = 0;
    virtual void Register(Component*) = 0;
    virtual void DeRegister(Component*) = 0;
    virtual void Execute() = 0;
    virtual void Destroy() = 0;
};

class Component{
    friend Game;
    friend GameObject;

    void DeRegister() {
        ComponentManager* manager = GetManager();
        if (manager != nullptr) {
            manager->DeRegister(this);
        }
    }
    
protected:

    GameObject* gameObject;
    Component(GameObject* go): gameObject(go){

    }

    virtual ~Component(){}

    virtual ComponentManager* GetManager() {
        return nullptr;
    }
public:
    virtual void Setup(){}

    virtual void Execute(){}

    static Symbol Type(){
        return Symbol("Component.Base");
    }

    virtual Symbol InstanceType(){
        return Symbol("Component.Base");
    }
};