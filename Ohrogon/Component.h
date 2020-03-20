#pragma once

#include "Symbol.h"
#include "Game.h"
#include "String.h"

class Component{
    friend Game;
    Symbol symbol = Symbol("Component.Base");;

public:

    Component(){}

    virtual void Setup(){}
    
    virtual void Execute(){}
};