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


using namespace glm;
using namespace std;


class CharModel {
public:
	CharModel(int shaderProgram) {
		worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
		colorLocation = glGetUniformLocation(shaderProgram, "objectColor");
		moveByX = 0.0f;
		rotateAroundY = 0.0f;
		moveByZ = 0.0f;
		scaler = 1.0f;
		
	}

	float getScaler() {
		return scaler;
	}

	void setScaler(float value) {
		scaler = value;
	}

	// Member Increments
	void incMoveByX(float value) {
		moveByX += value;
	}

	void incMoveByZ(float value) {
		moveByZ += value;
	}

	void incScaler(float value) {
		scaler += value;
	}


	void incrotateAroundY(float value) {
		rotateAroundY += value;
	}


	// Draws Model depending on specified type. Necessary x and z displacement and scaling are calculated
	void drawCharModel(int type) {
		const float height = 1.0f;
		mat4 worldMatrix;
		glUniform3f(colorLocation, 1.0f, 233.0f / 255.0f, 0.0f);
		//vec3 y_axis = vec3(0.0f, 1.0f, 0.0f);

		// Draw S3 at origin
		if (type == 0) {
			//const float offsetX = 0.0f, offsetZ = 0.0f;
			mat4 scalingMatrix, translationMatrix;

			

			// Draw 3

			

			// Top
			scalingMatrix = scale(mat4(1.0f), vec3(scaler * 2.0f, scaler * 1.0f, scaler * 1.0f));
			translationMatrix = translate(mat4(1.0f), vec3(moveByX + scaler * 2.0f, scaler * 7.5f, moveByZ + scaler * 0.0f));
			glm::mat4 worldMatrix = translationMatrix * rotate(glm::mat4(1.0f), radians(rotateAroundY), vec3(0.0f, 1.0f, 0.0f)) * scalingMatrix;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36); // 3 vertices, starting at index 0

			//middle row
			translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(moveByX + scaler * 2.0f, scaler * 5.0f, moveByZ + scaler * 0.0f));
			worldMatrix = translationMatrix * rotate(glm::mat4(1.0f), radians(rotateAroundY), vec3(0.0f, 1.0f, 0.0f)) * scalingMatrix;
			
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

			//bottom row
			translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(moveByX + scaler * 2.0f, scaler * 2.5f, moveByZ + scaler * 0.0f));
			worldMatrix = translationMatrix * rotate(glm::mat4(1.0f), radians(rotateAroundY), vec3(0.0f, 1.0f, 0.0f)) * scalingMatrix;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			//right side
			scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scaler * 1.0f, scaler * 6.0f, scaler * 1.0f));
			translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(moveByX + scaler * 3.5f, scaler * 5.0f, moveByZ + scaler * 0.0f));
			worldMatrix = translationMatrix * rotate(glm::mat4(1.0f), radians(rotateAroundY), vec3(0.0f, 1.0f, 0.0f)) * scalingMatrix;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			//Draw S
			scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scaler * 1.0f, scaler * 1.5f, scaler * 1.0f));
			translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(moveByX + scaler * -1.5f, scaler * 7.25f, moveByZ + scaler * 0.0f));
			worldMatrix = translationMatrix * rotate(glm::mat4(1.0f), radians(rotateAroundY), vec3(0.0f, 1.0f, 0.0f)) * scalingMatrix;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

			scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scaler * 1.0f, scaler *  1.0f, scaler * 1.0f));
			translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(moveByX + scaler * -2.5f, scaler * 7.5f, moveByZ + scaler * 0.0f));
			worldMatrix = translationMatrix * rotate(glm::mat4(1.0f), radians(rotateAroundY), vec3(0.0f, 1.0f, 0.0f)) * scalingMatrix;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

			scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scaler * 1.0f, scaler *  3.5f, scaler * 1.0f));
			translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(moveByX + scaler * -3.5f, scaler *  6.25f, moveByZ + scaler * 0.0f));
			worldMatrix = translationMatrix * rotate(glm::mat4(1.0f), radians(rotateAroundY), vec3(0.0f, 1.0f, 0.0f)) * scalingMatrix;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

			scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scaler * 1.0f, scaler * 1.0f, scaler * 1.0f));
			translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(moveByX + scaler * -2.5f, scaler * 5.0f, moveByZ + scaler * 0.0f));
			worldMatrix = translationMatrix * rotate(glm::mat4(1.0f), radians(rotateAroundY), vec3(0.0f, 1.0f, 0.0f)) * scalingMatrix;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

			scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scaler * 1.0f, scaler * 3.5f, scaler * 1.0f));
			translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(moveByX + scaler * -1.5f, scaler *  3.75f, moveByZ + scaler * 0.0f));
			worldMatrix = translationMatrix * rotate(glm::mat4(1.0f), radians(rotateAroundY), vec3(0.0f, 1.0f, 0.0f)) * scalingMatrix;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

			scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scaler * 1.0f, scaler * 1.0f, scaler * 1.0f));
			translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(moveByX + scaler * -2.5f, scaler *  2.5f, moveByZ + scaler * 0.0f));
			worldMatrix = translationMatrix * rotate(glm::mat4(1.0f), radians(rotateAroundY), vec3(0.0f, 1.0f, 0.0f)) * scalingMatrix;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

			scalingMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scaler * 1.0f, scaler * 1.5f, scaler * 1.0f));
			translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(moveByX + scaler * -3.5f, scaler * 2.75f, moveByZ + scaler * 0.0f));
			worldMatrix = translationMatrix * rotate(glm::mat4(1.0f), radians(rotateAroundY), vec3(0.0f, 1.0f, 0.0f)) * scalingMatrix;
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 12 * 3);




		}
		// Draw A9 (Gia-Khang Ernest Nguyen) centered at (-30.0f, -30.0f)
		else if (type == 1) {
			const float offsetX = -15.0f, offsetZ = -15.0f;
			// Draw A
			// Right Vertical Bar
			worldMatrix = translate(mat4(1.0f), vec3(-1.0f * scaler + offsetX + moveByX * scaler, height + 2.0f * scaler, offsetZ + moveByZ * scaler)) * scale(mat4(1.0f), vec3(scaler, scaler * 4.0f, scaler));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// Left Vertical Bar
			worldMatrix = translate(mat4(1.0f), vec3(-5.0f * scaler + offsetX + moveByX * scaler, height + 2.0f * scaler, offsetZ + moveByZ * scaler)) * scale(mat4(1.0f), vec3(scaler, scaler * 4.0f, scaler));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// Top Horizontal Bar
			worldMatrix = translate(mat4(1.0f), vec3(-3.0f * scaler + offsetX + moveByX * scaler, height + 4.5f * scaler, offsetZ + moveByZ * scaler)) * scale(mat4(1.0f), vec3(scaler * 3.5f, scaler, scaler));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// Bottom Horizontal Bar
			worldMatrix = translate(mat4(1.0f), vec3(-3.0f * scaler + offsetX + moveByX * scaler, height + 2.5f * scaler, offsetZ + moveByZ * scaler)) * scale(mat4(1.0f), vec3(scaler * 3.5f, scaler, scaler));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// Draw 9
			// Long Vertical Bar
			worldMatrix = translate(mat4(1.0f), vec3(5.0f * scaler + offsetX + moveByX * scaler, height + 2.5f * scaler, offsetZ + moveByZ * scaler)) * scale(mat4(1.0f), vec3(scaler, scaler * 5.0f, scaler));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// Short Vertical Bar
			worldMatrix = translate(mat4(1.0f), vec3(1.0f * scaler + offsetX + moveByX * scaler, height + 3.5f * scaler, offsetZ + moveByZ * scaler)) * scale(mat4(1.0f), vec3(scaler, scaler * 3.0f, scaler));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// Top Horizontal Bar
			worldMatrix = translate(mat4(1.0f), vec3(3.0f * scaler + offsetX + moveByX * scaler, height + 4.5f * scaler, offsetZ + moveByZ * scaler)) * scale(mat4(1.0f), vec3(scaler * 3.0f, scaler, scaler));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// Bottom Horizontal Bar
			worldMatrix = translate(mat4(1.0f), vec3(3.0f * scaler + offsetX + moveByX * scaler, height + 2.5f * scaler, offsetZ + moveByZ * scaler)) * scale(mat4(1.0f), vec3(scaler * 3.0f, scaler, scaler));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		// Draw N2 (Manuel Romero) centered at (30.0, -30.0f)
		else if (type == 2) {
			const float offsetX = 15.0f, offsetZ = -15.0f;

			// Draw N
			// Right Vertical Bar
			worldMatrix = translate(mat4(1.0f), vec3(-0.5f * scaler + offsetX + moveByX * scaler, height + 2.5f * scaler, 0.5 + offsetZ + moveByZ * scaler)) * scale(mat4(1.0f), vec3(scaler, scaler * 5.0f, scaler));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// Left Vertical Bar
			worldMatrix = translate(mat4(1.0f), vec3(-4.5f * scaler + offsetX + moveByX * scaler, height + 2.5f * scaler, 0.5 + offsetZ + moveByZ * scaler)) * scale(mat4(1.0f), vec3(scaler, scaler * 5.0f, scaler));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// Small bar 1
			worldMatrix = translate(mat4(1.0f), vec3(-3.5f * scaler + offsetX + moveByX * scaler, height + 3.5f * scaler, 0.5 + offsetZ + moveByZ * scaler)) * scale(mat4(1.0f), vec3(scaler, scaler*1.5f, scaler));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// Small bar 2
			worldMatrix = translate(mat4(1.0f), vec3(-2.5f * scaler + offsetX + moveByX * scaler, height + 2.5f * scaler, 0.5 + offsetZ + moveByZ * scaler)) * scale(mat4(1.0f), vec3(scaler, scaler*2.0f, scaler));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// Small bar 3
			worldMatrix = translate(mat4(1.0f), vec3(-1.5f * scaler + offsetX + moveByX * scaler, height + 1.5f * scaler, 0.5 + offsetZ + moveByZ * scaler)) * scale(mat4(1.0f), vec3(scaler, scaler*1.5f, scaler));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// Draw 2
			// Right Vertical Bar
			worldMatrix = translate(mat4(1.0f), vec3(5.5f * scaler + offsetX + moveByX * scaler, height + 3.5f * scaler, 0.5 + offsetZ + moveByZ * scaler)) * scale(mat4(1.0f), vec3(scaler, scaler * 2.5f, scaler));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// Left Vertical Bar 1
			worldMatrix = translate(mat4(1.0f), vec3(1.5f * scaler + offsetX + moveByX * scaler, height + 1.5f * scaler, 0.5 + offsetZ + moveByZ * scaler)) * scale(mat4(1.0f), vec3(scaler, scaler * 1.5f, scaler));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// Top Horizontal Bar
			worldMatrix = translate(mat4(1.0f), vec3(3.5f * scaler + offsetX + moveByX * scaler, height + 4.5f * scaler, 0.5 + offsetZ + moveByZ * scaler)) * scale(mat4(1.0f), vec3(scaler * 4.5f, scaler, scaler));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// Middle Horizontal Bar
			worldMatrix = translate(mat4(1.0f), vec3(3.5f * scaler + offsetX + moveByX * scaler, height + 2.5f * scaler, 0.5 + offsetZ + moveByZ * scaler)) * scale(mat4(1.0f), vec3(scaler * 4.0f, scaler, scaler));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// Bottom Horizontal Bar
			worldMatrix = translate(mat4(1.0f), vec3(3.5f * scaler + offsetX + moveByX * scaler, height + 0.5 * scaler, 0.5 + offsetZ + moveByZ * scaler)) * scale(mat4(1.0f), vec3(scaler * 5.0f, scaler, scaler));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);

		}

		//Draw N4
		else if (type == 3) {
			const float offsetX = -15.0f, offsetZ = 15.0f;

			// Draw N
			// Right Vertical Bar
			worldMatrix = translate(mat4(1.0f), vec3(-0.5f * scaler + offsetX + moveByX * scaler, height + 2.5f * scaler, 0.5 + offsetZ + moveByZ * scaler)) * scale(mat4(1.0f), vec3(scaler, scaler * 5.0f, scaler));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glUniform3f(colorLocation, 1.0f, 233.0f / 255.0f, 0.0f);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// Left Vertical Bar
			worldMatrix = translate(mat4(1.0f), vec3(-4.5f * scaler + offsetX + moveByX * scaler, height + 2.5f * scaler, 0.5 + offsetZ + moveByZ * scaler)) * scale(mat4(1.0f), vec3(scaler, scaler * 5.0f, scaler));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);


			// Diagonal bar
			worldMatrix = rotate(translate(mat4(1.0f), vec3(-2.5f * scaler + offsetX + moveByX * scaler, height + 2.5f * scaler, 0.5 + offsetZ + moveByZ * scaler)), radians(41.0f), vec3(0, 0, 1)) * scale(mat4(1.0f), vec3(scaler, scaler*5.75f, scaler));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// Draw 4
			// Right Vertical Bar
			worldMatrix = translate(mat4(1.0f), vec3(4.5f * scaler + offsetX + moveByX * scaler, height + 2.5f * scaler, 0.5 + offsetZ + moveByZ * scaler)) * scale(mat4(1.0f), vec3(scaler, scaler * 5.0f, scaler));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// Left Small Vertical Bar 
			worldMatrix = translate(mat4(1.0f), vec3(1.5f * scaler + offsetX + moveByX * scaler, height + 4.0f * scaler, 0.5 + offsetZ + moveByZ * scaler)) * scale(mat4(1.0f), vec3(scaler, scaler * 2.0f, scaler));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			// Middle Horizontal Bar
			worldMatrix = translate(mat4(1.0f), vec3(3.0f * scaler + offsetX + moveByX * scaler, height + 2.5f * scaler, 0.5 + offsetZ + moveByZ * scaler)) * scale(mat4(1.0f), vec3(scaler * 4.0f, scaler, scaler));
			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glDrawArrays(GL_TRIANGLES, 0, 36);

		}

	}

	// Resets position and scaling of the model
	void reset() {
		moveByX = 0.0f;
		moveByZ = 0.0f;
		scaler = 1.0f;
	}

private:
	GLuint worldMatrixLocation;
	GLuint colorLocation;
	float moveByX;
	float moveByZ;
	float scaler;
	float rotateAroundY;
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
//Input colorLocation is location of color (for fragment shader)
//
//Function will create a grid centered at (0,Y,0) in (X,Y,Z).
//Note: location of line object inside vertex array is hard-coded.
void drawGrid(GLuint worldMatrixLocation, GLuint colorLocation, float sideLength = 100, float cellLength = 1) {
	//const float sideLength = 100; //# of cells on side.
	//const float cellLength = 1; //length of side of a cell.

	const float height = 1;     //y-position of grid.

	mat4 lineWorldMatrix;
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
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &lineWorldMatrix[0][0]);
		glUniform3f(colorLocation, 1.0f, 1.0f, 1.0f);
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
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &lineWorldMatrix[0][0]);
		glUniform3f(colorLocation, 1.0f, 1.0f, 1.0f);
		glDrawArrays(GL_LINES, 36, 2);
	}
}

// Draws x, y, z axis at the center of the grid
// Input is location of worldMatrix and location of colorLocation
void drawAxis(GLuint worldMatrixLocation, GLuint colorLocation) {
	const float height = 1, cellLength = 1; // values from drawGrid
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

////not done
//void cameraControls(GLFWwindow* window, vec3* cameraPosition, vec3 cameraSideVector, float currentCameraSpeed, vec3* cameraLookAt, float* cameraHorizontalAngle, float* cameraVerticalAngle, float dt) {
//    // Camera parameters for view transform
//    const vec3 initial_cameraPosition(0.0f, 20.0f, 20.0f);
//    const vec3 initial_cameraLookAt(0.0f, 0.0f, -1.0f);
//    const vec3 initial_cameraUp(0.0f, 1.0f, 0.0f);
//
//    
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // move camera to the left
//    {
//        *cameraPosition -= cameraSideVector * currentCameraSpeed * dt;
//    }
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // move camera to the right
//    {
//        *cameraPosition += cameraSideVector * currentCameraSpeed * dt;
//    }
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // move camera up
//    {
//        *cameraPosition -= *cameraLookAt * currentCameraSpeed * dt;
//    }
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // move camera down
//    {
//        *cameraPosition += *cameraLookAt * currentCameraSpeed * dt;
//    }
//
//    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // pitch camera up
//    {
//        *cameraVerticalAngle += currentCameraSpeed * dt;
//    }
//    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) // pitch camera down
//    {
//        *cameraVerticalAngle -= currentCameraSpeed * dt;
//    }
//    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // yaw camera right
//    {
//        *cameraVerticalAngle += currentCameraSpeed * dt;
//    }
//    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // yaw camera left
//    {
//        *cameraVerticalAngle -= currentCameraSpeed * dt;
//    }
//
//    if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) // reset camera
//    {
//        *cameraPosition = initial_cameraPosition;
//        *cameraLookAt = initial_cameraLookAt;
//        *cameraUp = initial_cameraUp;
//    }
//}
int main(int argc, char*argv[])
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
	vec3 cameraPosition(0.0f, 10.0f, 50.0f);
	vec3 cameraLookAt(0.0f, 0.0f, 0.0f);
	vec3 cameraUp(0.0f, 1.0f, 0.0f);

	// Other camera parameters
	float cameraSpeed = 10.0f;
	float cameraFastSpeed = 2 * cameraSpeed;
	float cameraHorizontalAngle = 90.0f;
	float cameraVerticalAngle = 0.0f;
	bool  cameraFirstPerson = true; // press 1 or 2 to toggle this variable

	// Spinning cube at camera position
	float spinningCubeAngle = 0.0f;

	// Set projection matrix for shader, this won't change
	mat4 projectionMatrix = glm::perspective(70.0f,            // field of view in degrees
		800.0f / 600.0f,  // aspect ratio
		0.01f, 100.0f);   // near and far (near > 0)

	GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

	// Set initial view matrix
	mat4 viewMatrix = lookAt(cameraPosition,  // eye
		cameraPosition + cameraLookAt,  // center
		cameraUp); // up

	GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);



	// Define and upload geometry to the GPU here ...
	int vbo = createVertexBufferObject();

	// For frame time
	float lastFrameTime = glfwGetTime();
	int lastMouseLeftState = GLFW_RELEASE;
	double lastMousePosX, lastMousePosY;
	glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

	// Other OpenGL states to set once
	// Enable Backface culling
	glEnable(GL_CULL_FACE);

	// @TODO 1 - Enable Depth Test
	// ...
	glEnable(GL_DEPTH_TEST);

	// Container for projectiles to be implemented in tutorial
	//list<Projectile> projectileList;

	/*
	float modelMovementX = 0.0f; // 25.0f
	float modelMovementZ = 0.0f; // -25.0f
	float scaleFactor = 1.0f;
	*/

	// declaring model array and current model pointer
	const int nbOfModels = 4;
	CharModel models[nbOfModels] = { CharModel(shaderProgram), CharModel(shaderProgram), CharModel(shaderProgram), CharModel(shaderProgram) };
	CharModel * currentModel = &models[0];

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
		//glClear(GL_COLOR_BUFFER_BIT);


		// Draw geometry
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		//JASON BECCHERINI
		GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

		//float modelMoveSpeed = 25.0f;

		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		{
			//modelMovementX -= modelMoveSpeed;
			//scaleFactor = 1.5f;
			/*
			if (scaleFactor < 0.1f)
				scaleFactor = 0.1f;
			else
				scaleFactor -= 0.01f;
			 */
			if (currentModel->getScaler() < 0.1f)
				currentModel->setScaler(0.1f);
			else
				currentModel->incScaler(-0.01f);
		}
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
			//scaleFactor += 0.01f;
			currentModel->incScaler(0.01f);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			//modelMovementZ -= 0.01f;
			currentModel->incMoveByZ(-0.01f);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			//modelMovementZ += 0.01f;
			currentModel->incMoveByZ(+0.01f);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
				currentModel->incMoveByX(-0.01f);
			}

			else {

				currentModel->incrotateAroundY(-0.0872665f);
			}
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
				currentModel->incMoveByX(+0.01);
			}

			else {

				currentModel->incrotateAroundY(0.0872665f);
			}
		}
		// Press HOME -> reset world and view (TODO Perspective maybe?)
		if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS)
		{
			// Reset Model positions and scales
			for (int i = 0; i < nbOfModels; i++) {
				models[i].reset();
			}
			// Reset camera
			cameraPosition = vec3(0.0f, 5.0f, 40.0f);
			cameraLookAt = vec3(0.0f, 1.0f, 0.0f);
			cameraUp = vec3(0.0f, 1.0f, 0.0f);
			cameraHorizontalAngle = 90.0f;
			cameraVerticalAngle = 0.0f;
		}
		// Keybinds for selecting rendering mode (affects axis and models)
		if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// Keybinds for selecting the model to manipulate
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
			currentModel = &models[0];
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
			currentModel = &models[1];
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
			currentModel = &models[2];
		if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
			currentModel = &models[3];
		/*
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		{
		modelMovementX += modelMoveSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		{
		modelMovementZ -= modelMoveSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		{
		modelMovementZ += modelMoveSpeed;
		}
		*/

		/*

		/*
		// Draw ground
		mat4 groundWorldMatrix = translate(mat4(1.0f), vec3(0.0f, -0.01f, 0.0f)) * scale(mat4(1.0f), vec3(1000.0f, 0.02f, 1000.0f));
		worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &groundWorldMatrix[0][0]);
		//
		//glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0
		// Draw pillars
		mat4 lineWorldMatrix = translate(mat4(1.0f), vec3(0.0f, 10.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.0f, 20.0f, 2.0f));
		glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &lineWorldMatrix[0][0]);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		*/

		// Draw everything
		drawGrid(worldMatrixLocation, colorLocation);
		drawAxis(worldMatrixLocation, colorLocation);
		// drawA9model(worldMatrixLocation, colorLocation, modelMovementX, modelMovementZ, scaleFactor);
		// Draw models
		for (int i = 0; i < nbOfModels; i++) {
			models[i].drawCharModel(i);
		}

		// @TODO 3 - Update and draw projectiles
		// ...

		/*
		for (list<Projectile>::iterator it = projectileList.begin(); it != projectileList.end(); it++) {
			it->Update(dt);
			it->Draw();
		}
		*/

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
		//{
		//    // In third person view, let's draw the spinning cube in world space, like any other models
		//    mat4 spinningCubeWorldMatrix = translate(mat4(1.0f), cameraPosition) *
		//                                   rotate(mat4(1.0f), radians(spinningCubeAngle), vec3(0.0f, 1.0f, 0.0f)) *
		//                                   scale(mat4(1.0f), vec3(0.1f, 0.1f, 0.1f));
		//    
		//    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &spinningCubeWorldMatrix[0][0]);
		//}
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
		cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
		cameraVerticalAngle -= dy * cameraAngularSpeed * dt;

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

		cameraLookAt = vec3(cosf(phi)*cosf(theta), sinf(phi), -cosf(phi)*sinf(theta));
		vec3 cameraSideVector = glm::cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));

		glm::normalize(cameraSideVector);


		// @TODO 5 = use camera lookat and side vectors to update positions with ASDW
		// adjust code below
		//if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // move camera to the left
		//{
		//    cameraPosition.x -= currentCameraSpeed * dt;
		//}
		//
		//if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // move camera to the right
		//{
		//    cameraPosition.x += currentCameraSpeed * dt;
		//}
		//
		//if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // move camera up
		//{
		//    cameraPosition.y -= currentCameraSpeed * dt;
		//}
		//
		//if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // move camera down
		//{
		//    cameraPosition.y += currentCameraSpeed * dt;
		//}


		//cameraControls(window, &cameraPosition, cameraSideVector, currentCameraSpeed, &cameraLookAt, &cameraHorizontalAngle, &cameraVerticalAngle, dt);
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // move camera to the left
		{
			cameraPosition -= cameraSideVector * currentCameraSpeed * dt;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // move camera to the right
		{
			cameraPosition += cameraSideVector * currentCameraSpeed * dt;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) // move camera up
		{
			cameraPosition -= cameraLookAt * currentCameraSpeed * dt;
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // move camera down
		{
			cameraPosition += cameraLookAt * currentCameraSpeed * dt;
		}




		// TODO 6
		// Set the view matrix for first and third person cameras
		// - In first person, camera lookat is set like below
		// - In third person, camera position is on a sphere looking towards center
		//mat4 viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp );

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
	}


	// Shutdown GLFW
	glfwTerminate();

	return 0;
}
