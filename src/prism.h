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
    glm::vec3 position, size;
    color_t color;
    float rotation = 0;
    void draw(glm::mat4 VP);
    void set_position(float x, float z);
    void set_dimensions(float width, float height, float depth);
    bounding_box_t bounding_box();

private:
    VAO *object;
};


#endif // PRISM_H
