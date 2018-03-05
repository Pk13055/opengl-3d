#include "boat.h"
#include "cube.h"
#include "main.h"


#define RAND_COLOR { rand() % 255, rand() % 255, rand() % 255 }
#define SEA_OFFSET 0.6f

#define BOAT_WALL_X_THICKNESS 0.05f
#define BOAT_WALL_Y_HEIGHT 4.0f
#define BOAT_WALL_Z_LENGTH 4.5f
#define MAST_HEIGHT 5.0f

#define BOAT_WIDTH 4.0f
#define BOAT_LENGTH 6.0f

#define BOAT_MAX_SPEED 3.0f
#define ROLL_RATE 0.08f

#define GRAVITY 0.004f
#define WEAPON_COUNT 10

Boat::Boat(float x, float z, color_t color) {
    this->set_position(x, 0, z);
    this->set_speed(0, 0, 0);
    this->set_rotation(0, 0, 0);
    this->color = color;
    this->roll_rate = ROLL_RATE;

    for(int i = 0; i < WEAPON_COUNT; i++)
        this->weapons.push_back(Sphere(0.0f, SEA_OFFSET, -BOAT_WALL_Z_LENGTH / 2.0f,
            0.8f * (1 + (rand() % 3)), COLOR_RED));
    weapons.back().visible = true;

    /*  WALLS OF THE BOAT */
    color_t walls_color = RAND_COLOR;

    // Left Wall
    this->walls.push_back(Cube(BOAT_WIDTH / 2.0f, 0.0f, 0.0f,
        BOAT_WALL_X_THICKNESS, BOAT_WALL_Y_HEIGHT, BOAT_WALL_Z_LENGTH,
        walls_color));

    // Right Wall
    this->walls.push_back(Cube(- BOAT_WIDTH / 2.0f, 0.0f, 0.0f,
        BOAT_WALL_X_THICKNESS, BOAT_WALL_Y_HEIGHT, BOAT_WALL_Z_LENGTH,
        walls_color));

    // Back Wall
    this->walls.push_back(Cube(0.0f, 0.0f, BOAT_WALL_Z_LENGTH / 2.0f,
        BOAT_WIDTH, BOAT_WALL_Y_HEIGHT, BOAT_WALL_X_THICKNESS,
        walls_color));

    // Bottom Wall
    float angle = 45;

    // base
    this->walls.push_back(Cube(0.0f, SEA_OFFSET, 0.0f,
        BOAT_WIDTH, BOAT_WALL_X_THICKNESS, BOAT_WALL_Z_LENGTH,
        this->color));

    // frontal
    Cube cb_b = Cube(0.0f, SEA_OFFSET, -BOAT_WALL_Z_LENGTH / 2.0f,
        BOAT_WIDTH, BOAT_WALL_X_THICKNESS, BOAT_WALL_Z_LENGTH * sin(angle * M_PI / 180.0f),
        this->color);
    cb_b.rotation.y = 45;
    this->walls.push_back(cb_b);

    // front side

    color_t front_color = RAND_COLOR;
    // front left wall
    Cube cb_l = Cube((BOAT_WIDTH  - BOAT_WALL_Z_LENGTH * cos(angle * M_PI / 180.0f)) / 2.0f,
        0.0f,
        (- BOAT_WALL_Z_LENGTH - BOAT_WALL_Z_LENGTH * sin(angle * M_PI / 180.0f)) / 2.0f,
        BOAT_WALL_X_THICKNESS, BOAT_WALL_Y_HEIGHT, BOAT_WALL_Z_LENGTH,
        front_color);
    cb_l.rotation.y = angle;
    this->walls.push_back(cb_l);

    // front right wall
    Cube cb_r = Cube((-BOAT_WIDTH  + BOAT_WALL_Z_LENGTH * cos(angle * M_PI / 180.0f)) / 2.0f,
        0.0f,
        (- BOAT_WALL_Z_LENGTH - BOAT_WALL_Z_LENGTH * sin(angle * M_PI / 180.0f)) / 2.0f,
        BOAT_WALL_X_THICKNESS, BOAT_WALL_Y_HEIGHT, BOAT_WALL_Z_LENGTH,
        front_color);
    cb_r.rotation.y = -angle;
    this->walls.push_back(cb_r);

    // mast
    this->walls.push_back(Cube(0.0f, SEA_OFFSET + MAST_HEIGHT / 2.0f, 0.0f,
        BOAT_WALL_X_THICKNESS, MAST_HEIGHT, BOAT_WALL_X_THICKNESS,
        this->color));

}

void Boat::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate_y    = glm::rotate((float) (this->rotation.y * M_PI / 180.0f), glm::vec3(0, 1, 0));
    glm::mat4 rotate_z    = glm::rotate((float) (this->rotation.z * M_PI / 180.0f), glm::vec3(0, 0, 1));
    glm::mat4 rotate_x    = glm::rotate((float) (this->rotation.x * M_PI / 180.0f), glm::vec3(1, 0, 0));
    Matrices.model *= (translate * rotate_y * rotate_z * rotate_x);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    for(auto wall: this->walls) wall.draw(MVP);
    for(auto weapon: this->weapons)
        if(weapon.visible) { weapon.draw(MVP); break; }
    for(auto shoot: this->shot)
        if(shoot.visible) shoot.draw(MVP);
}

void Boat::set_position(float x, float y, float z = 0) {
    this->position = glm::vec3(x, y, z);
}

void Boat::set_speed(float x_speed, float y_speed, float z_speed) {
    this->speed = glm::vec3(x_speed, y_speed, z_speed);
}

void Boat::set_rotation(float x_rot, float y_rot, float z_rot) {
    this->rotation = glm::vec3(x_rot, y_rot, z_rot);
}

void Boat::tick() {

    if(this->speed.x < 0.2f && this->speed.x > -0.2f) this->rotation.x += this->roll_rate;
    else if(this->rotation.x) this->rotation.x += ((this->rotation.x > 0)? -1 : 1) * ROLL_RATE;
    if(this->speed.z < 0.2f && this->speed.z > -0.2f) this->rotation.z += this->roll_rate;
    else if(this->rotation.z) this->rotation.z += ((this->rotation.z > 0)? -1 : 1) * ROLL_RATE;

    if(this->rotation.x > 5.0f || this->rotation.x < -5.0f) {
        this->rotation.x = ((this->rotation.x > 0)? 1 : -1) * 5.0f;
        this->roll_rate *= -1;
    }

    this->position.y += this->speed.y;
    this->position.x += this->speed.x * sin(this->rotation.y * M_PI/180.0f);
    this->position.z += this->speed.z * cos(this->rotation.y * M_PI / 180.0f);

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
        this->position.y + BOAT_WALL_Y_HEIGHT / 2.0f,
        this->position.z - BOAT_WALL_Z_LENGTH / 2.0f,
        BOAT_WIDTH , // +/- 2.5f
        BOAT_WALL_Y_HEIGHT + MAST_HEIGHT, // BOAT_WALL_Y_HEIGHT
        BOAT_LENGTH // BOAT_WALL_Z_LENGTH * (1 + sin(rotation))
    };
    return bbox;
}
