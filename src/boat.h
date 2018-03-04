#include "main.h"
#include "objects.h"

#ifndef BOAT_H
#define BOAT_H


class Boat {
public:
    Boat() {}
    Boat(float x, float z, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
    double yspeed;
    bool jumping;
    bounding_box_t bounding_box();


    Cube plank[6];
};


#endif // BOAT_H
