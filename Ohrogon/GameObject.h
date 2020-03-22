#pragma once

#include <atyp_Transform.h>
#include <atyp_Array.h>
#include "Component.h"

class GameObject{
    Array<Component*> components;

  protected:
    bool active = true;
    GameObject(){

    }
    ~GameObject(){
        for(Component* component : components){
            delete component;
        }
    }
  public:
    Transform transform;

    template<typename T,
    typename = typename std::enable_if<std::is_base_of<Component, T>::value>::type>
    T* AddComponent(){
        T* component = new T();
        components.push(component);
        return component;
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