//Peer Andreas Stange 091041
#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<time.h>
#include<glew.h>
#include<glut.h>
using namespace std;
//includes
#include"airplane.h"
#include"terrain.h"
#include"Camera.h"

//creates a struct
struct slider_t
{
	int xPosition, yPosition;	//the slider position
	int start_position;	//start position
	int width, height;	//slider/bar width and height
	time_t time1, time2, time_difference;	//timers
	bool timer;	//checks for double-click
};

//declares pointers
Airplane *fly;
Terrain *hav;
Camera *kamera;

//global variables
int i =200;
int window_width = 800;
int window_height = 600;
int viewport[] = {0, 0, window_width, window_height};
float rotatex = 0;
static float offsetX = 0, offsetY = 0, offsetZ = 0;
static float light[4] = {0, 1000, 0, 1};
float m[16];
float fly_rotasjon_x = -90;
float fly_rotasjon_y = 0;
float fly_rotasjon_z = 0;

const int RENDER = 1;
const int SELECT = 2;
int mode = RENDER;
int cursorX, cursorY;
int left_button_down = 0;
bool red = false;
bool green = false;

slider_t slider1, slider2, bar1, bar2;

unsigned char rgb[3];

float red_color[] = { 1.0, 0.0, 0.0, 1.0};
float blue_color[] = {0.0, 0.0, 1.0, 0.5};
float white_color[] = {1.0, 1.0, 1.0, 1.0};
static float light_source[] = { 0, 1000, 0, 1};

//function declarations
void init();
void initGui();
void drawGui();
void display();
void reshape(int w_w, int h_h);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void idle();
void setStencil();
void skyggelegging();
void reflection();
void stop_picking();
void draw_slider(int w, int h, int pos_x, int pos_y);

float *localLightSource(int nx, int ny, int nz, float lx, float ly, float lz);

int main(int argc, char *argv[])
{
	//initialisation 
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);	
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Eksamen 2011");
	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutIdleFunc(idle);
	
	glewInit();
	init();
	//print instructions
	cout<<"q: roll right\n"
			<<"e: roll left\n"
			<<"a: yaw left\n"
			<<"d: yaw right\n"
			<<"w: pitch down\n"
			<<"s: pitch up\n"
			<<"+: zoom in\n"
			<<"-: zoom out\n";
	//creates new objects
	fly = new Airplane();
	hav = new Terrain(500, 500);
	kamera = new Camera(Point3D(0, 300, 500), Point3D(0, 0, 0), Vector3D(0, 1, 0));
	//mode checks for picking
	if(mode == SELECT)
			stop_picking();
	else
		glutMainLoop();

	return 0;
}

void init()
{	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	//set background to black
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(50, 1, 0.1, 1000);	//set perspective
	//enables depth and lights
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_source);

	initGui();	//initiates GUI
}

void initGui()
{	//define slider and bar size
	slider1.height = 50;
	slider1.width = 25;

	slider2.height = 50;
	slider2.width = 25;

	slider1.xPosition = 400;
	slider1.yPosition = 500;

	slider2.xPosition = 700;
	slider2.yPosition = 300;

	slider1.start_position = slider1.xPosition;
	slider2.start_position = slider2.yPosition;

	bar1.xPosition = 400;
	bar1.yPosition = 500;

	bar1.width = 400;
	bar1.height = 10;

	bar2.xPosition = 700;
	bar2.yPosition = 300;

	bar2.width = 10;
	bar2.height = 400;
}

void drawGui()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	{
		glLoadIdentity();
		//turns on ortho 
		gluOrtho2D(0, window_width, 0, window_height);
		glScalef(1, -1, 1);	//flips screen
		glTranslatef(0, -window_height, 0);
		glMatrixMode(GL_MODELVIEW);	//change to projection

		glDisable(GL_LIGHTING);
		glPushMatrix();
		{	//draws bottom slider
			glLoadIdentity();
			glColor3f(1, 0, 0);
			draw_slider(slider1.width, slider1.height, slider1.xPosition, slider1.yPosition);
		}
		glPopMatrix();

		glPushMatrix();
		{	//draws right slider
			glLoadIdentity();
			glColor3f(0, 1, 0);
			draw_slider(slider2.width, slider2.height, slider2.xPosition, slider2.yPosition);
		}
		glPopMatrix();

		glPushMatrix();
		{	//draws bottom slider
			glLoadIdentity();
			glColor3f(1, 1, 1);
			draw_slider(bar1.width, bar1.height, bar1.xPosition, bar1.yPosition);

		}
		glPopMatrix();

		glPushMatrix();
		{	//draws right slider
			glLoadIdentity();
			glColor3f(1, 1, 1);
			draw_slider(bar2.width, bar2.height, bar2.xPosition, bar2.yPosition);
		}
		glPopMatrix();

		glEnable(GL_LIGHTING);

		glMatrixMode(GL_PROJECTION);
	}
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
}

void display()
{	//clears the buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//functions
	drawGui();
	setStencil();
	skyggelegging();

	glPushMatrix();
	{	//draws plane
		glTranslatef(0, 200, 0);
		glRotatef(fly_rotasjon_x, 1, 0, 0);
		glRotatef(fly_rotasjon_y, 0, 0, 1);
		glRotatef(fly_rotasjon_z, 0, 1, 0);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, red_color);
		fly->draw();
	}
	glPopMatrix();

	glPushMatrix();
	{	//draws the terrain
		glMaterialfv(GL_FRONT, GL_DIFFUSE, blue_color);
		hav->draw();
	}
	glPopMatrix();	

	//checks for picking
	if(mode == SELECT)
		stop_picking();
	else
		glutSwapBuffers();
}

void reshape(int w_w, int h_h)
{	//resizes window
	glViewport(0, 0, w_w, h_h);
}

void keyboard(unsigned char key, int x, int y)
{	//key commands
	switch(key)
	{
		case '+': kamera->slide(10); break;
		case '-': kamera->slide(-10); break;
		case 'w': kamera->pitch(2); break;
		case 's': kamera->pitch(-2); break;	
		case 'q': kamera->roll(-2); break;
		case 'e': kamera->roll(2); break;
		case 'd': kamera->yaw(-2); break;
		case 'a': kamera->yaw(2); break;
	}
}

void mouse(int button, int state, int x, int y)
{	//checks if left button is down
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		left_button_down = 1;	
		cursorX = x;	//cursorX is equal to mouse x position
		cursorY = y;	//cursorY is equal to mouse y position
		mode = SELECT;
		time(&slider1.time1);	//get the time for slider1
		time(&slider2.time1);	//get the time for slider2
		
		slider1.time_difference = difftime(slider1.time1, slider1.time2);//get the time difference between mousepresses
		slider2.time_difference = difftime(slider2.time1, slider2.time2);

		stop_picking();	//checks picking
		if(red)	//if I hit red
		{
			slider1.timer = true;
			slider1.time2 = slider1.time1;
			if(slider1.time_difference < 0.02 && slider1.timer)	//checks for double click
			{
				slider1.time_difference = slider1.time1 = slider1.time2 = 0;
				slider1.xPosition = window_width/2;
				fly_rotasjon_y = 0;
				slider1.timer = false;
			}
		}
		else if(green)	//if I hit green
		{
			slider2.timer = true;
			slider2.time2 = slider1.time1;
			if(slider2.time_difference < 0.02 && slider2.timer)	//checks for double click
			{
				slider2.time_difference = slider2.time1 = slider2.time2 = 0;
				fly_rotasjon_z = 0;
				slider2.yPosition = window_height/2;
				slider2.timer = false;
			}
		}
	}
	else if(state == GLUT_UP)	//if mousebutton is released
	{
		red = false;
		green = false;
		left_button_down = 0;
	}
}

void motion(int x, int y)
{
	if(left_button_down && red == true)	//if button press and I hit red
	{
		if(x > 200 && x < 600)	//while between bar1
		{
			slider1.xPosition = x;	//slider position is equal to mouse x position
			if(slider1.xPosition > slider1.start_position && fly_rotasjon_y <= 45) //rotate right
			{
				fly_rotasjon_y += 1;
				slider1.start_position = x;	//changes start position 
			}
			else if(slider1.xPosition < slider1.start_position && fly_rotasjon_y >= -45)	//rotate left
			{
				fly_rotasjon_y -= 1;
				slider1.start_position = x;
			}
		}
	}
	else if(left_button_down && green == true)	//if button press and I hit green
	{
		if(y > 100 && y < 500)	//while between bar2
		{
			slider2.yPosition =  y;	//slider position is equal to mouse y position

			if(slider2.yPosition > slider2.start_position && fly_rotasjon_z <= 180)	//yaw right
			{
				fly_rotasjon_z +=  1.5;
				slider2.start_position = y;	//changes start position to current y position
			}
			else if(slider2.yPosition < slider2.start_position && fly_rotasjon_z >= -180)	//yaw left
			{
				fly_rotasjon_z -= 1.5;
				slider2.start_position = y;	//changes start position to current y position
			}
		}
	}
}

void idle()
{
	glutPostRedisplay();
}

void setStencil()
{
	glDisable(GL_DEPTH_TEST);	//disables depth test
	glDisable(GL_TEXTURE_2D);	//disables texturing
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);	//turn of coloring
	glEnable(GL_STENCIL_TEST);	//enables stencil
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
	glStencilFunc(GL_ALWAYS, 1, 0xff);
	
	glPushMatrix();
	{
		hav->draw();	//set which plane to draw the stencil on
	}
	glPopMatrix();

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);	//turn on coloring
	glEnable(GL_DEPTH_TEST);	//enable depth test
	glEnable(GL_TEXTURE_2D);	//enable texturing 

	reflection();	//draw reflection

	glDisable(GL_STENCIL_TEST);	//disables stencil drawing
}

void skyggelegging()
{
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL, 1, 0xff);
	glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);
	glEnable(GL_POLYGON_OFFSET_FILL);

	glDisable(GL_BLEND);	//disables blend
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_LIGHTING);	//turns of lighting
	glColor4f(0, 0, 0, 0.1);	//set to 10% shadows

	float *m = localLightSource(0, 1, 0, light[0], light[1], light[2]);	//sets the lightsource

	glPushMatrix();
	{	//translates the shadow 
		glTranslatef(0, 1, 0);	
		glTranslatef(light[0], light[1], light[2]);
		glMultMatrixf(m);
		glTranslatef(-light[0], - light[1], -light[2]);

		glPushMatrix();
		{
			glTranslatef(-50, 0, 0);
			glRotatef(fly_rotasjon_x, 1, 0, 0);	//make sure the shadow rotates along with the plane
			glRotatef(fly_rotasjon_y, 0, 0, 1);
			glRotatef(fly_rotasjon_z,0, 1, 0);
			fly->draw();	//draws the plane
		}
		glPopMatrix();
	}
	glPopMatrix();

	glEnable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisable(GL_STENCIL_TEST);
};

void reflection()
{
	glDisable(GL_CULL_FACE);	//turns of culling
	glStencilFunc(GL_EQUAL, 1, 0xff);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	glPushMatrix();
	{
		
		glTranslatef(0, -200, 0);	//translates the refletion to under the Terrain
		glScalef(1, -1, 1);	//flips the reflection
		glRotatef(fly_rotasjon_x, 1, 0, 0);	//makes sure it rotates acording to the real plane
		glRotatef(fly_rotasjon_y, 0, 0, 1);
		glRotatef(fly_rotasjon_z, 0, 1, 0);
		fly->draw();	//draws the plane reflextion
	}
	glPopMatrix();

	glEnable(GL_CULL_FACE);
}

void stop_picking()
{	//checks the current pixel color
	glReadPixels(cursorX, viewport[3] - cursorY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, rgb);

	if(rgb[0] == 255 && rgb[1] == 0)
	{
		red = true;	//if the pixel is red set red to true
	}
	else if(rgb[0] == 0 && rgb[1] == 255)
	{
		green = true;		//if the pixel is green set green to true
	}
	mode = RENDER;	
}
void draw_slider(int w, int h, int pos_x, int pos_y)
{	//function to draw the slider/bar
	glTranslatef(pos_x, pos_y, 0);
	glBegin(GL_QUADS);
	{
		glVertex2f(-(w/2), -(h/2));
		glVertex2f(-(w/2), (h/2));
		glVertex2f((w/2), (h/2));
		glVertex2f((w/2), -(h/2));
	}
	glEnd();
}
float *localLightSource(int nx, int ny, int nz, float lx, float ly, float lz)	//lightsource
{
	float dotLys = nx * (lx - offsetX) + ny * (ly - offsetY) + nz * (lz - offsetZ); 

	m[0] = dotLys;
	m[1] = 0;
	m[2] = 0;
	m[3] = 0;

	m[4] = 0;
	m[5] = dotLys;
	m[6] = 0;
	m[7] = -1; 

	m[8] = 0;
	m[9] = 0;
	m[10] = dotLys;
	m[11] = 0;

	m[12] = 0;
	m[13] = 0;
	m[14] = 0;
	m[15] = 0;
	return m;
}