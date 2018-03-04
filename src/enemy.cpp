#include "main.h"
#include "cube.h"
#include "prism.h"
#include "enemy.h"
#define RAND_COLOR { rand() % 255, rand() % 255, rand() % 255 }

Enemy::Enemy(float x, float y, float z, float width, float height, color_t color) {
	this->set_position(x, y, z);
	this->set_speed(0, 0, 0);
	this->set_dimensions(width, height, width);
	this->color = color;
	body = Cube(x, y + this->size.y / 2.0f, z, this->size.x, this->size.y / 2.0, this->size.z, this->color);
	spikes.push_back(Prism(
		this->position.x + ((rand() % 2 )? -1 : 1) * (rand() % (int)(this->size.x / 2.0f)),
		this->position.z + ((rand() % 2 )? -1 : 1) * (rand() % (int)(this->size.z / 2.0f)),
		this->size.x,
		this->size.y,
		this->size.z,
		this->color
		));
}

void Enemy::draw(glm::mat4 VP) {
	Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    for(auto spike: spikes) spike.draw(MVP);
    body.draw(MVP);
}

void Enemy::set_position(float x, float y, float z) {
	this->position = glm::vec3(x, y, z);
}

void Enemy::set_speed(float x_speed, float y_speed, float z_speed) {
	this->speed = glm::vec3(x_speed, y_speed, z_speed);
}

void Enemy::set_dimensions(float width, float height, float depth) {
	this->size = glm::vec3(width, height, depth);
}

void Enemy::tick(float x, float z) {
    this->position.z += ((this->position.z > z)? -1 : 1) * this->speed.z;
    this->position.x += ((this->position.x > x)? -1 : 1) * this->speed.x;
}

bounding_box_t Enemy::bounding_box() {
	bounding_box_t bbox = {
    	this->position.x,
    	this->position.y,
    	this->position.z,
    	this->size.x,
    	this->size.y,
    	this->size.z
    };
    return bbox;
}
