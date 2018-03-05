#include "main.h"
#include "timer.h"
#include "sea.h"
#include "boat.h"
#include "prism.h"
#include "cube.h"
#include "enemy.h"

#define RAND_COLOR { rand() % 255, rand() % 255, rand() % 255 }
#define Z_ACCLR 0.002f // speed increase
#define X_ACCLR 0.002f // speed increase
#define Y_PAN 5 // degrees


#define NO_ROCKS 200
#define NO_ENEMY 100

#define CINEMA_HEIGHT 30.0f
#define TOWER_X 20.0f
#define TOWER_Z 20.0f

using namespace std;

vector<Prism> rocks;
vector<Enemy> enemies;
Boat boat;
Sea sea;
Cube tower;

int boat_health = 100, score = 0;
int current_view = 0;
// 0 -> camera_follower
// 1 -> camera top view
// 2 -> camera cinema view
bool camera_follower_view = true, camera_top_view = false, camera_cinema_view = false,
sphere_hold = true;
float screen_zoom = 2.0, screen_center_x = 0, screen_center_y = 0, screen_center_z;
float eye_x,eye_y,eye_z;
float target_x, target_y, target_z;
float camera_rotation_angle = 95.0;


GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {


	// All Camera Positions defined here.
	if (camera_follower_view == true){
		target_x = boat.position.x;
		target_y = boat.position.y;
		target_z = boat.position.z;
		camera_rotation_angle = -boat.rotation.y + 95;


		eye_x = target_x + 5*cos(camera_rotation_angle*M_PI/180.0f);
		eye_y = target_y + 5;
		eye_z = target_z + 5*sin(camera_rotation_angle*M_PI/180.0f);

	}

	else if (camera_top_view == true){
		target_x = boat.position.x;
		target_y = boat.position.y;
		target_z = boat.position.z;


		eye_x = target_x + 5*cos(camera_rotation_angle*M_PI/180.0f);
		eye_y = target_y + 25;
		eye_z = target_z + 5*sin(camera_rotation_angle*M_PI/180.0f);

	}


	else if (camera_cinema_view == true) {
		target_x = boat.position.x;
		target_y = boat.position.y;
		target_z = boat.position.z;

		eye_x = TOWER_X;
		eye_y = CINEMA_HEIGHT;
		eye_z = TOWER_Z;

	}


	// clear the color and depth in the frame buffer
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use the loaded shader program
	// Don't change unless you know what you are doing
	glUseProgram (programID);

	// Eye - Location of camera. Don't change unless you are sure!!
	glm::vec3 eye (eye_x,eye_y,eye_z);
	// Target - Where is the camera looking at.  Don't change unless you are sure!!
	glm::vec3 target (target_x, target_y, target_z);
	// Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
	glm::vec3 up (0, 1, 0);

	// Compute Camera matrix (view)
	Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
	// Don't change unless you are sure!!
	// Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

	// Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
	// Don't change unless you are sure!!
	glm::mat4 VP = Matrices.projection * Matrices.view;

	// Send our transformation to the currently bound shader, in the "MVP" uniform
	// For each model you render, since the MVP will be different (at least the M part)
	// Don't change unless you are sure!!
	glm::mat4 MVP;  // MVP = Projection * View * Model

	// Scene render
	sea.draw(VP);
	if (tower.visible) tower.draw(VP);
	for(auto rock: rocks) rock.draw(VP);
	for(auto enemy: enemies) enemy.draw(VP);
	boat.draw(VP);

}

void tick_input(GLFWwindow *window) {

	// Developer keys
	int left  = glfwGetKey(window, GLFW_KEY_LEFT);
	int right = glfwGetKey(window, GLFW_KEY_RIGHT);
	int up    = glfwGetKey(window, GLFW_KEY_UP);
	int down    = glfwGetKey(window, GLFW_KEY_DOWN);

	// Boat movement
	int a   = glfwGetKey(window, GLFW_KEY_A);
	int s   = glfwGetKey(window, GLFW_KEY_S);
	int d   = glfwGetKey(window, GLFW_KEY_D);
	int w   = glfwGetKey(window, GLFW_KEY_W);
	int space = glfwGetKey(window, GLFW_KEY_SPACE);
	// Fireballs
	int f   = glfwGetKey(window, GLFW_KEY_F);

	// Camera Views
	int v = glfwGetKey(window, GLFW_KEY_V);

	if(left) camera_rotation_angle += 1;
	else if(right) camera_rotation_angle -= 1;

	if(w) 						boat.speed.z -= Z_ACCLR;
	else if(s) 					boat.speed.z += Z_ACCLR;
	else if (boat.speed.z < 0) 	boat.speed.z += Z_ACCLR;
	else if (boat.speed.z > 0) 	boat.speed.z -= Z_ACCLR;
	else 						boat.speed.z = 0;
	boat.speed.x = boat.speed.z;


	if(a) 		boat.rotation.y += Y_PAN;
	else if(d) 	boat.rotation.y -= Y_PAN;

	if(v) {
		current_view++;
		current_view = current_view % 3;
		tower.visible = true;
		camera_follower_view = camera_top_view = camera_cinema_view = false;
		switch(current_view) {
			case 0: 	camera_follower_view	= true;
						break;
			case 1: 	camera_top_view 		= true;
						break;
			case 2: 	camera_cinema_view 		= true;
						tower.visible = false;
						break;
			default: 	camera_follower_view 	= true;
						break;
		}
	}

	if(f && boat.weapons.size()){
		Sphere cur_sphere = boat.weapons.back();
		boat.weapons.pop_back();
		sphere_hold = false;
		cur_sphere.speed.z = 0.3f - boat.speed.z;
		cur_sphere.speed.x = 0.05f;
		cur_sphere.rotation = -boat.rotation.y;
		boat.shot.push_back(cur_sphere);
		if(boat.weapons.size()) boat.weapons.back().visible = true;
	}

	if(space && boat.is_jumping == false){
		boat.is_jumping = true;
		boat.speed.y = 0.2f;
	}

}

void tick_elements() {

	sea.tick();
	boat.tick();
	for(auto &enemy: enemies) enemy.tick(boat.position.x, boat.position.z);

	if(boat.weapons.size()) {
		Sphere cur_sphere = boat.weapons.back();
		if(sphere_hold == true) {
			cur_sphere.position.x = boat.position.x;
			cur_sphere.position.y = boat.position.y + 1;
			cur_sphere.position.z = boat.position.z - 1;
		}
		else sphere_hold = cur_sphere.tick();
	}
	for(auto shoot: boat.shot) {
		sphere_hold = shoot.tick();
		if(sphere_hold) shoot.visible = false;
	}
	if(boat.shot.size() && !boat.shot.back().visible) boat.shot.pop_back();

	sea.set_position(boat.position.x,boat.position.z);
}


void collision_function(){

	for(auto rock: rocks)
		if(detect_collision(boat.bounding_box(), rock.bounding_box())) {
			cout<<"Rock collided"<<rand()<<endl;
			// rocks.erase(rock);
		}

}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {

	/* Objects should be created before any other gl function and shaders */

	// Create the models
	boat    	= Boat(0, 0, COLOR_ORANGE);
	sea        	= Sea(0, 0, {119, 214, 255});
	tower = Cube(TOWER_X, CINEMA_HEIGHT / 2.0f, TOWER_Z, 5.0f, CINEMA_HEIGHT, 5.0f, {114, 39, 26});
	for(int i = 0; i < NO_ROCKS; i++) {
		Prism cur_prism = Prism(
			((rand() % 2 )? -1 : 1) * rand() % 200,
			((rand() % 2 )? -1 : 1) * rand() % 200,
			0.4f * (1 + (rand() % 4)),
			0.3f + (rand() % 4),
			0.4f * (1 + (rand() % 4)),
			{102, 102, 102});
		cur_prism.rotation.x = ((rand() % 2)? -1 : 1) * (rand() % 47);
		cur_prism.rotation.z = ((rand() % 2)? -1 : 1) * (rand() % 47);
		rocks.push_back(cur_prism);
	}
	for(int i = 0; i < NO_ENEMY; i++) {
		enemies.push_back(Enemy(
			((rand() % 2 )? -1 : 1) * rand() % 200,
			0.0f,
			((rand() % 2 )? -1 : 1) * rand() % 200,
			0.8f * (1 + (rand() % 4)),
			{112, 12, 178}));
	}



	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
	// Get a handle for our "MVP" uniform
	Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


	reshapeWindow (window, width, height);

	// Background color of the scene
	glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
	glClearDepth (1.0f);

	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LEQUAL);

	cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
	cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
	cout << "VERSION: " << glGetString(GL_VERSION) << endl;
	cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
	srand(time(0));
	int width  = 1600;
	int height = 1600;

	window = initGLFW(width, height);

	initGL (window, width, height);

	/* Draw in loop */
	while (!glfwWindowShouldClose(window)) {
		// Process timers

		if (t60.processTick()) {
			// 60 fps
			// OpenGL Draw commands

			draw();
			// Swap Frame Buffer in double buffering
			glfwSwapBuffers(window);

			// All elements update
			tick_elements();

			// Collision Engine
			collision_function();

			// Take input from user
			tick_input(window);

			reshapeWindow (window, width, height);

		}

		// Poll for Keyboard and mouse events
		glfwPollEvents();
	}

	quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
	return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
		   (abs(a.y - b.y) * 2 < (a.height + b.height)) &&
		   (abs(a.z - b.z) * 2 < (a.depth + b.depth));
}
