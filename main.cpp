// CS3241Lab3.cpp : Defines the entry point for the console application.
//#include <cmath>
#include "math.h"
#include <iostream>
#include <fstream>

#ifdef _WIN32
	#include <Windows.h>
	#include "glut.h"
	#define M_PI 3.141592654
#elif __APPLE__
	#include <OpenGL/gl.h>
	#include <GLUT/GLUT.h>
#endif

// global variable

bool m_Smooth = false;
bool m_Highlight = false;
GLfloat angle = 0;   /* in degrees */
GLfloat angle2 = 0;   /* in degrees */
GLfloat zoom = 1.0;
int mouseButton = 0;
int moving, startx, starty;

#define NO_OBJECT 4;
int current_object = 0;

using namespace std;

void setupLighting()
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);

	// Lights, material properties
    GLfloat	ambientProperties[]  = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat	diffuseProperties[]  = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat	specularProperties[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat lightPosition[] = {-100.0f,100.0f,100.0f,1.0f};
	
    glClearDepth( 1.0 );

    glLightfv( GL_LIGHT0, GL_AMBIENT, ambientProperties);
    glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseProperties);
	glLightfv( GL_LIGHT0, GL_SPECULAR, specularProperties);
	glLightfv( GL_LIGHT0, GL_POSITION, lightPosition);
	
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);

	// Default : lighting
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

}


void drawSphere(double r)
{

	float no_mat[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float mat_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    float mat_diffuse[] = {0.1f, 0.5f, 0.8f, 1.0f};
	float mat_emission[] = {0.3f, 0.2f, 0.2f, 0.0f};
	float no_shininess = 0.0f;
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	if(m_Highlight)
	{
		// your codes for highlight here
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_emission);
		glMaterialf(GL_FRONT, GL_SHININESS, 200);
	} else {
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
	}

	
    int i,j;
	int n = 20;
    for(i=0;i<n;i++)
		for(j=0;j<2*n;j++)
		if(m_Smooth)
		{
			glBegin(GL_POLYGON);

			    // the normal of each vertex is actaully its own coordinates normalized for a sphere

			    // your normal here
				glNormal3d(sin(i*M_PI / n)*cos(j*M_PI / n), cos(i*M_PI / n)*cos(j*M_PI / n), sin(j*M_PI / n));

				glVertex3d(r*sin(i*M_PI/n)*cos(j*M_PI/n),r*cos(i*M_PI/n)*cos(j*M_PI/n),r*sin(j*M_PI/n));
			    // your normal here
				glNormal3d(sin((i + 1)*M_PI / n)*cos(j*M_PI / n),cos((i + 1)*M_PI / n)*cos(j*M_PI / n),sin(j*M_PI / n));

				glVertex3d(r*sin((i+1)*M_PI/n)*cos(j*M_PI/n),r*cos((i+1)*M_PI/n)*cos(j*M_PI/n),r*sin(j*M_PI/n));
			    // your normal here
				glNormal3d(sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n),
					cos((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n),
					sin((j + 1)*M_PI / n));

				glVertex3d(r*sin((i+1)*M_PI/n)*cos((j+1)*M_PI/n),r*cos((i+1)*M_PI/n)*cos((j+1)*M_PI/n),r*sin((j+1)*M_PI/n));
			    // your normal here
				glNormal3d(sin(i*M_PI / n)*cos((j + 1)*M_PI / n),
					cos(i*M_PI / n)*cos((j + 1)*M_PI / n),
					sin((j + 1)*M_PI / n));

				glVertex3d(r*sin(i*M_PI/n)*cos((j+1)*M_PI/n),r*cos(i*M_PI/n)*cos((j+1)*M_PI/n),r*sin((j+1)*M_PI/n));
 			glEnd();
		} else	{
			glBegin(GL_POLYGON);
			    // Explanation: the normal of the whole polygon is the coordinate of the center of the polygon for a sphere
			    glNormal3d(sin((i+0.5)*M_PI/n)*cos((j+0.5)*M_PI/n),cos((i+0.5)*M_PI/n)*cos((j+0.5)*M_PI/n),sin((j+0.5)*M_PI/n));
				glVertex3d(r*sin(i*M_PI/n)*cos(j*M_PI/n),r*cos(i*M_PI/n)*cos(j*M_PI/n),r*sin(j*M_PI/n));
				glVertex3d(r*sin((i+1)*M_PI/n)*cos(j*M_PI/n),r*cos((i+1)*M_PI/n)*cos(j*M_PI/n),r*sin(j*M_PI/n));
				glVertex3d(r*sin((i+1)*M_PI/n)*cos((j+1)*M_PI/n),r*cos((i+1)*M_PI/n)*cos((j+1)*M_PI/n),r*sin((j+1)*M_PI/n));
				glVertex3d(r*sin(i*M_PI/n)*cos((j+1)*M_PI/n),r*cos(i*M_PI/n)*cos((j+1)*M_PI/n),r*sin((j+1)*M_PI/n));
 			glEnd();
		}

}

void setTexture(float no_mat[4], float mat_ambient[4], float mat_diffuse[4], float mat_emission[4], float mat_specular[4], float no_shininess) {

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	if (m_Highlight)
	{
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
	}
	else {
		glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
		glMaterialf(GL_FRONT, GL_SHININESS, no_shininess);
	}
}


void setColours(float amb1, float amb2, float amb3, float amb4, float diff1, float diff2, float diff3, float diff4, float emm1, float emm2,
	float emm3, float emm4, float spec1, float spec2, float spec3, float spec4, float no_shininess, bool isTransperant) {

	if (isTransperant == true) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	float no_mat[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float mat_ambient[] = { amb1, amb2, amb3, amb4 };
	float mat_diffuse[] = { diff1, diff2, diff3, diff4 };
	float mat_emission[] = { emm1, emm2, emm3, emm4 };
	float mat_specular[] = { spec1, spec2, spec3, spec4 };
	setTexture(no_mat, mat_ambient, mat_diffuse, mat_emission, mat_specular, no_shininess);

}

void drawBaseCircle(float baseDist, float radius) {	

	if (m_Smooth) {
		glPushMatrix();
		glTranslatef(0, 0, baseDist);
		
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++) {
			float rad = i*M_PI / 180;
			glNormal3f(cos(rad), sin(rad), 0);
			glVertex3f(cos(rad) * radius, sin(rad) * radius, 0);
		}
		glEnd();
		glPopMatrix();
	}
	else {
		glPushMatrix();
		glTranslatef(0, 0, baseDist);
		glColor3f(0, 0, 1);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++) {
			float rad = i*M_PI / 180;
			glVertex2f(cos(rad) * radius, sin(rad) * radius);
		}
		glEnd();
		glPopMatrix();
	}

}

void drawCylinder(float length, float out_radius, float in_radius)
{
	float hl = length * 0.5f;
	float a = 0.0f;
	int numSteps = 300;
	float step = M_PI * 2 / (float)numSteps;
	float x, y;
	
	if (m_Smooth) {

		glBegin(GL_TRIANGLE_STRIP);
		for (int i = 0; i < numSteps; ++i)
		{
			x = cos(a) * out_radius;
			y = sin(a) * out_radius;
			glNormal3d(x, y, -hl);
			glVertex3f(x, y, -hl);
			glNormal3d(x, y, hl);
			glVertex3f(x, y, hl);

			a += step;
		}
		glNormal3d(x / out_radius, y / out_radius, -hl);
		glVertex3f(x, y, -hl);
		glNormal3d(cos(0.0f), sin(0.0f), -hl);
		glVertex3f(cos(0.0f)*out_radius, sin(0.0f)*out_radius, -hl);

		glNormal3d(x / out_radius, y / out_radius, hl);
		glVertex3f(x, y, hl);
		glNormal3d(cos(0.0f), sin(0.0f), hl);
		glVertex3f(cos(0.0f)*out_radius, sin(0.0f)*out_radius, hl);

		glEnd();

		glBegin(GL_TRIANGLE_STRIP);
		
		hl = hl - 0.05;
		for (int i = 0; i < numSteps; ++i)
		{
			
			x = cos(a) * in_radius;
			y = sin(a) * in_radius;
			glNormal3d(-x, -y, hl);
			glVertex3f(x, y, -hl);
			glNormal3d(-x, -y, -hl);
			glVertex3f(x, y, hl);

			a += step;
		}
		glNormal3d(-x / in_radius, -y / in_radius, hl);
		glVertex3f(x, y, -hl);
		glNormal3d(-cos(0.0f), -sin(0.0f), hl);
		glVertex3f(cos(0.0f)*in_radius, sin(0.0f)*in_radius, -hl);

		glNormal3d(-x / in_radius, -y / in_radius, -hl);
		glVertex3f(x, y, hl);
		glNormal3d(-cos(0.0f), -sin(0.0f), -hl);
		glVertex3f(cos(0.0f)*in_radius, sin(0.0f)*in_radius, hl);

		glEnd();
	}
	else {

		glBegin(GL_TRIANGLE_STRIP);
		for (int i = 0; i < numSteps; ++i)
		{
			x = cos(a) * out_radius;
			y = sin(a) * out_radius;
			glVertex3f(x, y, -hl);
			glVertex3f(x, y, hl);

			a += step;
		}
		glVertex3f(x, y, -hl);
		glVertex3f(cos(0.0f)*out_radius, sin(0.0f)*out_radius, -hl);

		glVertex3f(x, y, hl);
		glVertex3f(cos(0.0f)*out_radius, sin(0.0f)*out_radius, hl);

		glEnd();

		glBegin(GL_TRIANGLE_STRIP);
		hl = hl - 0.05;
		for (int i = 0; i < numSteps; ++i)
		{

			x = cos(a) * in_radius;
			y = sin(a) * in_radius;
			glVertex3f(x, y, -hl);
			glVertex3f(x, y, hl);

			a += step;
		}
		
		glVertex3f(x, y, -hl);
		glVertex3f(cos(0.0f)*in_radius, sin(0.0f)*in_radius, -hl);
		glVertex3f(x, y, hl);
		glVertex3f(cos(0.0f)*in_radius, sin(0.0f)*in_radius, hl);

		glEnd();
	}
	

}

void drawGlassBase(float length, float radius) {
	setColours(0.3, 0.3, 0.3, 1.0, 0.588235, 0.670588, 0.729412, 1.0, 0.9, 0.9, 0.9, 1.0f, 1, 1, 1, 0, 96, true);
	drawBaseCircle(length , radius);
	//0, 0, 1, 0.6,
}

void drawGlass(float length, float radius) {
	setColours(0.3, 0.3, 0.3, 1.0, 0.588235, 0.670588, 0.729412, 1.0, 0.9, 0.9, 0.9, 1.0f, 1, 1, 1, 0, 96, true);
	drawCylinder(length, radius, radius-0.05);
	drawGlassBase(length/2.0, radius);
	// 0.0f, 0.49f, 1.0f, 0.4f, - diffues
	// 0.19f, 0.19f, 0.8f, 0.4f, - ambient 
}


void tinyGlass(float y_delta, float z_delta) {
	glPushMatrix();
	glTranslatef(0, y_delta, z_delta);
	glScalef(0.4, 0.4, 0.4);
	drawGlass(1.5, 0.5);
	glPopMatrix();
}

void drawJughead(double r) {
	int i, j;
	int n = 20;
	for (i = 0; i<n; i++)
		for (j = 0; j<2 * n; j++)
			if (m_Smooth)
			{
				glBegin(GL_POLYGON);
				glNormal3d(sin(i*M_PI / n)*cos(j*M_PI / n), cos(i*M_PI / n)*cos(j*M_PI / n), sin(j*M_PI / n));

				glVertex3d(r*sin(i*M_PI / n)*cos(j*M_PI / n), r*cos(i*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));
				glNormal3d(sin((i + 1)*M_PI / n)*cos(j*M_PI / n), cos((i + 1)*M_PI / n)*cos(j*M_PI / n), sin(j*M_PI / n));
				glVertex3d(r*sin((i + 1)*M_PI / n)*cos(j*M_PI / n), r*cos((i + 1)*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));
				glNormal3d(sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n),	cos((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n),sin((j + 1)*M_PI / n));
				glVertex3d(r*sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*cos((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));
				glNormal3d(sin(i*M_PI / n)*cos((j + 1)*M_PI / n),cos(i*M_PI / n)*cos((j + 1)*M_PI / n),	sin((j + 1)*M_PI / n));
				glVertex3d(r*sin(i*M_PI / n)*cos((j + 1)*M_PI / n), r*cos(i*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));
				glEnd();
			}
			else	{
				glBegin(GL_POLYGON);
				// Explanation: the normal of the whole polygon is the coordinate of the center of the polygon for a sphere
				glNormal3d(sin((i + 0.5)*M_PI / n)*cos((j + 0.5)*M_PI / n), cos((i + 0.5)*M_PI / n)*cos((j + 0.5)*M_PI / n), sin((j + 0.5)*M_PI / n));
				glVertex3d(r*sin(i*M_PI / n)*cos(j*M_PI / n), r*cos(i*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));
				glVertex3d(r*sin((i + 1)*M_PI / n)*cos(j*M_PI / n), r*cos((i + 1)*M_PI / n)*cos(j*M_PI / n), r*sin(j*M_PI / n));
				glVertex3d(r*sin((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*cos((i + 1)*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));
				glVertex3d(r*sin(i*M_PI / n)*cos((j + 1)*M_PI / n), r*cos(i*M_PI / n)*cos((j + 1)*M_PI / n), r*sin((j + 1)*M_PI / n));
				glEnd();
			}

}

void drawJug() {
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glTranslatef(-1.5, -1.5, -0.4);
	glScalef(0.15, 0.15, 0.15);
	setColours(0.3, 0.3, 0.3, 1.0, 0.588235, 0.670588, 0.729412, 1.0, 0.9, 0.9, 0.9, 1.0f, 1, 1, 1, 0, 96, true);
	drawCylinder(2, 1.0, 0.9);
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glTranslatef(-1.5, -1.5, -0.05);
	glScalef(0.15, 0.15, 0.15);	
	drawJughead(2.0);
	glPopMatrix();

	/*glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glTranslatef(-1.5, 0, -0.4);
	glScalef(0.15, 0.15, 0.15);
	setColours(0.3, 0.3, 0.3, 1.0, 0.588235, 0.670588, 0.729412, 1.0, 0.9, 0.9, 0.9, 1.0f, 1, 1, 1, 0, 96, true);
	drawCylinder(3, 0.8, 0.7);
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glTranslatef(-1.5, 0, -0.05);
	glScalef(0.15, 0.15, 0.15);
	drawJughead(2.0);
	glPushMatrix();*/
}


void drawTrayBase() {
	setColours(1.0f, 0.0f, 0.00f, 0.4f, 0.73f, 0.56f, 0.56f, 0.4f, 0.3, 0.2, 0.2, 0, 1, 1, 1, 0, 500, true);
	drawBaseCircle(0.1,2);
}

void drawTray() {
	glPushMatrix();
	glTranslatef(-0.1, 0, 0.25);
	glScalef(0.7, 0.7, 0.7);
	setColours(0.55f, 0.09f, 0.09f, 0.6f, 0.73f, 0.56f, 0.56f, 0.6f, 0.3, 0.2, 0.2, 0, 1, 1, 1, 0, 500, false);
	drawCylinder(0.2, 2, 1.95);
	drawTrayBase();
	glPopMatrix();
}


// Part 3: Composite Object 1 - Stack of glasses
void drawGlassWare() {
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glScalef(0.8, 0.8, 0.8);
	glTranslatef(-1, -1.5, 0);
	float y = 1.0;
	float z = 0.0;
	float y_temp = 0.0;
	for (int i = 5; i >= 1; i--) {
		y_temp = y;
		for (int j = 0; j <= i - 1; j++) {
			tinyGlass(y_temp, z);
			y_temp -= 0.5;
		}
		y -= 0.25;
		z -= 0.6;
	}
	drawTray();
	glPopMatrix();
	drawJug();

}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
		glTranslatef(0, 0, -6);

	    glRotatef(angle2, 1.0, 0.0, 0.0);
		glRotatef(angle, 0.0, 1.0, 0.0);

		glScalef(zoom,zoom,zoom);

		switch (current_object) {
		case 0:
			drawSphere(1);
			break;
		case 1:
			// draw your second primitive object here
			drawGlass(1, 0.5);
			break;
		case 2:
			// draw your first composite object here
			drawGlassWare();
			break;
		case 3:
			// draw your second composite object here			
			break;
		default:
			break;
		};
	glPopMatrix();
	glutSwapBuffers ();
}




void keyboard (unsigned char key, int x, int y)
{
	switch (key) {
	case 'p':
	case 'P':
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		break;			
	case 'w':
	case 'W':
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		break;			
	case 'v':
	case 'V':
		glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
		break;			
	case 's':
	case 'S':
		m_Smooth = !m_Smooth;
		break;
	case 'h':
	case 'H':
		m_Highlight = !m_Highlight;
		break;

	case '1':
	case '2':
	case '3':
	case '4':
		current_object = key - '1';
		break;

	case 27:
		exit(0);
		break;
		
	default:
	break;
	}

	glutPostRedisplay();
}



void
mouse(int button, int state, int x, int y)
{
  if (state == GLUT_DOWN) {
	mouseButton = button;
    moving = 1;
    startx = x;
    starty = y;
  }
  if (state == GLUT_UP) {
	mouseButton = button;
    moving = 0;
  }
}

void motion(int x, int y)
{
  if (moving) {
	if(mouseButton==GLUT_LEFT_BUTTON)
	{
		angle = angle + (x - startx);
		angle2 = angle2 + (y - starty);
	}
	else zoom += ((y-starty)*0.001);
    startx = x;
    starty = y;
	glutPostRedisplay();
  }
  
}

int main(int argc, char **argv)
{
	cout<<"CS3241 Lab 3"<< endl<< endl;

	cout << "1-4: Draw different objects"<<endl;
	cout << "S: Toggle Smooth Shading"<<endl;
	cout << "H: Toggle Highlight"<<endl;
	cout << "W: Draw Wireframe"<<endl;
	cout << "P: Draw Polygon"<<endl;
	cout << "V: Draw Vertices"<<endl;
	cout << "ESC: Quit" <<endl<< endl;

	cout << "Left mouse click and drag: rotate the object"<<endl;
	cout << "Right mouse click and drag: zooming"<<endl;

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (600, 600);
	glutInitWindowPosition (50, 50);
	glutCreateWindow ("CS3241 Assignment 3");
	glClearColor (1.0,1.0,1.0, 1.0);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	setupLighting();
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST); 
	glDepthMask(GL_TRUE);

    glMatrixMode(GL_PROJECTION);
    gluPerspective( /* field of view in degree */ 40.0,
  /* aspect ratio */ 1.0,
    /* Z near */ 1.0, /* Z far */ 80.0);
	glMatrixMode(GL_MODELVIEW);
	glutMainLoop();

	return 0;
}
