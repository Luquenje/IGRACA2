#include "stdafx.h"
#include "Missile.h"

vector<Missile> Missile::missileList;

Missile::Missile(float xpos, float ypos, float zpos, float tilt, float yaw, float Initvelocity)
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
	velocity = Initvelocity;
	x = 0;
	y = 0;
	z = 0;
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
			dist = velocity * cos(degToRad(startTiltAngle)) * timePassed;//yawangle
			x +=   dist*cos(degToRad(startYawAngle ));
			z +=  dist*sin(degToRad(startYawAngle ));

			y += velocity * sin(degToRad(startTiltAngle )) * timePassed - 0.5*9.81*pow(timePassed, 2);
			
			GLUquadric *Object = gluNewQuadric();

			glMaterialfv(GL_FRONT, GL_AMBIENT, yellowPlasticMaterial.ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, yellowPlasticMaterial.diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, yellowPlasticMaterial.specular);
			glMaterialfv(GL_FRONT, GL_SHININESS, yellowPlasticMaterial.shininess);


			glPushMatrix();
			if (y <= -1.3) {
				y = -1.3;
				velocity = 0;
			}

			glTranslatef(xMissile + x, yMissile + y, zMissile + z);
			glRotatef(90, 0, 1.0, 0);
			//glRotatef(startTiltAngle + fmod(startYawAngle, 180) > 0 ? timePassed * 40 : -timePassed * 40, 0, 0, 1);
			glRotatef(-startYawAngle, 0, 1.0, 0);
			

			gluCylinder(Object, .2, .2, 1.0, 16, 16);
			glPopMatrix();
			//debug
			debug("yangle", to_string(startYawAngle));

			



	

}


Missile::~Missile()
{
}
