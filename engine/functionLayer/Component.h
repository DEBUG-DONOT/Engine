#pragma once
#include<string>

	class GameObject;
	class Component
	{
	public:
		Component() = default;
		virtual ~Component() = default;
		virtual void tick();
		inline void SetParentGameObjectiPointer(GameObject* pp) { parentGOPointer = pp; }
		inline void SetName(std::string name) { _name = name; }
	private:
		GameObject* parentGOPointer = nullptr;
		std::string _name;
	};




