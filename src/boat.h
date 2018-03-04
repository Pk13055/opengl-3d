#include "main.h"
#include "cube.h"
#include "sphere.h"
#include <vector>

#define BOAT_LIFE 1000
using namespace std;

#ifndef BOAT_H
#define BOAT_H


class Boat {
public:
    Boat() {}
    Boat(float x, float z, color_t color);

    glm::vec3 position, speed;
    vector<Cube> walls;
    vector<Sphere> weapons, shot;
    color_t color;
    float rotation = 0, score = 0, lives = BOAT_LIFE;
    bool is_jumping = false;

    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void set_speed(float x_speed, float y_speed, float z_speed);
    void tick();

    bounding_box_t bounding_box();
};


#endif // BOAT_H
