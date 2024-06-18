//Shay Liberman 311184527 _ Alexandra Simonishvili 320568843
//Project
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <time.h>
#include"GL/glut.h"

//global constants
#define ANIMATION_DELAY 40
#define PI 3.14
#define ORTHOGRAPHIC_PROJECTION 5
#define PERSPECTIVE_PROJECTION 6

#define SKY_RIGHT 0
#define SKY_LEFT  1
#define SKY_UP    2
#define SKY_DOWN  3
#define SKY_BACK  4
#define SKY_FRONT 5

typedef struct Point {
	float x;
	float y;
	float z;
}Point;


//Glut functions
void drawingCB(void);
void reshapeCB(int width, int height);
void keyboardCB(unsigned char key, int x, int y);
void keyboardSpecialCB(int key, int x, int y);
void TimerCB(int value);
GLubyte* readBMP(char* imagepath, int* width, int* height);
GLuint generateTextures(char* imagepath);
void TerminationErrorFunc(char* ErrorString);
void menuCB(int value);

//Math Functions
void Deg2Vector(float VecDeg, float VecSize, Point* p);
float Deg2Rad(float deg);
void drawPoint(int x, int y);

//Drawing functions
void drawWing();
void drawTriangle();
void drawCylinder();
void drawVane();
void drawWood();
void drawRope();
void drawSeat();
void drawSwing();
void drawWall();
void drawBall();
void drawBouncingBall();
void drawSolarSystem();
void drawSpaceBox();
void drawParkBox();
void drawCarrousel();
void drawPaperPlane();
void drawFlyingPlane();
void drawMap();
void drawInstructions();
void drawSun();

//Texture variables
int shavTextureId[4];
int TextureMode[3];
int swingTextureId[4];
int ballTexture[2];
int wallTexture[2];
int space[6];
int Cube[6];
int parkCube[6];
int carrousel[4];
int sun = 0;
int mapTexture = 0, insTexture = 0;

//Object variables
float RotationAngle = 0, swingAngle = 0, ballDist = 0, flyingAngle = 0;
float lastAngle = 0, swingLastAngle = 0, flyingLastAngle = 0, ballLastDist = 0;

//Camera variables
double cameraX = 24.0, cameraY = 18.0, cameraZ = -35.0;
double camDegX = 73.0, camDegY = 73.0, camDegZ = 17.0;
double cameraUpX = 0.0, cameraUpY = 1.0, cameraUpZ = 0.0, centerX = 0.0, centerY = 0.0, centerZ = 0.0;
int FOVy = 43;
int ProjectionType = PERSPECTIVE_PROJECTION;

//Day/Night cycle 
int nightMode = 1;
double sunDeg = 0, lightW = 0, lightR = 0;

//Texture change
int switcheroo = 0;

//State variables
int cameraCycle = 0; //To cycle through static viewing angel 
int textureFlag = 1, lightFlag = 0, stopMotion = 0; // Turn textures/lights/motion/camera on/off
int light1 = 0, light23 = 0, light4 = 0, light5 = 0, light6 = 0; //Control specific lights, Light23 is for day/night light


Point A = { 0,0,0 }, B = { 0,0,0 }, C = { 0,0 ,0 }; // Stores the coordinates for triangle
int shavrotate = 1, swingdir = 1, balldir = 1, sunrotate = 1;
double wallD = 0;
double ballR = 1;
double  sp = 20;
Point cam;

int main(int argc, char** argv)
{

	//initizlizing GLUT
	glutInit(&argc, argv);

	//initializing window
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(900, 100);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow("3D world");

	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	//Light 2+3 for day/night
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);


	//registering callbacks
	glutDisplayFunc(drawingCB);
	glutReshapeFunc(reshapeCB);
	glutKeyboardFunc(keyboardCB);
	glutSpecialFunc(keyboardSpecialCB);
	glutTimerFunc(ANIMATION_DELAY, TimerCB, 0);

	//specify textures and setting Specific texture parameters
	mapTexture = generateTextures("Textures/park-map.bmp");
	insTexture = generateTextures("Textures/instructions.bmp");
	sun = generateTextures("Textures/sun5.bmp");

	shavTextureId[0] = generateTextures("Textures/wood2.bmp");
	shavTextureId[1] = generateTextures("Textures/silver.bmp");
	shavTextureId[2] = generateTextures("Textures/paper.bmp");
	shavTextureId[3] = generateTextures("Textures/paper2.bmp"); //2nd Texture Option for windmill

	swingTextureId[0] = generateTextures("Textures/wood2.bmp");
	swingTextureId[1] = generateTextures("Textures/rope.bmp");
	swingTextureId[2] = generateTextures("Textures/wood.bmp");
	swingTextureId[3] = generateTextures("Textures/wood3.bmp"); //2nd Texture Option for swing

	wallTexture[0] = generateTextures("Textures/brick.bmp");
	ballTexture[0] = generateTextures("Textures/football.bmp");
	wallTexture[1] = generateTextures("Textures/brick_gr.bmp");	//2nd Texture Option for wall
	ballTexture[1] = generateTextures("Textures/basketball.bmp"); //2nd Texture Option for ball

	space[0] = generateTextures("Textures/sun.bmp");
	space[1] = generateTextures("Textures/earth.bmp");
	space[2] = generateTextures("Textures/moon.bmp");
	space[3] = generateTextures("Textures/sun2.bmp"); //2nd Texture Option
	space[4] = generateTextures("Textures/earth2.bmp");//2nd Texture Option 
	space[5] = generateTextures("Textures/moon2.bmp"); //2nd Texture Option

	carrousel[1] = generateTextures("Textures/lines.bmp");
	carrousel[2] = generateTextures("Textures/lego.bmp"); //2nd Texture Option for carrousel's top

	parkCube[SKY_RIGHT] = generateTextures("Textures/1_ny.bmp");
	parkCube[SKY_LEFT] = generateTextures("Textures/1_py.bmp");
	parkCube[SKY_UP] = generateTextures("Textures/1_pz.bmp");
	parkCube[SKY_DOWN] = generateTextures("Textures/1_nz.bmp");
	parkCube[SKY_BACK] = generateTextures("Textures/1_px.bmp");
	parkCube[SKY_FRONT] = generateTextures("Textures/1_nx.bmp");


	//Menus
	//Lights submenu
	int lightMenu = glutCreateMenu(menuCB);
	glutAddMenuEntry("nightMode", 27);
	glutAddMenuEntry("On/Off", 28);
	glutAddMenuEntry("Light1", 1);
	glutAddMenuEntry("Light2", 2);
	glutAddMenuEntry("Light3", 3);
	glutAddMenuEntry("Light4", 4);
	glutAddMenuEntry("Light5", 5);

	//Textures submenu
	int textureMenu = glutCreateMenu(menuCB);
	glutAddMenuEntry("On/Off", 24);
	glutAddMenuEntry("Switcheroo", 25);

	int mainMenu = glutCreateMenu(menuCB);
	glutAddSubMenu("Lights", lightMenu);
	glutAddSubMenu("Textures", textureMenu);
	glutAddMenuEntry("Animation On/Off", 26);
	glutAddMenuEntry("Exit", 30);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	TextureMode[0] = GL_OBJECT_LINEAR;
	TextureMode[1] = GL_EYE_LINEAR;
	TextureMode[2] = GL_SPHERE_MAP;

	//setting Global texture parameters
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	//with shading modes. Can be: GL_MODULATE or GL_REPLACE 

	//starting main loop
	glutMainLoop();
}

void drawingCB(void)
{
	GLenum er;

	//Lighting
	//Ligh0 - (1,0,0,1) for red light
	GLfloat light_0_diffuse[] = { 0.6, 0.0, 0.0, 1.0 };
	GLfloat light_0_position[] = { 0, 0, 1.5, 1 }; //

	//Light1 - (0,1,0,1) for green light 
	GLfloat light_1_diffuse[] = { 0.0, 0.8, 0.0, 1 };
	GLfloat light_1_position[] = { 0, 0, 0, 1 };

	//Ligh2 - white light (1,1,1) - day-night off
	GLfloat light_2_specular[] = { 1,1,1, 1 };
	GLfloat light_2_diffuse[] = { 1,1,1, 1 };
	GLfloat light_2_position[] = { 15, 0, 0, 0 }; //

	//Ligh3 - The "Sun" for day cycle, white light (1,1,1)
	GLfloat light_3_specular[] = { lightR, lightW, lightW, 1 };
	GLfloat light_3_diffuse[] = { lightW, lightW, lightW, 1 };
	GLfloat light_3_position[] = { 28, 0, 0, 0 }; //

	//light4 - (0,0,1,1) for blue light
	GLfloat light_4_diffuse[] = { 0,0,1, 1 };
	GLfloat light_4_position[] = { 0, 3, 0, 1 }; //

	//light5 - (0,0,1,1) for yellow light
	GLfloat light_5_diffuse[] = { 0.8,0.8,0.4, 1 };
	GLfloat light_5_specular[] = { 0.8, 0.8, 0.4, 1.0 };
	GLfloat light_5_position[] = { 3.8, 0, 0, 1 };

	//light6 - for background coloring
	GLfloat light_6_ambient[] = { 0.6,0.6,0.6 ,1 };
	GLfloat light_6_position[] = { 0, 0, 0, 1 };

	//clearing the background
	glClearColor(0.5, 0.5, 0.5, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Lightnings properties
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_0_diffuse);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_1_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_2_specular);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_2_diffuse);
	glLightfv(GL_LIGHT3, GL_SPECULAR, light_3_specular);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, light_3_diffuse);
	glLightfv(GL_LIGHT4, GL_DIFFUSE, light_4_diffuse);
	glLightfv(GL_LIGHT5, GL_SPECULAR, light_5_specular);
	glLightfv(GL_LIGHT5, GL_DIFFUSE, light_5_diffuse);
	glLightfv(GL_LIGHT6, GL_AMBIENT, light_6_ambient);

	//Material properties
	GLfloat mat_ambient[] = { 0.24725,0.1995,0.0745,1.0 };
	GLfloat mat_diffuse[] = { 0.75164,0.60648,0.22648,1.0 };
	GLfloat mat_specular[] = { 0.628281,0.555802,0.366065,1.0 };
	GLfloat mat_shininess[] = { 51.2 };
	GLfloat default_diffuse[] = { 1.0, 1.0, 1.0, 1.0 }; // Default color
	glMaterialfv(GL_FRONT, GL_DIFFUSE, default_diffuse);

	//initializing modelview transformation matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//set background light
	glLightfv(GL_LIGHT6, GL_POSITION, light_6_position);

	//set Camera:
	//use motion degree for xy coordinates of circle motion
	Deg2Vector(camDegX, FOVy, &cam);
	cameraX = cam.x;
	cameraZ = cam.y;
	//use center of object observed, and set camera direction
	gluLookAt(cameraX + centerX, cameraY, cameraZ + centerZ, centerX, centerY, centerZ, cameraUpX, cameraUpY, cameraUpZ);

	//create the world
	drawParkBox();

	//draw the sun
	glPushMatrix();
	if (nightMode) {	//set lightning for day-night light
		glRotated(sunDeg, 0, 0, 1); //make sun and light rotation diynamic acording sun degree
		glLightfv(GL_LIGHT3, GL_POSITION, light_3_position); //{ sunX, sunY, 0, 0 };
	}
	else {				//set lightning for static light
		glRotated(90, 0, 0, 1); //make sun and light static degree above the world
		glLightfv(GL_LIGHT2, GL_POSITION, light_2_position); //{ sunX, sunY, 0, 0 };
	}
	//move sun from the world center
	glTranslated(30, 0, 0);
	drawSun();
	glPopMatrix();

	glPushMatrix();//added another PushMatrix. Otherwise console spams Error 1284 

	//show park map
	glPushMatrix();
	glTranslated(10, 2, 7);		//move park model due to park
	glPushMatrix();
	glTranslated(0, 8, 7);	//place yellow light for park map
	glLightfv(GL_LIGHT5, GL_POSITION, light_5_position);	//add yellow light
	glPopMatrix();
	glRotated(180, 0, 1, 0);	//rotate map direction
	glScaled(0.5, 0.5, 0.5);	//adjust map size
	drawMap();
	glPopMatrix();

	//show instructions
	glPushMatrix();
	glTranslated(-1, 0, 1);		//move instructions floor a bit from center
	glRotated(180, 0, 1, 0);	//rotate instructions
	glScaled(1.8, 1, 1.8);		//increase instructions floor size
	drawInstructions();
	glPopMatrix();

	//create the vane
	glPushMatrix();
	glTranslated(3, 0, -10);	//move the vane in park
	glPushMatrix();
	glTranslated(0, 4, -3);		//move light near the vane
	glLightfv(GL_LIGHT0, GL_POSITION, light_0_position);	//add red light
	glPopMatrix();
	drawVane();
	glPopMatrix();

	//create the swing
	glPushMatrix();
	glRotated(15, 0, 1, 0);		//rotate the swing a bit
	glTranslated(0, 0, -10);	//move the swing in park
	glLightfv(GL_LIGHT4, GL_POSITION, light_4_position);	//add blue light
	drawSwing();
	glPopMatrix();

	//create the bouncing Ball
	glPushMatrix();
	glTranslated(-10, 0, 10);	//move the playground in park
	glRotated(100, 0, 1, 0);	//rotate the playground around itself
	glPushMatrix();
	glTranslated(5, 6, 0);		//move first light
	glLightfv(GL_LIGHT0, GL_POSITION, light_0_position);	//add red light
	glPopMatrix();
	glPushMatrix();
	glTranslated(-5, 6, 0);		//move second light
	glLightfv(GL_LIGHT1, GL_POSITION, light_1_position);	//add green light
	glPopMatrix();
	glScaled(0.8, 0.8, 0.8);	//decrease the playground size
	drawBouncingBall();
	glPopMatrix();

	//create the solar system
	glPushMatrix();
	glRotated(-150, 0, 1, 0);	//rotate the mogel coordinates
	glTranslated(-14, 3.1, 0);	//move the model in the park
	glPushMatrix();				//set light movement angle
	if (sunrotate == 1) glRotated((GLdouble)RotationAngle, 0.0, 1.0, 0.0);
	else glRotated((GLdouble)lastAngle, 0.0, 0.0, 1.0);
	glLightfv(GL_LIGHT5, GL_POSITION, light_5_position);	//add yellow light
	glPopMatrix();
	drawSolarSystem();
	glPopMatrix();

	//create the carrousel
	glPushMatrix();
	glRotated(-20, 0, 1, 0);	//rotate the carrousel coordinates
	glTranslated(-12, 0, 0);	//move the carrousel in the park
	glLightfv(GL_LIGHT4, GL_POSITION, light_4_position);	//add blue light
	drawCarrousel();
	glPopMatrix();

	//create the paper plane
	glPushMatrix();
	glRotated(15, 0, 1, 0);		//rotate the planes coordinates
	glTranslated(0, 1, 20);		//move the planes in the park
	glRotated(-80, 0, 1, 0);	//rotate planes direction
	glScaled(0.8, 0.8, 0.8);	//decrease planes size
	drawFlyingPlane();
	glPopMatrix();

	printf("nightMode: %d, cameraX: %.2f, cameraY: %.2f, cameraZ: %.2f, FOVy: %d \n", nightMode, cameraX, cameraY, cameraZ, FOVy);

	//swapping buffers and displaying
	glutSwapBuffers();

	//check for errors
	er = glGetError();  //get errors. 0 for no error, find the error codes in: https://www.opengl.org/wiki/OpenGL_Error
	if (er) printf("error: %d\n", er);
}

void reshapeCB(int width, int height)
{
	float left, right, bottom, top, zNear, zFar;

	//define our ortho
	left = -2; right = 2;
	bottom = -2; top = 2;
	zNear = 0.1; zFar = 400;

	//update viewport
	glViewport(0, 0, width, height);

	//clear the transformation matrices (load identity)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//projection or gluOrtho2D
	switch (ProjectionType) {
	case ORTHOGRAPHIC_PROJECTION:
		glOrtho(-5, 5, -5, 5, zNear, zFar);
		break;
	case PERSPECTIVE_PROJECTION:
		gluPerspective(FOVy, 1, zNear, zFar);
		break;
	}
}

void menuCB(int value) {
	switch (value) {

	case 24:	 //add texture flag
		textureFlag = !textureFlag;
		if (textureFlag) {
			glEnable(GL_TEXTURE_2D);
		}
		else {
			glDisable(GL_TEXTURE_2D);
		}
		break;

	case 25:	//Texture switch
		switcheroo = !switcheroo;
		break;

	case 26:	//motion controll
		stopMotion = !stopMotion;
		break;

	case 27:	//day-night controll, day light enabel\disabel
		nightMode = !nightMode;
		if (nightMode) {
			glDisable(GL_LIGHT2);
			glEnable(GL_LIGHT3);
		}
		else {
			glDisable(GL_LIGHT3);
			glEnable(GL_LIGHT2);
		}
		break;

	case 28:	//all light controll
		lightFlag = !lightFlag;
		if (lightFlag) {
			glEnable(GL_LIGHTING);
		}
		else {
			glDisable(GL_LIGHTING);
		}
		break;

	case 1:		//add red & green light
		light1 = !light1;
		if (light1) {
			glEnable(GL_LIGHT1);
			glEnable(GL_LIGHT0);
		}
		else {
			glDisable(GL_LIGHT1);
			glDisable(GL_LIGHT0);
		}
		break;
	case 2:		//add blue light
		light4 = !light4;
		if (light4)glEnable(GL_LIGHT4);
		else glDisable(GL_LIGHT4);
		break;
	case 3:		//add yellow light
		light5 = !light5;
		if (light5)glEnable(GL_LIGHT5);
		else glDisable(GL_LIGHT5);
		break;
	case 4:		//add background light
		light6 = !light6;
		if (light6)glEnable(GL_LIGHT6);
		else glDisable(GL_LIGHT6);
		break;
	case 5:		//add day light
		light23 = !light23;
		if (light23) {
			glEnable(GL_LIGHT2);
			glEnable(GL_LIGHT3);
		}
		else {
			glDisable(GL_LIGHT2);
			glDisable(GL_LIGHT3);
		}
		break;

	case 30:	//finish
		exit(1);
	default: break;
	}
	glutPostRedisplay();
}

void keyboardCB(unsigned char key, int x, int y) {

	switch (key) {
	case 27:
		exit(0);
		break;
	case 'a': //stop animation motion
		stopMotion = !stopMotion;
		break;
	case 's': //Switch textures
		switcheroo = !switcheroo;
		break;

	case '+': //zoom in - encrease cam movement radius
		FOVy = FOVy - 1;
		reshapeCB(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		break;
	case '-': //zoom out - decrease cam movement radius
		FOVy = FOVy + 1;
		reshapeCB(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		break;

	case 'l':	//controll all light
		lightFlag = !lightFlag;
		if (lightFlag) {
			glEnable(GL_LIGHTING);
		}
		else {
			glDisable(GL_LIGHTING);
		}
		break;

	case 't':	//add texture
		textureFlag = !textureFlag;
		if (textureFlag) {
			glEnable(GL_TEXTURE_2D);
		}
		else {
			glDisable(GL_TEXTURE_2D);
		}
		break;

		//Set camera to look at the project's models
		//Cycle through the models by pressing c, add figures center coordinates
	case 'c':
		switch (cameraCycle) {
		case 0:
			//Bouncing ball
			FOVy = 25;
			cameraX = 14.0;
			cameraY = 8.0;
			cameraZ = 4.0;
			centerX = -20.0;
			centerX = -12;
			centerY = 3.0;
			centerZ = 9.5;
			centerZ = 8.3;
			break;

		case 1:
			//Carrousel
			FOVy = 22;
			cameraX = 5.3;
			cameraY = 3;
			cameraZ = -1.0;
			centerX = -25;
			centerX = -11.3;
			centerY = 3;
			centerZ = -6.5;
			centerZ = -4.1;
			break;

		case 2:
			//Swing
			FOVy = 21;
			cameraY = 2.5;
			centerX = -2.6;
			centerY = 3.0;
			centerZ = -9.6;
			break;

		case 3:
			//Windmill
			FOVy = 25;
			cameraY = 4.0;
			centerX = 3;
			centerY = 5.2;
			centerZ = -10;
			break;

		case 4:
			//Airplanes
			FOVy = 26;
			cameraY = 4.0;
			centerX = 5;
			centerY = 2.0;
			centerZ = 19.7;
			break;

		case 5:
			//Sun, Earth, Moon
			FOVy = 25;
			cameraY = 4.0;
			centerX = 12;
			centerY = 3.0;
			centerZ = -7;
			break;

		case 6:
			//map
			FOVy = 24;
			cameraY = 4.0;
			centerX = 14;
			centerY = 6.0;
			centerZ = 7;
			break;
		}
		cameraCycle++;
		if (cameraCycle == 7) cameraCycle = 0;
		break;

	case '0': //Default view, update default coordinates
		ProjectionType = PERSPECTIVE_PROJECTION;
		cameraUpX = 0.0;
		cameraUpY = 1.0;
		cameraUpZ = 0.0;
		cameraX = 24.0;
		cameraY = 18.0;
		cameraZ = -35.0;
		centerX = 0.0;
		centerY = 0.0;
		centerZ = 0.0;
		FOVy = 43.0;
		camDegX = 73.0;
		break;

		//Light controls
	case 'n':	//day-night light
		nightMode = !nightMode;
		if (nightMode) {
			glDisable(GL_LIGHT2);
			glEnable(GL_LIGHT3);
		}
		else {
			glDisable(GL_LIGHT3);
			glEnable(GL_LIGHT2);
		}
		break;

	case '1':	//green & red light 
		light1 = !light1;
		if (light1) {
			glEnable(GL_LIGHT1);
			glEnable(GL_LIGHT0);
		}
		else {
			glDisable(GL_LIGHT1);
			glDisable(GL_LIGHT0);
		}
		break;

	case '2':	//blue light
		light4 = !light4;
		if (light4)glEnable(GL_LIGHT4);
		else glDisable(GL_LIGHT4);
		break;
	case '3':	//yello light
		light5 = !light5;
		if (light5)glEnable(GL_LIGHT5);
		else glDisable(GL_LIGHT5);
		break;
	case '4':	//background light
		light6 = !light6;
		if (light6)glEnable(GL_LIGHT6);
		else glDisable(GL_LIGHT6);
		break;
	case '5':	//day light
		light23 = !light23;
		if (light23)glEnable(GL_LIGHT2), glEnable(GL_LIGHT3);
		else glDisable(GL_LIGHT2), glDisable(GL_LIGHT3);
		break;
	}
	reshapeCB(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	glutPostRedisplay();
}

void keyboardSpecialCB(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:	//move left in circle around the center 
		camDegX += 2.0;	//encrease degree
		if (camDegX >= 360) camDegX = 0;	//update degree
		Deg2Vector(camDegX, FOVy, &cam);
		cameraX = cam.x;
		cameraZ = cam.y;
		break;

	case GLUT_KEY_RIGHT:	//move right in circle around the center 
		camDegX -= 2.0;	//decrease degree
		if (camDegX <= 0) camDegX = 360;	//update degree
		Deg2Vector(camDegX, FOVy, &cam);
		cameraX = cam.x;
		cameraZ = cam.y;
		break;

	case GLUT_KEY_UP:	//move up
		cameraY += 0.35;
		break;

	case GLUT_KEY_DOWN:	//move down
		cameraY -= 0.35;
		break;
	}
	glutPostRedisplay();
}

void TimerCB(int value)
{
	if (!stopMotion) {		//motion flag controled

		RotationAngle = (RotationAngle + 2);		//for sun, carrousel, vane rotations
		if (RotationAngle >= 360)	//reset to 0
			RotationAngle = 0;

		swingAngle = (swingAngle + 2 * swingdir);	//for swing move

		if (swingAngle >= 45)
			swingdir = -swingdir;
		else if (swingAngle <= -45)
			swingdir = -swingdir;

		ballDist = (ballDist + 0.4 * balldir);		//for ball rolling
		if (ballDist >= wallD - 0.25)
			balldir = -balldir;
		else if (ballDist <= -wallD + 0.25)
			balldir = -balldir;

		flyingAngle = (flyingAngle + 2 + sp);		//for plane flying changings
		if (flyingAngle >= 360) {
			flyingAngle = 0;
			sp = 20;
		}
		if (sp <= 0) sp = 0;
		else {
			sp -= 1.1;
		}

		sunDeg = (sunDeg + 0.1);					//for sun and daylight move and light intensivity
		if (sunDeg >= 360)
			sunDeg = 0;
		if (sunDeg >= 0 && sunDeg < 20) {
			lightW += 0.005;	//more light
			lightR += 0.01;
		}
		if (sunDeg >= 160 && sunDeg < 180) {
			lightW -= 0.005;	//less light
			if (sunDeg >= 170)
				lightR -= 0.01;
		}


	}
	glutTimerFunc(ANIMATION_DELAY, TimerCB, value);
	glutPostRedisplay();
}

void drawMap() {
	int D = 8, d = 0;

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, mapTexture); //add texture to map 
	glTranslated(0, 8, 0);		//move map above the world ground
	glBegin(GL_QUADS);			//create map square 
	glTexCoord2f(0, 0); glVertex3f(-D + d, -D, +D);
	glTexCoord2f(1, 0); glVertex3f(-D + d, -D, -D);
	glTexCoord2f(1, 1); glVertex3f(-D + d, +D, -D);
	glTexCoord2f(0, 1); glVertex3f(-D + d, +D, +D);
	glEnd();
	glPopMatrix();
	glPushMatrix();
	glTranslated(-8, -4, -8);	//move map legs according the map itself
	glRotated(-90, 1, 0, 0);	//rotate map legs vertical
	glScaled(2, 2, 4);			//encrease legs size
	drawCylinder();
	glPopMatrix();
	glPushMatrix();
	glTranslated(-8, -4, 8);	//move map legs according the map itself
	glRotated(-90, 1, 0, 0);	//rotate map legs vertical
	glScaled(2, 2, 4);			//encrease legs size
	drawCylinder();
	glPopMatrix();

}

void drawInstructions() {
	int D = 8, d = 0;

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glPushMatrix();
	//up
	glBindTexture(GL_TEXTURE_2D, insTexture);	//add texture to instructions floor
	glTranslated(0, -7.9, 0);	//place instructions floor above the world ground
	glBegin(GL_QUADS);			//create instructions square 
	glTexCoord2f(1, 0); glVertex3f(-D, +D - d, -D);
	glTexCoord2f(0, 0); glVertex3f(+D, +D - d, -D);
	glTexCoord2f(0, 1); glVertex3f(+D, +D - d, +D);
	glTexCoord2f(1, 1); glVertex3f(-D, +D - d, +D);
	glEnd();
	glPopMatrix();
}

void drawWing() {
	int n, k;
	float r = 1, h = 0;
	A.x = 0, A.y = 2, A.z = 0;
	Point dot[20];
	for (n = 0; n < 20; n++) {
		Deg2Vector((n + 1.0) * 360.0 / 20, r, &dot[n]);
	}
	for (n = 19; n > 13; n--) {
		B.x = dot[n].x, B.y = h, B.z = dot[n].y;
		C.x = dot[n - 1].x, C.y = h, C.z = dot[n - 1].y;
		drawTriangle(0.0, 0.0, 0.5);	//add figure using triangles
	}
}

void drawTriangle()
{
	glBegin(GL_LINE_LOOP);
	glVertex3d(A.x, A.y, A.z);
	glVertex3d(B.x, B.y, B.z);
	glVertex3d(C.x, C.y, C.z);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3d(A.x, A.y, A.z);
	glVertex3d(B.x, B.y, B.z);
	glVertex3d(C.x, C.y, C.z);
	glEnd();
}

void drawCylinder() {
	GLUquadricObj* quadric = gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluQuadricTexture(quadric, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D, shavTextureId[1]);
	glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, TextureMode[0]);
	glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, TextureMode[0]);
	gluCylinder(quadric, 0.05, 0.05, 5.0, 20, 20); // base radius, top radius, height, slices, stacks
	gluDeleteQuadric(quadric);
}

void drawVane() {
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glPushMatrix();
	glRotated(-90, 1.0, 0.0, 0.0);
	drawCylinder(1, 0, 1); //the leg
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 5.25, 0);	//move up to draw the vane itself
	if (shavrotate == 1) glRotated((GLdouble)RotationAngle, 0.0, 0.0, 1.0);		//add rotation
	else glRotated((GLdouble)lastAngle, 0.0, 0.0, 1.0);
	glPushMatrix();

	//add texture
	if (textureFlag == 1) {
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glBindTexture(GL_TEXTURE_2D, shavTextureId[1]);
		glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, TextureMode[2]);
		glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, TextureMode[2]);

	}
	glutSolidSphere(0.25, 10, 15); //the center
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glPopMatrix();
	//add texture
	glPushMatrix();
	if (textureFlag == 1) {
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		if (switcheroo)glBindTexture(GL_TEXTURE_2D, shavTextureId[3]);
		else glBindTexture(GL_TEXTURE_2D, shavTextureId[2]);
		glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, TextureMode[0]);
		glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, TextureMode[0]);
	}
	for (int i = 0; i < 6; i++) {
		glPushMatrix();
		glRotated((GLdouble)60 * i, 0.0, 0.0, 1.0);		//change angles to past wings
		glTranslatef(0, -2.25, 0);
		drawWing(1, 0, 1);	//draw the vane wings
		glPopMatrix();
	}
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glPopMatrix();
	glPopMatrix();
}

void drawSun() {
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	GLUquadricObj* pObj = gluNewQuadric(); //creates new quadric 
	glBindTexture(GL_TEXTURE_2D, sun);	//add sun texture
	gluQuadricTexture(pObj, GL_TRUE);
	gluSphere(pObj, 1, 100, 100);
	gluDeleteQuadric(pObj);
}

void drawWood() {
	GLUquadricObj* quadric = gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	glBindTexture(GL_TEXTURE_2D, swingTextureId[0]);	//add wood texture
	gluQuadricTexture(quadric, GL_TRUE);
	glRotated(45, 0, 0, 1);
	gluCylinder(quadric, 0.1, 0.1, 5.0, 4, 20); // base radius, top radius, height, slices, stacks
	gluDeleteQuadric(quadric);
}

void drawRope() {
	GLUquadricObj* quadric = gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluQuadricTexture(quadric, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D, swingTextureId[1]);	//add rope texture
	gluCylinder(quadric, 0.02, 0.02, 4.0, 6, 20); // base radius, top radius, height, slices, stacks
	gluDeleteQuadric(quadric);
}

void drawSeat() {
	GLUquadricObj* quadric = gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluQuadricTexture(quadric, GL_TRUE);
	double D = 1, d = 1;
	if (switcheroo)glBindTexture(GL_TEXTURE_2D, swingTextureId[3]);	//add wood texture for seat
	else glBindTexture(GL_TEXTURE_2D, swingTextureId[2]);
	glPushMatrix();
	glRotated(90, 0.0, 1.0, 0.0);
	glTranslatef(0, -4, 0);
	glScaled(0.4, 0.05, 1);
	//draw seat
	//left	
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-D, -D, -D + d);
	glTexCoord2f(1, 0); glVertex3f(+D, -D, -D + d);
	glTexCoord2f(1, 1); glVertex3f(+D, +D, -D + d);
	glTexCoord2f(0, 1); glVertex3f(-D, +D, -D + d);
	glEnd();
	d = -1;
	//right
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(+D, -D, +D - d);
	glTexCoord2f(1, 0); glVertex3f(-D, -D, +D - d);
	glTexCoord2f(1, 1); glVertex3f(-D, +D, +D - d);
	glTexCoord2f(0, 1); glVertex3f(+D, +D, +D - d);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glRotated(90, 0.0, 1.0, 0.0);
	glTranslatef(0, -4, 0);
	glScaled(4, 0.5, 1);
	glRotated(45, 0, 0, 1);
	gluCylinder(quadric, 0.15, 0.15, 2.0, 4, 20); // base radius, top radius, height, slices, stacks
	glPopMatrix();
	gluDeleteQuadric(quadric);
}

void drawSwing() {
	double h, d;
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glPushMatrix();
	{
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
	}
	d = (GLfloat)cos(Deg2Rad(70)) * 5.0;
	h = (GLfloat)sin(Deg2Rad(70)) * 5.0;
	//draw the swing from its parts moving objects in space
	glPushMatrix();
	glTranslatef(2.5, 0, -d);
	glRotated((GLdouble)-70, 1.0, 0.0, 0.0);
	drawWood(1, 0, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.5, 0, -d);
	glRotated((GLdouble)-70, 1.0, 0.0, 0.0);
	drawWood(1, 0, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(2.5, h, 0);
	glRotated((GLdouble)70, 1.0, 0.0, 0.0);
	drawWood(1, 0, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.5, h, 0);
	glRotated((GLdouble)70, 1.0, 0.0, 0.0);
	drawWood(1, 0, 1);
	glPopMatrix();

	glPushMatrix();
	glRotated((GLdouble)90, 0.0, 1.0, 0.0);
	glTranslatef(0, h, -2.5);
	drawWood(1, 0, 1);
	glPopMatrix();

	glPopMatrix();


	glPushMatrix();
	glPushMatrix();
	glTranslatef(0, h, 0);
	if (shavrotate == 1) glRotated((GLdouble)swingAngle + 90, 1.0, 0.0, 0.0);	//for movement
	else glRotated((GLdouble)swingLastAngle + 90, 1.0, 0.0, 0.0);
	glPushMatrix();
	glTranslatef(1, 0, 0);
	drawRope(1, 0, 1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-1, 0, 0);
	drawRope(1, 0, 1);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glPushMatrix();
	glTranslatef(-1, h, 0);
	if (shavrotate == 1) glRotated((GLdouble)swingAngle, 1.0, 0.0, 0.0);	//for movement
	else glRotated((GLdouble)swingLastAngle, 1.0, 0.0, 0.0);
	drawSeat(1, 0, 1);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glPopMatrix();
	glPopMatrix();
}

void drawWall() {
	double D = 6, d = 0;
	glPushMatrix();
	wallD = D - ballR * 2 / 3;
	glPushMatrix();
	glScaled(0.05, 0.5, 1);
	//draw the wall as cube and reshape it
	//right
	if (switcheroo)glBindTexture(GL_TEXTURE_2D, wallTexture[1]);	//add wall texture
	else glBindTexture(GL_TEXTURE_2D, wallTexture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(+D - d, -D, -D);
	glTexCoord2f(1, 0); glVertex3f(+D - d, -D, +D);
	glTexCoord2f(1, 1); glVertex3f(+D - d, +D, +D);
	glTexCoord2f(0, 1); glVertex3f(+D - d, +D, -D);
	glEnd();
	//left
	if (switcheroo)glBindTexture(GL_TEXTURE_2D, wallTexture[1]);
	else glBindTexture(GL_TEXTURE_2D, wallTexture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-D + d, -D, +D);
	glTexCoord2f(1, 0); glVertex3f(-D + d, -D, -D);
	glTexCoord2f(1, 1); glVertex3f(-D + d, +D, -D);
	glTexCoord2f(0, 1); glVertex3f(-D + d, +D, +D);
	glEnd();
	//up
	if (switcheroo)glBindTexture(GL_TEXTURE_2D, wallTexture[1]);
	else glBindTexture(GL_TEXTURE_2D, wallTexture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-D, +D - d, -D);
	glTexCoord2f(1, 0); glVertex3f(+D, +D - d, -D);
	glTexCoord2f(1, 1); glVertex3f(+D, +D - d, +D);
	glTexCoord2f(0, 1); glVertex3f(-D, +D - d, +D);
	glEnd();
	//down
	if (switcheroo)glBindTexture(GL_TEXTURE_2D, wallTexture[1]);
	else glBindTexture(GL_TEXTURE_2D, wallTexture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-D, -D + d, +D);
	glTexCoord2f(1, 0); glVertex3f(+D, -D + d, +D);
	glTexCoord2f(1, 1); glVertex3f(+D, -D + d, -D);
	glTexCoord2f(0, 1); glVertex3f(-D, -D + d, -D);
	glEnd();
	//back
	if (switcheroo)glBindTexture(GL_TEXTURE_2D, wallTexture[1]);
	else glBindTexture(GL_TEXTURE_2D, wallTexture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-D, -D, -D + d);
	glTexCoord2f(1, 0); glVertex3f(+D, -D, -D + d);
	glTexCoord2f(1, 1); glVertex3f(+D, +D, -D + d);
	glTexCoord2f(0, 1); glVertex3f(-D, +D, -D + d);
	glEnd();
	//front
	if (switcheroo)glBindTexture(GL_TEXTURE_2D, wallTexture[1]);
	else glBindTexture(GL_TEXTURE_2D, wallTexture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(+D, -D, +D - d);
	glTexCoord2f(1, 0); glVertex3f(-D, -D, +D - d);
	glTexCoord2f(1, 1); glVertex3f(-D, +D, +D - d);
	glTexCoord2f(0, 1); glVertex3f(+D, +D, +D - d);
	glEnd();
	glPopMatrix();
	glPopMatrix();
}

void drawBall() {
	ballR = 1;
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	GLUquadricObj* quadric = gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);

	if (switcheroo)glBindTexture(GL_TEXTURE_2D, ballTexture[1]);	//add ball texture
	else glBindTexture(GL_TEXTURE_2D, ballTexture[0]);
	gluQuadricTexture(quadric, GL_TRUE);

	double delt;
	if (ballDist > 0) delt = 6.0 - ballDist;	//set move distance udjustments
	else delt = -(-6 - ballDist);
	glPushMatrix();
	if (shavrotate == 1) glTranslated(ballDist, 0, 0);	//add movement changing X coordinates
	else glTranslated(ballLastDist, 0, 0);
	if (delt <= ballR) {	//make the ball "pop" 
		glScaled(delt / ballR, ballR / delt, ballR / delt);
	}
	gluSphere(quadric, ballR, 30, 30);
	gluDeleteQuadric(quadric);
	glPopMatrix();
}

void drawBouncingBall() {
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glPushMatrix();
	glPushMatrix();
	glTranslated(0, 3, 0);	//place walls above the ground
	glPushMatrix();
	glTranslated(6.3, 0, 0);	//placing wall in space
	drawWall();
	glPopMatrix();
	glPushMatrix();
	glTranslated(-6.5, 0, 0);	//placing wall in space
	drawWall();
	glPopMatrix();
	glPopMatrix();
	glTranslated(0, ballR, 0);	//place ball above the ground
	drawBall();
	glPopMatrix();
}

void drawSolarSystem() {
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glPushMatrix();
	GLUquadricObj* pObj = gluNewQuadric(); //creates new quadric 
	if (switcheroo)glBindTexture(GL_TEXTURE_2D, space[3]);	//add sun texture
	else glBindTexture(GL_TEXTURE_2D, space[0]);
	gluQuadricTexture(pObj, GL_TRUE);
	if (sunrotate == 1) glRotated((GLdouble)RotationAngle, 0.0, 1.0, 0.0);	//add sun rotation 
	else glRotated((GLdouble)lastAngle, 0.0, 0.0, 1.0);

	glPushMatrix();
	glRotated((GLdouble)-90, 1.0, 0.0, 0.0);	//cange pic rotation
	gluSphere(pObj, 3, 100, 100);
	glPopMatrix();

	glPushMatrix();
	if (switcheroo)glBindTexture(GL_TEXTURE_2D, space[4]);	//add earth texture
	else glBindTexture(GL_TEXTURE_2D, space[1]);
	glTranslated(5, 0, 0);		//earth distance from sun
	if (sunrotate == 1) glRotated((GLdouble)RotationAngle * 4, 0.0, 1.0, 0.0);	//add earth rotation
	else glRotated((GLdouble)lastAngle, 0.0, 0.0, 1.0);

	glPushMatrix();
	glRotated((GLdouble)-90, 1.0, 0.0, 0.0);	//change pic rotation
	gluSphere(pObj, 0.5, 15, 15);
	glPopMatrix();

	if (switcheroo)glBindTexture(GL_TEXTURE_2D, space[5]);	//add moon texture
	else glBindTexture(GL_TEXTURE_2D, space[2]);
	glTranslated(0, 0, 1);		//moon distance from earth
	if (sunrotate == 1) glRotated((GLdouble)RotationAngle, 0.0, 1.0, 0.0);	//add moon rotation
	else glRotated((GLdouble)lastAngle, 0.0, 0.0, 1.0);

	glPushMatrix();
	glRotated((GLdouble)-90, 1.0, 0.0, 0.0);	//moon pic rotation
	gluSphere(pObj, 0.2, 15, 15);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	gluDeleteQuadric(pObj);
	glPopMatrix();

}

void drawParkBox() {
	for (int i = 0; i < 6; i++) {	//set park texture to SpaceBox
		Cube[i] = parkCube[i];
	}
	int D = 40, d = 2;
	glPushMatrix();
	glTranslated(0, 20, 0);	//move box up to make more air in space
	drawSpaceBox();
	glBindTexture(GL_TEXTURE_2D, Cube[SKY_DOWN]);	//bind texture for ground
	glTranslated(0, 18, 0);	//move up to make ground at start of xyz coordinates
	glBegin(GL_QUADS);		//create ground square
	glTexCoord2f(0, 0); glVertex3f(-D, -D + d, +D);
	glTexCoord2f(1, 0); glVertex3f(+D, -D + d, +D);
	glTexCoord2f(1, 1); glVertex3f(+D, -D + d, -D);
	glTexCoord2f(0, 1); glVertex3f(-D, -D + d, -D);
	glEnd();
	glPopMatrix();
}

void drawSpaceBox() {

	double D = 50;
	double d = 2.2;

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	//clearing the background
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//drawing a box and binding its textures
	glPushMatrix();
	//right
	glBindTexture(GL_TEXTURE_2D, Cube[SKY_RIGHT]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(+D - d, -D, -D);
	glTexCoord2f(1, 0); glVertex3f(+D - d, -D, +D);
	glTexCoord2f(1, 1); glVertex3f(+D - d, +D, +D);
	glTexCoord2f(0, 1); glVertex3f(+D - d, +D, -D);
	glEnd();
	//left
	glBindTexture(GL_TEXTURE_2D, Cube[SKY_LEFT]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-D + d, -D, +D);
	glTexCoord2f(1, 0); glVertex3f(-D + d, -D, -D);
	glTexCoord2f(1, 1); glVertex3f(-D + d, +D, -D);
	glTexCoord2f(0, 1); glVertex3f(-D + d, +D, +D);
	glEnd();
	//up
	glBindTexture(GL_TEXTURE_2D, Cube[SKY_UP]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-D, +D - d, -D);
	glTexCoord2f(1, 0); glVertex3f(+D, +D - d, -D);
	glTexCoord2f(1, 1); glVertex3f(+D, +D - d, +D);
	glTexCoord2f(0, 1); glVertex3f(-D, +D - d, +D);
	glEnd();
	//down
	glBindTexture(GL_TEXTURE_2D, Cube[SKY_DOWN]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-D, -D + d, +D);
	glTexCoord2f(1, 0); glVertex3f(+D, -D + d, +D);
	glTexCoord2f(1, 1); glVertex3f(+D, -D + d, -D);
	glTexCoord2f(0, 1); glVertex3f(-D, -D + d, -D);
	glEnd();
	//back
	glBindTexture(GL_TEXTURE_2D, Cube[SKY_BACK]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-D, -D, -D + d);
	glTexCoord2f(1, 0); glVertex3f(+D, -D, -D + d);
	glTexCoord2f(1, 1); glVertex3f(+D, +D, -D + d);
	glTexCoord2f(0, 1); glVertex3f(-D, +D, -D + d);
	glEnd();
	//front
	glBindTexture(GL_TEXTURE_2D, Cube[SKY_FRONT]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(+D, -D, +D - d);
	glTexCoord2f(1, 0); glVertex3f(-D, -D, +D - d);
	glTexCoord2f(1, 1); glVertex3f(-D, +D, +D - d);
	glTexCoord2f(0, 1); glVertex3f(+D, +D, +D - d);
	glEnd();
	glPopMatrix();
}

void drawCarrousel() {
	double h, d;
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	GLUquadricObj* quadric = gluNewQuadric();
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluQuadricTexture(quadric, GL_TRUE);
	//glColor3d(0.5, 0.5, 0.0);
	d = (GLfloat)cos(Deg2Rad(60)) * 5.0;
	h = (GLfloat)sin(Deg2Rad(60)) * 5.0;
	glPushMatrix();
	glRotated(90, 1, 0, 0);

	if (shavrotate == 1) glRotated((GLdouble)RotationAngle, 0.0, 0.0, 1.0);	//add carrousel rotating
	else glRotated((GLdouble)lastAngle, 0.0, 0.0, 1.0);

	if (switcheroo)glBindTexture(GL_TEXTURE_2D, carrousel[2]);	//set roof and sides texture
	else glBindTexture(GL_TEXTURE_2D, carrousel[1]);
	glPushMatrix();
	glTranslated(0, 0, -5);
	gluCylinder(quadric, 0, 3, 1.0, 20, 20); //draw roof	
	glPopMatrix();
	glPushMatrix();
	glTranslated(0, 0, -4);
	gluCylinder(quadric, 2.6, 2.6, 0.4, 20, 20);	//draw ceiling
	glPopMatrix();
	glPushMatrix();
	glTranslated(0, 0, -0.4);
	gluCylinder(quadric, 2.6, 2.6, 0.4, 20, 20);	//draw base
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, swingTextureId[0]);	//set floor texture
	glPushMatrix();
	glTranslated(0, 0, -3.8);
	gluDisk(quadric, 0.0, 2.6, 20, 20);	//draw ceiling
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, -0.4);
	gluDisk(quadric, 0.0, 2.6, 20, 20);	//draw base
	glPopMatrix();

	if (switcheroo)glBindTexture(GL_TEXTURE_2D, shavTextureId[0]);	//add poll texture
	else glBindTexture(GL_TEXTURE_2D, shavTextureId[1]);
	glPushMatrix();
	glTranslated(0, 0, -4);
	gluCylinder(quadric, 0.1, 0.1, 4, 10, 10);	//draw poll
	glPopMatrix();


	if (switcheroo)glBindTexture(GL_TEXTURE_2D, space[3]);	//add seats texture
	else glBindTexture(GL_TEXTURE_2D, space[0]);
	int i;
	for (i = 0; i < 6; i++) {
		glPushMatrix();
		glRotated(i * 60.0, 0, 0, 1);	//rotate to copy paste seats and ropes

		glBindTexture(GL_TEXTURE_2D, swingTextureId[1]);	//add rope texture
		glPushMatrix();
		glRotated(-10, 0, 0, 1);
		glTranslated(2, 0, -4.2);
		glRotated(30, 0, 1, 0);
		drawRope();
		glPopMatrix();

		glPushMatrix();
		glRotated(10, 0, 0, 1);
		glTranslated(2, 0, -4.2);
		glRotated(30, 0, 1, 0);
		drawRope();
		glPopMatrix();

		if (switcheroo)glBindTexture(GL_TEXTURE_2D, swingTextureId[3]);	//add seats texture
		else glBindTexture(GL_TEXTURE_2D, swingTextureId[2]);
		glPushMatrix();
		glTranslated(d + 1.8, 0, 0);
		glRotated(-60, 0, 1, 0);
		glPushMatrix();
		glRotated(90, 0, 0, 1);
		glTranslated(-1, h + 0.5, 0);
		drawSeat();
		glPopMatrix();
		glPopMatrix();
		glPopMatrix();
	}

	gluDeleteQuadric(quadric);
	glPopMatrix();

}

void drawPaperPlane() {
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	if (switcheroo)glBindTexture(GL_TEXTURE_2D, shavTextureId[3]);	//add paper texture
	else glBindTexture(GL_TEXTURE_2D, shavTextureId[2]);
	//draw the plane using dots coordinates
	glBegin(GL_POLYGON);
	glTexCoord2f(1, 1); glVertex3f(0, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(1, 0, 3);
	glTexCoord2f(1, 0); glVertex3f(0.2, 0, 3);
	glEnd();
	glBegin(GL_POLYGON);
	glTexCoord2f(1, 1); glVertex3f(0, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(1, 0, 3);
	glTexCoord2f(1, 0); glVertex3f(0.08, 0.05, 1.8);
	glEnd();
	glBegin(GL_POLYGON);
	glTexCoord2f(1, 1); glVertex3f(0, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(0.2, 0, 3);
	glTexCoord2f(1, 0); glVertex3f(0, -0.5, 3);
	glEnd();
	glBegin(GL_POLYGON);
	glTexCoord2f(1, 1); glVertex3f(0, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(0.08, 0.05, 1.8);
	glTexCoord2f(1, 0); glVertex3f(0, -0.2, 1.2);
	glEnd();

	glBegin(GL_POLYGON);
	glTexCoord2f(1, 1); glVertex3f(0, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(-1, 0, 3);
	glTexCoord2f(1, 0); glVertex3f(-0.2, 0, 3);
	glEnd();
	glBegin(GL_POLYGON);
	glTexCoord2f(1, 1); glVertex3f(0, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(-1, 0, 3);
	glTexCoord2f(1, 0); glVertex3f(-0.08, 0.05, 1.8);
	glEnd();
	glBegin(GL_POLYGON);
	glTexCoord2f(1, 1); glVertex3f(0, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(-0.2, 0, 3);
	glTexCoord2f(1, 0); glVertex3f(0, -0.5, 3);
	glEnd();
	glBegin(GL_POLYGON);
	glTexCoord2f(1, 1); glVertex3f(0, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(-0.08, 0.05, 1.8);
	glTexCoord2f(1, 0); glVertex3f(0, -0.2, 1.2);
	glEnd();
}

void drawFlyingPlane() {

	//drawParkBox();
	glPushMatrix();
	glTranslated(0, 2, 0);
	glRotated(-15, 0, 0, 1);
	if (shavrotate == 1) glRotated((GLdouble)flyingAngle, 0.0, 1.0, 0.0);	//add flying movement rotating angle
	else glRotated((GLdouble)flyingLastAngle, 0.0, 1.0, .0);
	glTranslated(6, 0, 0);	//set place
	glRotated(-15, 0, 0, 1);	//change movement bias
	drawPaperPlane();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 2, 0);
	glRotated(5, 0, 0, 1);
	if (shavrotate == 1) glRotated((GLdouble)flyingAngle - 10, 0.0, 1.0, 0.0);	//add flying movement rotating angle
	else glRotated((GLdouble)flyingLastAngle - 10, 0.0, 1.0, .0);
	glRotated((GLdouble)40, 0.0, 1.0, 0.0);
	glTranslated(6, 0, 5);	//set place
	glRotated(-15, 0, 0, 1);	//change movement bias
	drawPaperPlane();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 2, 2);
	glRotated(-30, 0, 0, 1);
	if (shavrotate == 1) glRotated((GLdouble)flyingAngle - 15, 0.0, 1.0, 0.0);	//add flying movement rotating angle
	else glRotated((GLdouble)flyingLastAngle - 15, 0.0, 1.0, .0);
	glRotated((GLdouble)30, 0.0, 1.0, 0.0);
	glTranslated(6, 2, 2);	//set place
	glRotated(-10, 0, 0, 1);	//change movement bias
	drawPaperPlane();
	glPopMatrix();

}


void drawPoint(int x, int y) {
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}


void TerminationErrorFunc(char* ErrorString)
{
	char string[256];
	printf(ErrorString);
	fgets(string, 256, stdin);

	exit(0);
}

GLuint generateTextures(char* imagepath)
{
	GLubyte* ImageData;
	int width, height;
	GLuint TextureID;

	//reading the image
	ImageData = readBMP(imagepath, &width, &height);

	//Giving a texture ID
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);

	//Assign Image as a texture 
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, ImageData);

	//freeing image memory
	free(ImageData);

	//setting Specific texture parameters 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_REPEAT or GL_CLAMP
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // GL_LINEAR or GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return TextureID;
}

// Function to load bmp file
// buffer for the image is allocated in this function, you should free this buffer
GLubyte* readBMP(char* imagepath, int* width, int* height)
{
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int imageSize;   // = width*height*3
	unsigned char* data;
	unsigned char tmp;
	int i;

	// Open the file
	FILE* file = fopen(imagepath, "rb");
	if (!file) {
		TerminationErrorFunc("Image could not be opened\n");
	}

	if (fread(header, 1, 54, file) != 54) { // If not 54 bytes read : problem
		TerminationErrorFunc("Not a correct BMP file\n");
	}

	if (header[0] != 'B' || header[1] != 'M') {
		TerminationErrorFunc("Not a correct BMP file\n");
	}

	// Read ints from the byte array
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	*width = *(int*)&(header[0x12]);
	*height = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)
		imageSize = *width * *height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)
		dataPos = 54; // The BMP header is done that way

	// Create a buffer
	data = malloc(imageSize * sizeof(GLubyte));

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);


	//swap the r and b values to get RGB (bitmap is BGR)
	for (i = 0; i < *width * *height; i++)
	{
		tmp = data[i * 3];
		data[i * 3] = data[i * 3 + 2];
		data[i * 3 + 2] = tmp;
	}


	//Everything is in memory now, the file can be closed
	fclose(file);

	return data;
}

// create vector from angle and size of vector
void Deg2Vector(float VecDeg, float VecSize, Point* p)
{
	p->x = (GLfloat)cos(Deg2Rad(VecDeg)) * VecSize;
	p->y = (GLfloat)sin(Deg2Rad(VecDeg)) * VecSize;
}

// convert degree to radians
float Deg2Rad(float deg)
{
	return deg * PI / 180.0;
}





