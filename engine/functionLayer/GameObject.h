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
			//�����漰������,����ʵ������õ��������
			T* cop = new T();
			components.push_back(cop);
		}
		

	private:
		std::string _name;
		std::vector<Component*> components;
	};


	class GOID
	{
		//GO��id������ʶ��ͬ��go
	};

