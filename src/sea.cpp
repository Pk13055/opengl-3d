#include "main.h"
#include "sea.h"

#define SEA_WIDTH 400
#define SEA_LENGTH 400
#define SEA_TURBULENCE 0.2f

Sea::Sea(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    float x_coord = SEA_WIDTH / 2,
    z_coord = SEA_LENGTH / 2;
    static const GLfloat vertex_buffer_data[] = {
        - x_coord, 0.0, z_coord,
        x_coord, 0.0, z_coord,
        x_coord, 0.0, - z_coord,

        x_coord, 0.0, - z_coord,
        - x_coord, 0.0, - z_coord,
        - x_coord, 0.0, z_coord
    };

    this->object = create3DObject(GL_TRIANGLES, 2 * 3, vertex_buffer_data, color, GL_FILL);
}

void Sea::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Sea::set_position(float x, float z) {
    this->position = glm::vec3(x, this->position.y , z);
}

void Sea::tick() {
    this->position.y += yspeed;
    this->yspeed += ((this->position.y <= 0.0)? 1 : -1 ) * SEA_TURBULENCE;
    this->position.y =  ((this->position.y > 2.5f)? 1 : -1 ) * 2.5f;
}

