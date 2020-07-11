//
// COMP 371 Labs Framework
//
// Created by Nicolas Bergeron on 20/06/2019.
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


using namespace glm;
using namespace std;

const int numMainModels = 5;

class Projectile
{
public:
    Projectile(vec3 position, vec3 velocity, int shaderProgram) : mPosition(position), mVelocity(velocity)
    {
        mWorldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
        mColorLocation = glGetUniformLocation(shaderProgram, "objectColor");
    }

    void Update(float dt)
    {
        mPosition += mVelocity * dt;
    }

    void Draw() {
        // this is a bit of a shortcut, since we have a single vbo, it is already bound
        // let's just set the world matrix in the vertex shader

        mat4 worldMatrix = translate(mat4(1.0f), mPosition) * rotate(mat4(1.0f), radians(180.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.2f, 0.2f));
        glUniformMatrix4fv(mWorldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glUniform3f(mColorLocation, 0.0f, 0.0f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

private:
    GLuint mWorldMatrixLocation;
    GLuint mColorLocation;
    vec3 mPosition;
    vec3 mVelocity;
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

    mat4 getRelativeScaleMatrix() {
        return relativeScaleMatrix;
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
        return relativeTranslateMatrix * relativeRotateMatrix * relativeScaleMatrix;
    }

    virtual void draw() {
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
protected:
    void drawBorder(float modelMaxHeight, float modelMaxWidth, float boxVerticalWidth, float boxHorizontalWidth, mat4 relativeWorldMatrix = mat4(1.0f), mat4 modelPositioningMatrix = mat4(1.0f)) {
        //draw box around model(extra)
        //const float boxVerticalWidth = heightScale / 100;   //thickness of horizontal bars
        //const float boxHorizontalWidth = widthScale / 20;   //thickness of vertical bars
        //const float modelMaxHeight = m_height;              //total height of model to surround
        //const float modelMaxWidth = m_maxWidth;             //total width of model to surround
        
        
        //modelPositioningMatrix is to set center of model for the box to surround. This is also sets its orientation (default flush against xy-plane).


        for (int i = 0; i < 4; i++) {
            const bool isHorizontal = i < 2;
            const int parity = pow(-1, i + 1);

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

    GLuint worldMatrixLocation;
    GLuint colorLocation;
    mat4 initial_relativeTranslateMatrix;   //Initial translate matrix, value to take when reset.
    mat4 initial_relativeRotateMatrix;      //Initial rotate matrix, value to take when reset.
    mat4 initial_relativeScaleMatrix;       //Initial scale matrix, value to take when reset.
private:
    mat4 relativeTranslateMatrix;   //Stored translate matrix
    mat4 relativeRotateMatrix;      //Stored rotate matrix
    mat4 relativeScaleMatrix;       //Stored scale matrix
};
//example usage of model subclass.
//To convert to your model:
//- replace Model_template with your model name.
//- set initial position/orientation/size in the world where you want the model to be with initial_relative[...]Matrix.
//- replace the drawModel function your draw function.
//- done!
//class Model_template : public CharModel {
//public:
//
//    Model_template(int shaderProgram) : CharModel(shaderProgram) {
//        //initialize initial_relativeTranslateMatrix here and set relativeTranslateMatrix to that value.
//        //example here has position set to (0, 0, -20).
//        initial_relativeTranslateMatrix =
//            translate(mat4(1.0f),
//                vec3(0,
//                    0,
//                    -20));
//
//        setRelativeTranslateMatrix(initial_relativeTranslateMatrix);
//    }
//
//    //no need to change anything here, except drawModel's name if you feel like it.
//    void draw() {
//        //pass arguments stored in parent class.
//        drawModel(worldMatrixLocation, colorLocation, getRelativeWorldMatrix());
//    }
//
//private:
//    void drawModel(GLuint worldMatrixLocation, GLuint colorLocation, mat4 relativeWorldMatrix) {
//        //code goes here
//
//        //pattern to draw models to make use of relativeWorldMatrix:
//        //worldMatrix = relativeWorldMatrix * (model's original matrices);
//        //glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
//        //glUniform3f(colorLocation, 1.0f, 233.0f / 255.0f, 0.0f);
//        //glDrawArrays(GL_TRIANGLES, 0, 36);
//    }
//};

class ModelV9 : public CharModel {
public:
	//Constructor
	ModelV9(int shaderProgram) : CharModel(shaderProgram) {
		const float model_heightScale = 5.0f; //<-make sure is same as constant in drawV9().
		//initialize position with translate matrix
		initial_relativeTranslateMatrix =
			translate(mat4(1.0f),
				vec3(45.5,
					model_heightScale / 2,
					45.5));

		setRelativeTranslateMatrix(initial_relativeTranslateMatrix);

		////
		//initialize orientation with rotate matrix
		initial_relativeRotateMatrix = mat4(1.0f);
		setRelativeRotateMatrix(initial_relativeRotateMatrix);
		//initialize size with scale matrix
		initial_relativeScaleMatrix = mat4(1.0f);
		setRelativeScaleMatrix(initial_relativeScaleMatrix);
	}

	//override draw method.
	void draw() {
		//pass arguments stored in parent class.
		drawV9(worldMatrixLocation, colorLocation, getRelativeWorldMatrix());
	}

private:
	void drawV9(GLuint worldMatrixLocation, GLuint colorLocation, mat4 relativeWorldMatrix) {

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
					0,
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


		//Draw 9.
		//total height is heightScale
		//total width is 2 * apothem

		//Offset for 9 to center the model.
		modelPositioningMatrix =
			translate(mat4(1.0f),
				vec3(letterHalfWidth,
					-(m_height / 2),
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
		{
			//draw box around model(extra)
			const float boxVerticalWidth = heightScale / 100;   //thickness of horizontal bars
			const float boxHorizontalWidth = widthScale / 20;   //thickness of vertical bars
			const float modelMaxHeight = m_height + boxVerticalWidth / 2;
			const float modelMaxWidth = m_maxWidth + boxHorizontalWidth / 2;
			drawBorder(modelMaxHeight, modelMaxWidth, boxVerticalWidth, boxHorizontalWidth, relativeWorldMatrix);
		}

	}
};
class ModelS3 : public CharModel {
public:

	ModelS3(int shaderProgram) : CharModel(shaderProgram) {
		//initialize initial_relativeTranslateMatrix here and set relativeTranslateMatrix to that value.
		initial_relativeTranslateMatrix =
			translate(mat4(1.0f),
				vec3(0,
					-2,
					0));

		setRelativeTranslateMatrix(initial_relativeTranslateMatrix);
	}

	//no need to change anything here, except drawModel's name if you feel like it.
	void draw() {
		//pass arguments stored in parent class.
		drawModel(worldMatrixLocation, colorLocation, getRelativeWorldMatrix());
	}

private:
	void drawModel(GLuint worldMatrixLocation, GLuint colorLocation, mat4 relativeWorldMatrix) {
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
		initial_relativeTranslateMatrix =
			translate(mat4(1.0f),
				vec3(-44.5,
					0,
					-44.5));

		setRelativeTranslateMatrix(initial_relativeTranslateMatrix);
	}

	//no need to change anything here, except drawModel's name if you feel like it.
	void draw() {
		//pass arguments stored in parent class.
		drawModel(worldMatrixLocation, colorLocation, getRelativeWorldMatrix());
	}

private:
	void drawModel(GLuint worldMatrixLocation, GLuint colorLocation, mat4 relativeWorldMatrix) {
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
		initial_relativeTranslateMatrix =
			translate(mat4(1.0f),
				vec3(44,
					0,
					-45));

		setRelativeTranslateMatrix(initial_relativeTranslateMatrix);
	}

	//no need to change anything here, except drawModel's name if you feel like it.
	void draw() {
		//pass arguments stored in parent class.
		drawModel(worldMatrixLocation, colorLocation, getRelativeWorldMatrix());
	}

private:
	void drawModel(GLuint worldMatrixLocation, GLuint colorLocation, mat4 relativeWorldMatrix) {
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
		initial_relativeTranslateMatrix =
			translate(mat4(1.0f),
				vec3(-45,
					0,
					45));

		setRelativeTranslateMatrix(initial_relativeTranslateMatrix);
	}

	//no need to change anything here, except drawModel's name if you feel like it.
	void draw() {
		//pass arguments stored in parent class.
		drawModel(worldMatrixLocation, colorLocation, getRelativeWorldMatrix());
	}

private:
	void drawModel(GLuint worldMatrixLocation, GLuint colorLocation, mat4 relativeWorldMatrix) {
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

const char* getVertexShaderSource()
{
    // For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
    return
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;"
        "layout (location = 1) in vec3 aColor;"
        ""
        "uniform mat4 worldMatrix;"
        "uniform mat4 viewMatrix = mat4(1.0);"  // default value for view matrix (identity)
        "uniform mat4 projectionMatrix = mat4(1.0);"
        ""
        "out vec3 vertexColor;"
        "void main()"
        "{"
        "   vertexColor = aColor;"
        "   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
        "   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
        "}";
}
const char* getFragmentShaderSource()
{
    return
        "#version 330 core\n"
        "uniform vec3 objectColor;"
        "in vec3 vertexColor;"
        "out vec4 FragColor;"
        "void main()"
        "{"
        //"   FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);"
        "   FragColor = vec4(objectColor.r, objectColor.g, objectColor.b, 1.0f);"
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


int createVertexBufferObject()
{
    // Cube model (used for models and axis)
    vec3 vertexArray[] = {  // position,                            color
        vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), //left - red
        vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),

        vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),

        vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), // far - blue
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),

        vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),

        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), // bottom - turquoise
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f),
        vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f),

        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f),
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f),

        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), // near - green
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
        vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), // right - purple
        vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f),
        vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f),

        vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f),
        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f),
        vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), // top - yellow
        vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f),

        vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f),

        //line (0,0,-0.5)to(0,0,0.5)
        vec3(0.0f, 0.0f, -0.5f), vec3(1.0f, 1.0f, 1.0f),
        vec3(0.0f, 0.0f, 0.5f), vec3(1.0f, 1.0f, 1.0f),
    };


    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);


    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        2 * sizeof(vec3), // stride - each vertex contain 2 vec3 (position, color)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(vec3),
        (void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);


    return vertexBufferObject;
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

    const float height = 0;     //y-position of grid.

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

//function will switch the selected draw mode on correct key presses.
//note: undefined priority in case multiple correct keys are pressed (but will select a draw mode).
void drawControl(GLFWwindow* window) {
    map<int, GLenum> inputsToDrawModes;
    map<int, GLenum>::iterator itr;
    inputsToDrawModes.insert(pair<int, GLenum>(GLFW_KEY_P, GL_POINT));
    inputsToDrawModes.insert(pair<int, GLenum>(GLFW_KEY_L, GL_LINE));
    inputsToDrawModes.insert(pair<int, GLenum>(GLFW_KEY_T, GL_FILL));

    //default return value;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) // capital letters only
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
mat4* modelControl(GLFWwindow* window, map<int, int> previousKeyStates) {
    mat4* selectedTransformation = new mat4[4];
    //default return values
    selectedTransformation[0] = mat4(1.0f);     //translate
    selectedTransformation[1] = mat4(1.0f);     //rotate
    selectedTransformation[2] = mat4(1.0f);     //scale

    selectedTransformation[3] = mat4(1.0f);     //signal to reset position and orientation if changed.

    //allows map value to contain 2 variables.
    struct transformation {
        mat4 matrix;
        int type;
    };
    map<int, transformation> inputsToModelMatrix;
    map<int, transformation>::iterator itr;
    float transformSpeed = 0.2f;
    float translateSpeed = transformSpeed;
    float rotateSpeed = 5.0f;   //specifications
    float scaleSpeed = transformSpeed / 4;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) // capital case letters
    {
        //translate model if pressed
        //vertical movement
        inputsToModelMatrix.insert(pair<int, transformation>(GLFW_KEY_W, {
            translate(mat4(1.0f),
                vec3(0,
                    0,
                    -translateSpeed)), 0 }));
        inputsToModelMatrix.insert(pair<int, transformation>(GLFW_KEY_S, {
            translate(mat4(1.0f),
                vec3(0,
                    0,
                    translateSpeed)), 0 }));
        //horizontal movement
        inputsToModelMatrix.insert(pair<int, transformation>(GLFW_KEY_D, {
            translate(mat4(1.0f),
                vec3(translateSpeed,
                    0,
                    0)), 0 }));
        inputsToModelMatrix.insert(pair<int, transformation>(GLFW_KEY_A, {
            translate(mat4(1.0f),
                vec3(-translateSpeed,
                    0,
                    0)), 0 }));

        //scale model if pressed
        inputsToModelMatrix.insert(pair<int, transformation>(GLFW_KEY_U, {
            scale(mat4(1.0f),
                vec3((1 + scaleSpeed), (1 + scaleSpeed), (1 + scaleSpeed))), 2 }));
        inputsToModelMatrix.insert(pair<int, transformation>(GLFW_KEY_J, {
            scale(mat4(1.0f),
                vec3(1.0f / (1 + scaleSpeed), 1.0f / (1 + scaleSpeed), 1.0f / (1 + scaleSpeed))), 2 }));
    }
    else {
        //rotate model if pressed
        inputsToModelMatrix.insert(pair<int, transformation>(GLFW_KEY_A, {
            rotate(mat4(1.0f),
                radians(rotateSpeed),
                vec3(0.0f, 1.0f, 0.0f)), 1 }));
        inputsToModelMatrix.insert(pair<int, transformation>(GLFW_KEY_D, {
            rotate(mat4(1.0f),
                radians(rotateSpeed),
                vec3(0.0f, -1.0f, 0.0f)), 1 }));
    }
    //reset position and orientation if pressed
    inputsToModelMatrix.insert(pair<int, transformation>(GLFW_KEY_HOME, {
        mat4(2.0f), 3 }));

    //iterate through all pressed keys.
    for (itr = inputsToModelMatrix.begin(); itr != inputsToModelMatrix.end(); ++itr) {
        //get previous key state if tracked, otherwise default release (true).
        //https://stackoverflow.com/questions/4527686/how-to-update-stdmap-after-using-the-find-method
        int previousState = GLFW_RELEASE;
        map<int, int>::iterator it = previousKeyStates.find(itr->first);
        if (it != previousKeyStates.end()) {
            previousState = it->second;
        }

        if (glfwGetKey(window, itr->first) == GLFW_PRESS && previousState == GLFW_RELEASE) // select model. Apply once for keys that are tracked.
        {
            if (itr->second.type == 3) {
                //signal to reset position and orientation.
                selectedTransformation[3] = mat4(2.0f);
            }
            else {
                selectedTransformation[itr->second.type] = itr->second.matrix * selectedTransformation[itr->second.type];
            }
        }
    }
    return selectedTransformation;
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
    // On windows, we set OpenGL version to 2.1, to support more hardware
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif

    // Create Window and rendering context using GLFW, resolution is 800x600
    GLFWwindow* window = glfwCreateWindow(1024, 768, "COMP 371 - A1 - Team 4", NULL, NULL);
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

    // Grey background
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    // Compile and link shaders here ...
    int shaderProgram = compileAndLinkShaders();

    // We can set the shader once, since we have only one
    glUseProgram(shaderProgram);

    // Used to overwrite color in the fragment shader
    GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");

    // Camera parameters for view transform
    const float initial_xpos = 0;
    const float initial_ypos = 5;
    const float initial_zpos = 20;
    const float initial_rpos = sqrt(powf(initial_xpos, 2) + powf(initial_zpos, 2));
    const vec3 initial_cameraPosition(initial_xpos, initial_ypos, initial_zpos);
    //const vec3 initial_cameraLookAt(0.0f, 0.0f, -1.0f);   //<-overridden by camera Horizontal/Vertical Angle anyways.
    const vec3 initial_cameraUp(0.0f, 1.0f, 0.0f);
    // Camera parameters for view transform
    vec3 cameraPosition = initial_cameraPosition;
    vec3 cameraLookAt /*= initial_cameraLookAt*/;
    vec3 cameraUp = initial_cameraUp;


    // Other camera parameters
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
    mat4 projectionMatrix = glm::perspective(radians(initialFoV),            // field of view in degrees
        800.0f / 600.0f,  // aspect ratio
        0.01f, 100.0f);   // near and far (near > 0)

//glm::mat4 projectionMatrix = glm::ortho(-4.0f, 4.0f,    // left/right
//    -3.0f, 3.0f,    // bottom/top
//    -100.0f, 100.0f);  // near/far (near == 0 is ok for ortho)

    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

    // Set initial view matrix
    //mat4 viewMatrix = lookAt(cameraPosition,  // eye
    //                         cameraPosition + cameraLookAt,  // center
    //                         cameraUp ); // up
    GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
    //glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);


    // Define and upload geometry to the GPU here ...
    int vbo = createVertexBufferObject();

    // For frame time
    float lastFrameTime = glfwGetTime();

    //Previous key states to track
    //int lastMouseLeftState = GLFW_RELEASE;
    map<int, int> previousKeyStates;
    previousKeyStates.insert(pair<int, int>(GLFW_KEY_A, GLFW_RELEASE));
    previousKeyStates.insert(pair<int, int>(GLFW_KEY_D, GLFW_RELEASE));
    previousKeyStates.insert(pair<int, int>(GLFW_KEY_U, GLFW_RELEASE));
    previousKeyStates.insert(pair<int, int>(GLFW_KEY_J, GLFW_RELEASE));

    double lastMousePosX, lastMousePosY;
    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

    // Other OpenGL states to set once
    // Enable Backface culling
    glEnable(GL_CULL_FACE);

    // @TODO 1 - Enable Depth Test
    // ...
    glEnable(GL_DEPTH_TEST);

    // Container for projectiles to be implemented in tutorial
    list<Projectile> projectileList;

    //Models
    CharModel* selectedModel;
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
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // Draw ground
        GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");


        //draw grid
        glUniform3f(colorLocation, 1.0f, 1.0f, 1.0f);
        drawGrid(worldMatrixLocation, colorLocation, mat4(1.0f));
        
        // draw axis
        drawAxis(worldMatrixLocation, colorLocation);

        //draw models
        {
            //selection with keys.
            drawControl(window);
            modelIndex = selectModelControl(window, modelIndex);
            selectedModel = models[modelIndex];
            //Control model key presses.
            mat4* relativeWorldMatrix = modelControl(window, previousKeyStates);

            //Home key has been pressed, so reset world matrices.
            if (relativeWorldMatrix[3] != mat4(1.0f)) {
                selectedModel->resetInitialRelativeMatrices();
                //reset camera too.
                cameraPosition = initial_cameraPosition;
                //cameraLookAt = initial_cameraLookAt;
                cameraHorizontalAngle = initial_cameraHorizontalAngle;
                cameraVerticalAngle = -initial_cameraVerticalAngle;
                cameraUp = initial_cameraUp;
                foV = initialFoV;
            }
            //Adjust selected model accordingly.
            selectedModel->addRelativeWorldMatrix(relativeWorldMatrix[0], relativeWorldMatrix[1], relativeWorldMatrix[2]);

            //draw all models
            glUniform3f(colorLocation, 1.0f, 233.0f / 255.0f, 0.0f);
            CharModel::draw(models);
        }

        // @TODO 3 - Update and draw projectiles
        // ...

        for (list<Projectile>::iterator it = projectileList.begin(); it != projectileList.end(); it++) {
            it->Update(dt);
            it->Draw();
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
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // End Frame
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Handle inputs
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        /*
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) // move camera down
        {
            cameraFirstPerson = true;
        }

        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) // move camera down
        {
            cameraFirstPerson = false;
        }
        */


        // This was solution for Lab02 - Moving camera exercise
        // We'll change this to be a first or third person camera
        bool fastCam = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
        float currentCameraSpeed = (fastCam) ? cameraFastSpeed : cameraSpeed;


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
        
        // Limit FoV between 1.0 deg and 120.0 deg
        if (foV < 1.0f)
            foV = 1.0f;
        else if (foV > 120.0f)
            foV = 120.0f;
        
        // Recompute projection matrix depending on FoV
        projectionMatrix = glm::perspective(radians(foV),            // field of view in degrees
        800.0f / 600.0f,  // aspect ratio
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

        // @TODO 2 - Shoot Projectiles
        //
        // shoot projectiles on mouse left click
        // To detect onPress events, we need to check the last state and the current state to detect the state change
        // Otherwise, you would shoot many projectiles on each mouse press
        // ...
        /*
        if (lastMouseLeftState == GLFW_RELEASE && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            const float projectileSpeed = 25.0f;
            projectileList.push_back(Projectile(cameraPosition, projectileSpeed * cameraLookAt, shaderProgram));

            //glClearColor(0.5f, 0.5f, 0.0f, 1.0f);
        }
        lastMouseLeftState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        */
        
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

        //update previous key states
        map<int, int>::iterator itr;
        for (itr = previousKeyStates.begin(); itr != previousKeyStates.end(); ++itr) {
            int currentState = glfwGetKey(window, itr->first);
            itr->second = currentState;
        }
    }


    // Shutdown GLFW
    glfwTerminate();

    return 0;
}
