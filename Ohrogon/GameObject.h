#pragma once

#include <atyp_Transform.h>
#include <atyp_Array.h>
#include "Component.h"
#include "Game.h"
#include "Console.h"

class GameObject{
    friend Game;
    Array<Component*> components;

    bool active = true;
  protected:
    GameObject* gameObject;

    GameObject(): gameObject(this){
        Console::Log(String::format("Constructing GameObect. Adress: %X", (uint64_t)this));
        //components = Array<Component*>();
        Game::AddObject(this);
    }

    GameObject(const GameObject& original){
        printf("GameObject Copy Constructor\n");
    }
  
    GameObject(const GameObject&& original){
        printf("GameObject MoveConstructor Constructor\n");
    }
    
    GameObject& operator =(const GameObject& other){
        printf("GameObject Copy Assignment\n");
        return *this;
    }

    GameObject& operator =(const GameObject&& other){
        printf("GameObject Move Assignment\n");
        return *this;
    }

    
  public:
    virtual ~GameObject(){
        Console::Error("Deconstructing GameObject");
        // for(Component* component : components){
        //     component->DeRegister();
        //     delete component;
        // }
    }

    Transform transform;

    template<class T,
    typename = typename std::enable_if<std::is_base_of<Component, T>::value>::type>
    T* AddComponent(){
        if(Game::GetInstance() == nullptr)return nullptr;
        T* component = new T(this);
        components.push(component);
        Game::Register<T>(component);
        return component;
    }

    template<class T,
    typename = typename std::enable_if<std::is_base_of<Component, T>::value>::type>
    T* GetComponent(){
        Symbol type = T::Type();
        for(Component* component : components){
            if(component->InstanceType() == type)
                return (T*)component;
        }
        return nullptr;
    }
  
    template<class T,
    typename = typename std::enable_if<std::is_base_of<Component, T>::value>::type>
    T* RemoveComponent(){
        Symbol type = T::Type();
        for(int i = 0; i < components.length; ++i){
            Component* component = components[i];
            if(component->InstanceType() == type){
                components.removeIndex(i);
                return component;
            }
        }
        return nullptr;
    }

    bool IsActive(){
        return active;
    }

    void SetActive(bool value){
        active = value;
    }

    virtual void Awake(){}
    virtual void Update(){}
    virtual void Destroy(){}
};