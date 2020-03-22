#pragma once

#include "Symbol.h"
#include "Game.h"
#include "String.h"

class Component{
    friend Game;

public:

    Component(){}

    virtual void Setup(){}
    
    virtual void Execute(){}

    virtual Symbol _IDENTIFIER(){
        return Symbol("Component.Base");
    }
};