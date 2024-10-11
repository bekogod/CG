#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

float PI = M_PI;
float px= 10.0;
float py= 10.0;
float pz= 10.0;

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


void drawCylinder(float radius, float height, int slices) {

glBegin(GL_TRIANGLES);

		float alpha = (2*M_PI) / slices;
		int x = 0;
		glColor3f(0.4f,0.23f,0.96f);	


		for(int i = 1;x<slices;i++){
			float angulo_atual =    i*alpha;
			float angulo_seguinte = (i+1)*alpha;
			

			//BASE BAIXO
			glColor3f(0.4f,0.23f,0.96f);
			glVertex3f(radius*sin(angulo_atual),0,radius*cos(angulo_atual));
			glVertex3f(0,0,0);
			glVertex3f(radius*sin(angulo_seguinte),0,radius*cos(angulo_seguinte));
			//BASE CIMA
			glColor3f(0.4f,0.23f,0.96f);
			glVertex3f(0,height,0);
			glVertex3f(radius*sin(angulo_atual),height,radius*cos(angulo_atual));
			glVertex3f(radius*sin(angulo_seguinte),height,radius*cos(angulo_seguinte));
			//LIGAÇOES ENTRE BASES TRIANGULO ESQUERDA
			glColor3f(1,0,0);
			glVertex3f(radius*sin(angulo_atual),height,radius*cos(angulo_atual));
			glVertex3f(radius*sin(angulo_atual),0,radius*cos(angulo_atual));
			glVertex3f(radius*sin(angulo_seguinte),0,radius*cos(angulo_seguinte));
			//LIGAÇOES ENTRE BASES TRAINGULO DIREITA
			glColor3f(0,1,0);
			glVertex3f(radius*sin(angulo_atual),height,radius*cos(angulo_atual));
			glVertex3f(radius*sin(angulo_seguinte),0,radius*cos(angulo_seguinte));
			glVertex3f(radius*sin(angulo_seguinte),height,radius*cos(angulo_seguinte));

			x++;
		}
			



	glEnd();


}


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(px,py,pz, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);


	// put axis drawing in here

	glBegin(GL_LINES);
		// X axis in red
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(
		-100.0f, 0.0f, 0.0f);
		glVertex3f( 100.0f, 0.0f, 0.0f);
		// Y Axis in Green
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f,
		-100.0f, 0.0f);
		glVertex3f(0.0f, 100.0f, 0.0f);
		// Z Axis in Blue
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f,
		-100.0f);
		glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();




	//CILINDRO
	drawCylinder(2,4,20);

	// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

	if (c == 'a' || c == 'A') {
		px-=0.5;
		pz+=0.5;
	}

	if (c == 'd' || c == 'D') {
		px+=0.5;
		pz-=0.5;
	}
	if (c == 'w' || c == 'W') {
		py+=0.1;
		py+=0.1;
	}
	if (c == 's' || c == 'S') {
		py-=0.1;
		py-=0.1;
	}


		glutPostRedisplay();
}



void processSpecialKeys(int key, int xx, int yy) {

// put code to process special keys in here

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
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
