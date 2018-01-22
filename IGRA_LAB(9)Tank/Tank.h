#pragma once
// for IGRA - OPENGL
#include <gl\gl.h> // Header File For The OpenGL32 Library
#include <gl\glu.h> // Header File For The GLu32 Library
#include "math.h"

#define DRAW_BASE_FUNCTION_ID 0
#define DRAW_LOWERARM_FUNCTION_ID 1
#define DRAW_UPPERARM_FUNCTION_ID 2
#define DRAW_JOINT_FUNCTION_ID 3
#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44

typedef struct TreeNode {
	GLfloat matrix[16];
	int drawFunctionID;
	struct TreeNode* sibling;
	struct TreeNode* child;
} TreeNode;
typedef struct Material {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat shininess[4];
} Material;
class Tank {
public:
	// Texture image      // Following Nodes need to available in HandleKeyDown        
	TreeNode* base;
	TreeNode* lowerarmjoint;
	TreeNode* lowerarmnode;
	TreeNode* upperarmjoint;
	TreeNode* upperarmnode;

	Material redPlasticMaterial;
	Material yellowPlasticMaterial;
	Material greenPlasticMaterial;


	double xPos;
	double yPos;
	double zPos;
	double yRotation;
	float ArmHeight;
	Tank();
	void Rotate(double angle);
	void MoveForward(double dist);
	float degToRad(float degAngle);
	void BuildTree();
	void DrawTree(TreeNode* root);
	void DrawFunction(int id);
	void Draw();
	void DrawBase();
	void DrawLowerArm();
	void DrawUpperArm();
	void DrawJoint();
	void HandleKeyDown(WPARAM wParam);
	~Tank();
};

