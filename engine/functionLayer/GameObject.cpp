#include "GameObject.h"
#include"Component.h"
void runtime::GameObject::addComponent(std::string componentName)
{
	if (ComponentMap.find(componentName) == ComponentMap.end())
	{
		//�������ִ���һ���µ����
		//ComponentMap[componentName] = std::vector<Component*>{};
	}
	else
	{

	}

}
