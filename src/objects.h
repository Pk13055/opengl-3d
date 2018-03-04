#include "main.h"

#ifndef OBJECTS
#define OBJECTS


class Cube {
public:
    Cube() {}
    Cube(float x, float y,float z,float width , float height , float depth,  color_t color);
    glm::vec3 position;
    glm::vec3 speed;
    glm::vec3 dimensions;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bounding_box_t bounding_box();
private:
    VAO *object;
};


class Prism {
public:
    Prism() {}
    Prism(float x, float z, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float z);
    void tick(float x,float z);
    double speed;
    bounding_box_t bounding_box();

private:
	VAO *object;
};



class Sphere {
public:
    Sphere() {}
    Sphere(float x, float y,float z, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y ,float z);
    bool tick();
    double speed;
    double yspeed;
    float radius;
    bounding_box_t bounding_box();
private:
    VAO *object;
};


#endif // OBJECTS