#pragma once

#include "GameObject.h"
#include "Mesh.h"

class Object : public GameObject{
    Mesh* mesh;
public:
    Object(){
        mesh = AddComponent<Mesh>();
    }
};