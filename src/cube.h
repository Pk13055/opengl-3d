#include "main.h"

#ifndef CUBE_H
#define CUBE_H

class Cube {
public:
	Cube() {}
	Cube(float x, float y, float z, float width, float height, float depth, color_t color);
	glm::vec3 position, speed, size;
	color_t color;
	float rotation = 0;
	void draw(glm::mat4 VP);
	void set_position(float x, float y, float z);
	void set_dimensions(float width, float height, float depth);
	void set_speed(float x_speed, float y_speed, float z_speed);
	bounding_box_t bounding_box();
private:
	VAO *object;
};

#endif // CUBE_H
