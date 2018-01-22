#include "stdafx.h"
#include "Player.h"
#include "math.h"

Player::Player()
{
	xPos = 0.0;
	yPos = 0.0;
	zPos = 0.0;
	material = {
		{ 0.4f, 0.0f, 0.0f, 1.0f }, // Ambient 
		{ 0.9f, 0.0f, 0.0f, 1.0f }, // Diffuse 
		{ 0.8f, 0.8f, 0.8f, 1.0f }, // Specular  
		32.0    // Shininess 
	};//red
}
void Player::SetMaterial(Material *m) {
	glMaterialfv(GL_FRONT, GL_AMBIENT, m->ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m->diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m->specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, m->shininess);
}

void Player::Draw() {
	SetMaterial(&material);
	glPushMatrix();
	// At startup time we make player face the negative z-axis
	glTranslatef(xPos, yPos + 0.075, zPos);
	// Local rotation
	glRotatef(yRotation + 180, 0, 1, 0);
	// Draw the player as a cone
	gluCylinder(cone, 0.15, 0.0, 0.5, 64, 64);
	glPopMatrix();
}
void Player::HandleKeyDown(WPARAM wParam) {
	double dist = 0.1;
	if (wParam == VK_A)
		Rotate(5); // Counter Clockwise
	if (wParam == VK_D)
		Rotate(-5); // Clockwise
	if (wParam == VK_W)
		MoveForward(0.1);
	if (wParam == VK_S)
		MoveForward(-0.1);
}
void Player::MoveForward(double dist) {
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
void Player::Rotate(double angle) {
	// Update the rotation (yaw)
	yRotation = yRotation + angle;
}

float Player::degToRad(float degAngle) {
	double pi = 3.1415926535; // You could be more precise!
	return degAngle / 180.0 * pi;
}

Player::~Player()
{
}
