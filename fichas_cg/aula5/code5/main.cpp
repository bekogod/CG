#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

float alfa = 0.0f, beta = 0.5f, radius = 100.0f;
float camX, camY, camZ;
float alfaazul = 360.0f/8.0f;
float alfavermelho = 360.0f/16.0f;
float cores_arvore[10][3] = {
        {0.2, 0.5, 0.1},
        {0.4, 0.7, 0.2},
        {0.3, 0.4, 0.1},
        {0.6, 0.4, 0.2},
        {0.5, 0.3, 0.1},
        {0.3, 0.2, 0.1},
        {0.1, 0.6, 0.3},
        {0.7, 0.5, 0.2},
        {0.2, 0.3, 0.1},
        {0.8, 0.6, 0.4}
    };

float teapotRotate = 0.0f;


void spherical2Cartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}




void drawTree(GLfloat x,GLfloat z,int height){
	glPushMatrix();
	glTranslatef(x,0,z); //DECIDIR POSICAO DA ARVORE

	glPushMatrix();
	glRotatef(-90,1,0,0);
	glColor3f(0.4,0.2,0.02); //CASTANHO
	glutSolidCone(0.5,height,10,10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0,height/2,0);
	glRotatef(-90,1,0,0);
	int qual_cor = rand() % 10;
	glColor3f(cores_arvore[qual_cor][0],cores_arvore[qual_cor][1],cores_arvore[qual_cor][2]); //COR DA FOLHAS
	glutSolidCone(1,height*1.5,10,10);
	glPopMatrix();


	glPopMatrix();
	
	}


void drawPinkTorus(){
	glColor3f(1,0.5,1);
	glutSolidTorus(1.0f, 3.0f, 10, 10);
}



void drawBluePots(){
	glColor3f(0,0,1);
	glPushMatrix();
	glRotatef(-teapotRotate,0,1,0);

	for (int i = 0; i < 8; i++)
	{

		glRotatef(alfaazul, 0, 1, 0); // ang in degrees
		glPushMatrix();
		glTranslatef(15, 2, 0.0f);
		glutSolidTeapot(2);
		glPopMatrix();

	}

	glPopMatrix();
	
	
}

void drawRedPots(){
	glColor3f(1,0,0);
	glPushMatrix();
	glRotatef(teapotRotate,0,1,0);
	for(int i = 1; i<=16;i++){
		glRotatef(alfavermelho,0,1,0);
		glPushMatrix();
		glTranslatef(35,1.5,0);
		glutSolidTeapot(1.5);
		glPopMatrix();

	}

	glPopMatrix();
	
	
}


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}



void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	glColor3f(0.2f, 0.8f, 0.2f);
	glBegin(GL_TRIANGLES);
		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);

		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);
		glVertex3f(100.0f, 0, 100.0f);
	glEnd();
	
	// put code to draw scene in here
	srand(1);
	for(int i = 0; i<750; i++){
		GLfloat x= rand() % 200 -100;
		GLfloat z= rand() % 200 -100;
		if(pow(x,2) + pow(z,2) >= pow(50,2)){
			int height = rand() % 5 + 3;
			drawTree(x,z,height);
		}
		else i--;
	}

	
	drawPinkTorus();
	drawBluePots();
	drawRedPots();
	
	teapotRotate+=0.2f;
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here

}


void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_RIGHT:
		alfa -= 0.1; break;

	case GLUT_KEY_LEFT:
		alfa += 0.1; break;

	case GLUT_KEY_UP:
		beta += 0.1f;
		if (beta > 1.5f)
			beta = 1.5f;
		break;

	case GLUT_KEY_DOWN:
		beta -= 0.1f;
		if (beta < -1.5f)
			beta = -1.5f;
		break;

	case GLUT_KEY_PAGE_DOWN: radius -= 2.0f;
		if (radius < 1.0f)
			radius = 1.0f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 2.0f; break;
	}
	spherical2Cartesian();
	glutPostRedisplay();

}


void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n");
	printf("Home and End control the distance from the camera to the origin");
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	spherical2Cartesian();

	printInfo();

// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
