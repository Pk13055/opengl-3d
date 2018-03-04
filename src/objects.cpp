#include "objects.h"
#include "main.h"
/*
// Define Sphere


Sphere::Sphere(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    this->speed = 0.0;
    this->yspeed = 0.0;
    this->radius = 1.0f;

    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A Sphere has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat vertex_buffer_data[90000];
    int sides = 100;
    float angle = (2*M_PI)/float(sides);

    for(int j=0;j<sides;j++)
        for(int i=0;i<sides;i++)
        {
            int k = sides*i + j;
            vertex_buffer_data[9*k] = 0.0f;
            vertex_buffer_data[9*k + 1] = 0.0f;
            vertex_buffer_data[9*k + 2] = radius*sin(j*angle);
            vertex_buffer_data[9*k + 3] = radius*cos(i*angle)*cos(j*angle);
            vertex_buffer_data[9*k + 4] = radius*sin(i*angle)*cos(j*angle);
            vertex_buffer_data[9*k + 5] = radius*sin(j*angle);
            vertex_buffer_data[9*k + 6] = radius*cos((i+1)*angle)*cos(j*angle);
            vertex_buffer_data[9*k + 7] = radius*sin((i+1)*angle)*cos(j*angle);
            vertex_buffer_data[9*k + 8] = radius*sin(j*angle);
        }

    this->object = create3DObject(GL_TRIANGLES, sides*sides*3, vertex_buffer_data, color, GL_FILL);
}

void Sphere::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of Sphere arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Sphere::set_position(float x, float y,float z) {
    this->position = glm::vec3(x, y, z);
}

bool Sphere::tick() {


    this->position.y += yspeed;
    this->yspeed -= 0.001;

    this->position.z -= speed*cos(rotation*M_PI/180.0f);
    this->position.x += speed*sin(rotation*M_PI/180.0f);

    return this->position.y < -5.0f;

}

bounding_box_t Sphere::bounding_box() {
    float x = this->position.x, y = this->position.y, z = this->position.z;
    bounding_box_t bbox = { x, y,z, this->radius, this->radius, this->radius };
    return bbox;
}
*/
