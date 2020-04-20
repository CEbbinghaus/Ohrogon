#pragma once

#include "Game.h"
#include "GameObject.h"


Game* Game::instance = nullptr;

Game::Game(GLFWwindow* w) : window(w) {
    Console::Log(String::format("Constructing Game. Adress: %X", (uint64_t)this));
    managers = Array<ComponentManager*>();
    objects = Array<GameObject*>();
    glfwMakeContextCurrent(window);
    Keyboard::RegisterKeyboard(window);
    Mouse::RegisterMouse(window);
}

void Game::AddObject(GameObject* object) {
    if (instance == nullptr) return;

    Console::Log("Adding Game Object to Game");
    instance->objects.push(object);

    return;
}

Game* Game::CreateInstance(GLFWwindow* gl_Window) {
    if (instance != nullptr) return instance;
    instance = new Game(gl_Window);
    return instance;
}

Game* Game::GetInstance() {
    if (instance == nullptr) throw "No Instance was Created";
    return instance;
}

void Game::DestroyInstance() {
    if (instance != nullptr)
        delete instance;
    instance = nullptr;
}

void Game::Start() {
    if (instance == nullptr) return;
    instance->active = true;
    instance->runWhile();
}

void Game::Quit() {
    if (instance == nullptr) return;
    instance->active = false;
}

void Game::runWhile() {
    while (glfwWindowShouldClose(window) == false && active) {
        loop();
    }
}

void Game::loop() {
    BeforeUpdate();
    Update();
    AfterUpdate();
}

void Game::BeforeUpdate() {
}

void Game::AfterUpdate() {
}

void Game::Update() {
}

Game::~Game(){
    Console::Log("Deconstructing Game");
    Console::Log(String::format("Deconstructing %i GameObjects", objects.length));
    for (GameObject* object : objects) {
        Console::Log(String::format("Calling delete. adress of object: %X", (uint64_t)object));
        delete object;//->~GameObject();
    }
    objects.clear();


    Console::Log(String::format("Deconstructing %i Managers", objects.length));
    for(ComponentManager* manager : managers){
        manager->Destroy();
        delete manager;
    }
    managers.clear();
}