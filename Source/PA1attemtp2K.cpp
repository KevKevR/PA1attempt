//
// Concordia University COMP 371 Programming Assignment 3, Summer 2020.
//
// Created by :
//    Jason Beccherini (40130107)
//    Manuel Romero (40023706)
//    Gia-Khang Ernest Nguyen (40091426)
//    Wing-Fei Jason Tiu (40048495)
//    Kevin Rao (40095427)
// due August 21st, 2020.
//

// For more detailed information about the program, consult the README.
//cube controls (temporary)
// select model 1 - WASD to move cube, ad to rotate.
// - - - - - - - - - - - - - - - - - - - - - - 
// - cv to rotate selected side
// - fg to select side to rotate
// - r to make a random rotation
// - ctrl+r to make many random rotations (press again or wait 100 turns to stop)
// - home to reset



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

#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <vector>
#include <time.h>

using namespace glm;
using namespace std;

GLuint loadTexture(const char* filename);

const int numMainModels = 7;
const int numAttachedModelsPerMain = 6;
const int numDigits = 11;

// Timer variables
int hours = 0, minutes = 0, seconds = 0, decimals = 0;
float timePause = 0, timeResume = 0, totalTimePaused = 0, timeReset = 0, currentTime;
bool paused = false;

// Define (initial) window width and height
int window_width = 1024, window_height = 768;

// Position of light source
//slightly offcenter, because the light doesn't like looking straight down.
vec3 lightPos = vec3(-10.0f, 30.0f, 30);
//vec3 lightPos = vec3(-2.0f, 4.0f, -1.0f);

// Callback function for handling window resize and key input
void window_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Define timer-related functions
string getTimerString();
void adjustTimer();
void handlePauseTimer();
void resetTimer();

// Forward declare CharModel class in order to declare a pointer to selected model to manipulate
class CharModel;
CharModel* selectedModel;
CharModel* selectedPartModel;

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
struct Color {
    float red;
    float green;
    float blue;
    Color() {
        red = 0;
        green = 0;
        blue = 0;
    }
    Color(float r, float g, float b) {
        red = r;
        green = g;
        blue = b;
    }
};
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
            walk(dt);
            break;
        }
        case 2:
        {
            //return to 0.
            returnFromWalk(dt);
            break;
        }
        case 0:
        default:
        {
            //stop
            stop(dt);
            break;
        }
        }
        //bound to approx. [-0.5f, 0.5f] interval.
        bound();
    }

    //allow walkState to change.
    virtual void setState(int s) {
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
    virtual float getPosition() {
        //position between [-1, 1].
        //return currentPosition * 2;
        return sinf(radians(180.0f * (currentPosition * 2)));
    }
protected:
    virtual void walk(float dt) {
        //walking, periodic movement from -0.5 to 0.5.
        //direction -1 or 1;
        const int direction = 1;
        currentPosition += dt / timePerState * direction;
    }
    /*virtual*/ void returnFromWalk(float dt) {
        //return to 0.
        int towardsCenter = sign(currentPosition);
        if (fabsf(currentPosition) < 0.25f) {
            towardsCenter *= -1;
        }
        currentPosition += copysignf(dt / timePerState, towardsCenter);

        //stop when returned to position 0.
        if (signbit(currentPosition) != signbit(stopWalkPosition)) {
            stop(dt);
            setState(0);
        }

        //stop when returned to position 0. timer in case.
        timeInState += dt;
        if (timeInState >= timePerState) {
            stop(dt);
            setState(0);
        }
    }
    /*virtual*/ void stop(float dt) {
        //stop
        currentPosition = 0;
    }

    virtual void bound() {
        //bound to approx. [-0.5f, 0.5f] interval.
        //positive modulo 
        //https://stackoverflow.com/questions/14997165/fastest-way-to-get-a-positive-modulo-in-c-c
        //currentPosition = fmodf(currentPosition + 0.5f, 1) -0.5f ;
        const float mod = 1;
        currentPosition = fmodf(mod + fmodf(currentPosition + 0.5f, mod), mod) - 0.5f;
    }

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
    //set direction to -1 or 1;
    int direction;
};
//derive to replace getPosition and reuse rest
class RotateCycle : public WalkCycle{
public:
    RotateCycle(float tPerState = 1) : WalkCycle(tPerState) { }

    float getPosition() {
        //float a = 2*currentPosition - 1;
        //const float smoothPos = a / (1 + abs(a)) + 0.5f;
        //const float smoothPos = 2 * currentPosition / (1 + currentPosition);
        //const float smoothPos = 2 * currentPosition - currentPosition * currentPosition;
        //derivative is parabola with value 0 at pos 0 and 1.
        //const float smoothPos = 3 * currentPosition * currentPosition - 2 * currentPosition * currentPosition * currentPosition;
        //modified. squared then restrict to portion.
        //const float offsetPos = currentPosition - 1;
        //const float smoothPos = 1-pow(3 * offsetPos * offsetPos + 2 * offsetPos * offsetPos * offsetPos, 2);
        //modified again, different portion of previous curve.
        const float smoothPos = (currentPosition * currentPosition * pow(currentPosition-3, 4))/16;
        //const float smoothPos = pow(currentPosition-1 ,5)+1;
        //const float smoothPos = 2*currentPosition/(1+abs(currentPosition));
        //positive modulo 
        //https://stackoverflow.com/questions/14997165/fastest-way-to-get-a-positive-modulo-in-c-c
        const float mod = -1;
        const float pos = fmodf(mod + fmodf(smoothPos, mod), mod);
        return pos;
        //return currentPosition;
    }
    void setState(int s) {
        WalkCycle::setState(s);
        //also reset position, to skip previous rotate and do from beginning.
        currentPosition = 0;
    }
    void walk(float dt) {
        WalkCycle::walk(dt);

        //stop rotation after it completes
        if (abs(currentPosition) >= 1.0f) {
            setState(0);

        }
    }
    void bound() {
        //do not bound
        // * technically, walk() already bounds the position in [-1, 1].
    }
};
//holds TRS matrices
class TRSMatricesHolder {
public:
    mat4 translateMatrix;   //Stored translate matrix
    mat4 rotateMatrix;      //Stored rotate matrix
    mat4 scaleMatrix;       //Stored scale matrix

    TRSMatricesHolder() {
        translateMatrix = mat4(1.0f);
        rotateMatrix = mat4(1.0f);
        scaleMatrix = mat4(1.0f);
    }
    TRSMatricesHolder(const TRSMatricesHolder &copy) {
        translateMatrix = copy.translateMatrix;
        rotateMatrix = copy.rotateMatrix;
        scaleMatrix = copy.scaleMatrix;
    }
    TRSMatricesHolder(mat4 t, mat4 r, mat4 s) {
        translateMatrix = t;
        rotateMatrix = r;
        scaleMatrix = s;
    }

    TRSMatricesHolder addMatrices(TRSMatricesHolder mh) {
        translateMatrix *= mh.translateMatrix;
        rotateMatrix *= mh.rotateMatrix;
        scaleMatrix *= mh.scaleMatrix;
        return *this;
    }

    mat4 trs() {
        return translateMatrix * rotateMatrix * scaleMatrix;
    }
};
struct MotionData {
    vec3 axisRotation;
};
class CharModel {
public:
    CharModel(int shaderProgram, TRSMatricesHolder init_relTRSMatrices = TRSMatricesHolder()) : color(Color()){
        init(init_relTRSMatrices);
        worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
        colorLocation = glGetUniformLocation(shaderProgram, "objectColor");

        initY = 0;

        //walking
        walkState = WalkCycle(1.0f);
        sphereOffset = { 0,0, 1 };

        //attached models
        cumulativeTRS = TRSMatricesHolder();

        //modeMotion = true;
        //data used in motion. For now, here is only for axis of rotation.
        motionD = {vec3(0,0,-1)};
    }

    //set color for model to use.
    void setColor(Color colorRGB) {
        color = colorRGB;
    }
    void setAttachedColor(Color colorRGB) {
        vector<CharModel*>::iterator it;
        for (it = attachedModels.begin(); it != attachedModels.end(); it++) {
            if (*it) {
                (*it)->setColor(colorRGB);
            }
        }
    }

    MotionData getMotionData() {
        return motionD;
    }
    //set information relevant to motion.
    void setAttachedMotionData(MotionData md) {
        vector<CharModel*>::iterator it;
        for (it = attachedModels.begin(); it != attachedModels.end(); it++) {
            if (*it) {
                (*it)->motionD = md;
            }
        }
    }

    float updateWalkProgress(float dt, int st = -1, float tState = -1) {
        //increment time
        getCycle()->stepForward(dt);

        //update walkState if set to do so.
        if (!(st < 0)) {
            getCycle()->setState(st);
        }
        //update time per walkState if set to do so.
        if (!(tState < 0)) {
            getCycle()->setTimePerState(tState);
        }

        //return position
        return getCycle()->getPosition();
    }
    GLuint swapWorldMatrixLocation(GLuint wml) {
        GLuint temp = worldMatrixLocation;
        worldMatrixLocation = wml;
        return temp;
    }

    //lazy getter. derived can overwrite this to return its own Cycle component.
    virtual WalkCycle* getCycle() {
        return &walkState;
    }

    //attached parts functions
    vector<CharModel*> getAttachedModels() {
        return attachedModels;
    }
    void setAttachedModels(vector<CharModel*> attM) {
        attachedModels = attM;
    }
    void resetCumulativeTRS() {
        cumulativeTRS = TRSMatricesHolder();
        resetAttachedCumulativeTRS();
    }
    void accumulateTRS(TRSMatricesHolder trs) {
        cumulativeTRS.addMatrices(trs);
    }
    void updateAttachedCumulativeTRS(){
        vector<CharModel*>::iterator it;
        //set cumulative matrices to attached models.
        TRSMatricesHolder tempMH = cumulativeTRS;
        tempMH.addMatrices(TRSMatricesHolder(getRelativeTranslateMatrix(), getRelativeRotateMatrix(), getRelativeScaleMatrix()));
        for (it = attachedModels.begin(); it != attachedModels.end(); it++) {
            if (*it) {
                (*it)->accumulateTRS(tempMH);
                (*it)->updateAttachedCumulativeTRS();
            }
        }
    }
    void drawAttachedModels(int part) {
        vector<CharModel*>::iterator it;
        //draw a part of every attached model
        switch (part) {
        case 0:
            //Letter
        {
            for (it = attachedModels.begin(); it != attachedModels.end(); it++) {
                if (*it) {
                    (*it)->drawLetter();
                    (*it)->drawAttachedModels(part);
                }
            }
            break;
        }
        case 1:
            //Number
        {
            for (it = attachedModels.begin(); it != attachedModels.end(); it++) {
                if (*it) {
                    (*it)->drawNumber();
                    (*it)->drawAttachedModels(part);
                }
            }
            break;
        }
        case 2:
            //Sphere
        {
            for (it = attachedModels.begin(); it != attachedModels.end(); it++) {
                if (*it) {
                    (*it)->drawSphere();
                    (*it)->drawAttachedModels(part);
                }
            }
            break;
        }
        case 3:
            //draw
        {
            for (it = attachedModels.begin(); it != attachedModels.end(); it++) {
                if (*it) {
                    (*it)->draw();
                    (*it)->drawAttachedModels(part);
                }
            }
            break;
        }
        //etc
        }
    }

    //TRS functions
    mat4 getRelativeTranslateMatrix() {
        mat4 motion = getMotion(getCyclePosition()).translateMatrix;
        return relativeTranslateMatrix * motion;
    }
    void setRelativeTranslateMatrix(mat4 relTWM) {
        relativeTranslateMatrix = relTWM;
    }
    void addRelativeTranslateMatrix(mat4 relTWM) {
        relativeTranslateMatrix = relTWM * relativeTranslateMatrix;
    }

    mat4 getRelativeRotateMatrix() {
        mat4 motion = getMotion(getCyclePosition()).rotateMatrix;
        return motion * relativeRotateMatrix;
    }
    void setRelativeRotateMatrix(mat4 relRWM) {
        relativeRotateMatrix = relRWM;
    }
    void addRelativeRotateMatrix(mat4 relRWM) {
        relativeRotateMatrix = relRWM * relativeRotateMatrix;
    }

    //return scale matrix without shear elements
    mat4 getRelativeUndeformedScaleMatrix() {
        //mat4 motion = getMotion(getCyclePosition());
        mat4 scale = relativeScaleMatrix/* * motion*/;

        //float heightOffset = sphereOffset.yOffset;
        //float sideMovement = motion[1].x * heightOffset;
        //float verticalMovement = (motion[1].y - 1) * heightOffset;
        //mat4 follow = translate(mat4(1.0f),
        //    vec3(sideMovement,
        //        verticalMovement,
        //        0));

        //float minScale = min(min(relativeScaleMatrix[0].x, relativeScaleMatrix[1].y), relativeScaleMatrix[2].z);
        float minScale = fmin(fmin(scale[0].x, scale[1].y), scale[2].z);
        mat4 diagonal = mat4(minScale, 0, 0, 0,
            0, minScale, 0, 0,
            0, 0, minScale, 0,
            0, 0, 0, 1);

        return diagonal/* * follow*/;
    }
    mat4 getRelativeScaleMatrix() {
        mat4 motion = getMotion(getCyclePosition()).scaleMatrix;
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
        return cumulativeTRS.trs() * getRelativeTranslateMatrix() * getRelativeRotateMatrix() * getRelativeScaleMatrix();
        //return getRelativeTranslateMatrix() * getRelativeRotateMatrix() * getRelativeScaleMatrix();
        //return getRelativeTranslateMatrix() * getRelativeRotateMatrix() * getRelativeScaleMatrix() * cumulativeTRS.trs();
        //return getRelativeTranslateMatrix() * cumulativeTRS.translateMatrix * getRelativeRotateMatrix() * cumulativeTRS.rotateMatrix * getRelativeScaleMatrix()* cumulativeTRS.scaleMatrix;
    }
    //return without shear elements
    mat4 getRelativeUndeformedWorldMatrix() {
        return cumulativeTRS.trs() * getRelativeTranslateMatrix() * getRelativeRotateMatrix() * getRelativeUndeformedScaleMatrix();
    }
    //reverts TRS matrices back to initial settings.
    void resetInitialRelativeMatrices() {
        setRelativeWorldMatrix(initial_relativeTranslateMatrix, initial_relativeRotateMatrix, initial_relativeScaleMatrix);
    }

    //Derived class functions.
    virtual void next() {
        //implement in derived class please.
    }
    virtual void prev() {
        //implement in derived class please.
    }
    virtual void draw() {
        //implement in derived class please.
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
    virtual void reset() {
        resetInitialRelativeMatrices();
        updateWalkProgress(0,0);
        vector<CharModel*>::iterator it;
        for (it = attachedModels.begin(); it != attachedModels.end(); it++) {
            if (*it) {
                (*it)->reset();
            }
        }
    }
    //Timer stuff: to display timer with passed string, or with chars, depending on implementation.
    virtual void selectMode(string) {
        //implement in derived class please.
        //mostly for border model (contains digit models, so will decompose string into chars to distribute).
    }
    virtual void selectMode(char) {
        //implement in derived class please.
        //mostly for digit model.
    }

    void attachedNext() {
        vector<CharModel*>::iterator it;
        for (it = attachedModels.begin(); it != attachedModels.end(); it++) {
            if (*it) {
                (*it)->next();
            }
        }
    }
    void attachedPrev() {
        vector<CharModel*>::iterator it;
        for (it = attachedModels.begin(); it != attachedModels.end(); it++) {
            if (*it) {
                (*it)->prev();
            }
        }
    }
    //get rubik properties
    void setFace(int f) {
        face = f;
    }
    void setPosIndex(int pos) {
        posIndex = pos;
    }

	//Class method to draws all passed models. (issues when unloaded models).
    static void drawLetter(vector<CharModel*> arr) {
        vector<CharModel*>::iterator it;
        for (it = arr.begin(); it != arr.end(); it++) {
            if (*it) {
                (*it)->drawLetter();
                (*it)->drawAttachedModels(0);
            }
        }
    }
    static void draw(vector<CharModel*> arr) {
        vector<CharModel*>::iterator it;
        for (it = arr.begin(); it != arr.end(); it++) {
            if (*it) {
                (*it)->draw();
                (*it)->drawAttachedModels(3);
            }
        }
	}
    static void drawNumber(vector<CharModel*> arr) {
        vector<CharModel*>::iterator it;
        for (it = arr.begin(); it != arr.end(); it++) {
            if (*it) {
                (*it)->drawNumber();
                (*it)->drawAttachedModels(1);
            }
        }
    }
    static void drawSphere(vector<CharModel*> arr) {
        vector<CharModel*>::iterator it;
        for (it = arr.begin(); it != arr.end(); it++) {
            if (*it) {
                (*it)->drawSphere();
                (*it)->drawAttachedModels(2);
            }
        }
    }
    //walking
    static vector<int> update(vector<CharModel*> arr, float dt) {
        vector<CharModel*>::iterator it;
        vector<int> positions;
        for (it = arr.begin(); it != arr.end(); it++) {
            if (*it) {
                positions.push_back((*it)->updateWalkProgress(dt));
            }
        } 
        return positions;
    }
    //attached models
    static void updateAttachedCumulativeTRS(vector<CharModel*> arr) {
        vector<CharModel*>::iterator it;
        for (it = arr.begin(); it != arr.end(); it++) {
            if (*it) {
                (*it)->updateAttachedCumulativeTRS();
            }
        }
    }
    static void resetCumulativeTRS(vector<CharModel*> arr) {
        vector<CharModel*>::iterator it;
        for (it = arr.begin(); it != arr.end(); it++) {
            if (*it) {
                (*it)->resetCumulativeTRS();
            }
        }
    }
    //used for instancing rubik
    static vector<mat4> getTRS(vector<CharModel*> arr) {
        vector<CharModel*>::iterator it;
        vector<mat4> trsMatrices(0);
        for (it = arr.begin(); it != arr.end(); it++) {
            if (*it) {
                trsMatrices.push_back((*it)->getRelativeWorldMatrix());
            }
        }
        return trsMatrices;
    }
    //used for instancing rubik
    static vector<int> getFace(vector<CharModel*> arr) {
        vector<CharModel*>::iterator it;
        vector<int> faces(0);
        for (it = arr.begin(); it != arr.end(); it++) {
            if (*it) {
                faces.push_back((*it)->face);
            }
        }
        return faces;
    }
    //used for instancing rubik
    static vector<int> getPosIndex(vector<CharModel*> arr) {
        vector<CharModel*>::iterator it;
        vector<int> pos(0);
        for (it = arr.begin(); it != arr.end(); it++) {
            if (*it) {
                pos.push_back((*it)->posIndex);
            }
        }
        return pos;
    }
    //used for instancing rubik
    static vector<vec3> getColor(vector<CharModel*> arr) {
        vector<CharModel*>::iterator it;
        vector<vec3> colors(0);
        for (it = arr.begin(); it != arr.end(); it++) {
            if (*it) {
                Color tempC = (*it)->color;
                colors.push_back(vec3(tempC.red, tempC.green, tempC.blue));
            }
        }
        return colors;
    }

    static GLuint swapWorldMatrixLocation(vector<CharModel*> arr, GLuint wml) {
        vector<CharModel*>::iterator it;
        GLuint temp = arr[0]->worldMatrixLocation;
        for (it = arr.begin(); it != arr.end(); it++) {
            if (*it) {
                temp = (*it)->swapWorldMatrixLocation(wml);
            }
        }
        return temp;
    }
    // Return initial y-position of model
    float getInitY() {return initY;}
    
protected:
    float getCyclePosition() {
        return getCycle()->getPosition();
    }

    //initializes some values.
    void init(TRSMatricesHolder ini_relTRSMatrices) {
        initial_relativeTranslateMatrix = ini_relTRSMatrices.translateMatrix;
        initial_relativeRotateMatrix = ini_relTRSMatrices.rotateMatrix;
        initial_relativeScaleMatrix = ini_relTRSMatrices.scaleMatrix;

        relativeTranslateMatrix = initial_relativeTranslateMatrix;
        relativeRotateMatrix = initial_relativeRotateMatrix;
        relativeScaleMatrix = initial_relativeScaleMatrix;
    }
    //attached models
    static void drawAttachedModels(vector<CharModel*> arr, int part) {
        vector<CharModel*>::iterator it;
        for (it = arr.begin(); it != arr.end(); it++) {
            if (*it) {
                (*it)->drawAttachedModels(part);
            }
        }
    }
    void resetAttachedCumulativeTRS() {
        vector<CharModel*>::iterator it;
        for (it = attachedModels.begin(); it != attachedModels.end(); it++) {
            if (*it) {
                (*it)->resetCumulativeTRS();
            }
        }
    }

    //gives motion
    virtual TRSMatricesHolder getMotion(float position) {
        //return modeMotion ? walkMotion(position) : rotateMotion(position);
        return walkMotion(position);
    }
    //shear motion when walking
    TRSMatricesHolder walkMotion(float position) {
        const float amplitude = 4;
        //const float position = getCyclePosition();

        //shear side-to-side + vertical scaling.
        mat4 motion =
            mat4(1, 0, 0, 0,  // first column
            //amplitude * sinf(radians(180.0f*position)), 1, 0, 0,  // second column
            amplitude * position, 1 + sqrt(amplitude) * powf(position, 2), 0, 0,  // second column
            0, 0, 1, 0,  // third column
            0, 0, 0, 1); // fourth column
        TRSMatricesHolder(tempT);
        tempT.scaleMatrix = motion;
        return tempT;
    }
    mat4 sphereFollow() {
        //untested besides scale component
        TRSMatricesHolder motion = getMotion(getCyclePosition());

        float heightOffset = sphereOffset.yOffset;
        float sideMovement = motion.scaleMatrix[1].x * heightOffset;
        float verticalMovement = (motion.scaleMatrix[1].y - 1) * heightOffset;
        mat4 follow = motion.translateMatrix * translate(mat4(1.0f),
            vec3(sideMovement,
                verticalMovement,
                0))
            * motion.rotateMatrix;
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
        const int heightParts = 22;
        const int ringParts = 30;
        const int numVertices = (heightParts - 1) * ringParts * 2;
        glDrawArrays(GL_TRIANGLE_STRIP, 0, numVertices);
    }
    //bool modeMotion;

    //let other class handle walking.
    WalkCycle walkState;

    GLuint worldMatrixLocation;
    GLuint colorLocation;
    mat4 initial_relativeTranslateMatrix;   //Initial translate matrix, value to take when reset.
    mat4 initial_relativeRotateMatrix;      //Initial rotate matrix, value to take when reset.
    mat4 initial_relativeScaleMatrix;       //Initial scale matrix, value to take when reset.
    float initY;                            // Initial y-position in initial translate matrix (note: value assigned to child constructor)
    int face;       //boxes' face
    int posIndex;   //boxes' position index


    //information for motion.
    MotionData motionD;
    //object color
    Color color;

    SphereOffset sphereOffset;
    //cumulating TRS of models this is attached to.
    TRSMatricesHolder cumulativeTRS;
    //models attached to this.
    vector<CharModel*> attachedModels;
private:
    mat4 relativeTranslateMatrix;   //Stored translate matrix
    mat4 relativeRotateMatrix;      //Stored rotate matrix
    mat4 relativeScaleMatrix;       //Stored scale matrix
};

//placeholder border for now, can change stuff later.
class Model_Border : public CharModel {
public:
    Model_Border(int shaderProgram, TRSMatricesHolder ini_relTRSMatrices = TRSMatricesHolder()) : CharModel(shaderProgram, ini_relTRSMatrices) {
        //let super do the initializations.
    }

    void selectMode(string time) {
        vector<CharModel*>::iterator it;
        //index i from 0 to number of digits attached to iterate on, in case needed. delete if not necessary. 
        int i = 0;
        for (it = attachedModels.begin(); it != attachedModels.end(); it++, i++) {
            if (*it) {
                //separate string to char
                char digit = time[i];
                (*it)->selectMode(digit);
            }
        }
    }

    void drawNumber() {
        //border's constants
        //for now, hard-coded constants
        const float width_base_model = 2;
        const float spacing = (0.24f + 0.2f) * width_base_model;
        const float total_width = width_base_model * numDigits + spacing * (numDigits);
        const float total_height = 2 * width_base_model + spacing;
        const float border_thickness = width_base_model / 5;
        const float correctiveXOffset = -1.4f;
        //skate constant
        const float skate_height = border_thickness;
        mat4 borderPosition = translate(mat4(1.0f),
            vec3(-total_width / 2 + (numDigits + 1) / 2 * (width_base_model + spacing) + correctiveXOffset,
                total_height / 2 + border_thickness + skate_height,
                0))
            //lessen the depth
            * scale(mat4(1.0f),
                vec3(1.0f, 1.0f, 0.25f));
        drawBorder(total_height, total_width, border_thickness, border_thickness, mat4(1.0f), getRelativeWorldMatrix() * borderPosition);
    }
protected:
private:
};
class SkyBox : public CharModel {
public:
    SkyBox(int shaderProgram, int vAO, TRSMatricesHolder ini_relTRSMatrices = TRSMatricesHolder()) : CharModel(shaderProgram, ini_relTRSMatrices) {
        //let super do the initializations.
        mode = false;
        sphereOffset = { 0,0, 75 };//origin and 75 radius, specifications
        vao = vAO;
    }

    void next() {
        mode = !mode;
    }
    void prev() {
        next();
    }
    void draw() {
        glBindVertexArray(vao);
        //do not let model be transformed.
        (!mode) ? drawSemiSphere(mat4(1.0f)) : drawBox(mat4(1.0f));
    }

protected:
    void drawSemiSphere(mat4 relativeWorldMatrix) {
        // Draw sphere
        const float yHover = 0.0f;

        const float xOffset = sphereOffset.xOffset;
        const float yOffset = sphereOffset.yOffset + yHover;
        const float scaler = sphereOffset.scaler;

        mat4 worldMatrix = translate(mat4(1.0f), vec3(xOffset, yOffset, 0.0f)) * scale(mat4(1.0f), glm::vec3(-scaler, -scaler, -scaler));
        mat4 mWorldMatrix = relativeWorldMatrix * worldMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);


        //match these numbers to those passed to sphereVertices().
        const int heightParts = 22;
        const int ringParts = 30;
        const int numVertices = (heightParts - 1) * ringParts * 8;
        glDrawArrays(GL_TRIANGLE_STRIP, 0, numVertices / 2);
    }
    void drawBox(mat4 relativeWorldMatrix) {
        // Draw box
        //omit top, then scale top by -0.5f.
        const float yHover = 0.0f;

        const float xOffset = sphereOffset.xOffset;
        const float yOffset = sphereOffset.yOffset + yHover;
        const float scaler = sphereOffset.scaler;

        mat4 worldMatrix = translate(mat4(1.0f), vec3(xOffset, yOffset, 0.0f)) * scale(mat4(1.0f), glm::vec3(-scaler, -0.5f * scaler, -scaler));
        mat4 mWorldMatrix = relativeWorldMatrix * worldMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);

        glDrawArrays(GL_TRIANGLES, 0, 30);
    }
    bool mode;
    int vao;
};
class Model_DigitalFont : public CharModel {
public:
    Model_DigitalFont(int shaderProgram, TRSMatricesHolder ini_relTRSMatrices = TRSMatricesHolder()) : CharModel(shaderProgram, ini_relTRSMatrices), toDrawTop{}, toDrawBot{} {
        //let super do the initializations.
        init_charToDraw = '0';   //default char to make compiler happy.
        charToDraw = init_charToDraw;
    }
    Model_DigitalFont(int shaderProgram, char letter, TRSMatricesHolder ini_relTRSMatrices = TRSMatricesHolder()) : CharModel(shaderProgram, ini_relTRSMatrices) {
        //let super do the initializations.
        init_charToDraw = letter;
        charToDraw = init_charToDraw;
        selectDraw(letter);
    }

    void drawNumber() {
        //set color to draw with, and texture.
        //glUniform3f(colorLocation, color.red, color.green, color.blue);
        //glBindTexture(GL_TEXTURE_2D, textureID);
        drawDigitalFont();
    }
    //select display next character 
    void next() {
        selectNext();
        selectDraw(charToDraw);
    }
    //select display previous character
    void prev() {
        selectPrev();
        selectDraw(charToDraw);
    }
    void reset() {
        charToDraw = init_charToDraw;
        resetInitialRelativeMatrices();
        selectDraw(charToDraw);
    }
    //select display character
    void selectMode(char cTD) {
        setCharToDraw(cTD);
        selectDraw(charToDraw);
    }
protected:

    //set char to draw
    void setCharToDraw(char cTD) {
        charToDraw = cTD;
    }
    void selectNext() {
        if (charToDraw == '9') {
            charToDraw = 'A';
        }
        else if (toupper(charToDraw) != 'Z') {
            charToDraw += 1;
        }
    }
    void selectPrev() {
        if (toupper(charToDraw) == 'A') {
            charToDraw = '9';
        }
        else if (charToDraw != '0') {
            charToDraw -= 1;
        }
    }
    //void Model_DigitalFont::drawBar(mat4 inertialWorldMatrix);
    //void Model_DigitalFont::drawBarWithSlits(mat4 inertialWorldMatrix);
    void drawBar(mat4 inertialWorldMatrix);
    void drawBarWithSlits(mat4 inertialWorldMatrix);
    //Draw "8"-styled digital clock placements. bool array inputs to control which portions to draw.
    void drawDigitalFont() {
        //bool toDrawTop[6] = {true, true , true , true , true , true }; //order: 0top, 1right, 2bottom, 3left, 4main diagonal, 5secondary diagonal. Top half.
        //bool toDrawBot[6] = {};                                        //order: 0top, 1right, 2bottom, 3left, 4main diagonal, 5secondary diagonal. Bottom half.
        // ^ ^ ^ note that top's 2bottom and bottom's 0top will draw the same location.
        // That's why bottom's 0top will be hard-coded skipped, regardless of value inputted.

        mat4 relativeWorldMatrix = getRelativeWorldMatrix();
        //code goes here
        const float dimensionMult = 1.2f; //Since, for the bars (with or without slits), the diagonals allow shorter lengths than grid length (when taking width into consideration), the bars will touch when this value is below (0.75*sqrt(2) ~ 1.061f).
        //(x, y, z), x is base bar + 2 * slits (even though slit takes 1/4 square surface (1/2 in 1D), treat is as whole square to overlap easier)
        const float objectDimensions[3] = { (1 + 0.5f * 2) * dimensionMult, 0.5f * dimensionMult, 0.25f * dimensionMult }; //Treat as if the bars were dimensionMult% as large. (creates spacing in real model if these were to touch)
        const int corners = 4;      //Square shape


        //Draw "8"-styled digital clock placements.
        //total height is objectDimensions[0] * 2 - objectDimensions[1].
        //total width is objectDimensions[0] + objectDimensions[1]

        //recycling function to draw the '9' of V9 model(mine) from PA1 
        //Then only thing to change is angles to draw on + make diagonals at centers.
        //total height is heightScale
        //total width is 2 * apothem
        const float heightScale = objectDimensions[0] * 2 - objectDimensions[1];    //total height of letter (don't fix what ain't broke)
        const float m9_apothem = objectDimensions[0] / 2;     //center to outside edge

        const float thickness = objectDimensions[1];        //outside edge to inner edge
        const float m9_centralAngle = radians(360.0f / corners);

        mat4 inertialWorldMatrix = mat4(1.0f);
        //Offset for placements to center the model.
        //modelPositioningMatrix =
        //    translate(mat4(1.0f),
        //        vec3(letterHalfWidth,
        //            -(m_height / 2),
        //            0));

        //Draw in shape of regular polygons.
        for (int i = 0; i < corners; ++i)
        {
            const float angle_i = m9_centralAngle * i;
            //const float thickness = widthScale;

            //draw head
            if (toDrawTop[i]) {
                inertialWorldMatrix =
                    //top height, place center point of polygon.
                    translate(mat4(1.0f),
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
                            0));
                //no need to scale, as the object is in its final shape.
                //* scale(mat4(1.0f),
                //    vec3(m9_base, thickness, 1.0f));
                drawBarWithSlits(inertialWorldMatrix);


            }
            //draw tail

            if (i == 0) {
                //this would draw a bar already controlled by the top half, so skip it and let top draw it if needed.
                continue;
            }
            if (toDrawBot[i]) {
                inertialWorldMatrix =
                    //bottom height, center of polygon.
                    translate(mat4(1.0f),
                        vec3(0,
                            m9_apothem,
                            0))
                    //rotate by angle_i. done after translating to simplify process.
                    * rotate(mat4(1.0f),
                        angle_i,
                        vec3(0.0f, 0.0f, -1.0f))
                    //apothem distance.
                    * translate(mat4(1.0f),
                        vec3(0,
                            m9_apothem - thickness / 2,
                            0));
                //no need to scale, as the object is in its final shape.
                //scale to sides of polygon match target.
                //* scale(mat4(1.0f),
                //    vec3(m9_base, thickness, 1.0f));
                drawBarWithSlits(inertialWorldMatrix);
            }
        }
        //draw cell diagonals
        for (int i = 4; i < 6; i++) {
            const int parity = (int)pow(-1, i + 1);
            //top diagonals
            if (toDrawTop[i]) {
                inertialWorldMatrix =
                    //top height, center of polygon.
                    translate(mat4(1.0f),
                        vec3(0, heightScale - m9_apothem, 0))
                    //rotate by 45.
                    * rotate(mat4(1.0f),
                        radians(45.0f),
                        vec3(0.0f, 0.0f, parity));
                //no need to scale, as the object is in its final shape.
                drawBarWithSlits(inertialWorldMatrix);
            }

            //bottom diagonals
            if (toDrawBot[i]) {
                inertialWorldMatrix =
                    //bottom height, center of polygon.
                    translate(mat4(1.0f),
                        vec3(0, m9_apothem, 0))
                    //rotate by 45.
                    * rotate(mat4(1.0f),
                        radians(45.0f),
                        vec3(0.0f, 0.0f, parity));
                //no need to scale, as the object is in its final shape.
                drawBarWithSlits(inertialWorldMatrix);
            }
        }

        //pattern to draw models to make use of relativeWorldMatrix:
        //worldMatrix = relativeWorldMatrix * (model's original matrices);
        //glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        //glUniform3f(colorLocation, 1.0f, 233.0f / 255.0f, 0.0f);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    char charToDraw;
    char init_charToDraw;
    bool toDrawTop[6];
    bool toDrawBot[6];

private:
        void selectDraw(char);
};
//draw the main bar for the digital font, size according to specifications.
void Model_DigitalFont::drawBar(mat4 inertialWorldMatrix) {
    mat4 modelMatrix = mat4(1.0f);
    //main bar
    modelMatrix =
        scale(mat4(1.0f),
            vec3(1.0f, 0.5f, 0.25f));   //specification dimensions
    mat4 worldMatrix = getRelativeWorldMatrix() * inertialWorldMatrix * modelMatrix;
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
//draw slits(? vocabulary. the protrusion on the sides) along with the main bar.
void Model_DigitalFont::drawBarWithSlits(mat4 inertialWorldMatrix) {
    mat4 modelMatrix = mat4(1.0f);
    //main bar
    drawBar(inertialWorldMatrix);
    const float height_bar = 0.5f;
    const float sideSlit = sqrt(2 * powf(height_bar, 2)) / 2;
    //slits to add on each side
    for (int i = 0; i < 2; i++) {
        const int parity = (int)pow(-1, i + 1);

        modelMatrix =
            //center on side's surface
            translate(mat4(1.0f),
                vec3(0.5f * parity, 0.0f, 0.0f))
            //rotate to get triangular protrusion
            * rotate(mat4(1.0f),
                radians(45.0f),
                vec3(0.0f, 0.0f, -parity))
            //scale to match bar's size
            * scale(mat4(1.0f),
                vec3(sideSlit, sideSlit, 0.25f));

        mat4 worldMatrix = getRelativeWorldMatrix() * inertialWorldMatrix * modelMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}
// Selects which parts of the 11-segment display to light up, depending on the selected char. 
//undefined selection in case selected char is not in domain ([0-9][A-Z][a-z]). (most likely retains previous selection.)
//https://stackoverflow.com/questions/12328301/cleanest-way-to-copy-a-constant-size-array-in-c11/12328330#12328330
void Model_DigitalFont::selectDraw(char c) {
    switch (toupper(c)) {
    case 'A':
    {
        toDrawTop[0] = true;        //top
        toDrawTop[1] = true;        //right
        toDrawTop[2] = true;        //bot
        toDrawTop[3] = true;        //left
        toDrawTop[4] = false;        //main diagonal
        toDrawTop[5] = false;       //secondary diagonal

        //toDrawBot[0] = false;     //top
        toDrawBot[1] = true;        //right
        toDrawBot[2] = false;       //bot
        toDrawBot[3] = true;        //left
        toDrawBot[4] = false;        //main diagonal
        toDrawBot[5] = false;       //secondary diagonal
        break;
    }
    //for v1 of 'B', see current '8'.
    //v2 small 'b'.
    case 'B':   //small 'b' used instead
    {
        bool top[6] = { false , false, true, true, false, false };
        bool bot[6] = { false, false, false, true, false, true };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    case 'C':
    {
        bool top[6] = { true, false, false, true, false, false };
        bool bot[6] = { false, false, true, true, false, false };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    case 'D':
    {
        bool top[6] = { false, false, false, true, true, false };
        bool bot[6] = { false, false, false, true, false, true };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    case 'E':
    {
        bool top[6] = { true, false, true, true, false, false };
        bool bot[6] = { false, false, true, true, false, false };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    case 'F':
    {
        bool top[6] = { true, false, true, true, false, false };
        bool bot[6] = { false, false, false, true, false, false };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    case 'G':   // G needs short center segment
    {
        bool top[6] = { true, false, false, true, false, false };
        bool bot[6] = { false, true, true, true, false, true };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    case 'H':
    {
        bool top[6] = { false, true, true, true, false, false };
        bool bot[6] = { false, true, false, true, false, false };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    //for v1 of 'I', see current case of '1'.
    //v2 left side I
    case 'I':   //sans-serif. only option is maybe make one shorter, or on opposite sides.
    {
        bool top[6] = { false, false, false, true, false, false };
        bool bot[6] = { false, false, false, true , false, false };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    case 'J':
    {
        bool top[6] = { false, true, false, false, false, false };
        bool bot[6] = { false, true, true, false , false, false };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    case 'K':
    {
        bool top[6] = { false, false, false, true, false, true };
        bool bot[6] = { false, false, false, true , true, false };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    case 'L':
    {
        bool top[6] = { false, false, false, true, false, false };
        bool bot[6] = { false, false, true, true , false, false };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    case 'M':   //M is missing its inner teeth
    {
        bool top[6] = { true, true, false, true, false, true };
        bool bot[6] = { false, true, false, true , false, false };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    case 'N':
    {
        bool top[6] = { false, true, false, true, true, false };
        bool bot[6] = { false, true, false, true , false, false };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    //case 'O':
    //{
    //    bool top[6] = { true, true, false, true, false, false };
    //    bool bot[6] = { false, true, true, true, false, false };
    //    copy(top, top + 6, toDrawTop);
    //    copy(bot, bot + 6, toDrawBot);
    //    break;
    //}
    ////v2 small o
    case 'O':
    {
        bool top[6] = { false, false, true, false, false, false };
        bool bot[6] = { false, true, true, true, false, false };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    case 'P':
    {
        bool top[6] = { true, true, true, true, false, false };
        bool bot[6] = { false, false, false, true, false, false };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    case 'Q': // diagonal need to be smaller
    {
        bool top[6] = { true, true, false, true, false, false };
        bool bot[6] = { false, true, true, true, true, false };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    case 'R':
    {
        bool top[6] = { true, true, true, true, false, false };
        bool bot[6] = { false, false, false, true, true, false };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    case 'S':       //small 's'
    {
        bool top[6] = { false, false, true, false, false, true };
        bool bot[6] = { false, false, false, false, false, true };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    case 'T':       //needs middle collumn
    {
        bool top[6] = { true, false, false, true, false, false };
        bool bot[6] = { false, false, false, true, false, false };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    case 'U':
    {
        bool top[6] = { false, true, false, true, false, false };
        bool bot[6] = { false, true, true, true, false, false };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    case 'V':
    {
        bool top[6] = { false, true, false, true, false, false };
        bool bot[6] = { false, false, false, true, false, true };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    case 'W':       //needs middle teeth/collumn
    {
        bool top[6] = { false, true, false, true, false, false };
        bool bot[6] = { false, true, true, true, true, false };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    //v1 straight edge
    case 'X':
    {
        bool top[6] = { false, true, false, false, true, false };
        bool bot[6] = { false, true, false, false, false, true };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    ////v2 small 'x'
    //case 'X':
    //{
    //    bool top[6] = { false, false, false, false, false, false };
    //    bool bot[6] = { false, false , false, false, true, true };
    //    copy(top, top + 6, toDrawTop);
    //    copy(bot, bot + 6, toDrawBot);
    //    break;
    //}
    ////v1 small case 'y'
    //case 'Y':
    //{
    //    bool top[6] = { false, true, true, true, false, false };
    //    bool bot[6] = { false, true, true, false, false, false };
    //    copy(top, top + 6, toDrawTop);
    //    copy(bot, bot + 6, toDrawBot);
    //    break;
    //}
    case 'Y':
    {
        bool top[6] = { false, true, false, false, true, false };
        bool bot[6] = { false, true, false, false, false, false };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    case 'Z':   //small 'z' used
    {
        bool top[6] = { false, false, true, false, false, false };
        bool bot[6] = { false, false, true, false, false, true };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }

    //v1 similar to O
    case '0':
    {
        bool top[6] = { true, true, false, true, false, false };
        bool bot[6] = { false, true, true, true, false, false };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    //// v2 parallelogram
    //case '0':
    //{
    //    bool top[6] = { false, true, false, false, false, true};
    //    bool bot[6] = { false, false, false, true, false, true };
    //    copy(top, top + 6, toDrawTop);
    //    copy(bot, bot + 6, toDrawBot);
    //    break;
    //}

    case '1':
    {
        bool top[6] = { false, true, false, false, false, false };
        bool bot[6] = { false, true, false, false , false, false };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    case '2':
    {
        bool top[6] = { true, true, true, false, false, false };
        bool bot[6] = { false, false, true, true, false, false };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    case '3':
    {
        bool top[6] = { true, true, true, false, false, false };
        bool bot[6] = { false, true, true, false, false, false };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    case '4':
    {
        bool top[6] = { false, true, true, true, false, false };
        bool bot[6] = { false, true, false , false, false, false };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    case '5':
    {
        bool top[6] = { true, false, true, true, false, false };
        bool bot[6] = { false, true, true, false, false, false };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    case '6':
    {
        bool top[6] = { true, false, true, true, false, false };
        bool bot[6] = { false, true, true, true, false, false };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    case '7':
    {
        bool top[6] = { true, true, false, false, false, false };
        bool bot[6] = { false, false, false, false, false, true };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    case '8':   //old 'B' code
        toDrawTop[0] = true;        //top
        toDrawTop[1] = true;        //right
        toDrawTop[2] = true;        //bot
        toDrawTop[3] = true;        //left
        toDrawTop[4] = false;        //main diagonal
        toDrawTop[5] = false;       //secondary diagonal

        //toDrawBot[0] = false;     //top
        toDrawBot[1] = true;        //right
        toDrawBot[2] = true;       //bot
        toDrawBot[3] = true;        //left
        toDrawBot[4] = false;        //main diagonal
        toDrawBot[5] = false;       //secondary diagonal
        break;
    case '9':
    {
        bool top[6] = { true, true, true, true, false, false };
        bool bot[6] = { false, true, true, false, false, false };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
    case '-':
        bool top[6] = { false, false, true, false, false, false };
        bool bot[6] = { false, false, false, false, false, false };
        copy(top, top + 6, toDrawTop);
        copy(bot, bot + 6, toDrawBot);
        break;
    }
}
class ModelBox : public CharModel {
public:
    ModelBox(int shaderProgram, TRSMatricesHolder ini_relTRSMatrices = TRSMatricesHolder())
        : CharModel(shaderProgram, ini_relTRSMatrices){
        
        //replace motion with rotation
        // or could use own attribute instead.
        rotateState = RotateCycle(0.6f);

        //angle to rotate towards.
        targetAngle = 0;
    }
    //replaced with instanced draw inside main.
    void draw() {
        //pass arguments stored in parent class.
        //glUniform3f(colorLocation, 0.0f, 233.0f / 255.0f, 1.0f);
        glUniform3f(colorLocation, color.red, color.green, color.blue);
        drawCube(worldMatrixLocation, colorLocation, getRelativeWorldMatrix());
        //drawCube(worldMatrixLocation, colorLocation, getRelativeWorldMatrix() * cubeOffset * cumulativeTRS.trs());
    }
    void next() {
        targetAngle = 90.0f;
        //start walking
        updateWalkProgress(0, 1);
        //rotate next state
        addRelativeRotateMatrix(
            rotate(mat4(1.0f),
                radians(90.0f),
                motionD.axisRotation));
    }
    void prev() {
        targetAngle = -90.0f;
        //start walking
        updateWalkProgress(0, 1);
        //rotate nexte state
        addRelativeRotateMatrix(
            rotate(mat4(1.0f),
                radians(-90.0f),
                motionD.axisRotation));
    }
    void reset() {
        CharModel::reset();
        targetAngle = 0;
    }
    WalkCycle* getCycle() {
        return &rotateState;
    }
protected:
    TRSMatricesHolder getMotion(float position) {
        return rotateMotion(position);
    }
    TRSMatricesHolder rotateMotion(float position) {
        //position should be between 0 to 1.
        const float amplitude = 4;
        const float rotationAngle = targetAngle;
        //const float position = updateWalkProgress(0);
    
        //rotate along its axis CW or CCW.
        mat4 motion =
            rotate(mat4(1.0f),
                radians(rotationAngle) * (position),
                motionD.axisRotation);
        TRSMatricesHolder(tempR);
        tempR.rotateMatrix = motion;
        return tempR;
    }

    void drawCube(GLuint worldMatrixLocation, GLuint colorLocation, mat4 relativeWorldMatrix) {
        //code goes here
        mat4 mWorldMatrix;
        mat4 translationMatrix, scalingMatrix;
        mat4 worldMatrix;
    
        translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        scalingMatrix = mat4(1.0f);//glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 3.0f, 3.0f));
        worldMatrix = translationMatrix * scalingMatrix;
        mWorldMatrix = relativeWorldMatrix * worldMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    float targetAngle;
    RotateCycle rotateState;
private:
};

//only stores axis for rotation purposes.
//honestly, this should be replaced by CharModel since it has no attributes of its own. Only purpose now is to distinguish its role.
class ModelFace : public CharModel{
public:
    ModelFace(int shaderProgram, TRSMatricesHolder ini_relTRSMatrices = TRSMatricesHolder(), vec3 axis = vec3(1.0f, 0, 0))
        : CharModel(shaderProgram, ini_relTRSMatrices)/*, axis(axis)*/ {}

    //vec3 getAxis() {
    //    //return axis;
    //}
    //axis should only be modified when constructing, but this makes it easier to code with.
    void setAxis(vec3 axisR) {
        //axis = axisR;

        motionD.axisRotation = axisR;
    }
        //vec3 axis;
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

    ModelS3(int shaderProgram, TRSMatricesHolder init_relTRSMatrices = TRSMatricesHolder()) : CharModel(shaderProgram, init_relTRSMatrices){
		//initialize initial_relativeTranslateMatrix here and set relativeTranslateMatrix to that value.
        initY = -2.0f;

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
vector<vec3> sphereVertices(const int heightParts, const int ringParts) {

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

    vector<vec3> vecSphereArray (0);

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

                //position
                vecSphereArray.push_back(vec3(x, y, z));
                //normal
                vecSphereArray.push_back(vec3(x, y, z));
                //uv
                //bound to [0.0f, 1.0f] interval from [-1.0f, 1.0f].
                float norm_x = (x + 1) / (2 * radius);
                float norm_z = (z + 1) / (2 * radius);
                //ignore 3rd float
                vecSphereArray.push_back(vec3(norm_x, norm_z, 0.0f));
            }
        }
    }

    //return sphereArray;
    return vecSphereArray;
}
vector<vec3> halfSphereVertices(const int heightParts, const int ringParts) {

    //sphere radius, rho
    const float radius = 1.0f;
    vector<vec3> vecSphereArray(0);

    //for every height part
    for (int i = 0; i < heightParts - 1; i++) {
        //for each part of a ring
        for (int j = 0; j < ringParts; j++) {
            //alternate between heights to form a strip.
            //slightly redundant on bottommost and topmost vertices.
            for (int k = 0; k < 2; k++) {
                int alternate = 1 - k;
                //y is height
                // v v v diameter divided by area part count, then center to origin. so spacing is by diameter
                //float ya = (i + alternate) * radius / (heightParts - 1);
                // v v v height segment is given by phi angle.
                //float y = -radius * (sinf(radians(2 * 90 * ((float)(i + alternate) / (heightParts)))));
                float y = radius * (sinf(radians(2 * 90 * ((float)(i + alternate) / (heightParts - 1) - 0.5f))));
                y = -(y + 1) / 2;
                //vertices for a xz-ring at height y.
                //r is radial distance
                float r = sqrt(radius * radius - y * y);

                //ring
                float theta = radians(j * 360.0f / ringParts);
                float x = r * cosf(theta);
                float z = r * sinf(theta);
                //every height will have ringParts# * 2 of elements

                //position
                vecSphereArray.push_back(vec3(x, y, z));
                //normal
                vecSphereArray.push_back(vec3(x, y, z));
                //uv
                //bound to [0.0f, 1.0f] interval from [-1.0f, 1.0f].
                float norm_x = (x + 1) / (2 * radius);
                float norm_z = (z + 1) / (2 * radius);
                //ignore 3rd float
                vecSphereArray.push_back(vec3(norm_x, norm_z, 0.0f));
            }
        }
    }
    return vecSphereArray;
}
int createTexturedCubeVertexArrayObject()
{
    // Cube model (used for models and axis)

    vec3 vertexArray[] = {  
        // position and normal and face
        //cube (-0.5,-0.5,-0.5) to (0.5,0.5,0.5)
        //left
        vec3(-0.5f,-0.5f,-0.5f), vec3(-1.0f, 0.0f, 0.0f), vec3(5, 0.0f, 0.0f),
        vec3(-0.5f,-0.5f, 0.5f), vec3(-1.0f, 0.0f, 0.0f), vec3(5, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(-1.0f, 0.0f, 0.0f), vec3(5, 0.0f, 0.0f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(-1.0f, 0.0f, 0.0f), vec3(5, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(-1.0f, 0.0f, 0.0f), vec3(5, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(-1.0f, 0.0f, 0.0f), vec3(5, 0.0f, 0.0f),
        // far                                            vec3(-1.0f, 0.0f, 0.0f),
        vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, -1.0f),  vec3(1, 0.0f, 0.0f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, -1.0f), vec3(1, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, -1.0f), vec3(1, 0.0f, 0.0f),
        vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, -1.0f),  vec3(1, 0.0f, 0.0f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, -1.0f),  vec3(1, 0.0f, 0.0f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, -1.0f), vec3(1, 0.0f, 0.0f),
        // bottom                                         vec3(-1.0f, 0.0f, 0.0f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, -1.0f, 0.0f),  vec3(2, 0.0f, 0.0f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, -1.0f, 0.0f), vec3(2, 0.0f, 0.0f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, -1.0f, 0.0f),  vec3(2, 0.0f, 0.0f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, -1.0f, 0.0f),  vec3(2, 0.0f, 0.0f),
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, -1.0f, 0.0f), vec3(2, 0.0f, 0.0f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, -1.0f, 0.0f), vec3(2, 0.0f, 0.0f),
        // near                                           vec3(-1.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),  vec3(0, 0.0f, 0.0f),
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),  vec3(0, 0.0f, 0.0f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),   vec3(0, 0.0f, 0.0f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),   vec3(0, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),  vec3(0, 0.0f, 0.0f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 0.0f, 1.0f),   vec3(0, 0.0f, 0.0f),
        // right                                          vec3(-1.0f, 0.0f, 0.0f),
        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),   vec3(4, 0.0f, 0.0f),
        vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),   vec3(4, 0.0f, 0.0f),
        vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),   vec3(4, 0.0f, 0.0f),
        vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),   vec3(4, 0.0f, 0.0f),
        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),   vec3(4, 0.0f, 0.0f),
        vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),   vec3(4, 0.0f, 0.0f),
        // top                                            vec3(-1.0f, 0.0f, 0.0f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),   vec3(3, 0.0f, 0.0f),
        vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 1.0f, 0.0f),   vec3(3, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 1.0f, 0.0f),  vec3(3, 0.0f, 0.0f),
        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),   vec3(3, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 1.0f, 0.0f),  vec3(3, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),  vec3(3, 0.0f, 0.0f),

        //line (0,0,-0.5)to(0,0,0.5)
        vec3(0.0f, 0.0f, -0.5f), vec3(0.0f, 1.0f, 0.0f), vec3(0, 0.0f, 0.0f),
        vec3(0.0f, 0.0f, 0.5f), vec3(0.0f, 1.0f, 0.0f),  vec3(0, 0.0f, 0.0f),

        //Square (-0.5,0,-0.5) to (0.5,0,0.5)
        vec3(0.5f, 0.0f, 0.5f),vec3(0.0f, 1.0f, 0.0f),   vec3(0, 0.0f, 0.0f),
        vec3(0.5f, 0.0f,-0.5f), vec3(0.0f, 1.0f, 0.0f),  vec3(0, 0.0f, 0.0f),
        vec3(-0.5f, 0.0f,-0.5f),vec3(0.0f, 1.0f, 0.0f),  vec3(0, 0.0f, 0.0f),
        vec3(0.5f, 0.0f, 0.5f), vec3(0.0f, 1.0f, 0.0f),  vec3(0, 0.0f, 0.0f),
        vec3(-0.5f, 0.0f,-0.5f), vec3(0.0f, 1.0f, 0.0f), vec3(0, 0.0f, 0.0f),
        vec3(-0.5f, 0.0f, 0.5f),vec3(0.0f, 1.0f, 0.0f),  vec3(0, 0.0f, 0.0f)
    };
    //int faceArray[] = {
    //    //left
    //    5,5,5, 5,5,5,
    //    // far
    //    1,1,1, 1,1,1,
    //    // bottom
    //    2,2,2, 2,2,2,
    //    // near
    //    0,0,0, 0,0,0,
    //    // right
    //    4,4,4, 4,4,4,
    //    // top
    //    3,3,3, 3,3,3
    //};
    //create instancing array for tile grid. 
    const int sideLength = 100;
    const float cellLength = 1.0f;
    const float height = 0.0f;
    vec3 offsetArray[sideLength * sideLength + 27] = {};
    for (int i = 0; i < sideLength; ++i)
    {
        for (int j = 0; j < sideLength; ++j)
        {
            //skip the extra one (the last one)
            if (i == sideLength - 1 && j == sideLength - 1) {
                break;
            }
            offsetArray[sideLength * i + j + 1 + 27] =
                vec3(-cellLength * sideLength + (i + 1) * cellLength,
                    height,
                    -cellLength * sideLength + (j + 1) * cellLength);

            //offsetArray[sideLength * i + j + 1] = vec3(0, 0, 0);
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
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);
    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    //GLuint vertexBufferObject;
    //glGenBuffers(1, &vertexBufferObject);
    //glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        3 * sizeof(vec3),        // stride - each vertex contains vec3 (position)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(4,                   // attribute 4 matches aNormal in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        3 * sizeof(vec3),        // stride - each vertex contains vec3 (position)
        (void*)sizeof(vec3)  // array buffer offset
    );
    glEnableVertexAttribArray(4);

    //Setting up the face array. is in vec3 because method is known and had trouble with only int.
    glVertexAttribPointer(5,                   // attribute 5 matches face in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        3 * sizeof(vec3),        // stride - each vertex contains vec3 (position)
        (void*)(2* sizeof(vec3))  // array buffer offset
    );
    glEnableVertexAttribArray(5);


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

int createTexturedSphereVertexArrayObject()
{
    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    //Have multiple arrays/buffer.
    //https://www.khronos.org/opengl/wiki/Tutorial2:_VAOs,_VBOs,_Vertex_and_Fragment_Shaders_(C_/_SDL)
    GLuint vertexBufferObject[3];
    glGenBuffers(3, vertexBufferObject);
    //glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject[0]);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(texturedCubeVertexArray), texturedCubeVertexArray, GL_STATIC_DRAW);

    //);
    //glEnableVertexAttribArray(1);

    const int heightParts = 22;
    const int ringParts = 30;
    const int numStride = 3;
    const int numVertexes = (heightParts - 1) * ringParts * 2 * numStride;
    //make sure the sphere draw function draws [(heightParts-1)*ringParts*2] figures.
    //https://stackoverflow.com/questions/4264304/how-to-return-an-array-from-a-function
    vector<vec3> vertexArraySphere = sphereVertices(heightParts, ringParts);

    vec3 vertexArr2[numVertexes];
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
        3 * sizeof(vec3),        // stride - each vertex contains vec3 (position)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2,                            // attribute 2 matches aUV in Vertex Shader
        2,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(vec3),
        (void*)(2 * sizeof(vec3))      // uv is offseted by 2 vec3 (comes after position and normal)
    );
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(4,                   // attribute 1 matches aNormal in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        3 * sizeof(vec3),        // stride - each vertex contains vec3 (position)
        (void*)sizeof(vec3)  // array buffer offset
    );
    glEnableVertexAttribArray(4);


    vec3 offsetArray[1] = { vec3(0) };
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
int createTexturedHalfSphereVertexArrayObject()
{
    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    //Have multiple arrays/buffer.
    //https://www.khronos.org/opengl/wiki/Tutorial2:_VAOs,_VBOs,_Vertex_and_Fragment_Shaders_(C_/_SDL)
    GLuint vertexBufferObject[2];
    glGenBuffers(2, vertexBufferObject);

    const int heightParts = 22;
    const int ringParts = 30;
    const int numStride = 3;
    const int numVertexes = (heightParts - 1) * ringParts * 2 * numStride;
    //make sure the sphere draw function draws [numVertexes/numStride] figures.
    //https://stackoverflow.com/questions/4264304/how-to-return-an-array-from-a-function
    vector<vec3> vertexArraySphere = halfSphereVertices(heightParts, ringParts);

    vec3 vertexArr2[numVertexes];
    //convert it to array
    for (int i = 0; i < vertexArraySphere.size(); i++) {
        vertexArr2[i] = vertexArraySphere[i];
    }

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArr2), vertexArr2, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        3 * sizeof(vec3),        // stride - each vertex contains vec3 (position)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2,                            // attribute 2 matches aUV in Vertex Shader
        2,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(vec3),
        (void*)(2 * sizeof(vec3))      // uv is offseted by 2 vec3 (comes after position and normal)
    );
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(4,                   // attribute 1 matches aNormal in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        3 * sizeof(vec3),        // stride - each vertex contains vec3 (position)
        (void*)sizeof(vec3)  // array buffer offset
    );
    glEnableVertexAttribArray(4);


    vec3 offsetArray[1] = { vec3(0) };
    //https://learnopengl.com/Advanced-OpenGL/Instancing
    //Setting up the instance array
    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(offsetArray), offsetArray, GL_STATIC_DRAW);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(3, 1);

    return vertexArrayObject;
}
//glsl functions, thank you for making coding in strings slightly less trial and error.
//https://www.shaderific.com/glsl-functions
//main shaders for program.
const char* getVertexShaderSource()
{
    return
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;"           //vertex position
        //"layout (location = 1) in vec3 aColor;"         //vertex color    (unused)
        "layout (location = 2) in vec2 aUV;"            //vertex texture location
        "layout (location = 3) in vec3 instanceVec; "   // instancing https://learnopengl.com/Advanced-OpenGL/Instancing
        "layout (location = 4) in vec3 aNormal;"        //vertex normal
        "layout (location = 5) in vec3 face;"            //int face orientation
        ""
        "uniform vec3 objectColor[27];"     //object inherent color
        "uniform mat4 worldMatrix[27];"
        "uniform int mainFace[27];"
        "uniform int position[27];"
        "uniform bool isRubik;"
        "uniform mat4 viewMatrix = mat4(1.0);"  // default value for view matrix (identity)
        "uniform mat4 projectionMatrix = mat4(1.0);"
        "uniform mat4 lightSpaceMatrix;"                //shadow
        ""
        "out vec3 normalVec;"
        "out vec3 fragPos;"
        "out vec2 vertexUV;"
        "out vec4 fragPosLightSpace;"                   //shadow
        "out vec3 objectC;"     //object inherent color
        "flat out int face_s;"

        //rotate for vec2 (uv purpose).
        "vec2 rotate(float angle, vec2 uv) {"
        "    float s = sin(radians(angle));"
        "    float c = cos(radians(angle));"
        "    mat2 rotateM = mat2(c, -s, s, c);"
        "    return rotateM * uv;"
        "}"

        "void main()"
        "{"
        "   int voidFace = 7-1;"
        "   int instanceIndex = 0;"
        "   face_s = 0;"
        "   int facingDir = 0;"      
        "   if (isRubik){"
        "      instanceIndex = gl_InstanceID;"
        "      face_s = mainFace[instanceIndex];"
        "      facingDir = int(face.x);"  
        "   }"
        "   normalVec = mat3(transpose(inverse(worldMatrix[instanceIndex]))) * aNormal;"
        "   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix[instanceIndex];"
        //"   gl_Position = modelViewProjection * vec4(aPos + instanceVec, 1.0);"
        "   gl_Position = modelViewProjection * vec4(aPos + instanceVec, 1.0);"
        "   fragPos = vec3(worldMatrix[instanceIndex] * vec4(aPos + instanceVec, 1.0));"
        //fragPos from light's view
        "   fragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);"
        "   objectC = objectColor[instanceIndex];"
        "   int x = position[instanceIndex]%3;"
        "   int y = position[instanceIndex]/3;"
        "   vertexUV  = aUV;"

        "   int angle = 0;"
        "switch (facingDir) {"
        //if drawing inside, //void? also maybe add void to mainface[center]
        "case 0:"
        "   if (isRubik){"
        "       face_s = voidFace;"
        "   }"
        "   break;"
        //if drawing outside,
        "case 1:"
        "   vertexUV = vec2(aUV.x/3 + x/3.0f, aUV.y/3 + y/3.0f);"
        "	break;"
        //if drawing right side, then draw right side texture
        "case 4:"
        "   face_s = int(mod(face_s + 1 , 4));"
        "   if (x != 2){"
        "       face_s = voidFace;"
        "   }"
        "   vertexUV = vec2(aUV.x, aUV.y);"
        //translate
        "   vertexUV = vec2(vertexUV.x - 0.5f, vertexUV.y - 0.5f);"
        //rotate and flip
        "   vertexUV = vec2(vertexUV.y, vertexUV.x);"
        //translate
        "   vertexUV = vec2(vertexUV.x + 0.5f, vertexUV.y + 0.5f);"
        //shift
        "   x -=2;"
        "   vertexUV = vec2(vertexUV.x/3+ x/3.0f, vertexUV.y/3 + y/3.0f);"
        "	break;"
        //if drawing left side, then draw left side texture //void?
        "case 5:"
        "   face_s = int(mod(face_s - 1 , 4));"
        "   face_s = voidFace;"
        "	break;"
        "case 3:"
        //if drawing top, then choose top texture
        "   face_s = 4;"
        "   if (y != 2){"
        "       face_s = voidFace;"
        "   }"

        "   angle = mainFace[instanceIndex] * -90;"
        //shift
        "   y -=2;"
        "   vertexUV = vec2(vertexUV.x/3+ x/3.0f, vertexUV.y/3 + y/3.0f);"
        //translate
        "   vertexUV = vec2(vertexUV.x - 0.5f, vertexUV.y - 0.5f);"
        //rotate
        "   vertexUV = rotate(angle, vertexUV);"
        //translate
        "   vertexUV = vec2(vertexUV.x + 0.5f, vertexUV.y + 0.5f);"
        "	break;"

        //if drawing bot, then choose bot texture
        "case 2:"
        "   face_s = 5;"
        "   if (y != 0){"
        "       face_s = voidFace;"
        "   }"

        "   angle = mainFace[instanceIndex] * 90;"
        //flip and shift
        "   vertexUV = vec2(aUV.x, -aUV.y);"
        "   vertexUV = vec2(vertexUV.x/3+ x/3.0f, vertexUV.y/3 + y/3.0f);"
        //translate
        "   vertexUV = vec2(vertexUV.x - 0.5f, vertexUV.y - 0.5f);"
        //rotate
        "   vertexUV = rotate(angle, vertexUV);"
        //translate
        "   vertexUV = vec2(vertexUV.x + 0.5f, vertexUV.y + 0.5f);"
        "	break;"
        "}" //end switch

        //"   if (mainFace[instanceIndex] == 6){"
        //"       face_s = voidFace;"
        //"   }"
        "   if (isRubik){"
        "      face_s++;"
        "   }"
        "}";
}
const char* getFragmentShaderSource()
{
    return
    "#version 330 core\n"
    "uniform vec3 lightPos;"        //light properties
    "uniform vec3 viewPos;"
	"uniform sampler2D textureSampler[9];" //texture properties
	"uniform bool hasTexture;"
    "uniform sampler2D shadowMap;"  //shadow
    "uniform bool hasShadow;"

    "vec3 lightColor = vec3(1.0, 1.0, 1.0);"    //light property
    "vec4 textureColor;"                        //texture property
        
    "in vec3 objectC;"     //object inherent color
    "flat in int face_s;"     //texture to use
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
    //"float bias = 0.005;"
    // calculate bias (based on depth map resolution and slope)
    "       vec3 normal = normalize(normalVec);"
    "       vec3 lightDir = normalize(lightPos - fragPos);"
    "       float bias = max(0.015 * (1.0 - dot(normal, lightDir)), 0.005);"
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
		"      textureColor = texture( textureSampler[face_s], vertexUV );"
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
        "   vec4 result = vec4( (ambient + (1-shadow) * (diffuse + specular)) * objectC, 1.0f);"
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

//left-over code from online resource/tutorial. Kept for quick shadow debugging.
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
//left-over code to test shader for shadows
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
//left-over code to test shader for shadows
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
//left-over code to test shader for shadows
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
//shader for shadows
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
//boiler-plate code to like shaders.
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

//creates depth map for shadow.
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
//left-over code from online resource/tutorial. Kept for quick shadow debugging.
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

//left-over code from online resource/tutorial. Kept for quick shadow debugging.
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
}

//left-over code from online resource/tutorial. Kept for quick shadow debugging.
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

//boiler-plate to load textures from file name.
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
    const int numCells = (int)pow(sideLength,2) + 27;
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
void drawAxes(GLuint worldMatrixLocation, GLuint colorLocation) {
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

//checks if a shift key is being pressed.
bool isShiftPressed(GLFWwindow* window) {
    return glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
}

//checks what should the previous state be of tracked keys.
int returnPreviousKeyState(GLFWwindow* window, int key, map<int, KeyState> previousKeyStates) {
    //get previous key state if tracked, otherwise default release (true).
    //default released if not tracked
    int previousState = GLFW_RELEASE;

    //https://stackoverflow.com/questions/4527686/how-to-update-stdmap-after-using-the-find-method
    map<int, KeyState>::iterator it = previousKeyStates.find(key);
    //when key is a tracked one,
    if (it != previousKeyStates.end()) {        
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
    return previousState;
}
//checks if a key related to movement is being pressed.
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
        int previousState = returnPreviousKeyState(window, *itr, previousKeyStates);

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
    inputsToModelIndex.insert(pair<int, GLchar>(GLFW_KEY_6, 5));
    inputsToModelIndex.insert(pair<int, GLchar>(GLFW_KEY_7, 6));

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
        //TODO: delete later, since this isn't needed anymore.
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
        int previousState = returnPreviousKeyState(window, itr->first, previousKeyStates);

        if (glfwGetKey(window, itr->first) == GLFW_PRESS && previousState == GLFW_RELEASE) // select model. Apply once for keys that are tracked.
        {
            //group up the applied transformations.
            selectedTransformation[itr->second.type] = itr->second.matrix * selectedTransformation[itr->second.type];
        }
    }
    return selectedTransformation;
}
//function will return a partIndex increment on correct key presses.
int indexControl(GLFWwindow* window, int prevPartIndex, int mod, map<int, int> inputsToIndexIncrement, map<int, KeyState> previousKeyStates) {

    //map<int, int> inputsToIndexIncrement;
    map<int, int>::iterator itr;
    //inputsToIndexIncrement.insert(pair<int, GLchar>(GLFW_KEY_F, -1));
    //inputsToIndexIncrement.insert(pair<int, GLchar>(GLFW_KEY_G, 1));

    int indexIncrement = 0;
    //iterate through all keys that could be pressed.
    for (itr = inputsToIndexIncrement.begin(); itr != inputsToIndexIncrement.end(); ++itr) {
        int keyState = glfwGetKey(window, itr->first);
        int previousState = returnPreviousKeyState(window, itr->first, previousKeyStates);
        if (keyState == GLFW_PRESS && previousState == GLFW_RELEASE) // increment/decrement partIndex
        {
            indexIncrement += itr->second;
        }
    }

    //positive modulo 
    //https://stackoverflow.com/questions/14997165/fastest-way-to-get-a-positive-modulo-in-c-c
    //const float mod = numAttachedModelsPerMain;
    const int newPartIndex = fmodf(mod + fmodf(prevPartIndex + indexIncrement, mod), mod);
    return newPartIndex;
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
        {GLFW_KEY_X, true},
        //signal to toggle shadows
        {GLFW_KEY_B, true},
        //signal to go to next mode
        {GLFW_KEY_C},
        {GLFW_KEY_C, true},
        //signal to go to previous mode
        {GLFW_KEY_V},
        {GLFW_KEY_V, true},
        //signal to make random turn (Rubik)
        {GLFW_KEY_R},
        {GLFW_KEY_R, true},
        //signal to cycle through textures
        { GLFW_KEY_Z },
        { GLFW_KEY_Z, true },
        //signal toggle skybox  
        {GLFW_KEY_O, true},
        //signal toggle lights  //not implemented
        {GLFW_KEY_L, true},
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
        int previousState = returnPreviousKeyState(window, keyControl[i].key, previousKeyStates);

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

//individual fields are id of textures.
struct TextureId {
    int depthMap;
    int tiledTextureID;
    //int boxTextureID;
    int metalTextureID;
    //etc
    int skybox;
};
//individual fields are VAO
struct VAO {
    int cubeVAO;
    int sphereVAO; 
    int halfSphereVAO;
};
//has info on all settings to render.
struct RenderInfo {
    int shaderProgram;
    GLuint colorLocation;
    GLuint enableTextureLocation;
    GLuint enableShadowLocation;
    //GLuint enableRubikLocation;
    GLuint worldMatrixLocation;

    int enableTexture;
    int enableShadow;
    bool toggleSkybox;
    //int enableRubik;
    TextureId textures;

    VAO vao;
    //etc

    Color backgroundColor;
};

void drawBox(GLuint worldMatrixLocation, mat4 relativeWorldMatrix) {
    // Draw box
    //omit top, then scale top by -0.5f.
    const float yHover = 0.0f;

    const float scaler = 75;
    const float xOffset = 0;
    const float yOffset = scaler / 4;

    mat4 worldMatrix = translate(mat4(1.0f), vec3(xOffset, yOffset, 0.0f)) * scale(mat4(1.0f), glm::vec3(-scaler, -0.5f * scaler, -scaler));
    mat4 mWorldMatrix = relativeWorldMatrix * worldMatrix;
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, 30);
}
void halfSphereSkybox(GLuint worldMatrixLocation, mat4 relativeWorldMatrix) {
    // Draw half sphere
    const float yHover = 0.0f;

    const float xOffset = 0;
    const float yOffset = 0;
    const float scaler = 75;

    mat4 worldMatrix = translate(mat4(1.0f), vec3(xOffset, yOffset, 0.0f)) * scale(mat4(1.0f), glm::vec3(-scaler, -scaler, -scaler));
    mat4 mWorldMatrix = relativeWorldMatrix * worldMatrix;
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);


    //match these numbers to those passed to sphereVertices().
    const int heightParts = 22;
    const int ringParts = 30;
    const int numVertices = (heightParts - 1) * ringParts * 2;
    glDrawArrays(GL_TRIANGLE_STRIP, 0, numVertices);
}
void renderDecor(RenderInfo renderInfo) {
    int shaderProgram = renderInfo.shaderProgram;
    GLuint colorLocation = renderInfo.colorLocation;
    GLuint enableTextureLocation = renderInfo.enableTextureLocation;
    GLuint enableShadowLocation = renderInfo.enableShadowLocation;
    //GLuint enableRubikLocation = renderInfo.enableRubikLocation;
    GLuint worldMatrixLocation = renderInfo.worldMatrixLocation;
    int enableTexture = renderInfo.enableTexture;
    int enableShadow = renderInfo.enableShadow;
    //int enableRubik = renderInfo.enableRubik;
    int tiledTextureID = renderInfo.textures.tiledTextureID;
    int skyboxTexture = renderInfo.textures.skybox;
    bool toggleSkybox = renderInfo.toggleSkybox;

    int cubeVAOa = renderInfo.vao.cubeVAO;
    int halfSphereVAOa = renderInfo.vao.halfSphereVAO;

    Color backgroundColor = renderInfo.backgroundColor;

    glBindVertexArray(cubeVAOa);
    glActiveTexture(GL_TEXTURE1);
    // Draw ground
    glUniform1i(enableShadowLocation, enableShadow);
    //glUniform1i(enableRubikLocation, enableRubik);
    GLuint enableRubikLocation = glGetUniformLocation(shaderProgram, "isRubik");
    glUniform1i(enableRubikLocation, 0);
    //draw grid
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform3f(colorLocation, 1.0f, 1.0f, 1.0f);
    glUniform1i(enableTextureLocation, 0);
    drawGrid(worldMatrixLocation, colorLocation, mat4(1.0f));
    // draw axis
    drawAxes(worldMatrixLocation, colorLocation);
    glUniform1i(enableTextureLocation, enableTexture);

    //draw tiles
    glBindTexture(GL_TEXTURE_2D, tiledTextureID);
    glUniform3f(colorLocation, 0.8f, 0.4f, 0.8f);
    drawTileGrid(worldMatrixLocation, mat4(1.0f));
    //skybox
    Color skyColor = Color(0.5f, 0.75f, 1.0f);
    skyColor.red += backgroundColor.red;
    skyColor.green += backgroundColor.green;
    skyColor.blue += backgroundColor.blue;
    glUniform3f(colorLocation, skyColor.red, skyColor.green, skyColor.blue);
    glBindTexture(GL_TEXTURE_2D, skyboxTexture);
    if (toggleSkybox) {
        glBindVertexArray(halfSphereVAOa);
        halfSphereSkybox(worldMatrixLocation, mat4(1.0f));
    }
    else {
        glBindVertexArray(cubeVAOa);
        drawBox(worldMatrixLocation, mat4(1.0f));
    }
}
void renderModels(RenderInfo renderInfo, vector<CharModel*> models, vector<CharModel*> attachedToCore, bool shadow = false) {
    int shaderProgram = renderInfo.shaderProgram;
    GLuint colorLocation = renderInfo.colorLocation;
    GLuint enableTextureLocation = renderInfo.enableTextureLocation;
    GLuint worldMatrixLocation = renderInfo.worldMatrixLocation;
    int enableTexture = renderInfo.enableTexture;

    //int boxTextureID = renderInfo.textures.boxTextureID;
    int metalTextureID = renderInfo.textures.metalTextureID;

    int cubeVAOa = renderInfo.vao.cubeVAO;
    int sphereVAOa = renderInfo.vao.sphereVAO;

    glBindVertexArray(cubeVAOa);
    glUniform1i(enableTextureLocation, enableTexture);
    //glUniform1i(enableShadowLocation, enableShadow);
    //glUniform1i(enableRubikLocation, enableRubik);
    GLuint temp = CharModel::swapWorldMatrixLocation(models, worldMatrixLocation);

    //draw all models

    //boxes
    GLuint enableRubikLocation = glGetUniformLocation(shaderProgram, "isRubik");
    glUniform1i(enableRubikLocation, 1);
    vector<mat4> trsMatrices = CharModel::getTRS(attachedToCore);
    vector<int> mainFace = CharModel::getFace(attachedToCore);
    vector<int> posIndex = CharModel::getPosIndex(attachedToCore);
    vector<vec3> colors = CharModel::getColor(attachedToCore);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "worldMatrix"), 27, false, &trsMatrices[0][0][0]);
    glUniform1iv(glGetUniformLocation(shaderProgram, "mainFace"), 27, &mainFace[0]);
    glUniform1iv(glGetUniformLocation(shaderProgram, "position"), 27, &posIndex[0]);
    glUniform3fv(colorLocation, 27, &colors[0][0]);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 27);
    glUniform1i(enableRubikLocation, 0);

    //remove textures from everything else
    glUniform1i(enableTextureLocation, 0);

    //glUniform3f(colorLocation, 1.0f, 233.0f / 255.0f, 0.0f);
    glUniform3f(colorLocation, 0.2f, 0.2f, 1.0f);
    //glBindTexture(GL_TEXTURE_2D, boxTextureID);
    CharModel::drawLetter(models);
    //glBindTexture(GL_TEXTURE_2D, metalTextureID);
    CharModel::drawNumber(models);
    //Sphere has no texture for now.
    //glBindTexture(GL_TEXTURE_2D, 0);
    //glUniform1i(enableTextureLocation, 0);
    if(shadow)CharModel::draw(models);
    glBindVertexArray(sphereVAOa);
    CharModel::drawSphere(models);
    glUniform1i(enableTextureLocation, enableTexture);

    //swap back
    CharModel::swapWorldMatrixLocation(models, temp);
}

//index of box at cooridnates, 3x3x3 only
int getCoordB(int x, int y, int z) {
    //return 1 * x + 3 * y + 9 * z;
    int arr[3][3][3] = {
        { {19,  0,  1}, {21,  2,  3}, {23,  4 , 5} },
        { {18, 25,  6}, {20, 26,  8}, {22, 24 ,10} },
        { {13, 12,  7}, {15, 14,  9}, {17, 16 ,11} }
    };
    int a= arr[z][y][x];
    return a;
}
//index of box at cooridnates, 3x3x3 only
int getCoord(int x, int y, int z) {
    return 1 * x + 3 * y + 9 * z;
    //int arr[3][3][3] = {
    //    { {19,  0,  1}, {21,  2,  3}, {23,  4 , 5} },
    //    { {18, 25,  6}, {20, 26,  8}, {22, 24 ,10} },
    //    { {13, 12,  7}, {15, 14,  9}, {17, 16 ,11} }
    //};
    //int a = arr[z][y][x];
    //return a;
}
bool DEBUG_RUBIKCUBE_FRONT = false;
bool DEBUG_RUBIKCUBE_TOP= false;
class RubikCube {
public:
    RubikCube(vector<CharModel*> boxes) : boxes(boxes) {
        reset();
    }
    //reset positions
    void reset() {
        //for (int i = 0; i < 3; i++) {
        //    for (int j = 0; j < 3; j++) {
        //        for (int k = 0; k < 3; k++) {
        //            positionsIndex[1 * k + 3 * j + 9 * i] = getCoord(k,j,i);
        //        }
        //    }
        //}
        for (int i = 0; i < 27; i++) {
            positionsIndex[i] = i;
        }
        //for (int i = 0; i < 3; i++) {
        //    for (int j = 0; j < 3; j++) {
        //        for (int k = 0; k < 3; k++) {
        //            positionsIndex[getCoord(k, j, i)] = 1 * k + 3 * j + 9 * i;
        //        }
        //    }
        //}
    }

    //returns boxes located on corresponding face.
    vector<CharModel*> getFront() {
        vector<CharModel*> result(0);
        if (DEBUG_RUBIKCUBE_FRONT)cout << "Front:\n";
        int debugi = 0;
        for (int i = 0; i < 27; i++) {
            //or if ((i/9)%3==2)
            if (i > 17) {
                result.push_back(boxes[positionsIndex[i]]);

                if (DEBUG_RUBIKCUBE_FRONT) {
                    cout << positionsIndex[i];
                    cout << " ";
                    debugi++;
                    if (debugi % 3 == 0) cout << "\n";
                }
            }
        }
        return result;
    }
    vector<CharModel*> getBack() {
        vector<CharModel*> result(0);
        for (int i = 0; i < 27; i++) {
            if (i < 9) {
                result.push_back(boxes[positionsIndex[i]]);
            }
        }
        return result;
    }
    vector<CharModel*> getTop() {
        vector<CharModel*> result(0);
        if (DEBUG_RUBIKCUBE_TOP)cout << "Top:\n";
        int debugi = 0;
        for (int i = 0; i < 27; i++) {
            if ((i % 9) / 3 == 2) {
                result.push_back(boxes[positionsIndex[i]]);
                if (DEBUG_RUBIKCUBE_TOP) {
                    cout << positionsIndex[i];
                    cout << " ";
                    debugi++;
                    if (debugi % 3 == 0) cout << "\n";
                }
            }
        }
        return result;
    }
    vector<CharModel*> getBot() {
        vector<CharModel*> result(0);
        for (int i = 0; i < 27; i++) {
            if ((i % 9) / 3 == 0) {
                result.push_back(boxes[positionsIndex[i]]);
            }
        }
        return result;
    }
    vector<CharModel*> getPort() {
        vector<CharModel*> result(0);
        for (int i = 0; i < 27; i++) {
            if ((i % 9) % 3 == 2) {
                result.push_back(boxes[positionsIndex[i]]);
            }
        }
        return result;
    }
    vector<CharModel*> getStarboard() {
        vector<CharModel*> result(0);
        for (int i = 0; i < 27; i++) {
            if ((i % 9) % 3 == 0) {
                result.push_back(boxes[positionsIndex[i]]);
            }
        }
        return result;
    }

    //returns a rotated 2d-array.
    vector<vector<int>> rotateFaceCW(vector<vector<int>> arr) {
        //vector<vector<int>> result;
        //3x3 vector array
        vector<int> row(3, -1);
        vector<vector<int> > result(3, row);

        const int size = 3;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                result[i][j] = arr[size - 1 - j][i];
            }
        }
        return result;
    }
    vector<vector<int>> rotateFaceCCW(vector<vector<int>> arr) {
        //vector<vector<int>> result;
        //3x3 vector array
        vector<int> row(3, -1);
        vector<vector<int> > result(3, row);

        const int size = 3;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                result[i][j] = arr[j][size - 1 - i];
            }
        }
        return result;
    }

    //some redundancy in code.

    //rotate along face.
    //direction negative makes rotation CCW, otherwise rotation is CW.
    void rotateFront(float direction) {
        //3x3 vector array
        vector<int> row(3, -1);
        vector<vector<int> > arr(3, row);
        const int size = 3;
        //get indexes of face
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                arr[j][i] = positionsIndex[getCoord(i, j, 2)];
            }
        }
        //rotate face
        vector<vector<int> > result;
        if (signbit(direction)) {
            result = rotateFaceCCW(arr);
        }
        else {
            result = rotateFaceCW(arr);
        }

        //update indexes
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                positionsIndex[getCoord(i, j, 2)] = result[j][i];
            }
        }
    }
    //direction negative makes rotation CCW, otherwise rotation is CW.
    void rotateBack(float direction) {
        //3x3 vector array
        vector<int> row(3, -1);
        vector<vector<int> > arr(3, row);
        const int size = 3;
        //get indexes of face
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                arr[j][i] = positionsIndex[getCoord(i, j, 0)];
            }
        }
        //rotate face
        vector<vector<int> > result;
        if (signbit(direction)) {
            result = rotateFaceCCW(arr);

        }
        else {
            result = rotateFaceCW(arr);
        }

        //update indexes
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                positionsIndex[getCoord(i, j, 0)] = result[j][i];
            }
        }
    }
    //direction negative makes rotation CCW, otherwise rotation is CW.
    void rotateTop(float direction) {
        //3x3 vector array
        vector<int> row(3, -1);
        vector<vector<int> > arr(3, row);
        const int size = 3;
        //get indexes of face
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                arr[j][i] = positionsIndex[getCoord(i, 2, j)];
                // maybe instead?: arr[j][i] = positionsIndex[getCoord(size - 1 - i, 2, j)];
            }
        }
        //rotate face
        vector<vector<int> > result;
        if (signbit(direction)) {
            result = rotateFaceCCW(arr);

        }
        else {
            result = rotateFaceCW(arr);
        }

        //update indexes
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                positionsIndex[getCoord(i, 2, j)] = result[j][i];
            }
        }
    }
    //direction negative makes rotation CCW, otherwise rotation is CW.
    void rotateBot(float direction) {
        //3x3 vector array
        vector<int> row(3, -1);
        vector<vector<int> > arr(3, row);
        const int size = 3;
        //get indexes of face
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                arr[j][i] = positionsIndex[getCoord(i, 0, j)];
                // maybe instead?: arr[j][i] = positionsIndex[getCoord(size - 1 - i, 0, j)];
            }
        }
        //rotate face
        vector<vector<int> > result;
        if (signbit(direction)) {
            result = rotateFaceCCW(arr);

        }
        else {
            result = rotateFaceCW(arr);
        }

        //update indexes
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                positionsIndex[getCoord(i, 0, j)] = result[j][i];
            }
        }
    }
    //direction negative makes rotation CCW, otherwise rotation is CW.
    void rotatePort(float direction) {
        //3x3 vector array
        vector<int> row(3, -1);
        vector<vector<int> > arr(3, row);
        const int size = 3;
        //get indexes of face
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                arr[j][i] = positionsIndex[getCoord(2, j, i)];
            }
        }
        //rotate face
        vector<vector<int> > result;
        if (signbit(direction)) {
            result = rotateFaceCCW(arr);

        }
        else {
            result = rotateFaceCW(arr);
        }

        //update indexes
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                positionsIndex[getCoord(2, j, i)] = result[j][i];
            }
        }
    }
    //direction negative makes rotation CCW, otherwise rotation is CW.
    void rotateStarboard(float direction) {
        //3x3 vector array
        vector<int> row(3, -1);
        vector<vector<int> > arr(3, row);
        const int size = 3;
        //get indexes of face
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                arr[j][i] = positionsIndex[getCoord(0, j, i)];
            }
        }
        //rotate face
        vector<vector<int> > result;
        if (signbit(direction)) {
            result = rotateFaceCCW(arr);

        }
        else {
            result = rotateFaceCW(arr);
        }

        //update indexes
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                positionsIndex[getCoord(0, j, i)] = result[j][i];
            }
        }
    }

    //outside can just call a single method, side is determined with argument.
    void rotateFace(float direction, int face) {
        switch (face) {
        case 0:
            //Front
        {
            rotateFront(direction);
        }
        break;
        case 1:
            //Back
        {
            rotateBack(direction);
        }
        break;
        case 2:
            //Top
        {
            rotateTop(direction);
        }
        break;
        case 3:
            //Bot
        {
            rotateBot(direction);
        }
        break;
        case 4:
            //Port
        {
            rotatePort(direction);
        }
        break;
        case 5:
            //Starboard
        {
            rotateStarboard(direction);
        }
        break;

        }
    }

    bool checkSolved() {
        bool solved = true;
        for (int i = 0; i < 27; i++) {
            if (positionsIndex[i] != i) {
                solved = false;
                break;
            }
        }
        return solved;
    }
private:
    //boxes indexed in order
    vector<CharModel*> boxes;
    //index points to box that should be in cell position, according to specific order.
    int positionsIndex[27];

    //box indexes order (if it'd start from 1)
    //Back to Front
    //  7  8  9
    //  4  5  6
    //  1  2  3

    // 16 17 18
    // 13 14 15
    // 10 11 12

    // 25 26 27
    // 22 23 24
    // 19 20 21
};

//attaches rubik internal model to actual cube.
//attachedToRotater has sides of cube (front, back, top, bot, port, starboard).
void attachBoxToCube(vector<CharModel*> attachedToRotater, RubikCube rCube) {
    //front
    attachedToRotater[0]->setAttachedModels(rCube.getFront());
    //back
    attachedToRotater[1]->setAttachedModels(rCube.getBack());
    //top
    attachedToRotater[2]->setAttachedModels(rCube.getTop());
    //bot
    attachedToRotater[3]->setAttachedModels(rCube.getBot());
    //port
    attachedToRotater[4]->setAttachedModels(rCube.getPort());
    //starboard
    attachedToRotater[5]->setAttachedModels(rCube.getStarboard());
}
//Make a random turn on RubikCube.
int randomTurn(vector<CharModel*> attachedToRotater, RubikCube* rCube) {
    const int randNum = rand();
    //random part index
    const int randPartIndex = randNum % attachedToRotater.size();
    //random direction
    const int randDirection = (randNum/10) % 2;
    //Prepare randomly selected part to rotate.
    CharModel* randPart = attachedToRotater[randPartIndex];
    randPart->setAttachedMotionData(randPart->getMotionData());
    //rotate internal model, and apply to cube.
    if (randDirection) {
        randPart->attachedNext();
        rCube->rotateFace(1, randPartIndex);
    }
    else {
        randPart->attachedPrev();
        rCube->rotateFace(-1, randPartIndex);
    }
    attachBoxToCube(attachedToRotater, *rCube);
    return randPartIndex;
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
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "COMP 371 - A3 - Team 4", NULL, NULL);
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

#if defined(PLATFORM_OSX)

#else
#endif
    // Load Textures
#if defined(PLATFORM_OSX)
    //GLuint tiledTextureID = loadTexture("Textures/brick.jpg");
    //GLuint cementTextureID = loadTexture("Textures/cement.jpg");
    //GLuint metalTextureID = loadTexture("Textures/metal.jpg");
    //GLuint boxTextureID = loadTexture("Textures/box.jpg");
    GLuint jaguarTextureID = loadTexture("Textures/jaguar.jpg");
    GLuint woodTextureID = loadTexture("Textures/wood.jpg");

    //Theme 1: Colors
    GLuint redTextureID = loadTexture("Textures/red.jpg");
    GLuint blueTextureID = loadTexture("Textures/blue.jpg");
    GLuint yellowTextureID = loadTexture("Textures/yellow.jpg");
    GLuint greenTextureID = loadTexture("Textures/green.jpg");
    GLuint orangeTextureID = loadTexture("Textures/orange.jpg");
    GLuint purpleTextureID = loadTexture("Textures/purple.jpg");

    //Theme 2: Sports
    GLuint baseballTextureID = loadTexture("Textures/baseball.jpg");
    GLuint basketballTextureID = loadTexture("Textures/basketball.jpg");
    GLuint hockeyTextureID = loadTexture("Textures/hockey.jpg");
    GLuint soccerTextureID = loadTexture("Textures/soccer.jpg");
    GLuint tennisTextureID = loadTexture("Textures/tennis.jpg");
    GLuint bowlingballTextureID = loadTexture("Textures/bowlingball.jpg");

    //Theme 3: Music
    GLuint drumTextureID = loadTexture("Textures/drum.jpg");
    GLuint fluteTextureID = loadTexture("Textures/flute.jpg");
    GLuint guitaurTextureID = loadTexture("Textures/guitaur.jpg");
    GLuint pianoTextureID = loadTexture("Textures/piano.jpg");
    GLuint triangleTextureID = loadTexture("Textures/triangle.jpg");
    GLuint trumpetTextureID = loadTexture("Textures/trumpet.jpg");

    //Theme 4: Logos
    GLuint Logo1TextureID = loadTexture("Textures/ProgrammingLogo1.jpg");
    GLuint Logo2TextureID = loadTexture("Textures/ProgrammingLogo2.jpg");
    GLuint Logo3TextureID = loadTexture("Textures/ProgrammingLogo3.jpg");
    GLuint Logo4TextureID = loadTexture("Textures/ProgrammingLogo4.jpg");
    GLuint Logo5TextureID = loadTexture("Textures/ProgrammingLogo5.jpg");
    GLuint Logo6TextureID = loadTexture("Textures/ProgrammingLogo6.jpg");



#else
    //GLuint tiledTextureID = loadTexture("../Assets/Textures/brick.jpg");
    //GLuint cementTextureID = loadTexture("../Assets/Textures/cement.jpg");
    GLuint metalTextureID = loadTexture("../Assets/Textures/metal.jpg");
    //GLuint boxTextureID = loadTexture("../Assets/Textures/box.jpg");
    //GLuint brickTextureID = loadTexture("Assets/Textures/brick.jpg");
    //GLuint tiledTextureID = loadTexture("Assets/Textures/tiled.jpg");
    //GLuint cementTextureID = loadTexture("Assets/Textures/cement.jpg");
    //GLuint metalTextureID = loadTexture("Assets/Textures/metal.jpg");
    //GLuint boxTextureID = loadTexture("Assets/Textures/box.jpg");

    GLuint tiledTextureID = loadTexture("../Assets/Textures/tiled.jpg");


    GLuint a = loadTexture("../Assets/Textures/ProgrammingLogo1.jpg");
    GLuint b = loadTexture("../Assets/Textures/ProgrammingLogo2.jpg");
    GLuint c = loadTexture("../Assets/Textures/ProgrammingLogo3.jpg");
    GLuint d = loadTexture("../Assets/Textures/ProgrammingLogo4.jpg");
    GLuint e = loadTexture("../Assets/Textures/ProgrammingLogo5.jpg");
    GLuint f = loadTexture("../Assets/Textures/ProgrammingLogo6.jpg");

    GLuint woodTextureID = loadTexture("../Assets/Textures/wood.jpg");
    //Theme 1 : Color
    GLuint redTextureID     = loadTexture("../Assets/Textures/red.jpg");
    GLuint blueTextureID    = loadTexture("../Assets/Textures/blue.jpg");
    GLuint yellowTextureID  = loadTexture("../Assets/Textures/yellow.jpg");
    GLuint greenTextureID   = loadTexture("../Assets/Textures/green.jpg");
    GLuint orangeTextureID  = loadTexture("../Assets/Textures/orange.jpg");
    GLuint purpleTextureID  = loadTexture("../Assets/Textures/purple.jpg");

    //Theme 2: Sports
    GLuint baseballTextureID    = loadTexture("../Assets/Textures/baseball.jpg");
    GLuint basketballTextureID  = loadTexture("../Assets/Textures/basketball.jpg");
    GLuint hockeyTextureID      = loadTexture("../Assets/Textures/hockey.jpg");
    GLuint soccerTextureID      = loadTexture("../Assets/Textures/soccer.jpg");
    GLuint tennisTextureID      = loadTexture("../Assets/Textures/tennis.jpg");
    GLuint bowlingballTextureID = loadTexture("../Assets/Textures/bowlingball.jpg");

    //Theme 3: Music
    GLuint drumTextureID        = loadTexture("../Assets/Textures/drum.jpg");
    GLuint fluteTextureID       = loadTexture("../Assets/Textures/flute.jpg");
    GLuint guitaurTextureID     = loadTexture("../Assets/Textures/guitaur.jpg");
    GLuint pianoTextureID       = loadTexture("../Assets/Textures/piano.jpg");
    GLuint triangleTextureID    = loadTexture("../Assets/Textures/triangle.jpg");
    GLuint trumpetTextureID     = loadTexture("../Assets/Textures/trumpet.jpg");

    //Theme 4: Logos
    GLuint Logo1TextureID = loadTexture("../Assets/Textures/ProgrammingLogo1.jpg");
    GLuint Logo2TextureID = loadTexture("../Assets/Textures/ProgrammingLogo2.jpg");
    GLuint Logo3TextureID = loadTexture("../Assets/Textures/ProgrammingLogo3.jpg");
    GLuint Logo4TextureID = loadTexture("../Assets/Textures/ProgrammingLogo4.jpg");
    GLuint Logo5TextureID = loadTexture("../Assets/Textures/ProgrammingLogo5.jpg");
    GLuint Logo6TextureID = loadTexture("../Assets/Textures/ProgrammingLogo6.jpg");

    //skybox
    //courtesy of Kevin Rao with paint.
    GLuint t5ID = loadTexture("../Assets/Textures/T5.png");
#endif
    GLuint rubikFrontTexture     = redTextureID   ;
    GLuint rubikBackTexture      = blueTextureID  ;
    GLuint rubikTopTexture       = yellowTextureID;
    GLuint rubikBotTexture       = greenTextureID ;
    GLuint rubikPortTexture      = orangeTextureID;
    GLuint rubikStarboardTexture = purpleTextureID;
    GLuint rubikInnerTexture     = metalTextureID ;
    //randomize rand();
    srand(time(0));
    // Gull Grey background
    //glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
    Color backgroundColor = Color(164 / 255.0f, 173 / 255.0f, 176 / 255.0f);
    glClearColor(backgroundColor.red, backgroundColor.green, backgroundColor.blue, 1.0f);

    // Compile and link shaders here ...
    int shaderProgram = compileAndLinkShaders();
    int shaderProgramTest = compileAndLinkShadersTest();
    int shaderProgramTest2 = compileAndLinkShadersTest2();
    int shaderProgramShadow = compileAndLinkShadersShadow();

    // We can set the shader configurations
    //https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "shadowmap"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler[0]"), 1);
    glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler[1]"), 2);
    glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler[2]"), 3);
    glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler[3]"), 4);
    glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler[4]"), 5);
    glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler[5]"), 6);
    glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler[6]"), 7);
    glUniform1i(glGetUniformLocation(shaderProgram, "textureSampler[7]"), 8);

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

    //toggle skybox
    bool toggleSkybox = true;

    // Camera parameters for view transform
    const float initial_xpos = -10;
    const float initial_ypos = 20;
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
        0.01f, 200.0f);   // near and far (near > 0)

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
    int sphereVAOa = createTexturedSphereVertexArrayObject();
    int halfSphereVAOa = createTexturedHalfSphereVertexArrayObject();
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
    //select part
    previousKeyStates.insert(pair<int, KeyState>(GLFW_KEY_F, { GLFW_RELEASE , false }));
    previousKeyStates.insert(pair<int, KeyState>(GLFW_KEY_G, { GLFW_RELEASE , false }));
    //select part mode
    previousKeyStates.insert(pair<int, KeyState>(GLFW_KEY_C, { GLFW_RELEASE , false }));
    previousKeyStates.insert(pair<int, KeyState>(GLFW_KEY_V, { GLFW_RELEASE , false }));
    //make random turn (Rubik)
    previousKeyStates.insert(pair<int, KeyState>(GLFW_KEY_R, { GLFW_RELEASE , false }));
    //cycle through textures
    previousKeyStates.insert(pair<int, KeyState>(GLFW_KEY_Z, { GLFW_RELEASE , false }));
    //toggle lights //not implemented
    previousKeyStates.insert(pair<int, KeyState>(GLFW_KEY_L, { GLFW_RELEASE , true }));
    //toggle skybox //not implemented
    previousKeyStates.insert(pair<int, KeyState>(GLFW_KEY_O, { GLFW_RELEASE , true }));

    double lastMousePosX, lastMousePosY;
    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

    // Other OpenGL states to set once
    // Enable Backface culling
    glEnable(GL_CULL_FACE);

    // @TODO 1 - Enable Depth Test
    // ...
    glEnable(GL_DEPTH_TEST);


    //configure initialized positions for boxes
    vector<TRSMatricesHolder> init_T(0);
    vector<int> faces(0);
    vector<int> posIndex(0);
    //vector<mat4> init_T(0);
    //vector<mat4>::iterator init_T_itr;
    const float boxSideLength = 3.0f;
    const float boxSpacing = boxSideLength * 0.10f;
    const int boxPerSide = 3;
    const float cubeLength = boxPerSide / 2 * (boxSideLength + boxSpacing);
    const float cubeCenterHeight = boxPerSide / 2 * boxSideLength + boxSideLength*1.5;
    //// indexes 0 to 2 is back bottom row
    //// indexes 0 to 8 is back wall
    //for (int i = 0; i < boxPerSide; i++) {
    //    for (int j = 0; j < boxPerSide; j++) {
    //        for (int k = 0; k < boxPerSide; k++) {
    //            mat4 tempInit_T = translate(mat4(1.0f),
    //                vec3(-(cubeLength * (boxPerSide - 1) / 2) + k * cubeLength,
    //                    -(cubeLength * (boxPerSide - 1) / 2) + j * cubeLength,
    //                    -(cubeLength * (boxPerSide - 1) / 2) + i * cubeLength));
    //            init_T.push_back(tempInit_T);
    //        }
    //    }
    //}
    mat4 initScale = scale(glm::mat4(1.0f), glm::vec3(boxSideLength));
    //indexes 0 to 5 are 6 left-most cells of tall 3x3 in the back.
    //construct initial positions for sides
    //i from [0,3] are sides, [4,5] are top/bot, [6] center
    for (int i = 0; i < 7; i++) {
        //mat4 rotateM;
        switch (i) {
        case 0: case 1: case 2: case 3:
        {
            for (int j = 0; j < boxPerSide; j++) {
                for (int k = 1; k < boxPerSide; k++) {
                    mat4 tempInit_T = 
                        rotate(mat4(1.0f), radians(90.0f * i), vec3(0.0f, -1.0f, 0.0f)) *
                        translate(mat4(1.0f),
                            vec3(-(cubeLength * (boxPerSide - 1) / 2) + k * cubeLength,
                                -(cubeLength * (boxPerSide - 1) / 2) + j * cubeLength,
                                -(cubeLength * (boxPerSide - 1) / 2)));
                    TRSMatricesHolder tempinit_TRS = TRSMatricesHolder(mat4(1.0f), tempInit_T, initScale);
                    init_T.push_back(tempinit_TRS);
                    //init_T.push_back(tempInit_T);
                    faces.push_back(i);
                    posIndex.push_back(j * 3 + k);
                }
            }
        }
        break;
        case 4: case 5:
        {
            mat4 tempInit_T =
                 rotate(mat4(1.0f), radians(90.0f + 180 * i), vec3(1.0f, 0.0f, 0.0f)) *
                 translate(mat4(1.0f),
                    vec3(0,
                        0,
                        -(cubeLength * (boxPerSide - 1) / 2)));
            TRSMatricesHolder tempinit_TRS = TRSMatricesHolder(mat4(1.0f), tempInit_T, initScale);
            init_T.push_back(tempinit_TRS);
            //init_T.push_back(tempInit_T);
            faces.push_back(i);
            posIndex.push_back(1 * 3 + 1);
        }
        break;
        case 6:
        {
            mat4 tempInit_T =
                translate(mat4(1.0f),
                    vec3(0,
                        0,
                        0));
            TRSMatricesHolder tempinit_TRS = TRSMatricesHolder(mat4(1.0f), tempInit_T, initScale);
            init_T.push_back(tempinit_TRS);
            //init_T.push_back(tempInit_T);
            faces.push_back(i);
            posIndex.push_back(1 * 3 + 1);
        }
        break;
        }
    }
    //Models
    //CharModel* selectedModel;
    //CharModel* models[numMainModels];
    vector<CharModel*> vModels;
    // v v work-around to get the shadow map of the parts.
    vector<CharModel*> modelsAndParts;
    int modelIndex = 0;
    int partIndex = 0;
    int textureThemeIndex = 1;


    //Prepare initial positions of digits.
    TRSMatricesHolder pos_m[numDigits];
    //get some units to position models in + measurements for extra border around.
    const float z_position = 0;
    const float width_base_model = 2;
    const float spacing = (0.24f + 0.2f) * width_base_model;
    //border measurement
    const float total_width = width_base_model * numDigits + spacing * (numDigits);
    const float total_height = 2 * width_base_model + spacing;
    //border's constant
    const float border_thickness = width_base_model / 5;
    //skate constant
    const float skate_height = border_thickness;
    for (int i = 0; i < numDigits; i++) {
        pos_m[i].translateMatrix = translate(mat4(1.0f),
            vec3(-total_width / 2 + (i + 0.5f) * (width_base_model + spacing),
                border_thickness + spacing / 2 + skate_height,
                0));
    }

    TRSMatricesHolder(initBase);
    initBase.translateMatrix = translate(mat4(1.0f), vec3(0.0f,0.0f,6.0f));
    
    

    //base
    Model_Border base(shaderProgram, initBase);
    CharModel boxCore(shaderProgram, TRSMatricesHolder(translate(mat4(1.0f), vec3(0, cubeCenterHeight, 0)), rotate(mat4(1.0f), radians(180.0f), vec3(0,0,1)) , mat4(1.0f)));
    ModelFace boxTop(shaderProgram);
    ModelFace boxBot(shaderProgram);
    ModelFace boxPort(shaderProgram);
    ModelFace boxStarboard(shaderProgram);
    ModelFace boxFront(shaderProgram);
    ModelFace boxBack(shaderProgram);

    CharModel boxRotater(shaderProgram);

    boxTop.setAxis(vec3(0, -1, 0));
    boxBot.setAxis(vec3(0, -1, 0));
    boxPort.setAxis(vec3(-1, 0, 0));
    boxStarboard.setAxis(vec3(-1, 0, 0));
    boxFront.setAxis(vec3(0, 0, 1));
    boxBack.setAxis(vec3(0, 0, 1));

    ModelV9 v9(shaderProgram);
    ModelS3 s3(shaderProgram, TRSMatricesHolder(translate(mat4(1.0f), vec3(25,0,0)), mat4(1.0f), mat4(1.0f)));
    ModelA9 a9(shaderProgram);
    ModelN2 n2(shaderProgram);
    ModelN4 n4(shaderProgram);
    vModels.push_back(&boxCore);
    vModels.push_back(&base);
    vModels.push_back(&s3);
    vModels.push_back(&n2);
    vModels.push_back(&a9);
    vModels.push_back(&n4);
    vModels.push_back(&v9);

    //parts
    vector<CharModel*>::iterator itBox;
    //ModelN2 n2a(shaderProgram);
    //ModelA9 a9a(shaderProgram);
    //ModelS3 s3a(shaderProgram);
    //vector<CharModel*> attachedToS3;
    //attachedToS3.push_back(&n2a);
    //attachedToS3.push_back(&a9a);
    //vector<CharModel*> attachedToN2;
    //attachedToN2.push_back(&a9a);
    //attachedToN2.push_back(&s3a);
    //s3.setAttachedModels(attachedToS3);
    //n2.setAttachedModels(attachedToN2);


    //for some reason, keeps sending the same object reference.
    //for (init_T_itr = init_T.begin(); init_T_itr != init_T.end(); init_T_itr++) {
    //    TRSMatricesHolder tempTMatrix = TRSMatricesHolder(*init_T_itr, mat4(1.0f), mat4(1.0f));
    //    ModelBox tempBox = ModelBox(shaderProgram, tempTMatrix);
    //    attachedToBox.push_back(&tempBox);
    //}
    //do it manually instead
    ModelBox C1(shaderProgram, init_T[getCoordB(0,0,0)]);
    ModelBox C2(shaderProgram, init_T[getCoordB(1,0,0)]);
    ModelBox C3(shaderProgram, init_T[getCoordB(2,0,0)]);
    ModelBox C4(shaderProgram, init_T[getCoordB(0,1,0)]);
    ModelBox C5(shaderProgram, init_T[getCoordB(1,1,0)]);
    ModelBox C6(shaderProgram, init_T[getCoordB(2,1,0)]);
    ModelBox C7(shaderProgram, init_T[getCoordB(0,2,0)]);
    ModelBox C8(shaderProgram, init_T[getCoordB(1,2,0)]);
    ModelBox C9(shaderProgram, init_T[getCoordB(2,2,0)]);
                                              
    ModelBox B1(shaderProgram, init_T[getCoordB(0,0,1)]);
    ModelBox B2(shaderProgram, init_T[getCoordB(1,0,1)]);
    ModelBox B3(shaderProgram, init_T[getCoordB(2,0,1)]);
    ModelBox B4(shaderProgram, init_T[getCoordB(0,1,1)]);
    ModelBox B5(shaderProgram, init_T[getCoordB(1,1,1)]);
    ModelBox B6(shaderProgram, init_T[getCoordB(2,1,1)]);
    ModelBox B7(shaderProgram, init_T[getCoordB(0,2,1)]);
    ModelBox B8(shaderProgram, init_T[getCoordB(1,2,1)]);
    ModelBox B9(shaderProgram, init_T[getCoordB(2,2,1)]);
                                              
    ModelBox A1(shaderProgram, init_T[getCoordB(0,0,2)]);
    ModelBox A2(shaderProgram, init_T[getCoordB(1,0,2)]);
    ModelBox A3(shaderProgram, init_T[getCoordB(2,0,2)]);
    ModelBox A4(shaderProgram, init_T[getCoordB(0,1,2)]);
    ModelBox A5(shaderProgram, init_T[getCoordB(1,1,2)]);
    ModelBox A6(shaderProgram, init_T[getCoordB(2,1,2)]);
    ModelBox A7(shaderProgram, init_T[getCoordB(0,2,2)]);
    ModelBox A8(shaderProgram, init_T[getCoordB(1,2,2)]);
    ModelBox A9(shaderProgram, init_T[getCoordB(2,2,2)]);

    vector<CharModel*> attachedToCore(0);
    attachedToCore.push_back(&C1);
    attachedToCore.push_back(&C2);
    attachedToCore.push_back(&C3);
    attachedToCore.push_back(&C4);
    attachedToCore.push_back(&C5);
    attachedToCore.push_back(&C6);
    attachedToCore.push_back(&C7);
    attachedToCore.push_back(&C8);
    attachedToCore.push_back(&C9);

    attachedToCore.push_back(&B1);
    attachedToCore.push_back(&B2);
    attachedToCore.push_back(&B3);
    attachedToCore.push_back(&B4);
    attachedToCore.push_back(&B5);
    attachedToCore.push_back(&B6);
    attachedToCore.push_back(&B7);
    attachedToCore.push_back(&B8);
    attachedToCore.push_back(&B9);

    attachedToCore.push_back(&A1);
    attachedToCore.push_back(&A2);
    attachedToCore.push_back(&A3);
    attachedToCore.push_back(&A4);
    attachedToCore.push_back(&A5);
    attachedToCore.push_back(&A6);
    attachedToCore.push_back(&A7);
    attachedToCore.push_back(&A8);
    attachedToCore.push_back(&A9);

    vector<CharModel*> attachedToRotater(0);
    attachedToRotater.push_back(&boxFront);
    attachedToRotater.push_back(&boxBack);
    attachedToRotater.push_back(&boxTop);
    attachedToRotater.push_back(&boxBot);
    attachedToRotater.push_back(&boxPort);
    attachedToRotater.push_back(&boxStarboard);

    //set face and position index
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                attachedToCore[getCoord(k, j, i)]->setFace(faces[getCoordB(k, j, i)]);
                attachedToCore[getCoord(k, j, i)]->setPosIndex(posIndex[getCoordB(k, j, i)]);
            }
        }
    }

    //internal model to track 27 box positions
    RubikCube rubik(attachedToCore);
    attachBoxToCube(attachedToRotater, rubik);

    //attach parts to base
    boxCore.setAttachedModels(attachedToCore);
    boxRotater.setAttachedModels(attachedToRotater);

    const Color initialBoxColor = { 0.8f,0.8f,0.8f };
    Color selectedBoxColor = { 2,1,1 };
    //set color of boxes
    {
        //set color of all boxes
        boxCore.setAttachedColor(initialBoxColor);
        //set color of initial selection
        selectedPartModel = boxRotater.getAttachedModels()[partIndex];
        selectedPartModel->setAttachedColor(selectedBoxColor);
    }


    //digits parts
    Model_DigitalFont m_hourTens    (shaderProgram, '0', pos_m[0]);
    Model_DigitalFont m_hourOnes    (shaderProgram, '0', pos_m[1]);
    Model_DigitalFont m_hmSeparator (shaderProgram, '-', pos_m[2]);
    Model_DigitalFont m_minutesTens (shaderProgram, '0', pos_m[3]);
    Model_DigitalFont m_minutesOnes (shaderProgram, '0', pos_m[4]);
    Model_DigitalFont m_msSeparator (shaderProgram, '-', pos_m[5]);
    Model_DigitalFont m_secondsTens (shaderProgram, '0', pos_m[6]);
    Model_DigitalFont m_secondsOnes (shaderProgram, '0', pos_m[7]);
    Model_DigitalFont m_ssSeparator (shaderProgram, '-', pos_m[8]);
    Model_DigitalFont m_centiSTens  (shaderProgram, '0', pos_m[9]);
    Model_DigitalFont m_centiSOnes  (shaderProgram, '0', pos_m[10]);

    vector<CharModel*> attachedToBase;
    attachedToBase.push_back(&m_hourTens    );
    attachedToBase.push_back(&m_hourOnes    );
    attachedToBase.push_back(&m_hmSeparator );
    attachedToBase.push_back(&m_minutesTens );
    attachedToBase.push_back(&m_minutesOnes );
    attachedToBase.push_back(&m_msSeparator );
    attachedToBase.push_back(&m_secondsTens );
    attachedToBase.push_back(&m_secondsOnes );
    attachedToBase.push_back(&m_ssSeparator );
    attachedToBase.push_back(&m_centiSTens  );
    attachedToBase.push_back(&m_centiSOnes  );

    //attach to base
    base.setAttachedModels(attachedToBase);


    //for some reason, shadows don't display if the parts' draw call aren't called directly.
    modelsAndParts = vModels;
    for (int i = 0; i < numMainModels; i++) {
        vector<CharModel*> temp;
        vector<CharModel*>::iterator it;
        switch (i) {
        case 0: 
            temp = attachedToCore;
            break;
        case 1:
            temp = attachedToBase;
            break;
        //case 2:
        //    temp = attachedToS3;
        //    break;
        //case 3:
        //    temp = attachedToN2;
        //    break;
        }
        for (it = temp.begin(); it != temp.end(); it++) {
            //parts
            if (*it) {
                modelsAndParts.push_back(*it);
            }
        }
    }
    //previous frame, if valid input to model.
    bool prevHadMovement = false;
    
    //tracks number of random turns to make.
    int turnNum = 0;
    float timeThreshold = 0;
    const float timeTurnThreshold = 0.15f;
    //float time = 0;
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix[0]");
    GLuint shadowWorldMatrixLocation = glGetUniformLocation(shaderProgramShadow, "model");


    //inputs for index control
    //part selection
    map<int, int> inputsToIndexIncrement_Part;;
    inputsToIndexIncrement_Part.insert(pair<int, GLchar>(GLFW_KEY_F, -1));
    inputsToIndexIncrement_Part.insert(pair<int, GLchar>(GLFW_KEY_G, 1));
    map<int, int> inputsToIndexIncrement_Theme;;
    inputsToIndexIncrement_Theme.insert(pair<int, GLchar>(GLFW_KEY_Z, 1));

    //Theme sets
    vector<GLuint> textureSet1(0);
    //faces: front, back, top, bot, port, starboard.
    //inner
    textureSet1.push_back(redTextureID);
    textureSet1.push_back(blueTextureID);
    textureSet1.push_back(yellowTextureID);
    textureSet1.push_back(greenTextureID);
    textureSet1.push_back(orangeTextureID);
    textureSet1.push_back(purpleTextureID);
    textureSet1.push_back(metalTextureID);
    vector<GLuint> textureSet2(0);
    textureSet2.push_back(baseballTextureID);
    textureSet2.push_back(basketballTextureID);
    textureSet2.push_back(hockeyTextureID);
    textureSet2.push_back(soccerTextureID);
    textureSet2.push_back(tennisTextureID);
    textureSet2.push_back(bowlingballTextureID);
    textureSet2.push_back(metalTextureID);
    vector<GLuint> textureSet3(0);
    textureSet3.push_back(drumTextureID);
    textureSet3.push_back(fluteTextureID);
    textureSet3.push_back(guitaurTextureID);
    textureSet3.push_back(pianoTextureID);
    textureSet3.push_back(triangleTextureID);
    textureSet3.push_back(trumpetTextureID);
    textureSet3.push_back(metalTextureID);
    vector<GLuint> textureSet4(0);
    textureSet4.push_back(Logo1TextureID);
    textureSet4.push_back(Logo2TextureID);
    textureSet4.push_back(Logo3TextureID);
    textureSet4.push_back(Logo4TextureID);
    textureSet4.push_back(Logo5TextureID);
    textureSet4.push_back(Logo6TextureID);
    textureSet4.push_back(metalTextureID);
    vector<vector<GLuint>> textureSets(0);
    textureSets.push_back(textureSet1);
    textureSets.push_back(textureSet2);
    textureSets.push_back(textureSet3);
    textureSets.push_back(textureSet4);
    //theme selection

    //put store info, so can render easier for shadow map.
    RenderInfo renderInfo;

    renderInfo.colorLocation = colorLocation;
    renderInfo.enableTextureLocation = enableTextureLocation;
    renderInfo.enableShadowLocation = enableShadowLocation;

    renderInfo.enableTexture = enableTexture;
    renderInfo.enableShadow = enableShadow;
    renderInfo.toggleSkybox = toggleSkybox;

    renderInfo.textures.depthMap = depthMap;
    renderInfo.textures.tiledTextureID = tiledTextureID;
    //renderInfo.textures.boxTextureID = boxTextureID;
    renderInfo.textures.metalTextureID = metalTextureID;
    renderInfo.textures.skybox = t5ID;

    renderInfo.vao.cubeVAO = cubeVAOa;
    renderInfo.vao.sphereVAO = sphereVAOa;
    renderInfo.vao.halfSphereVAO = halfSphereVAOa;

    renderInfo.backgroundColor = backgroundColor;
    // Entering Main Loop
    while (!glfwWindowShouldClose(window))
    {
        // Frame time calculation
        currentTime = glfwGetTime();
        float dt = currentTime - lastFrameTime;
        lastFrameTime += dt;
        timeThreshold += dt;

        selectedBoxColor = Color(
            1.6f + 0.4f * cosf(radians(currentTime * 22.5f*10 / 8)),
            1.6f + 0.4f * cosf(radians(currentTime * 20 * 10 / 9 + 120)),
            1.6f + 0.4f * cosf(radians(currentTime * 25 * 10 / 7 - 120)));
        // Each frame, reset color of each pixel to glClearColor
        // @TODO 1 - Clear Depth Buffer Bit as well
        // ...
        //const float speed = dt * 30;
        //time += speed;

        //if cube is solved, wildly change background color.
        if (rubik.checkSolved()) { 
            backgroundColor = Color(0.4f * (1 + cosf(radians(45 * 1.3f * currentTime))), 0.4f * (1 + cosf(radians(45 * 1.5f * currentTime + 120))), 0.4f * (1 + cosf(radians(45 * 1.7f * currentTime - 120))));
        }
        else {
            backgroundColor = Color(164 / 255.0f, 173 / 255.0f, 176 / 255.0f);
        }
        glClearColor(backgroundColor.red, backgroundColor.green, backgroundColor.blue, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Handle timer
        string time = getTimerString();
        if (!paused)
        {
            // Computes seconds and decimals elapsed since the last minute
            float elapsed = (currentTime - timeReset - totalTimePaused) - (hours * 60 * 60 + minutes * 60);
            seconds = (int) elapsed;
            decimals = (int) ((elapsed - seconds) * 100);
            // Increment minutes and hours as needed
            adjustTimer();
        }
        
        //https://learnopengl.com/Advanced-Lighting/Shadows/Shadow-Mapping
        // 1. render depth of scene to texture (from light's perspective)
       // --------------------------------------------------------------
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = -40.0f, far_plane = 90.0f;
        lightProjection = glm::ortho(-80.0f, 80.0f, -80.0f, 80.0f, near_plane, far_plane);
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
        //glBindTexture(GL_TEXTURE_2D, tiledTextureID); 
        //avoid peter panning
        //glCullFace(GL_FRONT);

        renderInfo.shaderProgram = shaderProgramShadow;
        renderInfo.worldMatrixLocation = shadowWorldMatrixLocation;
        renderInfo.backgroundColor = backgroundColor;

        //rubik textures
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, rubikBackTexture);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, rubikPortTexture);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, rubikFrontTexture);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, rubikStarboardTexture);
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, rubikTopTexture);
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, rubikBotTexture);
        //inner
        glActiveTexture(GL_TEXTURE8);
        glBindTexture(GL_TEXTURE_2D, rubikInnerTexture);
        glActiveTexture(GL_TEXTURE0);
        

        //renderScene(shaderProgramShadow, cubeVAOa);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVAOa);
        //renderDecor(renderInfo);
        renderModels(renderInfo, modelsAndParts, attachedToCore,true);

        //{
        //    // Draw sphere
        //    //attach the sphere to the model with a point lower/higher than its center.
        //    const float yHover = 5.0f;
        //    const float xOffset = 15;
        //    const float yOffset = 0 + yHover;
        //    const float scaler = 5.5f;
        //    //mat4 worldMatrix = translate(mat4(1.0f), vec3(xOffset, yOffset, 0.0f)) * scale(mat4(1.0f), glm::vec3(-scaler, -scaler, -scaler));
        //    mat4 worldMatrix = translate(mat4(1.0f), vec3(xOffset, yOffset, 0.0f)) * scale(mat4(1.0f), glm::vec3(scaler, scaler, scaler));
        //    mat4 mWorldMatrix =  worldMatrix;
        //    glUniformMatrix4fv(shadowWorldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
        //    //match these numbers to those passed to sphereVertices().
        //    const int heightParts = 16;
        //    const int ringParts = 30;
        //    const int numVertices = (heightParts - 1) * ringParts * 2 * 2;
        //    glDrawArrays(GL_TRIANGLE_STRIP, 44, numVertices);
        //    //CharModel::drawLetter(models);
        //    //CharModel::drawNumber(models);
        //    //Sphere has no texture for now.
        //    //CharModel::drawSphere(models);
        //}
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
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glBindVertexArray(cubeVAOa);
        // Draw geometry
        glBindBuffer(GL_ARRAY_BUFFER, cubeVAOa);

        //bind shadow map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tiledTextureID);

        renderInfo.shaderProgram = shaderProgram;
        renderInfo.worldMatrixLocation = worldMatrixLocation;
        renderInfo.toggleSkybox = toggleSkybox;

        //Draw ground
        renderDecor(renderInfo);
        //draw models
        {
            //selection with keys.
            drawControl(window);
            modelIndex = selectModelControl(window, modelIndex);
            partIndex = indexControl(window, partIndex, numAttachedModelsPerMain, inputsToIndexIncrement_Part, previousKeyStates);
            textureThemeIndex = indexControl(window, textureThemeIndex, textureSets.size(), inputsToIndexIncrement_Theme, previousKeyStates);
            CharModel* prevModel = selectedModel;
            selectedModel = vModels[modelIndex];
            //CharModel* selectedPartModel = selectedModel->getAttachedModels()[partIndex];
            CharModel* prevSelectedPartModel = selectedPartModel;
            selectedPartModel = boxRotater.getAttachedModels()[partIndex];
            //Control model key presses.
            mat4* relativeWorldMatrix = modelControl(window, dt, previousKeyStates);
            bool hasMovement = checkModelMovement(window, previousKeyStates);
            bool* selectedSetting = customControl(window, previousKeyStates);
            selectedPartModel->setAttachedColor(selectedBoxColor);
            //Home key has been pressed, so reset world matrices.
            if (selectedSetting[0] || selectedSetting[1]) {
                //reset TRS matrices
                selectedModel->reset();
                //reset walk state.
                selectedModel->updateWalkProgress(0, 0);

                //reset whole cube
                rubik.reset();
                attachBoxToCube(attachedToRotater, rubik);
                //set color of boxes
                {
                    //set color of all boxes
                    boxCore.setAttachedColor(initialBoxColor);
                    //set color of selection
                    selectedPartModel->setAttachedColor(selectedBoxColor);
                }

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
            }
            //B key has been pressed, so toggle shadows.
            if (selectedSetting[3]) {
                enableShadow = enableShadow * -1 + 1;
            }
            //c pressed, so go to next mode (rotation)
            if (selectedSetting[4] || selectedSetting[5]) {
                //make selected part model display next mode
                selectedPartModel->setAttachedMotionData(selectedPartModel->getMotionData());
                selectedPartModel->attachedNext();
                
                //rotate internal model, and apply to cube.
                rubik.rotateFace(1, partIndex);
                attachBoxToCube(attachedToRotater, rubik);
            }
            //v pressed, so go to prev mode (rotation)
            if (selectedSetting[6] || selectedSetting[7]) {
                //make selected part model display previous mode
                selectedPartModel->setAttachedMotionData(selectedPartModel->getMotionData());
                selectedPartModel->attachedPrev();

                //rotate internal model, and apply to cube.
                rubik.rotateFace(-1, partIndex);
                attachBoxToCube(attachedToRotater, rubik);
            }
            //r pressed, so do random turn
            if (selectedSetting[8] || selectedSetting[9]) {
                //if control is also held, make many random turns.
                if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
                {
                    timeThreshold = 0 + timeTurnThreshold;  //make first turn immediately
                    //toggle many random turns
                    if (turnNum) {
                        turnNum = 0;
                    }
                    else {
                        turnNum = 100;
                    }
                }
                //otherwise, make a random turn per input.
                else {
                    int randIndex = randomTurn(attachedToRotater, &rubik);//set color of selection
                    //reset and set colors
                    boxRotater.getAttachedModels()[randIndex]->setAttachedColor(initialBoxColor);
                    selectedPartModel->setAttachedColor(selectedBoxColor);
                }
            }
            //z key has been pressed, so cycle through textures.
            if (selectedSetting[10] || selectedSetting[11]) {
                rubikFrontTexture     = textureSets[textureThemeIndex][0];
                rubikBackTexture      = textureSets[textureThemeIndex][1];
                rubikTopTexture       = textureSets[textureThemeIndex][2];
                rubikBotTexture       = textureSets[textureThemeIndex][3];
                rubikPortTexture      = textureSets[textureThemeIndex][4];
                rubikStarboardTexture = textureSets[textureThemeIndex][5];
                rubikInnerTexture     = textureSets[textureThemeIndex][6];
            }
            //O key has been pressed, so toggle skybox.
            if (selectedSetting[12]) {
                toggleSkybox = !toggleSkybox;
            }
            //many random turns made over period of time.
            //also make time in-between turns vary a bit.
            if (turnNum > 0 && (1 + 0.2f*sin(lastFrameTime*2))*timeThreshold > timeTurnThreshold) {
                turnNum--;
                timeThreshold -= timeTurnThreshold;
                int randIndex = randomTurn(attachedToRotater, &rubik);//set color of selection
                //reset and set colors
                boxRotater.getAttachedModels()[randIndex]->setAttachedColor(initialBoxColor);
                selectedPartModel->setAttachedColor(selectedBoxColor);
            }

            //Adjust selected model accordingly.
            selectedModel->addRelativeWorldMatrix(relativeWorldMatrix[0], relativeWorldMatrix[1], relativeWorldMatrix[2]);

            //checks whether the models move
            if (hasMovement) {
                //start walking
                selectedModel->updateWalkProgress(0, 1);
            }
            else if (prevHadMovement){
                //stop walking
                selectedModel->updateWalkProgress(0, 2);
            }
            prevHadMovement = hasMovement;

            //makes previous model stop walking, if it is walking.
            if (prevModel != selectedModel && prevModel && prevModel->getCycle()->getState() != 0) {
                prevModel->updateWalkProgress(0, 2);
            }
            
            //when selecting different faces,
            if (prevSelectedPartModel != selectedPartModel && prevSelectedPartModel) {
                //reset color of previous selection
                prevSelectedPartModel->setAttachedColor(initialBoxColor);
                //set color of selection
                selectedPartModel->setAttachedColor(selectedBoxColor);
            }

            renderModels(renderInfo, vModels, attachedToCore);
            //update
            CharModel::update(vModels, dt);
            CharModel::update(attachedToCore, dt);
            CharModel::resetCumulativeTRS(vModels);
            CharModel::updateAttachedCumulativeTRS(vModels);

            //display time on timer
            base.selectMode(time);
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
        projectionMatrix = glm::perspective(radians(foV),   // field of view in degrees
            (float)window_width / (float)window_height,     // aspect ratio
            0.01f, 200.0f);                                 // near and far (near > 0)
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
    
    // Temporary controls for timer
    if (key == GLFW_KEY_9 && action == GLFW_PRESS) {
        paused = !paused; // pause/resume timer
        handlePauseTimer();
    }
    if (key == GLFW_KEY_0 && action == GLFW_PRESS)
        resetTimer(); // reset timer
}

// Returns the current timer in "HH-MM-SS-DD" format
string getTimerString() {
    string timerOutput = "";
    
    //add hours
    if (hours < 10)
        timerOutput += "0" + to_string(hours);
    else
        timerOutput += to_string(hours);
    
    //hours-minutes separator
    timerOutput += "-";
    
    //add minutes
    if (minutes < 10)
        timerOutput += "0" + to_string(minutes);
    else
        timerOutput += to_string(minutes);

    //minutes-seconds separator
    timerOutput += "-";
    
    //add seconds
    if (seconds < 10)
        timerOutput += "0" + to_string(seconds);
    else
        timerOutput += to_string(seconds);

    //seconds-centiseconds separator
    timerOutput += "-";
        
    //add centiseconds
    if (decimals < 10)
        timerOutput += "0" + to_string(decimals);
    else
        timerOutput += to_string(decimals);
    
    //output time in string
    return timerOutput;
}

// Reset seconds and minutes variables when they reach 60
void adjustTimer()
{
    // if seconds reaches 60
    if (seconds >= 60)
    {
        // increment minutes
        minutes++;
        // if minutes reaches 60
        if (minutes == 60)
        {
            // increment hours
            hours++;
            minutes = 0;
        }
        // reduce seconds by equivalent minutes.
        seconds -= 60;
    }
}

// Handle pause/resume timer events
void handlePauseTimer()
{
    if (paused)
        // Store time at which timer is paused
        timePause = currentTime;
    else
    {
        // Store time at which timer is resumed
        timeResume = currentTime;
        // Add to cumulated time for which the time is paused
        totalTimePaused += timeResume - timePause;
    }
}

// Reset timer to inital time
void resetTimer()
{
    // Reset time variables
    hours = 0;
    minutes = 0;
    seconds = 0;
    decimals = 0;
    // Store time at which the time was reset
    timeReset = currentTime;
    // Handle pause functionalitites for resetting
    totalTimePaused = 0;
    if (paused)
        timePause = timeReset;
}
