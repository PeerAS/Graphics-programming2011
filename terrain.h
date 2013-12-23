#ifndef TERRAIN_H
#define TERRAIN_H

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include<glew.h>
#include<glut.h>


class Terrain
{
	private:

		float x, y, z;	//terrain coordinates

		//terrain arrays
		GLfloat terrain_vertices[4*3];
		GLfloat terrain_normals[4*3];
		GLfloat terrain_tex_coords[4*2];

		int image_width, image_height;	//size of image

		GLuint vertexID, normalID, texID, pboID;	//buffer IDs

		unsigned char *image;	//holds the image

	public:
		//function declarations
		Terrain(float width, float length);

		void draw();

		int readRaw(char *fileName);

		void animate_terrain();

		void setMipMapTexture();
};
#endif TERRAIN_H

