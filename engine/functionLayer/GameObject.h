#pragma once
#include<vector>
#include<string>

	class Component;
	class GameObject
	{
	public:
		inline void SetGameObjectName(std::string& name) { _name = name; }
		void addComponent(std::string componentName);
		template <typename T>
		T* addComponent()
		{
			//这里涉及到反射,从类实例里面得到类的名字
			T* cop = new T();
			components.push_back(cop);
		}
		

	private:
		std::string _name;
		std::vector<Component*> components;
	};


	class GOID
	{
		//GO的id，用来识别不同的go
	};

