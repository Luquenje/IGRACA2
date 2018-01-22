#pragma once

// for IGRA - OPENGL
#include <gl\gl.h> // Header File For The OpenGL32 Library
#include <gl\glu.h> // Header File For The GLu32 Library

class Square {
public:
	float left;
	float top;
	float width;
	float height;
	bool isSelected;


	void Draw();
	void HandleLButtonDown(float xpos, float ypos);
	void HandleMouseMove(float xpos, float ypos);

	Square();
	~Square();
};

