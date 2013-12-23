#include"terrain.h"


Terrain::Terrain(float width, float length)
{
	//sets Terrain size
	x = width;
	y = 0;
	z = length;
	//image size
	image_width = 1280;
	image_height = 850;

	GLfloat terrain_vertices[4*3] = {	//defines the arrays
		-x, y, z,
		 x, y, z,
		 x, y,-z,
		-x, y,-z	};

	GLfloat terrain_normals[4*3] = {
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		0, 1, 0};

	GLfloat terrain_tex_coords[4*2] = {
		0,0,
		0,1,
		1,1,
		1, 0};


	glGenBuffers(1, &vertexID);	//generate vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vertexID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(4*3), terrain_vertices, GL_STATIC_DRAW);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glGenBuffers(1, &normalID);	//generate normal buffers
	glBindBuffer(GL_ARRAY_BUFFER, normalID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(4*3), terrain_normals, GL_STATIC_DRAW);
	glNormalPointer(GL_INT, 0, 0);

	glGenBuffers(1, &texID);	//generate texture buffers
	glBindBuffer(GL_ARRAY_BUFFER, texID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(4*2), terrain_tex_coords, GL_STATIC_DRAW);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	glEnable(GL_TEXTURE_2D);
	readRaw("ocean.raw");	//reads in a raw file

	glGenBuffers(1, &pboID);	//generates a pbo buffer
	glBindTexture(GL_TEXTURE_2D, texID);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboID);

	glBufferData(GL_PIXEL_UNPACK_BUFFER, image_width * image_height * 3, 0, GL_STATIC_DRAW);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Terrain::draw()
{
	//binds the texture to the Terrain
	glBindTexture(GL_TEXTURE_2D, texID);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboID);
	//binds the vertex and normal buffers
	glBindBuffer(GL_ARRAY_BUFFER, vertexID);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, normalID);
	glNormalPointer(GL_INT, 0, 0);

	glEnable(GL_TEXTURE_2D);
	glBindBuffer(GL_ARRAY_BUFFER, pboID);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glDrawArrays(GL_QUADS, 0, 4);	//draws the Terrain

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//empties the texture buffers
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	glDisable(GL_TEXTURE_2D);
}

int Terrain::readRaw(char *fileName)	//reads a raw file
{
	FILE *file = 0;
	if (!(file = fopen(fileName, "r")))
		return 0;

	image = (unsigned char*) malloc(image_width * image_height * 3);
	fread((char *) image, image_width * image_height * 3, 1, file);
	fclose(file);

	setMipMapTexture();
	free(image);

	return 1;
}

void Terrain::animate_terrain()
{
	
}

void Terrain::setMipMapTexture()
{
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	gluBuild2DMipmaps(
			GL_TEXTURE_2D, 
			GL_RGB,
			image_width, 
			image_height, 
			GL_RGB, 
			GL_UNSIGNED_BYTE, 
			image);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); //GL_MODULATE, GL_DECAL, GL_BLEND, or GL_REPLACE

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}