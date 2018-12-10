/*********************************************************
FILE : main.cpp (csci3260 2018-2019 Assignment 2)
*********************************************************/
/*********************************************************
Student Information
Student ID:1155083016
Student Name:LAM MING YUEN
*********************************************************/
#define _CRT_SECURE_NO_WARNINGS
#define SPACEBAR 32
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
using glm::vec3;
using glm::mat4;
GLfloat viewWidth = 0.0f, viewHeight = 0.0f;
GLuint diffuseLight_c, specularLight_c;
GLuint VertexArrayID1, vertexbuffer1, uvbuffer1, normalsbuffer1;
GLuint VertexArrayID2, vertexbuffer2, uvbuffer2, normalsbuffer2;
GLuint VertexArrayID3, vertexbuffer3, uvbuffer3, normalsbuffer3;
GLuint VertexArrayID4, vertexbuffer4, uvbuffer4, normalsbuffer4;
bool flag = false;
int drawSize[4];
glm::mat4 projectionMatrix;
GLuint projectionMatrixUniformLocation;
GLuint Texture;
GLint programID;
GLfloat diffuse = 1.0f, specular = 1.0f;
GLfloat ha = 0.0f, va = 0.0f;
int object = 0;
GLuint loadBMP_custom(const char * imagepath);
bool rotte = true;
float theta = 0.0f;
float theta_delta = 0.0008f;
//a series utilities for setting shader parameters 
void setMat4(const std::string &name, glm::mat4& value)
{
	unsigned int transformLoc = glGetUniformLocation(programID, name.c_str());
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(value));
}
void setVec4(const std::string &name, glm::vec4 value)
{
	glUniform4fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}
void setVec3(const std::string &name, glm::vec3 value)
{
	glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}
void setFloat(const std::string &name, float value)
{
	glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}
void setInt(const std::string &name, int value)
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}
bool checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;

		delete[] buffer;
		return false;
	}
	return true;
}
bool checkShaderStatus(GLuint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}
bool checkProgramStatus(GLuint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}
string readShaderCode(const char* fileName)
{
	ifstream meInput(fileName);
	if (!meInput.good())
	{
		cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>()
	);
}
void installShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	string temp = readShaderCode("VertexShaderCode.glsl"); 
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		return;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glUseProgram(programID);
}
int key_key, key_x, key_y;
void keyboard(unsigned char key, int x, int y)
{
	//TODO: Use keyboard to do interactive events and animation
	switch (key)
	{
	case SPACEBAR: 	flag = !flag; break;
	case '1':Texture = loadBMP_custom("sources/theme1.bmp"); break;
	case '2':Texture = loadBMP_custom("sources/theme2.bmp"); break;
	case '3':Texture = loadBMP_custom("sources/theme3.bmp"); break;
	case 'q': diffuse+=0.1; break;
	case 'w': diffuse-=0.1; break;
	case 'z': specular+=0.1; break;
	case 'x': if (specular > 0)specular -= 0.1; break;
	case 'a': object = 1; break;//plane
	case 's': object = 2; break;//block
	case 'd': object = 3; break;//jeep
	case 'r': rotte = !rotte; break;
	};

	
}
GLfloat move_key, move_x, move_y;
void move(int key, int x, int y)
{
	//TODO: Use arrow keys to do interactive events and animation
	switch (key)
	{
	case GLUT_KEY_UP:
		cout << "up" << endl;
		move_y += 0.1;
		va += 0.1;
		break;
	case GLUT_KEY_DOWN:
		cout << "down" << endl;
		move_y -= 0.1;
		va -= 0.1;
		break;
	case GLUT_KEY_LEFT:
		cout << "left" << endl;
		move_x -= 0.1;
		ha -= 0.1;
		break;
	case GLUT_KEY_RIGHT:
		cout << "right" << endl;
		move_x += 0.1;
		ha += 0.1;
		break;
	}
}
float mouse_x, mouse_y;

void PassiveMouse(int x, int y)
{
	//TODO: Use Mouse to do interactive events and animation
	if (flag) {
		cout << "SPACEBAR is here" << endl;
		viewWidth+= (mouse_x - x)* 0.2;
		viewHeight+= (mouse_y - y)* 0.2;
		glutPostRedisplay();
	}
	mouse_x = x;
	mouse_y = y;
}
bool loadOBJ(
	const char * path,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals) {
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE * file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 6 for details\n");
		getchar();
		return false;
	}

	while (1) {

		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

				   // else : parse lineHeader

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y;
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else {
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i<vertexIndices.size(); i++) {

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);

	}

	return true;
}
GLuint loadBMP_custom(const char * imagepath) {

	printf("Reading image %s\n", imagepath);

	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	unsigned char * data;

	FILE * file = fopen(imagepath, "rb");
	if (!file) { printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); return 0; }

	if (fread(header, 1, 54, file) != 54) {
		printf("Not a correct BMP file\n");
		return 0;
	}
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		return 0;
	}
	if (*(int*)&(header[0x1E]) != 0) { printf("Not a correct BMP file\n");    return 0; }
	if (*(int*)&(header[0x1C]) != 24) { printf("Not a correct BMP file\n");    return 0; }

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);
	if (imageSize == 0)    imageSize = width * height * 3;
	if (dataPos == 0)      dataPos = 54;

	data = new unsigned char[imageSize];
	fread(data, 1, imageSize, file);
	fclose(file);


	GLuint textureID;
	// Create one OpenGL texture
	glGenTextures(1, &textureID);
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR,
		GL_UNSIGNED_BYTE, data);
	//TODO: Create one OpenGL texture and set the texture parameter 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	// OpenGL has now copied the data. Free our own version
	delete[] data;

	return textureID;
}
void sendDataToOpenGL()
{

	//TODO:
	//Load objects and bind to VAO & VBO
	//Load texture
	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	//plane
	bool res_plane = loadOBJ("sources/plane.obj", vertices, uvs, normals);	glGenVertexArrays(1, &VertexArrayID1);
	glBindVertexArray(VertexArrayID1);	glGenBuffers(1, &vertexbuffer1);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer1);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	glGenBuffers(1, &uvbuffer1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer1);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);	glGenBuffers(1, &normalsbuffer1);
	glBindBuffer(GL_ARRAY_BUFFER, normalsbuffer1);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	drawSize[0] = vertices.size();	vertices.clear();	uvs.clear();	normals.clear();	bool res_jeep = loadOBJ("sources/jeep.obj", vertices, uvs, normals);	glGenVertexArrays(1, &VertexArrayID2);
	glBindVertexArray(VertexArrayID2);	glGenBuffers(1, &vertexbuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	glGenBuffers(1, &uvbuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer2);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);	glGenBuffers(1, &normalsbuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, normalsbuffer2);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	drawSize[1] = vertices.size();
	vertices.clear();	uvs.clear();	normals.clear();

	bool res_block = loadOBJ("sources/block.obj", vertices, uvs, normals);	glGenVertexArrays(1, &VertexArrayID3);
	glBindVertexArray(VertexArrayID3);	glGenBuffers(1, &vertexbuffer3);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer3);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	glGenBuffers(1, &uvbuffer3);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer3);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);	glGenBuffers(1, &normalsbuffer3);
	glBindBuffer(GL_ARRAY_BUFFER, normalsbuffer3);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	drawSize[2] = vertices.size();
	vertices.clear();	uvs.clear();	normals.clear();

	bool res_wolf = loadOBJ("sources/Wolf_obj.obj", vertices, uvs, normals);	glGenVertexArrays(1, &VertexArrayID4);
	glBindVertexArray(VertexArrayID4);	glGenBuffers(1, &vertexbuffer4);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer4);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	glGenBuffers(1, &uvbuffer4);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer4);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);	glGenBuffers(1, &normalsbuffer4);
	glBindBuffer(GL_ARRAY_BUFFER, normalsbuffer4);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);	drawSize[3] = vertices.size();

	//texture
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glUniform1i(TextureID, 0);
}
void paintGL(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	glClearColor(0.0f, 0.0f, 0.5f, 1.0f); //specify the background color
	glEnable(GL_DEPTH_TEST);
		GLint eyePositionUniformLocation = glGetUniformLocation(programID, "eyePositionWorld");	vec3 eyePosition(15+viewWidth, 15+viewHeight, 15.0f);	glUniform3fv(eyePositionUniformLocation, 1, &eyePosition[0]);	GLint lightPositionUniformLocation = glGetUniformLocation(programID, "lightPositionWorld");	vec3 lightPosition(0.f, 20.0f, -22.f);	glUniform3fv(lightPositionUniformLocation, 1, &lightPosition[0]);
	glm::mat4 modelTransformMatrix = glm::mat4(1.0f);
	modelTransformMatrix = glm::lookAt(eyePosition, glm::vec3(-100.0f, -100.0f, -100.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	GLint modelTransformMatrixUniformLocation = glGetUniformLocation(programID, "modelTransformMatrix");
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);

	projectionMatrix = glm::perspective(20.0f, 1.0f, 0.5f, 100.f);
	projectionMatrixUniformLocation = glGetUniformLocation(programID, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

	modelTransformMatrix = glm::lookAt(eyePosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);

	glBindVertexArray(VertexArrayID1);	glDrawArrays(GL_TRIANGLES, 0, drawSize[0]);
	if (object == 1)//jeep
		modelTransformMatrix = glm::lookAt(eyePosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f))*glm::translate(glm::mat4(), glm::vec3(ha, va, 0.0f));
	else
		modelTransformMatrix = glm::lookAt(eyePosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f))*glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	glBindVertexArray(VertexArrayID2);	glDrawArrays(GL_TRIANGLES, 0, drawSize[1]);

	if(object == 2)//blcok
		modelTransformMatrix = glm::lookAt(eyePosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f))*glm::translate(glm::mat4(), glm::vec3(ha, va, 0.0f));
	else
		modelTransformMatrix = glm::lookAt(eyePosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f))*glm::translate(glm::mat4(), glm::vec3(0.0f, 5.0f, -7.0f));
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	glBindVertexArray(VertexArrayID3);	glDrawArrays(GL_TRIANGLES, 0, drawSize[2]);

	if(rotte)
		theta+=0.0002;
	modelTransformMatrix = glm::lookAt(eyePosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f))
		*glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 0.0f))
		*glm::scale(glm::mat4(), glm::vec3(5.0f, 5.0f, 5.0f))
		*glm::translate(glm::mat4(), glm::vec3(1.1f, 0.1f, 0.0f))
		*glm::rotate(glm::mat4(), theta, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	glBindVertexArray(VertexArrayID4);	glDrawArrays(GL_TRIANGLES, 0, drawSize[3]);


	diffuseLight_c = glGetUniformLocation(programID, "DiffuseLightColor");
	vec3 diffuseLight(diffuse, diffuse, diffuse);
	glUniform3fv(diffuseLight_c, 1, &diffuseLight[0]);

	specularLight_c = glGetUniformLocation(programID, "SpecularLightColor");
	vec3 specularLight(specular, specular, specular);
	glUniform3fv(specularLight_c, 1, &specularLight[0]);

	glFlush();
	glutPostRedisplay();
}


void initializedGL(void) //run only once
{
	glEnable(GL_DEPTH_TEST);
	glewInit();
	installShaders();
	sendDataToOpenGL();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutCreateWindow("Assignment 2");

	//TODO:
	/*Register different CALLBACK function for GLUT to response
	with different events, e.g. window sizing, mouse click or
	keyboard stroke */
	initializedGL();
	glutDisplayFunc(paintGL);

	glutKeyboardFunc(keyboard);
	glutSpecialFunc(move);
	glutPassiveMotionFunc(PassiveMouse);

	glutMainLoop();

	return 0;
}