#pragma once

#include "Symbol.h"
#include "Game.h"
#include "String.h"

class ComponentManager;

class Component{
    friend Game;

public:


    virtual void Setup(){}
    
    virtual void Execute(){}

    virtual Symbol _IDENTIFIER(){
        return Symbol("Component.Base");
    }

    virtual ComponentManager* CreateManager(){
        return nullptr;
    }
};

class ComponentManager{
    friend Game;
    friend Component;

    ComponentManager(){}
public:
    virtual void Setup() = 0;
    virtual void Register(Component*) = 0;
    virtual void Execute() = 0;
    virtual void Destroy() = 0;
};