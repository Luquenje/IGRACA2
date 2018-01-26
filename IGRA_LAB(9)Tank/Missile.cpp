#include "stdafx.h"
#include "Missile.h"

vector<Missile> Missile::missileList;

Missile::Missile(float xpos, float ypos, float zpos, float tilt, float yaw)
{
	yellowPlasticMaterial = {
		{ 0.4f,  0.4f, 0.0f, 1.0f }, // Ambient 
		{ 0.9f, 0.9f, 0.0f, 1.0f }, // Diffuse 
		{ 0.8f, 0.8f, 0.8f, 1.0f }, // Specular  
		32   // Shininess 
	};
	xMissile = xpos;
	yMissile = ypos;
	zMissile = zpos;
	startTiltAngle = tilt;
	startYawAngle = yaw;
	timePassed = 0;
}




void Missile::DrawAllMissile()
{
	for (int i = 0; i < Missile::missileList.size(); i++)
	{
		Missile::missileList[i].DrawMissile();
	}
}

float Missile::degToRad(float degAngle)
{
	double pi = 3.1415926535; // You could be more precise!
	return degAngle / 180.0 * pi;
}

 void Missile::SetTimePassed(double deltatime)
{
	 for (int i = 0; i < Missile::missileList.size(); i++)
	 {
		 Missile::missileList[i].timePassed += deltatime;
	 }
}




void Missile::DrawMissile()
{
			dist = 5 * cos(degToRad(startYawAngle+90)) * timePassed;//yawangle
			x = xMissile + dist*cos(degToRad(startYawAngle + 90));
			z = zMissile + dist*sin(degToRad(startYawAngle + 90));

			y = 5 * sin(degToRad(startTiltAngle + 90)) * timePassed - 0.5*9.81*pow(timePassed, 2);

			GLUquadric *Object = gluNewQuadric();

			glMaterialfv(GL_FRONT, GL_AMBIENT, yellowPlasticMaterial.ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, yellowPlasticMaterial.diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, yellowPlasticMaterial.specular);
			glMaterialfv(GL_FRONT, GL_SHININESS, yellowPlasticMaterial.shininess);


			glPushMatrix();
			glTranslatef(xMissile +x, yMissile +y, zMissile+z);
			glRotatef(90, 0, 1.0, 0);
			glRotatef(-startTiltAngle, 0, 1.0, 0);
			glRotatef(-startYawAngle, 1.0, 0, 0);
			

			gluCylinder(Object, .2, .2, 1.0, 16, 16);
			glPopMatrix();
			//debug
			debug("yangle", to_string(startYawAngle));

			



	

}


Missile::~Missile()
{
}