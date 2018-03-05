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

    glm::vec3 position, speed, rotation, size;
    vector<Cube> walls;
    vector<Sphere> shots;
    color_t color;
    float score = 0, life = BOAT_LIFE, roll_rate = 0;
    bool is_jumping = false, draw_shot = true;

    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void set_speed(float x_speed, float y_speed, float z_speed);
    void set_rotation(float x_rot, float y_rot, float z_rot);
    void tick();

    bounding_box_t bounding_box();

private:
    VAO *object;
};


#endif // BOAT_H
