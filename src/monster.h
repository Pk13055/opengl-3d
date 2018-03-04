#include "main.h"
#include "objects.h"

#ifndef MONSTER_H
#define MONSTER_H

#define num_monsters 20

class Monster {
public:
    Monster() {}
    Monster(color_t color);
    void draw(glm::mat4 VP);
    void tick(float x, float z);
    std::vector<Prism> prism;
    void kill(std::vector <Prism> :: iterator ind);
    
};


#endif // MONSTER_H