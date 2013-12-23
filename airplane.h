#ifndef AIRPLANE_H
#define AIRPLANE_H

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<glew.h>
#include<glut.h>

class Airplane
{

	private:

		//array for coordinates
		GLfloat airplane_vertex[1500*3];
		GLfloat airplane_normals[1500*3];
		GLfloat airplane_tex_coords[1500*2];

		GLuint vertexID, normalID, texID, pboID;	//buffer IDs

		int image_width, image_height;	//size of image

		unsigned char *image;	//holds the image

	public:
		//function declarations
		Airplane();

		void draw();

		void draw_shadow();

		int readRaw(char *fileName);
		
		void setMipMapTexture();
};




#endif AIRPLANE_H