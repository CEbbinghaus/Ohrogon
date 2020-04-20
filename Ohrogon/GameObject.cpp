#pragma once
#include <atyp_Array.h>
#include <atyp_Transform.h>


#include "GameObject.h"

#include "Component.h"
#include "Console.h"

GameObject::GameObject() : gameObject(this) {
    Console::Log(String::format("Constructing GameObect. Adress: %X", (uint64_t)this));
    //components = Array<Component*>();
    Game::AddObject(this);
}

GameObject::GameObject(const GameObject& original) {
    printf("GameObject Copy Constructor\n");
}

GameObject::GameObject(const GameObject&& original) {
    printf("GameObject MoveConstructor Constructor\n");
}

GameObject& GameObject::operator=(const GameObject& other) {
    printf("GameObject Copy Assignment\n");
    return *this;
}

GameObject& GameObject::operator=(const GameObject&& other) {
    printf("GameObject Move Assignment\n");
    return *this;
}

GameObject::~GameObject() {
    Console::Error("Deconstructing GameObject");
    // for(Component* component : components){
    //     component->DeRegister();
    //     delete component;
    // }
}

bool GameObject::IsActive() {
    return active;
}

void GameObject::SetActive(bool value) {
    active = value;
}

void GameObject::Awake() {}
void GameObject::Update() {}
void GameObject::Destroy() {}