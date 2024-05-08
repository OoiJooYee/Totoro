/*
	FACULTY OF COMPUTING SESSION 2022/2023 SEMESTER 2
	FUNDAMENTAL OF COMPUTER GRAPHICS SECTION 02 (SECV 2213 - 02)
	DR. GOH EG SU
	PROJECT
	GROUP: 10
	THEME: MY NEIGHBOUR TOTORO
	MEMBERS: HENG XING YU A21EC0183, OOI JOO YEE A21EC0218, WAI JIA WEN A21EC0139
*/

#include <windows.h>
#include <gl/glut.h>
#include <math.h>
#include "imageloader.h"

// angle of rotation for the camera direction
float angle = 0.0f;

// actual vector representing the camera's diection
float lx = 0.0f, lz = -1.0f, ly = 0.0f;

// XZ position of the camera
float x = 0.0f, z = 5.0f, y = 1.0f;

// the key states. These variable will be zero
// when no key is being presses
float deltaAngle = 0.0f;
float deltaMove = 0;
int xOrigin = -1, yOrigin = -1;

// angle of rotation for the body of totoro
float rotateWholeBody = 0.0f, incrementRotateBody = 0.0f;
float rotateHead = 0.0f, incrementRotateHead = 0.0f;
float nodHead = 0.0f, incrementNodHead = 0.0f;

float leftShoulder = 0.0f, incrementLeftShoulder = 0.0f;
float leftShoulderRX = 0.0f;
float leftShoulderRZ = 1.0f;
float leftElbow = 0.0f, incrementLeftElbow = 0.0f;
float leftPalm = 0.0f, incrementLeftPalm = 0.0f;
float leftFingers = 0.0f, incrementLeftFingers = 0.0f;

float rightShoulder = 0.0f, incrementRightShoulder = 0.0f;
float rightShoulderRX = 0.0f;
float rightShoulderRZ = 1.0f;
float rightElbow = 0.0f, incrementRightElbow = 0.0f;
float rightPalm = 0.0f, incrementRightPalm = 0.0f;
float rightFingers = 0.0f, incrementRightFingers = 0.0f;

float leftLeg = 0.0f;
float rightLeg = 0.0f;
float walkx = 0.0f;
float walkz = 0.0f;

// lights on
GLfloat ambientColorOn[] = { 0.6f, 0.6f, 0.6f, 1.0f }; // Color(0.6, 0.6, 0.6)
GLfloat lightColor0On[] = { 0.8f, 0.8f, 0.8f, 1.0f }; // Color(0.8, 0.8, 0.8)
GLfloat lightColor1On[] = { 0.6f, 0.6f, 0.6f, 1.0f }; // Color(0.6, 0.6, 0.6)

// lights off
GLfloat ambientColorOff[] = { 0.2f, 0.2f, 0.2f, 1.0f }; // Color(0.2, 0.2, 0.2)
GLfloat lightColor0Off[] = { 0.2f, 0.2f, 0.2f, 1.0f }; // Color(0.2, 0.2, 0.2)
GLfloat lightColor1Off[] = { 0.2f, 0.2f, 0.2f, 1.0f }; // Color(0.2, 0.2, 0.2)

GLfloat lightPos0[] = { 2.0f, 0.0f, 8.0f, 1.0f }; // Positioned at (2, 0, 8)
GLfloat lightPos1[] = { -8.0f, 4.0f, 5.0f, 0.0f }; // Positioned at (-8, 4, 5)

// The id of the texture
GLuint _textureId1;
GLuint _textureId2;

// Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); // Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); // Tell OpenGL which texture to edit
	// Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D, // Always GL_TEXTURE_2D
		0, // 0 for now
		GL_RGB, // Format OpenGL uses for image
		image->width, image->height, // Width and height
		0, // The border of the image
		GL_RGB, // GL_RGB, because pixels are stored in RGB format
		GL_UNSIGNED_BYTE, // GL_UNSIGNED_BYTE, because pixels are stored as unsigned numbers
		image->pixels); // The actual pixel data
	return textureId; // Returns the id of the texture
}

void changeSize(int w, int h)
{
	// Prevent a divide by zero, when window is too short
	if (h == 0)
		h = 1;
	float ratio = w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

// Initializes 3D rendering
void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING); // Enable lighting
	glEnable(GL_LIGHT0); // Enable light #0 - light color
	glEnable(GL_LIGHT1); // Enable light #1 - light position
	glEnable(GL_NORMALIZE); // Automatically normalize normals
	glEnable(GL_COLOR_MATERIAL);

	// Add ambient light
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColorOn);

	// Add positioned light
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0On);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	// Add directed light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1On);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	Image* image1 = loadBMP("C:\\zmisc\\forest.bmp");
	_textureId1 = loadTexture(image1);
	delete image1;

	Image* image2 = loadBMP("C:\\zmisc\\ground.bmp");
	_textureId2 = loadTexture(image2);
	delete image2;
}

void drawTotoro()
{
	// PIVOT POINT FOR WHOLE BODY ROTATION (0, 1.8, -1.5)
	glTranslatef(0.0f, 1.8f, -1.5f);
	glTranslatef(walkx, 0.0f, walkz);
	glRotatef(rotateWholeBody, 0.0f, 1.0f, 0.0f); // rotate whole body

	// Draw Body
	glPushMatrix();
	{	
		// Draw Head
		glPushMatrix();
		{
			glRotatef(rotateHead, 0.0f, 1.0f, 0.0f); // rotate head
			glRotatef(nodHead, 1.0f, 0.0f, 0.0f); // nod head

			// Draw Face
			glScaled(1.4, 1.0, 1.0);
			glColor3f(0.3333, 0.3374, 0.3569); // GREY
			glutSolidSphere(0.5, 30, 30);

			// Draw Ears
			glPushMatrix();
				glTranslatef(-0.3f, 0.5f, 0.0f);
				glPushMatrix();
					glRotatef(-90, 1.0f, 0.0f, -0.3f);
					glutSolidCone(0.1f, 0.7f, 10, 10);
				glPopMatrix();
				glPushMatrix();
					glTranslatef(0.025f, -0.1f, 0.0f);
					glRotatef(-90, 1.0f, 0.0f, -0.3f);
					gluCylinder(gluNewQuadric(), 0.05f, 0.05f, 0.2f, 10, 10);
				glPopMatrix();
				glTranslatef(0.6f, 0.0f, 0.0f);
				glPushMatrix();
					glRotatef(-90, 1.0f, 0.0f, 0.3f);
					glutSolidCone(0.1f, 0.7f, 10, 10);
				glPopMatrix();
				glPushMatrix();
					glTranslatef(-0.025f, -0.1f, 0.0f);
					glRotatef(-90, 1.0f, 0.0f, 0.3f);
					gluCylinder(gluNewQuadric(), 0.05f, 0.05f, 0.2f, 10, 10);
				glPopMatrix();
			glPopMatrix();

			// Draw Eyeballs
			glPushMatrix();
				glTranslatef(0.22f, 0.02f, 0.4f);
				glColor3f(1, 1, 1); // WHITE
				glutSolidSphere(0.12, 20, 20);
				glTranslatef(-0.44f, 0.0f, 0.0f);
				glutSolidSphere(0.12, 20, 20);
			glPopMatrix();

			// Draw Pupils
			glPushMatrix();
				glColor3f(0, 0, 0); // BLACK
				glTranslatef(0.21f, 0.02f, 0.5f);
				glutSolidSphere(0.05, 20, 20);
				glTranslatef(-0.42f, 0.0f, 0.0f);
				glutSolidSphere(0.05, 20, 20);
			glPopMatrix();

			// Draw Nose
			glPushMatrix();
				glTranslatef(0.0f, 0.02f, 0.5f);
				glRotatef(100, 1.0f, 0.0f, 0.0f);
				glColor3f(0.1373, 0.1412, 0.0549);
				glutSolidCone(0.08f, 0.08f, 6, 4);
			glPopMatrix();

			// Draw Mouth
			glBegin(GL_LINES);
				glVertex3f(-0.03f, -0.125f, 0.5f);
				glVertex3f(0.03f, -0.125f, 0.5f);
			glEnd();

			// Draw Whiskers
			glPushMatrix();
				glColor3f(0.1373, 0.1412, 0.0549); // DARK GREY
				glPushMatrix();
					glTranslatef(0.4f, -0.1f, 0.3f);
					glRotatef(180, 0.5f, 0.0f, 1.0f);
					glutSolidCone(0.015f, 0.35f, 6, 2);
					glTranslatef(0.0f, -0.075f, 0.0f);
					glutSolidCone(0.015f, 0.4f, 6, 2);
					glTranslatef(0.0f, -0.075f, 0.0f);
					glutSolidCone(0.015f, 0.35f, 6, 2);
				glPopMatrix();
				glPushMatrix();
					glTranslatef(-0.4f, -0.1f, 0.3f);
					glRotatef(180, 0.5f, 0.0f, -1.0f);
					glutSolidCone(0.015f, 0.35f, 6, 2);
					glTranslatef(0.0f, -0.075f, 0.0f);
					glutSolidCone(0.015f, 0.4f, 6, 2);
					glTranslatef(0.0f, -0.075f, 0.0f);
					glutSolidCone(0.015f, 0.35f, 6, 2);
				glPopMatrix();
			glPopMatrix();
		}glPopMatrix();

		// Draw Body Surface
		glTranslatef(0.0f, -1.3f, 0.0f);
		glColor3f(0.3333, 0.3374, 0.3569); // GREY
		glScaled(1.5, 1.6, 1.3);
		glutSolidSphere(0.8, 40, 40);

		// Draw Stomach
		glPushMatrix();
		{
			glTranslatef(0.0f, -0.045f, 0.25f);
			glColor3f(0.7608, 0.6980, 0.5020); // SANDY GREY
			glScaled(1.1f, 1.05f, 1.0f);
			glutSolidSphere(0.65, 40, 40);
			// Draw Fur Pattern On Stomach
			glColor3f(0.3333, 0.3374, 0.3569); // GREY
			glPushMatrix();
				glTranslatef(-0.1f, 0.325f, 0.53f);
				gluPartialDisk(gluNewQuadric(), 0.07f, 0.11f, 10, 30, 300, 120);
				glTranslatef(0.2f, 0.0f, 0.0f);
				gluPartialDisk(gluNewQuadric(), 0.07f, 0.11f, 10, 30, 300, 120);
				glTranslatef(-0.4f, 0.0f, -0.05f);
				gluPartialDisk(gluNewQuadric(), 0.06f, 0.10f, 10, 30, 300, 120);
				glTranslatef(0.6f, 0.0f, 0.0f);
				gluPartialDisk(gluNewQuadric(), 0.06f, 0.10f, 10, 30, 300, 120);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.0f, 0.2f, 0.6f);
				gluPartialDisk(gluNewQuadric(), 0.07f, 0.11f, 10, 30, 300, 120);
				glTranslatef(-0.2f, 0.0f, -0.018f);
				gluPartialDisk(gluNewQuadric(), 0.06f, 0.10f, 10, 30, 300, 120);
				glTranslatef(0.4f, 0.0f, 0.0f);
				gluPartialDisk(gluNewQuadric(), 0.06f, 0.10f, 10, 30, 300, 120);
				glTranslatef(-0.6f, 0.0f, -0.085f);
				gluPartialDisk(gluNewQuadric(), 0.05f, 0.09f, 10, 30, 300, 120);
				glTranslatef(0.8f, 0.0f, 0.0f);
				gluPartialDisk(gluNewQuadric(), 0.05f, 0.09f, 10, 30, 300, 120);
			glPopMatrix();
		}glPopMatrix();

		// Draw Left Hand
		glPushMatrix();
		{
		glColor3f(0.3283, 0.3324, 0.3519); // DARKER GREY
			// Draw Upper Arm
			glPushMatrix();
				glTranslatef(0.35f, 0.5f, 0.0f);
				glRotatef(leftShoulder, leftShoulderRX, 0.0f, leftShoulderRZ); // rotate left arm
				glScaled(1.2, 1.0, 1.2);
				glutSolidSphere(0.24f, 30, 30);
				glPushMatrix();
					glRotatef(110, 1.0f, 0.0f, 0.5f);
					gluCylinder(gluNewQuadric(), 0.24f, 0.2f, 0.4f, 30, 30);
				glPopMatrix();

				// Draw Forearms
				glTranslatef(0.2f, -0.32f, -0.025f);
				glRotatef(leftElbow, 1.0f, 0.0f, 0.0f); // rotate left elbow
				glutSolidSphere(0.2f, 30, 30);
				glPushMatrix();
				glRotatef(110, 1.0f, 0.0f, 0.5f);
				gluCylinder(gluNewQuadric(), 0.2f, 0.15f, 0.3f, 30, 30);
			glPopMatrix();

				// Draw Hand
				glTranslatef(0.135f, -0.22f, -0.025f);
				glRotatef(leftPalm, 0.0f, 0.0f, 1.0f); // rotate left palm
				glutSolidSphere(0.15f, 30, 30);

				// Draw Finger
				glColor3f(0.1373, 0.1412, 0.0549); // DARK GREY
				glTranslatef(0.075f, -0.1f, 0.0f);
				glRotatef(leftFingers, 0.0f, 0.0f, 1.0f); // rotate left fingers
				glPushMatrix();
					glRotatef(90, 0.5f, 0.0f, 0.0f);
					glutSolidCone(0.02f, 0.15f, 6, 2);
					glTranslatef(0.0f, 0.05f, 0.0f);
					glutSolidCone(0.02f, 0.14f, 6, 2);
					glTranslatef(0.0f, 0.05f, -0.03f);
					glutSolidCone(0.02f, 0.13f, 6, 2);
					glTranslatef(0.0f, -0.15f, 0.03f);
					glutSolidCone(0.02f, 0.14f, 6, 2);
					glTranslatef(0.0f, -0.05f, -0.03f);
					glutSolidCone(0.02f, 0.13f, 6, 2);
				glPopMatrix();
			glPopMatrix();
		}glPopMatrix();

		// Draw Right Hand
		glPushMatrix();
		{
		glColor3f(0.3283, 0.3324, 0.3519); // DARKER GREY
			// Draw Upper Arm
			glPushMatrix();
				glTranslatef(-0.35f, 0.5f, 0.0f);
				glRotatef(rightShoulder, rightShoulderRX, 0.0f, rightShoulderRZ); // rotate right arm
				glScaled(1.2, 1.0, 1.2);
				glutSolidSphere(0.24f, 30, 30);
				glPushMatrix();
				glRotatef(110, 1.0f, 0.0f, -0.5f);
				gluCylinder(gluNewQuadric(), 0.24f, 0.2f, 0.4f, 30, 30);
				glPopMatrix();

				// Draw Forearms
				glTranslatef(-0.2f, -0.32f, -0.025f);
				glRotatef(rightElbow, 1.0f, 0.0f, 0.0f); // rotate right elbow
				glutSolidSphere(0.2f, 30, 30);
				glPushMatrix();
				glRotatef(110, 1.0f, 0.0f, -0.5f);
				gluCylinder(gluNewQuadric(), 0.2f, 0.15f, 0.3f, 30, 30);
			glPopMatrix();

				// Draw Hand
				glTranslatef(-0.135f, -0.22f, -0.025f);
				glRotatef(rightPalm, 0.0f, 0.0f, 1.0f); // rotate right palm
				glutSolidSphere(0.15f, 30, 30);

				// Draw Finger
				glColor3f(0.1373, 0.1412, 0.0549); // DARK GREY
				glTranslatef(-0.075f, -0.1f, 0.0f);
				glRotatef(rightFingers, 0.0f, 0.0f, 1.0f); // rotate right fingers
				glPushMatrix();
					glRotatef(90, 0.5f, 0.0f, 0.0f);
					glutSolidCone(0.02f, 0.15f, 6, 2);
					glTranslatef(0.0f, 0.05f, 0.0f);
					glutSolidCone(0.02f, 0.14f, 6, 2);
					glTranslatef(0.0f, 0.05f, -0.03f);
					glutSolidCone(0.02f, 0.13f, 6, 2);
					glTranslatef(0.0f, -0.15f, 0.03f);
					glutSolidCone(0.02f, 0.14f, 6, 2);
					glTranslatef(0.0f, -0.05f, -0.03f);
					glutSolidCone(0.02f, 0.13f, 6, 2);
				glPopMatrix();
			glPopMatrix();
		}glPopMatrix();

		// Draw Legs
		glPushMatrix();
		{
			glColor3f(0.3283, 0.3324, 0.3519); // DARKER GREY
			glTranslatef(0.0f, -0.4f, 0.0f);
			// Draw Right Thigh
			glPushMatrix();
				glTranslatef(-0.4f, 0.0f, 0.0f);
				glRotatef(rightLeg, 1.0f, 0.0f, 0.0f); // rotate right leg
				glScaled(1.0f, 1.6f, 1.0f);
				glutSolidSphere(0.25, 30, 30);

					glColor3f(0.1373, 0.1412, 0.0549); // DARK GREY
					// Draw Right Toes
					glPushMatrix();
						glTranslatef(0.0f, -0.225f, 0.08f);
						glutSolidCone(0.016f, 0.15f, 6, 2);
						glTranslatef(-0.03f, 0.0f, 0.0f);
						glutSolidCone(0.016f, 0.14f, 6, 2);
						glTranslatef(-0.03f, 0.0f, -0.03f);
						glutSolidCone(0.016f, 0.13f, 6, 2);
						glTranslatef(0.09f, 0.0f, 0.03f);
						glutSolidCone(0.016f, 0.14f, 6, 2);
						glTranslatef(0.03f, 0.0f, -0.03f);
						glutSolidCone(0.016f, 0.13f, 6, 2);
					glPopMatrix();
			glPopMatrix();

			// Draw Left Thigh
			glColor3f(0.3283, 0.3324, 0.3519); // DARKER GREY
			glPushMatrix();
				glTranslatef(0.4f, 0.0f, 0.0f);
				glRotatef(leftLeg , 1.0f, 0.0f, 0.0f); // rotate left leg
				glScaled(1.0f, 1.6f, 1.0f);
				glutSolidSphere(0.25, 30, 30);

					glColor3f(0.1373, 0.1412, 0.0549); // DARK GREY
					// Draw Left Toes
					glPushMatrix();
						glTranslatef(0.0f, -0.225f, 0.08f);
						glutSolidCone(0.016f, 0.15f, 6, 2);
						glTranslatef(-0.03f, 0.0f, 0.0f);
						glutSolidCone(0.016f, 0.14f, 6, 2);
						glTranslatef(-0.03f, 0.0f, -0.03f);
						glutSolidCone(0.016f, 0.13f, 6, 2);
						glTranslatef(0.09f, 0.0f, 0.03f);
						glutSolidCone(0.016f, 0.14f, 6, 2);
						glTranslatef(0.03f, 0.0f, -0.03f);
						glutSolidCone(0.016f, 0.13f, 6, 2);
					glPopMatrix();
			glPopMatrix();
		}glPopMatrix();
		
		// Draw Tail
		glPushMatrix();
			glTranslatef(0.0f, -0.5f, -0.8f);
			glRotatef(90, 1.0f, 0.0f, 0.0f);
			glScaled(1.2, 1.8, 1.1);
			glColor3f(0.3283, 0.3324, 0.3519); // DARKER GREY
			glutSolidSphere(0.25, 30, 30);
		glPopMatrix();
	}glPopMatrix();
}

void computePos(float deltaMove)
{
	x += deltaMove * lx * 0.1f;
	z += deltaMove * lz * 0.1f;
}

void renderScene(void)
{
	if (deltaMove)
		computePos(deltaMove);

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	// Reset transformations
	glLoadIdentity();
	// Set the camera
	gluLookAt(x, y, z, x + lx, y + ly, z + lz, 0.0f, 1.0f, 0.0f);

	glClearColor(0.5510, 0.6216, 0.5078, 1);

	// Draw Background with texture map
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureId1);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_QUADS);
			glNormal3f(0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-25.0f, 25.0f, -25.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(25.0f, 25.0f, -25.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(25.0f, -1.0f, -25.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-25.0f, -1.0f, -25.0f);
		glEnd();
	glPopMatrix();

	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureId1);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_QUADS);
			glNormal3f(1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-25.0f, 25.0f, 50.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-25.0f, 25.0f, -25.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-25.0f, -1.0f, -25.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-25.0f, -1.0f, 50.0f);
		glEnd();
	glPopMatrix();

	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureId1);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_QUADS);
			glNormal3f(-1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(25.0f, 25.0f, -25.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(25.0f, 25.0f, 50.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(25.0f, -1.0f, 50.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(25.0f, -1.0f, -25.0f);
		glEnd();
	glPopMatrix();

	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureId1);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_QUADS);
			glNormal3f(0.0f, 0.0f, -1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-25.0f, 25.0f, 50.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(25.0f, 25.0f, 50.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(25.0f, -1.0f, 50.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-25.0f, -1.0f, 50.0f);
		glEnd();
	glPopMatrix();

	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureId1);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_QUADS);
			glNormal3f(0.0f, -1.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-25.0f, 25.0f, -25.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-25.0f, 25.0f, 50.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(25.0f, 25.0f, 50.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(25.0f, 25.0f, -25.0f);
		glEnd();
	glPopMatrix();

	// Draw Floor with texture map
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureId2);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_QUADS);
			glNormal3f(0.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-25.0f, -1.0f, -25.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-25.0f, -1.0f, 50.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(25.0f, -1.0f, 50.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(25.0f, -1.0f, -25.0f);
		glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	// Draw Totoro
	drawTotoro();

	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int xx, int yy)
{
	if (key == 27)
		exit(0);
	switch (key)
	{
	// lights on
	case 'o':
	{
		// Set ambient light colour
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColorOn);

		// Set positioned light colour
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0On);

		// Set directed light colour
		glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1On);
	}break;

	// lights off
	case 'O':
	{
		// Set ambient light colour
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColorOff);

		// Set positioned light colour
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0Off);

		// Set directed light colour
		glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1Off);
	}break;

	// music on
	case 'm': PlaySound(TEXT("C:\\zmisc\\totoro.wav"), NULL, SND_ASYNC); break;

	// music off
	case 'M': PlaySound(NULL, 0, 0); break;
	
	// whole body rotation
	case 'b': incrementRotateBody = 2; break;
	case 'B': incrementRotateBody = 0; break;

	// head rotation
	case 'h': incrementRotateHead = 2; break;
	case 'H': incrementRotateHead = 0; break;

	// head nod
	case 'n': incrementNodHead = 0.5; break;
	case 'N': incrementNodHead = 0; break;

	// right hand
	case 'r': incrementRightShoulder = 2; break;
	case 'R': incrementRightShoulder = 0 ; break;

	// right elbow
	case 'e': incrementRightElbow = 2; break;
	case 'E': incrementRightElbow = 0; break;
	
	// right palm
	case 'p': incrementRightPalm = 2; break;
	case 'P': incrementRightPalm = 0; break;

	// right fingers
	case 'f': incrementRightFingers = 2; break;
	case 'F': incrementRightFingers = 0; break;

	// left hand
	case 'l': incrementLeftShoulder = 2; break;
	case 'L': incrementLeftShoulder = 0; break;

	// left elbow
	case '1': incrementLeftElbow = 2; break;
	case '2': incrementLeftElbow = 0; break;

	// left palm
	case '3': incrementLeftPalm = 2; break;
	case '4': incrementLeftPalm = 0; break;

	// left fingers
	case '5': incrementLeftFingers = 2; break;
	case '6': incrementLeftFingers = 0; break;

	// walk to left
	case 'w':
		incrementRotateBody = 0;
		incrementRightShoulder = 0; 
		incrementLeftShoulder = 0; 
		incrementRightElbow = 0;
		incrementLeftElbow = 0;
		if (walkx < 50.0) walkx += 0.5;
		if (walkz < 50.0) walkz += 0.5;
		if (rotateWholeBody < 10 || rotateWholeBody > 90)
		rotateWholeBody = 45;
		if (leftLeg == 0)
		{
			leftLeg = -35;
			rightLeg = 35;
		}
		else
		{
			rightLeg = 0;
			leftLeg = 0;
		}
		if (rightShoulder == 0)
		{
			rightShoulder = -45;
			rightShoulderRX = 1.0f;
			rightShoulderRZ = 0.0f;
			rightElbow = -25;

			leftShoulder = 45;
			leftShoulderRX = 1.0f;
			leftShoulderRZ = 0.0f;
			leftElbow = -25;
		}
		else
		{
			rightShoulder = 0;
			rightShoulderRX = 0.0f;
			rightShoulderRZ = 1.0f;
			rightElbow = 0;

			leftShoulder = 0;
			leftShoulderRX = 0.0f;
			leftShoulderRZ = 1.0f;
			leftElbow = 0;
		}

		glutPostRedisplay(); break;

	// walk to right
	case 'W':
		incrementRotateBody = 0;
		incrementRightShoulder = 0; 
		incrementLeftShoulder = 0; 
		incrementRightElbow = 0;
		incrementLeftElbow = 0;
		if (walkx > -50.0) walkx -= 0.5;
		if (walkz < 50.0) walkz += 0.5;
		if (rotateWholeBody < 270 || rotateWholeBody > 350)
		rotateWholeBody = 315;
		if (rightLeg == 0)
		{
			rightLeg = -35;
			leftLeg = 35;
		}
		else
		{
			rightLeg = 0;
			leftLeg = 0;
		}
		if (leftShoulder == 0)
		{
			leftShoulder = -45;
			leftShoulderRX = 1.0f;
			leftShoulderRZ = 0.0f;
			leftElbow = -25;

			rightShoulder = 45;
			rightShoulderRX = 1.0f;
			rightShoulderRZ = 0.0f;
			rightElbow = -25;
		}
		else
		{
			leftShoulder = 0;
			leftShoulderRX = 0.0f;
			leftShoulderRZ = 1.0f;
			leftElbow = 0;

			rightShoulder = 0;
			rightShoulderRX = 0.0f;
			rightShoulderRZ = 1.0f;
			rightElbow = 0;
		}
		
		glutPostRedisplay(); break;
	}
}

void update(int value) {
	// whole body rotation
	rotateWholeBody += incrementRotateBody;
	if (rotateWholeBody > 360) {
		rotateWholeBody -= 360;
	}

	// head rotation
	if (rotateHead < 70 && rotateHead >= 0) {
		rotateHead += incrementRotateHead;
	}
	else if (rotateHead == 70) {
		rotateHead = -70;
	}
	else if (rotateHead <= 0 && rotateHead >= -70) {
		rotateHead += incrementRotateHead;
	}
	else if (rotateHead == 0) {
		rotateHead = 0;
	}

	// head nod
	if (nodHead < 10 && nodHead >= 0) {
		nodHead += incrementNodHead;
	}
	else if (nodHead == 10) {
		nodHead = -10;
	}
	else if (nodHead <= 0 && nodHead >= -10) {
		nodHead += incrementNodHead;
	}
	else if (nodHead == 0) {
		nodHead = 0;
	}

	// right hand
	if (rightShoulder > -120 && rightShoulder <= 0) {
		rightShoulder -= incrementRightShoulder;
	}
	else if (rightShoulder == -120) {
		rightShoulder = 0;
	}

	// right elbow
	if (rightElbow > -120 && rightElbow <= 0) {
		rightElbow -= incrementRightElbow;
	}
	else if (rightElbow == -120) {
		rightElbow = 0;
	}

	// right palm
	if (rightPalm > -40 && rightPalm <= 0) {
		rightPalm -= incrementRightPalm;
	}
	else if (rightPalm == -40) {
		rightPalm = 20;
	}
	else if (rightPalm >= 0 && rightPalm <= 20) {
		rightPalm -= incrementRightPalm;
	}
	else if (rightPalm == 0) {
		rightPalm = 0;
	}

	// right fingers
	if (rightFingers > -20 && rightFingers <= 0) {
		rightFingers -= incrementRightFingers;
	}
	else if (rightFingers == -20) {
		rightFingers = 30;
	}
	else if (rightFingers >= 0 && rightFingers <= 30) {
		rightFingers -= incrementRightFingers;
	}
	else if (rightFingers == 0) {
		rightFingers = 0;
	}

	// left hand
	if (leftShoulder < 120 && leftShoulder >= 0) {
		leftShoulder += incrementLeftShoulder;
	}
	else if (leftShoulder == 120) {
		leftShoulder = 0;
	}

	// left elbow
	if (leftElbow > -120 && leftElbow <= 0) {
		leftElbow -= incrementLeftElbow;
	}
	else if (leftElbow == -120) {
		leftElbow = 0;
	}

	// left palm
	if (leftPalm < 40 && leftPalm >= 0) {
		leftPalm += incrementLeftPalm;
	}
	else if (leftPalm == 40) {
		leftPalm = -20;
	}
	else if (leftPalm <= 0 && leftPalm >= -20) {
		leftPalm += incrementLeftPalm;
	}
	else if (leftPalm == 0) {
		leftPalm = 0;
	}

	// left fingers
	if (leftFingers < 20 && leftFingers >= 0) {
		leftFingers += incrementLeftFingers;
	}
	else if (leftFingers == 20) {
		leftFingers = -30;
	}
	else if (leftFingers <= 0 && leftFingers >= -30) {
		leftFingers += incrementLeftFingers;
	}
	else if (leftFingers == 0) {
		leftFingers = 0;
	}

	glutPostRedisplay();
	glutTimerFunc(25, update, 0);
}

void pressKey(int key, int xx, int yy)
{
	switch (key)
	{
	case GLUT_KEY_UP: deltaMove = 0.1f; break;
	case GLUT_KEY_DOWN: deltaMove = -0.1f; break;
	}
}

void releaseKey(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN:
		deltaMove = 0;
		break;
	}
}

void mouseMove(int x, int y)
{
	// this will only be true when the left button is down
	if (xOrigin >= 0)
	{
		// update deltaAngle
		deltaAngle = (x - xOrigin) * 0.001f;

		// update camera's direction
		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);
	}
	// this will only be true when the right button is down
	if (yOrigin >= 0)
	{
		// update deltaAngle
		deltaAngle = (y - yOrigin) * 0.001f;

		// update camera's direction
		ly = sin(angle + deltaAngle);
	}
}

void mouseButton(int button, int state, int x, int y)
{
	// triger camera view from left to right or right to left
	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON)
	{
		// when the button is released
		if (state == GLUT_UP)
		{
			angle += deltaAngle;
			xOrigin = -1;
		}
		else // state == GLUT_DOWN
		{
			xOrigin = x;
		}
	}
	// triger camera view from top to bottom or bottom to top
	// only start motion if the right button is pressed
	if (button == GLUT_RIGHT_BUTTON)
	{
		// when the button is released
		if (state == GLUT_UP)
		{
			angle += deltaAngle;
			yOrigin = -1;
		}
		else // state == GLUT_DOWN
		{
			yOrigin = y;
		}
	}
}

int main(int argc, char** argv)
{
	// initialize GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 100);
	glutInitWindowSize(900, 600);
	glutCreateWindow("My Neighbor Totoro");

	initRendering();

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);

	// here are the two new functions
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	glutTimerFunc(25, update, 0); // add a timer

	// enter GLUT event processing cycle
	glutMainLoop();

	return 0;
}