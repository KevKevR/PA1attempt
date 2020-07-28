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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <vector>

using namespace glm;
using namespace std;

GLuint loadTexture(const char* filename);

const int numMainModels = 5;

// Define (initial) window width and height
int window_width = 1024, window_height = 768;

// Position of light source
vec3 lightPos = vec3(0.0f, 30.0f, 0.0f);

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
        switch(walkState){
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
    CharModel(int shaderProgram) {
        worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
        colorLocation = glGetUniformLocation(shaderProgram, "objectColor");

        initial_relativeTranslateMatrix = mat4(1.0f);
        initial_relativeRotateMatrix = mat4(1.0f);
        initial_relativeScaleMatrix = mat4(1.0f);

        relativeTranslateMatrix = mat4(1.0f);
        relativeRotateMatrix = mat4(1.0f);
        relativeScaleMatrix = mat4(1.0f);
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

	virtual void drawLetter() {
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
	static void drawLetter(CharModel* arr[numMainModels]) {
		for (int i = 0; i < numMainModels; i++) {
			if (arr[i]) {
				arr[i]->drawLetter();
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
    // Return initial y-position of model
    float getInitY() {return initY;}
    
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


        mat4 worldMatrix = translate(mat4(1.0f), vec3(xOffset, yOffset, 0.0f)) * scale(mat4(1.0f), glm::vec3(-scaler, -scaler, -scaler)); // to make circle transparent, make scale positive
        mat4 mWorldMatrix = relativeWorldMatrix * sphereFollow() * worldMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLE_STRIP, 44, 1261);
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

class ModelV9 : public CharModel {
public:
	//Constructor
	ModelV9(int shaderProgram) : CharModel(shaderProgram) {
		const float model_heightScale = 5.0f; //<-make sure is same as constant in drawV9().
        initY = 0;
		//initialize position with translate matrix
		initial_relativeTranslateMatrix =
			translate(mat4(1.0f),
				vec3(45.5,
					initY,
					45.5));

		setRelativeTranslateMatrix(initial_relativeTranslateMatrix);

		////
		//initialize orientation with rotate matrix
		initial_relativeRotateMatrix = mat4(1.0f);
		setRelativeRotateMatrix(initial_relativeRotateMatrix);
		//initialize size with scale matrix
		initial_relativeScaleMatrix = mat4(1.0f);
		setRelativeScaleMatrix(initial_relativeScaleMatrix);

        sphereOffset = { 0.0f, model_heightScale*1.1f, model_heightScale *0.98f};
	}

	//override draw method.
	void drawLetter() {
		//pass arguments stored in parent class.
        glUniform3f(colorLocation, 0.0f, 233.0f / 255.0f, 0.0f);
		drawModelLetterV9(worldMatrixLocation, colorLocation, getRelativeWorldMatrix());
	}

	void drawNumber() {
		//pass arguments stored in parent class.
        glUniform3f(colorLocation, 0.0f, 233.0f / 255.0f, 0.0f);
		drawModelNumberV9(worldMatrixLocation, colorLocation, getRelativeWorldMatrix());
	}
    void drawSphere() {
        //pass arguments stored in parent class.
        glUniform3f(colorLocation, 0.0f, 233.0f / 255.0f, 0.0f);
        CharModel::drawSphere(getRelativeUndeformedWorldMatrix());
    }

private:
	void drawModelLetterV9(GLuint worldMatrixLocation, GLuint colorLocation, mat4 relativeWorldMatrix) {
		//total height is heightScale
		//total width is 2*(letterHalfWidth+apothem)
		//parameters.
		const float heightScale = 5;
		const float widthScale = 1;
		const float angle = 20.0f;
		const int corners = 6;
		//corners = (corners < 3) ? 3 : corners;  //minimum 3 corners.

		mat4 inertialWorldMatrix;   //model from an inertial view of reference, ie centered at origin.
		mat4 worldMatrix;           //complete matrix after all transformations.

		//convert angles to radians
		const float r_angle = abs(radians((float)angle));

		//future calculations done early to determine modelPositioningMatrix for letter, and box measurements.
		const float m9_apothem = ((heightScale + widthScale) / 4);
		const float m9_centralAngle = radians(360.0f / corners);
		const float m9_radius = m9_apothem / cos(m9_centralAngle / 2);
		const float m9_base = 2 * m9_apothem * tanf(m9_centralAngle / 2);

		//base of cube to scale by
		const float mV_base = widthScale * cosf(r_angle);
		//heigtht of cube to scale by
		const float mV_height = (heightScale - widthScale * cosf(r_angle) * sinf(r_angle)) / cosf(r_angle);
		//absolute width of half of V
		const float letterHalfWidth = mV_height * sinf(r_angle) + mV_base * cosf(r_angle);

		//full model measurement
		const float m_height = heightScale;
		const float m_width = (2 * (letterHalfWidth + m9_apothem));
		const float m_maxWidth = 2 * (letterHalfWidth + m9_radius);

		//Offset for V to center the model. Origin will touch bottom center.
		mat4 modelPositioningMatrix =
			translate(mat4(1.0f),
				vec3((-(letterHalfWidth / 2 + m9_apothem)),
                    m_height/2,
					0));

		//Draw "V".
		//total height is heightScale
		//total width is 2 * letterHalfWidth
		//loops for left and right half
		for (int i = 0; i < 2; ++i)
		{
			const int parity = pow(-1, i + 1);
			const float angle_i = r_angle * parity;  //vertical angle of leg 

			inertialWorldMatrix =
				//move into position relative to center of model.
				modelPositioningMatrix
				//space out halves.
				* translate(mat4(1.0f),
					vec3(letterHalfWidth * i,
						0,
						0))
				//rotate by angle_i to angle the legs.
				* rotate(mat4(1.0f),
					angle_i,
					vec3(0.0f, 0.0f, -1.0f))
				//scale legs to match target.
				* scale(mat4(1.0f),
					vec3(mV_base, mV_height, 1.0f));

			//tramsformation to model as a whole
			worldMatrix = relativeWorldMatrix * inertialWorldMatrix;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			//glUniform3f(colorLocation, 1.0f, 233.0f / 255.0f, 0.0f);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//connect the bottom part of the V legs together
		const float base = widthScale * cosf(r_angle);
		const float bottomHeight = base * sinf(r_angle);
		const float bottomBase = base * cosf(r_angle) * 2;
		inertialWorldMatrix =
			//move into position relative to center of model.
			modelPositioningMatrix
			//position it bottom center.
			* translate(mat4(1.0f),
				vec3(letterHalfWidth / 2,
				(-heightScale + bottomHeight) / 2,
					0))
			//scale to fit bottom gap.
			* scale(mat4(1.0f),
				vec3(bottomBase, bottomHeight, 1.0f));
		worldMatrix = relativeWorldMatrix * inertialWorldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		//glUniform3f(colorLocation, 1.0f, 233.0f / 255.0f, 0.0f);
		glDrawArrays(GL_TRIANGLES, 0, 36);

	}
	void drawModelNumberV9(GLuint worldMatrixLocation, GLuint colorLocation, mat4 relativeWorldMatrix) {
		//total height is heightScale
		//total width is 2*(letterHalfWidth+apothem)
		//parameters.
		const float heightScale = 5;
		const float widthScale = 1;
		const float angle = 20.0f;
		const int corners = 6;
		//corners = (corners < 3) ? 3 : corners;  //minimum 3 corners.

		mat4 inertialWorldMatrix;   //model from an inertial view of reference, ie centered at origin.
		mat4 worldMatrix;           //complete matrix after all transformations.

		//convert angles to radians
		const float r_angle = abs(radians((float)angle));

		//future calculations done early to determine modelPositioningMatrix for letter, and box measurements.
		const float m9_apothem = ((heightScale + widthScale) / 4);
		const float m9_centralAngle = radians(360.0f / corners);
		const float m9_radius = m9_apothem / cos(m9_centralAngle / 2);
		const float m9_base = 2 * m9_apothem * tanf(m9_centralAngle / 2);

		//base of cube to scale by
		const float mV_base = widthScale * cosf(r_angle);
		//heigtht of cube to scale by
		const float mV_height = (heightScale - widthScale * cosf(r_angle) * sinf(r_angle)) / cosf(r_angle);
		//absolute width of half of V
		const float letterHalfWidth = mV_height * sinf(r_angle) + mV_base * cosf(r_angle);

		//full model measurement
		const float m_height = heightScale;
		const float m_width = (2 * (letterHalfWidth + m9_apothem));
		const float m_maxWidth = 2 * (letterHalfWidth + m9_radius);

		//Offset for V to center the model. Origin will touch bottom center.
		mat4 modelPositioningMatrix =
			translate(mat4(1.0f),
				vec3((-(letterHalfWidth / 2 + m9_apothem)),
                    m_height,
					0));


		//Draw 9.
		//total height is heightScale
		//total width is 2 * apothem

		//Offset for 9 to center the model.
		modelPositioningMatrix =
			translate(mat4(1.0f),
				vec3(letterHalfWidth,
					0,
					0));
		//Draw in shape of regular polygons.
		for (int i = 0; i < corners; ++i)
		{
			const float angle_i = m9_centralAngle * i;
			const float tailAngle_i = fmod(angle_i + radians(180.0f), radians(360.0f));
			const float thickness = widthScale;

			//draw head
			inertialWorldMatrix =
				//move into position relative to center of model.
				modelPositioningMatrix
				//top height, place center point of polygon.
				* translate(mat4(1.0f),
					vec3(0,
						heightScale - m9_apothem,
						0))
				//rotate by angle_i. done after translating to simplify process of creating a ring.
				* rotate(mat4(1.0f),
					angle_i,
					vec3(0.0f, 0.0f, -1.0f))
				//apothem distance.
				* translate(mat4(1.0f),
					vec3(0,
						m9_apothem - thickness / 2,  //cube object is centered at origin, not its corner.
						0))
				//scale to match target.
				* scale(mat4(1.0f),
					vec3(m9_base, thickness, 1.0f));
			worldMatrix = relativeWorldMatrix * inertialWorldMatrix;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			//glUniform3f(colorLocation, 1.0f, 233.0f / 255.0f, 0.0f);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			//draw tail
			//Offset angle_i by half a turn to match flush with ground
			if (tailAngle_i < radians(240.0f) && tailAngle_i > 0.0f) {
				inertialWorldMatrix =
					////rotate 90 degrees. flat on ground
					//rotate(mat4(1.0f),
					//    radians(90.0f),
					//    vec3(1.0f, 0.0f, 0.0f)) *
					//move into position relative to center of model.
					modelPositioningMatrix
					//bottom height, center of polygon.
					* translate(mat4(1.0f),
						vec3(0,
							m9_apothem,
							0))
					//rotate by angle_i. done after translating to simplify process.
					* rotate(mat4(1.0f),
						tailAngle_i,
						vec3(0.0f, 0.0f, -1.0f))
					//apothem distance.
					* translate(mat4(1.0f),
						vec3(0,
							m9_apothem - thickness / 2,
							0))
					//scale to sides of polygon match target.
					* scale(mat4(1.0f),
						vec3(m9_base, thickness, 1.0f));
				worldMatrix = relativeWorldMatrix * inertialWorldMatrix;
				glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
				//glUniform3f(colorLocation, 1.0f, 233.0f / 255.0f, 0.0f);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

	}
};
class ModelS3 : public CharModel {
public:

	ModelS3(int shaderProgram) : CharModel(shaderProgram) {
		//initialize initial_relativeTranslateMatrix here and set relativeTranslateMatrix to that value.
        initY = -2.0f;
		initial_relativeTranslateMatrix =
			translate(mat4(1.0f),
				vec3(0,
					initY,
					0));

		setRelativeTranslateMatrix(initial_relativeTranslateMatrix);

        sphereOffset = { 0.0f, 9.0f, 5.5f };
	}

	//no need to change anything here, except drawModel's name if you feel like it.
	void drawLetter() {
		//pass arguments stored in parent class.
        glUniform3f(colorLocation, 0.0f, 233.0f / 255.0f, 1.0f);
		drawModelLetterS3(worldMatrixLocation, colorLocation, getRelativeWorldMatrix());
	}

	void drawNumber() {
		//pass arguments stored in parent class.
        glUniform3f(colorLocation, 0.0f, 233.0f / 255.0f, 1.0f);
		drawModelNumberS3(worldMatrixLocation, colorLocation, getRelativeWorldMatrix());
	}
    void drawSphere() {
        //pass arguments stored in parent class.
        glUniform3f(colorLocation, 0.0f, 233.0f / 255.0f, 1.0f);
        CharModel::drawSphere(getRelativeUndeformedWorldMatrix());

    }


private:
	void drawModelNumberS3(GLuint worldMatrixLocation, GLuint colorLocation, mat4 relativeWorldMatrix) {
		//code goes here
		mat4 mWorldMatrix;
		mat4 scalingMatrix, translationMatrix;

		// Draw 3
		// Top
		scalingMatrix = scale(mat4(1.0f), vec3(2.0f, 1.0f, 1.0f));
		translationMatrix = translate(mat4(1.0f), vec3(2.0f, 7.5f, 0.0f));
		glm::mat4 worldMatrix = translationMatrix * scalingMatrix;
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//middle row
		translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 5.0f, 0.0f));
		worldMatrix = translationMatrix * scalingMatrix;
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//bottom row
		translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 2.5f, 0.0f));
		worldMatrix = translationMatrix * scalingMatrix;
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//right side
		scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 6.0f, 1.0f));
		translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(3.5f, 5.0f, 0.0f));
		worldMatrix = translationMatrix * scalingMatrix;
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	void drawModelLetterS3(GLuint worldMatrixLocation, GLuint colorLocation, mat4 relativeWorldMatrix) {
		//code goes here
		mat4 mWorldMatrix;
		mat4 scalingMatrix, translationMatrix;
		glm::mat4 worldMatrix = translationMatrix * scalingMatrix;

		//Draw S
		scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.5f, 1.0f));
		translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, 7.25f, 0.0f));
		worldMatrix = translationMatrix * scalingMatrix;
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2.5f, 7.5f, 0.0f));
		worldMatrix = translationMatrix * scalingMatrix;
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 3.5f, 1.0f));
		translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-3.5f, 6.25f, 0.0f));
		worldMatrix = translationMatrix * scalingMatrix;
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2.5f, 5.0f, 0.0f));
		worldMatrix = translationMatrix * scalingMatrix;
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 3.5f, 1.0f));
		translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, 3.75f, 0.0f));
		worldMatrix = translationMatrix * scalingMatrix;
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2.5f, 2.5f, 0.0f));
		worldMatrix = translationMatrix * scalingMatrix;
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.5f, 1.0f));
		translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-3.5f, 2.75f, 0.0f));
		worldMatrix = translationMatrix * scalingMatrix;
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//drawBorder(7, 9, 1, 1, relativeWorldMatrix, translate(mat4(1.0f), vec3(0, 5.0f, 0)));
        
	}
};

class ModelA9 : public CharModel {
public:

	ModelA9(int shaderProgram) : CharModel(shaderProgram) {
		//initialize initial_relativeTranslateMatrix here and set relativeTranslateMatrix to that value.
        initY = 0.0f;
		initial_relativeTranslateMatrix =
			translate(mat4(1.0f),
				vec3(-44.5,
                     initY,
					-44.5));

		setRelativeTranslateMatrix(initial_relativeTranslateMatrix);
        sphereOffset = { 0.0f, 8.0f, 7.0f };
	}

	//no need to change anything here, except drawModel's name if you feel like it.
	void drawLetter() {
		//pass arguments stored in parent class.
        glUniform3f(colorLocation, 233.0f / 255.0f, 1, 0.0f);
		drawModelLetterA9(worldMatrixLocation, colorLocation, getRelativeWorldMatrix());
	}

	void drawNumber() {
		//pass arguments stored in parent class.
        glUniform3f(colorLocation, 233.0f / 255.0f, 1, 0.0f);
		drawModelNumberA9(worldMatrixLocation, colorLocation, getRelativeWorldMatrix());
	}
    void drawSphere() {
        //pass arguments stored in parent class.
        glUniform3f(colorLocation, 233.0f / 255.0f, 1, 0.0f);
        CharModel::drawSphere(getRelativeUndeformedWorldMatrix());

    }

private:
	void drawModelLetterA9(GLuint worldMatrixLocation, GLuint colorLocation, mat4 relativeWorldMatrix) {
		//code goes here

		//pattern to draw models to make use of relativeWorldMatrix:
		//worldMatrix = relativeWorldMatrix * (model's original matrices);
		//glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		mat4 worldMatrix = mat4(1.0f);
		mat4 mWorldMatrix;
		// Draw A
		// Right Vertical Bar
		worldMatrix = translate(mat4(1.0f), vec3(-1.0f, 2.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f * 4.0f, 1.0f));

		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Left Vertical Bar
		worldMatrix = translate(mat4(1.0f), vec3(-5.0f, 2.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 4.0f, 1.0f));
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Top Horizontal Bar
		worldMatrix = translate(mat4(1.0f), vec3(-3.0f, 4.5f, 0.0f)) * scale(mat4(1.0f), vec3(3.5f, 1.0f, 1.0f));
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Bottom Horizontal Bar
		worldMatrix = translate(mat4(1.0f), vec3(-3.0f, 2.5f, 0.0f)) * scale(mat4(1.0f), vec3(3.5f, 1.0f, 1.0f));
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	void drawModelNumberA9(GLuint worldMatrixLocation, GLuint colorLocation, mat4 relativeWorldMatrix) {
		//code goes here
		mat4 worldMatrix = mat4(1.0f);
		mat4 mWorldMatrix;

		// Draw 9
		// Long Vertical Bar
		worldMatrix = translate(mat4(1.0f), vec3(5.0f, 2.5f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Short Vertical Bar
		worldMatrix = translate(mat4(1.0f), vec3(1.0f, 3.5f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 3.0f, 1.0f));
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Top Horizontal Bar
		worldMatrix = translate(mat4(1.0f), vec3(3.0f, 4.5f, 0.0f)) * scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Bottom Horizontal Bar
		worldMatrix = translate(mat4(1.0f), vec3(3.0f, 2.5f, 0.0f)) * scale(mat4(1.0f), vec3(3.0f, 1.0f, 1.0f));
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//drawBorder(7, 12, 1, 1, relativeWorldMatrix, translate(mat4(1.0f), vec3(0, 3.5f, 0)));
    
	}
};

class ModelN2 : public CharModel {
public:

	ModelN2(int shaderProgram) : CharModel(shaderProgram) {
		//initialize initial_relativeTranslateMatrix here and set relativeTranslateMatrix to that value.
		//example here has position set to (0, 0, -20).
        initY = 0.0f;
		initial_relativeTranslateMatrix =
			translate(mat4(1.0f),
				vec3(44,
					initY,
					-45));

		setRelativeTranslateMatrix(initial_relativeTranslateMatrix);
        sphereOffset = { 0.5f, 8.0f, 7.0f };
	}

	//no need to change anything here, except drawModel's name if you feel like it.
	void drawLetter() {
		//pass arguments stored in parent class.
        glUniform3f(colorLocation, 1, 0, 233.0f / 255.0f);
		drawModelLetterN2(worldMatrixLocation, colorLocation, getRelativeWorldMatrix());
	}

	void drawNumber() {
		//pass arguments stored in parent class.
        glUniform3f(colorLocation, 1, 0, 233.0f / 255.0f);
		drawModelNumberN2(worldMatrixLocation, colorLocation, getRelativeWorldMatrix());
	}
    void drawSphere() {
        //pass arguments stored in parent class.
        glUniform3f(colorLocation, 1, 0, 233.0f / 255.0f);
        CharModel::drawSphere(getRelativeUndeformedWorldMatrix());
    }

private:
	void drawModelLetterN2(GLuint worldMatrixLocation, GLuint colorLocation, mat4 relativeWorldMatrix) {
		//code goes here

		//pattern to draw models to make use of relativeWorldMatrix:
		//worldMatrix = relativeWorldMatrix * (model's original matrices);
		//glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		mat4 worldMatrix = mat4(1.0f);
		mat4 mWorldMatrix;

		// Draw N
		// Right Vertical Bar
		worldMatrix = translate(mat4(1.0f), vec3(-0.5f, 2.5f, 0.5f)) * scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Left Vertical Bar
		worldMatrix = translate(mat4(1.0f), vec3(-4.5f, 2.5f, 0.5f)) * scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Small bar 1
		worldMatrix = translate(mat4(1.0f), vec3(-3.5f, 3.5f, 0.5f)) * scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Small bar 2
		worldMatrix = translate(mat4(1.0f), vec3(-2.5f, 2.5f, 0.5f)) * scale(mat4(1.0f), vec3(1.0f, 2.0f, 1.0f));
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Small bar 3
		worldMatrix = translate(mat4(1.0f), vec3(-1.5f, 1.5f, 0.5f)) * scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	void drawModelNumberN2(GLuint worldMatrixLocation, GLuint colorLocation, mat4 relativeWorldMatrix) {
		//code goes here

		//pattern to draw models to make use of relativeWorldMatrix:
		//worldMatrix = relativeWorldMatrix * (model's original matrices);
		//glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		mat4 worldMatrix = mat4(1.0f);
		mat4 mWorldMatrix;

		// Draw 2
		// Right Vertical Bar
		worldMatrix = translate(mat4(1.0f), vec3(5.5f, 3.5f, 0.5f)) * scale(mat4(1.0f), vec3(1.0f, 2.5f, 1.0f));
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Left Vertical Bar 1
		worldMatrix = translate(mat4(1.0f), vec3(1.5f, 1.5f, 0.5f)) * scale(mat4(1.0f), vec3(1.0f, 1.5f, 1.0f));
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Top Horizontal Bar
		worldMatrix = translate(mat4(1.0f), vec3(3.5f, 4.5f, 0.5f)) * scale(mat4(1.0f), vec3(4.5f, 1.0f, 1.0f));
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Middle Horizontal Bar
		worldMatrix = translate(mat4(1.0f), vec3(3.5f, 2.5f, 0.5f)) * scale(mat4(1.0f), vec3(4.0f, 1.0f, 1.0f));
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Bottom Horizontal Bar
		worldMatrix = translate(mat4(1.0f), vec3(3.5f, 0.5f, 0.5f)) * scale(mat4(1.0f), vec3(5.0f, 1.0f, 1.0f));
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//drawBorder(7, 13.5f, 1, 1, relativeWorldMatrix, translate(mat4(1.0f), vec3(0.5, 3.5f, 0)));
        
	}
};

class ModelN4 : public CharModel {
public:

	ModelN4(int shaderProgram) : CharModel(shaderProgram) {
		//initialize initial_relativeTranslateMatrix here and set relativeTranslateMatrix to that value.
		//example here has position set to (0, 0, -20).
        initY = 0;
		initial_relativeTranslateMatrix =
			translate(mat4(1.0f),
				vec3(-45,
					initY,
					45));

		setRelativeTranslateMatrix(initial_relativeTranslateMatrix);
        sphereOffset = { 0.0f, 7.0f, 6.0f };
	}

	//no need to change anything here, except drawModel's name if you feel like it.
	void drawLetter() {
		//pass arguments stored in parent class.
        glUniform3f(colorLocation, 0.8, 0.8, 0.8f);
		drawModelLetterN4(worldMatrixLocation, colorLocation, getRelativeWorldMatrix());
	}

	void drawNumber() {
		//pass arguments stored in parent class.
        glUniform3f(colorLocation, 0.8, 0.8, 0.8f);
		drawModelNumberN4(worldMatrixLocation, colorLocation, getRelativeWorldMatrix());
	}
    void drawSphere() {
        //pass arguments stored in parent class.
        glUniform3f(colorLocation, 0.8, 0.8, 0.8f);
        CharModel::drawSphere(getRelativeUndeformedWorldMatrix());
    }

private:
	void drawModelLetterN4(GLuint worldMatrixLocation, GLuint colorLocation, mat4 relativeWorldMatrix) {
		//code goes here

		//pattern to draw models to make use of relativeWorldMatrix:
		//worldMatrix = relativeWorldMatrix * (model's original matrices);
		//glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		mat4 worldMatrix = mat4(1.0f);
		mat4 mWorldMatrix;

		// Draw N
		// Right Vertical Bar
		worldMatrix = translate(mat4(1.0f), vec3(-0.5f, 2.5f, 0.5f)) * scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
		//glUniform3f(colorLocation, 1.0f, 233.0f / 255.0f, 0.0f);
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Left Vertical Bar
		worldMatrix = translate(mat4(1.0f), vec3(-4.5f, 2.5f, 0.5f)) * scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		// Diagonal bar
		worldMatrix = rotate(translate(mat4(1.0f), vec3(-2.5f, 2.5f, 0.5f)), radians(41.0f), vec3(0, 0, 1)) * scale(mat4(1.0f), vec3(1.0f, 5.75f, 1.0f));
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	void drawModelNumberN4(GLuint worldMatrixLocation, GLuint colorLocation, mat4 relativeWorldMatrix) {
		//code goes here
		mat4 worldMatrix = mat4(1.0f);
		mat4 mWorldMatrix;

		// Draw 4
		// Right Vertical Bar
		worldMatrix = translate(mat4(1.0f), vec3(4.5f, 2.5f, 0.5f)) * scale(mat4(1.0f), vec3(1.0f, 5.0f, 1.0f));
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Left Small Vertical Bar
		worldMatrix = translate(mat4(1.0f), vec3(1.5f, 4.0f, 0.5f)) * scale(mat4(1.0f), vec3(1.0f, 2.0f, 1.0f));
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Middle Horizontal Bar
		worldMatrix = translate(mat4(1.0f), vec3(3.0f, 2.5f, 0.5f)) * scale(mat4(1.0f), vec3(4.0f, 1.0f, 1.0f));
		mWorldMatrix = relativeWorldMatrix * worldMatrix;
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//drawBorder(7, 12, 1, 1, relativeWorldMatrix, translate(mat4(1.0f), vec3(0, 3.5f, 0)));
        
	}
};

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
        
        // 1261 Sphere vertices (from lab5) (44)
        // Note: Normals may be wrong, but there's too many to modify
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(0.173648, 0.000000, -0.984808), vec3(0.173648, 0.000000, -0.984808),
        vec3(0.171010, 0.030154, -0.984808), vec3(0.171010, 0.030154, -0.984808),
        vec3(0.342020, 0.000000, -0.939693), vec3(0.342020, 0.000000, -0.939693),
        vec3(0.336824, 0.059391, -0.939693), vec3(0.336824, 0.059391, -0.939693),
        vec3(0.500000, 0.000000, -0.866025), vec3(0.500000, 0.000000, -0.866025),
        vec3(0.492404, 0.086824, -0.866025), vec3(0.492404, 0.086824, -0.866025),
        vec3(0.642788, 0.000000, -0.766044), vec3(0.642788, 0.000000, -0.766044),
        vec3(0.633022, 0.111619, -0.766044), vec3(0.633022, 0.111619, -0.766044),
        vec3(0.766044, 0.000000, -0.642788), vec3(0.766044, 0.000000, -0.642788),
        vec3(0.754407, 0.133022, -0.642788), vec3(0.754407, 0.133022, -0.642788),
        vec3(0.866025, 0.000000, -0.500000), vec3(0.866025, 0.000000, -0.500000),
        vec3(0.852869, 0.150384, -0.500000), vec3(0.852869, 0.150384, -0.500000),
        vec3(0.939693, 0.000000, -0.342020), vec3(0.939693, 0.000000, -0.342020),
        vec3(0.925417, 0.163176, -0.342020), vec3(0.925417, 0.163176, -0.342020),
        vec3(0.984808, 0.000000, -0.173648), vec3(0.984808, 0.000000, -0.173648),
        vec3(0.969846, 0.171010, -0.173648), vec3(0.969846, 0.171010, -0.173648),
        vec3(1.000000, 0.000000, 0.000000), vec3(1.000000, 0.000000, 0.000000),
        vec3(0.984808, 0.173648, 0.000000), vec3(0.984808, 0.173648, 0.000000),
        vec3(0.984808, 0.000000, 0.173648), vec3(0.984808, 0.000000, 0.173648),
        vec3(0.969846, 0.171010, 0.173648), vec3(0.969846, 0.171010, 0.173648),
        vec3(0.939693, 0.000000, 0.342020), vec3(0.939693, 0.000000, 0.342020),
        vec3(0.925417, 0.163176, 0.342020), vec3(0.925417, 0.163176, 0.342020),
        vec3(0.866025, 0.000000, 0.500000), vec3(0.866025, 0.000000, 0.500000),
        vec3(0.852869, 0.150384, 0.500000), vec3(0.852869, 0.150384, 0.500000),
        vec3(0.766044, 0.000000, 0.642788), vec3(0.766044, 0.000000, 0.642788),
        vec3(0.754407, 0.133022, 0.642788), vec3(0.754407, 0.133022, 0.642788),
        vec3(0.642788, 0.000000, 0.766044), vec3(0.642788, 0.000000, 0.766044),
        vec3(0.633022, 0.111619, 0.766044), vec3(0.633022, 0.111619, 0.766044),
        vec3(0.500000, 0.000000, 0.866025), vec3(0.500000, 0.000000, 0.866025),
        vec3(0.492404, 0.086824, 0.866025), vec3(0.492404, 0.086824, 0.866025),
        vec3(0.342020, 0.000000, 0.939693), vec3(0.342020, 0.000000, 0.939693),
        vec3(0.336824, 0.059391, 0.939693), vec3(0.336824, 0.059391, 0.939693),
        vec3(0.173648, 0.000000, 0.984808), vec3(0.173648, 0.000000, 0.984808),
        vec3(0.171010, 0.030154, 0.984808), vec3(0.171010, 0.030154, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(0.171010, 0.030154, 0.984808), vec3(0.171010, 0.030154, 0.984808),
        vec3(0.163176, 0.059391, 0.984808), vec3(0.163176, 0.059391, 0.984808),
        vec3(0.336824, 0.059391, 0.939693), vec3(0.336824, 0.059391, 0.939693),
        vec3(0.321394, 0.116978, 0.939693), vec3(0.321394, 0.116978, 0.939693),
        vec3(0.492404, 0.086824, 0.866025), vec3(0.492404, 0.086824, 0.866025),
        vec3(0.469846, 0.171010, 0.866025), vec3(0.469846, 0.171010, 0.866025),
        vec3(0.633022, 0.111619, 0.766044), vec3(0.633022, 0.111619, 0.766044),
        vec3(0.604023, 0.219846, 0.766044), vec3(0.604023, 0.219846, 0.766044),
        vec3(0.754407, 0.133022, 0.642788), vec3(0.754407, 0.133022, 0.642788),
        vec3(0.719846, 0.262003, 0.642788), vec3(0.719846, 0.262003, 0.642788),
        vec3(0.852869, 0.150384, 0.500000), vec3(0.852869, 0.150384, 0.500000),
        vec3(0.813798, 0.296198, 0.500000), vec3(0.813798, 0.296198, 0.500000),
        vec3(0.925417, 0.163176, 0.342020), vec3(0.925417, 0.163176, 0.342020),
        vec3(0.883022, 0.321394, 0.342020), vec3(0.883022, 0.321394, 0.342020),
        vec3(0.969846, 0.171010, 0.173648), vec3(0.969846, 0.171010, 0.173648),
        vec3(0.925417, 0.336824, 0.173648), vec3(0.925417, 0.336824, 0.173648),
        vec3(0.984808, 0.173648, 0.000000), vec3(0.984808, 0.173648, 0.000000),
        vec3(0.939693, 0.342020, 0.000000), vec3(0.939693, 0.342020, 0.000000),
        vec3(0.969846, 0.171010, -0.173648), vec3(0.969846, 0.171010, -0.173648),
        vec3(0.925417, 0.336824, -0.173648), vec3(0.925417, 0.336824, -0.173648),
        vec3(0.925417, 0.163176, -0.342020), vec3(0.925417, 0.163176, -0.342020),
        vec3(0.883022, 0.321394, -0.342020), vec3(0.883022, 0.321394, -0.342020),
        vec3(0.852869, 0.150384, -0.500000), vec3(0.852869, 0.150384, -0.500000),
        vec3(0.813798, 0.296198, -0.500000), vec3(0.813798, 0.296198, -0.500000),
        vec3(0.754407, 0.133022, -0.642788), vec3(0.754407, 0.133022, -0.642788),
        vec3(0.719846, 0.262003, -0.642788), vec3(0.719846, 0.262003, -0.642788),
        vec3(0.633022, 0.111619, -0.766044), vec3(0.633022, 0.111619, -0.766044),
        vec3(0.604023, 0.219846, -0.766044), vec3(0.604023, 0.219846, -0.766044),
        vec3(0.492404, 0.086824, -0.866025), vec3(0.492404, 0.086824, -0.866025),
        vec3(0.469846, 0.171010, -0.866025), vec3(0.469846, 0.171010, -0.866025),
        vec3(0.336824, 0.059391, -0.939693), vec3(0.336824, 0.059391, -0.939693),
        vec3(0.321394, 0.116978, -0.939693), vec3(0.321394, 0.116978, -0.939693),
        vec3(0.171010, 0.030154, -0.984808), vec3(0.171010, 0.030154, -0.984808),
        vec3(0.163176, 0.059391, -0.984808), vec3(0.163176, 0.059391, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(0.163176, 0.059391, -0.984808), vec3(0.163176, 0.059391, -0.984808),
        vec3(0.150384, 0.086824, -0.984808), vec3(0.150384, 0.086824, -0.984808),
        vec3(0.321394, 0.116978, -0.939693), vec3(0.321394, 0.116978, -0.939693),
        vec3(0.296198, 0.171010, -0.939693), vec3(0.296198, 0.171010, -0.939693),
        vec3(0.469846, 0.171010, -0.866025), vec3(0.469846, 0.171010, -0.866025),
        vec3(0.433013, 0.250000, -0.866025), vec3(0.433013, 0.250000, -0.866025),
        vec3(0.604023, 0.219846, -0.766044), vec3(0.604023, 0.219846, -0.766044),
        vec3(0.556670, 0.321394, -0.766044), vec3(0.556670, 0.321394, -0.766044),
        vec3(0.719846, 0.262003, -0.642788), vec3(0.719846, 0.262003, -0.642788),
        vec3(0.663414, 0.383022, -0.642788), vec3(0.663414, 0.383022, -0.642788),
        vec3(0.813798, 0.296198, -0.500000), vec3(0.813798, 0.296198, -0.500000),
        vec3(0.750000, 0.433013, -0.500000), vec3(0.750000, 0.433013, -0.500000),
        vec3(0.883022, 0.321394, -0.342020), vec3(0.883022, 0.321394, -0.342020),
        vec3(0.813798, 0.469846, -0.342020), vec3(0.813798, 0.469846, -0.342020),
        vec3(0.925417, 0.336824, -0.173648), vec3(0.925417, 0.336824, -0.173648),
        vec3(0.852869, 0.492404, -0.173648), vec3(0.852869, 0.492404, -0.173648),
        vec3(0.939693, 0.342020, 0.000000), vec3(0.939693, 0.342020, 0.000000),
        vec3(0.866025, 0.500000, 0.000000), vec3(0.866025, 0.500000, 0.000000),
        vec3(0.925417, 0.336824, 0.173648), vec3(0.925417, 0.336824, 0.173648),
        vec3(0.852869, 0.492404, 0.173648), vec3(0.852869, 0.492404, 0.173648),
        vec3(0.883022, 0.321394, 0.342020), vec3(0.883022, 0.321394, 0.342020),
        vec3(0.813798, 0.469846, 0.342020), vec3(0.813798, 0.469846, 0.342020),
        vec3(0.813798, 0.296198, 0.500000), vec3(0.813798, 0.296198, 0.500000),
        vec3(0.750000, 0.433013, 0.500000), vec3(0.750000, 0.433013, 0.500000),
        vec3(0.719846, 0.262003, 0.642788), vec3(0.719846, 0.262003, 0.642788),
        vec3(0.663414, 0.383022, 0.642788), vec3(0.663414, 0.383022, 0.642788),
        vec3(0.604023, 0.219846, 0.766044), vec3(0.604023, 0.219846, 0.766044),
        vec3(0.556670, 0.321394, 0.766044), vec3(0.556670, 0.321394, 0.766044),
        vec3(0.469846, 0.171010, 0.866025), vec3(0.469846, 0.171010, 0.866025),
        vec3(0.433013, 0.250000, 0.866025), vec3(0.433013, 0.250000, 0.866025),
        vec3(0.321394, 0.116978, 0.939693), vec3(0.321394, 0.116978, 0.939693),
        vec3(0.296198, 0.171010, 0.939693), vec3(0.296198, 0.171010, 0.939693),
        vec3(0.163176, 0.059391, 0.984808), vec3(0.163176, 0.059391, 0.984808),
        vec3(0.150384, 0.086824, 0.984808), vec3(0.150384, 0.086824, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(0.150384, 0.086824, 0.984808), vec3(0.150384, 0.086824, 0.984808),
        vec3(0.133022, 0.111619, 0.984808), vec3(0.133022, 0.111619, 0.984808),
        vec3(0.296198, 0.171010, 0.939693), vec3(0.296198, 0.171010, 0.939693),
        vec3(0.262003, 0.219846, 0.939693), vec3(0.262003, 0.219846, 0.939693),
        vec3(0.433013, 0.250000, 0.866025), vec3(0.433013, 0.250000, 0.866025),
        vec3(0.383022, 0.321394, 0.866025), vec3(0.383022, 0.321394, 0.866025),
        vec3(0.556670, 0.321394, 0.766044), vec3(0.556670, 0.321394, 0.766044),
        vec3(0.492404, 0.413176, 0.766044), vec3(0.492404, 0.413176, 0.766045),
        vec3(0.663414, 0.383022, 0.642788), vec3(0.663414, 0.383022, 0.642788),
        vec3(0.586824, 0.492404, 0.642788), vec3(0.586824, 0.492404, 0.642788),
        vec3(0.750000, 0.433013, 0.500000), vec3(0.750000, 0.433013, 0.500000),
        vec3(0.663414, 0.556670, 0.500000), vec3(0.663414, 0.556670, 0.500000),
        vec3(0.813798, 0.469846, 0.342020), vec3(0.813798, 0.469846, 0.342020),
        vec3(0.719846, 0.604023, 0.342020), vec3(0.719846, 0.604023, 0.342020),
        vec3(0.852869, 0.492404, 0.173648), vec3(0.852869, 0.492404, 0.173648),
        vec3(0.754407, 0.633022, 0.173648), vec3(0.754407, 0.633022, 0.173648),
        vec3(0.866025, 0.500000, 0.000000), vec3(0.866025, 0.500000, 0.000000),
        vec3(0.766044, 0.642788, 0.000000), vec3(0.766044, 0.642788, 0.000000),
        vec3(0.852869, 0.492404, -0.173648), vec3(0.852869, 0.492404, -0.173648),
        vec3(0.754407, 0.633022, -0.173648), vec3(0.754407, 0.633022, -0.173648),
        vec3(0.813798, 0.469846, -0.342020), vec3(0.813798, 0.469846, -0.342020),
        vec3(0.719846, 0.604023, -0.342020), vec3(0.719846, 0.604023, -0.342020),
        vec3(0.750000, 0.433013, -0.500000), vec3(0.750000, 0.433013, -0.500000),
        vec3(0.663414, 0.556670, -0.500000), vec3(0.663414, 0.556670, -0.500000),
        vec3(0.663414, 0.383022, -0.642788), vec3(0.663414, 0.383022, -0.642788),
        vec3(0.586824, 0.492404, -0.642788), vec3(0.586824, 0.492404, -0.642788),
        vec3(0.556670, 0.321394, -0.766044), vec3(0.556670, 0.321394, -0.766044),
        vec3(0.492404, 0.413176, -0.766044), vec3(0.492404, 0.413176, -0.766045),
        vec3(0.433013, 0.250000, -0.866025), vec3(0.433013, 0.250000, -0.866025),
        vec3(0.383022, 0.321394, -0.866025), vec3(0.383022, 0.321394, -0.866025),
        vec3(0.296198, 0.171010, -0.939693), vec3(0.296198, 0.171010, -0.939693),
        vec3(0.262003, 0.219846, -0.939693), vec3(0.262003, 0.219846, -0.939693),
        vec3(0.150384, 0.086824, -0.984808), vec3(0.150384, 0.086824, -0.984808),
        vec3(0.133022, 0.111619, -0.984808), vec3(0.133022, 0.111619, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(0.133022, 0.111619, -0.984808), vec3(0.133022, 0.111619, -0.984808),
        vec3(0.111619, 0.133022, -0.984808), vec3(0.111619, 0.133022, -0.984808),
        vec3(0.262003, 0.219846, -0.939693), vec3(0.262003, 0.219846, -0.939693),
        vec3(0.219846, 0.262003, -0.939693), vec3(0.219846, 0.262003, -0.939693),
        vec3(0.383022, 0.321394, -0.866025), vec3(0.383022, 0.321394, -0.866025),
        vec3(0.321394, 0.383022, -0.866025), vec3(0.321394, 0.383022, -0.866025),
        vec3(0.492404, 0.413176, -0.766044), vec3(0.492404, 0.413176, -0.766045),
        vec3(0.413176, 0.492404, -0.766044), vec3(0.413176, 0.492404, -0.766045),
        vec3(0.586824, 0.492404, -0.642788), vec3(0.586824, 0.492404, -0.642788),
        vec3(0.492404, 0.586824, -0.642788), vec3(0.492404, 0.586824, -0.642788),
        vec3(0.663414, 0.556670, -0.500000), vec3(0.663414, 0.556670, -0.500000),
        vec3(0.556670, 0.663414, -0.500000), vec3(0.556670, 0.663414, -0.500000),
        vec3(0.719846, 0.604023, -0.342020), vec3(0.719846, 0.604023, -0.342020),
        vec3(0.604023, 0.719846, -0.342020), vec3(0.604023, 0.719846, -0.342020),
        vec3(0.754407, 0.633022, -0.173648), vec3(0.754407, 0.633022, -0.173648),
        vec3(0.633022, 0.754407, -0.173648), vec3(0.633022, 0.754407, -0.173648),
        vec3(0.766044, 0.642788, 0.000000), vec3(0.766044, 0.642788, 0.000000),
        vec3(0.642788, 0.766044, 0.000000), vec3(0.642788, 0.766044, 0.000000),
        vec3(0.754407, 0.633022, 0.173648), vec3(0.754407, 0.633022, 0.173648),
        vec3(0.633022, 0.754407, 0.173648), vec3(0.633022, 0.754407, 0.173648),
        vec3(0.719846, 0.604023, 0.342020), vec3(0.719846, 0.604023, 0.342020),
        vec3(0.604023, 0.719846, 0.342020), vec3(0.604023, 0.719846, 0.342020),
        vec3(0.663414, 0.556670, 0.500000), vec3(0.663414, 0.556670, 0.500000),
        vec3(0.556670, 0.663414, 0.500000), vec3(0.556670, 0.663414, 0.500000),
        vec3(0.586824, 0.492404, 0.642788), vec3(0.586824, 0.492404, 0.642788),
        vec3(0.492404, 0.586824, 0.642788), vec3(0.492404, 0.586824, 0.642788),
        vec3(0.492404, 0.413176, 0.766044), vec3(0.492404, 0.413176, 0.766045),
        vec3(0.413176, 0.492404, 0.766044), vec3(0.413176, 0.492404, 0.766045),
        vec3(0.383022, 0.321394, 0.866025), vec3(0.383022, 0.321394, 0.866025),
        vec3(0.321394, 0.383022, 0.866025), vec3(0.321394, 0.383022, 0.866025),
        vec3(0.262003, 0.219846, 0.939693), vec3(0.262003, 0.219846, 0.939693),
        vec3(0.219846, 0.262003, 0.939693), vec3(0.219846, 0.262003, 0.939693),
        vec3(0.133022, 0.111619, 0.984808), vec3(0.133022, 0.111619, 0.984808),
        vec3(0.111619, 0.133022, 0.984808), vec3(0.111619, 0.133022, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(0.111619, 0.133022, 0.984808), vec3(0.111619, 0.133022, 0.984808),
        vec3(0.086824, 0.150384, 0.984808), vec3(0.086824, 0.150384, 0.984808),
        vec3(0.219846, 0.262003, 0.939693), vec3(0.219846, 0.262003, 0.939693),
        vec3(0.171010, 0.296198, 0.939693), vec3(0.171010, 0.296198, 0.939693),
        vec3(0.321394, 0.383022, 0.866025), vec3(0.321394, 0.383022, 0.866025),
        vec3(0.250000, 0.433013, 0.866025), vec3(0.250000, 0.433013, 0.866025),
        vec3(0.413176, 0.492404, 0.766044), vec3(0.413176, 0.492404, 0.766045),
        vec3(0.321394, 0.556670, 0.766044), vec3(0.321394, 0.556670, 0.766044),
        vec3(0.492404, 0.586824, 0.642788), vec3(0.492404, 0.586824, 0.642788),
        vec3(0.383022, 0.663414, 0.642788), vec3(0.383022, 0.663414, 0.642788),
        vec3(0.556670, 0.663414, 0.500000), vec3(0.556670, 0.663414, 0.500000),
        vec3(0.433013, 0.750000, 0.500000), vec3(0.433013, 0.750000, 0.500000),
        vec3(0.604023, 0.719846, 0.342020), vec3(0.604023, 0.719846, 0.342020),
        vec3(0.469846, 0.813798, 0.342020), vec3(0.469846, 0.813798, 0.342020),
        vec3(0.633022, 0.754407, 0.173648), vec3(0.633022, 0.754407, 0.173648),
        vec3(0.492404, 0.852869, 0.173648), vec3(0.492404, 0.852869, 0.173648),
        vec3(0.642788, 0.766044, 0.000000), vec3(0.642788, 0.766044, 0.000000),
        vec3(0.500000, 0.866025, 0.000000), vec3(0.500000, 0.866025, 0.000000),
        vec3(0.633022, 0.754407, -0.173648), vec3(0.633022, 0.754407, -0.173648),
        vec3(0.492404, 0.852869, -0.173648), vec3(0.492404, 0.852869, -0.173648),
        vec3(0.604023, 0.719846, -0.342020), vec3(0.604023, 0.719846, -0.342020),
        vec3(0.469846, 0.813798, -0.342020), vec3(0.469846, 0.813798, -0.342020),
        vec3(0.556670, 0.663414, -0.500000), vec3(0.556670, 0.663414, -0.500000),
        vec3(0.433013, 0.750000, -0.500000), vec3(0.433013, 0.750000, -0.500000),
        vec3(0.492404, 0.586824, -0.642788), vec3(0.492404, 0.586824, -0.642788),
        vec3(0.383022, 0.663414, -0.642788), vec3(0.383022, 0.663414, -0.642788),
        vec3(0.413176, 0.492404, -0.766044), vec3(0.413176, 0.492404, -0.766045),
        vec3(0.321394, 0.556670, -0.766044), vec3(0.321394, 0.556670, -0.766044),
        vec3(0.321394, 0.383022, -0.866025), vec3(0.321394, 0.383022, -0.866025),
        vec3(0.250000, 0.433013, -0.866025), vec3(0.250000, 0.433013, -0.866025),
        vec3(0.219846, 0.262003, -0.939693), vec3(0.219846, 0.262003, -0.939693),
        vec3(0.171010, 0.296198, -0.939693), vec3(0.171010, 0.296198, -0.939693),
        vec3(0.111619, 0.133022, -0.984808), vec3(0.111619, 0.133022, -0.984808),
        vec3(0.086824, 0.150384, -0.984808), vec3(0.086824, 0.150384, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(0.086824, 0.150384, -0.984808), vec3(0.086824, 0.150384, -0.984808),
        vec3(0.059391, 0.163176, -0.984808), vec3(0.059391, 0.163176, -0.984808),
        vec3(0.171010, 0.296198, -0.939693), vec3(0.171010, 0.296198, -0.939693),
        vec3(0.116978, 0.321394, -0.939693), vec3(0.116978, 0.321394, -0.939693),
        vec3(0.250000, 0.433013, -0.866025), vec3(0.250000, 0.433013, -0.866025),
        vec3(0.171010, 0.469846, -0.866025), vec3(0.171010, 0.469846, -0.866025),
        vec3(0.321394, 0.556670, -0.766044), vec3(0.321394, 0.556670, -0.766044),
        vec3(0.219846, 0.604023, -0.766044), vec3(0.219846, 0.604023, -0.766044),
        vec3(0.383022, 0.663414, -0.642788), vec3(0.383022, 0.663414, -0.642788),
        vec3(0.262003, 0.719846, -0.642788), vec3(0.262003, 0.719846, -0.642788),
        vec3(0.433013, 0.750000, -0.500000), vec3(0.433013, 0.750000, -0.500000),
        vec3(0.296198, 0.813798, -0.500000), vec3(0.296198, 0.813798, -0.500000),
        vec3(0.469846, 0.813798, -0.342020), vec3(0.469846, 0.813798, -0.342020),
        vec3(0.321394, 0.883022, -0.342020), vec3(0.321394, 0.883022, -0.342020),
        vec3(0.492404, 0.852869, -0.173648), vec3(0.492404, 0.852869, -0.173648),
        vec3(0.336824, 0.925417, -0.173648), vec3(0.336824, 0.925417, -0.173648),
        vec3(0.500000, 0.866025, 0.000000), vec3(0.500000, 0.866025, 0.000000),
        vec3(0.342020, 0.939693, 0.000000), vec3(0.342020, 0.939693, 0.000000),
        vec3(0.492404, 0.852869, 0.173648), vec3(0.492404, 0.852869, 0.173648),
        vec3(0.336824, 0.925417, 0.173648), vec3(0.336824, 0.925417, 0.173648),
        vec3(0.469846, 0.813798, 0.342020), vec3(0.469846, 0.813798, 0.342020),
        vec3(0.321394, 0.883022, 0.342020), vec3(0.321394, 0.883022, 0.342020),
        vec3(0.433013, 0.750000, 0.500000), vec3(0.433013, 0.750000, 0.500000),
        vec3(0.296198, 0.813798, 0.500000), vec3(0.296198, 0.813798, 0.500000),
        vec3(0.383022, 0.663414, 0.642788), vec3(0.383022, 0.663414, 0.642788),
        vec3(0.262003, 0.719846, 0.642788), vec3(0.262003, 0.719846, 0.642788),
        vec3(0.321394, 0.556670, 0.766044), vec3(0.321394, 0.556670, 0.766044),
        vec3(0.219846, 0.604023, 0.766044), vec3(0.219846, 0.604023, 0.766044),
        vec3(0.250000, 0.433013, 0.866025), vec3(0.250000, 0.433013, 0.866025),
        vec3(0.171010, 0.469846, 0.866025), vec3(0.171010, 0.469846, 0.866025),
        vec3(0.171010, 0.296198, 0.939693), vec3(0.171010, 0.296198, 0.939693),
        vec3(0.116978, 0.321394, 0.939693), vec3(0.116978, 0.321394, 0.939693),
        vec3(0.086824, 0.150384, 0.984808), vec3(0.086824, 0.150384, 0.984808),
        vec3(0.059391, 0.163176, 0.984808), vec3(0.059391, 0.163176, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(0.059391, 0.163176, 0.984808), vec3(0.059391, 0.163176, 0.984808),
        vec3(0.030154, 0.171010, 0.984808), vec3(0.030154, 0.171010, 0.984808),
        vec3(0.116978, 0.321394, 0.939693), vec3(0.116978, 0.321394, 0.939693),
        vec3(0.059391, 0.336824, 0.939693), vec3(0.059391, 0.336824, 0.939693),
        vec3(0.171010, 0.469846, 0.866025), vec3(0.171010, 0.469846, 0.866025),
        vec3(0.086824, 0.492404, 0.866025), vec3(0.086824, 0.492404, 0.866025),
        vec3(0.219846, 0.604023, 0.766044), vec3(0.219846, 0.604023, 0.766044),
        vec3(0.111619, 0.633022, 0.766044), vec3(0.111619, 0.633022, 0.766044),
        vec3(0.262003, 0.719846, 0.642788), vec3(0.262003, 0.719846, 0.642788),
        vec3(0.133022, 0.754407, 0.642788), vec3(0.133022, 0.754407, 0.642788),
        vec3(0.296198, 0.813798, 0.500000), vec3(0.296198, 0.813798, 0.500000),
        vec3(0.150384, 0.852869, 0.500000), vec3(0.150384, 0.852869, 0.500000),
        vec3(0.321394, 0.883022, 0.342020), vec3(0.321394, 0.883022, 0.342020),
        vec3(0.163176, 0.925417, 0.342020), vec3(0.163176, 0.925417, 0.342020),
        vec3(0.336824, 0.925417, 0.173648), vec3(0.336824, 0.925417, 0.173648),
        vec3(0.171010, 0.969846, 0.173648), vec3(0.171010, 0.969846, 0.173648),
        vec3(0.342020, 0.939693, 0.000000), vec3(0.342020, 0.939693, 0.000000),
        vec3(0.173648, 0.984808, 0.000000), vec3(0.173648, 0.984808, 0.000000),
        vec3(0.336824, 0.925417, -0.173648), vec3(0.336824, 0.925417, -0.173648),
        vec3(0.171010, 0.969846, -0.173648), vec3(0.171010, 0.969846, -0.173648),
        vec3(0.321394, 0.883022, -0.342020), vec3(0.321394, 0.883022, -0.342020),
        vec3(0.163176, 0.925417, -0.342020), vec3(0.163176, 0.925417, -0.342020),
        vec3(0.296198, 0.813798, -0.500000), vec3(0.296198, 0.813798, -0.500000),
        vec3(0.150384, 0.852869, -0.500000), vec3(0.150384, 0.852869, -0.500000),
        vec3(0.262003, 0.719846, -0.642788), vec3(0.262003, 0.719846, -0.642788),
        vec3(0.133022, 0.754407, -0.642788), vec3(0.133022, 0.754407, -0.642788),
        vec3(0.219846, 0.604023, -0.766044), vec3(0.219846, 0.604023, -0.766044),
        vec3(0.111619, 0.633022, -0.766044), vec3(0.111619, 0.633022, -0.766044),
        vec3(0.171010, 0.469846, -0.866025), vec3(0.171010, 0.469846, -0.866025),
        vec3(0.086824, 0.492404, -0.866025), vec3(0.086824, 0.492404, -0.866025),
        vec3(0.116978, 0.321394, -0.939693), vec3(0.116978, 0.321394, -0.939693),
        vec3(0.059391, 0.336824, -0.939693), vec3(0.059391, 0.336824, -0.939693),
        vec3(0.059391, 0.163176, -0.984808), vec3(0.059391, 0.163176, -0.984808),
        vec3(0.030154, 0.171010, -0.984808), vec3(0.030154, 0.171010, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(0.030154, 0.171010, -0.984808), vec3(0.030154, 0.171010, -0.984808),
        vec3(0.000000, 0.173648, -0.984808), vec3(0.000000, 0.173648, -0.984808),
        vec3(0.059391, 0.336824, -0.939693), vec3(0.059391, 0.336824, -0.939693),
        vec3(0.000000, 0.342020, -0.939693), vec3(0.000000, 0.342020, -0.939693),
        vec3(0.086824, 0.492404, -0.866025), vec3(0.086824, 0.492404, -0.866025),
        vec3(0.000000, 0.500000, -0.866025), vec3(0.000000, 0.500000, -0.866025),
        vec3(0.111619, 0.633022, -0.766044), vec3(0.111619, 0.633022, -0.766044),
        vec3(0.000000, 0.642788, -0.766044), vec3(0.000000, 0.642788, -0.766044),
        vec3(0.133022, 0.754407, -0.642788), vec3(0.133022, 0.754407, -0.642788),
        vec3(0.000000, 0.766044, -0.642788), vec3(0.000000, 0.766044, -0.642788),
        vec3(0.150384, 0.852869, -0.500000), vec3(0.150384, 0.852869, -0.500000),
        vec3(0.000000, 0.866025, -0.500000), vec3(0.000000, 0.866025, -0.500000),
        vec3(0.163176, 0.925417, -0.342020), vec3(0.163176, 0.925417, -0.342020),
        vec3(0.000000, 0.939693, -0.342020), vec3(0.000000, 0.939693, -0.342020),
        vec3(0.171010, 0.969846, -0.173648), vec3(0.171010, 0.969846, -0.173648),
        vec3(0.000000, 0.984808, -0.173648), vec3(0.000000, 0.984808, -0.173648),
        vec3(0.173648, 0.984808, 0.000000), vec3(0.173648, 0.984808, 0.000000),
        vec3(0.000000, 1.000000, 0.000000), vec3(0.000000, 1.000000, 0.000000),
        vec3(0.171010, 0.969846, 0.173648), vec3(0.171010, 0.969846, 0.173648),
        vec3(0.000000, 0.984808, 0.173648), vec3(0.000000, 0.984808, 0.173648),
        vec3(0.163176, 0.925417, 0.342020), vec3(0.163176, 0.925417, 0.342020),
        vec3(0.000000, 0.939693, 0.342020), vec3(0.000000, 0.939693, 0.342020),
        vec3(0.150384, 0.852869, 0.500000), vec3(0.150384, 0.852869, 0.500000),
        vec3(0.000000, 0.866025, 0.500000), vec3(0.000000, 0.866025, 0.500000),
        vec3(0.133022, 0.754407, 0.642788), vec3(0.133022, 0.754407, 0.642788),
        vec3(0.000000, 0.766044, 0.642788), vec3(0.000000, 0.766044, 0.642788),
        vec3(0.111619, 0.633022, 0.766044), vec3(0.111619, 0.633022, 0.766044),
        vec3(0.000000, 0.642788, 0.766044), vec3(0.000000, 0.642788, 0.766044),
        vec3(0.086824, 0.492404, 0.866025), vec3(0.086824, 0.492404, 0.866025),
        vec3(0.000000, 0.500000, 0.866025), vec3(0.000000, 0.500000, 0.866025),
        vec3(0.059391, 0.336824, 0.939693), vec3(0.059391, 0.336824, 0.939693),
        vec3(0.000000, 0.342020, 0.939693), vec3(0.000000, 0.342020, 0.939693),
        vec3(0.030154, 0.171010, 0.984808), vec3(0.030154, 0.171010, 0.984808),
        vec3(0.000000, 0.173648, 0.984808), vec3(0.000000, 0.173648, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(0.000000, 0.173648, 0.984808), vec3(0.000000, 0.173648, 0.984808),
        vec3(-0.030154, 0.171010, 0.984808), vec3(-0.030154, 0.171010, 0.984808),
        vec3(0.000000, 0.342020, 0.939693), vec3(0.000000, 0.342020, 0.939693),
        vec3(-0.059391, 0.336824, 0.939693), vec3(-0.059391, 0.336824, 0.939693),
        vec3(0.000000, 0.500000, 0.866025), vec3(0.000000, 0.500000, 0.866025),
        vec3(-0.086824, 0.492404, 0.866025), vec3(-0.086824, 0.492404, 0.866025),
        vec3(0.000000, 0.642788, 0.766044), vec3(0.000000, 0.642788, 0.766044),
        vec3(-0.111619, 0.633022, 0.766044), vec3(-0.111619, 0.633022, 0.766044),
        vec3(0.000000, 0.766044, 0.642788), vec3(0.000000, 0.766044, 0.642788),
        vec3(-0.133022, 0.754407, 0.642788), vec3(-0.133022, 0.754407, 0.642788),
        vec3(0.000000, 0.866025, 0.500000), vec3(0.000000, 0.866025, 0.500000),
        vec3(-0.150384, 0.852869, 0.500000), vec3(-0.150384, 0.852869, 0.500000),
        vec3(0.000000, 0.939693, 0.342020), vec3(0.000000, 0.939693, 0.342020),
        vec3(-0.163176, 0.925417, 0.342020), vec3(-0.163176, 0.925417, 0.342020),
        vec3(0.000000, 0.984808, 0.173648), vec3(0.000000, 0.984808, 0.173648),
        vec3(-0.171010, 0.969846, 0.173648), vec3(-0.171010, 0.969846, 0.173648),
        vec3(0.000000, 1.000000, 0.000000), vec3(0.000000, 1.000000, 0.000000),
        vec3(-0.173648, 0.984808, 0.000000), vec3(-0.173648, 0.984808, 0.000000),
        vec3(0.000000, 0.984808, -0.173648), vec3(0.000000, 0.984808, -0.173648),
        vec3(-0.171010, 0.969846, -0.173648), vec3(-0.171010, 0.969846, -0.173648),
        vec3(0.000000, 0.939693, -0.342020), vec3(0.000000, 0.939693, -0.342020),
        vec3(-0.163176, 0.925417, -0.342020), vec3(-0.163176, 0.925417, -0.342020),
        vec3(0.000000, 0.866025, -0.500000), vec3(0.000000, 0.866025, -0.500000),
        vec3(-0.150384, 0.852869, -0.500000), vec3(-0.150384, 0.852869, -0.500000),
        vec3(0.000000, 0.766044, -0.642788), vec3(0.000000, 0.766044, -0.642788),
        vec3(-0.133022, 0.754407, -0.642788), vec3(-0.133022, 0.754407, -0.642788),
        vec3(0.000000, 0.642788, -0.766044), vec3(0.000000, 0.642788, -0.766044),
        vec3(-0.111619, 0.633022, -0.766044), vec3(-0.111619, 0.633022, -0.766044),
        vec3(0.000000, 0.500000, -0.866025), vec3(0.000000, 0.500000, -0.866025),
        vec3(-0.086824, 0.492404, -0.866025), vec3(-0.086824, 0.492404, -0.866025),
        vec3(0.000000, 0.342020, -0.939693), vec3(0.000000, 0.342020, -0.939693),
        vec3(-0.059391, 0.336824, -0.939693), vec3(-0.059391, 0.336824, -0.939693),
        vec3(0.000000, 0.173648, -0.984808), vec3(0.000000, 0.173648, -0.984808),
        vec3(-0.030154, 0.171010, -0.984808), vec3(-0.030154, 0.171010, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(-0.030154, 0.171010, -0.984808), vec3(-0.030154, 0.171010, -0.984808),
        vec3(-0.059391, 0.163176, -0.984808), vec3(-0.059391, 0.163176, -0.984808),
        vec3(-0.059391, 0.336824, -0.939693), vec3(-0.059391, 0.336824, -0.939693),
        vec3(-0.116978, 0.321394, -0.939693), vec3(-0.116978, 0.321394, -0.939693),
        vec3(-0.086824, 0.492404, -0.866025), vec3(-0.086824, 0.492404, -0.866025),
        vec3(-0.171010, 0.469846, -0.866025), vec3(-0.171010, 0.469846, -0.866025),
        vec3(-0.111619, 0.633022, -0.766044), vec3(-0.111619, 0.633022, -0.766044),
        vec3(-0.219846, 0.604023, -0.766044), vec3(-0.219846, 0.604023, -0.766044),
        vec3(-0.133022, 0.754407, -0.642788), vec3(-0.133022, 0.754407, -0.642788),
        vec3(-0.262003, 0.719846, -0.642788), vec3(-0.262003, 0.719846, -0.642788),
        vec3(-0.150384, 0.852869, -0.500000), vec3(-0.150384, 0.852869, -0.500000),
        vec3(-0.296198, 0.813798, -0.500000), vec3(-0.296198, 0.813798, -0.500000),
        vec3(-0.163176, 0.925417, -0.342020), vec3(-0.163176, 0.925417, -0.342020),
        vec3(-0.321394, 0.883022, -0.342020), vec3(-0.321394, 0.883022, -0.342020),
        vec3(-0.171010, 0.969846, -0.173648), vec3(-0.171010, 0.969846, -0.173648),
        vec3(-0.336824, 0.925417, -0.173648), vec3(-0.336824, 0.925417, -0.173648),
        vec3(-0.173648, 0.984808, 0.000000), vec3(-0.173648, 0.984808, 0.000000),
        vec3(-0.342020, 0.939693, 0.000000), vec3(-0.342020, 0.939693, 0.000000),
        vec3(-0.171010, 0.969846, 0.173648), vec3(-0.171010, 0.969846, 0.173648),
        vec3(-0.336824, 0.925417, 0.173648), vec3(-0.336824, 0.925417, 0.173648),
        vec3(-0.163176, 0.925417, 0.342020), vec3(-0.163176, 0.925417, 0.342020),
        vec3(-0.321394, 0.883022, 0.342020), vec3(-0.321394, 0.883022, 0.342020),
        vec3(-0.150384, 0.852869, 0.500000), vec3(-0.150384, 0.852869, 0.500000),
        vec3(-0.296198, 0.813798, 0.500000), vec3(-0.296198, 0.813798, 0.500000),
        vec3(-0.133022, 0.754407, 0.642788), vec3(-0.133022, 0.754407, 0.642788),
        vec3(-0.262003, 0.719846, 0.642788), vec3(-0.262003, 0.719846, 0.642788),
        vec3(-0.111619, 0.633022, 0.766044), vec3(-0.111619, 0.633022, 0.766044),
        vec3(-0.219846, 0.604023, 0.766044), vec3(-0.219846, 0.604023, 0.766044),
        vec3(-0.086824, 0.492404, 0.866025), vec3(-0.086824, 0.492404, 0.866025),
        vec3(-0.171010, 0.469846, 0.866025), vec3(-0.171010, 0.469846, 0.866025),
        vec3(-0.059391, 0.336824, 0.939693), vec3(-0.059391, 0.336824, 0.939693),
        vec3(-0.116978, 0.321394, 0.939693), vec3(-0.116978, 0.321394, 0.939693),
        vec3(-0.030154, 0.171010, 0.984808), vec3(-0.030154, 0.171010, 0.984808),
        vec3(-0.059391, 0.163176, 0.984808), vec3(-0.059391, 0.163176, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(-0.059391, 0.163176, 0.984808), vec3(-0.059391, 0.163176, 0.984808),
        vec3(-0.086824, 0.150384, 0.984808), vec3(-0.086824, 0.150384, 0.984808),
        vec3(-0.116978, 0.321394, 0.939693), vec3(-0.116978, 0.321394, 0.939693),
        vec3(-0.171010, 0.296198, 0.939693), vec3(-0.171010, 0.296198, 0.939693),
        vec3(-0.171010, 0.469846, 0.866025), vec3(-0.171010, 0.469846, 0.866025),
        vec3(-0.250000, 0.433013, 0.866025), vec3(-0.250000, 0.433013, 0.866025),
        vec3(-0.219846, 0.604023, 0.766044), vec3(-0.219846, 0.604023, 0.766044),
        vec3(-0.321394, 0.556670, 0.766044), vec3(-0.321394, 0.556670, 0.766044),
        vec3(-0.262003, 0.719846, 0.642788), vec3(-0.262003, 0.719846, 0.642788),
        vec3(-0.383022, 0.663414, 0.642788), vec3(-0.383022, 0.663414, 0.642788),
        vec3(-0.296198, 0.813798, 0.500000), vec3(-0.296198, 0.813798, 0.500000),
        vec3(-0.433013, 0.750000, 0.500000), vec3(-0.433013, 0.750000, 0.500000),
        vec3(-0.321394, 0.883022, 0.342020), vec3(-0.321394, 0.883022, 0.342020),
        vec3(-0.469846, 0.813798, 0.342020), vec3(-0.469846, 0.813798, 0.342020),
        vec3(-0.336824, 0.925417, 0.173648), vec3(-0.336824, 0.925417, 0.173648),
        vec3(-0.492404, 0.852869, 0.173648), vec3(-0.492404, 0.852869, 0.173648),
        vec3(-0.342020, 0.939693, 0.000000), vec3(-0.342020, 0.939693, 0.000000),
        vec3(-0.500000, 0.866025, 0.000000), vec3(-0.500000, 0.866025, 0.000000),
        vec3(-0.336824, 0.925417, -0.173648), vec3(-0.336824, 0.925417, -0.173648),
        vec3(-0.492404, 0.852869, -0.173648), vec3(-0.492404, 0.852869, -0.173648),
        vec3(-0.321394, 0.883022, -0.342020), vec3(-0.321394, 0.883022, -0.342020),
        vec3(-0.469846, 0.813798, -0.342020), vec3(-0.469846, 0.813798, -0.342020),
        vec3(-0.296198, 0.813798, -0.500000), vec3(-0.296198, 0.813798, -0.500000),
        vec3(-0.433013, 0.750000, -0.500000), vec3(-0.433013, 0.750000, -0.500000),
        vec3(-0.262003, 0.719846, -0.642788), vec3(-0.262003, 0.719846, -0.642788),
        vec3(-0.383022, 0.663414, -0.642788), vec3(-0.383022, 0.663414, -0.642788),
        vec3(-0.219846, 0.604023, -0.766044), vec3(-0.219846, 0.604023, -0.766044),
        vec3(-0.321394, 0.556670, -0.766044), vec3(-0.321394, 0.556670, -0.766044),
        vec3(-0.171010, 0.469846, -0.866025), vec3(-0.171010, 0.469846, -0.866025),
        vec3(-0.250000, 0.433013, -0.866025), vec3(-0.250000, 0.433013, -0.866025),
        vec3(-0.116978, 0.321394, -0.939693), vec3(-0.116978, 0.321394, -0.939693),
        vec3(-0.171010, 0.296198, -0.939693), vec3(-0.171010, 0.296198, -0.939693),
        vec3(-0.059391, 0.163176, -0.984808), vec3(-0.059391, 0.163176, -0.984808),
        vec3(-0.086824, 0.150384, -0.984808), vec3(-0.086824, 0.150384, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(-0.086824, 0.150384, -0.984808), vec3(-0.086824, 0.150384, -0.984808),
        vec3(-0.111619, 0.133022, -0.984808), vec3(-0.111619, 0.133022, -0.984808),
        vec3(-0.171010, 0.296198, -0.939693), vec3(-0.171010, 0.296198, -0.939693),
        vec3(-0.219846, 0.262003, -0.939693), vec3(-0.219846, 0.262003, -0.939693),
        vec3(-0.250000, 0.433013, -0.866025), vec3(-0.250000, 0.433013, -0.866025),
        vec3(-0.321394, 0.383022, -0.866025), vec3(-0.321394, 0.383022, -0.866025),
        vec3(-0.321394, 0.556670, -0.766044), vec3(-0.321394, 0.556670, -0.766044),
        vec3(-0.413176, 0.492404, -0.766044), vec3(-0.413176, 0.492404, -0.766045),
        vec3(-0.383022, 0.663414, -0.642788), vec3(-0.383022, 0.663414, -0.642788),
        vec3(-0.492404, 0.586824, -0.642788), vec3(-0.492404, 0.586824, -0.642788),
        vec3(-0.433013, 0.750000, -0.500000), vec3(-0.433013, 0.750000, -0.500000),
        vec3(-0.556670, 0.663414, -0.500000), vec3(-0.556670, 0.663414, -0.500000),
        vec3(-0.469846, 0.813798, -0.342020), vec3(-0.469846, 0.813798, -0.342020),
        vec3(-0.604023, 0.719846, -0.342020), vec3(-0.604023, 0.719846, -0.342020),
        vec3(-0.492404, 0.852869, -0.173648), vec3(-0.492404, 0.852869, -0.173648),
        vec3(-0.633022, 0.754407, -0.173648), vec3(-0.633022, 0.754407, -0.173648),
        vec3(-0.500000, 0.866025, 0.000000), vec3(-0.500000, 0.866025, 0.000000),
        vec3(-0.642788, 0.766044, 0.000000), vec3(-0.642788, 0.766044, 0.000000),
        vec3(-0.492404, 0.852869, 0.173648), vec3(-0.492404, 0.852869, 0.173648),
        vec3(-0.633022, 0.754407, 0.173648), vec3(-0.633022, 0.754407, 0.173648),
        vec3(-0.469846, 0.813798, 0.342020), vec3(-0.469846, 0.813798, 0.342020),
        vec3(-0.604023, 0.719846, 0.342020), vec3(-0.604023, 0.719846, 0.342020),
        vec3(-0.433013, 0.750000, 0.500000), vec3(-0.433013, 0.750000, 0.500000),
        vec3(-0.556670, 0.663414, 0.500000), vec3(-0.556670, 0.663414, 0.500000),
        vec3(-0.383022, 0.663414, 0.642788), vec3(-0.383022, 0.663414, 0.642788),
        vec3(-0.492404, 0.586824, 0.642788), vec3(-0.492404, 0.586824, 0.642788),
        vec3(-0.321394, 0.556670, 0.766044), vec3(-0.321394, 0.556670, 0.766044),
        vec3(-0.413176, 0.492404, 0.766044), vec3(-0.413176, 0.492404, 0.766045),
        vec3(-0.250000, 0.433013, 0.866025), vec3(-0.250000, 0.433013, 0.866025),
        vec3(-0.321394, 0.383022, 0.866025), vec3(-0.321394, 0.383022, 0.866025),
        vec3(-0.171010, 0.296198, 0.939693), vec3(-0.171010, 0.296198, 0.939693),
        vec3(-0.219846, 0.262003, 0.939693), vec3(-0.219846, 0.262003, 0.939693),
        vec3(-0.086824, 0.150384, 0.984808), vec3(-0.086824, 0.150384, 0.984808),
        vec3(-0.111619, 0.133022, 0.984808), vec3(-0.111619, 0.133022, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(-0.111619, 0.133022, 0.984808), vec3(-0.111619, 0.133022, 0.984808),
        vec3(-0.133022, 0.111619, 0.984808), vec3(-0.133022, 0.111619, 0.984808),
        vec3(-0.219846, 0.262003, 0.939693), vec3(-0.219846, 0.262003, 0.939693),
        vec3(-0.262003, 0.219846, 0.939693), vec3(-0.262003, 0.219846, 0.939693),
        vec3(-0.321394, 0.383022, 0.866025), vec3(-0.321394, 0.383022, 0.866025),
        vec3(-0.383022, 0.321394, 0.866025), vec3(-0.383022, 0.321394, 0.866025),
        vec3(-0.413176, 0.492404, 0.766044), vec3(-0.413176, 0.492404, 0.766045),
        vec3(-0.492404, 0.413176, 0.766044), vec3(-0.492404, 0.413176, 0.766045),
        vec3(-0.492404, 0.586824, 0.642788), vec3(-0.492404, 0.586824, 0.642788),
        vec3(-0.586824, 0.492404, 0.642788), vec3(-0.586824, 0.492404, 0.642788),
        vec3(-0.556670, 0.663414, 0.500000), vec3(-0.556670, 0.663414, 0.500000),
        vec3(-0.663414, 0.556670, 0.500000), vec3(-0.663414, 0.556670, 0.500000),
        vec3(-0.604023, 0.719846, 0.342020), vec3(-0.604023, 0.719846, 0.342020),
        vec3(-0.719846, 0.604023, 0.342020), vec3(-0.719846, 0.604023, 0.342020),
        vec3(-0.633022, 0.754407, 0.173648), vec3(-0.633022, 0.754407, 0.173648),
        vec3(-0.754407, 0.633022, 0.173648), vec3(-0.754407, 0.633022, 0.173648),
        vec3(-0.642788, 0.766044, 0.000000), vec3(-0.642788, 0.766044, 0.000000),
        vec3(-0.766044, 0.642788, 0.000000), vec3(-0.766044, 0.642788, 0.000000),
        vec3(-0.633022, 0.754407, -0.173648), vec3(-0.633022, 0.754407, -0.173648),
        vec3(-0.754407, 0.633022, -0.173648), vec3(-0.754407, 0.633022, -0.173648),
        vec3(-0.604023, 0.719846, -0.342020), vec3(-0.604023, 0.719846, -0.342020),
        vec3(-0.719846, 0.604023, -0.342020), vec3(-0.719846, 0.604023, -0.342020),
        vec3(-0.556670, 0.663414, -0.500000), vec3(-0.556670, 0.663414, -0.500000),
        vec3(-0.663414, 0.556670, -0.500000), vec3(-0.663414, 0.556670, -0.500000),
        vec3(-0.492404, 0.586824, -0.642788), vec3(-0.492404, 0.586824, -0.642788),
        vec3(-0.586824, 0.492404, -0.642788), vec3(-0.586824, 0.492404, -0.642788),
        vec3(-0.413176, 0.492404, -0.766044), vec3(-0.413176, 0.492404, -0.766045),
        vec3(-0.492404, 0.413176, -0.766044), vec3(-0.492404, 0.413176, -0.766045),
        vec3(-0.321394, 0.383022, -0.866025), vec3(-0.321394, 0.383022, -0.866025),
        vec3(-0.383022, 0.321394, -0.866025), vec3(-0.383022, 0.321394, -0.866025),
        vec3(-0.219846, 0.262003, -0.939693), vec3(-0.219846, 0.262003, -0.939693),
        vec3(-0.262003, 0.219846, -0.939693), vec3(-0.262003, 0.219846, -0.939693),
        vec3(-0.111619, 0.133022, -0.984808), vec3(-0.111619, 0.133022, -0.984808),
        vec3(-0.133022, 0.111619, -0.984808), vec3(-0.133022, 0.111619, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(-0.133022, 0.111619, -0.984808), vec3(-0.133022, 0.111619, -0.984808),
        vec3(-0.150384, 0.086824, -0.984808), vec3(-0.150384, 0.086824, -0.984808),
        vec3(-0.262003, 0.219846, -0.939693), vec3(-0.262003, 0.219846, -0.939693),
        vec3(-0.296198, 0.171010, -0.939693), vec3(-0.296198, 0.171010, -0.939693),
        vec3(-0.383022, 0.321394, -0.866025), vec3(-0.383022, 0.321394, -0.866025),
        vec3(-0.433013, 0.250000, -0.866025), vec3(-0.433013, 0.250000, -0.866025),
        vec3(-0.492404, 0.413176, -0.766044), vec3(-0.492404, 0.413176, -0.766045),
        vec3(-0.556670, 0.321394, -0.766044), vec3(-0.556670, 0.321394, -0.766044),
        vec3(-0.586824, 0.492404, -0.642788), vec3(-0.586824, 0.492404, -0.642788),
        vec3(-0.663414, 0.383022, -0.642788), vec3(-0.663414, 0.383022, -0.642788),
        vec3(-0.663414, 0.556670, -0.500000), vec3(-0.663414, 0.556670, -0.500000),
        vec3(-0.750000, 0.433013, -0.500000), vec3(-0.750000, 0.433013, -0.500000),
        vec3(-0.719846, 0.604023, -0.342020), vec3(-0.719846, 0.604023, -0.342020),
        vec3(-0.813798, 0.469846, -0.342020), vec3(-0.813798, 0.469846, -0.342020),
        vec3(-0.754407, 0.633022, -0.173648), vec3(-0.754407, 0.633022, -0.173648),
        vec3(-0.852869, 0.492404, -0.173648), vec3(-0.852869, 0.492404, -0.173648),
        vec3(-0.766044, 0.642788, 0.000000), vec3(-0.766044, 0.642788, 0.000000),
        vec3(-0.866025, 0.500000, 0.000000), vec3(-0.866025, 0.500000, 0.000000),
        vec3(-0.754407, 0.633022, 0.173648), vec3(-0.754407, 0.633022, 0.173648),
        vec3(-0.852869, 0.492404, 0.173648), vec3(-0.852869, 0.492404, 0.173648),
        vec3(-0.719846, 0.604023, 0.342020), vec3(-0.719846, 0.604023, 0.342020),
        vec3(-0.813798, 0.469846, 0.342020), vec3(-0.813798, 0.469846, 0.342020),
        vec3(-0.663414, 0.556670, 0.500000), vec3(-0.663414, 0.556670, 0.500000),
        vec3(-0.750000, 0.433013, 0.500000), vec3(-0.750000, 0.433013, 0.500000),
        vec3(-0.586824, 0.492404, 0.642788), vec3(-0.586824, 0.492404, 0.642788),
        vec3(-0.663414, 0.383022, 0.642788), vec3(-0.663414, 0.383022, 0.642788),
        vec3(-0.492404, 0.413176, 0.766044), vec3(-0.492404, 0.413176, 0.766045),
        vec3(-0.556670, 0.321394, 0.766044), vec3(-0.556670, 0.321394, 0.766044),
        vec3(-0.383022, 0.321394, 0.866025), vec3(-0.383022, 0.321394, 0.866025),
        vec3(-0.433013, 0.250000, 0.866025), vec3(-0.433013, 0.250000, 0.866025),
        vec3(-0.262003, 0.219846, 0.939693), vec3(-0.262003, 0.219846, 0.939693),
        vec3(-0.296198, 0.171010, 0.939693), vec3(-0.296198, 0.171010, 0.939693),
        vec3(-0.133022, 0.111619, 0.984808), vec3(-0.133022, 0.111619, 0.984808),
        vec3(-0.150384, 0.086824, 0.984808), vec3(-0.150384, 0.086824, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(-0.150384, 0.086824, 0.984808), vec3(-0.150384, 0.086824, 0.984808),
        vec3(-0.163176, 0.059391, 0.984808), vec3(-0.163176, 0.059391, 0.984808),
        vec3(-0.296198, 0.171010, 0.939693), vec3(-0.296198, 0.171010, 0.939693),
        vec3(-0.321394, 0.116978, 0.939693), vec3(-0.321394, 0.116978, 0.939693),
        vec3(-0.433013, 0.250000, 0.866025), vec3(-0.433013, 0.250000, 0.866025),
        vec3(-0.469846, 0.171010, 0.866025), vec3(-0.469846, 0.171010, 0.866025),
        vec3(-0.556670, 0.321394, 0.766044), vec3(-0.556670, 0.321394, 0.766044),
        vec3(-0.604023, 0.219846, 0.766044), vec3(-0.604023, 0.219846, 0.766044),
        vec3(-0.663414, 0.383022, 0.642788), vec3(-0.663414, 0.383022, 0.642788),
        vec3(-0.719846, 0.262003, 0.642788), vec3(-0.719846, 0.262003, 0.642788),
        vec3(-0.750000, 0.433013, 0.500000), vec3(-0.750000, 0.433013, 0.500000),
        vec3(-0.813798, 0.296198, 0.500000), vec3(-0.813798, 0.296198, 0.500000),
        vec3(-0.813798, 0.469846, 0.342020), vec3(-0.813798, 0.469846, 0.342020),
        vec3(-0.883022, 0.321394, 0.342020), vec3(-0.883022, 0.321394, 0.342020),
        vec3(-0.852869, 0.492404, 0.173648), vec3(-0.852869, 0.492404, 0.173648),
        vec3(-0.925417, 0.336824, 0.173648), vec3(-0.925417, 0.336824, 0.173648),
        vec3(-0.866025, 0.500000, 0.000000), vec3(-0.866025, 0.500000, 0.000000),
        vec3(-0.939693, 0.342020, 0.000000), vec3(-0.939693, 0.342020, 0.000000),
        vec3(-0.852869, 0.492404, -0.173648), vec3(-0.852869, 0.492404, -0.173648),
        vec3(-0.925417, 0.336824, -0.173648), vec3(-0.925417, 0.336824, -0.173648),
        vec3(-0.813798, 0.469846, -0.342020), vec3(-0.813798, 0.469846, -0.342020),
        vec3(-0.883022, 0.321394, -0.342020), vec3(-0.883022, 0.321394, -0.342020),
        vec3(-0.750000, 0.433013, -0.500000), vec3(-0.750000, 0.433013, -0.500000),
        vec3(-0.813798, 0.296198, -0.500000), vec3(-0.813798, 0.296198, -0.500000),
        vec3(-0.663414, 0.383022, -0.642788), vec3(-0.663414, 0.383022, -0.642788),
        vec3(-0.719846, 0.262003, -0.642788), vec3(-0.719846, 0.262003, -0.642788),
        vec3(-0.556670, 0.321394, -0.766044), vec3(-0.556670, 0.321394, -0.766044),
        vec3(-0.604023, 0.219846, -0.766044), vec3(-0.604023, 0.219846, -0.766044),
        vec3(-0.433013, 0.250000, -0.866025), vec3(-0.433013, 0.250000, -0.866025),
        vec3(-0.469846, 0.171010, -0.866025), vec3(-0.469846, 0.171010, -0.866025),
        vec3(-0.296198, 0.171010, -0.939693), vec3(-0.296198, 0.171010, -0.939693),
        vec3(-0.321394, 0.116978, -0.939693), vec3(-0.321394, 0.116978, -0.939693),
        vec3(-0.150384, 0.086824, -0.984808), vec3(-0.150384, 0.086824, -0.984808),
        vec3(-0.163176, 0.059391, -0.984808), vec3(-0.163176, 0.059391, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(-0.163176, 0.059391, -0.984808), vec3(-0.163176, 0.059391, -0.984808),
        vec3(-0.171010, 0.030154, -0.984808), vec3(-0.171010, 0.030154, -0.984808),
        vec3(-0.321394, 0.116978, -0.939693), vec3(-0.321394, 0.116978, -0.939693),
        vec3(-0.336824, 0.059391, -0.939693), vec3(-0.336824, 0.059391, -0.939693),
        vec3(-0.469846, 0.171010, -0.866025), vec3(-0.469846, 0.171010, -0.866025),
        vec3(-0.492404, 0.086824, -0.866025), vec3(-0.492404, 0.086824, -0.866025),
        vec3(-0.604023, 0.219846, -0.766044), vec3(-0.604023, 0.219846, -0.766044),
        vec3(-0.633022, 0.111619, -0.766044), vec3(-0.633022, 0.111619, -0.766044),
        vec3(-0.719846, 0.262003, -0.642788), vec3(-0.719846, 0.262003, -0.642788),
        vec3(-0.754407, 0.133022, -0.642788), vec3(-0.754407, 0.133022, -0.642788),
        vec3(-0.813798, 0.296198, -0.500000), vec3(-0.813798, 0.296198, -0.500000),
        vec3(-0.852869, 0.150384, -0.500000), vec3(-0.852869, 0.150384, -0.500000),
        vec3(-0.883022, 0.321394, -0.342020), vec3(-0.883022, 0.321394, -0.342020),
        vec3(-0.925417, 0.163176, -0.342020), vec3(-0.925417, 0.163176, -0.342020),
        vec3(-0.925417, 0.336824, -0.173648), vec3(-0.925417, 0.336824, -0.173648),
        vec3(-0.969846, 0.171010, -0.173648), vec3(-0.969846, 0.171010, -0.173648),
        vec3(-0.939693, 0.342020, 0.000000), vec3(-0.939693, 0.342020, 0.000000),
        vec3(-0.984808, 0.173648, 0.000000), vec3(-0.984808, 0.173648, 0.000000),
        vec3(-0.925417, 0.336824, 0.173648), vec3(-0.925417, 0.336824, 0.173648),
        vec3(-0.969846, 0.171010, 0.173648), vec3(-0.969846, 0.171010, 0.173648),
        vec3(-0.883022, 0.321394, 0.342020), vec3(-0.883022, 0.321394, 0.342020),
        vec3(-0.925417, 0.163176, 0.342020), vec3(-0.925417, 0.163176, 0.342020),
        vec3(-0.813798, 0.296198, 0.500000), vec3(-0.813798, 0.296198, 0.500000),
        vec3(-0.852869, 0.150384, 0.500000), vec3(-0.852869, 0.150384, 0.500000),
        vec3(-0.719846, 0.262003, 0.642788), vec3(-0.719846, 0.262003, 0.642788),
        vec3(-0.754407, 0.133022, 0.642788), vec3(-0.754407, 0.133022, 0.642788),
        vec3(-0.604023, 0.219846, 0.766044), vec3(-0.604023, 0.219846, 0.766044),
        vec3(-0.633022, 0.111619, 0.766044), vec3(-0.633022, 0.111619, 0.766044),
        vec3(-0.469846, 0.171010, 0.866025), vec3(-0.469846, 0.171010, 0.866025),
        vec3(-0.492404, 0.086824, 0.866025), vec3(-0.492404, 0.086824, 0.866025),
        vec3(-0.321394, 0.116978, 0.939693), vec3(-0.321394, 0.116978, 0.939693),
        vec3(-0.336824, 0.059391, 0.939693), vec3(-0.336824, 0.059391, 0.939693),
        vec3(-0.163176, 0.059391, 0.984808), vec3(-0.163176, 0.059391, 0.984808),
        vec3(-0.171010, 0.030154, 0.984808), vec3(-0.171010, 0.030154, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(-0.171010, 0.030154, 0.984808), vec3(-0.171010, 0.030154, 0.984808),
        vec3(-0.173648, 0.000000, 0.984808), vec3(-0.173648, 0.000000, 0.984808),
        vec3(-0.336824, 0.059391, 0.939693), vec3(-0.336824, 0.059391, 0.939693),
        vec3(-0.342020, 0.000000, 0.939693), vec3(-0.342020, 0.000000, 0.939693),
        vec3(-0.492404, 0.086824, 0.866025), vec3(-0.492404, 0.086824, 0.866025),
        vec3(-0.500000, 0.000000, 0.866025), vec3(-0.500000, 0.000000, 0.866025),
        vec3(-0.633022, 0.111619, 0.766044), vec3(-0.633022, 0.111619, 0.766044),
        vec3(-0.642788, 0.000000, 0.766044), vec3(-0.642788, 0.000000, 0.766044),
        vec3(-0.754407, 0.133022, 0.642788), vec3(-0.754407, 0.133022, 0.642788),
        vec3(-0.766044, 0.000000, 0.642788), vec3(-0.766044, 0.000000, 0.642788),
        vec3(-0.852869, 0.150384, 0.500000), vec3(-0.852869, 0.150384, 0.500000),
        vec3(-0.866025, 0.000000, 0.500000), vec3(-0.866025, 0.000000, 0.500000),
        vec3(-0.925417, 0.163176, 0.342020), vec3(-0.925417, 0.163176, 0.342020),
        vec3(-0.939693, 0.000000, 0.342020), vec3(-0.939693, 0.000000, 0.342020),
        vec3(-0.969846, 0.171010, 0.173648), vec3(-0.969846, 0.171010, 0.173648),
        vec3(-0.984808, 0.000000, 0.173648), vec3(-0.984808, 0.000000, 0.173648),
        vec3(-0.984808, 0.173648, 0.000000), vec3(-0.984808, 0.173648, 0.000000),
        vec3(-1.000000, 0.000000, 0.000000), vec3(-1.000000, 0.000000, 0.000000),
        vec3(-0.969846, 0.171010, -0.173648), vec3(-0.969846, 0.171010, -0.173648),
        vec3(-0.984808, 0.000000, -0.173648), vec3(-0.984808, 0.000000, -0.173648),
        vec3(-0.925417, 0.163176, -0.342020), vec3(-0.925417, 0.163176, -0.342020),
        vec3(-0.939693, 0.000000, -0.342020), vec3(-0.939693, 0.000000, -0.342020),
        vec3(-0.852869, 0.150384, -0.500000), vec3(-0.852869, 0.150384, -0.500000),
        vec3(-0.866025, 0.000000, -0.500000), vec3(-0.866025, 0.000000, -0.500000),
        vec3(-0.754407, 0.133022, -0.642788), vec3(-0.754407, 0.133022, -0.642788),
        vec3(-0.766044, 0.000000, -0.642788), vec3(-0.766044, 0.000000, -0.642788),
        vec3(-0.633022, 0.111619, -0.766044), vec3(-0.633022, 0.111619, -0.766044),
        vec3(-0.642788, 0.000000, -0.766044), vec3(-0.642788, 0.000000, -0.766044),
        vec3(-0.492404, 0.086824, -0.866025), vec3(-0.492404, 0.086824, -0.866025),
        vec3(-0.500000, 0.000000, -0.866025), vec3(-0.500000, 0.000000, -0.866025),
        vec3(-0.336824, 0.059391, -0.939693), vec3(-0.336824, 0.059391, -0.939693),
        vec3(-0.342020, 0.000000, -0.939693), vec3(-0.342020, 0.000000, -0.939693),
        vec3(-0.171010, 0.030154, -0.984808), vec3(-0.171010, 0.030154, -0.984808),
        vec3(-0.173648, 0.000000, -0.984808), vec3(-0.173648, 0.000000, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(-0.173648, 0.000000, -0.984808), vec3(-0.173648, 0.000000, -0.984808),
        vec3(-0.171010, -0.030154, -0.984808), vec3(-0.171010, -0.030154, -0.984808),
        vec3(-0.342020, 0.000000, -0.939693), vec3(-0.342020, 0.000000, -0.939693),
        vec3(-0.336824, -0.059391, -0.939693), vec3(-0.336824, -0.059391, -0.939693),
        vec3(-0.500000, 0.000000, -0.866025), vec3(-0.500000, 0.000000, -0.866025),
        vec3(-0.492404, -0.086824, -0.866025), vec3(-0.492404, -0.086824, -0.866025),
        vec3(-0.642788, 0.000000, -0.766044), vec3(-0.642788, 0.000000, -0.766044),
        vec3(-0.633022, -0.111619, -0.766044), vec3(-0.633022, -0.111619, -0.766044),
        vec3(-0.766044, 0.000000, -0.642788), vec3(-0.766044, 0.000000, -0.642788),
        vec3(-0.754407, -0.133022, -0.642788), vec3(-0.754407, -0.133022, -0.642788),
        vec3(-0.866025, 0.000000, -0.500000), vec3(-0.866025, 0.000000, -0.500000),
        vec3(-0.852869, -0.150384, -0.500000), vec3(-0.852869, -0.150384, -0.500000),
        vec3(-0.939693, 0.000000, -0.342020), vec3(-0.939693, 0.000000, -0.342020),
        vec3(-0.925417, -0.163176, -0.342020), vec3(-0.925417, -0.163176, -0.342020),
        vec3(-0.984808, 0.000000, -0.173648), vec3(-0.984808, 0.000000, -0.173648),
        vec3(-0.969846, -0.171010, -0.173648), vec3(-0.969846, -0.171010, -0.173648),
        vec3(-1.000000, 0.000000, 0.000000), vec3(-1.000000, 0.000000, 0.000000),
        vec3(-0.984808, -0.173648, 0.000000), vec3(-0.984808, -0.173648, 0.000000),
        vec3(-0.984808, 0.000000, 0.173648), vec3(-0.984808, 0.000000, 0.173648),
        vec3(-0.969846, -0.171010, 0.173648), vec3(-0.969846, -0.171010, 0.173648),
        vec3(-0.939693, 0.000000, 0.342020), vec3(-0.939693, 0.000000, 0.342020),
        vec3(-0.925417, -0.163176, 0.342020), vec3(-0.925417, -0.163176, 0.342020),
        vec3(-0.866025, 0.000000, 0.500000), vec3(-0.866025, 0.000000, 0.500000),
        vec3(-0.852869, -0.150384, 0.500000), vec3(-0.852869, -0.150384, 0.500000),
        vec3(-0.766044, 0.000000, 0.642788), vec3(-0.766044, 0.000000, 0.642788),
        vec3(-0.754407, -0.133022, 0.642788), vec3(-0.754407, -0.133022, 0.642788),
        vec3(-0.642788, 0.000000, 0.766044), vec3(-0.642788, 0.000000, 0.766044),
        vec3(-0.633022, -0.111619, 0.766044), vec3(-0.633022, -0.111619, 0.766044),
        vec3(-0.500000, 0.000000, 0.866025), vec3(-0.500000, 0.000000, 0.866025),
        vec3(-0.492404, -0.086824, 0.866025), vec3(-0.492404, -0.086824, 0.866025),
        vec3(-0.342020, 0.000000, 0.939693), vec3(-0.342020, 0.000000, 0.939693),
        vec3(-0.336824, -0.059391, 0.939693), vec3(-0.336824, -0.059391, 0.939693),
        vec3(-0.173648, 0.000000, 0.984808), vec3(-0.173648, 0.000000, 0.984808),
        vec3(-0.171010, -0.030154, 0.984808), vec3(-0.171010, -0.030154, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(-0.171010, -0.030154, 0.984808), vec3(-0.171010, -0.030154, 0.984808),
        vec3(-0.163176, -0.059391, 0.984808), vec3(-0.163176, -0.059391, 0.984808),
        vec3(-0.336824, -0.059391, 0.939693), vec3(-0.336824, -0.059391, 0.939693),
        vec3(-0.321394, -0.116978, 0.939693), vec3(-0.321394, -0.116978, 0.939693),
        vec3(-0.492404, -0.086824, 0.866025), vec3(-0.492404, -0.086824, 0.866025),
        vec3(-0.469846, -0.171010, 0.866025), vec3(-0.469846, -0.171010, 0.866025),
        vec3(-0.633022, -0.111619, 0.766044), vec3(-0.633022, -0.111619, 0.766044),
        vec3(-0.604023, -0.219846, 0.766044), vec3(-0.604023, -0.219846, 0.766044),
        vec3(-0.754407, -0.133022, 0.642788), vec3(-0.754407, -0.133022, 0.642788),
        vec3(-0.719846, -0.262003, 0.642788), vec3(-0.719846, -0.262003, 0.642788),
        vec3(-0.852869, -0.150384, 0.500000), vec3(-0.852869, -0.150384, 0.500000),
        vec3(-0.813798, -0.296198, 0.500000), vec3(-0.813798, -0.296198, 0.500000),
        vec3(-0.925417, -0.163176, 0.342020), vec3(-0.925417, -0.163176, 0.342020),
        vec3(-0.883022, -0.321394, 0.342020), vec3(-0.883022, -0.321394, 0.342020),
        vec3(-0.969846, -0.171010, 0.173648), vec3(-0.969846, -0.171010, 0.173648),
        vec3(-0.925417, -0.336824, 0.173648), vec3(-0.925417, -0.336824, 0.173648),
        vec3(-0.984808, -0.173648, 0.000000), vec3(-0.984808, -0.173648, 0.000000),
        vec3(-0.939693, -0.342020, 0.000000), vec3(-0.939693, -0.342020, 0.000000),
        vec3(-0.969846, -0.171010, -0.173648), vec3(-0.969846, -0.171010, -0.173648),
        vec3(-0.925417, -0.336824, -0.173648), vec3(-0.925417, -0.336824, -0.173648),
        vec3(-0.925417, -0.163176, -0.342020), vec3(-0.925417, -0.163176, -0.342020),
        vec3(-0.883022, -0.321394, -0.342020), vec3(-0.883022, -0.321394, -0.342020),
        vec3(-0.852869, -0.150384, -0.500000), vec3(-0.852869, -0.150384, -0.500000),
        vec3(-0.813798, -0.296198, -0.500000), vec3(-0.813798, -0.296198, -0.500000),
        vec3(-0.754407, -0.133022, -0.642788), vec3(-0.754407, -0.133022, -0.642788),
        vec3(-0.719846, -0.262003, -0.642788), vec3(-0.719846, -0.262003, -0.642788),
        vec3(-0.633022, -0.111619, -0.766044), vec3(-0.633022, -0.111619, -0.766044),
        vec3(-0.604023, -0.219846, -0.766044), vec3(-0.604023, -0.219846, -0.766044),
        vec3(-0.492404, -0.086824, -0.866025), vec3(-0.492404, -0.086824, -0.866025),
        vec3(-0.469846, -0.171010, -0.866025), vec3(-0.469846, -0.171010, -0.866025),
        vec3(-0.336824, -0.059391, -0.939693), vec3(-0.336824, -0.059391, -0.939693),
        vec3(-0.321394, -0.116978, -0.939693), vec3(-0.321394, -0.116978, -0.939693),
        vec3(-0.171010, -0.030154, -0.984808), vec3(-0.171010, -0.030154, -0.984808),
        vec3(-0.163176, -0.059391, -0.984808), vec3(-0.163176, -0.059391, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(-0.163176, -0.059391, -0.984808), vec3(-0.163176, -0.059391, -0.984808),
        vec3(-0.150384, -0.086824, -0.984808), vec3(-0.150384, -0.086824, -0.984808),
        vec3(-0.321394, -0.116978, -0.939693), vec3(-0.321394, -0.116978, -0.939693),
        vec3(-0.296198, -0.171010, -0.939693), vec3(-0.296198, -0.171010, -0.939693),
        vec3(-0.469846, -0.171010, -0.866025), vec3(-0.469846, -0.171010, -0.866025),
        vec3(-0.433013, -0.250000, -0.866025), vec3(-0.433013, -0.250000, -0.866025),
        vec3(-0.604023, -0.219846, -0.766044), vec3(-0.604023, -0.219846, -0.766044),
        vec3(-0.556670, -0.321394, -0.766044), vec3(-0.556670, -0.321394, -0.766044),
        vec3(-0.719846, -0.262003, -0.642788), vec3(-0.719846, -0.262003, -0.642788),
        vec3(-0.663414, -0.383022, -0.642788), vec3(-0.663414, -0.383022, -0.642788),
        vec3(-0.813798, -0.296198, -0.500000), vec3(-0.813798, -0.296198, -0.500000),
        vec3(-0.750000, -0.433013, -0.500000), vec3(-0.750000, -0.433013, -0.500000),
        vec3(-0.883022, -0.321394, -0.342020), vec3(-0.883022, -0.321394, -0.342020),
        vec3(-0.813798, -0.469846, -0.342020), vec3(-0.813798, -0.469846, -0.342020),
        vec3(-0.925417, -0.336824, -0.173648), vec3(-0.925417, -0.336824, -0.173648),
        vec3(-0.852869, -0.492404, -0.173648), vec3(-0.852869, -0.492404, -0.173648),
        vec3(-0.939693, -0.342020, 0.000000), vec3(-0.939693, -0.342020, 0.000000),
        vec3(-0.866025, -0.500000, 0.000000), vec3(-0.866025, -0.500000, 0.000000),
        vec3(-0.925417, -0.336824, 0.173648), vec3(-0.925417, -0.336824, 0.173648),
        vec3(-0.852869, -0.492404, 0.173648), vec3(-0.852869, -0.492404, 0.173648),
        vec3(-0.883022, -0.321394, 0.342020), vec3(-0.883022, -0.321394, 0.342020),
        vec3(-0.813798, -0.469846, 0.342020), vec3(-0.813798, -0.469846, 0.342020),
        vec3(-0.813798, -0.296198, 0.500000), vec3(-0.813798, -0.296198, 0.500000),
        vec3(-0.750000, -0.433013, 0.500000), vec3(-0.750000, -0.433013, 0.500000),
        vec3(-0.719846, -0.262003, 0.642788), vec3(-0.719846, -0.262003, 0.642788),
        vec3(-0.663414, -0.383022, 0.642788), vec3(-0.663414, -0.383022, 0.642788),
        vec3(-0.604023, -0.219846, 0.766044), vec3(-0.604023, -0.219846, 0.766044),
        vec3(-0.556670, -0.321394, 0.766044), vec3(-0.556670, -0.321394, 0.766044),
        vec3(-0.469846, -0.171010, 0.866025), vec3(-0.469846, -0.171010, 0.866025),
        vec3(-0.433013, -0.250000, 0.866025), vec3(-0.433013, -0.250000, 0.866025),
        vec3(-0.321394, -0.116978, 0.939693), vec3(-0.321394, -0.116978, 0.939693),
        vec3(-0.296198, -0.171010, 0.939693), vec3(-0.296198, -0.171010, 0.939693),
        vec3(-0.163176, -0.059391, 0.984808), vec3(-0.163176, -0.059391, 0.984808),
        vec3(-0.150384, -0.086824, 0.984808), vec3(-0.150384, -0.086824, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(-0.150384, -0.086824, 0.984808), vec3(-0.150384, -0.086824, 0.984808),
        vec3(-0.133022, -0.111619, 0.984808), vec3(-0.133022, -0.111619, 0.984808),
        vec3(-0.296198, -0.171010, 0.939693), vec3(-0.296198, -0.171010, 0.939693),
        vec3(-0.262003, -0.219846, 0.939693), vec3(-0.262003, -0.219846, 0.939693),
        vec3(-0.433013, -0.250000, 0.866025), vec3(-0.433013, -0.250000, 0.866025),
        vec3(-0.383022, -0.321394, 0.866025), vec3(-0.383022, -0.321394, 0.866025),
        vec3(-0.556670, -0.321394, 0.766044), vec3(-0.556670, -0.321394, 0.766044),
        vec3(-0.492404, -0.413176, 0.766044), vec3(-0.492404, -0.413176, 0.766045),
        vec3(-0.663414, -0.383022, 0.642788), vec3(-0.663414, -0.383022, 0.642788),
        vec3(-0.586824, -0.492404, 0.642788), vec3(-0.586824, -0.492404, 0.642788),
        vec3(-0.750000, -0.433013, 0.500000), vec3(-0.750000, -0.433013, 0.500000),
        vec3(-0.663414, -0.556670, 0.500000), vec3(-0.663414, -0.556670, 0.500000),
        vec3(-0.813798, -0.469846, 0.342020), vec3(-0.813798, -0.469846, 0.342020),
        vec3(-0.719846, -0.604023, 0.342020), vec3(-0.719846, -0.604023, 0.342020),
        vec3(-0.852869, -0.492404, 0.173648), vec3(-0.852869, -0.492404, 0.173648),
        vec3(-0.754407, -0.633022, 0.173648), vec3(-0.754407, -0.633022, 0.173648),
        vec3(-0.866025, -0.500000, 0.000000), vec3(-0.866025, -0.500000, 0.000000),
        vec3(-0.766044, -0.642788, 0.000000), vec3(-0.766044, -0.642788, 0.000000),
        vec3(-0.852869, -0.492404, -0.173648), vec3(-0.852869, -0.492404, -0.173648),
        vec3(-0.754407, -0.633022, -0.173648), vec3(-0.754407, -0.633022, -0.173648),
        vec3(-0.813798, -0.469846, -0.342020), vec3(-0.813798, -0.469846, -0.342020),
        vec3(-0.719846, -0.604023, -0.342020), vec3(-0.719846, -0.604023, -0.342020),
        vec3(-0.750000, -0.433013, -0.500000), vec3(-0.750000, -0.433013, -0.500000),
        vec3(-0.663414, -0.556670, -0.500000), vec3(-0.663414, -0.556670, -0.500000),
        vec3(-0.663414, -0.383022, -0.642788), vec3(-0.663414, -0.383022, -0.642788),
        vec3(-0.586824, -0.492404, -0.642788), vec3(-0.586824, -0.492404, -0.642788),
        vec3(-0.556670, -0.321394, -0.766044), vec3(-0.556670, -0.321394, -0.766044),
        vec3(-0.492404, -0.413176, -0.766044), vec3(-0.492404, -0.413176, -0.766045),
        vec3(-0.433013, -0.250000, -0.866025), vec3(-0.433013, -0.250000, -0.866025),
        vec3(-0.383022, -0.321394, -0.866025), vec3(-0.383022, -0.321394, -0.866025),
        vec3(-0.296198, -0.171010, -0.939693), vec3(-0.296198, -0.171010, -0.939693),
        vec3(-0.262003, -0.219846, -0.939693), vec3(-0.262003, -0.219846, -0.939693),
        vec3(-0.150384, -0.086824, -0.984808), vec3(-0.150384, -0.086824, -0.984808),
        vec3(-0.133022, -0.111619, -0.984808), vec3(-0.133022, -0.111619, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(-0.133022, -0.111619, -0.984808), vec3(-0.133022, -0.111619, -0.984808),
        vec3(-0.111619, -0.133022, -0.984808), vec3(-0.111619, -0.133022, -0.984808),
        vec3(-0.262003, -0.219846, -0.939693), vec3(-0.262003, -0.219846, -0.939693),
        vec3(-0.219846, -0.262003, -0.939693), vec3(-0.219846, -0.262003, -0.939693),
        vec3(-0.383022, -0.321394, -0.866025), vec3(-0.383022, -0.321394, -0.866025),
        vec3(-0.321394, -0.383022, -0.866025), vec3(-0.321394, -0.383022, -0.866025),
        vec3(-0.492404, -0.413176, -0.766044), vec3(-0.492404, -0.413176, -0.766045),
        vec3(-0.413176, -0.492404, -0.766044), vec3(-0.413176, -0.492404, -0.766045),
        vec3(-0.586824, -0.492404, -0.642788), vec3(-0.586824, -0.492404, -0.642788),
        vec3(-0.492404, -0.586824, -0.642788), vec3(-0.492404, -0.586824, -0.642788),
        vec3(-0.663414, -0.556670, -0.500000), vec3(-0.663414, -0.556670, -0.500000),
        vec3(-0.556670, -0.663414, -0.500000), vec3(-0.556670, -0.663414, -0.500000),
        vec3(-0.719846, -0.604023, -0.342020), vec3(-0.719846, -0.604023, -0.342020),
        vec3(-0.604023, -0.719846, -0.342020), vec3(-0.604023, -0.719846, -0.342020),
        vec3(-0.754407, -0.633022, -0.173648), vec3(-0.754407, -0.633022, -0.173648),
        vec3(-0.633022, -0.754407, -0.173648), vec3(-0.633022, -0.754407, -0.173648),
        vec3(-0.766044, -0.642788, 0.000000), vec3(-0.766044, -0.642788, 0.000000),
        vec3(-0.642788, -0.766044, 0.000000), vec3(-0.642788, -0.766044, 0.000000),
        vec3(-0.754407, -0.633022, 0.173648), vec3(-0.754407, -0.633022, 0.173648),
        vec3(-0.633022, -0.754407, 0.173648), vec3(-0.633022, -0.754407, 0.173648),
        vec3(-0.719846, -0.604023, 0.342020), vec3(-0.719846, -0.604023, 0.342020),
        vec3(-0.604023, -0.719846, 0.342020), vec3(-0.604023, -0.719846, 0.342020),
        vec3(-0.663414, -0.556670, 0.500000), vec3(-0.663414, -0.556670, 0.500000),
        vec3(-0.556670, -0.663414, 0.500000), vec3(-0.556670, -0.663414, 0.500000),
        vec3(-0.586824, -0.492404, 0.642788), vec3(-0.586824, -0.492404, 0.642788),
        vec3(-0.492404, -0.586824, 0.642788), vec3(-0.492404, -0.586824, 0.642788),
        vec3(-0.492404, -0.413176, 0.766044), vec3(-0.492404, -0.413176, 0.766045),
        vec3(-0.413176, -0.492404, 0.766044), vec3(-0.413176, -0.492404, 0.766045),
        vec3(-0.383022, -0.321394, 0.866025), vec3(-0.383022, -0.321394, 0.866025),
        vec3(-0.321394, -0.383022, 0.866025), vec3(-0.321394, -0.383022, 0.866025),
        vec3(-0.262003, -0.219846, 0.939693), vec3(-0.262003, -0.219846, 0.939693),
        vec3(-0.219846, -0.262003, 0.939693), vec3(-0.219846, -0.262003, 0.939693),
        vec3(-0.133022, -0.111619, 0.984808), vec3(-0.133022, -0.111619, 0.984808),
        vec3(-0.111619, -0.133022, 0.984808), vec3(-0.111619, -0.133022, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(-0.111619, -0.133022, 0.984808), vec3(-0.111619, -0.133022, 0.984808),
        vec3(-0.086824, -0.150384, 0.984808), vec3(-0.086824, -0.150384, 0.984808),
        vec3(-0.219846, -0.262003, 0.939693), vec3(-0.219846, -0.262003, 0.939693),
        vec3(-0.171010, -0.296198, 0.939693), vec3(-0.171010, -0.296198, 0.939693),
        vec3(-0.321394, -0.383022, 0.866025), vec3(-0.321394, -0.383022, 0.866025),
        vec3(-0.250000, -0.433013, 0.866025), vec3(-0.250000, -0.433013, 0.866025),
        vec3(-0.413176, -0.492404, 0.766044), vec3(-0.413176, -0.492404, 0.766045),
        vec3(-0.321394, -0.556670, 0.766044), vec3(-0.321394, -0.556670, 0.766044),
        vec3(-0.492404, -0.586824, 0.642788), vec3(-0.492404, -0.586824, 0.642788),
        vec3(-0.383022, -0.663414, 0.642788), vec3(-0.383022, -0.663414, 0.642788),
        vec3(-0.556670, -0.663414, 0.500000), vec3(-0.556670, -0.663414, 0.500000),
        vec3(-0.433013, -0.750000, 0.500000), vec3(-0.433013, -0.750000, 0.500000),
        vec3(-0.604023, -0.719846, 0.342020), vec3(-0.604023, -0.719846, 0.342020),
        vec3(-0.469846, -0.813798, 0.342020), vec3(-0.469846, -0.813798, 0.342020),
        vec3(-0.633022, -0.754407, 0.173648), vec3(-0.633022, -0.754407, 0.173648),
        vec3(-0.492404, -0.852869, 0.173648), vec3(-0.492404, -0.852869, 0.173648),
        vec3(-0.642788, -0.766044, 0.000000), vec3(-0.642788, -0.766044, 0.000000),
        vec3(-0.500000, -0.866025, 0.000000), vec3(-0.500000, -0.866025, 0.000000),
        vec3(-0.633022, -0.754407, -0.173648), vec3(-0.633022, -0.754407, -0.173648),
        vec3(-0.492404, -0.852869, -0.173648), vec3(-0.492404, -0.852869, -0.173648),
        vec3(-0.604023, -0.719846, -0.342020), vec3(-0.604023, -0.719846, -0.342020),
        vec3(-0.469846, -0.813798, -0.342020), vec3(-0.469846, -0.813798, -0.342020),
        vec3(-0.556670, -0.663414, -0.500000), vec3(-0.556670, -0.663414, -0.500000),
        vec3(-0.433013, -0.750000, -0.500000), vec3(-0.433013, -0.750000, -0.500000),
        vec3(-0.492404, -0.586824, -0.642788), vec3(-0.492404, -0.586824, -0.642788),
        vec3(-0.383022, -0.663414, -0.642788), vec3(-0.383022, -0.663414, -0.642788),
        vec3(-0.413176, -0.492404, -0.766044), vec3(-0.413176, -0.492404, -0.766045),
        vec3(-0.321394, -0.556670, -0.766044), vec3(-0.321394, -0.556670, -0.766044),
        vec3(-0.321394, -0.383022, -0.866025), vec3(-0.321394, -0.383022, -0.866025),
        vec3(-0.250000, -0.433013, -0.866025), vec3(-0.250000, -0.433013, -0.866025),
        vec3(-0.219846, -0.262003, -0.939693), vec3(-0.219846, -0.262003, -0.939693),
        vec3(-0.171010, -0.296198, -0.939693), vec3(-0.171010, -0.296198, -0.939693),
        vec3(-0.111619, -0.133022, -0.984808), vec3(-0.111619, -0.133022, -0.984808),
        vec3(-0.086824, -0.150384, -0.984808), vec3(-0.086824, -0.150384, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(-0.086824, -0.150384, -0.984808), vec3(-0.086824, -0.150384, -0.984808),
        vec3(-0.059391, -0.163176, -0.984808), vec3(-0.059391, -0.163176, -0.984808),
        vec3(-0.171010, -0.296198, -0.939693), vec3(-0.171010, -0.296198, -0.939693),
        vec3(-0.116978, -0.321394, -0.939693), vec3(-0.116978, -0.321394, -0.939693),
        vec3(-0.250000, -0.433013, -0.866025), vec3(-0.250000, -0.433013, -0.866025),
        vec3(-0.171010, -0.469846, -0.866025), vec3(-0.171010, -0.469846, -0.866025),
        vec3(-0.321394, -0.556670, -0.766044), vec3(-0.321394, -0.556670, -0.766044),
        vec3(-0.219846, -0.604023, -0.766044), vec3(-0.219846, -0.604023, -0.766044),
        vec3(-0.383022, -0.663414, -0.642788), vec3(-0.383022, -0.663414, -0.642788),
        vec3(-0.262003, -0.719846, -0.642788), vec3(-0.262003, -0.719846, -0.642788),
        vec3(-0.433013, -0.750000, -0.500000), vec3(-0.433013, -0.750000, -0.500000),
        vec3(-0.296198, -0.813798, -0.500000), vec3(-0.296198, -0.813798, -0.500000),
        vec3(-0.469846, -0.813798, -0.342020), vec3(-0.469846, -0.813798, -0.342020),
        vec3(-0.321394, -0.883022, -0.342020), vec3(-0.321394, -0.883022, -0.342020),
        vec3(-0.492404, -0.852869, -0.173648), vec3(-0.492404, -0.852869, -0.173648),
        vec3(-0.336824, -0.925417, -0.173648), vec3(-0.336824, -0.925417, -0.173648),
        vec3(-0.500000, -0.866025, 0.000000), vec3(-0.500000, -0.866025, 0.000000),
        vec3(-0.342020, -0.939693, 0.000000), vec3(-0.342020, -0.939693, 0.000000),
        vec3(-0.492404, -0.852869, 0.173648), vec3(-0.492404, -0.852869, 0.173648),
        vec3(-0.336824, -0.925417, 0.173648), vec3(-0.336824, -0.925417, 0.173648),
        vec3(-0.469846, -0.813798, 0.342020), vec3(-0.469846, -0.813798, 0.342020),
        vec3(-0.321394, -0.883022, 0.342020), vec3(-0.321394, -0.883022, 0.342020),
        vec3(-0.433013, -0.750000, 0.500000), vec3(-0.433013, -0.750000, 0.500000),
        vec3(-0.296198, -0.813798, 0.500000), vec3(-0.296198, -0.813798, 0.500000),
        vec3(-0.383022, -0.663414, 0.642788), vec3(-0.383022, -0.663414, 0.642788),
        vec3(-0.262003, -0.719846, 0.642788), vec3(-0.262003, -0.719846, 0.642788),
        vec3(-0.321394, -0.556670, 0.766044), vec3(-0.321394, -0.556670, 0.766044),
        vec3(-0.219846, -0.604023, 0.766044), vec3(-0.219846, -0.604023, 0.766044),
        vec3(-0.250000, -0.433013, 0.866025), vec3(-0.250000, -0.433013, 0.866025),
        vec3(-0.171010, -0.469846, 0.866025), vec3(-0.171010, -0.469846, 0.866025),
        vec3(-0.171010, -0.296198, 0.939693), vec3(-0.171010, -0.296198, 0.939693),
        vec3(-0.116978, -0.321394, 0.939693), vec3(-0.116978, -0.321394, 0.939693),
        vec3(-0.086824, -0.150384, 0.984808), vec3(-0.086824, -0.150384, 0.984808),
        vec3(-0.059391, -0.163176, 0.984808), vec3(-0.059391, -0.163176, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(-0.059391, -0.163176, 0.984808), vec3(-0.059391, -0.163176, 0.984808),
        vec3(-0.030154, -0.171010, 0.984808), vec3(-0.030154, -0.171010, 0.984808),
        vec3(-0.116978, -0.321394, 0.939693), vec3(-0.116978, -0.321394, 0.939693),
        vec3(-0.059391, -0.336824, 0.939693), vec3(-0.059391, -0.336824, 0.939693),
        vec3(-0.171010, -0.469846, 0.866025), vec3(-0.171010, -0.469846, 0.866025),
        vec3(-0.086824, -0.492404, 0.866025), vec3(-0.086824, -0.492404, 0.866025),
        vec3(-0.219846, -0.604023, 0.766044), vec3(-0.219846, -0.604023, 0.766044),
        vec3(-0.111619, -0.633022, 0.766044), vec3(-0.111619, -0.633022, 0.766044),
        vec3(-0.262003, -0.719846, 0.642788), vec3(-0.262003, -0.719846, 0.642788),
        vec3(-0.133022, -0.754407, 0.642788), vec3(-0.133022, -0.754407, 0.642788),
        vec3(-0.296198, -0.813798, 0.500000), vec3(-0.296198, -0.813798, 0.500000),
        vec3(-0.150384, -0.852869, 0.500000), vec3(-0.150384, -0.852869, 0.500000),
        vec3(-0.321394, -0.883022, 0.342020), vec3(-0.321394, -0.883022, 0.342020),
        vec3(-0.163176, -0.925417, 0.342020), vec3(-0.163176, -0.925417, 0.342020),
        vec3(-0.336824, -0.925417, 0.173648), vec3(-0.336824, -0.925417, 0.173648),
        vec3(-0.171010, -0.969846, 0.173648), vec3(-0.171010, -0.969846, 0.173648),
        vec3(-0.342020, -0.939693, 0.000000), vec3(-0.342020, -0.939693, 0.000000),
        vec3(-0.173648, -0.984808, 0.000000), vec3(-0.173648, -0.984808, 0.000000),
        vec3(-0.336824, -0.925417, -0.173648), vec3(-0.336824, -0.925417, -0.173648),
        vec3(-0.171010, -0.969846, -0.173648), vec3(-0.171010, -0.969846, -0.173648),
        vec3(-0.321394, -0.883022, -0.342020), vec3(-0.321394, -0.883022, -0.342020),
        vec3(-0.163176, -0.925417, -0.342020), vec3(-0.163176, -0.925417, -0.342020),
        vec3(-0.296198, -0.813798, -0.500000), vec3(-0.296198, -0.813798, -0.500000),
        vec3(-0.150384, -0.852869, -0.500000), vec3(-0.150384, -0.852869, -0.500000),
        vec3(-0.262003, -0.719846, -0.642788), vec3(-0.262003, -0.719846, -0.642788),
        vec3(-0.133022, -0.754407, -0.642788), vec3(-0.133022, -0.754407, -0.642788),
        vec3(-0.219846, -0.604023, -0.766044), vec3(-0.219846, -0.604023, -0.766044),
        vec3(-0.111619, -0.633022, -0.766044), vec3(-0.111619, -0.633022, -0.766044),
        vec3(-0.171010, -0.469846, -0.866025), vec3(-0.171010, -0.469846, -0.866025),
        vec3(-0.086824, -0.492404, -0.866025), vec3(-0.086824, -0.492404, -0.866025),
        vec3(-0.116978, -0.321394, -0.939693), vec3(-0.116978, -0.321394, -0.939693),
        vec3(-0.059391, -0.336824, -0.939693), vec3(-0.059391, -0.336824, -0.939693),
        vec3(-0.059391, -0.163176, -0.984808), vec3(-0.059391, -0.163176, -0.984808),
        vec3(-0.030154, -0.171010, -0.984808), vec3(-0.030154, -0.171010, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(-0.030154, -0.171010, -0.984808), vec3(-0.030154, -0.171010, -0.984808),
        vec3(-0.000000, -0.173648, -0.984808), vec3(-0.000000, -0.173648, -0.984808),
        vec3(-0.059391, -0.336824, -0.939693), vec3(-0.059391, -0.336824, -0.939693),
        vec3(-0.000000, -0.342020, -0.939693), vec3(-0.000000, -0.342020, -0.939693),
        vec3(-0.086824, -0.492404, -0.866025), vec3(-0.086824, -0.492404, -0.866025),
        vec3(-0.000000, -0.500000, -0.866025), vec3(-0.000000, -0.500000, -0.866025),
        vec3(-0.111619, -0.633022, -0.766044), vec3(-0.111619, -0.633022, -0.766044),
        vec3(-0.000000, -0.642788, -0.766044), vec3(-0.000000, -0.642788, -0.766044),
        vec3(-0.133022, -0.754407, -0.642788), vec3(-0.133022, -0.754407, -0.642788),
        vec3(-0.000000, -0.766044, -0.642788), vec3(-0.000000, -0.766044, -0.642788),
        vec3(-0.150384, -0.852869, -0.500000), vec3(-0.150384, -0.852869, -0.500000),
        vec3(-0.000000, -0.866025, -0.500000), vec3(-0.000000, -0.866025, -0.500000),
        vec3(-0.163176, -0.925417, -0.342020), vec3(-0.163176, -0.925417, -0.342020),
        vec3(-0.000000, -0.939693, -0.342020), vec3(-0.000000, -0.939693, -0.342020),
        vec3(-0.171010, -0.969846, -0.173648), vec3(-0.171010, -0.969846, -0.173648),
        vec3(-0.000000, -0.984808, -0.173648), vec3(-0.000000, -0.984808, -0.173648),
        vec3(-0.173648, -0.984808, 0.000000), vec3(-0.173648, -0.984808, 0.000000),
        vec3(-0.000000, -1.000000, 0.000000), vec3(-0.000000, -1.000000, 0.000000),
        vec3(-0.171010, -0.969846, 0.173648), vec3(-0.171010, -0.969846, 0.173648),
        vec3(-0.000000, -0.984808, 0.173648), vec3(-0.000000, -0.984808, 0.173648),
        vec3(-0.163176, -0.925417, 0.342020), vec3(-0.163176, -0.925417, 0.342020),
        vec3(-0.000000, -0.939693, 0.342020), vec3(-0.000000, -0.939693, 0.342020),
        vec3(-0.150384, -0.852869, 0.500000), vec3(-0.150384, -0.852869, 0.500000),
        vec3(-0.000000, -0.866025, 0.500000), vec3(-0.000000, -0.866025, 0.500000),
        vec3(-0.133022, -0.754407, 0.642788), vec3(-0.133022, -0.754407, 0.642788),
        vec3(-0.000000, -0.766044, 0.642788), vec3(-0.000000, -0.766044, 0.642788),
        vec3(-0.111619, -0.633022, 0.766044), vec3(-0.111619, -0.633022, 0.766044),
        vec3(-0.000000, -0.642788, 0.766044), vec3(-0.000000, -0.642788, 0.766044),
        vec3(-0.086824, -0.492404, 0.866025), vec3(-0.086824, -0.492404, 0.866025),
        vec3(-0.000000, -0.500000, 0.866025), vec3(-0.000000, -0.500000, 0.866025),
        vec3(-0.059391, -0.336824, 0.939693), vec3(-0.059391, -0.336824, 0.939693),
        vec3(-0.000000, -0.342020, 0.939693), vec3(-0.000000, -0.342020, 0.939693),
        vec3(-0.030154, -0.171010, 0.984808), vec3(-0.030154, -0.171010, 0.984808),
        vec3(-0.000000, -0.173648, 0.984808), vec3(-0.000000, -0.173648, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(-0.000000, -0.173648, 0.984808), vec3(-0.000000, -0.173648, 0.984808),
        vec3(0.030154, -0.171010, 0.984808), vec3(0.030154, -0.171010, 0.984808),
        vec3(-0.000000, -0.342020, 0.939693), vec3(-0.000000, -0.342020, 0.939693),
        vec3(0.059391, -0.336824, 0.939693), vec3(0.059391, -0.336824, 0.939693),
        vec3(-0.000000, -0.500000, 0.866025), vec3(-0.000000, -0.500000, 0.866025),
        vec3(0.086824, -0.492404, 0.866025), vec3(0.086824, -0.492404, 0.866025),
        vec3(-0.000000, -0.642788, 0.766044), vec3(-0.000000, -0.642788, 0.766044),
        vec3(0.111619, -0.633022, 0.766044), vec3(0.111619, -0.633022, 0.766044),
        vec3(-0.000000, -0.766044, 0.642788), vec3(-0.000000, -0.766044, 0.642788),
        vec3(0.133022, -0.754407, 0.642788), vec3(0.133022, -0.754407, 0.642788),
        vec3(-0.000000, -0.866025, 0.500000), vec3(-0.000000, -0.866025, 0.500000),
        vec3(0.150384, -0.852869, 0.500000), vec3(0.150384, -0.852869, 0.500000),
        vec3(-0.000000, -0.939693, 0.342020), vec3(-0.000000, -0.939693, 0.342020),
        vec3(0.163176, -0.925417, 0.342020), vec3(0.163176, -0.925417, 0.342020),
        vec3(-0.000000, -0.984808, 0.173648), vec3(-0.000000, -0.984808, 0.173648),
        vec3(0.171010, -0.969846, 0.173648), vec3(0.171010, -0.969846, 0.173648),
        vec3(-0.000000, -1.000000, 0.000000), vec3(-0.000000, -1.000000, 0.000000),
        vec3(0.173648, -0.984808, 0.000000), vec3(0.173648, -0.984808, 0.000000),
        vec3(-0.000000, -0.984808, -0.173648), vec3(-0.000000, -0.984808, -0.173648),
        vec3(0.171010, -0.969846, -0.173648), vec3(0.171010, -0.969846, -0.173648),
        vec3(-0.000000, -0.939693, -0.342020), vec3(-0.000000, -0.939693, -0.342020),
        vec3(0.163176, -0.925417, -0.342020), vec3(0.163176, -0.925417, -0.342020),
        vec3(-0.000000, -0.866025, -0.500000), vec3(-0.000000, -0.866025, -0.500000),
        vec3(0.150384, -0.852869, -0.500000), vec3(0.150384, -0.852869, -0.500000),
        vec3(-0.000000, -0.766044, -0.642788), vec3(-0.000000, -0.766044, -0.642788),
        vec3(0.133022, -0.754407, -0.642788), vec3(0.133022, -0.754407, -0.642788),
        vec3(-0.000000, -0.642788, -0.766044), vec3(-0.000000, -0.642788, -0.766044),
        vec3(0.111619, -0.633022, -0.766044), vec3(0.111619, -0.633022, -0.766044),
        vec3(-0.000000, -0.500000, -0.866025), vec3(-0.000000, -0.500000, -0.866025),
        vec3(0.086824, -0.492404, -0.866025), vec3(0.086824, -0.492404, -0.866025),
        vec3(-0.000000, -0.342020, -0.939693), vec3(-0.000000, -0.342020, -0.939693),
        vec3(0.059391, -0.336824, -0.939693), vec3(0.059391, -0.336824, -0.939693),
        vec3(-0.000000, -0.173648, -0.984808), vec3(-0.000000, -0.173648, -0.984808),
        vec3(0.030154, -0.171010, -0.984808), vec3(0.030154, -0.171010, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(0.030154, -0.171010, -0.984808), vec3(0.030154, -0.171010, -0.984808),
        vec3(0.059391, -0.163176, -0.984808), vec3(0.059391, -0.163176, -0.984808),
        vec3(0.059391, -0.336824, -0.939693), vec3(0.059391, -0.336824, -0.939693),
        vec3(0.116978, -0.321394, -0.939693), vec3(0.116978, -0.321394, -0.939693),
        vec3(0.086824, -0.492404, -0.866025), vec3(0.086824, -0.492404, -0.866025),
        vec3(0.171010, -0.469846, -0.866025), vec3(0.171010, -0.469846, -0.866025),
        vec3(0.111619, -0.633022, -0.766044), vec3(0.111619, -0.633022, -0.766044),
        vec3(0.219846, -0.604023, -0.766044), vec3(0.219846, -0.604023, -0.766044),
        vec3(0.133022, -0.754407, -0.642788), vec3(0.133022, -0.754407, -0.642788),
        vec3(0.262003, -0.719846, -0.642788), vec3(0.262003, -0.719846, -0.642788),
        vec3(0.150384, -0.852869, -0.500000), vec3(0.150384, -0.852869, -0.500000),
        vec3(0.296198, -0.813798, -0.500000), vec3(0.296198, -0.813798, -0.500000),
        vec3(0.163176, -0.925417, -0.342020), vec3(0.163176, -0.925417, -0.342020),
        vec3(0.321394, -0.883022, -0.342020), vec3(0.321394, -0.883022, -0.342020),
        vec3(0.171010, -0.969846, -0.173648), vec3(0.171010, -0.969846, -0.173648),
        vec3(0.336824, -0.925417, -0.173648), vec3(0.336824, -0.925417, -0.173648),
        vec3(0.173648, -0.984808, 0.000000), vec3(0.173648, -0.984808, 0.000000),
        vec3(0.342020, -0.939693, 0.000000), vec3(0.342020, -0.939693, 0.000000),
        vec3(0.171010, -0.969846, 0.173648), vec3(0.171010, -0.969846, 0.173648),
        vec3(0.336824, -0.925417, 0.173648), vec3(0.336824, -0.925417, 0.173648),
        vec3(0.163176, -0.925417, 0.342020), vec3(0.163176, -0.925417, 0.342020),
        vec3(0.321394, -0.883022, 0.342020), vec3(0.321394, -0.883022, 0.342020),
        vec3(0.150384, -0.852869, 0.500000), vec3(0.150384, -0.852869, 0.500000),
        vec3(0.296198, -0.813798, 0.500000), vec3(0.296198, -0.813798, 0.500000),
        vec3(0.133022, -0.754407, 0.642788), vec3(0.133022, -0.754407, 0.642788),
        vec3(0.262003, -0.719846, 0.642788), vec3(0.262003, -0.719846, 0.642788),
        vec3(0.111619, -0.633022, 0.766044), vec3(0.111619, -0.633022, 0.766044),
        vec3(0.219846, -0.604023, 0.766044), vec3(0.219846, -0.604023, 0.766044),
        vec3(0.086824, -0.492404, 0.866025), vec3(0.086824, -0.492404, 0.866025),
        vec3(0.171010, -0.469846, 0.866025), vec3(0.171010, -0.469846, 0.866025),
        vec3(0.059391, -0.336824, 0.939693), vec3(0.059391, -0.336824, 0.939693),
        vec3(0.116978, -0.321394, 0.939693), vec3(0.116978, -0.321394, 0.939693),
        vec3(0.030154, -0.171010, 0.984808), vec3(0.030154, -0.171010, 0.984808),
        vec3(0.059391, -0.163176, 0.984808), vec3(0.059391, -0.163176, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(0.059391, -0.163176, 0.984808), vec3(0.059391, -0.163176, 0.984808),
        vec3(0.086824, -0.150384, 0.984808), vec3(0.086824, -0.150384, 0.984808),
        vec3(0.116978, -0.321394, 0.939693), vec3(0.116978, -0.321394, 0.939693),
        vec3(0.171010, -0.296198, 0.939693), vec3(0.171010, -0.296198, 0.939693),
        vec3(0.171010, -0.469846, 0.866025), vec3(0.171010, -0.469846, 0.866025),
        vec3(0.250000, -0.433013, 0.866025), vec3(0.250000, -0.433013, 0.866025),
        vec3(0.219846, -0.604023, 0.766044), vec3(0.219846, -0.604023, 0.766044),
        vec3(0.321394, -0.556670, 0.766044), vec3(0.321394, -0.556670, 0.766044),
        vec3(0.262003, -0.719846, 0.642788), vec3(0.262003, -0.719846, 0.642788),
        vec3(0.383022, -0.663414, 0.642788), vec3(0.383022, -0.663414, 0.642788),
        vec3(0.296198, -0.813798, 0.500000), vec3(0.296198, -0.813798, 0.500000),
        vec3(0.433013, -0.750000, 0.500000), vec3(0.433013, -0.750000, 0.500000),
        vec3(0.321394, -0.883022, 0.342020), vec3(0.321394, -0.883022, 0.342020),
        vec3(0.469846, -0.813798, 0.342020), vec3(0.469846, -0.813798, 0.342020),
        vec3(0.336824, -0.925417, 0.173648), vec3(0.336824, -0.925417, 0.173648),
        vec3(0.492404, -0.852869, 0.173648), vec3(0.492404, -0.852869, 0.173648),
        vec3(0.342020, -0.939693, 0.000000), vec3(0.342020, -0.939693, 0.000000),
        vec3(0.500000, -0.866025, 0.000000), vec3(0.500000, -0.866025, 0.000000),
        vec3(0.336824, -0.925417, -0.173648), vec3(0.336824, -0.925417, -0.173648),
        vec3(0.492404, -0.852869, -0.173648), vec3(0.492404, -0.852869, -0.173648),
        vec3(0.321394, -0.883022, -0.342020), vec3(0.321394, -0.883022, -0.342020),
        vec3(0.469846, -0.813798, -0.342020), vec3(0.469846, -0.813798, -0.342020),
        vec3(0.296198, -0.813798, -0.500000), vec3(0.296198, -0.813798, -0.500000),
        vec3(0.433013, -0.750000, -0.500000), vec3(0.433013, -0.750000, -0.500000),
        vec3(0.262003, -0.719846, -0.642788), vec3(0.262003, -0.719846, -0.642788),
        vec3(0.383022, -0.663414, -0.642788), vec3(0.383022, -0.663414, -0.642788),
        vec3(0.219846, -0.604023, -0.766044), vec3(0.219846, -0.604023, -0.766044),
        vec3(0.321394, -0.556670, -0.766044), vec3(0.321394, -0.556670, -0.766044),
        vec3(0.171010, -0.469846, -0.866025), vec3(0.171010, -0.469846, -0.866025),
        vec3(0.250000, -0.433013, -0.866025), vec3(0.250000, -0.433013, -0.866025),
        vec3(0.116978, -0.321394, -0.939693), vec3(0.116978, -0.321394, -0.939693),
        vec3(0.171010, -0.296198, -0.939693), vec3(0.171010, -0.296198, -0.939693),
        vec3(0.059391, -0.163176, -0.984808), vec3(0.059391, -0.163176, -0.984808),
        vec3(0.086824, -0.150384, -0.984808), vec3(0.086824, -0.150384, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(0.086824, -0.150384, -0.984808), vec3(0.086824, -0.150384, -0.984808),
        vec3(0.111619, -0.133022, -0.984808), vec3(0.111619, -0.133022, -0.984808),
        vec3(0.171010, -0.296198, -0.939693), vec3(0.171010, -0.296198, -0.939693),
        vec3(0.219846, -0.262003, -0.939693), vec3(0.219846, -0.262003, -0.939693),
        vec3(0.250000, -0.433013, -0.866025), vec3(0.250000, -0.433013, -0.866025),
        vec3(0.321394, -0.383022, -0.866025), vec3(0.321394, -0.383022, -0.866025),
        vec3(0.321394, -0.556670, -0.766044), vec3(0.321394, -0.556670, -0.766044),
        vec3(0.413176, -0.492404, -0.766044), vec3(0.413176, -0.492404, -0.766045),
        vec3(0.383022, -0.663414, -0.642788), vec3(0.383022, -0.663414, -0.642788),
        vec3(0.492404, -0.586824, -0.642788), vec3(0.492404, -0.586824, -0.642788),
        vec3(0.433013, -0.750000, -0.500000), vec3(0.433013, -0.750000, -0.500000),
        vec3(0.556670, -0.663414, -0.500000), vec3(0.556670, -0.663414, -0.500000),
        vec3(0.469846, -0.813798, -0.342020), vec3(0.469846, -0.813798, -0.342020),
        vec3(0.604023, -0.719846, -0.342020), vec3(0.604023, -0.719846, -0.342020),
        vec3(0.492404, -0.852869, -0.173648), vec3(0.492404, -0.852869, -0.173648),
        vec3(0.633022, -0.754407, -0.173648), vec3(0.633022, -0.754407, -0.173648),
        vec3(0.500000, -0.866025, 0.000000), vec3(0.500000, -0.866025, 0.000000),
        vec3(0.642788, -0.766044, 0.000000), vec3(0.642788, -0.766044, 0.000000),
        vec3(0.492404, -0.852869, 0.173648), vec3(0.492404, -0.852869, 0.173648),
        vec3(0.633022, -0.754407, 0.173648), vec3(0.633022, -0.754407, 0.173648),
        vec3(0.469846, -0.813798, 0.342020), vec3(0.469846, -0.813798, 0.342020),
        vec3(0.604023, -0.719846, 0.342020), vec3(0.604023, -0.719846, 0.342020),
        vec3(0.433013, -0.750000, 0.500000), vec3(0.433013, -0.750000, 0.500000),
        vec3(0.556670, -0.663414, 0.500000), vec3(0.556670, -0.663414, 0.500000),
        vec3(0.383022, -0.663414, 0.642788), vec3(0.383022, -0.663414, 0.642788),
        vec3(0.492404, -0.586824, 0.642788), vec3(0.492404, -0.586824, 0.642788),
        vec3(0.321394, -0.556670, 0.766044), vec3(0.321394, -0.556670, 0.766044),
        vec3(0.413176, -0.492404, 0.766044), vec3(0.413176, -0.492404, 0.766045),
        vec3(0.250000, -0.433013, 0.866025), vec3(0.250000, -0.433013, 0.866025),
        vec3(0.321394, -0.383022, 0.866025), vec3(0.321394, -0.383022, 0.866025),
        vec3(0.171010, -0.296198, 0.939693), vec3(0.171010, -0.296198, 0.939693),
        vec3(0.219846, -0.262003, 0.939693), vec3(0.219846, -0.262003, 0.939693),
        vec3(0.086824, -0.150384, 0.984808), vec3(0.086824, -0.150384, 0.984808),
        vec3(0.111619, -0.133022, 0.984808), vec3(0.111619, -0.133022, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(0.111619, -0.133022, 0.984808), vec3(0.111619, -0.133022, 0.984808),
        vec3(0.133022, -0.111619, 0.984808), vec3(0.133022, -0.111619, 0.984808),
        vec3(0.219846, -0.262003, 0.939693), vec3(0.219846, -0.262003, 0.939693),
        vec3(0.262003, -0.219846, 0.939693), vec3(0.262003, -0.219846, 0.939693),
        vec3(0.321394, -0.383022, 0.866025), vec3(0.321394, -0.383022, 0.866025),
        vec3(0.383022, -0.321394, 0.866025), vec3(0.383022, -0.321394, 0.866025),
        vec3(0.413176, -0.492404, 0.766044), vec3(0.413176, -0.492404, 0.766045),
        vec3(0.492404, -0.413176, 0.766044), vec3(0.492404, -0.413176, 0.766045),
        vec3(0.492404, -0.586824, 0.642788), vec3(0.492404, -0.586824, 0.642788),
        vec3(0.586824, -0.492404, 0.642788), vec3(0.586824, -0.492404, 0.642788),
        vec3(0.556670, -0.663414, 0.500000), vec3(0.556670, -0.663414, 0.500000),
        vec3(0.663414, -0.556670, 0.500000), vec3(0.663414, -0.556670, 0.500000),
        vec3(0.604023, -0.719846, 0.342020), vec3(0.604023, -0.719846, 0.342020),
        vec3(0.719846, -0.604023, 0.342020), vec3(0.719846, -0.604023, 0.342020),
        vec3(0.633022, -0.754407, 0.173648), vec3(0.633022, -0.754407, 0.173648),
        vec3(0.754407, -0.633022, 0.173648), vec3(0.754407, -0.633022, 0.173648),
        vec3(0.642788, -0.766044, 0.000000), vec3(0.642788, -0.766044, 0.000000),
        vec3(0.766044, -0.642788, 0.000000), vec3(0.766044, -0.642788, 0.000000),
        vec3(0.633022, -0.754407, -0.173648), vec3(0.633022, -0.754407, -0.173648),
        vec3(0.754407, -0.633022, -0.173648), vec3(0.754407, -0.633022, -0.173648),
        vec3(0.604023, -0.719846, -0.342020), vec3(0.604023, -0.719846, -0.342020),
        vec3(0.719846, -0.604023, -0.342020), vec3(0.719846, -0.604023, -0.342020),
        vec3(0.556670, -0.663414, -0.500000), vec3(0.556670, -0.663414, -0.500000),
        vec3(0.663414, -0.556670, -0.500000), vec3(0.663414, -0.556670, -0.500000),
        vec3(0.492404, -0.586824, -0.642788), vec3(0.492404, -0.586824, -0.642788),
        vec3(0.586824, -0.492404, -0.642788), vec3(0.586824, -0.492404, -0.642788),
        vec3(0.413176, -0.492404, -0.766044), vec3(0.413176, -0.492404, -0.766045),
        vec3(0.492404, -0.413176, -0.766044), vec3(0.492404, -0.413176, -0.766045),
        vec3(0.321394, -0.383022, -0.866025), vec3(0.321394, -0.383022, -0.866025),
        vec3(0.383022, -0.321394, -0.866025), vec3(0.383022, -0.321394, -0.866025),
        vec3(0.219846, -0.262003, -0.939693), vec3(0.219846, -0.262003, -0.939693),
        vec3(0.262003, -0.219846, -0.939693), vec3(0.262003, -0.219846, -0.939693),
        vec3(0.111619, -0.133022, -0.984808), vec3(0.111619, -0.133022, -0.984808),
        vec3(0.133022, -0.111619, -0.984808), vec3(0.133022, -0.111619, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(0.133022, -0.111619, -0.984808), vec3(0.133022, -0.111619, -0.984808),
        vec3(0.150384, -0.086824, -0.984808), vec3(0.150384, -0.086824, -0.984808),
        vec3(0.262003, -0.219846, -0.939693), vec3(0.262003, -0.219846, -0.939693),
        vec3(0.296198, -0.171010, -0.939693), vec3(0.296198, -0.171010, -0.939693),
        vec3(0.383022, -0.321394, -0.866025), vec3(0.383022, -0.321394, -0.866025),
        vec3(0.433013, -0.250000, -0.866025), vec3(0.433013, -0.250000, -0.866025),
        vec3(0.492404, -0.413176, -0.766044), vec3(0.492404, -0.413176, -0.766045),
        vec3(0.556670, -0.321394, -0.766044), vec3(0.556670, -0.321394, -0.766044),
        vec3(0.586824, -0.492404, -0.642788), vec3(0.586824, -0.492404, -0.642788),
        vec3(0.663414, -0.383022, -0.642788), vec3(0.663414, -0.383022, -0.642788),
        vec3(0.663414, -0.556670, -0.500000), vec3(0.663414, -0.556670, -0.500000),
        vec3(0.750000, -0.433013, -0.500000), vec3(0.750000, -0.433013, -0.500000),
        vec3(0.719846, -0.604023, -0.342020), vec3(0.719846, -0.604023, -0.342020),
        vec3(0.813798, -0.469846, -0.342020), vec3(0.813798, -0.469846, -0.342020),
        vec3(0.754407, -0.633022, -0.173648), vec3(0.754407, -0.633022, -0.173648),
        vec3(0.852869, -0.492404, -0.173648), vec3(0.852869, -0.492404, -0.173648),
        vec3(0.766044, -0.642788, 0.000000), vec3(0.766044, -0.642788, 0.000000),
        vec3(0.866025, -0.500000, 0.000000), vec3(0.866025, -0.500000, 0.000000),
        vec3(0.754407, -0.633022, 0.173648), vec3(0.754407, -0.633022, 0.173648),
        vec3(0.852869, -0.492404, 0.173648), vec3(0.852869, -0.492404, 0.173648),
        vec3(0.719846, -0.604023, 0.342020), vec3(0.719846, -0.604023, 0.342020),
        vec3(0.813798, -0.469846, 0.342020), vec3(0.813798, -0.469846, 0.342020),
        vec3(0.663414, -0.556670, 0.500000), vec3(0.663414, -0.556670, 0.500000),
        vec3(0.750000, -0.433013, 0.500000), vec3(0.750000, -0.433013, 0.500000),
        vec3(0.586824, -0.492404, 0.642788), vec3(0.586824, -0.492404, 0.642788),
        vec3(0.663414, -0.383022, 0.642788), vec3(0.663414, -0.383022, 0.642788),
        vec3(0.492404, -0.413176, 0.766044), vec3(0.492404, -0.413176, 0.766045),
        vec3(0.556670, -0.321394, 0.766044), vec3(0.556670, -0.321394, 0.766044),
        vec3(0.383022, -0.321394, 0.866025), vec3(0.383022, -0.321394, 0.866025),
        vec3(0.433013, -0.250000, 0.866025), vec3(0.433013, -0.250000, 0.866025),
        vec3(0.262003, -0.219846, 0.939693), vec3(0.262003, -0.219846, 0.939693),
        vec3(0.296198, -0.171010, 0.939693), vec3(0.296198, -0.171010, 0.939693),
        vec3(0.133022, -0.111619, 0.984808), vec3(0.133022, -0.111619, 0.984808),
        vec3(0.150384, -0.086824, 0.984808), vec3(0.150384, -0.086824, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(0.150384, -0.086824, 0.984808), vec3(0.150384, -0.086824, 0.984808),
        vec3(0.163176, -0.059391, 0.984808), vec3(0.163176, -0.059391, 0.984808),
        vec3(0.296198, -0.171010, 0.939693), vec3(0.296198, -0.171010, 0.939693),
        vec3(0.321394, -0.116978, 0.939693), vec3(0.321394, -0.116978, 0.939693),
        vec3(0.433013, -0.250000, 0.866025), vec3(0.433013, -0.250000, 0.866025),
        vec3(0.469846, -0.171010, 0.866025), vec3(0.469846, -0.171010, 0.866025),
        vec3(0.556670, -0.321394, 0.766044), vec3(0.556670, -0.321394, 0.766044),
        vec3(0.604023, -0.219846, 0.766044), vec3(0.604023, -0.219846, 0.766044),
        vec3(0.663414, -0.383022, 0.642788), vec3(0.663414, -0.383022, 0.642788),
        vec3(0.719846, -0.262003, 0.642788), vec3(0.719846, -0.262003, 0.642788),
        vec3(0.750000, -0.433013, 0.500000), vec3(0.750000, -0.433013, 0.500000),
        vec3(0.813798, -0.296198, 0.500000), vec3(0.813798, -0.296198, 0.500000),
        vec3(0.813798, -0.469846, 0.342020), vec3(0.813798, -0.469846, 0.342020),
        vec3(0.883022, -0.321394, 0.342020), vec3(0.883022, -0.321394, 0.342020),
        vec3(0.852869, -0.492404, 0.173648), vec3(0.852869, -0.492404, 0.173648),
        vec3(0.925417, -0.336824, 0.173648), vec3(0.925417, -0.336824, 0.173648),
        vec3(0.866025, -0.500000, 0.000000), vec3(0.866025, -0.500000, 0.000000),
        vec3(0.939693, -0.342020, 0.000000), vec3(0.939693, -0.342020, 0.000000),
        vec3(0.852869, -0.492404, -0.173648), vec3(0.852869, -0.492404, -0.173648),
        vec3(0.925417, -0.336824, -0.173648), vec3(0.925417, -0.336824, -0.173648),
        vec3(0.813798, -0.469846, -0.342020), vec3(0.813798, -0.469846, -0.342020),
        vec3(0.883022, -0.321394, -0.342020), vec3(0.883022, -0.321394, -0.342020),
        vec3(0.750000, -0.433013, -0.500000), vec3(0.750000, -0.433013, -0.500000),
        vec3(0.813798, -0.296198, -0.500000), vec3(0.813798, -0.296198, -0.500000),
        vec3(0.663414, -0.383022, -0.642788), vec3(0.663414, -0.383022, -0.642788),
        vec3(0.719846, -0.262003, -0.642788), vec3(0.719846, -0.262003, -0.642788),
        vec3(0.556670, -0.321394, -0.766044), vec3(0.556670, -0.321394, -0.766044),
        vec3(0.604023, -0.219846, -0.766044), vec3(0.604023, -0.219846, -0.766044),
        vec3(0.433013, -0.250000, -0.866025), vec3(0.433013, -0.250000, -0.866025),
        vec3(0.469846, -0.171010, -0.866025), vec3(0.469846, -0.171010, -0.866025),
        vec3(0.296198, -0.171010, -0.939693), vec3(0.296198, -0.171010, -0.939693),
        vec3(0.321394, -0.116978, -0.939693), vec3(0.321394, -0.116978, -0.939693),
        vec3(0.150384, -0.086824, -0.984808), vec3(0.150384, -0.086824, -0.984808),
        vec3(0.163176, -0.059391, -0.984808), vec3(0.163176, -0.059391, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(0.163176, -0.059391, -0.984808), vec3(0.163176, -0.059391, -0.984808),
        vec3(0.171010, -0.030154, -0.984808), vec3(0.171010, -0.030154, -0.984808),
        vec3(0.321394, -0.116978, -0.939693), vec3(0.321394, -0.116978, -0.939693),
        vec3(0.336824, -0.059391, -0.939693), vec3(0.336824, -0.059391, -0.939693),
        vec3(0.469846, -0.171010, -0.866025), vec3(0.469846, -0.171010, -0.866025),
        vec3(0.492404, -0.086824, -0.866025), vec3(0.492404, -0.086824, -0.866025),
        vec3(0.604023, -0.219846, -0.766044), vec3(0.604023, -0.219846, -0.766044),
        vec3(0.633022, -0.111619, -0.766044), vec3(0.633022, -0.111619, -0.766044),
        vec3(0.719846, -0.262003, -0.642788), vec3(0.719846, -0.262003, -0.642788),
        vec3(0.754407, -0.133022, -0.642788), vec3(0.754407, -0.133022, -0.642788),
        vec3(0.813798, -0.296198, -0.500000), vec3(0.813798, -0.296198, -0.500000),
        vec3(0.852869, -0.150384, -0.500000), vec3(0.852869, -0.150384, -0.500000),
        vec3(0.883022, -0.321394, -0.342020), vec3(0.883022, -0.321394, -0.342020),
        vec3(0.925417, -0.163176, -0.342020), vec3(0.925417, -0.163176, -0.342020),
        vec3(0.925417, -0.336824, -0.173648), vec3(0.925417, -0.336824, -0.173648),
        vec3(0.969846, -0.171010, -0.173648), vec3(0.969846, -0.171010, -0.173648),
        vec3(0.939693, -0.342020, 0.000000), vec3(0.939693, -0.342020, 0.000000),
        vec3(0.984808, -0.173648, 0.000000), vec3(0.984808, -0.173648, 0.000000),
        vec3(0.925417, -0.336824, 0.173648), vec3(0.925417, -0.336824, 0.173648),
        vec3(0.969846, -0.171010, 0.173648), vec3(0.969846, -0.171010, 0.173648),
        vec3(0.883022, -0.321394, 0.342020), vec3(0.883022, -0.321394, 0.342020),
        vec3(0.925417, -0.163176, 0.342020), vec3(0.925417, -0.163176, 0.342020),
        vec3(0.813798, -0.296198, 0.500000), vec3(0.813798, -0.296198, 0.500000),
        vec3(0.852869, -0.150384, 0.500000), vec3(0.852869, -0.150384, 0.500000),
        vec3(0.719846, -0.262003, 0.642788), vec3(0.719846, -0.262003, 0.642788),
        vec3(0.754407, -0.133022, 0.642788), vec3(0.754407, -0.133022, 0.642788),
        vec3(0.604023, -0.219846, 0.766044), vec3(0.604023, -0.219846, 0.766044),
        vec3(0.633022, -0.111619, 0.766044), vec3(0.633022, -0.111619, 0.766044),
        vec3(0.469846, -0.171010, 0.866025), vec3(0.469846, -0.171010, 0.866025),
        vec3(0.492404, -0.086824, 0.866025), vec3(0.492404, -0.086824, 0.866025),
        vec3(0.321394, -0.116978, 0.939693), vec3(0.321394, -0.116978, 0.939693),
        vec3(0.336824, -0.059391, 0.939693), vec3(0.336824, -0.059391, 0.939693),
        vec3(0.163176, -0.059391, 0.984808), vec3(0.163176, -0.059391, 0.984808),
        vec3(0.171010, -0.030154, 0.984808), vec3(0.171010, -0.030154, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(0.171010, -0.030154, 0.984808), vec3(0.171010, -0.030154, 0.984808),
        vec3(0.173648, -0.000000, 0.984808), vec3(0.173648, -0.000000, 0.984808),
        vec3(0.336824, -0.059391, 0.939693), vec3(0.336824, -0.059391, 0.939693),
        vec3(0.342020, -0.000000, 0.939693), vec3(0.342020, -0.000000, 0.939693),
        vec3(0.492404, -0.086824, 0.866025), vec3(0.492404, -0.086824, 0.866025),
        vec3(0.500000, -0.000000, 0.866025), vec3(0.500000, -0.000000, 0.866025),
        vec3(0.633022, -0.111619, 0.766044), vec3(0.633022, -0.111619, 0.766044),
        vec3(0.642788, -0.000000, 0.766044), vec3(0.642788, -0.000000, 0.766044),
        vec3(0.754407, -0.133022, 0.642788), vec3(0.754407, -0.133022, 0.642788),
        vec3(0.766044, -0.000000, 0.642788), vec3(0.766044, -0.000000, 0.642788),
        vec3(0.852869, -0.150384, 0.500000), vec3(0.852869, -0.150384, 0.500000),
        vec3(0.866025, -0.000000, 0.500000), vec3(0.866025, -0.000000, 0.500000),
        vec3(0.925417, -0.163176, 0.342020), vec3(0.925417, -0.163176, 0.342020),
        vec3(0.939693, -0.000000, 0.342020), vec3(0.939693, -0.000000, 0.342020),
        vec3(0.969846, -0.171010, 0.173648), vec3(0.969846, -0.171010, 0.173648),
        vec3(0.984808, -0.000000, 0.173648), vec3(0.984808, -0.000000, 0.173648),
        vec3(0.984808, -0.173648, 0.000000), vec3(0.984808, -0.173648, 0.000000),
        vec3(1.000000, -0.000000, 0.000000), vec3(1.000000, -0.000000, 0.000000),
        vec3(0.969846, -0.171010, -0.173648), vec3(0.969846, -0.171010, -0.173648),
        vec3(0.984808, -0.000000, -0.173648), vec3(0.984808, -0.000000, -0.173648),
        vec3(0.925417, -0.163176, -0.342020), vec3(0.925417, -0.163176, -0.342020),
        vec3(0.939693, -0.000000, -0.342020), vec3(0.939693, -0.000000, -0.342020),
        vec3(0.852869, -0.150384, -0.500000), vec3(0.852869, -0.150384, -0.500000),
        vec3(0.866025, -0.000000, -0.500000), vec3(0.866025, -0.000000, -0.500000),
        vec3(0.754407, -0.133022, -0.642788), vec3(0.754407, -0.133022, -0.642788),
        vec3(0.766044, -0.000000, -0.642788), vec3(0.766044, -0.000000, -0.642788),
        vec3(0.633022, -0.111619, -0.766044), vec3(0.633022, -0.111619, -0.766044),
        vec3(0.642788, -0.000000, -0.766044), vec3(0.642788, -0.000000, -0.766044),
        vec3(0.492404, -0.086824, -0.866025), vec3(0.492404, -0.086824, -0.866025),
        vec3(0.500000, -0.000000, -0.866025), vec3(0.500000, -0.000000, -0.866025),
        vec3(0.336824, -0.059391, -0.939693), vec3(0.336824, -0.059391, -0.939693),
        vec3(0.342020, -0.000000, -0.939693), vec3(0.342020, -0.000000, -0.939693),
        vec3(0.171010, -0.030154, -0.984808), vec3(0.171010, -0.030154, -0.984808),
        vec3(0.173648, -0.000000, -0.984808), vec3(0.173648, -0.000000, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000)
    };


    //create instancing array for tile grid. 
    const int sideLength = 100;
    const float cellLength = 1.0f;
    const float height = 0.0f;
    vec3 offsetArray[sideLength*sideLength] = {};
    for (int i = 0; i < sideLength; ++i)
    {
        for (int j = 0; j < sideLength; ++j)
        {
            //skip the extra one (the last one)
            if (i == sideLength - 1 && j == sideLength - 1 ) {
                break;
            }
            offsetArray[sideLength * i + j +1] =
                    vec3(-cellLength * sideLength + (i+1) * cellLength,
                        height,
                        -cellLength * sideLength + (j+1) * cellLength);
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



    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

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
        ""
        "out vec3 normalVec;"
        "out vec3 fragPos;"
		"out vec2 vertexUV;"
        "void main()"
        "{"
        "   normalVec = mat3(transpose(inverse(worldMatrix))) * aNormal;"
        "   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
        "   gl_Position = modelViewProjection * vec4(aPos + instanceVec, 1.0);"
		"   vertexUV = aUV;"
        "   fragPos = vec3(worldMatrix * vec4(aPos, 1.0));"
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

    "vec3 lightColor = vec3(1.0, 1.0, 1.0);"    //light property
    "vec4 textureColor;"                        //texture property

    "in vec3 fragPos;"
    "in vec3 normalVec;"    //light properties
	"in vec2 vertexUV;"     //texture properties

    "out vec4 FragColor;"   //output
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
        // Final color output from phong model
        "   vec4 result = vec4((ambient + diffuse + specular) * objectColor, 1.0f);"
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
    const int numCells = (int)pow(sideLength,2);
    const float height = 0;     //y-position of grid.

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
    for ( itr = inputs.begin(); itr != inputs.end(); ++itr) {
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
    const float shearSpeed = 0.1f;
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
	    //shear model along x-axis if pressed
		inputsToModelMatrix.insert(pair<int, transformation>(GLFW_KEY_E, {
			
			mat4(1, 0, 0, 0,  // first column
			shearSpeed, 1, 0, 0,  // second column
			0, 0, 1, 0,  // third column
			0, 0, translateSpeed, 1) // fourth column
			, 2 }));

		inputsToModelMatrix.insert(pair<int, transformation>(GLFW_KEY_Q, {
			mat4(1, 0, 0, 0,  // first column
			-shearSpeed, 1, 0, 0,  // second column
			0, 0, 1, 0,  // third column
			0, 0, -translateSpeed, 1) // fourth column
			, 2 }));
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
bool* customControl(GLFWwindow * window, map<int, KeyState> previousKeyStates) {
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
        {GLFW_KEY_X, true}
    };

    //all below is automatic
    //consider if shift is pressed (referred to multiple times, so store it in a variable).
    const bool shiftPressed = isShiftPressed(window);
    //number of keys tied to settings.
    const int numKeys = sizeof(keyControl)/ sizeof(keyControl[0]);
    
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
    mat4 randomRelativeTranslateMatrix = translate(mat4(1.0f), vec3((float) randomPosX, selectedModel->getInitY(), (float) randomPosZ));
    selectedModel->setRelativeTranslateMatrix(randomRelativeTranslateMatrix);
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
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "COMP 371 - A1 - Team 4", NULL, NULL);
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
    GLuint brickTextureID = loadTexture("Textures/brick.jpg");
    GLuint cementTextureID = loadTexture("Textures/cement.jpg");
    GLuint metalTextureID = loadTexture("Textures/metal.jpg");
    GLuint boxTextureID = loadTexture("Textures/box.jpg");

#else
    //GLuint brickTextureID = loadTexture("../Assets/Textures/brick.jpg");
    //GLuint cementTextureID = loadTexture("../Assets/Textures/cement.jpg");
    //GLuint metalTextureID = loadTexture("../Assets/Textures/metal.jpg");
    //GLuint boxTextureID = loadTexture("../Assets/Textures/box.jpg");
    GLuint brickTextureID = loadTexture("Assets/Textures/brick.jpg");
    GLuint cementTextureID = loadTexture("Assets/Textures/cement.jpg");
    GLuint metalTextureID = loadTexture("Assets/Textures/metal.jpg");
    GLuint boxTextureID = loadTexture("Assets/Textures/box.jpg");

#endif

    // Grey background
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);

    // Compile and link shaders here ...
    int shaderProgram = compileAndLinkShaders();

    // We can set the shader once, since we have only one
    glUseProgram(shaderProgram);

    // Set light position vector in fragment shader to current light position value (done once)
    GLuint lightPosLocation = glGetUniformLocation(shaderProgram, "lightPos");
    glUniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z);

    // Used to ovewrite camera postion in fragment shader
    GLuint viewPosLocation = glGetUniformLocation(shaderProgram, "viewPos");

    // Used to overwrite color in the fragment shader
    GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");

    GLuint enableTextureLocation = glGetUniformLocation(shaderProgram, "hasTexture");
    int enableTexture = 1;	//1 for on, 0 for off

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
    //mat4 viewMatrix = lookAt(cameraPosition,  // eye
    //                         cameraPosition + cameraLookAt,  // center
    //                         cameraUp ); // up
    GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
    //glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

    //used for draw instancing.
    vec3 instanceVec[100];
    instanceVec[0] = vec3(0, 0, 0);
    // Define and upload geometry to the GPU here ...
    int cubeVAO = createTexturedCubeVertexArrayObject();

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
    //TODO: code shearing into separate movement function (since the shearing itself is not in the user's control, only the movement)
    previousKeyStates.insert(pair<int, KeyState>(GLFW_KEY_Q, { GLFW_RELEASE , false }));
    previousKeyStates.insert(pair<int, KeyState>(GLFW_KEY_E, { GLFW_RELEASE , false }));
    //toggle textures
	previousKeyStates.insert(pair<int, KeyState>(GLFW_KEY_X, { GLFW_RELEASE , true }));

	double lastMousePosX, lastMousePosY;
	glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

    // Other OpenGL states to set once
    // Enable Backface culling
    glEnable(GL_CULL_FACE);

    // @TODO 1 - Enable Depth Test
    // ...
    glEnable(GL_DEPTH_TEST);


    //Models
    //CharModel* selectedModel;
    CharModel* models[numMainModels];
    int modelIndex = 0;

    ModelV9 v9(shaderProgram);
    ModelS3 s3(shaderProgram);
    ModelA9 a9(shaderProgram);
    ModelN2 n2(shaderProgram);
    ModelN4 n4(shaderProgram);

    models[0] = &s3;
    models[1] = &n2;
    models[2] = &a9;
    models[3] = &n4;
    models[4] = &v9;

    //previous frame, if valid input to model.
    bool prevHadMovement = false;

    // Entering Main Loop
    while (!glfwWindowShouldClose(window))
    {
        // Frame time calculation
        float dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;

        // Each frame, reset color of each pixel to glClearColor
        // @TODO 1 - Clear Depth Buffer Bit as well
        // ...
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw geometry
        glBindBuffer(GL_ARRAY_BUFFER, cubeVAO);

        // Draw ground
        GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

        //draw grid
        glBindTexture(GL_TEXTURE_2D, 0);
        glUniform3f(colorLocation, 1.0f, 1.0f, 1.0f);
        glUniform1i(enableTextureLocation, 0);
        drawGrid(worldMatrixLocation, colorLocation, mat4(1.0f));
        // draw axis
        drawAxis(worldMatrixLocation, colorLocation);
        glUniform1i(enableTextureLocation, enableTexture);

        //draw tiles
        glBindTexture(GL_TEXTURE_2D, brickTextureID);
        glUniform3f(colorLocation, 0.8f, 0.4f, 0.8f);
        drawTileGrid(worldMatrixLocation, mat4(1.0f));
        
        //draw models
        {
            //selection with keys.
            drawControl(window);
            modelIndex = selectModelControl(window, modelIndex);
            CharModel* prevModel = selectedModel;
            selectedModel = models[modelIndex];
            //Control model key presses.
            mat4* relativeWorldMatrix = modelControl(window, dt, previousKeyStates);
            bool hasMovement = checkModelMovement(window, previousKeyStates);
            bool* selectedSetting = customControl(window, previousKeyStates);
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
            //Adjust selected model accordingly.
            selectedModel->addRelativeWorldMatrix(relativeWorldMatrix[0], relativeWorldMatrix[1], relativeWorldMatrix[2]);

            //checks whether the models move
            if (hasMovement) {
                //start walking
                selectedModel->walkState.setState(1);
            }
            else if (prevHadMovement){
                //stop walking
                selectedModel->walkState.setState(2);
            }
            prevHadMovement = hasMovement;

            //makes previous model stop walking
            if (prevModel != selectedModel && prevModel) {
                prevModel->walkState.setState(2);
            }

			//draw all models
			//glUniform3f(colorLocation, 1.0f, 233.0f / 255.0f, 0.0f);
			glUniform3f(colorLocation, 0.2f, 0.2f, 1.0f);

			glBindTexture(GL_TEXTURE_2D, boxTextureID);
			CharModel::drawLetter(models);
			glBindTexture(GL_TEXTURE_2D, metalTextureID);
			CharModel::drawNumber(models);
            //Sphere has no texture for now.
            glBindTexture(GL_TEXTURE_2D, 0);
            glUniform1i(enableTextureLocation, 0);
            CharModel::drawSphere(models);
            glUniform1i(enableTextureLocation, enableTexture);
            CharModel::update(models, dt);
		}


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
        (float) window_width / (float) window_height,  // aspect ratio
        0.01f, 100.0f);   // near and far (near > 0)
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

        // @TODO 5 = use camera lookat and side vectors to update positions with HNBM
        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) // move camera down
        {
            cameraPosition += cameraLookAt * currentCameraSpeed * dt;
        }
        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) // move camera up
        {
            cameraPosition -= cameraLookAt * currentCameraSpeed * dt;
        }
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) // move camera to the left
        {
            cameraPosition -= cameraSideVector * currentCameraSpeed * dt;
        }
        if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) // move camera to the right
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

        mat4 viewMatrix = mat4(1.0);

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
