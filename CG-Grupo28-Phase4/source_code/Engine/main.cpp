#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "pugixml.hpp"

#include <IL/il.h>
#include <GL/glew.h>
#include <GL/glut.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>

#define MAX_MODELS 100

int texturesIds = 1;

std::string CONFIGS_DIR = "C:\\Users\\super\\OneDrive\\Ambiente de Trabalho\\cg\\test_files";

float camX, camY, camZ;
float lookAtX, lookAtY, lookAtZ;
float upX, upY, upZ;
int WINDOW_WIDTH, WINDOW_HEIGHT;

int num_models = 0;

double fov, nearPlane, farPlane;

float frames;

float mode = GL_FILL;
int timebase = 0;
int frame = 0;

float alfa = 0.0f, beta = 0.0f, radius = 3.0f;

void sphericalToCartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}



void buildRotMatrix(float *x, float *y, float *z, float *m)
{

	m[0] = x[0];
	m[1] = x[1];
	m[2] = x[2];
	m[3] = 0;
	m[4] = y[0];
	m[5] = y[1];
	m[6] = y[2];
	m[7] = 0;
	m[8] = z[0];
	m[9] = z[1];
	m[10] = z[2];
	m[11] = 0;
	m[12] = 0;
	m[13] = 0;
	m[14] = 0;
	m[15] = 1;
}

void cross(float *a, float *b, float *res)
{

	res[0] = a[1] * b[2] - a[2] * b[1];
	res[1] = a[2] * b[0] - a[0] * b[2];
	res[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(float *a)
{

	float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	a[0] = a[0] / l;
	a[1] = a[1] / l;
	a[2] = a[2] / l;
}

float length(float *v)
{

	float res = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	return res;
}

void multMatrixVector(float *m, float *v, float *res)
{

	for (int j = 0; j < 4; ++j)
	{
		res[j] = 0;
		for (int k = 0; k < 4; ++k)
		{
			res[j] += v[k] * m[j * 4 + k];
		}
	}
}

float multVectors(float *m, float *v)
{
	float res = 0;
	for (int i = 0; i < 4; ++i)
	{
		res = res + m[i] * v[i];
	}
	return res;
}

enum LightType {
	DIRECTIONAL,
	POINT,
	SPOT
};

class Light
{
private:
	int id;
	LightType type;
	float dirx, diry, dirz; // For directional and spot lights
	float posx, posy, posz; // For point and spot lights
	float cutoff; // For spot lights

public:
	Light(LightType type, int id = 0, float dirx = 0.0f, float diry = 0.0f, float dirz = 0.0f,
		float posx = 0.0f, float posy = 0.0f, float posz = 0.0f, float cutoff = 0.0f)
		: type(type), id(id), dirx(dirx), diry(diry), dirz(dirz),
		posx(posx), posy(posy), posz(posz), cutoff(cutoff){}


	float getId() const { return id; }
	LightType getType() const { return type; }
	float getDirX() const { return dirx; }
	float getDirY() const { return diry; }
	float getDirZ() const { return dirz; }
	float getPosX() const { return posx; }
	float getPosY() const { return posy; }
	float getPosZ() const { return posz; }
	float getCutoff() const { return cutoff; }

	void apply() {
		GLfloat position[] = { posx, posy, posz, 1.0f };
		GLfloat direction[] = { dirx, diry, dirz,0.0f };
		/*
		glPushMatrix();
		glTranslatef(posx, posy, posz);
		glutSolidSphere(0.3, 20, 20); // Draw a sphere with radius 0.1
		glPopMatrix();
		*/
		switch (type) {
		case DIRECTIONAL:
			glLightfv(GL_LIGHT0 + id, GL_POSITION, direction);
			break;

		case POINT:
			glLightfv(GL_LIGHT0 + id, GL_POSITION, position);
			break;

		case SPOT:
			glLightfv(GL_LIGHT0 + id, GL_POSITION, position);
			glLightfv(GL_LIGHT0 + id, GL_SPOT_DIRECTION, direction);
			glLightf(GL_LIGHT0 + id, GL_SPOT_CUTOFF, cutoff);
			break;
		}
	}
	std::string toString() const {
		std::stringstream ss;
		ss << "Light[id=" << id << ", type=";
		switch (type) {
		case DIRECTIONAL:
			ss << "DIRECTIONAL";
			break;
		case POINT:
			ss << "POINT";
			break;
		case SPOT:
			ss << "SPOT";
			break;
		}
		ss << ", dir=(" << dirx << ", " << diry << ", " << dirz << ")";
		ss << ", pos=(" << posx << ", " << posy << ", " << posz << ")";
		ss << ", cutoff=" << cutoff << "]";
		return ss.str();
	}
};

std::vector<Light> vectorLight;


LightType stringToLightType(const std::string& typeStr) {
	if (typeStr == "directional") {
		return DIRECTIONAL;
	}
	else if (typeStr == "point") {
		return POINT;
	}
	else if (typeStr == "spot") {
		return SPOT;
	}
	else {
		throw std::runtime_error("Unknown light type: " + typeStr);
	}
}


class Point
{
private:
	double x, y, z;

public:
	Point(double _x = 0.0, double _y = 0.0, double _z = 0.0) : x(_x), y(_y), z(_z) {}

	double getX() const { return x; }
	double getY() const { return y; }
	double getZ() const { return z; }

	void setX(double _x) { x = _x; }
	void setY(double _y) { y = _y; }
	void setZ(double _z) { z = _z; }
};

class Transformation
{
private:
	std::string type;
	double x, y, z;
	float angle;
	double time;
	bool align;
	std::vector<Point> points;

public:
	Transformation()
		: type(""), x(0.0), y(0.0), z(0.0), angle(0.0f), time(0.0), align(false) {}

	Transformation(std::string type, double x, double y, double z, float angle = 0.0f, double time = 0.0, bool align = false)
		: type(type), x(x), y(y), z(z), angle(angle), time(time), align(align) {}

	// Getters
	std::string getType() const { return type; }
	double getX() const { return x; }
	double getY() const { return y; }
	double getZ() const { return z; }
	float getAngle() const { return angle; }
	double getTime() const { return time; } // Getter for time
	bool getAlign() const { return align; } // Getter for align

	// Setters
	void setType(const std::string &newType) { type = newType; }
	void setX(double newX) { x = newX; }
	void setY(double newY) { y = newY; }
	void setZ(double newZ) { z = newZ; }
	void setAngle(float newAngle) { angle = newAngle; }
	void setTime(double newTime) { time = newTime; }   // Setter for time
	void setAlign(bool newAlign) { align = newAlign; } // Setter for align

	std::string toString() const {
		// Implement a meaningful string representation of the Transformation
		return "Transformation()"; // Placeholder
	}

	void addPoint(const Point &point)
	{
		points.push_back(point);
	}

	void getCatmullRomPoint(float t, Point p0, Point p1, Point p2, Point p3, Point *pos, Point *deriv)
	{

		// catmull-rom matrix
		float m[4][4] = {{-0.5f, 1.5f, -1.5f, 0.5f},
						 {1.0f, -2.5f, 2.0f, -0.5f},
						 {-0.5f, 0.0f, 0.5f, 0.0f},
						 {0.0f, 1.0f, 0.0f, 0.0f}};

		// Compute A = M * P
		float vecX[4] = {p0.getX(), p1.getX(), p2.getX(), p3.getX()}; // x
		float vecY[4] = {p0.getY(), p1.getY(), p2.getY(), p3.getY()}; // y
		float vecZ[4] = {p0.getZ(), p1.getZ(), p2.getZ(), p3.getZ()}; // z

		float A[3][4];
		multMatrixVector(*m, vecX, A[0]);
		multMatrixVector(*m, vecY, A[1]);
		multMatrixVector(*m, vecZ, A[2]);

		// Compute pos = T * A

		float tv[4] = {t * t * t, t * t, t, 1};

		(*pos).setX(multVectors(A[0], tv));
		(*pos).setY(multVectors(A[1], tv));
		(*pos).setZ(multVectors(A[2], tv));

		//		std:: cout <<"valores de p:"<<pos.getX()<<", "<< pos.getY()<<", " <<pos.getZ() <<std::endl;

		// compute deriv = T' * A

		float tvd[4] = {3 * t * t, 2 * t, 1, 0};
		Point derivV;
		(*deriv).setX(multVectors(A[0], tvd));
		(*deriv).setY(multVectors(A[1], tvd));
		(*deriv).setZ(multVectors(A[2], tvd));

		// ...
	}

	// given  global t, returns the point in the curve
	void getGlobalCatmullRomPoint(float gt, Point *pos, Point *deriv)
	{
		int point_count = points.size();
		float t = gt * point_count; // this is the real global t
		int index = floor(t);		// which segment
		t = t - index;				// where within  the segment

		// indices store the points
		int indices[4];
		indices[0] = (index + point_count - 1) % point_count;
		indices[1] = (indices[0] + 1) % point_count;
		indices[2] = (indices[1] + 1) % point_count;
		indices[3] = (indices[2] + 1) % point_count;

		getCatmullRomPoint(t, points[indices[0]], points[indices[1]],
						   points[indices[2]], points[indices[3]], pos, deriv);
	}

	void renderCatmullRomCurve(Point pos, Point deriv)
	{

		// draw curve using line segments with GL_LINE_LOOP
		float gt = 0.0;
		int NUM_STEPS = 1000;
		float step = 1.0 / NUM_STEPS;
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < NUM_STEPS; i++)
		{
			getGlobalCatmullRomPoint(gt, &pos, &deriv);

			glVertex3f(pos.getX(), pos.getY(), pos.getZ());

			gt += step;
		}
		glEnd();
	}

	void apply()
	{
		if (type == "translate")
		{
			if (points.size() == 0)
			{
				glTranslatef(x, y, z);
			}
			else
			{
				float static t = 0;
				Point pos1;
				Point deriv1;
				float elapse = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
				float gt = elapse / time;
				renderCatmullRomCurve(pos1, deriv1);

				Point pos2;
				Point deriv2;
				getGlobalCatmullRomPoint(gt, &pos2, &deriv2);
				glTranslatef(pos2.getX(), pos2.getY(), pos2.getZ());

				if (align)
				{

					float vecAux[3] = {0, 1, 0};
					float vecX[3] = {deriv2.getX(), deriv2.getY(), deriv2.getZ()};
					normalize(vecX);

					float vecZ[3];
					cross(vecX, vecAux, vecZ);
					normalize(vecZ);

					float vecY[3];
					cross(vecZ, vecX, vecY);
					normalize(vecY);

					float m[16];
					buildRotMatrix(vecX, vecY, vecZ, m);

					glMultMatrixf(m);
				}
			}
		}
		else if (type == "rotate")
		{
			if (time == 0.0)
			{
				glRotatef(angle, x, y, z);
			}
			else
			{
				float elapsed = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
				angle = (elapsed / time) * 360.0;
				glRotatef(angle, x, y, z);
			}
		}
		else if (type == "scale")
		{
			glScalef(x, y, z);
		}
	}
};

class Color {
public:
	Color(float r = 0, float g = 0, float b = 0) : r(r), g(g), b(b) {}

	float getR() const { return r; }
	float getG() const { return g; }
	float getB() const { return b; }

	void setR(float r) { this->r = r; }
	void setG(float g) { this->g = g; }
	void setB(float b) { this->b = b; }
	
	float* colorToColorGL() const {
		float* colorGL = new float[4];
		colorGL[0] = r / 255.0f;
		colorGL[1] = g / 255.0f;
		colorGL[2] = b / 255.0f;
		colorGL[3] = 1.0f; // Alpha value
		return colorGL;
	}

	std::string toString() const {
		std::ostringstream oss;
		oss << "Color(r: " << r << ", g: " << g << ", b: " << b << ")";
		return oss.str();
	}
private:
	int r, g, b;
};

void loadTexture(unsigned int* textureId, std::string textureFile) {

	unsigned int t, tw, th;
	unsigned char* texData;
	ilGenImages(1, &t);
	ilBindImage(t);
	ilLoadImage((ILstring)textureFile.data());
	tw = ilGetInteger(IL_IMAGE_WIDTH);
	th = ilGetInteger(IL_IMAGE_HEIGHT);
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	texData = ilGetData();

	glGenTextures(texturesIds, textureId);
	glBindTexture(GL_TEXTURE_2D, *textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	texturesIds++;
}



class Model
{
private:
	unsigned int textureId = 0;


	GLuint buffersID[3];

	std::string filePath;
	std::string fileTexture = "None";
	int numVertices;
	std::vector<Transformation> transformationList;

	Color diffuse;
	Color ambient;
	Color specular;
	Color emissive;
	float shininess;


public:
	Model() :  textureId(0), numVertices(0){}
	/*
		diffuse(Color(1.0f, 1.0f, 1.0f)),    // white diffuse color
		ambient(Color(0.1f, 0.1f, 0.1f)),    // dark grey ambient color
		specular(Color(1.0f, 1.0f, 1.0f)),   // white specular color
		emissive(Color(0.0f, 0.0f, 0.0f)),   // black emissive color
		shininess(32.0f) {}                  // default shininess value
		*/

	void setDiffuse(const Color& color) { diffuse = color; }
	void setAmbient(const Color& color) { ambient = color; }
	void setSpecular(const Color& color) { specular = color; }
	void setEmissive(const Color& color) { emissive = color; }
	void setShininess(float value) { shininess = value; }



	void setFilePath(std::string fp) { filePath = fp; }
	void setFileText(std::string ft) { fileTexture = ft; }
	void setTextureId(unsigned int tid) {textureId = tid;}
	
	std::string getPath()
	{
		return filePath;
	}

	GLuint getNumVertices()
	{
		return numVertices;
	}

	std::vector<Transformation> getTransf()
	{
		return transformationList;
	}

	void setTransf(std::vector<Transformation> tList)
	{
		transformationList = tList;
	}

	std::string toString() const {
		std::ostringstream oss;
		oss << "Model(";
		oss << "filePath: " << filePath << ", ";
		oss << "numVertices: " << numVertices << ", ";
		oss << "transformations: [";
		for (const auto& t : transformationList) {
			oss << t.toString() << ", ";
		}
		oss << "], ";
		oss << "diffuse: " << diffuse.toString() << ", ";
		oss << "ambient: " << ambient.toString() << ", ";
		oss << "specular: " << specular.toString() << ", ";
		oss << "emissive: " << emissive.toString() << ", ";
		oss << "shininess: " << shininess;
		oss << ")";
		return oss.str();
	}

	void importModel(GLint bufferIndex)
	{
		
		std::ifstream arquivo(filePath);

		if (!arquivo.is_open())
		{
			std::cerr << "Erro ao abrir o arquivo. " << filePath << std::endl;
			return;
		}
		else
		{
			std::cout << "Abriu! " << filePath << std::endl;
		}

		std::string linha;

		std::vector<float> vertexB;
		std::vector<float> normalB;
		std::vector<float> texturB;

		while (std::getline(arquivo, linha))
		{

			std::istringstream iss(linha);
			char tipo;
			float num1, num2, num3;
			iss >> tipo; // Le o Tipo
			iss.ignore();		// Ignora o ponto e virgula
			iss >> num1;		// Le o primeiro numero
			iss.ignore();		// Ignora o ponto e virgula
			iss >> num2;		// Le o segundo numero
			iss.ignore();		// Ignora o ponto e virgula
			iss >> num3;		// Le o terceiro numero

			if (tipo == 'v') {
				vertexB.push_back(num1);
				vertexB.push_back(num2);
				vertexB.push_back(num3);	
			}
			else if( tipo == 'n') {
				normalB.push_back(float (num1));
				normalB.push_back(float (num2));
				normalB.push_back(float (num3));
			
			}
			else if (tipo == 't') {
				texturB.push_back(float(num1));
				texturB.push_back(float(num2));
			}
		}
		
		numVertices = vertexB.size() / 3;
		
		if(fileTexture != "None") loadTexture(&textureId,fileTexture);

		glGenBuffers(3, buffersID);

		glBindBuffer(GL_ARRAY_BUFFER, buffersID[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexB.size(), vertexB.data(), GL_STATIC_DRAW);				 
		

		glBindBuffer(GL_ARRAY_BUFFER, buffersID[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * normalB.size(), normalB.data(), GL_STATIC_DRAW);
		
		glBindBuffer(GL_ARRAY_BUFFER, buffersID[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * texturB.size(), texturB.data(),	 GL_STATIC_DRAW);


		std::cout <<"Texture V: " << texturB.size() << "Vertexes: " << numVertices;

	}

	void draw()
	{

		if (buffersID[0] != 0)
		{

			for (Transformation t : transformationList)
				t.apply();
			//std::cout << "Desenhar " << numVertices << " Vertices" << std::endl;
			//std::cout << "buffersID " << buffersID[0] << buffersID[1]<< " ;"<< std::endl;
			
		// Para cada modelo:
			if (textureId == 0) {
				glMaterialfv(GL_FRONT, GL_AMBIENT, ambient.colorToColorGL());
				glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse.colorToColorGL());
				glMaterialfv(GL_FRONT, GL_SPECULAR, specular.colorToColorGL());
				glMaterialfv(GL_FRONT, GL_EMISSION, emissive.colorToColorGL());
				glMaterialf(GL_FRONT, GL_SHININESS, shininess);
			}
			else {

				glBindTexture(GL_TEXTURE_2D, textureId);

			}
			

			glBindBuffer(GL_ARRAY_BUFFER, buffersID[0]);	// ESCOLHE O BUFFER NA POS bufferID
			glVertexPointer(3, GL_FLOAT, 0, 0);			// dizer qual é a config do buffer

			
			glBindBuffer(GL_ARRAY_BUFFER, buffersID[1]);
			glNormalPointer(GL_FLOAT, 0, 0);

			if (textureId != 0) {

				glBindBuffer(GL_ARRAY_BUFFER, buffersID[2]);
				glTexCoordPointer(2, GL_FLOAT, 0, 0);
				
			}


			glDrawArrays(GL_TRIANGLES, 0, numVertices);
			
			if (textureId != 0) glBindTexture(GL_TEXTURE_2D, 0);

		}

		

	}
};

Model modelsArray[MAX_MODELS];

void drawAxis()
{
	glBegin(GL_LINES);
	// X axis in red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(
		-400.0f, 0.0f, 0.0f);
	glVertex3f(4200.0f, 0.0f, 0.0f);
	// Y Axis in Green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f,
			   -400.0f, 0.0f);
	glVertex3f(0.0f, 400.0f, 0.0f);
	// Z Axis in Blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f,
			   -400.0f);
	glVertex3f(0.0f, 0.0f, 400.0f);

	// Color white
	glColor3f(1.0f, 1.0f, 1.0f);

	glEnd();
}

void changeSize(int w, int h)
{

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
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
	gluPerspective(fov, ratio, nearPlane, farPlane);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void printTransformations(const std::vector<Transformation> &transformations)
{
	for (const auto &transformation : transformations)
	{
		std::cout << "Type: " << transformation.getType() << ", X: " << transformation.getX() << ", Y: " << transformation.getY() << ", Z: " << transformation.getZ() << std::endl;
	}
}

void calculateFPS()
{

	frame++;
	float fps = 0.0f;
	int time = glutGet(GLUT_ELAPSED_TIME);

	if (time - timebase > 1000)
	{

		fps = frame * 1000.0 / (time - timebase);
		timebase = time;
		frame = 0;

		// Update window title with FPS
		char title[50];
		sprintf(title, "%.2f FPS", fps);
		glutSetWindowTitle(title);
	}
}

void renderScene(void)
{
	glClearColor(0.0f,0.0f,0.0f,0.0f);

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// set the camera
	glLoadIdentity();

	gluLookAt(camX, camY, camZ,
			  lookAtX, lookAtY, lookAtZ,
			  upX, upY, upZ);
	

	glPolygonMode(GL_FRONT_AND_BACK, mode);


	for (Light light : vectorLight) {
		//std::cout << light.toString() <<std::endl;
		light.apply();

	}
	
	// desenhar models VBO's
	for (int i = 0; i < num_models; i++)
	{
		glPushMatrix();
		modelsArray[i].draw();
		glPopMatrix();
	}
	// desenhar axis
	glDisable(GL_LIGHTING);
	drawAxis();
	glEnable(GL_LIGHTING);


	calculateFPS();
	// glutSetWindowTitle("frame");

	// End of frame
	glutSwapBuffers();
}

void processKeys(unsigned char key, int xx, int yy)
{

	// Mudar modos de visualiza��o
	switch (key) {
	case '1':
		mode = GL_FILL;
		break;

	case '2':
		mode = GL_LINE;
		break;

	case '3':
		mode = GL_POINT;
		break;
}

	glutPostRedisplay();
}

void processSpecialKeys(int key, int xx, int yy)
{
	switch (key) {

		case GLUT_KEY_RIGHT:
			alfa += 0.1; break;

		case GLUT_KEY_LEFT:
			alfa -= 0.1; break;

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

		case GLUT_KEY_PAGE_DOWN: radius -= 0.1f;
			if (radius < 0.1f)
				radius = 0.1f;
			break;

		case GLUT_KEY_PAGE_UP: radius += 0.1f; break;

}
	sphericalToCartesian();

}

void printInfo()
{

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n");
	printf("Page Up and Page Down control the distance from the camera to the origin\n");
}

Transformation parseSingleTransformationNode(pugi::xml_node transformation)
{
	std::string type;
	double x, y, z;
	float angle = 0.0f; // Default angle
	double time = 0.0;
	bool align = false;

	type = std::string(transformation.name());
	Transformation t;
	t.setType(type);

	if (type == "translate")
	{
		// Check if there are multiple <point> elements
		if (transformation.child("point"))
		{

			time = transformation.attribute("time").as_double();
			align = transformation.attribute("align").as_bool();
			t.setTime(time);
			t.setAlign(align);

			for (pugi::xml_node point : transformation.children("point"))
			{

				double x = point.attribute("x").as_double();
				double y = point.attribute("y").as_double();
				double z = point.attribute("z").as_double();

				t.addPoint(Point(x, y, z));
			}
		}
		else
		{
			// Directly read x, y, z attributes
			x = transformation.attribute("x").as_double();
			y = transformation.attribute("y").as_double();
			z = transformation.attribute("z").as_double();
			t.setX(x);
			t.setY(y);
			t.setZ(z);
		}
	}
	else if (type == "rotate")
	{
		if (transformation.attribute("time"))
		{
			time = transformation.attribute("time").as_double();
			t.setTime(time);
		}
		else
		{

			angle = transformation.attribute("angle").as_float();
			t.setAngle(angle);
			t.setTime(0.0);
		}

		x = transformation.attribute("x").as_double();
		y = transformation.attribute("y").as_double();
		z = transformation.attribute("z").as_double();

		t.setX(x);
		t.setY(y);
		t.setZ(z);
	}
	else if (type == "scale")
	{
		x = transformation.attribute("x").as_double();
		y = transformation.attribute("y").as_double();
		z = transformation.attribute("z").as_double();
		t.setX(x);
		t.setY(y);
		t.setZ(z);
		t.setAngle(angle);
	}

	return t;
}



void readGroup(pugi::xml_node group, std::vector<Transformation> transVector)
{

	for (pugi::xml_node transformation : group.child("transform").children())
	{
		transVector.push_back(parseSingleTransformationNode(transformation));
	}

	for (pugi::xml_node model : group.child("models").children("model"))
	{
		std::string modelFile = model.attribute("file").as_string();

		for (pugi::xml_node color : model.child("color").children()) {

			std::string name = color.name();

			if (name == "diffuse") {
				float r = color.attribute("R").as_float();
				float g = color.attribute("G").as_float();
				float b = color.attribute("B").as_float();
				modelsArray[num_models].setDiffuse(Color(r, g, b));
			}
			else if (name == "ambient") {
				float r = color.attribute("R").as_float();
				float g = color.attribute("G").as_float();
				float b = color.attribute("B").as_float();
				modelsArray[num_models].setAmbient(Color(r, g, b));
			}
			else if (name == "specular") {
				float r = color.attribute("R").as_float();
				float g = color.attribute("G").as_float();
				float b = color.attribute("B").as_float();
				modelsArray[num_models].setSpecular(Color(r, g, b));
			}
			else if (name == "emissive") {
				float r = color.attribute("R").as_float();
				float g = color.attribute("G").as_float();
				float b = color.attribute("B").as_float();
				modelsArray[num_models].setEmissive(Color(r, g, b));
			}
			else if (name == "shininess") {
				float value = color.attribute("value").as_float();
				modelsArray[num_models].setShininess(value);
			}
		}

		pugi::xml_node textureNode = model.child("texture");

		if (textureNode) {

			std::string textureFile = textureNode.attribute("file").as_string();
			modelsArray[num_models].setFileText(textureFile);

		}
		
		modelsArray[num_models].setFilePath("models/" + modelFile); // linux

		modelsArray[num_models].setTransf(transVector);

		num_models++;
	}

	for (pugi::xml_node childGroup : group.children("group"))
	{
		std::vector<Transformation> newVec = transVector;
		readGroup(childGroup, newVec);
	}
}

void readConfig(const pugi::xml_node &world)
{

	pugi::xml_node window = world.child("window");
	WINDOW_WIDTH = window.attribute("width").as_int();
	WINDOW_HEIGHT = window.attribute("height").as_int();

	pugi::xml_node camera = world.child("camera");

	camX = camera.child("position").attribute("x").as_double();
	camY = camera.child("position").attribute("y").as_double();
	camZ = camera.child("position").attribute("z").as_double();

	lookAtX = camera.child("lookAt").attribute("x").as_double();
	lookAtY = camera.child("lookAt").attribute("y").as_double();
	lookAtZ = camera.child("lookAt").attribute("z").as_double();

	upX = camera.child("up").attribute("x").as_double();
	upY = camera.child("up").attribute("y").as_double();
	upZ = camera.child("up").attribute("z").as_double();

	fov = camera.child("projection").attribute("fov").as_double();
	nearPlane = camera.child("projection").attribute("near").as_double();
	farPlane = camera.child("projection").attribute("far").as_double();

	pugi::xml_node lights = world.child("lights");
	
	int lightId = 0;

	for (pugi::xml_node light : lights.children("light")) {

		LightType type = stringToLightType( light.attribute("type").as_string());

		float dirx = 0.0f, diry = 0.0f, dirz = 0.0f;
		float posx = 0.0f, posy = 0.0f, posz = 0.0f;
		float cutoff = 0.0f;

		switch (type) {
		case DIRECTIONAL:
			dirx = light.attribute("dirx").as_float();
			diry = light.attribute("diry").as_float();
			dirz = light.attribute("dirz").as_float();

			break;

		case POINT:
			
			posx = light.attribute("posx").as_float();
			posy = light.attribute("posy").as_float();
			posz = light.attribute("posz").as_float();

			break;

		case SPOT:
			posx = light.attribute("posx").as_float();
			posy = light.attribute("posy").as_float();
			posz = light.attribute("posz").as_float();
			dirx = light.attribute("dirx").as_float();
			diry = light.attribute("diry").as_float();
			dirz = light.attribute("dirz").as_float();
			cutoff = light.attribute("cutoff").as_float();

			break;

		default:
			throw std::runtime_error("Unknown light type");
		}
		vectorLight.push_back( Light(type,lightId, dirx, diry, dirz, posx, posy, posz, cutoff));
		lightId++;

	}

	for (pugi::xml_node group : world.children("group"))
	{
		std::vector<Transformation> transVector;
		readGroup(group, transVector);
	}
}

void printConfig(const pugi::xml_node &node, int depth = 0)
{
	// Output node name with indentation based on depth
	std::cout << std::string(depth * 2, ' ') << "Node: " << node.name() << std::endl;

	// Print attributes of the node
	for (pugi::xml_attribute attr : node.attributes())
	{
		std::cout << std::string(depth * 2 + 2, ' ') << "Attribute: " << attr.name() << " = " << attr.value() << std::endl;
	}

	const char *value = node.child_value();

	if (value && strlen(value) > 0)
	{
		std::cout << std::string(depth * 2 + 2, ' ') << "Value: " << value << std::endl;
	}

	// Recursively print children
	for (pugi::xml_node child : node.children())
	{
		printConfig(child, depth + 1);
	}
}

int main(int argc, char **argv)
{
	
	// init GLUT and the window
	glutInit(&argc, argv);
	
	std::string xmlFilePath = CONFIGS_DIR + "\\test_files_phase_4\\" + "test_4_5.xml";

	// Load the XML file
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(xmlFilePath.c_str());

	pugi::xml_node world = doc.child("world");
	readConfig(world);
	printConfig(world);


	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("CG@DI-UM");

	// Required callback registry
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

	// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);


	
	// init GLEW;
	glewInit();
	ilInit();

	//  OpenGL settings
	glEnable(GL_RESCALE_NORMAL);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnableClientState(GL_VERTEX_ARRAY); // MODO VBO ATIVO
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_LIGHTING);


	// Inicialização:
	float amb[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float white[4] = { 1.0, 1.0, 1.0, 1.0 };

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);

	for (size_t i = 0; i < vectorLight.size(); i++)
	{
		glEnable(GL_LIGHT0 + i);
		glLightfv(GL_LIGHT0 + i, GL_DIFFUSE, white);
		glLightfv(GL_LIGHT0 + i, GL_SPECULAR, white);
	}
	

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	printInfo();

	for (size_t i = 1; i < num_models+1; i++)
	{

		modelsArray[i-1].importModel(i);

	}


	// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}
