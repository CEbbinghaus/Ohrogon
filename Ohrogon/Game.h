#pragma once

#include <glfw3.h>
#include <gl_core_4_5.h>

#include <atyp_Array.h>
#include <type_traits>
#include "Component.h"

class ComponentManager;
class GameObject;

class Game{
    friend GameObject;
    
    static Game* instance;
    GLFWwindow* window;

    Array<GameObject*> objects;
    Array<ComponentManager*> managers;
    bool active = true;

    Game(GLFWwindow* w): window(w){
        managers = Array<ComponentManager*>();
        objects = Array<GameObject*>();
        glfwMakeContextCurrent(window);
        Keyboard::RegisterKeyboard(window);
        Mouse::RegisterMouse(window);
    }

    ~Game(){
        for (GameObject* object : objects) {
            delete object;
        }

        objects.clear();

        for(ComponentManager* manager : managers){
            manager->Destroy();
            delete manager;
        }
    }

    static void AddObject(GameObject* object){
        if(instance == nullptr)return;

        instance->objects.push(object);

        return;
    }

    template<class T,
    typename = typename std::enable_if<std::is_base_of<Component, T>::value>::type>
    static void Register(T* component){
        if(instance == nullptr)return;

        Array<ComponentManager*>& managers = instance->managers;
        ComponentManager* manager = component->GetManager();
        
        if(manager == nullptr)return;
        
        if(managers.indexOf(manager) == -1){
            managers.push(manager);
            manager->Setup();
            manager->Register(component);
        }else{
            manager->Register(component);
        }
        return;
    }
public:
    static Game* CreateInstance(GLFWwindow* gl_Window){
        if(instance != nullptr)return instance;
        instance = new Game(gl_Window);
        return instance;
    }

    static Game* GetInstance(){
        if(instance == nullptr)throw "No Instance was Created";
        return instance;
    }

    static void DestroyInstance(){
        if(instance != nullptr)
            delete instance;
        instance = nullptr;
    }

    static void Start(){
        if(instance == nullptr)return;
        instance->active = true;
        instance->runWhile();
    }

    static void Quit(){
        if(instance == nullptr)return;
        instance->active = false;
    }
    
    void runWhile(){
        while(glfwWindowShouldClose(window) == false && active){
            loop();
        }
    }

    void loop(){
        BeforeUpdate();
        Update();
        AfterUpdate();
    }

    void BeforeUpdate(){

    }

    void AfterUpdate(){

    }

    void Update(){

    }
};



Game* Game::instance = nullptr;