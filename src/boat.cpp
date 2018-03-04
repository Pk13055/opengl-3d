#include "boat.h"
#include "main.h"
// #include "cube.h"
Boat::Boat(float x, float z, color_t color) {
    this->position = glm::vec3(x, 0, z);
    this->rotation = 0;
    this->jumping = false;
    this->speed = 0;
    this->yspeed = 0;

    // left plank
    this->plank[0] =  Cube(-2, 0,0, 0.2,4,6   ,color);
    // right plank
    this->plank[1] =  Cube( 2, 0,0, 0.2,4,6   ,color);
    // bottom plank 
    this->plank[2] =  Cube( 0, 1,0, 3.8,0.2,6 ,color);
    // back plank
    this->plank[3] =   Cube( 0, 0,3, 3.8,4,0.2 ,color);
    // front head 1
    this->plank[4] =   Cube(1,0, -4 ,0.2,4,4, color);
    this->plank[4].rotation = 30;
    // front head 2
    this->plank[5] =   Cube(-1,0, -4 ,0.2,4,4, color);;
    this->plank[5].rotation = -30;
}

void Boat::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    // No need as coords centered at 0, 0, 0 of Boat arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    // draw3DObject(this->object);
    
    for(int i=0;i<6;i++)
        plank[i].draw(MVP);

}

void Boat::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Boat::tick(){

    this->position.y += yspeed;

    this->position.z += speed*cos(rotation*M_PI/180.0f);   
    this->position.x += speed*sin(rotation*M_PI/180.0f);   

    if(this->position.y > 0)
        yspeed -= 0.003;
    else{
        this->position.y = 0;
        yspeed = 0;
        this->jumping = false;
    }

}

bounding_box_t Boat::bounding_box() {
    float x = this->position.x, y = this->position.y, z = this->position.z;
    // The error in  z is due to the forward pla
    bounding_box_t bbox = { x, y + 2,z - 1.5, 4.0f, 2.0f, 8 };
    return bbox;
}