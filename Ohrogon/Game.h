#pragma once

#include <gl_core_4_5.h>
#include <glfw3.h>

#include <atyp_Array.h>
#include <type_traits>
#include "Component.h"
#include "Console.h"
#include "Keyboard.h"
#include "Mouse.h"

class ComponentManager;
class GameObject;

class Game{
    friend GameObject;
    
    static Game* instance;
    GLFWwindow* window;

    Array<GameObject*> objects;
    Array<ComponentManager*> managers;
    bool active = true;

    Game(GLFWwindow* w);

    ~Game();

    static void AddObject(GameObject* object);

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
    static Game* CreateInstance(GLFWwindow* gl_Window);

    static Game* GetInstance();

    static void DestroyInstance();

    static void Start();

    static void Quit();
    
    void runWhile();

    void loop();

    void BeforeUpdate();

    void AfterUpdate();

    void Update();
};