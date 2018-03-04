#include "main.h"
#include "cube.h"

Cube::Cube(float x, float y, float z, float width, float height, float depth, color_t color) {
	/*
		@param x float  	-> x coordinate
		@param y float  	-> y coordinate
		@param z float 		-> z coordinate
		@param width float 	-> x size
		@param height float -> y size
		@param depth float 	-> z size
		@param color color_t
	*/
	this->color = color;
	this->set_position(x, y, z);
	this->set_dimensions(width, height, depth);
	this->set_speed(0, 0, 0);

    #define x_coord this->size.x / 2.0f
    #define y_coord this->size.y / 2.0f
    #define z_coord this->size.z / 2.0f

    int faces = 6, traingles_per_face = 2, vertices = 3;

	GLfloat vertex_buffer_data[] = {
		x_coord,       - y_coord,     z_coord,
		- x_coord,     - y_coord,   - z_coord,
		x_coord,       - y_coord,     - z_coord,

		x_coord,       y_coord,       - z_coord,
		x_coord,       - y_coord,     - z_coord,
		- x_coord,     - y_coord,   - z_coord,

		- x_coord,     - y_coord,   - z_coord,
		- x_coord,     - y_coord,   z_coord,
		- x_coord,      y_coord,     z_coord,

		x_coord,       y_coord,       - z_coord,
		- x_coord,     - y_coord,   - z_coord,
		- x_coord,      y_coord,     - z_coord,

		- x_coord,      y_coord,     z_coord,
		- x_coord,     - y_coord,   z_coord,
		x_coord,       - y_coord,     z_coord,

		x_coord,       y_coord,       z_coord,
		x_coord,       - y_coord,     - z_coord,
		x_coord,       y_coord,       - z_coord,

		- x_coord,     - y_coord,   - z_coord,
		- x_coord,      y_coord,     z_coord,
		- x_coord,      y_coord,     - z_coord,

		x_coord,       - y_coord,     z_coord,
		- x_coord,     - y_coord,   z_coord,
		- x_coord,     - y_coord,   - z_coord,

		x_coord,       - y_coord,     - z_coord,
		x_coord,       y_coord,       z_coord,
		x_coord,       - y_coord,     z_coord,

		x_coord,       y_coord,       z_coord,
		x_coord,       y_coord,       - z_coord,
		- x_coord,      y_coord,     - z_coord,

		x_coord,       y_coord,       z_coord,
		- x_coord,      y_coord,     - z_coord,
		- x_coord,      y_coord,     z_coord,

		x_coord,       y_coord,       z_coord,
		- x_coord,      y_coord,     z_coord,
		x_coord,       - y_coord,     z_coord

	};

	#undef x_coord
	#undef y_coord
	#undef z_coord

	this->object = create3DObject(GL_TRIANGLES,  faces * traingles_per_face * vertices,
	 vertex_buffer_data, color, GL_FILL);
}

void Cube::set_position(float x, float y, float z) {
	this->position = glm::vec3(x, y, z);
}


void Cube::set_dimensions(float width, float height, float depth) {
	this->size = glm::vec3(width, height, depth);
}


void Cube::set_speed(float x_speed, float y_speed, float z_speed) {
	this->speed = glm::vec3(x_speed, y_speed, z_speed);
}

void Cube::draw(glm::mat4 VP) {
	Matrices.model = glm::mat4(VP);
	glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

bounding_box_t Cube::bounding_box() {
	bounding_box_t bbox = {
		this->position.x,
		this->position.y,
		this->position.z,
		this->size.x / 2.0f,
		this->size.y / 2.0f,
		this->size.z / 2.0f
	};
	return bbox;
}
