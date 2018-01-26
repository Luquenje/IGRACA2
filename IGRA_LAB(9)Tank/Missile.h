#pragma once
// for IGRA - OPENGL
#include <gl\gl.h> // Header File For The OpenGL32 Library
#include <gl\glu.h> // Header File For The GLu32 Library
#include "math.h"




class Missile
{
public:
	typedef struct Material {
		GLfloat ambient[4];
		GLfloat diffuse[4];
		GLfloat specular[4];
		GLfloat shininess[4];
	} Material;

	Missile(float xpos, float ypos, float zpos, float tilt, float yaw);
	Material yellowPlasticMaterial;
	bool isFired = false;
	float xMissileRot;
	float yMissileRot;
	double timePassed;
	float x;
	float y;
	float z;
	float dist;
	float startTiltAngle;
	float startYawAngle;
	


	static vector<Missile> missileList;
	static void DrawAllMissile();
	float degToRad(float degAngle);
	static void SetTimePassed(double deltatime);
	void DrawMissile();
	float xMissile;
	float yMissile;
	float zMissile;
	void getValuesFromTankClass(float armHeight, double xPos, double zPos);

	~Missile();
};

