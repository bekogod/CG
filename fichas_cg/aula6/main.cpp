

#include<stdio.h>
#include<stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#include <IL/il.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif


float camX = 00, camY = 30, camZ = 40;
int startX, startY, tracking = 0;

int alpha = 0, beta = 45, r = 50;

unsigned int t, tw, th;
unsigned char *imageData;

GLuint buffers;

float h (int i, int j){
	return imageData[i*tw+j];
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective
	gluPerspective(45,ratio,1,1000);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}



void drawTerrain() {

    // colocar aqui o código de desenho do terreno usando VBOs com TRIANGLE_STRIPS
	glPolygonMode(GL_FRONT, GL_LINE);
	glBindBuffer(GL_ARRAY_BUFFER, buffers);
	glVertexPointer(3, GL_FLOAT, 0, 0);
		for (int i = 0; i < th - 1 ; i++) {
	glDrawArrays(GL_TRIANGLE_STRIP, (tw) * 2 * i, (tw) * 2);
	}

}



void renderScene(void) {

	float pos[4] = {-1.0, 1.0, 1.0, 0.0};

	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(camX, camY, camZ, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);
	
	
	glColor3f(1,1,1);
	drawTerrain();

	// just so that it renders something before the terrain is built
	// to erase when the terrain is ready
	

// End of frame
	glutSwapBuffers();
}



void processKeys(unsigned char key, int xx, int yy) {

// put code to process regular keys in here
}



void processMouseButtons(int button, int state, int xx, int yy) {
	
	if (state == GLUT_DOWN)  {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
		else
			tracking = 0;
	}
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha += (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			
			r -= yy - startY;
			if (r < 3)
				r = 3.0;
		}
		tracking = 0;
	}
}


void processMouseMotion(int xx, int yy) {

	int deltaX, deltaY;
	int alphaAux, betaAux;
	int rAux;

	if (!tracking)
		return;

	deltaX = xx - startX;
	deltaY = yy - startY;

	if (tracking == 1) {


		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0)
			betaAux = 85.0;
		else if (betaAux < -85.0)
			betaAux = -85.0;

		rAux = r;
	}
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r - deltaY;
		if (rAux < 3)
			rAux = 3;
	}
	camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camY = rAux * 							     sin(betaAux * 3.14 / 180.0);
}


void init() {

// 	Load the height map "terreno.jpg"
	ilInit();

	ilGenImages(1,&t);
	ilBindImage(t);

	ilLoadImage((ILstring)"terreno.jpg");
	ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);
	
	tw = ilGetInteger(IL_IMAGE_WIDTH);
	th = ilGetInteger(IL_IMAGE_HEIGHT);
	printf("tw = %d\n", tw);
	printf("th = %d\n", th);

	imageData = ilGetData();

// 	Build the vertex arrays

	std::vector<float> p;

	printf("antes de criar grid tw/2 : %d\n",tw/2);
	printf("antes de criar grid h : %f\n",h(0,0));
	for (int j =-127; j<=128; j++){
		for (int i = -127; i<=128; i++){

			int y11=i+127;
			int y21 = j+127;
			int y22 = j+128;

			p.push_back(i);
			p.push_back(h(y11,y21)/8.5);
			p.push_back(j);

			p.push_back(i);
			p.push_back(h(y11,y22)/8.5);
			p.push_back(j+1);
		}

	}

	int arraySize = p.size() * sizeof(float);


	// criar o VBO
	glGenBuffers(1, &buffers);

	// copiar o vector para a memória gráfica
	glBindBuffer(GL_ARRAY_BUFFER,buffers);
	glBufferData(GL_ARRAY_BUFFER,arraySize, p.data(), GL_STATIC_DRAW);


// 	OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("CG@DI-UM");
	

	glewInit();
	glEnableClientState(GL_VERTEX_ARRAY);
	init();	
		

// Required callback registry 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);



// enter GLUT's main cycle
	glutMainLoop();
	
	return 0;
}
