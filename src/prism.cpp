#include "main.h"
#include "prism.h"

Prism::Prism(float x, float z, float width, float height, float depth, color_t color) {
	/*
		@param x float -> x coord
		@param z float -> z coord
		@param color color_t -> color
	*/
	this->set_position(x, 0.0f, z);
	this->set_dimensions(width, height, depth);
	this->set_rotation(0, 0, 0);
	this->color = color;

	#define x_coord this->size.x / 2.0f
	#define y_coord this->size.y
	#define z_coord this->size.z / 2.0f

	GLfloat vertex_buffer_data[] = {
       	0.0f, y_coord, 0.0f,
		- x_coord, 0.0f, z_coord,
		- x_coord, 0.0f, - z_coord,

		0.0f, y_coord, 0.0f,
		- x_coord, 0.0f, z_coord,
		x_coord, 0.0f, z_coord,

		0.0f, y_coord, 0.0f,
		x_coord, 0.0f, - z_coord,
		- x_coord, 0.0f, - z_coord,

		0.0f, y_coord, 0.0f,
		x_coord, 0.0f, - z_coord,
		x_coord, 0.0f, z_coord,
    };

    this->object = create3DObject(GL_TRIANGLES, 4*3, vertex_buffer_data, color, GL_FILL);
}

void Prism::set_position(float x, float y, float z) {
	this->position = glm::vec3(x, y, z);
}

void Prism::set_dimensions(float width, float height, float depth) {
	this->size = glm::vec3(width, height, depth);
}

void Prism::set_rotation(float x_rot, float y_rot, float z_rot) {
	this->rotation = glm::vec3(x_rot, y_rot, z_rot);
}

void Prism::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);
    glm::mat4 rotate_y    = glm::rotate((float) (this->rotation.y * M_PI / 180.0f), glm::vec3(0, 1, 0));
    glm::mat4 rotate_z    = glm::rotate((float) (this->rotation.z * M_PI / 180.0f), glm::vec3(0, 0, 1));
    glm::mat4 rotate_x    = glm::rotate((float) (this->rotation.x * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate_y * rotate_z * rotate_x);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

bounding_box_t Prism::bounding_box() {
    bounding_box_t bbox = {
    	this->position.x,
    	this->position.y + 1.5f,
    	this->position.z,
    	this->size.x,
    	this->size.y,
    	this->size.z
    };
    return bbox;
}
