#include "main.h"

#ifndef SEA_H
#define SEA_H


class Sea {
public:
    Sea() {}
    Sea(float x, float y, color_t color);
    glm::vec3 position;
    float rotation = 0, yspeed = 0;
    void draw(glm::mat4 VP);
    void set_position(float x, float z);
    void tick();
private:
    VAO *object;
};

#endif // SEA_H
