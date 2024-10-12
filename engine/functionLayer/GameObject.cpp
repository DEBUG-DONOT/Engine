#include "GameObject.h"
#include"Component.h"
void runtime::GameObject::addComponent(std::string componentName)
{
	if (ComponentMap.find(componentName) == ComponentMap.end())
	{
		//根据名字创建一个新的组件
		//ComponentMap[componentName] = std::vector<Component*>{};
	}
	else
	{

	}

}
