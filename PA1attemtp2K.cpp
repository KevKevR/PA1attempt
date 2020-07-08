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

class Projectile
{
public:
    Projectile(vec3 position, vec3 velocity, int shaderProgram) : mPosition(position), mVelocity(velocity)
    {
        mWorldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

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
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    
private:
    GLuint mWorldMatrixLocation;
    vec3 mPosition;
    vec3 mVelocity;
};

//used to pass a variable number of parameters through a shared function.
//note to others: you may expand the number of options to suit your needs.
struct options {
    float option1f;
    float option2f;
    float option3f;
    float option4f;
    float option5f;



    //etc...
    vec3 option1v3;
};
class CharModel {
public:
    CharModel(int shaderProgram) {
        worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
        colorLocation = glGetUniformLocation(shaderProgram, "objectColor");
        //relativeWorldMatrix = mat4(1.0f);

        relativeTranslateMatrix = mat4(1.0f);
        relativeRotateMatrix = mat4(1.0f);
        relativeScaleMatrix = mat4(1.0f);
        animateWorldMatrix = mat4(1.0f);

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


    mat4 getaAnimateWorldMatrix() {
        return animateWorldMatrix;
    }
    void setAnimateWorldMatrix(mat4 aniWM) {
        animateWorldMatrix = aniWM;
    }

    //deprecated, replaced with getRelevantWorldMatrix()
    //mat4 getRelativeWorldMatrix() {
    //    return relativeWorldMatrix;
    //}
    //deprecated, replaced with TRS
    //void setRelativeWorldMatrix(mat4 relWM) {
    //    relativeWorldMatrix = relWM;
    //}
    // deprecated, replaced with TRS
    //void addRelativeWorldMatrix(mat4 relWM) {
    //    relativeWorldMatrix = relWM * relativeWorldMatrix;
    //}
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
    mat4 getRelevantWorldMatrix() {
        return relativeTranslateMatrix * relativeRotateMatrix * relativeScaleMatrix * animateWorldMatrix;
    }

    virtual void draw(options opt, GLchar drawMode = GL_TRIANGLES) {
        //implement in derived class please.
    }

    //draws all passed models. for now hardcoded to 2.
    static void draw(CharModel* arr[5], options opt[5], GLchar drawMode) {
        for (int i = 0; i < 5; i++) {

            if (i == 2) { break; }      //<- remove later

            if (arr[i]) {
                arr[i]->draw(opt[i], drawMode);
            }
        }
    }
    //sets relativeWorldMatrix for passed model/matrix pair. deprecated, replaced with TRS
    //static void setRelativeWorldMatrix(map<CharModel*, mat4> mRelWM) {
    //    map<CharModel*, mat4>::iterator itr;
    //    for (itr = mRelWM.begin(); itr != mRelWM.end(); ++itr) {
    //        itr->first->setRelativeWorldMatrix(itr->second);
    //    }
    //}
    //adds relativeWorldMatrix for passed model/matrix pair. deprecated, replaced with TRS
    //static void addRelativeWorldMatrix(map<CharModel*, mat4> mRelWM) {
    //    map<CharModel*, mat4>::iterator itr;
    //    for (itr = mRelWM.begin(); itr != mRelWM.end(); ++itr) {
    //        itr->first->addRelativeWorldMatrix(itr->second);
    //    }
    //}
    //sets animateWorldMatrix for passed model/matrix pair.
    static void setAnimateWorldMatrix(map<CharModel*, mat4> mAniWM) {
        map<CharModel*, mat4>::iterator itr;
        for (itr = mAniWM.begin(); itr != mAniWM.end(); ++itr) {
            itr->first->setAnimateWorldMatrix(itr->second);
        }
    }
protected:
    GLuint worldMatrixLocation;
    GLuint colorLocation;
private:
    //mat4 relativeWorldMatrix;     //deprecated, replaced with TRS
    mat4 relativeTranslateMatrix;
    mat4 relativeRotateMatrix;
    mat4 relativeScaleMatrix;
    mat4 animateWorldMatrix;
};

class V9Model : public CharModel {
public:

    V9Model(int shaderProgram):CharModel(shaderProgram) {
        setRelativeTranslateMatrix(
            translate(mat4(1.0f),
                vec3(0,
                    0,
                    -20)));
    }

    void draw(options opt, GLchar drawMode = GL_TRIANGLES) {
        float heightScale = opt.option1f;
        float widthScale = opt.option2f;
        float angle = opt.option3f;
        int corners = opt.option4f;

        drawV9(worldMatrixLocation, drawMode, getRelevantWorldMatrix(), heightScale, widthScale, angle, corners);
    }
private:
    void drawV9(GLuint worldMatrixLocation, GLchar drawMode, mat4 relativeWorldMatrix = mat4(1.0f), float heightScale = 1, float widthScale = 1, float angle = 20, int corners = 4);
};

//example usage of model subclass.
//To convert to your model:
//- replace the three instances of class name with your model name.
//- replace the three instances of drawModel function signature with function that draws your model.
//- if parameters are needed, use the options struct to pass them through the draw function.
//  - optionally name variables to the appropriate option's use before passing them along to the draw model function.
//Just to clarify, the function code goes inside Model_template::drawModel() found below this class.
class Model_template : public CharModel {
public:

    Model_template(int shaderProgram) : CharModel(shaderProgram) {
        //initialize anything in case.
    }

    void draw(options opt, GLchar drawMode = GL_TRIANGLES) {
        // options
        float argument_1 = opt.option1f;

        //calls function to draw model with drawMode and getRelevantWorldMatrix(), plus any option arguments.
        drawModel(worldMatrixLocation, drawMode, getRelevantWorldMatrix(), argument_1);
    }
    
private:
    void drawModel(GLuint worldMatrixLocation, GLchar drawMode, mat4 relativeWorldMatrix, float parameter);

    //private stuff in case.
};
//replace this part with your draw function/code.
void Model_template::drawModel(GLuint worldMatrixLocation, GLchar drawMode, mat4 relativeWorldMatrix, float parameter){
    //code to draw model.
    //relativeWorldMatrix should apply to all parts of model, eg.
    // calculate partMatrix for each part
    // worldMatrix = relativeWorldMatrix * partMatrix;
    // put worldMatrix in worldMatrixLocation.
    // draw part.
    // do same for next part, etc.
}

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
                "in vec3 vertexColor;"
                "out vec4 FragColor;"
                "void main()"
                "{"
                "   FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0f);"
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
    // Cube model
    vec3 vertexArray[] = {  // position,                            color
        vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), //left - red
        vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
        
        vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f),
        
        vec3( 0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), // far - blue
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),
        
        vec3( 0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3( 0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f),
        
        vec3( 0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), // bottom - turquoise
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f),
        vec3( 0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f),
        
        vec3( 0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f),
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f),
        vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f),
        
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), // near - green
        vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
        vec3( 0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
        
        vec3( 0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
        vec3( 0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f),
        
        vec3( 0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), // right - purple
        vec3( 0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f),
        vec3( 0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f),
        
        vec3( 0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f),
        vec3( 0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f),
        vec3( 0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f),
        
        vec3( 0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), // top - yellow
        vec3( 0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f),
        
        vec3( 0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f),
        vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f),

        //line (0,0,-0.5)to(0,0,0.5)
        vec3(0.0f, 0.0f, -0.5f), vec3(0.8f, 0.4f, 0.0f),
        vec3(0.0f, 0.0f, 0.5f), vec3(0.2f, 0.0f, 0.8f)
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
                          2*sizeof(vec3), // stride - each vertex contain 2 vec3 (position, color)
                          (void*)0             // array buffer offset
                          );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          2*sizeof(vec3),
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
void drawGrid(GLuint worldMatrixLocation, mat4 relativeWorldMatrix = mat4(1.0f), float sideLength = 100, float cellLength = 1) {
    //const float sideLength = 100; //# of cells on side.
    //const float cellLength = 1; //length of side of a cell.

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
        glDrawArrays(GL_LINES, 36, 2);
    }
}

//Input worldMatrixLocation is location of worldMatrix.
//Input drawMode is mode to draw in.
//Input relativeWorldMatrix is matrix transformation to apply to model as a whole.
//Input heightScale is absolute height of model. Does not include box. (ie the box will be placed outside this height boundary).
//Input widthScale is horizontal thickness for V, and thickness for 9.
//Input angle is vertical angle the legs of V will make relative to the zenith.
//Input corners is the number of corners(or sides) of the regular polygon used to form the 9. Forced minimum value of 3.
//
//Function will create a letter-number pair model (V9), surrounded by a border, centered at (0,0,0) in (X,Y,Z).
//note: assumes cube object in vertex array is centered and normalized, from (-0.5,-0.5,-0.5)to(0.5,0.5,0.5).
//void drawV9(GLuint worldMatrixLocation, GLchar drawMode, mat4 relativeWorldMatrix = mat4(1.0f), float heightScale = 1, float widthScale = 1, float angle = 20, int corners = 4) {
void V9Model::drawV9(GLuint worldMatrixLocation, GLchar drawMode, mat4 relativeWorldMatrix, float heightScale, float widthScale, float angle, int corners) {
    corners = (corners < 3) ? 3 : corners;  //minimum 3 corners.
    
    mat4 inertialWorldMatrix;   //model from an inertial view of reference, ie centered at origin.

    mat4 worldMatrix;
    //total height = heightScale
    //total width = spacing
    //loops for left and right half
    float r_angle = radians((float)angle);

    //future calculations done early to determine modelPositioningMatrix for letter, and box measurements.
    float future_apothem = (heightScale + widthScale) / 4;
    float future_centraAngle = radians(360.0f / corners);
    float future_radius = future_apothem/cos(future_centraAngle/2);

    float letterWidth;  //absolute width of V
    letterWidth = (heightScale - fabs(widthScale * cosf(r_angle) * sinf(r_angle))) / cosf(r_angle) * sinf(r_angle) + widthScale * cosf(r_angle) * cosf(r_angle);
    mat4 modelPositioningMatrix =
        translate(mat4(1.0f),
            vec3(-(letterWidth/2 + future_apothem),
                0,
                0));

    //Draw "V".
    for (int i = 0; i < 2; ++i)
    {
        float angle_i = r_angle *(-1+2*i);  //vertical angle of leg
        float base = widthScale * cosf(angle_i); //base of cube to scale by
        float height = (heightScale - fabs(widthScale * cosf(angle_i) * sinf(angle_i))) / cosf(angle_i);    //heigtht of cube to scale by
        float spacing = height * sinf(angle_i) + base * cosf(angle_i);  //only works with i = 0, 1.     //absolute width of half a V, \/.

        inertialWorldMatrix =
            ////rotate 90 degrees. flat on ground
            //rotate(mat4(1.0f),
            //    radians(90.0f),
            //    vec3(1.0f, 0.0f, 0.0f))*
            //move into position relative to center of model.
            modelPositioningMatrix
            //space out halves.
            * translate(mat4(1.0f),
                vec3(spacing * i,
                    0,
                    0))
            //rotate by angle_i.
            * rotate(mat4(1.0f),
                angle_i,
                vec3(0.0f, 0.0f, -1.0f))
            //scale to match target.
            * scale(mat4(1.0f),
                vec3(base, height, 1.0f));

        //tramsformation to model as a whole
        worldMatrix = relativeWorldMatrix * inertialWorldMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glDrawArrays(drawMode, 0, 36);
    }
    float base = widthScale * cosf(r_angle);
    float bottomHeight = base * sinf(r_angle);
    float bottomBase = base * cosf(r_angle)*2;
    inertialWorldMatrix =
        ////rotate 90 degrees. flat on ground
        //rotate(mat4(1.0f),
        //    radians(90.0f),
        //    vec3(1.0f, 0.0f, 0.0f))*
        //move into position relative to center of model.
        modelPositioningMatrix
        //position.
        * translate(mat4(1.0f),
            vec3(letterWidth /2,
                (-heightScale + bottomHeight)/2,
                0))
        //scale to fit bottom.
        * scale(mat4(1.0f),
            vec3(bottomBase, bottomHeight, 1.0f));
    worldMatrix = relativeWorldMatrix * inertialWorldMatrix;
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    glDrawArrays(drawMode, 0, 36);


    //Draw 9.
    //total height = heightScale
    //total width = 2 * apothem
    modelPositioningMatrix =
        translate(mat4(1.0f),
            vec3(letterWidth,
                -heightScale / 2,
                0));
    for (int i = 0; i < corners; ++i)
    {
        float centralAngle = radians(360.0f / corners);
        float angle_i = centralAngle * i;
        float width = widthScale;
        float apothem = (heightScale + width) / 4;
        
        base = 2 * apothem * tanf(centralAngle/2);

        //draw head
        inertialWorldMatrix =
            ////rotate 90 degrees. flat on ground
            //rotate(mat4(1.0f),
            //    radians(90.0f),
            //    vec3(1.0f, 0.0f, 0.0f))*
            //move into position relative to center of model.
            modelPositioningMatrix
            //top height, place center point of polygon.
            * translate(mat4(1.0f),
                vec3(0,
                    heightScale - apothem,
                    0))
            //rotate by angle_i. done after translating to simplify process.
            * rotate(mat4(1.0f),
                angle_i,
                vec3(0.0f, 0.0f, -1.0f))
            //apothem distance.
            * translate(mat4(1.0f),
                vec3(0,
                    apothem - width/2,  //cube object is centered at origin, not its corner.
                    0))
            //scale to match target.
            * scale(mat4(1.0f),
                vec3(base, width, 1.0f));
        worldMatrix = relativeWorldMatrix * inertialWorldMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glDrawArrays(drawMode, 0, 36);

        //draw tail
        //Offset angle_i by half a turn to match flush with ground
        angle_i += radians(180.0f);
        angle_i -= (angle_i > radians(360.0f)) ? radians(360.0f) : 0;
        if (angle_i < radians(240.0f) && angle_i > 0.0f) {
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
                        apothem,
                        0))
                //rotate by angle_i. done after translating to simplify process.
                * rotate(mat4(1.0f),
                    angle_i,
                    vec3(0.0f, 0.0f, -1.0f))
                //apothem distance.
                * translate(mat4(1.0f),
                    vec3(0,
                        apothem - width / 2,
                        0))
                //scale to match target.
                * scale(mat4(1.0f),
                    vec3(base, width, 1.0f));
            worldMatrix = relativeWorldMatrix * inertialWorldMatrix;
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
            glDrawArrays(drawMode, 0, 36);
        }
    }

    //draw box around model(extra)
    float boxVerticalWidth = heightScale/100;   //thickness of horizontal bars
    float boxHorizontalWidth = widthScale/20;   //thickness of vertical bars
    for (int i = 0; i < 4; i++) {
        bool isHorizontal = i < 2;
        int parity = pow(-1, i + 1);
        //horizontal and vertical bar distinction. 
        mat4 translateMatrix = (isHorizontal)
            ? translate(mat4(1.0f),
                vec3(0,
                    (heightScale + boxVerticalWidth) / 2 * parity,
                    0))
            : translate(mat4(1.0f),
                vec3((letterWidth + future_radius + boxHorizontalWidth/2) * parity,
                    0,
                    0));
        mat4 scaleMatrix = (isHorizontal)
            ? scale(mat4(1.0f),
                vec3(2 * (letterWidth + future_radius + boxHorizontalWidth), boxVerticalWidth, 1.0f))
            : scale(mat4(1.0f),
                vec3(boxHorizontalWidth, heightScale, 1.0f));

        inertialWorldMatrix =
            ////rotate 90 degrees. flat on ground
            //rotate(mat4(1.0f),
            //    radians(90.0f),
            //    vec3(1.0f, 0.0f, 0.0f))*
            //position around rest of model.
            translateMatrix
            //scale to whole width.
            * scaleMatrix;
        worldMatrix = relativeWorldMatrix * inertialWorldMatrix;
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
        glDrawArrays(drawMode, 0, 36);
    
    }
}

//function will return the selected draw mode on correct key presses.
//note: undefined priority in case multiple correct keys are pressed (but will select a draw mode).
GLchar drawControl(GLFWwindow* window, GLchar previousDrawMode) {
    map<int, GLchar> inputsToDrawModes;
    map<int, GLchar>::iterator itr;
    inputsToDrawModes.insert(pair<int, GLchar>(GLFW_KEY_P, GL_POINTS));
    inputsToDrawModes.insert(pair<int, GLchar>(GLFW_KEY_L, GL_LINES));
    inputsToDrawModes.insert(pair<int, GLchar>(GLFW_KEY_T, GL_TRIANGLES));

    GLchar selectedMode = previousDrawMode;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) // capital letters only
    {
        //if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) // draw points
        //{
        //    selectedMode = GL_POINTS;
        //}
        //if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) // draw lines
        //{
        //    selectedMode = GL_LINES;
        //}
        //if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) // draw triangles
        //{
        //    selectedMode = GL_TRIANGLES;
        //}

        for (itr = inputsToDrawModes.begin(); itr != inputsToDrawModes.end(); ++itr) {
            if (glfwGetKey(window, itr->first) == GLFW_PRESS) // draw mode
            {
                selectedMode = itr->second;
            }
        }
    }
    return selectedMode;
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
mat4* modelControl(GLFWwindow* window) {
    mat4* selectedTransformation = new mat4[3];
    selectedTransformation[0] = mat4(1.0f);
    selectedTransformation[1] = mat4(1.0f);
    selectedTransformation[2] = mat4(1.0f);

    struct transformation {
        mat4 matrix;
        int type;
    };
    map<int, transformation> inputsToModelMatrix;
    map<int, transformation>::iterator itr;
    float transformSpeed = 0.2f;
    float translateSpeed = transformSpeed;
    float rotateSpeed = transformSpeed*20;
    float scaleSpeed = transformSpeed/4;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) // capital case letters
    {
        inputsToModelMatrix.insert(pair<int, transformation>(GLFW_KEY_W, { 
            translate(mat4(1.0f),
                vec3(0,
                    transformSpeed,
                    0)), 0 }));
        inputsToModelMatrix.insert(pair<int, transformation>(GLFW_KEY_A, { 
            translate(mat4(1.0f),
                vec3(transformSpeed,
                    0,
                    0)), 0 }));
        inputsToModelMatrix.insert(pair<int, transformation>(GLFW_KEY_S, { 
            translate(mat4(1.0f),
                vec3(0,
                    -transformSpeed,
                    0)), 0 }));
        inputsToModelMatrix.insert(pair<int, transformation>(GLFW_KEY_D, { 
            translate(mat4(1.0f),
                vec3(-transformSpeed,
                    0,
                    0)), 0 }));

        inputsToModelMatrix.insert(pair<int, transformation>(GLFW_KEY_U, { 
            scale(mat4(1.0f),
                vec3((1 + scaleSpeed), (1 + scaleSpeed), (1 + scaleSpeed))), 2 }));
        inputsToModelMatrix.insert(pair<int, transformation>(GLFW_KEY_J, { 
            scale(mat4(1.0f),
                vec3(1.0f / (1 + scaleSpeed), 1.0f / (1 + scaleSpeed), 1.0f / (1 + scaleSpeed))), 2 }));
    }
    else { // lower case letters
        //inputsToModelMatrix.insert(pair<int, transformation>(GLFW_KEY_W, {
        //    rotate(mat4(1.0f),
        //        radians(90.0f),
        //        vec3(1.0f, 0.0f, 0.0f)), 1 }));
        inputsToModelMatrix.insert(pair<int, transformation>(GLFW_KEY_A, {
            rotate(mat4(1.0f),
                radians(rotateSpeed),
                vec3(0.0f, 1.0f, 0.0f)), 1 }));
        //inputsToModelMatrix.insert(pair<int, transformation>(GLFW_KEY_S, {
        //    rotate(mat4(1.0f),
        //        radians(90.0f),
        //        vec3(1.0f, 0.0f, 0.0f)), 1 }));
        inputsToModelMatrix.insert(pair<int, transformation>(GLFW_KEY_D, {
            rotate(mat4(1.0f),
                radians(rotateSpeed),
                vec3(0.0f, -1.0f, 0.0f)), 1 }));
    }

    for (itr = inputsToModelMatrix.begin(); itr != inputsToModelMatrix.end(); ++itr) {
        if (glfwGetKey(window, itr->first) == GLFW_PRESS) // select model
        {
            selectedTransformation[itr->second.type] = itr->second.matrix * selectedTransformation[itr->second.type];
        }
    }
    return selectedTransformation;
}

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
    GLFWwindow* window = glfwCreateWindow(800, 600, "Comp371 - Lab 03", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // @TODO 3 - Disable mouse cursor
    // ...
    
    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    // Compile and link shaders here ...
    int shaderProgram = compileAndLinkShaders();
    
    // We can set the shader once, since we have only one
    glUseProgram(shaderProgram);

    // Camera parameters for view transform
    const vec3 initial_cameraPosition(0.0f, 5.0f, 20.0f);
    const vec3 initial_cameraLookAt(0.0f, 0.0f, -1.0f);
    const vec3 initial_cameraUp(0.0f, 1.0f, 0.0f);
    // Camera parameters for view transform
    vec3 cameraPosition = initial_cameraPosition;
    //vec3 cameraLookAt = initial_cameraLookAt;
    vec3 cameraUp = initial_cameraUp;
    //vec3 cameraPosition;
    vec3 cameraLookAt;
    //vec3 cameraUp;


    // Other camera parameters
    float cameraSpeed = 10.0f;
    float cameraFastSpeed = 2 * cameraSpeed;
    float cameraHorizontalAngle = 90.0f;
    float cameraVerticalAngle = -14.0f;
    bool  cameraFirstPerson = true; // press 1 or 2 to toggle this variable

    // Spinning cube at camera position
    float spinningCubeAngle = 0.0f;
    
    // Set projection matrix for shader, this won't change
    mat4 projectionMatrix = glm::perspective(70.0f,            // field of view in degrees
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
    //mat4 viewMatrix;
    
    
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
    list<Projectile> projectileList;
    
    GLchar drawMode = GL_TRIANGLES;

    //V9 model variables
    float angle = 0.0f;
    float v_angle = 10.0f;
    const float v_minAngle = 10.0f;
    const float v_maxAngle = 28.0f;
    float spinningSpeed = 20.0f;
    int cycle = 0;
    bool spin = false;

    //Models
    CharModel* selectedModel;
    CharModel* models[5];
    options opt[5];
    int modelIndex = 0;

    V9Model v9(shaderProgram);
    V9Model v9_2(shaderProgram);    //placeholder
    //3
    //4
    //5

    models[0] = &v9;
    models[1] = &v9_2;
    //models[2] = &;
    //models[3] = &;
    //models[4] = &;
    
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

        drawMode = drawControl(window, drawMode);
        modelIndex = selectModelControl(window, modelIndex);
        selectedModel = models[modelIndex];

        // Draw geometry
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // Draw ground
        mat4 groundWorldMatrix;
        groundWorldMatrix = translate(mat4(1.0f), vec3(0.0f, -0.01f, 0.0f)) * scale(mat4(1.0f), vec3(1000.0f, 0.02f, 1000.0f));
        GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
        //glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &groundWorldMatrix[0][0]);
        //
        //glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0
        
        {
            // Draw pillars
            mat4 lineWorldMatrix;
            //lineWorldMatrix = translate(mat4(1.0f), vec3(0.0f, 10.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.0f, 20.0f, 2.0f));
            lineWorldMatrix = translate(mat4(1.0f), vec3(0.0f, 2.0f, 0.0f)) * scale(mat4(1.0f), vec3(2.0f, 4.0f, 2.0f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &lineWorldMatrix[0][0]);
            //glDrawArrays(drawMode, 0, 36);

            drawGrid(worldMatrixLocation, mat4(1.0f), 40, 0.5f);
            lineWorldMatrix = translate(mat4(1.0f), vec3(10.0f, 10.0f, 10.0f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &lineWorldMatrix[0][0]);
            glDrawArrays(drawMode, 0, 36);

            lineWorldMatrix = translate(mat4(1.0f), vec3(10.0f, 8.0f, 10.0f)) * scale(mat4(1.0f), vec3(0.5f, 1.0f, 2.0f));
            //glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &lineWorldMatrix[0][0]);
            //glDrawArrays(GL_LINES, 36, 2);
            glLineWidth(10);
            drawGrid(worldMatrixLocation, lineWorldMatrix, 10, 1.2f);
            glLineWidth(1);
        }
        //animation calculation for V9 model.
        {
            angle += dt * spinningSpeed;
            angle -= (angle > 360) ? 360 : 0;
            mat4 spinningMatrix =
                (spin) ? rotate(mat4(1.0f),
                    radians(angle),
                    vec3(0.0f, 1.0f, 0.0f))
                : mat4(1.0f);
            if (angle > 180) {
                cycle = -angle / 20 + 22;
                glClearColor(2.0f - angle / 180, 0.3f, 0.4f, 1.0f);
            }
            else {
                cycle = angle / 20 + 5;
                glClearColor(angle / 180, 0.3f, 0.4f, 1.0f);
            }
            if (angle > 300) {
                v_angle = v_maxAngle - (angle - 300) / 60 * (v_maxAngle - v_minAngle);
            }
            else {
                v_angle = angle / 300 * (v_maxAngle - v_minAngle) + v_minAngle;
            }
            v_angle += 2 * sin(radians(angle * 3) + 1);
            //drawV9(worldMatrixLocation, drawMode, spinningMatrix, 5, 1, v_angle, cycle);
        }
        
        //compute relativeWorldMatrices (passive) for each model.
        mat4 animateWorldMatrix1 = //<- will replace these later/placeholder/demonstration.
            rotate(mat4(1.0f),
                radians(angle),
                vec3(0.0f, 1.0f, 0.0f))
            * scale(mat4(1.0f),
                vec3(0.5f, 2.0f, 5.0f));
        mat4 animateWorldMatrix2 =
            rotate(mat4(1.0f),
                radians(angle) * cos(radians(angle*4)),
                vec3(0.0f, 0.0f, 1.0f))
            * scale(mat4(1.0f),
                vec3(0.4f, 0.6f, 1.0f));

        //assign options to models (if applicable)
        opt[0].option1f = 5;
        opt[0].option2f = 1;
        opt[0].option3f = v_angle;
        opt[0].option4f = cycle;

        opt[1] = opt[0];
        opt[1].option3f = 20;

        //etc.
        
        //assign animateWorldMatrices to each model.
        map<CharModel*, mat4> modelsAnimateMatrices;
        modelsAnimateMatrices.insert(pair<CharModel*, mat4>(&v9, animateWorldMatrix1));
        //modelsAnimateMatrices.insert(pair<CharModel*, mat4>(&v9_2, animateWorldMatrix2));
        //modelsAnimateMatrices.insert(pair<CharModel, mat4>(, animateWorldMatrix3));
        //modelsAnimateMatrices.insert(pair<CharModel, mat4>(, animateWorldMatrix4));
        //modelsAnimateMatrices.insert(pair<CharModel, mat4>(, animateWorldMatrix5));

        CharModel::setAnimateWorldMatrix(modelsAnimateMatrices);

        //selectedModel = &v9;
        //selectedModel->setRelativeWorldMatrix(relativeWorldMatrix1);
        //selectedModel = &v9_2;
        //selectedModel->setRelativeWorldMatrix(relativeWorldMatrix2);

        //done previously, but could move the code here instead.
        //modelIndex = selectModelControl(window, modelIndex);
        //selectedModel = models[modelIndex];

        mat4* relativeWorldMatrix = modelControl(window);

        selectedModel->addRelativeWorldMatrix(relativeWorldMatrix[0], relativeWorldMatrix[1], relativeWorldMatrix[2]);

        //draw all models
        CharModel::draw(models, opt, drawMode);

        //glBegin(GL_LINES);
        //glLineWidth(3);
        //glColor3f(0.0f, 1.0f, 0.0f);
        //glVertex2f(-0.7f, -1.0f);
        //glColor3f(0.0f, 0.4f, 1.0f);
        //glVertex2f(-0.7f, 1.0f);
        //glEnd();
        //glBegin(GL_LINES);

        //for (int i = 10; i != 0; i--) {

        //    // po x osi
        //    glVertex2f(-i, -0.1);
        //    glVertex2f(-i, 0.1);
        //    glVertex2f(i, -0.1);
        //    glVertex2f(i, 0.1);
        //    //po y osi
        //    glVertex2f(-0.1, -i);
        //    glVertex2f(0.1, -i);
        //    glVertex2f(-0.1, i);
        //    glVertex2f(0.1, i);

        //    //po z osi
        //    glColor3f(1.0f, 0.0f, 1.0f); //seems it doesnt register
        //    glVertex3f(0, -0.3, -i);
        //    glVertex3f(0, 0.3, -i);
        //    glVertex3f(0, -0.3, i);
        //    glVertex3f(0, 0.3, i);
        //}
        //glEnd();
    
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
        
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) // move camera down
        {
            cameraFirstPerson = true;
        }

        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) // move camera down
        {
            cameraFirstPerson = false;
        }

        
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

        //doesn't work?
        if (glfwGetKey(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) // control camera with mouse
        {
            //cameraPosition -= cameraSideVector * currentCameraSpeed * dt * dx;
            cameraVerticalAngle -= dy * cameraAngularSpeed * dt;
            cameraVerticalAngle -= 50;
            glClearColor(0.4f, 0.3f, 1.0f, 1.0f);
        }

        cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;
        cameraVerticalAngle   -= dy * cameraAngularSpeed * dt;
        
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

        //doesn't work?
        if (glfwGetKey(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) // control camera with mouse
        {
            glClearColor(0.4f, 0.3f, 1.0f, 1.0f);
            cameraPosition -= cameraSideVector * (float)(currentCameraSpeed * dt * dx);
            cameraPosition -= cameraSideVector * 10.0f;
        }
        
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
        //cameraControls(window, &cameraPosition, &cameraLookAt, cameraSideVector, currentCameraSpeed, &cameraHorizontalAngle, &cameraVerticalAngle, dt);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // move camera to the left
        {
            cameraPosition -= cameraSideVector * currentCameraSpeed * dt;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // move camera to the right
        {
            cameraPosition += cameraSideVector * currentCameraSpeed * dt;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // move camera up
        {
            cameraPosition -= cameraLookAt * currentCameraSpeed * dt;
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // move camera down
        {
            cameraPosition += cameraLookAt * currentCameraSpeed * dt;
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

        if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) // reset camera
        {
            cameraPosition = initial_cameraPosition;
            cameraLookAt = initial_cameraLookAt;
            cameraUp = initial_cameraUp;
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
        if (lastMouseLeftState == GLFW_RELEASE && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            const float projectileSpeed = 25.0f;
            projectileList.push_back(Projectile(cameraPosition, projectileSpeed * cameraLookAt, shaderProgram));

            glClearColor(0.5f, 0.5f, 0.0f, 1.0f);
        }
        lastMouseLeftState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);


    }

    
    // Shutdown GLFW
    glfwTerminate();
    
	return 0;
}
