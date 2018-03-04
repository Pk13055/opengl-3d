#include "boat.h"
#include "cube.h"
#include "main.h"

#define BOAT_WALL_X_THICKNESS 0.3f
#define BOAT_WALL_Y_HEIGHT 4.0f
#define BOAT_WALL_Z_LENGTH 6.0f

#define BOAT_WIDTH 8.0f
#define BOAT_LENGTH 10.0f

#define BOAT_MAX_SPEED 5.0f

#define GRAVITY 0.004f
#define WEAPON_COUNT 10

Boat::Boat(float x, float z, color_t color) {
    this->set_position(x, 0, z);
    this->set_speed(0, 0, 0);
    this->color = color;

    for(int i = 0; i < WEAPON_COUNT; i++)
        this->weapons.push_back(Sphere(this->position.x,
            this->position.y + BOAT_WALL_Y_HEIGHT / 2.0f,
            this->position.z, 0.03 + 0.01 * (rand() % 3), COLOR_RED));
    weapons.back().visible = true;

    // right wall
    walls.push_back(Cube(BOAT_WIDTH / 2.0f, 0.0f, 0.0f,
        BOAT_WALL_X_THICKNESS, BOAT_WALL_Y_HEIGHT, BOAT_WALL_Z_LENGTH, color));
    // left wall
    walls.push_back(Cube(-BOAT_WIDTH / 2.0f, 0.0f, 0.0f,
        BOAT_WALL_X_THICKNESS, BOAT_WALL_Y_HEIGHT, BOAT_WALL_Z_LENGTH, color));

    // bottom base
    walls.push_back(Cube(0.0f, -BOAT_WALL_Y_HEIGHT / 2.0f, 0.0f,
        BOAT_WIDTH, BOAT_WALL_X_THICKNESS, BOAT_LENGTH/* * (1 - sin(this->rotation * M_PI / 180.0f))*/, color));

    // front triangle

    // front right wall
    Cube cb_r = Cube(-BOAT_WIDTH / 2.0f, 0.0f, -BOAT_LENGTH * (1 - sin(this->rotation * M_PI / 180.0f)),
        BOAT_WALL_X_THICKNESS, BOAT_WALL_Y_HEIGHT, BOAT_WALL_Z_LENGTH - 2.0f, color);
    cb_r.rotation = -30;
    walls.push_back(cb_r);

    // front left wall
    Cube cb_l = Cube(BOAT_WIDTH / 2.0f, 0.0f, -BOAT_LENGTH * (1 - sin(this->rotation * M_PI / 180.0f)),
        BOAT_WALL_X_THICKNESS, BOAT_WALL_Y_HEIGHT, BOAT_WALL_Z_LENGTH - 2.0f, color);
    cb_l.rotation = 30;
    walls.push_back(cb_l);
}

void Boat::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    for(auto wall: this->walls) wall.draw(MVP);
    for(auto weapon: this->weapons)
        if(weapon.visible) weapon.draw(MVP);
    for(auto shoot: this->shot)
        if(shoot.visible) shoot.draw(MVP);
}

void Boat::set_position(float x, float y, float z = 0) {
    this->position = glm::vec3(x, y, z);
}

void Boat::set_speed(float x_speed, float y_speed, float z_speed) {
    this->speed = glm::vec3(x_speed, y_speed, z_speed);
}

void Boat::tick() {

    this->position.y += this->speed.y;
    this->position.x += this->speed.x * sin(this->rotation * M_PI/180.0f);
    this->position.z += this->speed.z * cos(this->rotation * M_PI / 180.0f);

    if(this->position.y > 0)
        this->speed.y -= GRAVITY;
    else {
        this->position.y = 0;
        this->speed.y = 0;
        this->is_jumping = false;
    }

    if(this->speed.x > BOAT_MAX_SPEED)
        this->speed.x = BOAT_MAX_SPEED;
    if(this->speed.y > BOAT_MAX_SPEED)
        this->speed.y = BOAT_MAX_SPEED;
    if(this->speed.z > BOAT_MAX_SPEED)
        this->speed.z = BOAT_MAX_SPEED;
}

bounding_box_t Boat::bounding_box() {
    bounding_box_t bbox = {
        this->position.x,
        this->position.y + 2.0f,
        this->position.z - 1.5f,
        BOAT_WIDTH / 2.0f, // +/- 2.5f
        BOAT_WALL_Y_HEIGHT / 2.0f, // BOAT_WALL_Y_HEIGHT
        BOAT_LENGTH / 2.0f // BOAT_WALL_Z_LENGTH * (1 + sin(rotation))
    };
    return bbox;
}
