#include "main.h"
#include "prism.h"
#include "cube.h"

#define DEFAULT_SMART 0
#define DEFAULT_LIFE 10

#ifndef ENEMY_H
#define ENEMY_H


class Enemy {
public:

    Enemy() {}
    Enemy(float x, float y, float z, float width, color_t color);

    glm::vec3 position, speed, size, rotation;
    bool visible = true, is_smart = DEFAULT_SMART, is_jumping = false;
    int life = DEFAULT_LIFE;
    float roll_rate = 0;
    color_t color;

    Cube body;
    vector<Prism> spikes;

    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void set_speed(float x_speed, float y_speed, float z_speed);
    void set_rotation(float x_rot, float y_rot, float z_rot);
    void set_dimensions(float width);
    void tick(float x, float z);

    bounding_box_t bounding_box();
};


#endif // ENEMY_H
