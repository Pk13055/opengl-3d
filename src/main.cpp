#include "main.h"
#include "timer.h"
#include "sea.h"
#include "boat.h"
#include "objects.h"
#include "monster.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Monster monsters;
Boat boat;
Sea sea;
Sphere sphere1;

int boat_health = 100,score = 0;
float screen_zoom = 2.0, screen_center_x = 0, screen_center_y = 0, screen_center_z;
float eye_x,eye_y,eye_z;
float target_x,target_y,target_z;
float camera_rotation_angle = 95.0;
bool camera_follower = true,camera_top_view=false;
bool sphere_hold = true;


Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {


	// All Camera Positions defined here.
	if (camera_follower == true){
		target_x = boat.position.x;
		target_y = boat.position.y;
		target_z = boat.position.z;
		camera_rotation_angle = -boat.rotation + 95;


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
	boat.draw(VP);
	sphere1.draw(VP);
	monsters.draw(VP);
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
	int t   = glfwGetKey(window, GLFW_KEY_T);
	int u   = glfwGetKey(window, GLFW_KEY_U);

	if(left){
		camera_rotation_angle +=1;
	}


	if(right){
		camera_rotation_angle -=1;
	}


	// if(up){
	//     sphere1.position.y += 0.01;
	// }


	// if(down){
	//     sphere1.position.y -= 0.01;
	// }


	if(w){
		boat.speed -= 0.001;
	}

	else if(s){
		boat.speed += 0.001;
	}

	else {
		if (boat.speed < 0) {
			boat.speed += 0.001;
		}
		else if (boat.speed > 0) {
			boat.speed -= 0.001;
		}
		else {
			boat.speed = 0;
		}
	}




	if(a){
		boat.rotation +=1;
	}

	if(d){
		boat.rotation -=1;
	}


	if(t){
		camera_top_view = true;
		camera_follower = false;

	}

	if(u){
		camera_follower = true;
		camera_top_view = false;

	}

	if(f){
		sphere_hold = false;
		sphere1.speed = 0.3f - boat.speed;
		sphere1.yspeed = 0.05f;
		sphere1.rotation = -boat.rotation;
	}

	if(space && boat.jumping == false){
		boat.jumping = true;
		boat.yspeed = 0.2f;
	}


}

void tick_elements() {

	monsters.tick(boat.position.x, boat.position.z);
	
	sea.tick();
	boat.tick();

	if(sphere_hold == true)
		{
		sphere1.position.x = boat.position.x;
		sphere1.position.y = boat.position.y + 1;
		sphere1.position.z = boat.position.z - 1;
	}

	else
		sphere_hold = sphere1.tick();

	sea.set_position(boat.position.x,boat.position.z);


}


void collision_function(){

	// if(detect_collision(boat.bounding_box(),sphere1.bounding_box()))
	//     printf("FireBall on the ship\n");
	// else
	//     printf("Fireball flying\n");


	// Check Monster collision with fireball or boat

	vector <Prism> :: iterator it;
	for(it = monsters.prism.begin(); it < monsters.prism.end(); it++)
	{
		if(detect_collision(boat.bounding_box(),it->bounding_box()))
		{   
			boat_health -= 10;
			score += 100;
			monsters.kill(it);
			printf("Monster Attack. Health:%d Score:%d\n",boat_health,score);
		 }

	}


	for(it = monsters.prism.begin(); it < monsters.prism.end(); it++)
	if(detect_collision(sphere1.bounding_box(),it->bounding_box()))
		{
			score += 100;
			printf("Fireball Hit Score:%d\n",score);
			monsters.kill(it);
		}



}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
	/* Objects should be created before any other gl function and shaders */
	// Create the models

	boat       = Boat(0, 0, COLOR_ORANGE);
	
	monsters    = Monster(COLOR_RED);

	sea        = Sea( 0, 0, COLOR_BLUE);
	sphere1     = Sphere( 0, 1,-1, COLOR_BLACK);

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
			if(boat_health < 0)
				break;



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
	// printf("%lf\n", (abs(a.y - b.y) * 2 - (a.height + b.height)));
	return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
		   (abs(a.y - b.y) * 2 < (a.height + b.height)) &&
		   (abs(a.z - b.z) * 2 < (a.depth + b.depth));


}
