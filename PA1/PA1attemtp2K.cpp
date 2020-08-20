//
// Concordia University COMP 371 Programming Assignment 3, Summer 2020.
//
// Created by :
//    Jason Beccherini (40130107)
//    Manuel Romero (40023706)
//    Gia-Khang Ernest Nguyen (40091426)
//    Wing-Fei Jason Tiu (40048495)
//    Kevin Rao (40095427)
// due August 21th, 2020.
//

//cube controls (temporary)
// select model 1 - WASD to move cube, ad to rotate.
// - - - - - - - - - - - - - - - - - - - - - - 
// - cv to rotate selected side
// - fg to select side to rotate
//
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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <vector>

using namespace glm;
using namespace std;

GLuint loadTexture(const char* filename);

const int numMainModels = 6;
const int numAttachedModelsPerMain = 6;

// Define (initial) window width and height
int window_width = 1024, window_height = 768;

// Position of light source
//slightly offcenter, because the light doesn't like looking straight down.
vec3 lightPos = vec3(0.0f, 30.0f, 0.001f);
//vec3 lightPos = vec3(-2.0f, 4.0f, -1.0f);

// Callback function for handling window resize and key input
void window_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

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

        direction = 1;
    }
    //1 or -1 direction
    void setDirection(int dir) {
        direction = dir/labs(dir);
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
        //const float smoothPos = a / (1 + a);
        //const float smoothPos = 2 * currentPosition / (1 + currentPosition);
        //const float smoothPos = 2 * currentPosition - currentPosition * currentPosition;
        //derivative is parabola with value 0 at pos 0 and 1.
        const float smoothPos = 3 * currentPosition * currentPosition - 2 * currentPosition * currentPosition * currentPosition;
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
        //*attachedModels = nullptr;
        numAttachedModels = 0;

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

    //lazy getter
    virtual WalkCycle* getCycle() {
        return &walkState;
    }

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


    mat4 getRelativeTranslateMatrix() {
        mat4 motion = getMotion(updateWalkProgress(0)).translateMatrix;
        return relativeTranslateMatrix * motion;
    }
    void setRelativeTranslateMatrix(mat4 relTWM) {
        relativeTranslateMatrix = relTWM;
    }
    void addRelativeTranslateMatrix(mat4 relTWM) {
        relativeTranslateMatrix = relTWM * relativeTranslateMatrix;
    }

    mat4 getRelativeRotateMatrix() {
        mat4 motion = getMotion(updateWalkProgress(0)).rotateMatrix;
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
        //mat4 motion = getMotion(updateWalkProgress(0));
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
        mat4 motion = getMotion(updateWalkProgress(0)).scaleMatrix;
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
    ////temp
    //virtual void applyOffset(mat4){}
    //static void setOffset(vector<CharModel*> arr, mat4 off) {
    //    vector<CharModel*>::iterator it;
    //    for (it = arr.begin(); it != arr.end(); it++) {
    //        if (*it) {
    //            (*it)->applyOffset(off);
    //        }
    //    }
    //}

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


    void attachedNext() {
        vector<CharModel*>::iterator it;
        for (it = attachedModels.begin(); it != attachedModels.end(); it++) {
            if (*it) {
                (*it)->next();
                //temp to start
                (*it)->updateWalkProgress(0, 1);
            }
        }
    }
    void attachedPrev() {
        vector<CharModel*>::iterator it;
        for (it = attachedModels.begin(); it != attachedModels.end(); it++) {
            if (*it) {
                (*it)->prev();
                //temp to start
                (*it)->updateWalkProgress(0, 1);
            }
        }
    }
    //reverts TRS matrices back to initial settings.
    void resetInitialRelativeMatrices() {
        setRelativeWorldMatrix(initial_relativeTranslateMatrix, initial_relativeRotateMatrix, initial_relativeScaleMatrix);
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
    //let other class handle walking.
    WalkCycle walkState;

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
        //const float position = updateWalkProgress(0);

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
    ////rotation
    //TRSMatricesHolder rotateMotion(float position) {
    //    //position should be between 0 to 1.
    //    const float amplitude = 4;
    //    const float rotationAngle = 90.0f;
    //    //const float position = updateWalkProgress(0);

    //    //shear side-to-side + vertical scaling.
    //    mat4 motion =
    //        rotate(mat4(1.0f),
    //            radians(90.0f) * position,
    //            vec3(0.0f, 0.0f, -1.0f));
    //    TRSMatricesHolder(tempR);
    //    tempR.rotateMatrix = motion;
    //    return tempR;
    //}
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
        //untested besides scale component
        TRSMatricesHolder motion = getMotion(updateWalkProgress(0));

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
        glDrawArrays(GL_TRIANGLE_STRIP, 44, numVertices);
    }
    //bool modeMotion;

    GLuint worldMatrixLocation;
    GLuint colorLocation;
    mat4 initial_relativeTranslateMatrix;   //Initial translate matrix, value to take when reset.
    mat4 initial_relativeRotateMatrix;      //Initial rotate matrix, value to take when reset.
    mat4 initial_relativeScaleMatrix;       //Initial scale matrix, value to take when reset.
    float initY;                            // Initial y-position in initial translate matrix (note: value assigned to child constructor)

    MotionData motionD;
    Color color;
    SphereOffset sphereOffset;
    TRSMatricesHolder cumulativeTRS;
    vector<CharModel*> attachedModels;
    int numAttachedModels;
private:
    mat4 relativeTranslateMatrix;   //Stored translate matrix
    mat4 relativeRotateMatrix;      //Stored rotate matrix
    mat4 relativeScaleMatrix;       //Stored scale matrix
};
class ModelBox : public CharModel {
public:
    ModelBox(int shaderProgram, TRSMatricesHolder ini_relTRSMatrices = TRSMatricesHolder(), mat4 modelOffset = mat4(1.0f))
        : CharModel(shaderProgram, ini_relTRSMatrices), cubeOffset(modelOffset){
        
        //replace motion with rotation
        // or could use own attribute instead.
        rotateState = RotateCycle(0.8f);

        //angle to rotate towards.
        targetAngle = 0;
    }
    void applyOffset(mat4 offset) {
        cubeOffset = offset * cubeOffset;
    }
    void draw() {
        //pass arguments stored in parent class.
        //glUniform3f(colorLocation, 0.0f, 233.0f / 255.0f, 1.0f);
        glUniform3f(colorLocation, color.red, color.green, color.blue);
        drawCube(worldMatrixLocation, colorLocation, getRelativeWorldMatrix() * cubeOffset);
        //drawCube(worldMatrixLocation, colorLocation, getRelativeWorldMatrix() * cubeOffset * cumulativeTRS.trs());
    }
    void next() {
        targetAngle = 90.0f;
        addRelativeRotateMatrix(
            rotate(mat4(1.0f),
                radians(90.0f),
                motionD.axisRotation));
    }
    void prev() {
        targetAngle = -90.0f;
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
    
        //shear side-to-side + vertical scaling.
        //mat4 motion =
        //    rotate(mat4(1.0f),
        //        radians(rotationAngle) * position,
        //        vec3(0.0f, 0.0f, -1.0f));
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
        scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 3.0f, 3.0f));
        worldMatrix = translationMatrix * scalingMatrix;
        mWorldMatrix = relativeWorldMatrix * worldMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &mWorldMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    float targetAngle;
    RotateCycle rotateState;
private:
    mat4 cubeOffset;
};

//only stores axis for rotation purposes.
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
vector<vec3> sphereVertices(vec3 prevVertexArray[44 * 2],const int heightParts, const int ringParts) {

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
    for (int i = 0; i < vertexArrNum * 2 ; i++) {
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
                float y = radius *(sinf(radians(2* 90*((float)(i + alternate) / (heightParts - 1) - 0.5f))));
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
        
        // 1261 Sphere vertices (from lab5) (44)
    };


    ////fix change in way storing normal (should be same as position).
    //for (int i = 0; i < 44; i++) {
    //    vertexArray[2*i + 1] = vertexArray[2 * i];
    //}
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


    const int vertexArrayNum = 44;
    const int heightParts = 22;
    const int ringParts = 30; 
    //make sure the sphere draw function draws [(heightParts-1)*ringParts*2] figures.
    //https://stackoverflow.com/questions/4264304/how-to-return-an-array-from-a-function
    vector<vec3> vertexArraySphere = sphereVertices(vertexArray, heightParts, ringParts);

    vec3 vertexArr2[vertexArrayNum *2 + (heightParts-1)*ringParts*2*2];
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

    //////shadow calculation
    //"float ShadowCalculation(vec4 fragPosLightSpace)"
    //"{"
    ////manual perspective divide (for perspective projection)
    //"    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;"
    ////range from [-1, 1] to [0, 1].
    //"    projCoords = projCoords * 0.5 + 0.5;"
    ////closest depth value
    //"    float closestDepth = texture(shadowMap, projCoords.xy).r;"
    ////"    float closestDepth = 0;"
    ////current depth value
    //"    float currentDepth = projCoords.z;"
    ////is in shadow if not the closest depth.
    //"    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;"
    ////"    shadow = currentDepth;"
    //"    if (projCoords.z > 1.0){"
    //"        shadow = 0.0;"
    //"    }"
    //"    return shadow;"
    //"}"
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
    "float bias = 0.05;"
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

//float fakeTime = 0;
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

    //fakeTime += 0.01f;
    //float fakeCycle = 3.5f*sinf(fakeTime * 0.7f)* sinf(fakeTime * 0.7f);
    //model = glm::mat4(1.0f);
    //model = glm::translate(model, glm::vec3(2.0f, -1.5f, 2.0));
    //model = glm::rotate(model, radians(45.0f + fakeTime*20),glm::vec3(2.0f, -1.5f + fakeCycle, 2.0));
    //model = glm::scale(model, glm::vec3(1.1f + fakeCycle));
    //glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
    //renderCube();
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
    const int numCells = (int)pow(sideLength,2);
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
int partModelControl(GLFWwindow* window, int prevPartIndex, map<int, KeyState> previousKeyStates) {

    map<int, int> inputsToModelIndex;
    map<int, int>::iterator itr;
    inputsToModelIndex.insert(pair<int, GLchar>(GLFW_KEY_F, -1));
    inputsToModelIndex.insert(pair<int, GLchar>(GLFW_KEY_G, 1));

    int indexIncrement = 0;
    //iterate through all keys that could be pressed.
    for (itr = inputsToModelIndex.begin(); itr != inputsToModelIndex.end(); ++itr) {
        int keyState = glfwGetKey(window, itr->first);
        int previousState = returnPreviousKeyState(window, itr->first, previousKeyStates);
        if (keyState == GLFW_PRESS && previousState == GLFW_RELEASE) // increment/decrement partIndex
        {
            indexIncrement += itr->second;
        }
    }

    //positive modulo 
    //https://stackoverflow.com/questions/14997165/fastest-way-to-get-a-positive-modulo-in-c-c
    const float mod = numAttachedModelsPerMain;
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
        //signal toggle lights  //not implemented
        {GLFW_KEY_L, true},
        //signal toggle skybox  //not implemented
        {GLFW_KEY_O, true},
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
    int boxTextureID;
    int metalTextureID;
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

    // Draw ground
    glUniform1i(enableShadowLocation, enableShadow);
    //draw grid
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform3f(colorLocation, 1.0f, 1.0f, 1.0f);
    glUniform1i(enableTextureLocation, 0);
    drawGrid(worldMatrixLocation, colorLocation, mat4(1.0f));
    // draw axis
    drawAxes(worldMatrixLocation, colorLocation);
    glBindVertexArray(cubeVAOa);
    glUniform1i(enableTextureLocation, enableTexture);

    //draw tiles
    glBindTexture(GL_TEXTURE_2D, tiledTextureID);
    glUniform3f(colorLocation, 0.8f, 0.4f, 0.8f);
    drawTileGrid(worldMatrixLocation, mat4(1.0f));
}
void renderModels(RenderInfo renderInfo, vector<CharModel*> models) {
    int shaderProgram = renderInfo.shaderProgram;
    GLuint colorLocation = renderInfo.colorLocation;
    GLuint enableTextureLocation = renderInfo.enableTextureLocation;
    GLuint worldMatrixLocation = renderInfo.worldMatrixLocation;
    int enableTexture = renderInfo.enableTexture;
    int boxTextureID = renderInfo.textures.boxTextureID;
    int metalTextureID = renderInfo.textures.metalTextureID;

    //draw all models
    GLuint temp = CharModel::swapWorldMatrixLocation(models, worldMatrixLocation);
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
    CharModel::draw(models);
    glUniform1i(enableTextureLocation, enableTexture);

    //swap back
    CharModel::swapWorldMatrixLocation(models, temp);
}

//index of box at cooridnates, 3x3x3 only
int getCoord(int x, int y, int z) {
    return 1 * x + 3 * y + 9 * z;
    //int arr[3][3][3] = {
    //    { {19,  0,  1}, {21,  2,  3}, {23,  4 , 5} },
    //    { {18, 25,  6}, {20, 26,  8}, {22, 24 ,10} },
    //    { {13, 12,  7}, {15, 14,  9}, {17, 16 ,11} }
    //};
    //int a= arr[z][y][x];
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
        for (int i = 0; i < 27; i++) {
            positionsIndex[i] = i;
        }
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
    GLuint tiledTextureID = loadTexture("Textures/brick.jpg");
    GLuint cementTextureID = loadTexture("Textures/cement.jpg");
    GLuint metalTextureID = loadTexture("Textures/metal.jpg");
    GLuint boxTextureID = loadTexture("Textures/box.jpg");

#else
    //GLuint tiledTextureID = loadTexture("../Assets/Textures/brick.jpg");
    //GLuint cementTextureID = loadTexture("../Assets/Textures/cement.jpg");
    //GLuint metalTextureID = loadTexture("../Assets/Textures/metal.jpg");
    //GLuint boxTextureID = loadTexture("../Assets/Textures/box.jpg");
    GLuint brickTextureID = loadTexture("Assets/Textures/brick.jpg");
    GLuint tiledTextureID = loadTexture("Assets/Textures/tiled.jpg");
    GLuint cementTextureID = loadTexture("Assets/Textures/cement.jpg");
    GLuint metalTextureID = loadTexture("Assets/Textures/metal.jpg");
    GLuint boxTextureID = loadTexture("Assets/Textures/box.jpg");

#endif
    // Grey background
    //glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
    glClearColor(164 / 255.0f, 173 / 255.0f, 176 / 255.0f, 1.0f);

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

    //used for draw instancing.
    vec3 instanceVec[100];
    instanceVec[0] = vec3(0, 0, 0);
    // Define and upload geometry to the GPU here ...
    int cubeVAOa = createTexturedCubeVertexArrayObject();
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
    //toggle lights
    previousKeyStates.insert(pair<int, KeyState>(GLFW_KEY_L, { GLFW_RELEASE , true }));
    //toggle skybox
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

    //vector<TRSMatricesHolder> init_T(0);
    vector<mat4> init_T(0);
    vector<mat4>::iterator init_T_itr;
    const float boxSideLength = 3.0f;
    const float boxSpacing = boxSideLength * 0.10f *5;
    const int boxPerSide = 3;
    const float cubeLength = boxPerSide / 2 * (boxSideLength + boxSpacing);
    //const float cubeLengtha = boxPerSide / 2 * (boxSideLength + boxSpacing);
    const float cubeCenterHeight = boxPerSide / 2 * boxSideLength + boxSideLength;
    // indexes 0 to 2 is back bottom row
    // indexes 0 to 8 is back wall
    for (int i = 0; i < boxPerSide; i++) {
        for (int j = 0; j < boxPerSide; j++) {
            for (int k = 0; k < boxPerSide; k++) {
                mat4 tempInit_T = translate(mat4(1.0f),
                    vec3(-(cubeLength * (boxPerSide - 1) / 2) + k * cubeLength,
                        -(cubeLength * (boxPerSide - 1) / 2) + j * cubeLength,
                        -(cubeLength * (boxPerSide - 1) / 2) + i * cubeLength));
                //TRSMatricesHolder tempinit_TRS = TRSMatricesHolder(tempInit_T, mat4(1.0f), mat4(1.0f));
                //init_T.push_back(tempinit_TRS);
                init_T.push_back(tempInit_T);
            }
        }
    }

    //indexes 0 to 5 are 6 left-most cells of tall 3x3 in the back.
    //construct initial positions for sides
    //i from [0,3] are sides, [4,5] are top/bot, [6] center
    //float cubeLength = cubeLengtha;
    //for (int i = 0; i < 7; i++) {
    //    mat4 rotateM;
    //    switch (i) {
    //    case 0: case 1: case 2: case 3:
    //    {
    //        for (int j = 0; j < boxPerSide; j++) {
    //            for (int k = 1; k < boxPerSide; k++) {
    //                mat4 tempInit_T = 
    //                    rotate(mat4(1.0f), radians(90.0f * i), vec3(0.0f, -1.0f, 0.0f)) *
    //                    translate(mat4(1.0f),
    //                        vec3(-(cubeLength * (boxPerSide - 1) / 2) + k * cubeLength,
    //                            -(cubeLength * (boxPerSide - 1) / 2) + j * cubeLength,
    //                            -(cubeLength * (boxPerSide - 1) / 2)));
    //                //TRSMatricesHolder tempinit_TRS = TRSMatricesHolder(tempInit_T, mat4(1.0f), mat4(1.0f));
    //                //init_T.push_back(tempinit_TRS);
    //                init_T.push_back(tempInit_T);
    //            }
    //        }
    //    }
    //    break;
    //    case 4: case 5:
    //    {
    //        mat4 tempInit_T =
    //             //rotate(mat4(1.0f), radians(90.0f + 180 * i), vec3(1.0f, 0.0f, 0.0f)) *
    //             translate(mat4(1.0f),
    //                vec3(0,
    //                    0,
    //                    -(cubeLength * (boxPerSide - 1) / 2)));
    //        //TRSMatricesHolder tempinit_TRS = TRSMatricesHolder(tempInit_T, mat4(1.0f), mat4(1.0f));
    //        //init_T.push_back(tempinit_TRS);
    //        init_T.push_back(tempInit_T);
    //    }
    //    break;
    //    case 6:
    //    {
    //        mat4 tempInit_T =
    //            translate(mat4(1.0f),
    //                vec3(0,
    //                    0,
    //                    0));
    //        //TRSMatricesHolder tempinit_TRS = TRSMatricesHolder(tempInit_T, mat4(1.0f), mat4(1.0f));
    //        //init_T.push_back(tempinit_TRS);
    //        init_T.push_back(tempInit_T);
    //    }
    //    break;
    //    }
    //    //cubeLength *= 1.2;
    //}

    //Models
    //CharModel* selectedModel;
    //CharModel* models[numMainModels];
    vector<CharModel*> vModels;
    // v v work-around to get the shadow map of the parts.
    vector<CharModel*> modelsAndParts;
    int modelIndex = 0;
    int partIndex = 0;


    //base
    ////                                                              [x,      y,      z]
    //ModelBox boxCore     (shaderProgram, TRSMatricesHolder(), init_T[1 + 3 * 1 + 9 * 1]);
    //ModelBox boxTop      (shaderProgram, TRSMatricesHolder(), init_T[1 + 3 * 2 + 9 * 1]);
    //ModelBox boxBot      (shaderProgram, TRSMatricesHolder(), init_T[1 + 3 * 0 + 9 * 1]);
    //ModelBox boxPort     (shaderProgram, TRSMatricesHolder(), init_T[2 + 3 * 1 + 9 * 1]);
    //ModelBox boxStarboard(shaderProgram, TRSMatricesHolder(), init_T[0 + 3 * 1 + 9 * 1]);
    //ModelBox boxFront    (shaderProgram, TRSMatricesHolder(), init_T[1 + 3 * 1 + 9 * 2]);
    //ModelBox boxBack     (shaderProgram, TRSMatricesHolder(), init_T[1 + 3 * 1 + 9 * 0]);

    TRSMatricesHolder tempinit_TRS = TRSMatricesHolder();
    //                                   [x       y       z]
    //tempinit_TRS.translateMatrix = init_T[1 + 3 * 1 + 9 * 1];
    //CharModel boxCore     (shaderProgram, tempinit_TRS);
    CharModel boxCore(shaderProgram, TRSMatricesHolder(mat4(1.0f), translate(mat4(1.0f), vec3(0, cubeCenterHeight, 0)), mat4(1.0f)));
    //tempinit_TRS.translateMatrix = init_T[1 + 3 * 2 + 9 * 1];
    //tempinit_TRS.rotateMatrix = rotate(mat4(1.0f), radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
    ModelFace boxTop(shaderProgram, tempinit_TRS);
    tempinit_TRS = TRSMatricesHolder();
    //tempinit_TRS.translateMatrix = init_T[1 + 3 * 0 + 9 * 1];
    //tempinit_TRS.rotateMatrix = rotate(mat4(1.0f), radians(90.0f - 180), vec3(1.0f, 0.0f, 0.0f));
    ModelFace boxBot(shaderProgram, tempinit_TRS);
    tempinit_TRS = TRSMatricesHolder();
    //tempinit_TRS.translateMatrix = init_T[2 + 3 * 1 + 9 * 1];
    //tempinit_TRS.rotateMatrix = rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, -1.0f, 0.0f));
    ModelFace boxPort(shaderProgram, tempinit_TRS);
    //tempinit_TRS.translateMatrix = init_T[0 + 3 * 1 + 9 * 1];
    //tempinit_TRS.rotateMatrix = rotate(mat4(1.0f), radians(270.0f), vec3(0.0f, -1.0f, 0.0f));
    ModelFace boxStarboard(shaderProgram, tempinit_TRS);
    //tempinit_TRS.translateMatrix = init_T[1 + 3 * 1 + 9 * 2];
    //tempinit_TRS.rotateMatrix = rotate(mat4(1.0f), radians(-90.0f), vec3(0.0f, -1.0f, 0.0f));
    ModelFace boxFront(shaderProgram, tempinit_TRS);
    //tempinit_TRS.translateMatrix = init_T[1 + 3 * 1 + 9 * 0];
    tempinit_TRS = TRSMatricesHolder();
    ModelFace boxBack(shaderProgram, tempinit_TRS);

    //tempinit_TRS.translateMatrix = init_T[1 + 3 * 1 + 9 * 1];
    CharModel boxRotater(shaderProgram, tempinit_TRS);
    ////                                         [x,      y,      z]
    //CharModel boxCore     (shaderProgram, init_T[1 + 3 * 1 + 9 * 1]);
    //CharModel boxTop      (shaderProgram, init_T[1 + 3 * 2 + 9 * 1]);
    //CharModel boxBot      (shaderProgram, init_T[1 + 3 * 0 + 9 * 1]);
    //CharModel boxPort     (shaderProgram, init_T[2 + 3 * 1 + 9 * 1]);
    //CharModel boxStarboard(shaderProgram, init_T[0 + 3 * 1 + 9 * 1]);
    //CharModel boxFront    (shaderProgram, init_T[1 + 3 * 1 + 9 * 2]);
    //CharModel boxBack     (shaderProgram, init_T[1 + 3 * 1 + 9 * 0]);

    //boxTop.setAxis(vec3(0, 1, 0));
    //boxBot.setAxis(vec3(0, -1, 0));
    //boxPort.setAxis(vec3(1, 0, 0));
    //boxStarboard.setAxis(vec3(-1, 0, 0));
    //boxFront.setAxis(vec3(0, 0, 1));
    //boxBack.setAxis(vec3(0, 0, -1));

    boxTop.setAxis(vec3(0, -1, 0));
    boxBot.setAxis(vec3(0, -1, 0));
    boxPort.setAxis(vec3(-1, 0, 0));
    boxStarboard.setAxis(vec3(-1, 0, 0));
    boxFront.setAxis(vec3(0, 0, 1));
    boxBack.setAxis(vec3(0, 0, 1));
    //boxTop.setAttachedMotionData({ vec3(0, 1, 0) });
    //boxBot.setAttachedMotionData({ vec3(0, -1, 0) });
    //boxPort.setAttachedMotionData({ vec3(1, 0, 0) });
    //boxStarboard.setAttachedMotionData({ vec3(-1, 0, 0) });
    //boxFront.setAttachedMotionData({ vec3(0, 0, 1) });
    //boxBack.setAttachedMotionData({ vec3(0, 0, -1) });


    ModelV9 v9(shaderProgram);
    ModelS3 s3(shaderProgram, TRSMatricesHolder(translate(mat4(1.0f), vec3(100)), mat4(1.0f), mat4(1.0f)));
    ModelA9 a9(shaderProgram);
    ModelN2 n2(shaderProgram);
    ModelN4 n4(shaderProgram);

    //models[0] = &s3;
    //models[1] = &n2;
    //models[2] = &a9;
    //models[3] = &n4;
    //models[4] = &v9;

    //vModels.push_back(&boxFront);
    //vModels.push_back(&boxBack);
    //vModels.push_back(&boxPort);
    //vModels.push_back(&boxStarboard);
    //vModels.push_back(&boxTop);
    //vModels.push_back(&boxBot);
    vModels.push_back(&boxCore);
    //vModels.push_back(&boxRotater);
    vModels.push_back(&s3);
    vModels.push_back(&n2);
    vModels.push_back(&a9);
    vModels.push_back(&n4);
    vModels.push_back(&v9);

    //parts
    vector<CharModel*>::iterator itBox;
    ModelN2 n2a(shaderProgram);
    ModelA9 a9a(shaderProgram);
    ModelS3 s3a(shaderProgram);
    vector<CharModel*> attachedToS3;
    attachedToS3.push_back(&n2a);
    attachedToS3.push_back(&a9a);
    vector<CharModel*> attachedToN2;
    attachedToN2.push_back(&a9a);
    attachedToN2.push_back(&s3a);
    s3.setAttachedModels(attachedToS3);
    n2.setAttachedModels(attachedToN2);


    //for some reason, keeps sending the same object reference.
    //for (init_T_itr = init_T.begin(); init_T_itr != init_T.end(); init_T_itr++) {
    //    TRSMatricesHolder tempTMatrix = TRSMatricesHolder(*init_T_itr, mat4(1.0f), mat4(1.0f));
    //    ModelBox tempBox = ModelBox(shaderProgram, tempTMatrix);
    //    attachedToBox.push_back(&tempBox);
    //}
    //do it manually instead
    //                                                    [x,      y,      z]
    ModelBox C1(shaderProgram, TRSMatricesHolder(), init_T[0 + 3 * 0 + 9 * 0]);
    ModelBox C2(shaderProgram, TRSMatricesHolder(), init_T[1 + 3 * 0 + 9 * 0]);
    ModelBox C3(shaderProgram, TRSMatricesHolder(), init_T[2 + 3 * 0 + 9 * 0]);
    ModelBox C4(shaderProgram, TRSMatricesHolder(), init_T[0 + 3 * 1 + 9 * 0]);
    ModelBox C5(shaderProgram, TRSMatricesHolder(), init_T[1 + 3 * 1 + 9 * 0]);
    ModelBox C6(shaderProgram, TRSMatricesHolder(), init_T[2 + 3 * 1 + 9 * 0]);
    ModelBox C7(shaderProgram, TRSMatricesHolder(), init_T[0 + 3 * 2 + 9 * 0]);
    ModelBox C8(shaderProgram, TRSMatricesHolder(), init_T[1 + 3 * 2 + 9 * 0]);
    ModelBox C9(shaderProgram, TRSMatricesHolder(), init_T[2 + 3 * 2 + 9 * 0]);

    ModelBox B1(shaderProgram, TRSMatricesHolder(), init_T[0 + 3 * 0 + 9 * 1]);
    ModelBox B2(shaderProgram, TRSMatricesHolder(), init_T[1 + 3 * 0 + 9 * 1]);
    ModelBox B3(shaderProgram, TRSMatricesHolder(), init_T[2 + 3 * 0 + 9 * 1]);
    ModelBox B4(shaderProgram, TRSMatricesHolder(), init_T[0 + 3 * 1 + 9 * 1]);
    ModelBox B5(shaderProgram, TRSMatricesHolder(), init_T[1 + 3 * 1 + 9 * 1]);
    ModelBox B6(shaderProgram, TRSMatricesHolder(), init_T[2 + 3 * 1 + 9 * 1]);
    ModelBox B7(shaderProgram, TRSMatricesHolder(), init_T[0 + 3 * 2 + 9 * 1]);
    ModelBox B8(shaderProgram, TRSMatricesHolder(), init_T[1 + 3 * 2 + 9 * 1]);
    ModelBox B9(shaderProgram, TRSMatricesHolder(), init_T[2 + 3 * 2 + 9 * 1]);

    ModelBox A1(shaderProgram, TRSMatricesHolder(), init_T[0 + 3 * 0 + 9 * 2]);
    ModelBox A2(shaderProgram, TRSMatricesHolder(), init_T[1 + 3 * 0 + 9 * 2]);
    ModelBox A3(shaderProgram, TRSMatricesHolder(), init_T[2 + 3 * 0 + 9 * 2]);
    ModelBox A4(shaderProgram, TRSMatricesHolder(), init_T[0 + 3 * 1 + 9 * 2]);
    ModelBox A5(shaderProgram, TRSMatricesHolder(), init_T[1 + 3 * 1 + 9 * 2]);
    ModelBox A6(shaderProgram, TRSMatricesHolder(), init_T[2 + 3 * 1 + 9 * 2]);
    ModelBox A7(shaderProgram, TRSMatricesHolder(), init_T[0 + 3 * 2 + 9 * 2]);
    ModelBox A8(shaderProgram, TRSMatricesHolder(), init_T[1 + 3 * 2 + 9 * 2]);
    ModelBox A9(shaderProgram, TRSMatricesHolder(), init_T[2 + 3 * 2 + 9 * 2]);

    //ModelBox C1(shaderProgram, init_T[0 + 3 * 0 + 9 * 0]);
    //ModelBox C2(shaderProgram, init_T[1 + 3 * 0 + 9 * 0]);
    //ModelBox C3(shaderProgram, init_T[2 + 3 * 0 + 9 * 0]);
    //ModelBox C4(shaderProgram, init_T[0 + 3 * 1 + 9 * 0]);
    //ModelBox C5(shaderProgram, init_T[1 + 3 * 1 + 9 * 0]);
    //ModelBox C6(shaderProgram, init_T[2 + 3 * 1 + 9 * 0]);
    //ModelBox C7(shaderProgram, init_T[0 + 3 * 2 + 9 * 0]);
    //ModelBox C8(shaderProgram, init_T[1 + 3 * 2 + 9 * 0]);
    //ModelBox C9(shaderProgram, init_T[2 + 3 * 2 + 9 * 0]);

    //ModelBox B1(shaderProgram, init_T[0 + 3 * 0 + 9 * 1]);
    //ModelBox B2(shaderProgram, init_T[1 + 3 * 0 + 9 * 1]);
    //ModelBox B3(shaderProgram, init_T[2 + 3 * 0 + 9 * 1]);
    //ModelBox B4(shaderProgram, init_T[0 + 3 * 1 + 9 * 1]);
    //ModelBox B5(shaderProgram, init_T[1 + 3 * 1 + 9 * 1]);
    //ModelBox B6(shaderProgram, init_T[2 + 3 * 1 + 9 * 1]);
    //ModelBox B7(shaderProgram, init_T[0 + 3 * 2 + 9 * 1]);
    //ModelBox B8(shaderProgram, init_T[1 + 3 * 2 + 9 * 1]);
    //ModelBox B9(shaderProgram, init_T[2 + 3 * 2 + 9 * 1]);

    //ModelBox A1(shaderProgram, init_T[0 + 3 * 0 + 9 * 2]);
    //ModelBox A2(shaderProgram, init_T[1 + 3 * 0 + 9 * 2]);
    //ModelBox A3(shaderProgram, init_T[2 + 3 * 0 + 9 * 2]);
    //ModelBox A4(shaderProgram, init_T[0 + 3 * 1 + 9 * 2]);
    //ModelBox A5(shaderProgram, init_T[1 + 3 * 1 + 9 * 2]);
    //ModelBox A6(shaderProgram, init_T[2 + 3 * 1 + 9 * 2]);
    //ModelBox A7(shaderProgram, init_T[0 + 3 * 2 + 9 * 2]);
    //ModelBox A8(shaderProgram, init_T[1 + 3 * 2 + 9 * 2]);
    //ModelBox A9(shaderProgram, init_T[2 + 3 * 2 + 9 * 2]);

    //ModelBox C1(shaderProgram, TRSMatricesHolder(), init_T[getCoord(0,0,0)]);
    //ModelBox C2(shaderProgram, TRSMatricesHolder(), init_T[getCoord(1,0,0)]);
    //ModelBox C3(shaderProgram, TRSMatricesHolder(), init_T[getCoord(2,0,0)]);
    //ModelBox C4(shaderProgram, TRSMatricesHolder(), init_T[getCoord(0,1,0)]);
    //ModelBox C5(shaderProgram, TRSMatricesHolder(), init_T[getCoord(1,1,0)]);
    //ModelBox C6(shaderProgram, TRSMatricesHolder(), init_T[getCoord(2,1,0)]);
    //ModelBox C7(shaderProgram, TRSMatricesHolder(), init_T[getCoord(0,2,0)]);
    //ModelBox C8(shaderProgram, TRSMatricesHolder(), init_T[getCoord(1,2,0)]);
    //ModelBox C9(shaderProgram, TRSMatricesHolder(), init_T[getCoord(2,2,0)]);
    //
    //ModelBox B1(shaderProgram, TRSMatricesHolder(), init_T[getCoord(0,0,1)]);
    //ModelBox B2(shaderProgram, TRSMatricesHolder(), init_T[getCoord(1,0,1)]);
    //ModelBox B3(shaderProgram, TRSMatricesHolder(), init_T[getCoord(2,0,1)]);
    //ModelBox B4(shaderProgram, TRSMatricesHolder(), init_T[getCoord(0,1,1)]);
    //ModelBox B5(shaderProgram, TRSMatricesHolder(), init_T[getCoord(1,1,1)]);
    //ModelBox B6(shaderProgram, TRSMatricesHolder(), init_T[getCoord(2,1,1)]);
    //ModelBox B7(shaderProgram, TRSMatricesHolder(), init_T[getCoord(0,2,1)]);
    //ModelBox B8(shaderProgram, TRSMatricesHolder(), init_T[getCoord(1,2,1)]);
    //ModelBox B9(shaderProgram, TRSMatricesHolder(), init_T[getCoord(2,2,1)]);
    //
    //ModelBox A1(shaderProgram, TRSMatricesHolder(), init_T[getCoord(0,0,2)]);
    //ModelBox A2(shaderProgram, TRSMatricesHolder(), init_T[getCoord(1,0,2)]);
    //ModelBox A3(shaderProgram, TRSMatricesHolder(), init_T[getCoord(2,0,2)]);
    //ModelBox A4(shaderProgram, TRSMatricesHolder(), init_T[getCoord(0,1,2)]);
    //ModelBox A5(shaderProgram, TRSMatricesHolder(), init_T[getCoord(1,1,2)]);
    //ModelBox A6(shaderProgram, TRSMatricesHolder(), init_T[getCoord(2,1,2)]);
    //ModelBox A7(shaderProgram, TRSMatricesHolder(), init_T[getCoord(0,2,2)]);
    //ModelBox A8(shaderProgram, TRSMatricesHolder(), init_T[getCoord(1,2,2)]);
    //ModelBox A9(shaderProgram, TRSMatricesHolder(), init_T[getCoord(2,2,2)]);

    //vector<CharModel*> attachedToFront(0);
    //attachedToFront.push_back(&A1);
    //attachedToFront.push_back(&A2);
    //attachedToFront.push_back(&A3);
    //attachedToFront.push_back(&A4);
    //attachedToFront.push_back(&A5);
    //attachedToFront.push_back(&A6);
    //attachedToFront.push_back(&A7);
    //attachedToFront.push_back(&A8);
    //attachedToFront.push_back(&A9);
    //vector<CharModel*> attachedToBack(0);
    //attachedToBack.push_back(&C1);
    //attachedToBack.push_back(&C2);
    //attachedToBack.push_back(&C3);
    //attachedToBack.push_back(&C4);
    //attachedToBack.push_back(&C5);
    //attachedToBack.push_back(&C6);
    //attachedToBack.push_back(&C7);
    //attachedToBack.push_back(&C8);
    //attachedToBack.push_back(&C9);
    //vector<CharModel*> attachedToPort(0);
    //attachedToPort.push_back(&C3);
    //attachedToPort.push_back(&B3);
    //attachedToPort.push_back(&A3);
    //attachedToPort.push_back(&C6);
    //attachedToPort.push_back(&B6);
    //attachedToPort.push_back(&A6);
    //attachedToPort.push_back(&C9);
    //attachedToPort.push_back(&B9);
    //attachedToPort.push_back(&A9);
    //vector<CharModel*> attachedToStarboard(0);
    //attachedToStarboard.push_back(&C1);
    //attachedToStarboard.push_back(&B1);
    //attachedToStarboard.push_back(&A1);
    //attachedToStarboard.push_back(&C4);
    //attachedToStarboard.push_back(&B4);
    //attachedToStarboard.push_back(&A4);
    //attachedToStarboard.push_back(&C7);
    //attachedToStarboard.push_back(&B7);
    //attachedToStarboard.push_back(&A7);
    //vector<CharModel*> attachedToTop(0);
    //attachedToTop.push_back(&C7);
    //attachedToTop.push_back(&C8);
    //attachedToTop.push_back(&C9);
    //attachedToTop.push_back(&B7);
    //attachedToTop.push_back(&B8);
    //attachedToTop.push_back(&B9);
    //attachedToTop.push_back(&A7);
    //attachedToTop.push_back(&A8);
    //attachedToTop.push_back(&A9);
    //vector<CharModel*> attachedToBot(0);
    //attachedToBot.push_back(&C1);
    //attachedToBot.push_back(&C2);
    //attachedToBot.push_back(&C3);
    //attachedToBot.push_back(&B1);
    //attachedToBot.push_back(&B2);
    //attachedToBot.push_back(&B3);
    //attachedToBot.push_back(&A1);
    //attachedToBot.push_back(&A2);
    //attachedToBot.push_back(&A3);


    vector<CharModel*> attachedToCore(0);
    //attachedToCore.push_back(&boxFront);
    //attachedToCore.push_back(&boxBack);
    //attachedToCore.push_back(&boxTop);
    //attachedToCore.push_back(&boxBot);
    //attachedToCore.push_back(&boxPort);
    //attachedToCore.push_back(&boxStarboard);

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

    //boxFront.setAttachedModels(attachedToFront);
    //boxBack.setAttachedModels(attachedToBack);
    //boxTop.setAttachedModels(attachedToTop);
    //boxBot.setAttachedModels(attachedToBot);
    //boxPort.setAttachedModels(attachedToPort);
    //boxStarboard.setAttachedModels(attachedToStarboard);

    //internal model to track 27 box positions
    RubikCube rubik(attachedToCore);
    attachBoxToCube(attachedToRotater, rubik);

    boxCore.setAttachedModels(attachedToCore);
    boxRotater.setAttachedModels(attachedToRotater);

    const Color initialBoxColor = { 1,1,1 };
    Color selectedBoxColor = { 1,0.8,0.8 };
    //set color of boxes
    {
        //set color of all boxes
        boxCore.setAttachedColor(initialBoxColor);
        //set color of initial selection
        selectedPartModel = boxRotater.getAttachedModels()[partIndex];
        selectedPartModel->setAttachedColor(selectedBoxColor);
    }
    //CharModel::setOffset(boxTop.getAttachedModels(), rotate(mat4(1.0f), radians(-90.0f), vec3(1.0f, 0.0f, 0.0f)));
    //CharModel::setOffset(boxBot.getAttachedModels(), rotate(mat4(1.0f), radians(90.0f), vec3(1.0f, 0.0f, 0.0f)));
    //CharModel::setOffset(boxFront.getAttachedModels(), rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, -1.0f, 0.0f)));
    //CharModel::setOffset(boxBack.getAttachedModels(), rotate(mat4(1.0f), radians(0.0f), vec3(1.0f, 0.0f, 0.0f)));
    //CharModel::setOffset(boxPort.getAttachedModels(), rotate(mat4(1.0f), radians(-90.0f), vec3(0.0f, -1.0f, 0.0f)));
    //CharModel::setOffset(boxStarboard.getAttachedModels(), rotate(mat4(1.0f), radians(90.0f), vec3(0.0f, -1.0f, 0.0f)));

    //boxBot.setAttachedModels(attachedToBack);
    //workaround to get the shadow map to include the parts.
    //base
    modelsAndParts = vModels;
    //modelsAndParts.push_back(&n2a);
    //modelsAndParts.push_back(&a9a);
    //modelsAndParts.push_back(&s3a);
    for (int i = 0; i < numMainModels; i++) {
        vector<CharModel*> temp;
        vector<CharModel*>::iterator it;
        switch (i) {
        case 0:
            temp = attachedToS3;
            break;
        case 1:
            temp = attachedToN2;
            break;
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

    //float time = 0;
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    GLuint shadowWorldMatrixLocation = glGetUniformLocation(shaderProgramShadow, "model");
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
        float near_plane = -40.0f, far_plane = 35.0f;
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
        glBindTexture(GL_TEXTURE_2D, tiledTextureID); 
        //avoid peter panning
        //glCullFace(GL_FRONT);

        //put store info, so can render easier for shadow map.
        RenderInfo renderInfo;
        renderInfo.shaderProgram = shaderProgramShadow;

        renderInfo.colorLocation = colorLocation;
        renderInfo.worldMatrixLocation = shadowWorldMatrixLocation;
        renderInfo.enableTextureLocation = enableTextureLocation;
        renderInfo.enableShadowLocation = enableShadowLocation;

        renderInfo.enableTexture = enableTexture;
        renderInfo.enableShadow = enableShadow;

        renderInfo.textures.depthMap = depthMap;
        renderInfo.textures.tiledTextureID = tiledTextureID;
        renderInfo.textures.boxTextureID = boxTextureID;
        renderInfo.textures.metalTextureID = metalTextureID;

        renderInfo.cubeVAOa = cubeVAOa;
        //renderScene(shaderProgramShadow, cubeVAOa);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVAOa);
        renderDecor(renderInfo);
        renderModels(renderInfo, modelsAndParts);

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
        

        //Draw ground
        renderDecor(renderInfo);
        //draw models
        {
            //selection with keys.
            drawControl(window);
            modelIndex = selectModelControl(window, modelIndex);
            partIndex = partModelControl(window, partIndex, previousKeyStates);
            CharModel* prevModel = selectedModel;
            selectedModel = vModels[modelIndex];
            //CharModel* selectedPartModel = selectedModel->getAttachedModels()[partIndex];
            CharModel* prevSelectedPartModel = selectedPartModel;
            selectedPartModel = boxRotater.getAttachedModels()[partIndex];
            //Control model key presses.
            mat4* relativeWorldMatrix = modelControl(window, dt, previousKeyStates);
            bool hasMovement = checkModelMovement(window, previousKeyStates);
            bool* selectedSetting = customControl(window, previousKeyStates);
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

            renderModels(renderInfo, vModels);
            //update
            CharModel::update(vModels, dt);
            CharModel::update(attachedToCore, dt);
            CharModel::resetCumulativeTRS(vModels);
            TRSMatricesHolder(tempTop);
            tempTop.rotateMatrix = rotate(mat4(1.0f), radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
            //boxTop.accumulateTRS(tempTop);
            CharModel::updateAttachedCumulativeTRS(vModels);
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
            0.01f, 100.0f);                                 // near and far (near > 0)
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
}
