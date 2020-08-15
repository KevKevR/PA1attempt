//
// Concordia University COMP 371 Programming Assignment 2, Summer 2020.
//
// Created by :
//    Jason Beccherini (40130107)
//    Manuel Romero (40023706)
//    Gia-Khang Ernest Nguyen (40091426)
//    Wing-Fei Jason Tiu (40048495)
//    Kevin Rao (40095427)
// due July 27th, 2020.
//

//TODO:
// -when the shadow is figured out, can delete shader with "[...]Test()" method.

#include <iostream>
#include <list>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // cross-platform interface for creating a graphical context,
						// initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>
#include <map> 
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <vector>

using namespace glm;
using namespace std;

GLuint loadTexture(const char* filename);

const int numMainModels = 27;

// Define (initial) window width and height
int window_width = 1024, window_height = 768;

// Position of light source
//slightly offcenter, because the light doesn't like looking straight down.
vec3 lightPos = vec3(0.0f, 10.0f, 0.001f);
//vec3 lightPos = vec3(-2.0f, 4.0f, -1.0f);

// Callback function for handling window resize and key input
void window_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Forward declare CharModel class in order to declare a pointer to selected model to manipulate
class CharModel;
CharModel* selectedModel;

struct SphereOffset {
	float xOffset;
	float yOffset;
	float scaler;
};

struct TexturedColoredVertex
{
	TexturedColoredVertex(vec3 _position, vec3 _color, vec2 _uv)
		: position(_position), color(_color), uv(_uv) {}

	vec3 position;
	vec3 color;
	vec2 uv;
};

// Textured Cube model
const TexturedColoredVertex texturedCubeVertexArray[] = {  // position,                            color
	//                      position(unused)        color(unused)           uv
	TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)), //left - red
	TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)),

	TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f)),

	TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)), // far - blue
	TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f)),

	TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)),
	TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),

	TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)), // bottom - turquoise
	TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)),

	TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)),
	TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)),
	TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)),

	TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)), // near - green
	TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f)),

	TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)),
	TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f)),

	TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)), // right - purple
	TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f)),

	TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
	TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)),
	TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f)),

	TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)), // top - yellow
	TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f)),

	TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f)),

	//line
	TexturedColoredVertex(vec3(0.0f, 0.0f, -0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f)),
	TexturedColoredVertex(vec3(0.0f, 0.0f, 0.5f), vec3(0.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f)),

	//Square (-0.5,0,-0.5) to (0.5,0,0.5)
	TexturedColoredVertex(vec3(0.5f, 0.0f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)), // top - yellow
	TexturedColoredVertex(vec3(0.5f, 0.0f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.0f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),

	TexturedColoredVertex(vec3(0.5f, 0.0f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.0f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f)),
	TexturedColoredVertex(vec3(-0.5f, 0.0f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f)),
};

struct KeyState {
	int keyState;
	bool needShiftPressed;
	bool prevWithShiftPressed;
};
//class MatrixHolder {
//    ~MatrixHolder() {
//        mat4 matrix = mat4(1.0f);
//    }
//    mat4 getMatrix() {
//        return matrix;
//    }
//    void setMatrix(mat4 m) {
//        matrix = m;
//    }
//    void addMatrix(mat4 m) {
//        matrix = m * matrix;
//    }
//private:
//    mat4 matrix;
//};
class WalkCycle {
public:
	WalkCycle(float tPerState = 1) {
		timePerState = tPerState;
		timeInState = 0;
		walkState = 0;
		currentPosition = 0;
		stopWalkPosition = 0;
	}
	//increment time step
	void stepForward(float dt) {
		//speed controlled with timePerState.
		switch (walkState) {
		case 1:
		{
			//walking, periodic movement from -0.5 to 0.5.
			//set direction to -1 or 1;
			const int direction = 1;
			currentPosition += dt / timePerState * direction;
			break;
		}
		case 2:
		{
			//return to 0.
			int towardsCenter = sign(currentPosition);
			if (fabsf(currentPosition) < 0.25f) {
				towardsCenter *= -1;
			}
			currentPosition += copysignf(dt / timePerState, towardsCenter);

			//stop when returned to position 0.
			if (signbit(currentPosition) != signbit(stopWalkPosition)) {
				currentPosition = 0;
				setState(0);
			}

			//stop when returned to position 0. timer in case.
			timeInState += dt;
			if (timeInState >= timePerState) {
				currentPosition = 0;
				setState(0);
			}
			break;
		}
		case 0:
		default:
		{
			//stop
			currentPosition = 0;
			break;
		}
		}
		//bound to approx. [-0.5f, 0.5f] interval.
		//positive modulo 
		//https://stackoverflow.com/questions/14997165/fastest-way-to-get-a-positive-modulo-in-c-c
		//currentPosition = fmodf(currentPosition + 0.5f, 1) -0.5f ;
		const float mod = 1;
		currentPosition = fmodf(mod + fmodf(currentPosition + 0.5f, mod), mod) - 0.5f;
	}

	//allow walkState to change.
	void setState(int s) {
		walkState = s;
		timeInState = 0;
		stopWalkPosition = currentPosition;
	}
	int getState() {
		return walkState;
	}
	//allow period to change
	void setTimePerState(float tPerState) {
		timePerState = tPerState;
	}

	//return position
	float getPosition() {
		//position between [-1, 1].
		//return currentPosition * 2;
		return sinf(radians(180.0f * (currentPosition * 2)));
	}
private:
	//walkState
	// 0: stop.
	// 1: walking, periodic movement.
	// 2: return to default position.
	int walkState;
	//period
	float timePerState;
	//duration
	float timeInState;
	//current position.
	float currentPosition;
	//stop position
	float stopWalkPosition;
};

class CharModel {
public:
	CharModel(int shaderProgram, mat4 initialTranslateMatrix = mat4(1.0f)) {
		worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
		colorLocation = glGetUniformLocation(shaderProgram, "objectColor");

		initial_relativeTranslateMatrix = initialTranslateMatrix;
		initial_relativeRotateMatrix = mat4(1.0f);
		initial_relativeScaleMatrix = mat4(1.0f);

		relativeTranslateMatrix = initial_relativeTranslateMatrix;
		relativeRotateMatrix = initial_relativeRotateMatrix;
		relativeScaleMatrix = initial_relativeScaleMatrix;
		initY = 0;

		walkState = WalkCycle(1);
		sphereOffset = { 0,0 };
	}

	//let other class handle walking.
	WalkCycle walkState;
	float updateWalkProgress(float dt, int st = -1, float tState = -1) {
		//increment time
		walkState.stepForward(dt);

		//update walkState if set to do so.
		if (!(st < 0)) {
			walkState.setState(st);
		}
		//update time per walkState if set to do so.
		if (!(tState < 0)) {
			walkState.setTimePerState(tState);
		}

		//return position
		return walkState.getPosition();
	}
	GLuint swapWorldMatrixLocation(GLuint wml) {
		GLuint temp = worldMatrixLocation;
		worldMatrixLocation = wml;
		return temp;
	}

	mat4 getRelativeTranslateMatrix() {
		return relativeTranslateMatrix;
	}
	void setRelativeTranslateMatrix(mat4 relTWM) {
		relativeTranslateMatrix = relTWM;
	}
	void addRelativeTranslateMatrix(mat4 relTWM) {
		relativeTranslateMatrix = relTWM * relativeTranslateMatrix;
	}

	mat4 getRelativeRotateMatrix() {
		return relativeRotateMatrix;
	}
	void setRelativeRotateMatrix(mat4 relRWM) {
		relativeRotateMatrix = relRWM;
	}
	void addRelativeRotateMatrix(mat4 relRWM) {
		relativeRotateMatrix = relRWM * relativeRotateMatrix;
	}

	//return scale matrix without shear elements
	mat4 getRelativeUndeformedScaleMatrix() {
		//mat4 motion = walkMotion(walkState.getPosition());
		mat4 scale = relativeScaleMatrix/* * motion*/;

		//float heightOffset = sphereOffset.yOffset;
		//float sideMovement = motion[1].x * heightOffset;
		//float verticalMovement = (motion[1].y - 1) * heightOffset;
		//mat4 follow = translate(mat4(1.0f),
		//    vec3(sideMovement,
		//        verticalMovement,
		//        0));

		//float minScale = min(min(relativeScaleMatrix[0].x, relativeScaleMatrix[1].y), relativeScaleMatrix[2].z);
		float minScale = min(min(scale[0].x, scale[1].y), scale[2].z);
		mat4 diagonal = mat4(minScale, 0, 0, 0,
			0, minScale, 0, 0,
			0, 0, minScale, 0,
			0, 0, 0, 1);

		return diagonal/* * follow*/;
	}
	mat4 getRelativeScaleMatrix() {
		mat4 motion = walkMotion(walkState.getPosition());
		return relativeScaleMatrix * motion;
	}
	void setRelativeScaleMatrix(mat4 relSWM) {
		relativeScaleMatrix = relSWM;
	}
	void addRelativeScaleMatrix(mat4 relSWM) {
		relativeScaleMatrix = relSWM * relativeScaleMatrix;
	}

	void setRelativeWorldMatrix(mat4 relTWM, mat4 relRWM, mat4 relSWM) {
		setRelativeTranslateMatrix(relTWM);
		setRelativeRotateMatrix(relRWM);
		setRelativeScaleMatrix(relSWM);
	}
	void addRelativeWorldMatrix(mat4 relTWM, mat4 relRWM, mat4 relSWM) {
		addRelativeTranslateMatrix(relTWM);
		addRelativeRotateMatrix(relRWM);
		addRelativeScaleMatrix(relSWM);
	}
	mat4 getRelativeWorldMatrix() {
		return getRelativeTranslateMatrix() * getRelativeRotateMatrix() * getRelativeScaleMatrix();
	}
	//return without shear elements
	mat4 getRelativeUndeformedWorldMatrix() {
		return getRelativeTranslateMatrix() * getRelativeRotateMatrix() * getRelativeUndeformedScaleMatrix();
	}

	virtual void draw() {
		//implement in derived class please.
	}

	virtual void drawNumber() {
		//implement in derived class please.
	}
	virtual void drawSphere() {
		//implement in derived class please.
	}

	//reverts TRS matrices back to initial settings.
	void resetInitialRelativeMatrices() {
		setRelativeWorldMatrix(initial_relativeTranslateMatrix, initial_relativeRotateMatrix, initial_relativeScaleMatrix);
	}

	//Class method to draws all passed models. (issues when unloaded models).
	static void draw(CharModel* arr[numMainModels]) {
		for (int i = 0; i < numMainModels; i++) {
			if (arr[i]) {
				arr[i]->draw();
			}
		}
	}
	static void drawNumber(CharModel* arr[numMainModels]) {
		for (int i = 0; i < numMainModels; i++) {
			if (arr[i]) {
				arr[i]->drawNumber();
			}
		}
	}
	static void drawSphere(CharModel* arr[numMainModels]) {
		for (int i = 0; i < numMainModels; i++) {
			if (arr[i]) {
				arr[i]->drawSphere();
			}
		}
	}
	static vector<int> update(CharModel* arr[numMainModels], float dt) {
		vector<int> positions(numMainModels);
		for (int i = 0; i < numMainModels; i++) {
			if (arr[i]) {
				positions[i] = arr[i]->updateWalkProgress(dt);
			}
		}
		return positions;
	}
	static GLuint swapWorldMatrixLocation(CharModel* arr[numMainModels], GLuint wml) {
		GLuint temp = arr[0]->worldMatrixLocation;
		for (int i = 0; i < numMainModels; i++) {
			if (arr[i]) {
				temp = arr[i]->swapWorldMatrixLocation(wml);
			}
		}
		return temp;
	}
	// Return initial y-position of model
	float getInitY() { return initY; }

protected:
	SphereOffset sphereOffset;
	//shear motion when walking
	mat4 walkMotion(float position) {
		const float amplitude = 4;
		//const float position = walkState.getPosition();

		//shear side-to-side + vertical scaling.
		mat4 motion =
			mat4(1, 0, 0, 0,  // first column
			//amplitude * sinf(radians(180.0f*position)), 1, 0, 0,  // second column
				amplitude * position, 1 + sqrt(amplitude) * powf(position, 2), 0, 0,  // second column
				0, 0, 1, 0,  // third column
				0, 0, 0, 1); // fourth column
		return motion;
	}
	//mat4 followWalkMotion(mat4 motion) {
	//    float sideMovement = motion[1].x;
	//    float verticalMovement = motion[1].y;
	//    mat4 follow = translate(mat4(1.0f),
	//        vec3(sideMovement,
	//            verticalMovement,
	//            0));
	//    return follow;
	//}
	mat4 sphereFollow() {
		mat4 motion = walkMotion(walkState.getPosition());

		float heightOffset = sphereOffset.yOffset;
		float sideMovement = motion[1].x * heightOffset;
		float verticalMovement = (motion[1].y - 1) * heightOffset;
		mat4 follow = translate(mat4(1.0f),
			vec3(sideMovement,
				verticalMovement,
				0));
		return follow;
	}

	void drawBorder(float modelMaxHeight, float modelMaxWidth, float boxVerticalWidth, float boxHorizontalWidth, mat4 relativeWorldMatrix = mat4(1.0f), mat4 modelPositioningMatrix = mat4(1.0f)) {
		//draw box around model(extra)
		//const float boxVerticalWidth = heightScale / 100;   //thickness of horizontal bars
		//const float boxHorizontalWidth = widthScale / 20;   //thickness of vertical bars
		//const float modelMaxHeight = m_height;              //total height of model to surround
		//const float modelMaxWidth = m_maxWidth;             //total width of model to surround


		//modelPositioningMatrix is to set center of model for the box to surround. This is also sets its orientation (default flush against xy-plane).


		for (int i = 0; i < 4; i++) {
			const bool isHorizontal = i < 2;
			const int parity = powf(-1, i + 1);

			//horizontal and vertical bar distinction. 
			mat4 translateMatrix = (isHorizontal)
				//Move bar to top/bottom
				? translate(mat4(1.0f),
					vec3(0,
						(modelMaxHeight + boxVerticalWidth) / 2 * parity,
						0))
				//Move bar to sides
				: translate(mat4(1.0f),
					vec3((modelMaxWidth + boxHorizontalWidth) / 2 * parity,
						0,
						0));
			mat4 scaleMatrix = (isHorizontal)
				//Size bar to span width
				? scale(mat4(1.0f),
					vec3(modelMaxWidth + 2 * boxHorizontalWidth, boxVerticalWidth, 1.0f))
				//Size bar to span height
				: scale(mat4(1.0f),
					vec3(boxHorizontalWidth, modelMaxHeight, 1.0f));

			mat4 inertialWorldMatrix =
				modelPositioningMatrix
				//position around shape of model.
				* translateMatrix
				//scale to whole width.
				* scaleMatrix;
			mat4 worldMatrix = relativeWorldMatrix * inertialWorldMatrix;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			//glUniform3f(colorLocation, 1.0f, 233.0f / 255.0f, 0.0f);
			glDrawArrays(GL_TRIANGLES, 0, 36);

		}
	}

	// Draw a sphere surrounding model
	void drawSphere(mat4 relativeWorldMatrix) {
		// Draw sphere
		//attach the sphere to the model with a point lower/higher than its center.
		const float yHover = 0.0f;

		const float xOffset = sphereOffset.xOffset;
		const float yOffset = sphereOffset.yOffset + yHover;
		const float scaler = sphereOffset.scaler;


		//mat4 worldMatrix = translate(mat4(1.0f), vec3(xOffset, yOffset, 0.0f)) * scale(mat4(1.0f), glm::vec3(-scaler, -scaler, -scaler));
		mat4 worldMatrix = translate(mat4(1.0f), vec3(xOffset, yOffset, 0.0f)) * scale(mat4(1.0f), glm::vec3(scaler, scaler, scaler));
		mat4 mWorldMatrix = relativeWorldMatrix * sphereFollow() * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);


		//match these numbers to those passed to sphereVertices().
		const int heightParts = 16;
		const int ringParts = 30;
		const int numVertices = (heightParts - 1) * ringParts * 2 * 2;
		glDrawArrays(GL_TRIANGLE_STRIP, 44, numVertices);
	}


	GLuint worldMatrixLocation;
	GLuint colorLocation;
	mat4 initial_relativeTranslateMatrix;   //Initial translate matrix, value to take when reset.
	mat4 initial_relativeRotateMatrix;      //Initial rotate matrix, value to take when reset.
	mat4 initial_relativeScaleMatrix;       //Initial scale matrix, value to take when reset.
	float initY;                            // Initial y-position in initial translate matrix (note: value assigned to child constructor)
private:

	mat4 relativeTranslateMatrix;   //Stored translate matrix
	mat4 relativeRotateMatrix;      //Stored rotate matrix
	mat4 relativeScaleMatrix;       //Stored scale matrix
};

class ModelBoxFace : public CharModel {
public:
	ModelBoxFace(int shaderProgram, mat4 initialTranslateMatrix = mat4(1.0f)) : CharModel(shaderProgram, initialTranslateMatrix) {

		sphereOffset = { 0.0f, 9.0f, 5.5f };
	}

	void draw() {
		//pass arguments stored in parent class.
		glUniform3f(colorLocation, 1.0f, 1.0f, 1.0f);
		drawFace(worldMatrixLocation, colorLocation, getRelativeWorldMatrix());
	}


private:
	void drawFace(GLuint worldMatrixLocation, GLuint colorLocation, mat4 relativeWorldMatrix) {
		//code goes here
		mat4 mWorldMatrix;
		mat4 translationMatrix, scalingMatrix;
		mat4 worldMatrix;

		translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 3.0f, 3.0f));
		worldMatrix = translationMatrix * scalingMatrix;
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}


};

vector<vec3> sphereVertices(vec3 prevVertexArray[44 * 2], const int heightParts, const int ringParts) {

	const int vertexArrNum = 44;
	/*
	vec3 vertexArr[vertexArrNum * 2] = {
		//left
		vec3(-0.5f, -0.5f, -0.5f), vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f, -0.5f, 0.5f), vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f, 0.5f, 0.5f), vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f, -0.5f, -0.5f), vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f, 0.5f, 0.5f), vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f, 0.5f, -0.5f), vec3(-1.0f, 0.0f, 0.0f),
		// far
		vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, -1.0f),
		vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, -1.0f),
		vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, -1.0f),
		vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 0.0f, -1.0f),
		vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, -1.0f),
		vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, 0.0f, -1.0f),
		// bottom
		vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, -1.0f, 0.0f),
		vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, -1.0f, 0.0f),
		vec3(0.5f, -0.5f, -0.5f), vec3(0.0f, -1.0f, 0.0f),
		vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, -1.0f, 0.0f),
		vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, -1.0f, 0.0f),
		vec3(-0.5f, -0.5f, -0.5f), vec3(0.0f, -1.0f, 0.0f),
		// near
		vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		vec3(-0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		vec3(0.5f, -0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		// right
		vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f, 0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f, -0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f, -0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		// top
		vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
		vec3(0.5f, 0.5f, -0.5f), vec3(0.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 1.0f, 0.0f),
		vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.5f, -0.5f), vec3(0.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
		//line (0,0,-0.5)to(0,0,0.5)
		vec3(0.0f, 0.0f, -0.5f), vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, 0.0f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
	};
	*/
	vec3* vertexArr = prevVertexArray;

	//plan to compute sphere vertices by iterating along cylindrical coordinates -> convert to rectangular.
	//sphere radius, rho
	const float radius = 1.0f;
	//division of sphere parts/resolution
	//# of angles to divide ring in xz-plane.
	//const int ringParts = 5;
	//ringParts minimum is 3.
	//# of segments height in y is divided into.
	//const int heightParts = 4;
	//heightParts minimum is 3.


	//# of vertices is 2 * ringParts per heightParts, ommitting the topmost.
	//44*2 + (heightParts-1)*ringParts*2*2
	//const int numVertices = (heightParts - 1) * ringParts * 2;

	//vec3 sphereArray[numVertices] = {};
	vector<vec3> vecSphereArray(vertexArrNum * 2);


	//prepend previous array
	for (int i = 0; i < vertexArrNum * 2; i++) {
		vecSphereArray[i] = vertexArr[i];
	}


	/// attempt 3
	//for every height part
	for (int i = 0; i < heightParts - 1; i++) {
		//for each part of a ring
		for (int j = 0; j < ringParts; j++) {
			//alternate between heights to form a strip.
			//slightly redundant on bottommost and topmost vertices.
			for (int k = 0; k < 2; k++) {
				int alternate = k;
				//y is height
				// v v v diameter divided by area part count, then center to origin. so spacing is by diameter
				float ya = (i + alternate) * 2 * radius / (heightParts - 1) - radius;
				// v v v height segment is given by phi angle.
				float y = radius * (sinf(radians(2 * 90 * ((float)(i + alternate) / (heightParts - 1) - 0.5f))));
				//vertices for a xz-ring at height y.
				//r is radial distance
				float r = sqrt(radius * radius - y * y);

				//ring
				float theta = radians(j * 360.0f / ringParts);
				float x = r * cosf(theta);
				float z = r * sinf(theta);
				//every height will have ringParts# * 2 of elements

				vecSphereArray.push_back(vec3(x, y, z));
				vecSphereArray.push_back(vec3(x, y, z));
			}
		}
	}

	//return sphereArray;
	return vecSphereArray;
}

int createTexturedCubeVertexArrayObject()
{
	// Cube model (used for models and axis)
	vec3 vertexArray[] = {  // position and normal
		//cube (-0.5,-0.5,-0.5) to (0.5,0.5,0.5)
		//left
		vec3(-0.5f,-0.5f,-0.5f), vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f,-0.5f, 0.5f), vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f, 0.5f, 0.5f), vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f,-0.5f,-0.5f), vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f, 0.5f, 0.5f), vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f, 0.5f,-0.5f), vec3(-1.0f, 0.0f, 0.0f),
		// far
		vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, -1.0f),
		vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, -1.0f),
		vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, -1.0f),
		vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, -1.0f),
		vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, -1.0f),
		vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, -1.0f),
		// bottom
		vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, -1.0f, 0.0f),
		vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, -1.0f, 0.0f),
		vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, -1.0f, 0.0f),
		vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, -1.0f, 0.0f),
		vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, -1.0f, 0.0f),
		vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, -1.0f, 0.0f),
		// near
		vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		// right
		vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		// top
		vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
		vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 1.0f, 0.0f),
		vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),

		//line (0,0,-0.5)to(0,0,0.5)
		vec3(0.0f, 0.0f, -0.5f), vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, 0.0f, 0.5f), vec3(0.0f, 1.0f, 0.0f),

		//Square (-0.5,0,-0.5) to (0.5,0,0.5)
		vec3(0.5f, 0.0f, 0.5f),vec3(0.0f, 1.0f, 0.0f),
		vec3(0.5f, 0.0f,-0.5f), vec3(0.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.0f,-0.5f),vec3(0.0f, 1.0f, 0.0f),
		vec3(0.5f, 0.0f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.0f,-0.5f), vec3(0.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.0f, 0.5f),vec3(0.0f, 1.0f, 0.0f),
	};

	//create instancing array for tile grid. 
	const int sideLength = 100;
	const float cellLength = 1.0f;
	const float height = 0.0f;
	vec3 offsetArray[sideLength * sideLength] = {};
	for (int i = 0; i < sideLength; ++i)
	{
		for (int j = 0; j < sideLength; ++j)
		{
			//skip the extra one (the last one)
			if (i == sideLength - 1 && j == sideLength - 1) {
				break;
			}
			offsetArray[sideLength * i + j + 1] =
				vec3(-cellLength * sideLength + (i + 1) * cellLength,
					height,
					-cellLength * sideLength + (j + 1) * cellLength);
		}
	}

	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	//Have multiple arrays/buffer.
	//https://www.khronos.org/opengl/wiki/Tutorial2:_VAOs,_VBOs,_Vertex_and_Fragment_Shaders_(C_/_SDL)
	GLuint vertexBufferObject[3];
	glGenBuffers(3, vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texturedCubeVertexArray), texturedCubeVertexArray, GL_STATIC_DRAW);

	//// unused location
	//glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
	//    3,
	//    GL_FLOAT,
	//    GL_FALSE,
	//    sizeof(TexturedColoredVertex),
	//    (void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
	//);
	//glEnableVertexAttribArray(1);

	glVertexAttribPointer(2,                            // attribute 2 matches aUV in Vertex Shader
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(TexturedColoredVertex),
		(void*)(2 * sizeof(vec3))      // uv is offseted by 2 vec3 (comes after position and color)
	);
	glEnableVertexAttribArray(2);


	const int vertexArrayNum = 44;
	const int heightParts = 22;
	const int ringParts = 30;
	//make sure the sphere draw function draws [(heightParts-1)*ringParts*2] figures.
	//https://stackoverflow.com/questions/4264304/how-to-return-an-array-from-a-function
	vector<vec3> vertexArraySphere = sphereVertices(vertexArray, heightParts, ringParts);

	vec3 vertexArr2[vertexArrayNum * 2 + (heightParts - 1) * ringParts * 2 * 2];
	//convert it to array
	for (int i = 0; i < vertexArraySphere.size(); i++) {
		vertexArr2[i] = vertexArraySphere[i];
	}

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject[1]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);
	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	//GLuint vertexBufferObject;
	//glGenBuffers(1, &vertexBufferObject);
	//glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArr2), vertexArr2, GL_STATIC_DRAW);

	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		2 * sizeof(vec3),        // stride - each vertex contains vec3 (position)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(4,                   // attribute 1 matches aNormal in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		2 * sizeof(vec3),        // stride - each vertex contains vec3 (position)
		(void*)sizeof(vec3)  // array buffer offset
	);
	glEnableVertexAttribArray(4);

	//https://learnopengl.com/Advanced-OpenGL/Instancing
	//Setting up the instance array
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(offsetArray), offsetArray, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(3, 1);

	return vertexArrayObject;
}

int createTexturedCubeFaceVertexArrayObject(int offset)
{
	vec3 vertexArray[] = {  
		// position and normal
		//cube (-0.5,-0.5,-0.5) to (0.5,0.5,0.5)
		//left
		vec3(-0.5f,-0.5f,-0.5f), vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f,-0.5f, 0.5f), vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f, 0.5f, 0.5f), vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f,-0.5f,-0.5f), vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f, 0.5f, 0.5f), vec3(-1.0f, 0.0f, 0.0f),
		vec3(-0.5f, 0.5f,-0.5f), vec3(-1.0f, 0.0f, 0.0f),
		// far
		vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, -1.0f),
		vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, -1.0f),
		vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, -1.0f),
		vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, -1.0f),
		vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, -1.0f),
		vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, -1.0f),
		// bottom
		vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, -1.0f, 0.0f),
		vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, -1.0f, 0.0f),
		vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, -1.0f, 0.0f),
		vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, -1.0f, 0.0f),
		vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, -1.0f, 0.0f),
		vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, -1.0f, 0.0f),
		// near
		vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),
		// right
		vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
		// top
		vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
		vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 1.0f, 0.0f),
		vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 1.0f, 0.0f),
		vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f)
	};
	const int numVertices = 6;
	const int vecOffset = offset * numVertices;
	vec3 va_pos[6];
	vec2 va_uv[6];
	vec3 va_nor[6];

	for (int i = 0; i < numVertices; i++) {
		va_pos[i] = texturedCubeVertexArray[i + vecOffset].position;
		//va_nor[i] = texturedCubeVertexArray[i + vecOffset].;
		va_uv[i] = texturedCubeVertexArray[i + vecOffset].uv;
		va_nor[i] = vertexArray[(i + vecOffset)*2+1];
	}
	// Create a vertex array
	GLuint vertexArrayObject;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);

	// Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
	//Have multiple arrays/buffer.
	//https://www.khronos.org/opengl/wiki/Tutorial2:_VAOs,_VBOs,_Vertex_and_Fragment_Shaders_(C_/_SDL)
	GLuint vertexBufferObject[4];
	glGenBuffers(4, vertexBufferObject);

	//pos
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(va_pos), va_pos, GL_STATIC_DRAW);
	glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		sizeof(vec3),        // stride - each vertex contains vec3 (position)
		(void*)0             // array buffer offset
	);
	glEnableVertexAttribArray(0);

	//uv
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(va_uv), va_uv, GL_STATIC_DRAW);
	glVertexAttribPointer(2,                            // attribute 2 matches aUV in Vertex Shader
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vec2),
		(void*)0      // uv is offseted by 2 vec3 (comes after position and normal)
	);
	glEnableVertexAttribArray(2);

	//normal
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(va_nor), va_nor, GL_STATIC_DRAW);
	glVertexAttribPointer(4,                   // attribute 1 matches aNormal in Vertex Shader
		3,                   // size
		GL_FLOAT,            // type
		GL_FALSE,            // normalized?
		sizeof(vec3),        // stride - each vertex contains vec3 (position)
		(void*)0  // array buffer offset
	);
	glEnableVertexAttribArray(4);

	//instancing
	vec3 offsetArray[1] = { vec3(0) };
	//https://learnopengl.com/Advanced-OpenGL/Instancing
	//Setting up the instance array
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(offsetArray), offsetArray, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(3, 1);

	return vertexArrayObject;
}
const char* getVertexShaderSource()
{
	return
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;"           //vertex position
		//"layout (location = 1) in vec3 aColor;"         //vertex color    (unused)
		"layout (location = 2) in vec2 aUV;"            //vertex texture location
		"layout (location = 3) in vec3 instanceVec; "   // instancing https://learnopengl.com/Advanced-OpenGL/Instancing
		"layout (location = 4) in vec3 aNormal;"        //vertex normal
		""
		"uniform mat4 worldMatrix;"
		"uniform mat4 viewMatrix = mat4(1.0);"  // default value for view matrix (identity)
		"uniform mat4 projectionMatrix = mat4(1.0);"
		"uniform mat4 lightSpaceMatrix;"                //shadow
		""
		"out vec3 normalVec;"
		"out vec3 fragPos;"
		"out vec2 vertexUV;"
		"out vec4 fragPosLightSpace;"                   //shadow
		"void main()"
		"{"
		"   normalVec = mat3(transpose(inverse(worldMatrix))) * aNormal;"
		"   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
		"   gl_Position = modelViewProjection * vec4(aPos + instanceVec, 1.0);"
		"   vertexUV = aUV;"
		"   fragPos = vec3(worldMatrix * vec4(aPos + instanceVec, 1.0));"
		//fragPos from light's view
		"   fragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);"
		"}";
}
const char* getFragmentShaderSource()
{
	return
		"#version 330 core\n"
		"uniform vec3 objectColor;"     //object inherent color
		"uniform vec3 lightPos;"        //light properties
		"uniform vec3 viewPos;"
		"uniform sampler2D textureSampler;" //texture properties
		"uniform bool hasTexture;"
		"uniform sampler2D shadowMap;"  //shadow
		"uniform bool hasShadow;"

		"vec3 lightColor = vec3(1.0, 1.0, 1.0);"    //light property
		"vec4 textureColor;"                        //texture property

		"in vec3 fragPos;"
		"in vec3 normalVec;"    //light properties
		"in vec2 vertexUV;"     //texture properties
		"in vec4 fragPosLightSpace;"    //shadow

		"out vec4 FragColor;"   //output

		"float ShadowCalculation(vec4 fragPosLightSpace)"
		"{"
		"    float shadow = 0.0f;"
		//manual perspective divide (for perspective projection)
		"    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;"
		//range from [-1, 1] to [0, 1].
		"    projCoords = projCoords * 0.5 + 0.5;"
		// keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
		//or if toggled off.
		"    if (!hasShadow || projCoords.z > 1.0){"
		"       shadow = 0.0f;"
		"    }"
		"    else {"
		//closest depth value
		"       float closestDepth = texture(shadowMap, projCoords.xy).r;"
		//current depth value
		"       float currentDepth = projCoords.z;"
		//is in shadow if not the closest depth.
	//"    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;"
	"float bias = 0.005;"
	// calculate bias (based on depth map resolution and slope)
		"       vec3 normal = normalize(normalVec);"
		"       vec3 lightDir = normalize(lightPos - fragPos);"
		//"       float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);"
		//"       shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;"
		"       shadow = 0.0f;"
		//Percentage close filter
		"       vec2 texelSize = 1.0 / textureSize(shadowMap, 0);"
		"       for (int x = -1; x <= 1; ++x)"
		"       {"
		"           for (int y = -1; y <= 1; ++y)"
		"           {"
		"               float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;"
		"               shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;"
		"           }"
		"       }"
		"       shadow /= 9.0;"
		""
		"    }"
		"    return shadow;"
		"}"
		"void main()"
		"{"
		//Texture
		//assign texture color
		"   if (hasTexture){"
		"      textureColor = texture( textureSampler, vertexUV );"
		"   }"
		//if it shouldn't have a texture, do not modify its color
		"   else {"
		"	   textureColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);"
		"   }"

		//Phong model lighting
		// Ambient
		"   float ambientStrength = 0.4;"
		"   vec3 ambient = ambientStrength * lightColor;"
		// Diffuse
		"   vec3 norm = normalize(normalVec);"
		"   vec3 lightDir = normalize(lightPos - fragPos);"
		"   float diff = max(dot(norm, lightDir), 0.0);"
		"   vec3 diffuse = diff * lightColor;"
		// Specular
		"   float specularStrength = 0.5;"
		"   vec3 viewDir = normalize(viewPos - fragPos);"
		"   vec3 reflectDir = reflect(-lightDir, norm);"
		"   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);"
		"   vec3 specular = specularStrength * spec * lightColor;"
		// Final color output from phong model + shadow
		"   float shadow = ShadowCalculation(fragPosLightSpace);"
		//"   float shadow = 0;"
		"   vec4 result = vec4( (ambient + (1-shadow) * (diffuse + specular)) * objectColor, 1.0f);"
		//"   vec4 result = vec4(  (ambient + (1-shadow) *(diffuse + specular)) * objectColor + shadow * vec3(1.0f, 0.0f,0.0f), 1.0f);"
		//"   vec4 result = vec4((ambient + diffuse + specular) * objectColor, 1.0f);"
		"   FragColor = result * textureColor;"
		"}";
}

int compileAndLinkShaders()
{
	// compile and link shader program
	// return shader program id
	// ------------------------------------

	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderSource = getVertexShaderSource();
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderSource = getFragmentShaderSource();
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

//https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
int createPlaneVertexArrayObject()
{
	// meshes
	unsigned int planeVAO;
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float planeVertices[] = {
		// positions            // normals         // texcoords
		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
		-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,

		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 10.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f
	};
	//for (int i = 0; i < 8 * 6; i++) {
	//    planeVertices[i] /= 2;
	//    switch (i % 8) {
	//    case 1:
	//        planeVertices[i] = 5.0f;
	//        break;
	//    }
	//}


	// plane VAO
	unsigned int planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);


	return planeVAO;
}
//https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
const char* getVertexShaderSourceTest()
{
	return
		"    #version 330 core\n"
		"    layout(location = 0) in vec3 aPos;"
		"layout(location = 1) in vec2 aTexCoords;"
		""
		"out vec2 TexCoords;"
		""
		"void main()"
		"{"
		"    TexCoords = aTexCoords;"
		"    gl_Position = vec4(aPos, 1.0);"
		"}";
}
//https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
const char* getFragmentShaderSourceTest()
{
	return
		"    #version 330 core\n"
		"    out vec4 FragColor;"
		""
		"in vec2 TexCoords;"
		""
		"uniform sampler2D depthMap;"
		"uniform float near_plane;"
		"uniform float far_plane;"
		""
		""// required when using a perspective projection matrix
		"float LinearizeDepth(float depth)"
		"{"
		"    float z = depth * 2.0 - 1.0;" // Back to NDC 
		"    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));"
		"}"
		""
		"void main()"
		"{"
		"    float depthValue = texture(depthMap, TexCoords).r;"
		"    FragColor = vec4(vec3(LinearizeDepth(depthValue) / far_plane), 1.0);" // perspective
		//"    FragColor = vec4(vec3(depthValue), 1.0);" // orthographic
		"}";
}
int compileAndLinkShadersTest()
{
	// compile and link shader program
	// return shader program id
	// ------------------------------------

	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderSource = getVertexShaderSourceTest();
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderSource = getFragmentShaderSourceTest();
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}
//https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
const char* getVertexShaderSourceTest2()
{
	return
		"    #version 330 core\n"
		"    layout(location = 0) in vec3 aPos;"
		"layout(location = 1) in vec3 aNormal;"
		"layout(location = 2) in vec2 aTexCoords;"
		""
		"out vec2 TexCoords;"
		""
		"out VS_OUT{"
		"    vec3 FragPos;"
		"    vec3 Normal;"
		"    vec2 TexCoords;"
		"    vec4 FragPosLightSpace;"
		"} vs_out;"
		""
		"uniform mat4 projection;"
		"uniform mat4 view;"
		"uniform mat4 model;"
		"uniform mat4 lightSpaceMatrix;"
		""
		"void main()"
		"{"
		"    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));"
		"    vs_out.Normal = transpose(inverse(mat3(model))) * aNormal;"
		"    vs_out.TexCoords = aTexCoords;"
		"    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);"
		"    gl_Position = projection * view * model * vec4(aPos, 1.0);"
		"}";
}
//https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
const char* getFragmentShaderSourceTest2()
{
	return
		"    #version 330 core\n"
		"    out vec4 FragColor;"
		""
		"in VS_OUT{"
		"    vec3 FragPos;"
		"    vec3 Normal;"
		"    vec2 TexCoords;"
		"    vec4 FragPosLightSpace;"
		"} fs_in;"
		""
		"uniform sampler2D diffuseTexture;"
		"uniform sampler2D shadowMap;"
		""
		"uniform vec3 lightPos;"
		"uniform vec3 viewPos;"
		""
		"float ShadowCalculation(vec4 fragPosLightSpace)"
		"{"
		//"    // perform perspective divide"
		"    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;"
		//"    // transform to [0,1] range"
		"    projCoords = projCoords * 0.5 + 0.5;"
		//"    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)"
		"    float closestDepth = texture(shadowMap, projCoords.xy).r;"
		//"    // get depth of current fragment from light's perspective"
		"    float currentDepth = projCoords.z;"
		//"    // check whether current frag pos is in shadow"
		//"    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;"
		//"float bias = 0.005;"
		// calculate bias (based on depth map resolution and slope)
		"    vec3 normal = normalize(fs_in.Normal);"
		"    vec3 lightDir = normalize(lightPos - fs_in.FragPos);"
		"    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);"
		//"    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;"
		//Percentage close filter
		"    float shadow = 0.0f;"
		"    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);"
		"    for (int x = -1; x <= 1; ++x)"
		"    {"
		"        for (int y = -1; y <= 1; ++y)"
		"        {"
		"            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;"
		"            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;"
		"        }"
		"    }"
		"    shadow /= 9.0;"
		""
		// keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
		"    if (projCoords.z > 1.0)"
		"        shadow = 0.0;"
		""
		"    return shadow;"
		"}"
		""
		"void main()"
		"{"
		"    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;"
		"    vec3 normal = normalize(fs_in.Normal);"
		"    vec3 lightColor = vec3(0.3);"
		//"    // ambient"
		"    vec3 ambient = 0.3 * color;"
		//"    // diffuse"
		"    vec3 lightDir = normalize(lightPos - fs_in.FragPos);"
		"    float diff = max(dot(lightDir, normal), 0.0);"
		"    vec3 diffuse = diff * lightColor;"
		//"    // specular"
		"    vec3 viewDir = normalize(viewPos - fs_in.FragPos);"
		"    vec3 reflectDir = reflect(-lightDir, normal);"
		"    float spec = 0.0;"
		"    vec3 halfwayDir = normalize(lightDir + viewDir);"
		"    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);"
		"    vec3 specular = spec * lightColor;"
		//"    // calculate shadow"
		"    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);"
		//"    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;"
		"   vec3 lighting = vec3( (ambient + (1-shadow) * (diffuse + specular)) * color + shadow * vec3(1.0f, 0.0f,0.0f));"
		""
		"    FragColor = vec4(lighting, 1.0);"
		"}";
}
int compileAndLinkShadersTest2()
{
	// compile and link shader program
	// return shader program id
	// ------------------------------------

	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderSource = getVertexShaderSourceTest2();
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderSource = getFragmentShaderSourceTest2();
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}
//https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
const char* getVertexShaderSourceShadow()
{
	return
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPos;"           //vertex position
		""
		"uniform mat4 lightSpaceMatrix;"
		"uniform mat4 model;"
		"void main()"
		"{"
		"   gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0);"
		"}";
}
//https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
const char* getFragmentShaderSourceShadow()
{
	return
		"#version 330 core\n"
		"void main()"
		"{"
		"   gl_FragDepth = gl_FragCoord.z;"
		"}";
}
int compileAndLinkShadersShadow()
{
	// compile and link shader program
	// return shader program id
	// ------------------------------------

	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderSource = getVertexShaderSourceShadow();
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderSource = getFragmentShaderSourceShadow();
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

//https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
vector<int> configureDepthMap() {
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	vector<int> depth(2);
	depth[0] = depthMapFBO;
	depth[1] = depthMap;
	return depth;
}
//https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
	// initialize (if necessary)
	if (cubeVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

//float fakeTime = 0;
//https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
// renders the 3D scene
// --------------------
void renderScene(int shaderProgram, int planeVAO)
{
	// floor
	glm::mat4 model = glm::mat4(1.0f);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	// cubes
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
	renderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
	model = glm::scale(model, glm::vec3(0.5f));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
	renderCube();
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.25));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
	renderCube();

	//fakeTime += 0.01f;
	//float fakeCycle = 3.5f*sinf(fakeTime * 0.7f)* sinf(fakeTime * 0.7f);
	//model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(2.0f, -1.5f, 2.0));
	//model = glm::rotate(model, radians(45.0f + fakeTime*20),glm::vec3(2.0f, -1.5f + fakeCycle, 2.0));
	//model = glm::scale(model, glm::vec3(1.1f + fakeCycle));
	//glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
	//renderCube();
}
//https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		for (int i = 0; i < 4 * 5; i++) {
			switch (i % 5) {
			case 0:
			case 1:
			case 2:
				quadVertices[i] /= 2;
				break;
			}
		}

		//vec3 quadVertices[] = {
		//    // positions        // texture Coords
		//    0.5f*vec3(-1.0f,  1.0f, 0.0f), vec3(0.0f, 1.0f,0.0f),
		//    0.5f * vec3(-1.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f,0.0f),
		//     0.5f * vec3(1.0f,  1.0f, 0.0f), vec3(1.0f, 1.0f,0.0f),
		//     0.5f * vec3(1.0f, -1.0f, 0.0f), vec3(1.0f, 0.0f,0.0f)
		//};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), (void*)0);
		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(vec3), (void*)(sizeof(vec3)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

GLuint loadTexture(const char* filename)
{
	// Step1 Create and bind textures
	GLuint textureId = 0;
	glGenTextures(1, &textureId);
	assert(textureId != 0);


	glBindTexture(GL_TEXTURE_2D, textureId);

	// Step2 Set filter parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Step3 Load Textures with dimension data
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (!data)
	{
		std::cerr << "Error::Texture could not load texture file:" << filename << std::endl;
		return 0;
	}

	// Step4 Upload the texture to the PU
	GLenum format = 0;
	if (nrChannels == 1)
		format = GL_RED;
	else if (nrChannels == 3)
		format = GL_RGB;
	else if (nrChannels == 4)
		format = GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height,
		0, format, GL_UNSIGNED_BYTE, data);

	// Step5 Free resources
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureId;
}

//Input sideLength is length of side of grid by cell count. Default 100.
//Input cellLength is length of side of single cell. Default 1.
//Input worldMatrixLocation is location of worldMatrix.
//
//Function will create a grid centered at (0,Y,0) in (X,Y,Z).
//Note: location of line object inside vertex array is hard-coded.
void drawGrid(GLuint worldMatrixLocation, GLuint colorLocation, mat4 relativeWorldMatrix = mat4(1.0f)) {
	const float sideLength = 100; //# of cells on side.
	const float cellLength = 1; //length of side of a cell.

	//make the grid pop-out a bit from the tile grid
	const float height = 0.03f;     //y-position of grid.

	mat4 lineWorldMatrix;
	mat4 worldMatrix;
	for (int i = 0; i <= sideLength; ++i)
	{
		//Draw first set.
		lineWorldMatrix =
			//spaced interval along x-axis.
			translate(mat4(1.0f),
				vec3(-cellLength * sideLength / 2 + i * cellLength,
					height,
					0))
			//scaled to side length.
			* scale(mat4(1.0f),
				vec3(1.0f, 1.0f, cellLength * sideLength));
		//tramsformation to grid as a whole
		worldMatrix = relativeWorldMatrix * lineWorldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		//glUniform3f(colorLocation, 1.0f, 1.0f, 1.0f);
		glDrawArrays(GL_LINES, 36, 2);

		//Draw perpendicular set.
		lineWorldMatrix =
			//spaced interval along z-axis.
			translate(mat4(1.0f),
				vec3(0,
					height,
					-cellLength * sideLength / 2 + i * cellLength))
			//rotate 90 degrees.
			* rotate(mat4(1.0f),
				radians(90.0f),
				vec3(0.0f, 1.0f, 0.0f))
			//scaled to side length.
			* scale(mat4(1.0f),
				vec3(1.0f, 1.0f, cellLength * sideLength));
		worldMatrix = relativeWorldMatrix * lineWorldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		//glUniform3f(colorLocation, 1.0f, 1.0f, 1.0f);
		glDrawArrays(GL_LINES, 36, 2);
	}
}

//draws ground with tiles arranged as a grid.
void drawTileGrid(GLuint worldMatrixLocation, mat4 relativeWorldMatrix = mat4(1.0f)) {
	const float sideLength = 100; //# of cells on side.
	const float cellLength = 1; //length of side of a cell.
	const int numCells = (int)pow(sideLength, 2);
	const float height = 0.0f;     //y-position of grid.

	mat4 tileWorldMatrix;
	mat4 worldMatrix;
	//set the first tile to draw as the one in (+x, +z) corner.
	tileWorldMatrix =
		//spaced interval along xz-plane.
		translate(mat4(1.0f),
			vec3(cellLength * (sideLength - 1) / 2,
				height,
				cellLength * (sideLength - 1) / 2))
		//scale to match length.
		* scale(mat4(1.0f),
			vec3(cellLength, 1.0f, cellLength));;
	worldMatrix = relativeWorldMatrix * tileWorldMatrix;
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
	//Call to use instances
	//https://learnopengl.com/Advanced-OpenGL/Instancing
	glDrawArraysInstanced(GL_TRIANGLES, 38, 6, numCells);
}

// Draws x, y, z axis at the center of the grid
// Input is location of worldMatrix and location of colorLocation
void drawAxis(GLuint worldMatrixLocation, GLuint colorLocation) {
	const float height = 0, cellLength = 1; // values from drawGrid
	mat4 lineWorldMatrix;

	// Draw x-axis (tranformed red cube)
	lineWorldMatrix = translate(mat4(1.0f), vec3(cellLength * 2.5f, cellLength * height, 0.0f)) * scale(mat4(1.0f), vec3(cellLength * 5.0f, cellLength * 0.05f, cellLength * 0.05f));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &lineWorldMatrix[0][0]);
	glUniform3f(colorLocation, 1.0f, 0.0f, 0.0f);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// Draw y-axis (tranformed green cube)
	lineWorldMatrix = translate(mat4(1.0f), vec3(0.0f, cellLength * (2.5f + height), 0.0f)) * scale(mat4(1.0f), vec3(cellLength * 0.05f, cellLength * 5.0f, cellLength * 0.05f));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &lineWorldMatrix[0][0]);
	glUniform3f(colorLocation, 0.0f, 1.0f, 0.0f);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	// Draw z-axis (transformed blue cube)
	lineWorldMatrix = translate(mat4(1.0f), vec3(0.0f, cellLength * height, cellLength * 2.5f)) * scale(mat4(1.0f), vec3(cellLength * 0.05f, cellLength * 0.05f, cellLength * 5.0f));
	glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &lineWorldMatrix[0][0]);
	glUniform3f(colorLocation, 0.0f, 0.0f, 1.0f);
	glDrawArrays(GL_TRIANGLES, 0, 36);

}

bool isShiftPressed(GLFWwindow* window) {
	return glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
}

bool checkModelMovement(GLFWwindow* window, map<int, KeyState> previousKeyStates) {
	vector<GLuint> inputs;
	vector<GLuint>::iterator itr;

	// capital case letters only
	if (isShiftPressed(window))
	{
		//translate model movement
		//vertical movement
		inputs.push_back(GLFW_KEY_W);
		inputs.push_back(GLFW_KEY_S);

		//horizontal movement
		inputs.push_back(GLFW_KEY_D);
		inputs.push_back(GLFW_KEY_A);
	}
	//lower case only
	else {
		//none for now
	}
	//lower or upper case allowed.
	{
		//none for now
	}

	//iterate through all keys that could be pressed.
	for (itr = inputs.begin(); itr != inputs.end(); ++itr) {
		////////////////
		//get previous key state if tracked, otherwise default release (true).
		//https://stackoverflow.com/questions/4527686/how-to-update-stdmap-after-using-the-find-method
		//default released if not tracked
		int previousState = GLFW_RELEASE;
		map<int, KeyState>::iterator it = previousKeyStates.find(*itr);
		if (it != previousKeyStates.end()) {        //key is a tracked one
			//toggling shift should not activate the key again.
			//So as long the shift was pressed at least once while holding the key, toggling the shift button further will not release the key.
			if ((it->second.needShiftPressed == isShiftPressed(window)) && it->second.prevWithShiftPressed) {
				previousState = GLFW_PRESS;    //so update to actual
				//glClearColor(0.4f, 0.3f, 0.0f, 1.0f);
			}
			else {
				//otherwise, update to actual previous key.
				previousState = it->second.keyState;
				//glClearColor(1.0f, 0.3f, 0.0f, 1.0f);
			}

		}
		////////////////

		//enter block if valid key is pressed for movement.
		if (glfwGetKey(window, *itr) == GLFW_PRESS && previousState == GLFW_RELEASE)
		{
			//key press detected
			return true;
		}
	}
	//no key press detected
	return false;
}

//function will switch the selected draw mode on correct key presses.
//note: undefined priority in case multiple correct keys are pressed (but will select a draw mode).
void drawControl(GLFWwindow* window) {
	map<int, GLenum> inputsToDrawModes;
	map<int, GLenum>::iterator itr;
	inputsToDrawModes.insert(pair<int, GLenum>(GLFW_KEY_P, GL_POINT));
	inputsToDrawModes.insert(pair<int, GLenum>(GLFW_KEY_L, GL_LINE));
	inputsToDrawModes.insert(pair<int, GLenum>(GLFW_KEY_T, GL_FILL));

	//default return value;
	if (isShiftPressed(window)) // capital letters only
	{
		for (itr = inputsToDrawModes.begin(); itr != inputsToDrawModes.end(); ++itr) {
			if (glfwGetKey(window, itr->first) == GLFW_PRESS) // draw mode
			{
				//JASON BECCHERINI
				// Keybinds for selecting rendering mode (affects axis and models)
				glPolygonMode(GL_FRONT_AND_BACK, itr->second);
			}
		}
	}
}

//function will return a model index on correct key presses.
//note: undefined priority in case multiple correct keys are pressed (but will select a model).
int selectModelControl(GLFWwindow* window, int previousModelIndex) {
	map<int, int> inputsToModelIndex;
	map<int, int>::iterator itr;
	inputsToModelIndex.insert(pair<int, GLchar>(GLFW_KEY_1, 0));
	inputsToModelIndex.insert(pair<int, GLchar>(GLFW_KEY_2, 1));
	inputsToModelIndex.insert(pair<int, GLchar>(GLFW_KEY_3, 2));
	inputsToModelIndex.insert(pair<int, GLchar>(GLFW_KEY_4, 3));
	inputsToModelIndex.insert(pair<int, GLchar>(GLFW_KEY_5, 4));

	//default return value
	GLchar selectedMode = previousModelIndex;

	int selectedModelIndex = previousModelIndex;
	for (itr = inputsToModelIndex.begin(); itr != inputsToModelIndex.end(); ++itr) {
		if (glfwGetKey(window, itr->first) == GLFW_PRESS) // select model
		{
			selectedModelIndex = itr->second;
		}
	}
	return selectedModelIndex;
}

//function will return a matrix for corresponding transformation of inputted keys.
//note: undefined order priority in case multiple correct keys are pressed (but will select a matrix).
mat4* modelControl(GLFWwindow* window, float dt, map<int, KeyState> previousKeyStates) {
	mat4* selectedTransformation = new mat4[3];
	//default return values
	selectedTransformation[0] = mat4(1.0f);     //translate
	selectedTransformation[1] = mat4(1.0f);     //rotate
	selectedTransformation[2] = mat4(1.0f);     //scale

	//allows map value to contain 2 variables.
	struct Transformation {
		mat4 matrix;
		int type;
		//type, same type should be communitative.
		//0 : translate
		//1 : rotate
		//2 : scale
	};
	map<int, Transformation> inputsToModelMatrix;
	map<int, Transformation>::iterator itr;
	const float transformSpeed = 6 * dt;
	const float translateSpeed = transformSpeed;
	const float rotateSpeed = 5.0f;   //specifications
	const float scaleSpeed = transformSpeed / 12;
	//TODO: code shearing into separate movement function (since the shearing itself is not in the user's control, only the movement)
	const float shearSpeed = transformSpeed * 3;        //temporary, will move to another method for movement
	// capital case letters only
	if (isShiftPressed(window))
	{
		//translate model if pressed
		//vertical movement
		inputsToModelMatrix.insert(pair<int, Transformation>(GLFW_KEY_W, {
			translate(mat4(1.0f),
				vec3(0,
					0,
					-translateSpeed)), 0 }));
		inputsToModelMatrix.insert(pair<int, Transformation>(GLFW_KEY_S, {
			translate(mat4(1.0f),
				vec3(0,
					0,
					translateSpeed)), 0 }));
		//horizontal movement
		inputsToModelMatrix.insert(pair<int, Transformation>(GLFW_KEY_D, {
			translate(mat4(1.0f),
				vec3(translateSpeed,
					0,
					0)), 0 }));
		inputsToModelMatrix.insert(pair<int, Transformation>(GLFW_KEY_A, {
			translate(mat4(1.0f),
				vec3(-translateSpeed,
					0,
					0)), 0 }));

		//scale model if pressed
		inputsToModelMatrix.insert(pair<int, Transformation>(GLFW_KEY_U, {
			scale(mat4(1.0f),
			vec3((1 + scaleSpeed), (1 + scaleSpeed), (1 + scaleSpeed))), 2 }));
		inputsToModelMatrix.insert(pair<int, Transformation>(GLFW_KEY_J, {
			scale(mat4(1.0f),
			vec3(1.0f / (1 + scaleSpeed), 1.0f / (1 + scaleSpeed), 1.0f / (1 + scaleSpeed))), 2 }));
	}
	//lower case only
	else {
		//rotate model if pressed
		inputsToModelMatrix.insert(pair<int, Transformation>(GLFW_KEY_A, {
			rotate(mat4(1.0f),
			radians(rotateSpeed),
			vec3(0.0f, 1.0f, 0.0f)), 1 }));
		inputsToModelMatrix.insert(pair<int, Transformation>(GLFW_KEY_D, {
			rotate(mat4(1.0f),
			radians(rotateSpeed),
			vec3(0.0f, -1.0f, 0.0f)), 1 }));
	}
	//lower or upper case allowed.
	{
		//shear model if pressed
		//temporary, will move to another method for movement
		//TODO: code shearing into separate movement function (since the shearing itself is not in the user's control, only the movement)
		inputsToModelMatrix.insert(pair<int, Transformation>(GLFW_KEY_Q, {
			mat4(1, 0, 0, 0,  // first column
			0, 1, 0, 0,  // second column
			shearSpeed, 0, 1, 0,  // third column
			0, 0, 0, 1) // fourth column
			, 2 }));
		inputsToModelMatrix.insert(pair<int, Transformation>(GLFW_KEY_E, {
			mat4(1, 0, 0, 0,  // first column
			0, 1, 0, 0,  // second column
			-shearSpeed, 0, 1, 0,  // third column
			0, 0, 0, 1) // fourth column
			, 2 }));
	}

	//iterate through all keys that could be pressed.
	for (itr = inputsToModelMatrix.begin(); itr != inputsToModelMatrix.end(); ++itr) {
		////////////////
		//get previous key state if tracked, otherwise default release (true).
		//https://stackoverflow.com/questions/4527686/how-to-update-stdmap-after-using-the-find-method
		//default released if not tracked
		int previousState = GLFW_RELEASE;
		map<int, KeyState>::iterator it = previousKeyStates.find(itr->first);
		if (it != previousKeyStates.end()) {        //key is a tracked one
			//toggling shift should not activate the key again.
			//So as long the shift was pressed at least once while holding the key, toggling the shift button further will not release the key.
			if ((it->second.needShiftPressed == isShiftPressed(window)) && it->second.prevWithShiftPressed) {
				previousState = GLFW_PRESS;    //so update to actual
				//glClearColor(0.4f, 0.3f, 0.0f, 1.0f);
			}
			//else if (isShiftPressed(window) && !it->second.prevWithShiftPressed) {
			//    previousState = GLFW_PRESS;
			//    glClearColor(0.4f, 1, 0.0f, 1.0f);
			//} 
			else {
				//otherwise, update to actual previous key.
				previousState = it->second.keyState;
				//glClearColor(1.0f, 0.3f, 0.0f, 1.0f);
			}

		}
		////////////////

		if (glfwGetKey(window, itr->first) == GLFW_PRESS && previousState == GLFW_RELEASE) // select model. Apply once for keys that are tracked.
		{
			//group up the applied transformations.
			selectedTransformation[itr->second.type] = itr->second.matrix * selectedTransformation[itr->second.type];
		}
	}
	return selectedTransformation;
}
//retrieve bool on whether a setting should be applied or not, based on key press.
bool* customControl(GLFWwindow* window, map<int, KeyState> previousKeyStates) {
	struct Key {
		GLuint key;     //key
		bool shiftKey;  //whether shift should be pressed along with key

		Key(GLuint k, bool sk = false) {
			key = k;
			shiftKey = sk;
		}
	};

	//can modify this area
	//array of keys tied to settings.
	const Key keyControl[] = {
		//{KEY, shift requirement(default false)}
		//signal to reset position and orientation
		{GLFW_KEY_HOME},
		{GLFW_KEY_HOME, true},
		//signal to toggle textures
		{GLFW_KEY_X, true},
		//signal to toggle shadows
		{GLFW_KEY_B, true}
	};

	//all below is automatic
	//consider if shift is pressed (referred to multiple times, so store it in a variable).
	const bool shiftPressed = isShiftPressed(window);
	//number of keys tied to settings.
	const int numKeys = sizeof(keyControl) / sizeof(keyControl[0]);

	bool* selectedSetting = new bool[numKeys];
	//initialize values as false.
	for (int i = 0; i < numKeys; i++) {
		selectedSetting[i] = false;
	}

	//for every key tied to a setting, check if it is pressed.
	for (int i = 0; i < numKeys; i++) {
		////////////////
		//get previous key state if tracked, otherwise default release (true).
		//https://stackoverflow.com/questions/4527686/how-to-update-stdmap-after-using-the-find-method
		//default released if not tracked
		int previousState = GLFW_RELEASE;
		map<int, KeyState>::iterator it = previousKeyStates.find(keyControl[i].key);
		if (it != previousKeyStates.end()) {        //key is a tracked one
			//toggling shift should not activate the key again.
			//So as long the shift was pressed at least once while holding the key, toggling the shift button further will not release the key.
			if (it->second.needShiftPressed == shiftPressed && it->second.prevWithShiftPressed) {
				previousState = GLFW_PRESS;    //so update to actual
				//glClearColor(0.4f, 0.3f, 0.0f, 1.0f);
			}
			else {
				//otherwise, update to actual previous key.
				previousState = it->second.keyState;
				//glClearColor(1.0f, 0.3f, 0.0f, 1.0f);
			}
		}
		////////////////
		if (glfwGetKey(window, keyControl[i].key) == GLFW_PRESS
			&& previousState == GLFW_RELEASE
			&& keyControl[i].shiftKey == shiftPressed) // key action. Apply once for keys that are tracked.
		{
			selectedSetting[i] = true;
		}
	}
	return selectedSetting;
}

// Function to randomly position a selected model on spacebar input
void randomPosModel(CharModel* selectedModel) {
	// Generate random x and z coordinates inside the grid (int values)
	int randomPosX = rand() % 91 - 45; // -45 to 45 (not 50 for aesthetic purposes)
	int randomPosZ = rand() % 91 - 45; // -45 to 45

	// Change relative translate matrix of selected model
	mat4 randomRelativeTranslateMatrix = translate(mat4(1.0f), vec3((float)randomPosX, selectedModel->getInitY(), (float)randomPosZ));
	selectedModel->setRelativeTranslateMatrix(randomRelativeTranslateMatrix);
}

//individual fields are id of textures.
struct TextureId {
	int depthMap;
	int tiledTextureID;
	int boxTextureID;
	int metalTextureID;
	int brickTextureID;
	int cloverTextureID;
	int jaguarTextureID;
	int woodTextureID;

	//etc
};
//has info on all settings to render.
struct RenderInfo {
	int shaderProgram;
	GLuint colorLocation;
	GLuint enableTextureLocation;
	GLuint enableShadowLocation;
	GLuint worldMatrixLocation;

	int cubeVAOa;
	int cubeVAOFront;
	int cubeVAOLeft;
	int cubeVAORight;
	int cubeVAOTop;
	int cubeVAOBottom;
	int cubeVAOBack;


	int enableTexture;
	int enableShadow;
	TextureId textures;

	//etc
};

void renderDecor(RenderInfo renderInfo) {
	int shaderProgram = renderInfo.shaderProgram;
	GLuint colorLocation = renderInfo.colorLocation;
	GLuint enableTextureLocation = renderInfo.enableTextureLocation;
	GLuint enableShadowLocation = renderInfo.enableShadowLocation;
	GLuint worldMatrixLocation = renderInfo.worldMatrixLocation;
	int enableTexture = renderInfo.enableTexture;
	int enableShadow = renderInfo.enableShadow;
	int tiledTextureID = renderInfo.textures.tiledTextureID;

	int cubeVAOa = renderInfo.cubeVAOa;
	int cubeVAOFront = renderInfo.cubeVAOFront;
	int cubeVAOLeft = renderInfo.cubeVAOLeft;
	int cubeVAORight = renderInfo.cubeVAORight;
	int cubeVAOBack = renderInfo.cubeVAOBack;
	int cubeVAOTop = renderInfo.cubeVAOTop;
	int cubeVAOBottom = renderInfo.cubeVAOBottom;


	// Draw ground
	glUniform1i(enableShadowLocation, enableShadow);
	//draw grid
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform3f(colorLocation, 1.0f, 1.0f, 1.0f);
	glUniform1i(enableTextureLocation, 0);
	drawGrid(worldMatrixLocation, colorLocation, mat4(1.0f));
	// draw axis
	drawAxis(worldMatrixLocation, colorLocation);
	glBindVertexArray(cubeVAOa);
	glUniform1i(enableTextureLocation, enableTexture);

	//draw tiles
	glBindTexture(GL_TEXTURE_2D, tiledTextureID);
	glUniform3f(colorLocation, 0.8f, 0.4f, 0.8f);
	drawTileGrid(worldMatrixLocation, mat4(1.0f));
}

void renderModels(RenderInfo renderInfo, CharModel* models[numMainModels]) {
	int shaderProgram = renderInfo.shaderProgram;
	GLuint colorLocation = renderInfo.colorLocation;
	GLuint enableTextureLocation = renderInfo.enableTextureLocation;
	GLuint worldMatrixLocation = renderInfo.worldMatrixLocation;
	int enableTexture = renderInfo.enableTexture;
	int boxTextureID = renderInfo.textures.boxTextureID;
	int tiledTextureID = renderInfo.textures.tiledTextureID;
	int metalTextureID = renderInfo.textures.metalTextureID;
	int brickTextureID = renderInfo.textures.brickTextureID;
	int cloverTextureID = renderInfo.textures.cloverTextureID;
	int jaguarTextureID = renderInfo.textures.jaguarTextureID;
	int woodTextureID = renderInfo.textures.woodTextureID;

	int cubeVAOa = renderInfo.cubeVAOa;
	int cubeVAOFront = renderInfo.cubeVAOFront;
	int cubeVAOLeft = renderInfo.cubeVAOLeft;
	int cubeVAORight = renderInfo.cubeVAORight;
	int cubeVAOBack = renderInfo.cubeVAOBack;
	int cubeVAOTop = renderInfo.cubeVAOTop;
	int cubeVAOBottom = renderInfo.cubeVAOBottom;

	//draw all models
	GLuint temp = CharModel::swapWorldMatrixLocation(models, worldMatrixLocation);
	//glUniform3f(colorLocation, 1.0f, 233.0f / 255.0f, 0.0f);
	glUniform3f(colorLocation, 1.0f, 1.0f, 1.0f);

	//front
	glBindVertexArray(cubeVAOFront);
	glBindTexture(GL_TEXTURE_2D, brickTextureID);
	CharModel::draw(models);
	//back
	glBindVertexArray(cubeVAOBack);
	glBindTexture(GL_TEXTURE_2D, tiledTextureID);
	CharModel::draw(models);
	//left
	glBindVertexArray(cubeVAOLeft);
	glBindTexture(GL_TEXTURE_2D, cloverTextureID);
	CharModel::draw(models);
	//right
	glBindVertexArray(cubeVAORight);
	glBindTexture(GL_TEXTURE_2D, jaguarTextureID);
	CharModel::draw(models);
	//top
	glBindVertexArray(cubeVAOTop);
	glBindTexture(GL_TEXTURE_2D, boxTextureID);
	CharModel::draw(models);
	//bot
	glBindVertexArray(cubeVAOBottom);
	glBindTexture(GL_TEXTURE_2D, woodTextureID);
	CharModel::draw(models);

	glBindVertexArray(cubeVAOa);
	//number
	glBindTexture(GL_TEXTURE_2D, metalTextureID);
	CharModel::drawNumber(models);
	//Sphere has no texture for now.
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(enableTextureLocation, 0);
	CharModel::drawSphere(models);
	glUniform1i(enableTextureLocation, enableTexture);

	//swap back
	CharModel::swapWorldMatrixLocation(models, temp);
}

int main(int argc, char* argv[])
{
	// Initialize GLFW and OpenGL version
	glfwInit();

#if defined(PLATFORM_OSX)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
	// On windows, we set OpenGL version to 3.1.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif

	// Create Window and rendering context using GLFW, resolution is 800x600
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "COMP 371 - A2 - Team 4", NULL, NULL);
	if (window == NULL)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// @TODO 3 - Disable mouse cursor
	// ...
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to create GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Register callback functions
	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetKeyCallback(window, key_callback);


	// Load Textures
#if defined(PLATFORM_OSX)
	GLuint tiledTextureID = loadTexture("Textures/brick2.jpg");
	GLuint cementTextureID = loadTexture("Textures/cement.jpg");
	GLuint metalTextureID = loadTexture("Textures/metal.jpg");
	GLuint boxTextureID = loadTexture("Textures/box.jpg");
	GLuint cloverTextureID = loadTexture("Textures/clover.jpg");
	GLuint jaguarTextureID = loadTexture("Textures/jaguar.jpg");
	GLuint woodTextureID = loadTexture("Textures/wood.jpg");


#else
	//GLuint tiledTextureID = loadTexture("../Assets/Textures/brick.jpg");
	//GLuint cementTextureID = loadTexture("../Assets/Textures/cement.jpg");
	//GLuint metalTextureID = loadTexture("../Assets/Textures/metal.jpg");
	//GLuint boxTextureID = loadTexture("../Assets/Textures/box.jpg");

	GLuint brickTextureID = loadTexture("../Assets/Textures/brick.jpg");
	GLuint tiledTextureID = loadTexture("../Assets/Textures/tiled.jpg");
	GLuint cementTextureID = loadTexture("../Assets/Textures/cement.jpg");
	GLuint metalTextureID = loadTexture("../Assets/Textures/metal.jpg");
	GLuint boxTextureID = loadTexture("../Assets/Textures/box.jpg");
	GLuint cloverTextureID = loadTexture("../Assets/Textures/clover.jpg");
	GLuint jaguarTextureID = loadTexture("../Assets/Textures/jaguar.jpg");
	GLuint woodTextureID = loadTexture("../Assets/Textures/wood.jpg");


#endif
	// Grey background
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);

	// Compile and link shaders here ...
	int shaderProgram = compileAndLinkShaders();
	int shaderProgramTest = compileAndLinkShadersTest();
	int shaderProgramTest2 = compileAndLinkShadersTest2();
	int shaderProgramShadow = compileAndLinkShadersShadow();

	// We can set the shader configurations
	//https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
	glUseProgram(shaderProgram);
	glUniform1i(glGetUniformLocation(shaderProgram, "shadowmap"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler"), 1);

	vector<int> depth = configureDepthMap();
	int depthMapFBO = depth[0];
	int depthMap = depth[1];
	glUseProgram(shaderProgramTest);
	glUniform1i(glGetUniformLocation(shaderProgramTest, "depthMap"), 0);

	glUseProgram(shaderProgramTest2);
	glUniform1i(glGetUniformLocation(shaderProgramTest2, "shadowmap"), 0);
	glUniform1i(glGetUniformLocation(shaderProgramTest2, "diffuseTexture"), 1);


	// Set light position vector in fragment shader to current light position value (done once)
	GLuint lightPosLocation = glGetUniformLocation(shaderProgram, "lightPos");
	glUniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z);

	// Used to ovewrite camera postion in fragment shader
	GLuint viewPosLocation = glGetUniformLocation(shaderProgram, "viewPos");

	// Used to overwrite color in the fragment shader
	GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");

	//toggle texture location
	GLuint enableTextureLocation = glGetUniformLocation(shaderProgram, "hasTexture");
	int enableTexture = 1;	//1 for on, 0 for off
	//toggle shadow location
	GLuint enableShadowLocation = glGetUniformLocation(shaderProgram, "hasShadow");
	int enableShadow = 1;//1 for on, 0 for off
	// Used to set light position for shadow calculation in shader
	GLuint lightSpaceMatrixLocation = glGetUniformLocation(shaderProgram, "lightSpaceMatrix");

	// Camera parameters for view transform
	const float initial_xpos = 0;
	const float initial_ypos = 5;
	const float initial_zpos = 20;
	//distance to origin in xz-plane.
	const float initial_rpos = sqrt(powf(initial_xpos, 2) + powf(initial_zpos, 2));
	const vec3 initial_cameraPosition(initial_xpos, initial_ypos, initial_zpos);
	//const vec3 initial_cameraLookAt(0.0f, 0.0f, -1.0f);   //<-overridden by camera Horizontal/Vertical Angle anyways.
	const vec3 initial_cameraUp(0.0f, 1.0f, 0.0f);
	// Camera parameters for view transform
	vec3 cameraPosition = initial_cameraPosition;
	vec3 cameraLookAt /*= initial_cameraLookAt*/;
	vec3 cameraUp = initial_cameraUp;

	// Other camera parameters, set to look at origin.
	const float initial_cameraHorizontalAngle = degrees(atan2f(initial_zpos, -initial_xpos));
	const float initial_cameraVerticalAngle = degrees(atan2f(initial_ypos, initial_rpos));
	const float initialFoV = 70.0f;

	float cameraSpeed = 10.0f;
	float cameraFastSpeed = 2 * cameraSpeed;
	float cameraHorizontalAngle = initial_cameraHorizontalAngle;
	float cameraVerticalAngle = -initial_cameraVerticalAngle;
	bool  cameraFirstPerson = true; // press 1 or 2 to toggle this variable
	float foV = initialFoV;

	// Mouse input parameters (true if pressed, false if released)
	bool leftMouseButton = false;
	bool rightMouseButton = false;
	bool middleMouseButton = false;

	// Spinning cube at camera position
	float spinningCubeAngle = 0.0f;

	// Set projection matrix for shader
	mat4 projectionMatrix = glm::perspective(radians(initialFoV),   // field of view in degrees
		800.0f / 600.0f,  // aspect ratio
		0.01f, 100.0f);   // near and far (near > 0)

	//glm::mat4 projectionMatrix = glm::ortho(-40.0f, 40.0f,    // left/right
	//    -40.0f, 40.0f,    // bottom/top
	//    -100.0f, 100.0f);  // near/far (near == 0 is ok for ortho)

	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

	// Set initial view matrix
	mat4 viewMatrix = lookAt(cameraPosition,  // eye
		cameraPosition + cameraLookAt,  // center
		cameraUp); // up
	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	//glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

	// Define and upload geometry to the GPU here ...
	int cubeVAOa = createTexturedCubeVertexArrayObject();
	int cubeVAOFront = createTexturedCubeFaceVertexArrayObject(3);
	int cubeVAOLeft = createTexturedCubeFaceVertexArrayObject(0);
	int cubeVAORight = createTexturedCubeFaceVertexArrayObject(4);
	int cubeVAOTop = createTexturedCubeFaceVertexArrayObject(5);
	int cubeVAOBottom = createTexturedCubeFaceVertexArrayObject(2);
	int cubeVAOBack = createTexturedCubeFaceVertexArrayObject(1);

	int planeVAO = createPlaneVertexArrayObject();
	// For frame time
	float lastFrameTime = glfwGetTime();

	//Previous key states to track
	//int lastMouseLeftState = GLFW_RELEASE;
	map<int, KeyState> previousKeyStates;
	//(<key>, { <initialized previous state> , <shift required>})
	//model y-axis rotations
	previousKeyStates.insert(pair<int, KeyState>(GLFW_KEY_A, { GLFW_RELEASE , false }));
	previousKeyStates.insert(pair<int, KeyState>(GLFW_KEY_D, { GLFW_RELEASE , false }));
	//model scalings
	previousKeyStates.insert(pair<int, KeyState>(GLFW_KEY_U, { GLFW_RELEASE , true }));
	previousKeyStates.insert(pair<int, KeyState>(GLFW_KEY_J, { GLFW_RELEASE , true }));
	//shear model (temporary)
	previousKeyStates.insert(pair<int, KeyState>(GLFW_KEY_Q, { GLFW_RELEASE , false }));
	previousKeyStates.insert(pair<int, KeyState>(GLFW_KEY_E, { GLFW_RELEASE , false }));
	//toggle textures
	previousKeyStates.insert(pair<int, KeyState>(GLFW_KEY_X, { GLFW_RELEASE , true }));
	//toggle shadows
	previousKeyStates.insert(pair<int, KeyState>(GLFW_KEY_B, { GLFW_RELEASE , true }));

	double lastMousePosX, lastMousePosY;
	glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

	// Other OpenGL states to set once
	// Enable Backface culling
	glEnable(GL_CULL_FACE);

	// @TODO 1 - Enable Depth Test
	// ...
	glEnable(GL_DEPTH_TEST);



	//prepare initial positions of box
	vector<mat4> init_T(0);
	vector<mat4>::iterator init_T_itr;
	const float boxSideLength = 3.0f;
	const int boxPerSide = 3;
	const float boxCenterHeight = boxPerSide / 2 * boxSideLength + boxSideLength;
	// indexes 0 to 3 is back bottom row
	// indexes 0 to 9 is back wall
	for (int i = 0; i < boxPerSide; i++) {
		for (int j = 0; j < boxPerSide; j++) {
			for (int k = 0; k < boxPerSide; k++) {
				mat4 tempInit_T = translate(mat4(1.0f),
					vec3(-(boxSideLength * (boxPerSide - 1) / 2) + k * boxSideLength,
						-(boxSideLength * (boxPerSide - 1) / 2) + j * boxSideLength + boxCenterHeight,
						-(boxSideLength * (boxPerSide - 1) / 2) + i * boxSideLength));
				init_T.push_back(tempInit_T);
			}
		}
	}

	//Models
	//CharModel* selectedModel;
	CharModel* models[numMainModels];
	int modelIndex = 0;
	//								 [    x       y       z]
	ModelBoxFace f1(shaderProgram, init_T[1 * 0 + 3 * 0 + 9 * 2]);
	ModelBoxFace f2(shaderProgram, init_T[1 * 1 + 3 * 0 + 9 * 2]);
	ModelBoxFace f3(shaderProgram, init_T[1 * 2 + 3 * 0 + 9 * 2]);
	ModelBoxFace f4(shaderProgram, init_T[1 * 0 + 3 * 1 + 9 * 2]);
	ModelBoxFace f5(shaderProgram, init_T[1 * 1 + 3 * 1 + 9 * 2]);
	ModelBoxFace f6(shaderProgram, init_T[1 * 2 + 3 * 1 + 9 * 2]);
	ModelBoxFace f7(shaderProgram, init_T[1 * 0 + 3 * 2 + 9 * 2]);
	ModelBoxFace f8(shaderProgram, init_T[1 * 1 + 3 * 2 + 9 * 2]);
	ModelBoxFace f9(shaderProgram, init_T[1 * 2 + 3 * 2 + 9 * 2]);
	ModelBoxFace m1(shaderProgram, init_T[1 * 0 + 3 * 0 + 9 * 1]);
	ModelBoxFace m2(shaderProgram, init_T[1 * 1 + 3 * 0 + 9 * 1]);
	ModelBoxFace m3(shaderProgram, init_T[1 * 2 + 3 * 0 + 9 * 1]);
	ModelBoxFace m4(shaderProgram, init_T[1 * 0 + 3 * 1 + 9 * 1]);
	ModelBoxFace m5(shaderProgram, init_T[1 * 1 + 3 * 1 + 9 * 1]);
	ModelBoxFace m6(shaderProgram, init_T[1 * 2 + 3 * 1 + 9 * 1]);
	ModelBoxFace m7(shaderProgram, init_T[1 * 0 + 3 * 2 + 9 * 1]);
	ModelBoxFace m8(shaderProgram, init_T[1 * 1 + 3 * 2 + 9 * 1]);
	ModelBoxFace m9(shaderProgram, init_T[1 * 2 + 3 * 2 + 9 * 1]);
	ModelBoxFace b1(shaderProgram, init_T[1 * 0 + 3 * 0 + 9 * 0]);
	ModelBoxFace b2(shaderProgram, init_T[1 * 1 + 3 * 0 + 9 * 0]);
	ModelBoxFace b3(shaderProgram, init_T[1 * 2 + 3 * 0 + 9 * 0]);
	ModelBoxFace b4(shaderProgram, init_T[1 * 0 + 3 * 1 + 9 * 0]);
	ModelBoxFace b5(shaderProgram, init_T[1 * 1 + 3 * 1 + 9 * 0]);
	ModelBoxFace b6(shaderProgram, init_T[1 * 2 + 3 * 1 + 9 * 0]);
	ModelBoxFace b7(shaderProgram, init_T[1 * 0 + 3 * 2 + 9 * 0]);
	ModelBoxFace b8(shaderProgram, init_T[1 * 1 + 3 * 2 + 9 * 0]);
	ModelBoxFace b9(shaderProgram, init_T[1 * 2 + 3 * 2 + 9 * 0]);

	models[0] = &f1;
	models[1] = &f2;
	models[2] = &f3;
	models[3] = &f4;
	models[4] = &f5;
	models[5] = &f6;
	models[6] = &f7;
	models[7] = &f8;
	models[8] = &f9;
	models[9] = &m1;
	models[10] = &m2;
	models[11] = &m3;
	models[12] = &m4;
	models[13] = &m5;
	models[14] = &m6;
	models[15] = &m7;
	models[16] = &m8;
	models[17] = &m9;
	models[18] = &b1;
	models[19] = &b2;
	models[20] = &b3;
	models[21] = &b4;
	models[22] = &b5;
	models[23] = &b6;
	models[24] = &b7;
	models[25] = &b8;
	models[26] = &b9;

	//previous frame, if valid input to model.
	bool prevHadMovement = false;

	//float time = 0;
	GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
	GLuint shadowWorldMatrixLocation = glGetUniformLocation(shaderProgramShadow, "model");


	//put store info, so can render easier for shadow map.
	RenderInfo renderInfo;

	renderInfo.colorLocation = colorLocation;
	renderInfo.enableTextureLocation = enableTextureLocation;
	renderInfo.enableShadowLocation = enableShadowLocation;

	renderInfo.enableTexture = enableTexture;
	renderInfo.enableShadow = enableShadow;

	renderInfo.textures.depthMap = depthMap;
	renderInfo.textures.tiledTextureID = tiledTextureID;
	renderInfo.textures.boxTextureID = boxTextureID;
	renderInfo.textures.metalTextureID = metalTextureID;
	renderInfo.textures.brickTextureID = brickTextureID;
	renderInfo.textures.cloverTextureID = cloverTextureID;
	renderInfo.textures.jaguarTextureID = jaguarTextureID;
	renderInfo.textures.woodTextureID = woodTextureID;

	renderInfo.cubeVAOa = cubeVAOa;
	renderInfo.cubeVAOFront = cubeVAOFront;
	renderInfo.cubeVAOBack = cubeVAOBack;
	renderInfo.cubeVAOLeft = cubeVAOLeft;
	renderInfo.cubeVAORight = cubeVAORight;
	renderInfo.cubeVAOTop = cubeVAOTop;
	renderInfo.cubeVAOBottom = cubeVAOBottom;
	// Entering Main Loop
	while (!glfwWindowShouldClose(window))
	{
		// Frame time calculation
		float dt = glfwGetTime() - lastFrameTime;
		lastFrameTime += dt;
		// Each frame, reset color of each pixel to glClearColor
		// @TODO 1 - Clear Depth Buffer Bit as well
		// ...
		//const float speed = dt * 30;
		//time += speed;
		//glClearColor(0.4f * (1 + cosf(radians(1.3f * time))), 0.4f * (1 + cosf(radians(1.5f * time + 120))), 0.4f * (1 + cosf(radians(1.7f * time - 120))), 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
		// 1. render depth of scene to texture (from light's perspective)
	   // --------------------------------------------------------------
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 0.0f, far_plane = 35.0f;
		lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		//set lightSpaceMatrix in shader
		glUseProgram(shaderProgram);
		glUniformMatrix4fv(lightSpaceMatrixLocation, 1, GL_FALSE, &lightSpaceMatrix[0][0]);
		// render scene from light's point of view
		glUseProgram(shaderProgramShadow);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgramShadow, "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);
		////
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tiledTextureID);
		//avoid peter panning
		//glCullFace(GL_FRONT);

		//renderScene(shaderProgramShadow, cubeVAOa);

		//render with shader used to create a shadow map
		renderInfo.shaderProgram = shaderProgramShadow;
		renderInfo.worldMatrixLocation = shadowWorldMatrixLocation;

		glBindBuffer(GL_ARRAY_BUFFER, cubeVAOa);
		renderDecor(renderInfo);
		renderModels(renderInfo, models);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glCullFace(GL_BACK);
		////
		//// reset viewport
		glViewport(0, 0, window_width, window_height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//
				// 2. render scene as normal using the generated depth/shadow map  
		// --------------------------------------------------------------

		glBindVertexArray(planeVAO);
		glViewport(0, 0, window_width, window_height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaderProgramTest2);
		//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		//glm::mat4 view = camera.GetViewMatrix();
		mat4 projection = projectionMatrix;
		mat4 view = viewMatrix;
		glUniformMatrix4fv(glGetUniformLocation(shaderProgramTest2, "projection"), 1, GL_FALSE, &projection[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgramTest2, "view"), 1, GL_FALSE, &view[0][0]);
		// set light uniforms
		glUniform3fv(glGetUniformLocation(shaderProgramTest2, "viewPos"), 1, &cameraPosition[0]);
		glUniform3fv(glGetUniformLocation(shaderProgramTest2, "lightPos"), 1, &lightPos[0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgramTest2, "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tiledTextureID);
		//renderScene(shaderProgramTest2, planeVAO);

		// render Depth map to quad for visual debugging
		// ---------------------------------------------
		glUseProgram(shaderProgramTest);
		glUniform1f(glGetUniformLocation(shaderProgramTest, "near_plane"), near_plane);
		glUniform1f(glGetUniformLocation(shaderProgramTest, "far_plane"), far_plane);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		//renderQuad();




		glUseProgram(shaderProgram);
		glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, &lightPos[0]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);
		//bind shadow map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);


		//bind a VAO
		glBindVertexArray(cubeVAOa);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVAOa);
		//bind a texture
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tiledTextureID);

		//render with main shader
		renderInfo.shaderProgram = shaderProgram;
		renderInfo.worldMatrixLocation = worldMatrixLocation;

		//Draw ground
		renderDecor(renderInfo);

		//update and draw control and models
		{
			//get key inputs
			//selection with keys.
			drawControl(window);
			modelIndex = selectModelControl(window, modelIndex);
			//Control model key presses.
			mat4* relativeWorldMatrix = modelControl(window, dt, previousKeyStates);
			bool hasMovement = checkModelMovement(window, previousKeyStates);
			bool* selectedSetting = customControl(window, previousKeyStates);

			//apply effect of key inputs.
			CharModel* prevModel = selectedModel;
			selectedModel = models[modelIndex];

			//special keys
			//Home key has been pressed, so reset world matrices.
			if (selectedSetting[0] || selectedSetting[1]) {
				//reset TRS matrices
				selectedModel->resetInitialRelativeMatrices();
				//reset walk state.
				selectedModel->walkState.setState(0);
				//reset camera too.
				cameraPosition = initial_cameraPosition;
				//cameraLookAt = initial_cameraLookAt;
				cameraHorizontalAngle = initial_cameraHorizontalAngle;
				cameraVerticalAngle = -initial_cameraVerticalAngle;
				cameraUp = initial_cameraUp;
				foV = initialFoV;
			}
			//X key has been pressed, so toggle textures.
			if (selectedSetting[2]) {
				enableTexture = enableTexture * -1 + 1;


				//manual toggle shear movement.
				//ToDo: delete this comment.
				//int temp = selectedModel->walkState.getState();
				//temp = (temp ==1) ? 2: 1;
				//selectedModel->walkState.setState(temp);
			}
			//B key has been pressed, so toggle shadows.
			if (selectedSetting[3]) {
				enableShadow = enableShadow * -1 + 1;
			}

			//draw models
			renderModels(renderInfo, models);

			//then update (order of draw and update is arbitrary. done after because shadows aren't from updated models)
			CharModel::update(models, dt);

			//Adjust selected model accordingly.
			selectedModel->addRelativeWorldMatrix(relativeWorldMatrix[0], relativeWorldMatrix[1], relativeWorldMatrix[2]);

			//checks whether the models move
			if (hasMovement) {
				//start walking
				selectedModel->walkState.setState(1);
			}
			else if (prevHadMovement) {
				//stop walking
				selectedModel->walkState.setState(2);
			}
			prevHadMovement = hasMovement;

			//makes previous model stop walking
			if (prevModel != selectedModel && prevModel && prevModel->walkState.getState() != 0) {

				prevModel->walkState.setState(2);
			}
		}

		//misc


		glBindVertexArray(cubeVAOa);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVAOa);

		// Spinning cube at camera position
		spinningCubeAngle += 180.0f * dt;

		// @TODO 7 - Draw in view space for first person camera
		if (cameraFirstPerson) {
			mat4 spinningCubeWorldMatrix = mat4(1.0);
			mat4 spinningCubeViewMatrix = translate(mat4(1.0f), vec3(0.0f, 0.0f, -1.0f)) *
				rotate(mat4(1.0f), radians(spinningCubeAngle), vec3(0.0f, 1.0f, 0.0f)) *
				scale(mat4(1.0f), vec3(0.01f, 0.01f, 0.01f));

			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &spinningCubeWorldMatrix[0][0]);
			glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &spinningCubeViewMatrix[0][0]);
			//glClearColor(0.0f, 0.3f, 0.4f, 1.0f);
		}
		else {
			// In third person view, let's draw the spinning cube in world space, like any other models
			mat4 spinningCubeWorldMatrix = translate(mat4(1.0f), cameraPosition) *
				rotate(mat4(1.0f), radians(spinningCubeAngle), vec3(0.0f, 1.0f, 0.0f)) *
				scale(mat4(1.0f), vec3(0.1f, 0.1f, 0.1f));

			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &spinningCubeWorldMatrix[0][0]);
			//glClearColor(0.4f, 0.3f, 0.0f, 1.0f);
		}
		glUniform3f(colorLocation, 0.0f, 0.5f, 0.5f);
		//no texture
		glBindTexture(GL_TEXTURE_2D, 0);
		glUniform1i(enableTextureLocation, 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glUniform1i(enableTextureLocation, enableTexture);


		//update previous key states.
		map<int, KeyState>::iterator itr;
		for (itr = previousKeyStates.begin(); itr != previousKeyStates.end(); ++itr) {
			int currentState = glfwGetKey(window, itr->first);
			//only update to pressed if in the right shift mode.
			if (itr->second.needShiftPressed == isShiftPressed(window)) {
				itr->second.keyState = currentState;
			}
			//Consider not pressed before otherwise
			else {
				itr->second.keyState = GLFW_RELEASE;
			}

			//consider shift being held with the button, when either shift is held or was held with it and the button is held.
			itr->second.prevWithShiftPressed = (isShiftPressed(window) || itr->second.prevWithShiftPressed) && currentState == GLFW_PRESS;
		}

		// End Frame
		glfwSwapBuffers(window);
		glfwPollEvents();

		// Handle inputs
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);


		// This was solution for Lab02 - Moving camera exercise
		bool fastCam = isShiftPressed(window);
		float currentCameraSpeed = (fastCam) ? cameraFastSpeed : cameraSpeed;


		//lab 03 code
		// @TODO 4 - Calculate mouse motion dx and dy
		//         - Update camera horizontal and vertical angle
		// Please understand the code when you un-comment it!

		double mousePosX, mousePosY;
		glfwGetCursorPos(window, &mousePosX, &mousePosY);

		double dx = mousePosX - lastMousePosX;
		double dy = mousePosY - lastMousePosY;

		lastMousePosX = mousePosX;
		lastMousePosY = mousePosY;

		// Convert to spherical coordinates
		const float cameraAngularSpeed = 60.0f;

		// Moved to mouse controls
		//cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
		//cameraVerticalAngle -= dy * cameraAngularSpeed * dt;

		// Clamp vertical angle to [-85, 85] degrees
		cameraVerticalAngle = std::fmax(-85.0f, std::fmin(85.0f, cameraVerticalAngle));
		if (cameraHorizontalAngle > 360)
		{
			cameraHorizontalAngle -= 360;
		}
		else if (cameraHorizontalAngle < -360)
		{
			cameraHorizontalAngle += 360;
		}

		float theta = radians(cameraHorizontalAngle);
		float phi = radians(cameraVerticalAngle);

		cameraLookAt = vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));
		vec3 cameraSideVector = glm::cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));

		glm::normalize(cameraSideVector);

		// Update camera position in fragment vector
		glUniform3f(viewPosLocation, cameraPosition.x, cameraPosition.y, cameraPosition.z);

		// Limit FoV between 1.0 deg and 120.0 deg
		if (foV < 1.0f)
			foV = 1.0f;
		else if (foV > 120.0f)
			foV = 120.0f;

		// Recompute projection matrix depending on FoV
		projectionMatrix = glm::perspective(radians(foV),            // field of view in degrees
			(float)window_width / (float)window_height,  // aspect ratio
			0.01f, 100.0f);   // near and far (near > 0)
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

		// @TODO 5 = use camera lookat and side vectors to update positions with HNBM
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) // move camera down
		{
			cameraPosition += cameraLookAt * currentCameraSpeed * dt;
		}
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) // move camera up
		{
			cameraPosition -= cameraLookAt * currentCameraSpeed * dt;
		}
		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) // move camera to the left
		{
			cameraPosition -= cameraSideVector * currentCameraSpeed * dt;
		}
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) // move camera to the right
		{
			cameraPosition += cameraSideVector * currentCameraSpeed * dt;
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // pitch camera up
		{
			cameraVerticalAngle += currentCameraSpeed * dt;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) // pitch camera down
		{
			cameraVerticalAngle -= currentCameraSpeed * dt;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // yaw camera right
		{
			cameraHorizontalAngle -= currentCameraSpeed * dt;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // yaw camera left
		{
			cameraHorizontalAngle += currentCameraSpeed * dt;
		}

		// TODO 6
		// Set the view matrix for first and third person cameras
		// - In first person, camera lookat is set like below
		// - In third person, camera position is on a sphere looking towards center

		//mat4 viewMatrix = mat4(1.0);

		if (cameraFirstPerson) {
			viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
		}
		else {
			float radius = 5.0f;
			glm::vec3 position = cameraPosition - radius * cameraLookAt;
			viewMatrix = lookAt(position, position + cameraLookAt, cameraUp);
		}

		GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);



		// Process mouse button inputs (press assigns button to true, release to false)
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
			leftMouseButton = true;
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
			leftMouseButton = false;
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
			rightMouseButton = true;
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
			rightMouseButton = false;
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
			middleMouseButton = true;
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE)
			middleMouseButton = false;

		// Define mouse controls from specific mouse button input
		// Note: All mouse actions can be used simultaneously
		// Press left mouse button -> Zoom in and out (by modifiying FoV)
		if (leftMouseButton)
			foV += dy * cameraAngularSpeed * dt;
		// Press right mouse button -> pan left and right (yaw)
		if (rightMouseButton)
			cameraHorizontalAngle -= dx * cameraAngularSpeed * dt; // taken from Lab 3
		// Press middle mouse button -> tilt up and down (pitch)
		if (middleMouseButton)
			cameraVerticalAngle -= dy * cameraAngularSpeed * dt; // taken from Lab 3

	}

	// Shutdown GLFW
	glfwTerminate();

	return 0;
}

// Handles window resizing. Retrieves window and buffer dimensions, and viewport is adjusted to current buffer dimensions
void window_size_callback(GLFWwindow* window, int width, int height)
{
	window_width = width;
	window_height = height;
	// Define the viewport dimensions
	int vp_width, vp_height;
	// This function retrieves the size, in pixels, of the framebuffer of the specified window
	glfwGetFramebufferSize(window, &vp_width, &vp_height);
	glViewport(0, 0, vp_width, vp_height);
}

// Key callback method (to not constantly poll some key inputs)
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	// Spacebar will translate a selected model to a random position
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		randomPosModel(selectedModel);
}