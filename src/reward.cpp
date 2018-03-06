#include "main.h"
#include "prism.h"
#include "reward.h"

#define REWARD_WIDTH 1.5
#define ARROW_LENGTH 3.0f

Reward::Reward(float x, float z) {
    this->set_position(x, 0.0f, z);
    this->set_rotation(0.0f, 0.0f, 0.0f);
    this->set_dimension(3 * REWARD_WIDTH, ARROW_LENGTH * (1 + sin(M_PI / 4.0f)), REWARD_WIDTH);

    // rendering the elements
    color_t top_color = {255, 0, 242};

    // top arrow
    Prism top_prism = Prism(0.0f, 0.0f, 2.5f, ARROW_LENGTH, REWARD_WIDTH, top_color);
    top_prism.set_position(0.0f, 5.0f, 0.0f);
    top_prism.set_rotation(180.0f, 0.0f, 0.0f);
    this->elements.push_back(top_prism);

    // bottom two

    Prism bottom_left = top_prism, bottom_right = top_prism;
    bottom_left.set_position(2 * REWARD_WIDTH, 0.0f, 0.0f);
    bottom_left.set_rotation(0.0f, 0.0f, 45.0f);
    bottom_left.color = this->color;
    this->elements.push_back(bottom_left);

    bottom_right.set_position(-2 * REWARD_WIDTH, 0.0f, 0.0f);
    bottom_right.set_rotation(0.0f, 0.0f, -45.0f);
    bottom_right.color = this->color;
    this->elements.push_back(bottom_right);

}

void Reward::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate_y    = glm::rotate((float) (this->rotation.y * M_PI / 180.0f), glm::vec3(0, 1, 0));
    glm::mat4 rotate_z    = glm::rotate((float) (this->rotation.z * M_PI / 180.0f), glm::vec3(0, 0, 1));
    glm::mat4 rotate_x    = glm::rotate((float) (this->rotation.x * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate_y * rotate_z * rotate_x);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);

    for(auto &element: elements) element.draw(MVP);
}


void Reward::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}


void Reward::set_rotation(float x_rot, float y_rot, float z_rot) {
    this->rotation = glm::vec3(x_rot, y_rot, z_rot);
}

void Reward::set_dimension(float x, float y, float z) {
    this->size = glm::vec3(x, y, z);
}

void Reward::tick() {
    if(this->counter)
        this->counter--;
    else
        is_visible = false;
}

bounding_box_t Reward::bounding_box() {
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
