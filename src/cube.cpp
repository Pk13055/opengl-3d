#include "main.h"
#include "cube.h"
#include <iostream>
using namespace std;


// Define Cube

Cube::Cube(float x, float y,float z, float width , float height , float depth,  color_t color) {
    /*
        @param x float -> coordinates
        @param y float -> ''
        @param z float -> ''
        @param width float -> size
        @param height float -> ''
        @param depth float -> ''
        @param color color_t -> color of the cube
    */
    this->set_position(x, y, z);
    this->set_dimensions(width, height, depth);
    this->set_speed(0, 0, 0);
    this->set_rotation(0, 0, 0);

    int faces = 6, traingles_per_face = 2, vertices = 3;

    #define x_coord this->size.x / 2.0f
    #define y_coord this->size.y / 2.0f
    #define z_coord this->size.z / 2.0f

    GLfloat vertex_buffer_data[] = {

        x_coord , - y_coord ,  z_coord,
        - x_coord , - y_coord , - z_coord,
        x_coord , - y_coord , - z_coord,

        x_coord ,  y_coord , - z_coord,
        x_coord , - y_coord , - z_coord,
        - x_coord , - y_coord , - z_coord,

        - x_coord , - y_coord , - z_coord,
        - x_coord , - y_coord ,  z_coord,
        - x_coord ,  y_coord ,  z_coord,

        x_coord ,  y_coord , - z_coord,
        - x_coord , - y_coord , - z_coord,
        - x_coord ,  y_coord , - z_coord,

        - x_coord ,  y_coord ,  z_coord,
        - x_coord , - y_coord ,  z_coord,
        x_coord , - y_coord ,  z_coord,

        x_coord ,  y_coord ,  z_coord,
        x_coord , - y_coord , - z_coord,
        x_coord ,  y_coord , - z_coord,

        - x_coord , - y_coord , - z_coord,
        - x_coord ,  y_coord ,  z_coord,
        - x_coord ,  y_coord , - z_coord,

        x_coord , - y_coord ,  z_coord,
        - x_coord , - y_coord ,  z_coord,
        - x_coord , - y_coord , - z_coord,

        x_coord , - y_coord , - z_coord,
        x_coord ,  y_coord ,  z_coord,
        x_coord , - y_coord ,  z_coord,

        x_coord ,  y_coord ,  z_coord,
        x_coord ,  y_coord , - z_coord,
        - x_coord ,  y_coord , - z_coord,

        x_coord ,  y_coord ,  z_coord,
        - x_coord ,  y_coord , - z_coord,
        - x_coord ,  y_coord ,  z_coord,

        x_coord ,  y_coord ,  z_coord,
        - x_coord ,  y_coord ,  z_coord,
        x_coord , - y_coord ,  z_coord
    };

    #undef x_coord
    #undef y_coord
    #undef z_coord

    this->object = create3DObject(GL_TRIANGLES,  faces * traingles_per_face * vertices,
       vertex_buffer_data, color, GL_FILL);
}

void Cube::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate_y    = glm::rotate((float) (this->rotation.y * M_PI / 180.0f), glm::vec3(0, 1, 0));
    glm::mat4 rotate_z    = glm::rotate((float) (this->rotation.z * M_PI / 180.0f), glm::vec3(0, 0, 1));
    glm::mat4 rotate_x    = glm::rotate((float) (this->rotation.x * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate_y * rotate_z * rotate_x);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
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

void Cube::set_rotation(float x_rot, float y_rot, float z_rot) {
    this->rotation = glm::vec3(x_rot, y_rot, z_rot);
}

bounding_box_t Cube::bounding_box() {
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
