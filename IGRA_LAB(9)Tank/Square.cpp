#include "stdafx.h"
#include "Square.h"





Square::Square()
{
	left = -0.2;
	top = 0.2;
	width = 0.4;
	height = 0.4;
	isSelected = false;
}

//Helper Functions
bool ptInRect(float x, float y, float left, float top, float width, float height) {
	return(x >= left && x <= left + width && y >= top - height && y <= top);
}

Square::~Square()
{

}

void Square::Draw() 
{
	//set the color
	if (isSelected)
		glColor3f(0, 0, 1);
	else
		glColor3f(0, 0, 0);

	//Draw a square with a LINE_LOOP
	glBegin(GL_LINE_LOOP);
	glVertex2f(left, top);//LT
	glVertex2f(left + width, top);//RT
	glVertex2f(left + width, top - height);//RB
	glVertex2f(left, top - height);//LB
	glEnd();
}
void Square::HandleLButtonDown(float xpos, float ypos) 
{
	isSelected = ptInRect(xpos, ypos, left, top, width, height);
}
void Square::HandleMouseMove(float xpos, float ypos) 
{
	left = xpos;
	top = ypos;
}
