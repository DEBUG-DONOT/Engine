#pragma once
#include"Component.h"
#include"../ResourceLayer/Model.h"
namespace runtime
{
	class MeshComponent :public Component
	{
	public:
		//�����²�ӿڣ�����tick����Ⱦ
		

	private:
		void render();
		std::string modelPath;
		void LoadModel(const std::string& path);


	};



}