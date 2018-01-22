#pragma once
// for IGRA - OPENGL
#include <gl\gl.h> // Header File For The OpenGL32 Library
#include <gl\glu.h> // Header File For The GLu32 Library
#include "Tank.h"

#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44

//typedef struct Material {
//	GLfloat ambient[4];
//	GLfloat diffuse[4];
//	GLfloat specular[4];
//	GLfloat shininess[4];
//} Material;
class Player
{
	typedef struct Material {
		GLfloat ambient[4];
		GLfloat diffuse[4];
		GLfloat specular[4];
		GLfloat shininess[4];
	} Material;
public:
	Tank *newArm;
	GLUquadricObj *cone;
	Material material;
	double xPos;
	double yPos;
	double zPos;
	double yRotation;
	Player();
	void SetMaterial(Material *m);
	void Draw();
	void HandleKeyDown(WPARAM wParam);
	void Rotate(double angle);
	void MoveForward(double dist);
	float degToRad(float degAngle);
	~Player();

};

