#pragma once

#include <functional>
#include <atyp_String.h>

class Symbol{
    static std::hash<std::string> hash;
    String identifier;

public:
    Symbol(const String& name): identifier(name){}

    bool operator ==(const Symbol& other){
        return hash(std::string(identifier)) == hash(std::string(other.identifier));
    }
};