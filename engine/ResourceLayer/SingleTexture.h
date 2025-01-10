#pragma once
#include<string>

class SingleTexure
{
public:
	SingleTexure(std::string path);
	unsigned int texture_id;
	void bind();
private:
	unsigned int LoadTexture(std::string path);
	std::string path;
};