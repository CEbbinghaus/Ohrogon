#pragma once

#include <atyp_Array.h>
#include <type_traits>
#include "Component.h"

class Game{
    Array<void*> managers;
    Game(){
        managers = Array<void*>();
    }
protected:
    // template<class T,
    // class = class std::enable_if<std::is_base_of<Component, T>::value>::type>
    // void GetManager(T* instance){
    //     if(managers.indexOf(instance->GetManager()) == -1){

    //     }
    // }
public:

    
};