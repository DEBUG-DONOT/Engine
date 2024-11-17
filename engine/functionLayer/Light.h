#pragma once
#include<glm/glm.hpp>


class Light
{
public:
	Light(glm::vec3 _pos,glm::vec3 dir,glm::vec3 _color);
	inline glm::vec3 GetPos() { return position; }
	inline glm::vec3 GetDirection() { return direction; }
	inline glm::vec3 GetColor() { return color; }
private:
	glm::vec3 position, direction, color;

};
