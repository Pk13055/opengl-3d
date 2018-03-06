#include "main.h"
#include "prism.h"
#include <vector>

using namespace std;

#define REWARD_TIME 180
#define HEALTH 0
#define AMMO 1

#ifndef REWARD_H
#define REWARD_H

class Reward {
public:
    Reward() {}
    Reward(float x, float z);

    glm::vec3 position, rotation, size;
    color_t color = {0, 255, 233};

    vector<Prism> elements;
    int counter = REWARD_TIME, type = HEALTH;
    bool is_visible = true;

    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void set_dimension(float x, float y, float z);
    void set_rotation(float x_rot, float y_rot, float z_rot);
    void tick();

    bounding_box_t bounding_box();

private:
    VAO *object;
};


#endif // REWARD_H
