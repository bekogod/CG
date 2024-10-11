#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>

GLfloat angle = 0;
GLfloat tx = 0;
GLfloat ty = 0;
GLfloat tz = 0;
GLfloat sx = 0.5;
GLfloat sy = 0.5;
GLfloat sz = 0.5;


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
	gluLookAt(5.0,5.0,5.0, 
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


// put the geometric transformations here
	glScalef(sx,sy,sz);
	glRotatef(angle,0,1,0);
	glTranslatef(tx,ty,tz);

	

// put pyramid drawing instructions here
	glBegin(GL_TRIANGLES);

		//BASE
		glVertex3f(0.0f,0.0f,1.0f);
		glVertex3f(0.0f,0.0f,-1.0f);
		glVertex3f(-1.0f,0.0f,0.0f);

		glVertex3f(1.0f,0.0f,0.0f);
		glVertex3f(0.0f,0.0f,-1.0f);
		glVertex3f(0.0f,0.0f,1.0f);

		

		//paredes
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-1.0f,0.0f,0.0f);
		glVertex3f(0.0f,0.0f,1.0f);
		glVertex3f(0.0f,2.0f,0.0f);


		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f,2.0f,0.0f);
		glVertex3f(0.0f,0.0f,1.0f);
		glVertex3f(1.0f,0.0f,0.0f);

		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f,2.0f,0.0f);
		glVertex3f(1.0f,0.0f,0.0f);
		glVertex3f(0.0f,0.0f,-1.0f);

		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(0.0f,2.0f,0.0f);
		glVertex3f(0.0f,0.0f,-1.0f);
		glVertex3f(-1.0f,0.0f,0.0f);



	glEnd();

	// End of frame
	glutSwapBuffers();
}



// write function to process keyboard events
void processar_teclado (unsigned char key, int x, int y){

	//ROTAÇÃO
	if(key == 'q' || key == 'Q'){
		angle+=2;
	}
	if(key == 'e' || key == 'E'){
		angle-=2;
	}




	//TRANSLAÇÃO
	if(key == 'w' || key == 'W'){
		tx=tx-0.5f;
		tz=tz-0.5f;
	}

	if(key == 'd' || key == 'D'){
		tx+=0.5f;
		tz-=0.5f;
	}

	if(key == 's' || key == 'S'){
		tx=tx+0.5f;
		tz=tz+0.5f;
	}

	if(key == 'a' || key == 'A'){
		tx=tx-0.5f;
		tz=tz+0.5f;
	}


	//SCALING
	if(key == 'z' || key == 'Z'){
		sx=sx-0.5f;
		sy=sy-0.5f;
		sz=sz-0.5f;
	}

	if(key == 'x' || key == 'X'){
		sx=sx+0.5f;
		sy=sy+0.5f;
		sz=sz+0.5f;
	}



	glutPostRedisplay();


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

	
// put here the registration of the keyboard callbacks
	glutKeyboardFunc(processar_teclado);


//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
