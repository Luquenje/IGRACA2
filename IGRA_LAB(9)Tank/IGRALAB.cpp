// IGRALAB.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "IGRALAB.h"
#include "math.h"
#include"Vector3f.h"
#include <cmath>
#include <stdio.h>
#include <vector>
#include <string>
#include "Tank.h"
#include "Missile.h"

// for IGRA - OPENGL
#include <gl\gl.h> // Header File For The OpenGL32 Library
#include <gl\glu.h> // Header File For The GLu32 Library

#include "Square.h"
#include "Tank.h"

#define MAX_LOADSTRING 100
bool	keys[256];			// Array Used For The Keyboard Routine

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

//added for IGRA
int width = 400;//640
int height = 400;//480

GLuint PixelFormat;
HDC hDC = NULL;
HWND hWnd = NULL;
HGLRC hRC = NULL;

int EXTRA_HEIGHT = 58;
int EXTRA_WIDTH = 18;



//instantiate an object of class square
Square *sq = new Square();

// The position of the jumping circle
float xPosCircle = 0.0;
float yPosCircle = 0.0;

float moveFoward = 0.0;
float accelerator = 0.0;
float rotateLeft = 0.0;
float rotateRight = 0.0;
bool stop = false;

Tank* tank;

float xMouseOpenGL;
float yMouseOpenGL;

//float yRot = 0.0f;

//typedef struct materialStruct {
//	GLfloat ambient[4];
//	GLfloat diffuse[4];
//	GLfloat specular[4];
//	GLfloat shininess[4];
//} Material;
//
//Material redPlasticMaterial = {
//	{ 0.4, 0.0, 0.0, 1.0 }, // Ambient
//	{ 0.9, 0.0, 0.0, 1.0 }, // Diffuse
//	{ 0.8, 0.8, 0.8, 1.0 }, // Specular
//	32 // Shininess
//};





int InitOpenGL();
void DrawGLScene();
void ReSizeGLScene(GLsizei width, GLsizei height);
void Initialise();
void DrawColourfulTriangle();
void DrawXYAxis();
void DrawTriangleWireFrame();
void DrawSpiral(int i);
float degToRad(float degAngle);
void DrawCircle(float rad, float xc, float yc, int nrOfPoints, int r, int g, int b, bool wireframe);
void convertWindowToOpenGLCoordinates(int xWinPos, int yWinPos, float &xOpenGLPos, float &yOpenGLPos);
void DrawSineCurves(float startX, float startY, int nrOfSineCurves, float scale, float definition);
void Draw3D_AxisSystem();
void DrawCube();
void SetupLight();
void DrawSphere();
void SetMaterial(Material *m);
void DrawPyramid();
void DrawPyramidN(int config);
void NormalizeVector(float &Px, float &Py, float &Pz);
void DrawNormal(float nx, float ny, float nz, float cx, float cy, float cz);
void CalculateNormal(float firstX, float firstY, float firstZ,
	float secondX, float secondY, float secondZ,
	float thirdX, float thirdY, float thirdZ);
void DrawTexturedSquare();
void CreateCheckerBoardTexture();
void StartTimer();
double GetTimePassedSinceLastTime();
double GetTimePassedSinceStart();
void DrawPlane();
void DrawTank();

//array to store our texture image
GLubyte image[64][64][3];

__int64 startTimeInCounts = 0;
__int64 lastTimeInCounts = 0;
__int64 countsPerSecond;

void StartTimer() {
	__int64 currentTimeInCounts;
	// countsPerSecond depends on your PC

	//countsPerSecond = 2742188
	if (!QueryPerformanceFrequency(
		(LARGE_INTEGER *)&countsPerSecond)) {
		MessageBox(NULL, L"QueryPerformanceFrequency Failed.",L"ERROR",MB_OK | MB_ICONINFORMATION);
			return;
	}
	QueryPerformanceCounter(
		(LARGE_INTEGER *)&currentTimeInCounts);
	startTimeInCounts = currentTimeInCounts;
	lastTimeInCounts = currentTimeInCounts;
}

// Return absolute time in seconds -since start
double GetTimePassedSinceStart() {
	__int64 currentTimeInCounts;
	double timePassedSeconds;
	// Calculate time passed in seconds since timer was started
	QueryPerformanceCounter((LARGE_INTEGER *)&currentTimeInCounts);
	timePassedSeconds = (currentTimeInCounts - startTimeInCounts) /
		(double)countsPerSecond;

	return timePassedSeconds;
}

// Return relative time in seconds - since last measurement
double GetTimePassedSinceLastTime() {
	__int64 currentTimeInCounts, timePassedSinceLastTimeInCounts;
	// Calculate time passed in seconds since last call to
	// GetTimePassedSinceLastTime
	QueryPerformanceCounter((LARGE_INTEGER *)&currentTimeInCounts);
	timePassedSinceLastTimeInCounts =
		currentTimeInCounts - lastTimeInCounts;
	double timePassedSinceLastTimeInSeconds =
		(currentTimeInCounts - lastTimeInCounts) /
		(double)countsPerSecond;
	lastTimeInCounts = currentTimeInCounts;
	return timePassedSinceLastTimeInSeconds;
}

// The current rotation angle of the cube
float yRot = 0;
// We assume cube takes 10 seconds to make a full rotation
double rotationalVelicityInDegreesPerSeconds = 36.0;


void CreateCheckerBoardTexture() {
	int nrOfCheckersOnRow = 8;
	float dim = 64.0 / nrOfCheckersOnRow;
	int red = 0;
	int green = 0;
	int blue = 0;
	for (int i = 0; i<64; i++) {
		for (int j = 0; j<64; j++) {

			// Calculate in which checkerboard
			//rectangle the pixel falls
			int row = (int)(i / dim);
			int col = (int)(j / dim);
			int c = 0;
			if (row % 2 == 0) { // Even rows start with black
				if (col % 2 == 0) {
					// All even column will be black
					red = green = blue = 0;
				}
				else {
					green = blue = 0;
					red = 255;
				}
			}
			else {
				// Odd rows start with red
				if (col % 2 == 0) {
					// All even column will be red
					green = blue = 0;
					red = 255;
				}
				else {
					red = green = blue = 0;
				}
			}
			// Drawing a green border around the image
			if (i == 0 || i == 63 || j == 0 || j == 63) {
				red = 0;
				green = 255;
				blue = 0;
			}
			image[i][j][0] = (GLubyte)red;
			image[i][j][1] = (GLubyte)green;
			image[i][j][2] = (GLubyte)blue;
		}
	}
}




void SetMaterial(Material *m) {
	glMaterialfv(GL_FRONT, GL_AMBIENT, m->ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m->diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m->specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, m->shininess);
}


//for spheres, cylinders and cones
//use built in primitives
GLUquadric *mySphere;


static PIXELFORMATDESCRIPTOR pfd{
	sizeof(PIXELFORMATDESCRIPTOR),
	1,// Version Number
	PFD_DRAW_TO_WINDOW | // Format Must Support Window
	PFD_SUPPORT_OPENGL | // Format Must Support OpenGL
	PFD_DOUBLEBUFFER,// Must Support Double Buffering
	PFD_TYPE_RGBA, // Request An RGBA Format
	16, // Select Our Color Depth = 16
	0, 0, 0, 0, 0, 0, // Color Bits Ignored
	0,// No Alpha Buffer
	0,// Shift Bit Ignored
	0,// No Accumulation Buffer
	0, 0, 0, 0, // Accumulation Bits Ignored
	32, // 32 Bit Z-Buffer (Depth Buffer)
	0,// No Stencil Buffer
	0,// No Auxiliary Buffer
	PFD_MAIN_PLANE,// Main Drawing Layer
	0,// Reserved
	0, 0, 0// Layer Masks Ignored
};

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_IGRALAB, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_IGRALAB));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
			DrawGLScene();
			SetTimer(hWnd, 0, 1, 0);
			SwapBuffers(hDC);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IGRALAB));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_IGRALAB);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   /*HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);*/ //-------->default window size
   
   hWnd = CreateWindowW(szWindowClass,
	   szTitle, WS_OVERLAPPEDWINDOW,
	   0, 0, width + EXTRA_WIDTH, height + EXTRA_HEIGHT,
	   nullptr, nullptr, hInstance, nullptr);

   InitOpenGL();

   Initialise();

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   ReSizeGLScene(width, height);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_SIZE:
	{
		//resize the triangle with the screen	
		ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));
		break;
	}
	case WM_LBUTTONDOWN:
	{
		// Get the left mouse press coordinates
		int mouseX = LOWORD(lParam);
		int mouseY = HIWORD(lParam);
		// Display the mouse coordinate in a MessageBox
		/*char message[255];
		sprintf_s(message, "x= %d, y=%d", mouseX, mouseY);
		MessageBoxA(NULL, message, "Mouse Press Event", MB_OK | MB_ICONINFORMATION);*/

		convertWindowToOpenGLCoordinates(mouseX, mouseY, xPosCircle, yPosCircle);

		sq->HandleLButtonDown(xPosCircle, yPosCircle);
		
		// Adjust the circle coordinates
		//xPosCircle = 0.3f;
		//yPosCircle = 0.3f;
		return 0;
	}
	
	case WM_KEYDOWN:							// Is A Key Being Held Down?
	{
		//tank->Shoot(wParam, GetTimePassedSinceLastTime());
		
		
		keys[wParam] = TRUE;					// If So, Mark It As TRUE
		return 0;								// Jump Back
	}

	case WM_KEYUP:		
	{// Has A Key Been Released?
		
		keys[wParam] = FALSE;					// If So, Mark It As FALSE
		return 0;								// Jump Back
	}
	case WM_MOUSEMOVE:
	{
		// Get the left mouse press coordinates
		int mouseX = LOWORD(lParam);
		int mouseY = HIWORD(lParam);

		convertWindowToOpenGLCoordinates(mouseX, mouseY, xMouseOpenGL, yMouseOpenGL);

		if(wParam && MK_LBUTTON)
		sq->HandleMouseMove(xMouseOpenGL, yMouseOpenGL);
		
		break;
	}
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}



void ReSizeGLScene(GLsizei width, GLsizei height) {
	if (height == 0)// Prevent A Divide By Zero By
		height = 1; // Making Height Equal One
					// Reset The Current Viewport
	glViewport(0, 0, width, height);
	// Select The Projection Matrix
	glMatrixMode(GL_PROJECTION);
	// Reset The Projection Matrix
	glLoadIdentity();
	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	//gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}
int InitOpenGL() {
	// Get Device Dontext
	if (!(hDC = GetDC(hWnd))) {
		MessageBox(NULL, L"Can't Create A GL Device Context.",
			L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	// Check if Windows can find a matching Pixel Format
	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd))) {
		MessageBox(NULL, L"Can't Find A Suitable PixelFormat.",
			L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	// Try to set pixel format
	if (!SetPixelFormat(hDC, PixelFormat, &pfd)) {
		MessageBox(NULL, L"Can't Set The PixelFormat.", L"ERROR",
			MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	// Get a Rendering context
	if (!(hRC = wglCreateContext(hDC))) {
		MessageBox(NULL, L"Can't Create A GL Rendering Context.",
			L"ERROR", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}
	// Activate Rendering Context
	if (!wglMakeCurrent(hDC, hRC)) {
		MessageBox(NULL, L"Can't Activate The GL Rendering Context.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
			return 0;
	}

	//Initialise();
	//
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	return 1;
}

void DrawGLScene() {
	float y = 0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1, 1, 1, 1);
	//glLoadIdentity();

	// Locate camera in the (1,1,1) position then look at
	// origin (the intersection of the axis system)
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(15, 15, 20, // Camera's position
			  1, 0, 1, // Camera's target to look at
			  0.0, 1.0, 0.0); // Orientation of camera
	/*gluLookAt(1, 1, 1, // Camera's position
		0, 0, 0, // Camera’s target to look at
		0, 1, 0); // Orientation of camera*/


	//DrawSineCurves(-1.0, 0.0, 3.0, 1.0, 100);
	//sq->Draw();
	//DrawXYAxis();
	//DrawSpiral(50);
	//DrawCircle(0.1f, xPosCircle, yPosCircle, 24, 0, 0, 0, true);
	Draw3D_AxisSystem();


	//local rotations
	/*glPushMatrix();
	glRotatef(yRot, 0, 1, 0);
	yRot += 1;*/
	/*DrawCube();*/

	//Global Rotation
	//glRotatef(yRot, 0, 1, 0);//2nd transformation
	//glTranslatef(5, 0, 0);//1st transformations
	//yRot += 1;
	//DrawCube();


	//programming read from bottom to top

	//double timePassedAbsInSeconds = GetTimePassedSinceStart();
	//glPushMatrix();
	//yRot = (timePassedAbsInSeconds)*
	//	rotationalVelicityInDegreesPerSeconds;
	//glRotatef(yRot, 0, 1, 0);
	////DrawCheckerBoardCube();
	//DrawTexturedSquare();
	//glPopMatrix();

	/*double timePassedRelInSeconds = GetTimePassedSinceLastTime();
	glPushMatrix();
	yRot += timePassedRelInSeconds*
		rotationalVelicityInDegreesPerSeconds;
	glRotatef(yRot, 0, 1, 0);
	DrawTexturedSquare();
	glPopMatrix();*/

	//moveFoward -= accelerator;

	Missile::SetTimePassed(GetTimePassedSinceLastTime());
	tank->TankUpdate(GetTimePassedSinceLastTime());
	DrawPlane();
	glEnable(GL_LIGHTING);
	//glTranslatef(0, 0, moveFoward);
	tank->Draw();
	Missile::DrawAllMissile();
	
	glDisable(GL_LIGHTING);

	//tank->Draw();

}

void DrawSineCurves(float startX, float startY, int nrOfSineCurves, float scale, float definition) {
	glBegin(GL_LINE_STRIP);
	glColor3f(0, 0, 0);
	for (int i = 0; i < 5 * definition * nrOfSineCurves; i++)
	{
		glVertex2f(startX, abs(sin(degToRad((startX + 1) * (180 * nrOfSineCurves))) * scale + startY));
		startX += (0.5f / definition);
		scale -= 0.0023;
	}
	glEnd();
}

void DrawTank() {
	glEnable(GL_LIGHTING);

	tank->Draw();
	//glEnd();

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW); // Vertices in clockwise order
	glPolygonMode(GL_FRONT, GL_FILL); // Solid cube
									  
	GLfloat blue[] = { 0, 0, 1, 0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);

	//Back
	glBegin(GL_POLYGON);
	//glColor3f(0.0, 0.0, 1.0);//Red
	glVertex3f(1.0, 0.0, -1); // v7
	glVertex3f(1.0, 1.0, -1); // v6
	glVertex3f(-1.0, 1.0, -1.0); // v5
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
	glVertex3f(-1.0, 1.0, 1); // v1 Left Top
	glVertex3f(1.0, 1.0, 1); // v2 Right Top
	glVertex3f(1.0, 0.0, 1); // v3 Right Bottom
	glEnd();
	//Top
	glBegin(GL_POLYGON);
	//glColor3f(0.0, 0.0, 1.0);//black
	glVertex3f(-1.0, 1.0, 1); // v1
	glVertex3f(-1.0, 1.0, -1.0); // v5
	glVertex3f(1.0, 1.0, -1.0); // v6
	glVertex3f(1.0, 1.0, 1); // v2
	glEnd();
	//Right
	glBegin(GL_POLYGON);
	//glColor3f(0.0, 0.0, 1);//Blue
	glVertex3f(1.0, 0.0, 1); // v3
	glVertex3f(1.0, 1.0, 1); // v2
	glVertex3f(1.0, 1.0, -1.0); // v6
	glVertex3f(1.0, 0.0, -1.0); // v7
	glEnd();
	//Left
	glBegin(GL_POLYGON);
	//glColor3f(0.0, 0.0, 1.0);//Yellow
	glVertex3f(-1.0, 0.0, 1); // v0
	glVertex3f(-1.0, 0.0, -1); // v4
	glVertex3f(-1.0, 1.0, -1.0); // v5
	glVertex3f(-1.0, 1.0, 1.0); // v1
	glEnd();
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
}

void DrawCircle(float rad, float xc, float yc, int nrOfPoints, int r, int g, int b, bool wireframe) {
	float angularStep = 360.0 / nrOfPoints;
	float x, y = 0; // To store coordinates of circle vertices
	glColor3f(r, g, b); // Our circle will be drawn black
	if (wireframe) {
		// Draw the circle with a series of straight lines
		glBegin(GL_LINE_LOOP);
	}
	if (!wireframe) {
		glBegin(GL_POLYGON);
	}
	for (int i = 0; i<nrOfPoints; i++) {
		//We need to convert from degree to radians!
		x = xc + rad * cos(degToRad(angularStep*i));
		y = yc + rad * sin(degToRad(angularStep*i));
		glVertex2f(x, y);
	}
	glEnd();
}

void DrawColourfulTriangle() {
	// Draw a colourful triangle
	glBegin(GL_POLYGON);
	glColor3f(1, 0, 0); // Red
	glVertex2f(0, 0.5); // Top
	glColor3f(0, 1, 0); // Green
	glVertex2f(-0.5, -0.5); // Left
	glColor3f(0, 0, 1); // Blue
	glVertex2f(0.5, -0.5); // Right
	glEnd();
}

void DrawTriangleWireFrame() {
	glBegin(GL_LINE_LOOP);
	glColor3f(0, 0, 0);
	glVertex2f(0, 0.5); // Top
	glVertex2f(-0.5, -0.5); // Left
	glVertex2f(0.5, -0.5); // Right
	glEnd();
}

void DrawXYAxis() {
	glBegin(GL_LINES);
	glColor3f(0, 1, 0);
	glVertex2f(0, 0);
	glVertex2f(0, 10);
	glColor3f(1, 0, 0);
	glVertex2f(0, 0);
	glVertex2f(10, 0);
	glEnd();
}

void DrawSpiral(int a) {
	float x = 0;
	float y = 0;
	float value = 0.03;

	glBegin(GL_LINE_STRIP);
	bool xy = true;
	glColor3f(0, 0, 0);
	glVertex2f(0, 0);

	for (size_t i = 0; i < a; i++)
	{
		if (xy) {
			if (x <= 0) {
				x += value;
			}
			else if (x > 0) {
				x -= value;
			}
		}
		if (!xy) {
			if (y <= 0) {
				y += value;
			}
			else if (y > 0) {
				y -= value;
			}
			value += 0.03f;
		}
		glVertex2f(x, y);
		xy = !xy;
	}
	glEnd();
}

float degToRad(float degAngle) {
	double pi = 3.1415926535; // You could be more precise!
	return degAngle / 180.0 * pi;
}

void convertWindowToOpenGLCoordinates(int xWinPos ,int yWinPos, float &xOpenGLPos, float &yOpenGLPos) {
	// TODO: For you to implement
	xOpenGLPos = xWinPos / 200.0 - 1;
	yOpenGLPos = -yWinPos / 200.0 + 1;
}

void Draw3D_AxisSystem() {
	glBegin(GL_LINES);
	// Draw positive x-axis as red
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(100, 0, 0);
	// Draw positive y-axis as green
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 100, 0);
	// Draw positive z-axis as blue
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 100);
	glEnd();
}

void DrawCube() {
	glEnable(GL_LIGHTING);   
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW); // Vertices in clockwise order
	glPolygonMode(GL_FRONT, GL_FILL); // Solid cube
									  // Front Face in green
									  // Vertices in clock wise order v0, v1, v2, v3
	GLfloat blue[] = { 0, 0, 1, 0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue);

	//Back
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 1.0);//Red
	glVertex3f(1.0, -1.0, -1); // v7
	glVertex3f(1.0, 1.0, -1); // v6
	glVertex3f(-1.0, 1.0, -1.0); // v5
	glVertex3f(-1.0, -1.0, -1.0); // v4
	glEnd();
	//Bottom
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 1.0);//Grey
	glVertex3f(1.0, -1.0, 1.0); // v3
	glVertex3f(1.0, -1.0, -1.0); // v7
	glVertex3f(-1.0, -1.0, -1); // v4
	glVertex3f(-1.0, -1.0, 1); // v0
	glEnd();
	//Front
	glBegin(GL_POLYGON); // 0, 1, 2, 3
	glColor3f(0.0, 0.0, 1.0);//green
	glVertex3f(-1.0, -1.0, 1); // v0 Left Bottom
	glVertex3f(-1.0, 1.0, 1); // v1 Left Top
	glVertex3f(1.0, 1.0, 1); // v2 Right Top
	glVertex3f(1.0, -1.0, 1); // v3 Right Bottom
	glEnd();
	//Top
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 1.0);//black
	glVertex3f(-1.0, 1.0, 1); // v1
	glVertex3f(-1.0, 1.0, -1.0); // v5
	glVertex3f(1.0, 1.0, -1.0); // v6
	glVertex3f(1.0, 1.0, 1); // v2
	glEnd();
	//Right
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 1);//Blue
	glVertex3f(1.0, -1.0, 1); // v3
	glVertex3f(1.0, 1.0, 1); // v2
	glVertex3f(1.0, 1.0, -1.0); // v6
	glVertex3f(1.0, -1.0, -1.0); // v7
	glEnd();
	//Left
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 1.0);//Yellow
	glVertex3f(-1.0, -1.0, 1); // v0
	glVertex3f(-1.0, -1.0, -1); // v4
	glVertex3f(-1.0, 1.0, -1.0); // v5
	glVertex3f(-1.0, 1.0, 1.0); // v1
	glEnd();
	glDisable(GL_LIGHTING);

}

void DrawPlane() {
	glEnable(GL_TEXTURE_2D);
	glColor3f(0, 0, 0.7);
	/*glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
		64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER, GL_NEAREST);*/
	
	//glEnable(GL_LIGHTING);
	glBegin(GL_POLYGON);
	//glColor3f(0.0, 0.0, 1.0);//green
	glTexCoord2f(0.0, 0.0);
	glVertex3f(25.0, 0, 25.0); // v3
	glTexCoord2f(0.0, 1.0);
	glVertex3f(25.0, 0, -25.0); // v7
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-25.0, 0, -25.0); // v4
	glTexCoord2f(1.0, 0.0);
	glVertex3f(-25.0, 0, 25.0f); // v0
	glEnd();
	//glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
}

void SetupLight() {
	glShadeModel(GL_SMOOTH);
	GLfloat LightAmbient[] = { 0.5, 0.5, 0.5, 1 };
	GLfloat LightDiffuse[] = { 0.5, 0.5, 0.5, 1 };
	GLfloat LightSpecular[] = { 0.5, 0.5, 0.5, 1 };
	GLfloat LightPosition[] = { 10, 10, 10, 0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT0);
}

void Initialise() {
	SetupLight();

	CreateCheckerBoardTexture();
	mySphere = gluNewQuadric();
	gluQuadricDrawStyle(mySphere, GLU_FILL);
	tank = new Tank();
	StartTimer();

}
//void DrawSphere() {
//	glEnable(GL_LIGHTING);
//	gluQuadricNormals(mySphere, GLU_SMOOTH);
//	SetMaterial(&redPlasticMaterial);
//	gluSphere(mySphere, 1.0, 64, 64);
//	glDisable(GL_LIGHTING);
//}

void DrawPyramid()
{
	//glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW); // Vertices in clockwise order
	glPolygonMode(GL_FRONT, GL_FILL); // Solid cube
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0.0, 0);//Red
	glVertex3f(1.0, -1.0, -1); // v7
	glVertex3f(0, 1.0, 0); // v6
	glVertex3f(-1.0, -1.0, -1.0); // v4
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0.0, 0);//Red
	glVertex3f(-1.0, 3, -1); // v7
	glVertex3f(0, 1.0, 0); // v6
	glVertex3f(1.0, 3, -1.0); // v4
	glEnd();
	//Bottom
	glBegin(GL_POLYGON);
	glColor3f(0.5, 0.5, 0.5);//Grey
	glVertex3f(1.0, -1.0, 1.0); // v3
	glVertex3f(1.0, -1.0, -1.0); // v7
	glVertex3f(-1.0, -1.0, -1); // v4
	glVertex3f(-1.0, -1.0, 1); // v0
	glEnd();
	//Front
	glBegin(GL_TRIANGLES); // 0, 1, 2, 3
	glColor3f(0.0, 1.0, 0);//green
	glVertex3f(-1.0, -1.0, 1); // v0 Left Bottom
	glVertex3f(0, 1.0, 0); // v2 Right Top
	glVertex3f(1.0, -1.0, 1); // v3 Right Bottom
	glEnd();
	//invert Front
	glBegin(GL_TRIANGLES); // 0, 1, 2, 3
	glColor3f(0.0, 1.0, 0);//green
	glVertex3f(1, 3, 1); // v0 Left Bottom
	glVertex3f(0, 1.0, 0); // v2 Right Top
	glVertex3f(-1, 3, 1); // v3 Right Bottom
	glEnd();
	//Top
	glBegin(GL_POLYGON);
	glColor3f(0.0, 0.0, 0);//black
	glVertex3f(-1.0, 3.0, 1); // v1
	glVertex3f(-1.0, 3, -1.0); // v5
	glVertex3f(1.0, 3, -1.0); // v6
	glVertex3f(1.0, 3, 1); // v2
	glEnd();
	//Right
	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 0.0, 1);//Blue
	glVertex3f(1.0, -1.0, 1); // v3
	glVertex3f(0, 1.0, 0); // v6
	glVertex3f(1.0, -1.0, -1.0); // v7
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3f(0.0, 0.0, 1);//Blue
	glVertex3f(1.0, 3.0, -1); // v3
	glVertex3f(0, 1.0, 0); // v6
	glVertex3f(1.0, 3.0, 1.0); // v7
	glEnd();
	//Left
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 1.0, 0);//Yellow
	glVertex3f(-1.0, -1.0, 1); // v0
	glVertex3f(-1.0, -1.0, -1); // v4
	glVertex3f(0, 1.0, 0); // v6
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 1.0, 0);//Yellow
	glVertex3f(-1.0, 3.0, -1); // v0
	glVertex3f(-1.0, 3.0, 1); // v4
	glVertex3f(0, 1.0, 0); // v6
	glEnd();
}

void NormalizeVector(float &Px, float &Py, float &Pz) {
	float length = (Px * Px + Py * Py + Pz * Pz);

	length /= 5;

	Px /= length;
	Py /= length;
	Pz /= length;
}

//void DrawNormal(float nx, float ny, float nz, float cx, float cy, float cz) {
//	glLineWidth(2.5);
//	glColor3f(1.0, 0.0, 0.0);
//	glBegin(GL_LINES);
//	glVertex3f(nx, ny, nz);
//	glVertex3f(cx, cy, cz);
//	glEnd();
//}

void CalculateNormal(float firstX, float firstY, float firstZ,
	float secondX, float secondY, float secondZ,
	float thirdX, float thirdY, float thirdZ) {

	float Nx, Ny, Nz;

	Nx = firstY * (secondZ - thirdZ) + secondY * (thirdZ - firstZ) + thirdY * (firstZ - secondZ);
	Ny = firstZ * (secondX - thirdX) + secondZ * (thirdX - firstX) + thirdZ * (firstX - secondX);
	Nz = firstX * (secondY - thirdY) + secondX * (thirdY - firstY) + thirdX * (firstY - secondY);

	NormalizeVector(Nx, Ny, Nz);

	glNormal3f(Nx, Ny, Nz);

	float Cx, Cy, Cz;

	Cx = (firstX + secondX + thirdX) / 3.0;
	Cy = (firstY + secondY + thirdY) / 3.0;
	Cz = (firstZ + secondZ + thirdZ) / 3.0;

	DrawNormal(Nx, Ny, Nz, Cx, Cy, Cz);
}

void DrawNormal(float xN, float yN, float zN, float sX, float sY, float sZ)
{
	glBegin(GL_LINES);
	glVertex3f(sX, sY, sZ);
	glVertex3f(xN, yN, zN);
	glEnd();
}

void DrawPyramidN(int config)
{
	GLfloat Lightgrey[] = { 0.8,0.8, 0.8, 0 };
	Vector3f normal;
	Vector3f surface;
	Vector3f linenormal;
	std::vector<Vector3f> vertices;

	//glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW); // Vertices in clockwise order

	if (config == GL_TRIANGLES)
	{
		glEnable(GL_LIGHTING);
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, Lightgrey);
	}
	else
	{
		glLineWidth(2.0f);
		glColor3f(0, 0, 0);
	}
	//Back
	vertices = { Vector3f(-1.0, -1.0, -1.0), Vector3f(0, 1.0, 0), Vector3f(1.0, -1.0, -1) };
	normal = Vector3f::CalculateNormal(vertices[0], vertices[1], vertices[2]);
	normal.Normalise();
	surface = Vector3f::CalculateCenter(vertices[0], vertices[1], vertices[2]);
	linenormal = surface + normal;
	DrawNormal(linenormal.X, linenormal.Y, linenormal.Z, surface.X, surface.Y, surface.Z);
	glBegin(config);
	glNormal3f(normal.X, normal.Y, normal.Z);
	for (size_t i = 0; i < 3; i++)
	{
		glVertex3f(vertices[i].X, vertices[i].Y, vertices[i].Z);
	}
	glEnd();

	//Right
	vertices = { Vector3f(1.0, -1.0, -1.0), Vector3f(0, 1.0, 0), Vector3f(1.0, -1.0, 1) };
	normal = Vector3f::CalculateNormal(vertices[0], vertices[1], vertices[2]);
	normal.Normalise();
	surface = Vector3f::CalculateCenter(vertices[0], vertices[1], vertices[2]);
	linenormal = surface + normal;
	DrawNormal(linenormal.X, linenormal.Y, linenormal.Z, surface.X, surface.Y, surface.Z);
	glBegin(config);
	glNormal3f(normal.X, normal.Y, normal.Z);
	for (size_t i = 0; i < 3; i++)
	{
		glVertex3f(vertices[i].X, vertices[i].Y, vertices[i].Z);
	}
	glEnd();
	//Bottom
	vertices = { Vector3f(1.0, -1.0, -1.0),Vector3f(1.0, -1.0, 1.0), Vector3f(-1.0, -1.0, -1) };
	normal = Vector3f::CalculateNormal(vertices[0], vertices[1], vertices[2]);
	normal.Normalise();
	surface = Vector3f::CalculateCenter(vertices[0], vertices[1], vertices[2]);
	linenormal = surface + normal;
	DrawNormal(linenormal.X, linenormal.Y, linenormal.Z, surface.X, surface.Y, surface.Z);
	glBegin(config);
	glNormal3f(normal.X, normal.Y, normal.Z);
	for (size_t i = 0; i < 3; i++)
	{
		glVertex3f(vertices[i].X, vertices[i].Y, vertices[i].Z);
	}
	glEnd();

	vertices = { Vector3f(-1.0, -1.0, 1),Vector3f(-1.0, -1.0, -1.0), Vector3f(1.0, -1.0, 1.0) };
	normal = Vector3f::CalculateNormal(vertices[0], vertices[1], vertices[2]);
	normal.Normalise();
	surface = Vector3f::CalculateCenter(vertices[0], vertices[1], vertices[2]);
	linenormal = surface + normal;
	DrawNormal(linenormal.X, linenormal.Y, linenormal.Z, surface.X, surface.Y, surface.Z);
	glBegin(config);
	glNormal3f(normal.X, normal.Y, normal.Z);
	for (size_t i = 0; i < 3; i++)
	{
		glVertex3f(vertices[i].X, vertices[i].Y, vertices[i].Z);
	}
	glEnd();

	//Left
	vertices = { Vector3f(-1.0, -1.0, -1),Vector3f(-1.0, -1.0, 1), Vector3f(0, 1.0, 0) };
	normal = Vector3f::CalculateNormal(vertices[0], vertices[1], vertices[2]);
	normal.Normalise();
	surface = Vector3f::CalculateCenter(vertices[0], vertices[1], vertices[2]);
	linenormal = surface + normal;
	DrawNormal(linenormal.X, linenormal.Y, linenormal.Z, surface.X, surface.Y, surface.Z);
	glBegin(config);
	glNormal3f(normal.X, normal.Y, normal.Z);
	for (size_t i = 0; i < 3; i++)
	{
		glVertex3f(vertices[i].X, vertices[i].Y, vertices[i].Z);
	}
	glEnd();
	//Front
	vertices = { Vector3f(0, 1.0, 0),Vector3f(-1.0, -1.0, 1), Vector3f(1.0, -1.0, 1) };
	normal = Vector3f::CalculateNormal(vertices[0], vertices[1], vertices[2]);
	normal.Normalise();
	surface = Vector3f::CalculateCenter(vertices[0], vertices[1], vertices[2]);
	linenormal = surface + normal;
	DrawNormal(linenormal.X, linenormal.Y, linenormal.Z, surface.X, surface.Y, surface.Z);
	glBegin(config);
	glNormal3f(normal.X, normal.Y, normal.Z);
	for (size_t i = 0; i < 3; i++)
	{
		glVertex3f(vertices[i].X, vertices[i].Y, vertices[i].Z);
	}
	glEnd();

	if (config == GL_TRIANGLES)
	{
		glDisable(GL_LIGHTING);
	}
}

void DrawTexturedSquare() {
	glEnable(GL_TEXTURE_2D);
	glColor3f(1, 1, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
		64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// Draw a rectangle with a texture mapped onto it
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, -1.0, 0); // v0 Left Bottom   
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0, 1.0, 0); // v1 Left Top
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 1.0, 0); // v2 Right Top
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, -1.0, 0); // v3 Right Bottom
	glEnd();
	glDisable(GL_TEXTURE_2D);

	// Specify an indent between top two vertices
	// If indent is 1 then we have a square
	// If indent is 0 then we have a triangle
	//float indent = 0;//Try these values 0.75, 0.5, 0.25, 0
	//glBegin(GL_POLYGON); // 0, 1, 2, 3
	//glTexCoord2f(0.0, 0.0);
	//glVertex3f(-1.0, -1.0, 0); // v0 Left Bottom
	//glTexCoord2f(0.0, 1.0);
	//glVertex3f(-indent, 1.0, 0);// v1 Left-Centre Top
	//glTexCoord2f(1.0, 1.0);
	//glVertex3f(indent, 1.0, 0);// v2 Right-Centre Top
	//glTexCoord2f(1.0, 0.0);
	//glVertex3f(1.0, -1.0, 0); // v3 Right Bottom
	//glEnd();
}