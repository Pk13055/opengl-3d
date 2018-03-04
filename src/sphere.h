#include "main.h"

#ifndef SPHERE_H
#define SPHERE_H

class Sphere {
public:
    Sphere() {}
    Sphere(float x, float y, float z, float radius, color_t color);
    glm::vec3 position, speed;
    color_t color;
    float rotation = 0, radius = 0;
    bool visible = false;
    void draw(glm::mat4 VP);
    void set_position(float x, float y ,float z);
    void set_speed(float x_speed, float y_speed, float z_speed);
    bool tick();
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // SPHERE_H
