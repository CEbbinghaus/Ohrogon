#pragma once
#include "Component.h"

class ColliderManager;

class Collider : Component{

	Symbol _IDENTIFIER() override{
		return Symbol("Component.Collider");
	}

	ComponentManager* CreateManager() override{
		return ColliderManager::GetInstance();
	}
};

class ColliderManager: ComponentManager{
	friend Collider;
	static ColliderManager* ManagerInstance;
	ColliderManager(){}
public:
	static ColliderManager* GetInstance(){
		if(ManagerInstance != nullptr)return ManagerInstance;
		ManagerInstance = new ColliderManager();
	}

	void Setup(){}

	void Execute(){}

	void Register(Component* component){}
	void DeRegister(Component* component){}

	void Destroy(){}
};

ColliderManager* ColliderManager::ManagerInstance = nullptr;