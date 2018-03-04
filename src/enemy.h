#include "main.h"
#include "prism.h"
#include "cube.h"

#ifndef ENEMY_H
#define ENEMY_H


class Enemy {
public:

    Enemy() {}
    Enemy(float x, float y, float z, float width, float height, color_t color);

    glm::vec3 position, speed, size;
    float rotation = 0;
    bool visible = true;
    color_t color;

    Cube body;
    vector<Prism> spikes;

    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void set_speed(float x_speed, float y_speed, float z_speed);
    void set_dimensions(float width, float height, float depth);

    void tick(float x, float z);
    bounding_box_t bounding_box();
private:
    VAO *object;
};


#endif // ENEMY_H
