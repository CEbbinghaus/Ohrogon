#pragma once
#include "Component.h"

class Collider;

class ColliderManager: ComponentManager{
	friend Collider;

	static ColliderManager* ManagerInstance;

	ColliderManager(){}

	static ComponentManager* GetInstance(){
		if(ManagerInstance != nullptr)return ManagerInstance;
		ManagerInstance = new ColliderManager();
		return ManagerInstance;
	}
public:

	void Setup(){}

	void Execute();

	void Register(Component* component);
	void DeRegister(Component* component);

	void Destroy(){}
};

class Collider : public Component{
public:
	Collider(GameObject* gm): Component(gm){}

	static Symbol Type(){
		return Symbol("Component.Collider");
	}
	Symbol InstanceType() override{
		return Symbol("Component.Collider");
	}

	ComponentManager* GetManager() override{
		return ColliderManager::GetInstance();
	}
};

void ColliderManager::Register(Component* component){
	
}

void ColliderManager::DeRegister(Component* component){

}

void ColliderManager::Execute(){

}

ColliderManager* ColliderManager::ManagerInstance = nullptr;