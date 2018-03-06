#include "main.h"
#include "cube.h"
#include "prism.h"
#include "enemy.h"

#define RAND_COLOR { rand() % 255, rand() % 255, rand() % 255 }
#define SEA_OFFSET 0.4f

#define ENEMY_SPEED 0.02f
#define GRAVITY 0.004f
#define ROLL_RATE 0.1f

Enemy::Enemy(float x, float y, float z, float width, color_t color) {
	this->set_position(x, SEA_OFFSET + y, z);
	this->set_speed(ENEMY_SPEED, 0, ENEMY_SPEED);
	this->set_rotation(0, 0, 0);
	this->set_dimensions(width);
	this->roll_rate = ROLL_RATE;
	this->color = color;
}

void Enemy::draw(glm::mat4 VP) {
	Matrices.model = glm::mat4(1.0f);
	glm::mat4 translate = glm::translate (this->position);
	glm::mat4 rotate_y    = glm::rotate((float) (this->rotation.y * M_PI / 180.0f), glm::vec3(0, 1, 0));
	glm::mat4 rotate_z    = glm::rotate((float) (this->rotation.z * M_PI / 180.0f), glm::vec3(0, 0, 1));
	glm::mat4 rotate_x    = glm::rotate((float) (this->rotation.x * M_PI / 180.0f), glm::vec3(1, 0, 0));
	Matrices.model *= (translate * rotate_y * rotate_z * rotate_x);
	glm::mat4 MVP = VP * Matrices.model;
	glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
	body.draw(MVP);
	for(auto spike: spikes) spike.draw(MVP);
}

void Enemy::set_position(float x, float y, float z) {
	this->position = glm::vec3(x, y, z);
}

void Enemy::set_speed(float x_speed, float y_speed, float z_speed) {
	this->speed = glm::vec3(x_speed, y_speed, z_speed);
}

void Enemy::set_rotation(float x_rot, float y_rot, float z_rot) {
	this->rotation = glm::vec3(x_rot, y_rot, z_rot);
}

void Enemy::set_dimensions(float width) {

	this->size = glm::vec3(width, width, width);

	this->body = Cube(0.0f, 0.0f, 0.0f, width, width / 2.0f, width, this->color);

	spikes.clear();
	// spikes
	color_t smart_color = {255, 246, 0}, normal_color = {255, 144, 0};
	color_t spike_color = (this->is_smart)?  smart_color : normal_color;

	// top spike
	Prism spike_top = Prism(0.0f, 0.0f, width / 2.0f, 0.6 * width, 0.6 * width, spike_color);
	spike_top.set_position(0.0f, width / 4.0f, 0.0f);
	this->spikes.push_back(spike_top);

	// front and back
	Prism spike_back = spike_top, spike_front = spike_top;

	spike_back.set_position(0.0f, 0.0f, width / 2.0f);
	spike_back.set_rotation(90.0f, 0.0f, 0.0f);
	this->spikes.push_back(spike_back);

	spike_front.set_position(0.0f, 0.0f, - width / 2.0f);
	spike_front.set_rotation(-90.0f, 0.0f, 0.0f);
	this->spikes.push_back(spike_front);


	// left and right
	Prism spike_left = spike_top, spike_right = spike_top;

	spike_left.set_position(width / 2.0f, 0.0f, 0.0f);
	spike_left.set_rotation(0.0f, 0.0f, -90.0f);
	this->spikes.push_back(spike_left);

	spike_right.set_position(- width / 2.0f, 0.0f, 0.0f);
	spike_right.set_rotation(0.0f, 0.0f, 90.0f);
	this->spikes.push_back(spike_right);

}

void Enemy::tick(float x, float z) {

	// change the basic speed
	this->position.x += (((this->position.x > x)? -1 : 1) * this->speed.x);
	this->position.z += (((this->position.z > z)? -1 : 1) * this->speed.z);

	this->position.y += this->speed.y;
    if(this->position.y > 0)
        this->speed.y -= GRAVITY;
    else {
        this->position.y = 0;
        this->speed.y = 0;
        this->is_jumping = false;
    }


	if(this->is_smart) {

		if(this->speed.x < 0.2f && this->speed.x > -0.2f) this->rotation.x += this->roll_rate;
		else if(this->rotation.x) this->rotation.x += ((this->rotation.x > 0)? -1 : 1) * ROLL_RATE;
		if(this->speed.z < 0.2f && this->speed.z > -0.2f) this->rotation.z += this->roll_rate;
		else if(this->rotation.z) this->rotation.z += ((this->rotation.z > 0)? -1 : 1) * ROLL_RATE;

		if(this->rotation.x > 5.0f || this->rotation.x < -5.0f) {
			this->rotation.x = ((this->rotation.x > 0)? 1 : -1) * 5.0f;
			this->roll_rate *= -1;
		}

		if(abs(this->position.x - x) < 5.0f) this->speed.x -= 0.1f * ENEMY_SPEED;
		else if(abs(this->position.x - x) > 15.0f) this->speed.x += 0.05f * ENEMY_SPEED;

		if(abs(this->position.z - z) < 5.0) this->speed.z -= 0.1f * ENEMY_SPEED;
		else if(abs(this->position.z - z) > 15.0f) this->speed.z += 0.05f * ENEMY_SPEED;
	}

}

bounding_box_t Enemy::bounding_box() {
	bounding_box_t bbox = {
		this->position.x,
		this->position.y,
		this->position.z,
		1.5f * this->size.x,
		this->size.y,
		1.5f * this->size.z
	};
	return bbox;
}
