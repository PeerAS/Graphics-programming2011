#ifndef KAMERA_H
#define KAMERA_H


#include <stdlib.h>
#include <glut.h>

#include "mathtools.h"


//CHEAP camera class
class Camera
{
	Vector3D right, forward, up;
	

public :
	Point3D eye;
	Camera(Point3D &eye, Point3D &lookat, Vector3D &up)
	{
		this->eye = eye;
		this->up = up;
		forward = (lookat - eye).hat();
		right = forward.cross(up).hat();
		this->up = right.cross(forward);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(eye.x, eye.y, eye.z, 
			eye.x + forward.x, eye.y + forward.y, eye.z + forward.z, 
			up.x, up.y, up.z);
	}

	void slide(float amount)
	{
		eye = eye + amount * forward;
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(eye.x, eye.y, eye.z, 
			eye.x + forward.x, eye.y + forward.y, eye.z + forward.z, 
			up.x, up.y, up.z);
	}

	void yaw(float v)
	{
		v *= 3.14159265 / 180;
		Vector3D f = forward * cos(v) - right * sin(v);
		Vector3D r = forward * sin(v) + right * cos(v);
		forward = f;
		right = r;
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(eye.x, eye.y, eye.z, 
			eye.x + forward.x, eye.y + forward.y, eye.z + forward.z, 
			up.x, up.y, up.z);
	}

	void pitch(float v)
	{
		v *= 3.14159265 / 180;
		Vector3D f = forward * cos(v) - up * sin(v);
		Vector3D u = forward * sin(v) + up * cos(v);
		forward = f;
		up = u;
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(eye.x, eye.y, eye.z, 
			eye.x + forward.x, eye.y + forward.y, eye.z + forward.z, 
			up.x, up.y, up.z);
	}

	void roll(float v)
	{
		v *= 3.14159265 / 180;
		Vector3D u = up * cos(v) - right * sin(v);
		Vector3D r = up * sin(v) + right * cos(v);
		up = u;
		right = r;
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(eye.x, eye.y, eye.z, 
			eye.x + forward.x, eye.y + forward.y, eye.z + forward.z, 
			up.x, up.y, up.z);
	}
};


#endif