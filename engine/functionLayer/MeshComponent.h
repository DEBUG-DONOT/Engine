#pragma once
#include"Component.h"
#include"../ResourceLayer/Model.h"
namespace runtime
{
	class MeshComponent :public Component
	{
	public:
		//调用下层接口，并在tick中渲染
		

	private:
		void render();
		std::string modelPath;
		void LoadModel(const std::string& path);


	};



}