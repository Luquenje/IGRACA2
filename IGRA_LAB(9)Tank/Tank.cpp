#include "stdafx.h"
#include "Tank.h"
#include "Missile.h"

Tank::Tank()
{

	redPlasticMaterial = {
		{ 0.4f, 0.0f, 0.0f, 1.0f }, // Ambient 
		{ 0.9f, 0.0f, 0.0f, 1.0f }, // Diffuse 
		{ 0.8f, 0.8f, 0.8f, 1.0f }, // Specular  
		32.0    // Shininess 
	};
	greenPlasticMaterial = {
		{ 0.0f, 0.4f, 0.0f, 1.0f }, // Ambient 
		{ 0.0f, 0.9f, 0.0f, 1.0f }, // Diffuse 
		{ 0.8f, 0.8f, 0.8f, 1.0f }, // Specular  
		32.0    // Shininess 
	};
	yellowPlasticMaterial = {
		{ 0.4f,  0.4f, 0.0f, 1.0f }, // Ambient 
		{ 0.9f, 0.9f, 0.0f, 1.0f }, // Diffuse 
		{ 0.8f, 0.8f, 0.8f, 1.0f }, // Specular  
		32   // Shininess 
	};
	ArmHeight = 1.5;
	BuildTree();
	yawAngle = degToRad(-90);
	
}

void Tank::Draw() {
	glPushMatrix();
	// At startup time we make player face the negative z-axis
	glTranslatef(xPos, yPos + 0.075, zPos);
	// Local rotation
	glRotatef(yRotation + 180, 0, 1, 0);

	DrawTree(base);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW); // Vertices in clockwise order
	glPolygonMode(GL_FRONT, GL_FILL); // Solid cube

	GLfloat green[] = { 0, 1, 0, 0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green);

	//Back
	glBegin(GL_POLYGON);
	//glColor3f(0.0, 0.0, 1.0);//Red
	glVertex3f(1.0, 0.0, -1); // v7
	glVertex3f(0.8, 1.0, -0.8); // v6
	glVertex3f(-0.8, 1.0, -0.8); // v5
	glVertex3f(-1.0, 0.0, -1.0); // v4
	glEnd();
	//Bottom
	glBegin(GL_POLYGON);
	//glColor3f(0.0, 0.1, 1.0);//Grey
	glVertex3f(1.0, 0.0, 1.0);  // v3
	glVertex3f(1.0, 0.0, -1.0); // v7
	glVertex3f(-1.0, 0.0, -1); // v4
	glVertex3f(-1.0, 0.0, 1); // v0
	glEnd();
	//Front
	glBegin(GL_POLYGON); // 0, 1, 2, 3
						 //glColor3f(0.0, 0.0, 1.0);//green
	glVertex3f(-1.0, 0.0, 1); // v0 Left Bottom
	glVertex3f(-0.8, 1.0, 0.8); // v1 Left Top
	glVertex3f(0.8, 1.0, 0.8); // v2 Right Top
	glVertex3f(1.0, 0.0, 1); // v3 Right Bottom
	glEnd();
	//Top
	glBegin(GL_POLYGON);
	//glColor3f(0.0, 0.0, 1.0);//black
	glVertex3f(-0.8, 1.0, 0.8); // v1
	glVertex3f(-0.8, 1.0, -0.8); // v5
	glVertex3f(0.8, 1.0, -0.8); // v6
	glVertex3f(0.8, 1.0, 0.8); // v2
	glEnd();
	//Right
	glBegin(GL_POLYGON);
	//glColor3f(0.0, 0.0, 1);//Blue
	glVertex3f(1.0, 0.0, 1); // v3
	glVertex3f(0.8, 1.0, 0.8); // v2
	glVertex3f(0.8, 1.0, -0.8); // v6
	glVertex3f(1.0, 0.0, -1.0); // v7
	glEnd();
	//Left
	glBegin(GL_POLYGON);
	//glColor3f(0.0, 0.0, 1.0);//Yellow
	glVertex3f(-1.0, 0.0, 1); // v0
	glVertex3f(-1.0, 0.0, -1); // v4
	glVertex3f(-0.8, 1.0, -0.8); // v5
	glVertex3f(-0.8, 1.0, 0.8); // v1
	glEnd();
	glDisable(GL_CULL_FACE);
	glPopMatrix();
	glEnd();
}

void Tank::DrawFunction(int id)
{
	switch (id) {
	case DRAW_BASE_FUNCTION_ID:
		DrawBase();
		break;
	case DRAW_LOWERARM_FUNCTION_ID:
		DrawLowerArm();
		break;
	case DRAW_UPPERARM_FUNCTION_ID:
		DrawUpperArm();
		break;
	case DRAW_JOINT_FUNCTION_ID:
		DrawJoint();
		break;
	}
}


void Tank::DrawTree(TreeNode* root) {

	if (root == 0)
		return;
	glColor3f(0, 0, 0);

	glPushMatrix();
	glMultMatrixf(root->matrix);
	DrawFunction(root->drawFunctionID);

	if (root->child != 0)
		DrawTree(root->child);
	glPopMatrix();


}

float Tank::degToRad(float degAngle) {
	double pi = 3.1415926535; // You could be more precise!
	return degAngle / 180.0 * pi;
}

void Tank::MoveForward(double dist) {
	// Movement must be based on orientation of player
	double deltaX = 0;
	double deltaZ = 0;
	// Calculate translation as based on current yRotation angle
	deltaX = -dist*sin(degToRad(yRotation));
	deltaZ = -dist*cos(degToRad(yRotation));

	
	// Update the position
	xPos = xPos + deltaX;
	zPos = zPos + deltaZ;
}

void Tank::Rotate(float angle) {
	// Update the rotation (yaw)
	yawAngle -= degToRad(angle);
	yRotation = yRotation + angle;
}


void Tank::HandleKeyDown(WPARAM wParam) {
	double dist = 0.1;
	glPushMatrix();
	glLoadIdentity();

	if (GetAsyncKeyState(VK_LEFT)) {
		
	}
	switch (wParam) {
	case VK_LEFT:
		glMultMatrixf(base->matrix);
		yawAngle -= degToRad(2);
		glRotatef(2, 0, 0, 1);
		glGetFloatv(GL_MODELVIEW_MATRIX, base->matrix);
		break;
	case VK_RIGHT:
		glMultMatrixf(base->matrix);
		yawAngle += degToRad(2);
		glRotatef(-2, 0, 0, 1);
		glGetFloatv(GL_MODELVIEW_MATRIX, base->matrix);
		break;
	case VK_DOWN:
		glMultMatrixf(lowerarmjoint->matrix);
		tiltAngle -= degToRad(2);
		glRotatef(2, 1, 0, 0);
		glGetFloatv(GL_MODELVIEW_MATRIX, lowerarmjoint->matrix);
		break;
	case VK_UP:
		glMultMatrixf(lowerarmjoint->matrix);
		tiltAngle += degToRad(2);
		glRotatef(-2, 1, 0, 0);
		glGetFloatv(GL_MODELVIEW_MATRIX, lowerarmjoint->matrix);
		break;
	case VK_A:
		Rotate(5); // Counter Clockwise
		break;
	case VK_D:
		Rotate(-5); // Clockwise
		break;
	case VK_W:
		MoveForward(0.1);
		break;
	case VK_S:
		MoveForward(-0.1);
		break;
	case VK_SPACE:
			dist = ArmHeight*cos(tiltAngle);
			yMissile = (1.4 + ArmHeight* sin(tiltAngle));
			xMissile = (xPos + dist*cos(yawAngle));
			zMissile = (zPos + dist*sin(yawAngle));

			 xMissileRot = yawAngle * 180 / 3.1415926535;
			 yMissileRot = tiltAngle * 180 / 3.1415926535;

			 
			 Missile missile = Missile(xMissile, yMissile, zMissile, xMissileRot, yMissileRot);
			 Missile::missileList.push_back(missile);
		
		break;
	/*case VK_PRIOR:
		glMultMatrixf(upperarmjoint->matrix);
		glRotatef(-2, 1, 0, 0);
		glGetFloatv(GL_MODELVIEW_MATRIX, upperarmjoint->matrix);
		break;
	case VK_NEXT:
		glMultMatrixf(upperarmjoint->matrix);
		glRotatef(2, 1, 0, 0);
		glGetFloatv(GL_MODELVIEW_MATRIX, upperarmjoint->matrix);
		break;*/

	}
	glPopMatrix();
}

void Tank::DrawJoint() {
	GLUquadric *Object = gluNewQuadric();
	glMaterialfv(GL_FRONT, GL_AMBIENT, yellowPlasticMaterial.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, yellowPlasticMaterial.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, yellowPlasticMaterial.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, yellowPlasticMaterial.shininess);
	gluSphere(Object, 0.25, 64, 64);
}

void Tank::DrawBase() {
	GLUquadric *Object = gluNewQuadric();
	glMaterialfv(GL_FRONT, GL_AMBIENT, redPlasticMaterial.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, redPlasticMaterial.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, redPlasticMaterial.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, redPlasticMaterial.shininess);
	gluCylinder(Object, .4, .4, .45, 16, 16);
	glPushMatrix();
	glTranslatef(0, 0, 0.45);
	gluDisk(Object, 0, .4, 16, 16);

	glPopMatrix();
}


void Tank::DrawUpperArm() {

	GLUquadric *Object = gluNewQuadric();
	glMaterialfv(GL_FRONT, GL_AMBIENT, greenPlasticMaterial.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, greenPlasticMaterial.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, greenPlasticMaterial.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, greenPlasticMaterial.shininess);
	gluCylinder(Object, .4, .4, .45, 16, 16);
	gluDisk(Object, 0, .4, 16, 16);
}

void Tank::DrawLowerArm() {
	GLUquadric *Object = gluNewQuadric();
	glMaterialfv(GL_FRONT, GL_AMBIENT, redPlasticMaterial.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, redPlasticMaterial.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, redPlasticMaterial.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, redPlasticMaterial.shininess);
	//glRotatef(90.0f, 1.0, 0, 0);
	gluCylinder(Object, .15, .15, ArmHeight, 16, 16);
	
}

//double Tank::GetDeltaTime(double deltatime)
//{
//	delta
//}

void Tank::ShootMissile() {
	

}



//void Tank::DrawMissile() {
//	if (isFired) {
//		GLUquadric *Object = gluNewQuadric();
//
//		glMaterialfv(GL_FRONT, GL_AMBIENT, yellowPlasticMaterial.ambient);
//		glMaterialfv(GL_FRONT, GL_DIFFUSE, yellowPlasticMaterial.diffuse);
//		glMaterialfv(GL_FRONT, GL_SPECULAR, yellowPlasticMaterial.specular);
//		glMaterialfv(GL_FRONT, GL_SHININESS, yellowPlasticMaterial.shininess);
//
//
//
//		glTranslatef(xMissile, yMissile, zMissile);
//		glRotatef(90, 0, 1.0, 0);
//		glRotatef(-xMissileRot, 0, 1.0, 0);
//		glRotatef(-yMissileRot, 1.0, 0, 0);
//		gluCylinder(Object, .2, .2, 1.0, 16, 16);
//
//		//debug
//		//debug("yangle", to_string(yawAngleInDegree));
//
//		//x = 5 * cos(degToRad(-xMissileRot)) * 
//
//	}
//	
//	
//	
//}

void Tank::BuildTree() {

	glMatrixMode(GL_MODELVIEW); // The root of the tree (represents the base of Robot Arm)   
	base = new TreeNode;
	base->child = 0;
	base->drawFunctionID = DRAW_BASE_FUNCTION_ID;
	glLoadIdentity();
	glRotatef(-90, 1, 0, 0);
	glTranslatef(0, 0, 0.9);
	glGetFloatv(GL_MODELVIEW_MATRIX, base->matrix);

	glLoadIdentity();
	lowerarmjoint = new TreeNode;
	lowerarmjoint->child = 0;
	lowerarmjoint->drawFunctionID = DRAW_JOINT_FUNCTION_ID;
	glTranslatef(0, 0, 0.4);
	glRotatef(90.0f, 1.0, 0, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lowerarmjoint->matrix);

	glLoadIdentity();
	lowerarmnode = new TreeNode;
	lowerarmnode->child = 0;
	lowerarmnode->drawFunctionID = DRAW_LOWERARM_FUNCTION_ID;
	glTranslatef(0, 0, 0.1);
	glGetFloatv(GL_MODELVIEW_MATRIX, lowerarmnode->matrix);//lower arm


	//glLoadIdentity();
	//upperarmjoint = new TreeNode;
	//upperarmjoint->child = 0;
	//upperarmjoint->drawFunctionID = DRAW_JOINT_FUNCTION_ID;
	//glTranslatef(0, 0, ArmHeight);
	//glGetFloatv(GL_MODELVIEW_MATRIX, upperarmjoint->matrix);

	glLoadIdentity();
	upperarmnode = new TreeNode;
	upperarmnode->child = 0;
	upperarmnode->drawFunctionID = DRAW_UPPERARM_FUNCTION_ID;
	glTranslatef(0, 0, 1.2);
	glGetFloatv(GL_MODELVIEW_MATRIX, upperarmnode->matrix);

	

	base->child = lowerarmjoint;
	lowerarmjoint->child = lowerarmnode;
	lowerarmnode->child = upperarmnode;
	//upperarmjoint->child = upperarmnode;
}

Tank::~Tank()
{
}
