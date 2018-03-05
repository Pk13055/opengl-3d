#include "main.h"
#include "cube.h"
#include <vector>
using namespace std;

#ifndef PRISM_H
#define PRISM_H


class Prism {
public:
    Prism() {}
    Prism(float x, float z, float width, float height, float depth, color_t color);
    glm::vec3 position, size, rotation;
    color_t color;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void set_dimensions(float width, float height, float depth);
    void set_rotation(float x_rot, float y_rot, float z_rot);
    bounding_box_t bounding_box();

private:
    VAO *object;
};


#endif // PRISM_H
