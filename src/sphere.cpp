#include "main.h"
#include "sphere.h"

#define SPHERE_GRAVITY  0.02f
#define DISAPPEAR_DEPTH 5.0f

Sphere::Sphere(float x, float y, float z, float radius, color_t color) {
	this->color = color;
	this->radius = radius;
	this->set_position(x, y, z);
	this->set_speed(0, 0, 0);

    int faces = 100;
	GLfloat vertex_buffer_data[10 * faces * (1 + faces)];
    float _a = (2 * M_PI) / float(faces);

    for(int i = 0; i < faces; i++)
        for(int j = 0; j < faces; j++) {
            int idx = 9 * (faces * j + i);
            vertex_buffer_data[idx] = 0.0f,
            vertex_buffer_data[idx + 1] = 0.0f,
            vertex_buffer_data[idx + 2] = this->radius * sin(i * _a),
            vertex_buffer_data[idx + 3] = this->radius * cos(j * _a) * cos(i * _a),
            vertex_buffer_data[idx + 4] = this->radius * sin(j * _a) * cos(i * _a),
            vertex_buffer_data[idx + 5] = this->radius * sin(i * _a),
            vertex_buffer_data[idx + 6] = this->radius * cos((j + 1) * _a) * cos(i * _a),
            vertex_buffer_data[idx + 7] = this->radius * sin((j + 1 ) * _a) * cos(i * _a),
            vertex_buffer_data[idx + 8] = this->radius * sin(i * _a);
        }

    this->object = create3DObject(GL_TRIANGLES, faces*faces*3, vertex_buffer_data, color, GL_FILL);
}

void Sphere::set_position(float x, float y, float z) {
	this->position = glm::vec3(x, y, z);
}


void Sphere::set_speed(float x_speed, float y_speed, float z_speed) {
	this->speed = glm::vec3(x_speed, y_speed, z_speed);
}


bool Sphere::tick() {
    this->position.y += this->speed.y;
    this->speed.y -= SPHERE_GRAVITY;
    this->position.z -= this->speed.y * cos(rotation * M_PI / 180.0f);
    this->position.x += this->speed.y * sin(rotation * M_PI / 180.0f);
    return this->position.y < - DISAPPEAR_DEPTH;
}


void Sphere::draw(glm::mat4 VP) {
	Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}


bounding_box_t Sphere::bounding_box() {
    bounding_box_t bbox = {
    	this->position.x,
    	this->position.y,
    	this->position.z,
    	this->radius,
    	this->radius,
    	this->radius
    };
    return bbox;
}
