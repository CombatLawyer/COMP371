//
// COMP 371 Quiz 2
// Jason Liang 40186162
// Credit to Nicolas Bergeron for the lab 3 framework.
//

#include <iostream>
#include <algorithm>
#include <list>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <shaderloader.h>

#include <irrKlang.h>
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")

#include "OBJloader.h"    //For loading .obj files

using namespace glm;
using namespace std;

unsigned int indexCount;

struct TexturedColoredVertex
{
    TexturedColoredVertex(vec3 _position, vec3 _color, vec2 _uv, vec3 _normals)
        : position(_position), color(_color), uv(_uv), normals(_normals) {}

    vec3 position;
    vec3 color;
    vec2 uv;
    vec3 normals;
};

// Cube model
const TexturedColoredVertex texturedCubeVertexArray[] = {  // position, color, UV, normals
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec3(-1.0f,  0.0f, 0.0f)), //left - red
    TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 1.0f), vec3(-1.0f,  0.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f), vec3(-1.0f,  0.0f, 0.0f)),

    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(0.0f, 0.0f), vec3(-1.0f,  0.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 1.0f), vec3(-1.0f,  0.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 0.0f), vec2(1.0f, 0.0f), vec3(-1.0f,  0.0f, 0.0f)),

    TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), vec3(0.0f,  0.0f, -1.0f)), // far - blue
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), vec3(0.0f,  0.0f, -1.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f), vec3(0.0f,  0.0f, -1.0f)),

    TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), vec3(0.0f,  0.0f, -1.0f)),
    TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f), vec3(0.0f,  0.0f, -1.0f)),
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), vec3(0.0f,  0.0f, -1.0f)),

    TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f), vec3(0.0f,  -1.0f, 0.0f)), // bottom - turquoise
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f), vec3(0.0f,  -1.0f, 0.0f)),
    TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f), vec3(0.0f,  -1.0f, 0.0f)),

    TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f), vec3(0.0f,  -1.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f), vec3(0.0f,  -1.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f,-0.5f,-0.5f), vec3(0.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f), vec3(0.0f,  -1.0f, 0.0f)),

    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f), vec3(0.0f,  0.0f, 1.0f)), // near - green
    TexturedColoredVertex(vec3(-0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f), vec3(0.0f,  0.0f, 1.0f)),
    TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f), vec3(0.0f,  0.0f, 1.0f)),

    TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f), vec3(0.0f,  0.0f, 1.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f), vec3(0.0f,  0.0f, 1.0f)),
    TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(0.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f), vec3(0.0f,  0.0f, 1.0f)),

    TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), vec3(1.0f,  0.0f, 0.0f)), // right - purple
    TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), vec3(1.0f,  0.0f, 0.0f)),
    TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 0.0f), vec3(1.0f,  0.0f, 0.0f)),

    TexturedColoredVertex(vec3(0.5f,-0.5f,-0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 0.0f), vec3(1.0f,  0.0f, 0.0f)),
    TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(1.0f, 1.0f), vec3(1.0f,  0.0f, 0.0f)),
    TexturedColoredVertex(vec3(0.5f,-0.5f, 0.5f), vec3(1.0f, 0.0f, 1.0f), vec2(0.0f, 1.0f), vec3(1.0f,  0.0f, 0.0f)),

    TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f), vec3(0.0f,  1.0f, 0.0f)), // top - yellow
    TexturedColoredVertex(vec3(0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 0.0f), vec3(0.0f,  1.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f), vec3(0.0f,  1.0f, 0.0f)),

    TexturedColoredVertex(vec3(0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(1.0f, 1.0f), vec3(0.0f,  1.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f,-0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 0.0f), vec3(0.0f,  1.0f, 0.0f)),
    TexturedColoredVertex(vec3(-0.5f, 0.5f, 0.5f), vec3(1.0f, 1.0f, 0.0f), vec2(0.0f, 1.0f), vec3(0.0f,  1.0f, 0.0f))
};

int createTexturedCubeVertexArrayObject()
{
    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texturedCubeVertexArray), texturedCubeVertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        sizeof(TexturedColoredVertex), // stride - each vertex contain 2 vec3 (position, color)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(TexturedColoredVertex),
        (void*)sizeof(vec3)      // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,                            // attribute 2 matches aUV in Vertex Shader
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(TexturedColoredVertex),
        (void*)(2 * sizeof(vec3))      // uv is offseted by 2 vec3 (comes after position and color)
    );
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3,                            // attribute 3 matches normals in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(TexturedColoredVertex),
        (void*)(2 * sizeof(vec3) + sizeof(vec2))      // uv is offseted by 2 vec3 (comes after position and color) and a vec2 uv
    );
    glEnableVertexAttribArray(3);

    return vertexBufferObject;
}

int createSphereObject()
{
    // A vertex is a point on a polygon, it contains positions and other data (eg: colors)
    unsigned int sphereVAO;
    glGenVertexArrays(1, &sphereVAO);

    unsigned int vbo, ebo;
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    vector<vec3> positions;
    vector<vec2> uv;
    vector<vec3> normals;
    vector<unsigned int> indices;
    vector<vec3> colors;

    const unsigned int X_SEGMENTS = 64;
    const unsigned int Y_SEGMENTS = 64;
    const float PI = 3.14159265359;
    for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
    {
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
        {
            float xSegment = (float)x / (float)X_SEGMENTS;
            float ySegment = (float)y / (float)Y_SEGMENTS;
            float xPos = cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
            float yPos = cos(ySegment * PI);
            float zPos = sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

            positions.push_back(vec3(xPos, yPos, zPos));
            colors.push_back(vec3(1.0f, 0.0f, 0.0f));
            uv.push_back(vec2(xSegment, ySegment));
            normals.push_back(vec3(xPos, yPos, zPos));
        }
    }

    bool oddRow = false;
    for (unsigned int y = 0; y < Y_SEGMENTS; ++y)
    {
        if (!oddRow) // even rows: y == 0, y == 2; and so on
        {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
            {
                indices.push_back(y * (X_SEGMENTS + 1) + x);
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
            }
        }
        else
        {
            for (int x = X_SEGMENTS; x >= 0; --x)
            {
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                indices.push_back(y * (X_SEGMENTS + 1) + x);
            }
        }
        oddRow = !oddRow;
    }
    indexCount = indices.size();

    vector<float> data;
    for (unsigned int i = 0; i < positions.size(); ++i)
    {
        data.push_back(positions[i].x);
        data.push_back(positions[i].y);
        data.push_back(positions[i].z);
        if (colors.size() > 0) {
            data.push_back(colors[i].x);
            data.push_back(colors[i].y);
            data.push_back(colors[i].z);
        }
        if (uv.size() > 0)
        {
            data.push_back(uv[i].x);
            data.push_back(uv[i].y);
        }
        if (normals.size() > 0)
        {
            data.push_back(normals[i].x);
            data.push_back(normals[i].y);
            data.push_back(normals[i].z);
        }
    }
    glBindVertexArray(sphereVAO);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    float stride = (3 + 2 + 3 + 3) * sizeof(float);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)(8 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0); // VAO already stored the state we just defined, safe to unbind buffer
    glBindVertexArray(0); // Unbind to not modify the VAO

    return sphereVAO;
}

float skyboxVertices[] = {
    // positions          
    -100.0f,  100.0f, -100.0f,
    -100.0f, -100.0f, -100.0f,
     100.0f, -100.0f, -100.0f,
     100.0f, -100.0f, -100.0f,
     100.0f,  100.0f, -100.0f,
    -100.0f,  100.0f, -100.0f,

    -100.0f, -100.0f,  100.0f,
    -100.0f, -100.0f, -100.0f,
    -100.0f,  100.0f, -100.0f,
    -100.0f,  100.0f, -100.0f,
    -100.0f,  100.0f,  100.0f,
    -100.0f, -100.0f,  100.0f,

     100.0f, -100.0f, -100.0f,
     100.0f, -100.0f,  100.0f,
     100.0f,  100.0f,  100.0f,
     100.0f,  100.0f,  100.0f,
     100.0f,  100.0f, -100.0f,
     100.0f, -100.0f, -100.0f,

    -100.0f, -100.0f,  100.0f,
    -100.0f,  100.0f,  100.0f,
     100.0f,  100.0f,  100.0f,
     100.0f,  100.0f,  100.0f,
     100.0f, -100.0f,  100.0f,
    -100.0f, -100.0f,  100.0f,

    -100.0f,  100.0f, -100.0f,
     100.0f,  100.0f, -100.0f,
     100.0f,  100.0f,  100.0f,
     100.0f,  100.0f,  100.0f,
    -100.0f,  100.0f,  100.0f,
    -100.0f,  100.0f, -100.0f,

    -100.0f, -100.0f, -100.0f,
    -100.0f, -100.0f,  100.0f,
     100.0f, -100.0f, -100.0f,
     100.0f, -100.0f, -100.0f,
    -100.0f, -100.0f,  100.0f,
     100.0f, -100.0f,  100.0f
};

unsigned int loadCubemap(vector<string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

// shader variable setters
void SetUniformMat4(GLuint shader_id, const char* uniform_name, mat4 uniform_value)
{
    glUseProgram(shader_id);
    glUniformMatrix4fv(glGetUniformLocation(shader_id, uniform_name), 1, GL_FALSE, &uniform_value[0][0]);
}

void SetUniformVec3(GLuint shader_id, const char* uniform_name, vec3 uniform_value)
{
    glUseProgram(shader_id);
    glUniform3fv(glGetUniformLocation(shader_id, uniform_name), 1, value_ptr(uniform_value));
}

template <class T>
void SetUniform1Value(GLuint shader_id, const char* uniform_name, T uniform_value)
{
    glUseProgram(shader_id);
    glUniform1i(glGetUniformLocation(shader_id, uniform_name), uniform_value);
    glUseProgram(0);
}

void SetUniformfValue(GLuint shader_id, const char* uniform_name, float uniform_value)
{
    glUseProgram(shader_id);
    glUniform1f(glGetUniformLocation(shader_id, uniform_name),uniform_value);
    glUseProgram(0);
}


// Class used to draw the racket at the specified location
class Racket
{
public:
    int playerNumber; // Will keep track of the current active player
    vec3 centerOfRacket; // Keeps track of the location of the center of the racket

    Racket(float worldXAngle, float worldYAngle, vec3 location, GLuint shader, GLenum renderMode, int number, float initialAngle, float armAngle, float elbowAngle, float racketAngle, GLuint armTexture, GLuint racketTexture, bool texture)
    {
        // Set player number to this racket
        playerNumber = number;

        // First let us generate the arm
        mat4 partMatrix = scale(mat4(1.0f), vec3(0.3f, 2.0f, 0.3f));

        // Initial movement to rotate and place the model away from the origin
        mat4 armMatrix = translate(mat4(1.0f), location);
        armMatrix = rotate(armMatrix, radians(armAngle), vec3(0.0f, 1.0f, 0.0f)); //Rotate the arm
        armMatrix = rotate(armMatrix, radians(initialAngle), vec3(0.0f, 0.0f, 1.0f)); // Keeps track of the arm + forearm + racket

        mat4 worldMatrix = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f)) * armMatrix * partMatrix;

        // Draw the arm at the location
        if (texture)
        {
            glBindTexture(GL_TEXTURE_2D, armTexture);
        }

        SetUniformMat4(shader, "world_matrix", worldMatrix);
        SetUniformVec3(shader, "object_color", vec3(0.878f, 0.675f, 0.412f));
        glDrawArrays(renderMode, 0, 36);

        // Now we generate the forearm in respect to the arm's location
        partMatrix = scale(mat4(1.0f), vec3(0.3f, 2.0f, 0.3f));

        // Clamp elbow angle to realistic values
        if (elbowAngle > 100)
        {
            elbowAngle = 100.0f;
        }
        else if (elbowAngle < 0)
        {
            elbowAngle = 0.0f;
        }

        mat4 forearmMatrix = translate(mat4(1.0f), vec3(0.0f, 1.8f, 0.0f));
        forearmMatrix = rotate(forearmMatrix, radians(elbowAngle), vec3(0.0f, 0.0f, 1.0f)); // Keeps track of the forearm + racket

        // Adjusting for the slight movement to correct for the rotation
        forearmMatrix = forearmMatrix * translate(mat4(1.0f), vec3(-sin(radians(elbowAngle)) * 0.9f, 0.9f - sin(radians(90 - elbowAngle)), 0.0f));

        partMatrix = forearmMatrix * partMatrix;
        worldMatrix = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f)) * armMatrix * partMatrix;

        // Draw the forearm at the location
        SetUniformMat4(shader, "world_matrix", worldMatrix);
        SetUniformVec3(shader, "object_color", vec3(0.878f, 0.675f, 0.412f));
        glDrawArrays(renderMode, 0, 36);

        // Finally we draw the racket, starting with the handle
        partMatrix = scale(mat4(1.0f), vec3(0.2f, 1.2f, 0.2f));

        mat4 racketMatrix = translate(mat4(1.0f), vec3(-sin(radians(elbowAngle)) * 2.4f, 0.9f + cos(radians(elbowAngle)) * 2.4f, 0.0f));
        racketMatrix = rotate(racketMatrix, radians(elbowAngle), vec3(0.0f, 0.0f, 1.0f));

        partMatrix = racketMatrix * partMatrix;
        worldMatrix = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f)) * armMatrix * partMatrix;

        // Draw the handle at the end of the forearm
        if (texture)
        {
            glBindTexture(GL_TEXTURE_2D, racketTexture);
        }

        SetUniformMat4(shader, "world_matrix", worldMatrix);
        SetUniformVec3(shader, "object_color", vec3(0.643f, 0.455f, 0.286f));
        glDrawArrays(renderMode, 0, 36);

        // Now we draw the racket, the entire racket will be built using this as the center
        centerOfRacket = vec3(-sin(radians(elbowAngle)) * 3.9f, 0.9f + cos(radians(elbowAngle)) * 3.9f, 0.0f);

        // Start with the base
        partMatrix = scale(mat4(1.0f), vec3(1.2f, 0.2f, 0.2f));

        racketMatrix = translate(mat4(1.0f), vec3(centerOfRacket.x + 0.8f * sin(radians(elbowAngle)), centerOfRacket.y - 0.8f * cos(radians(elbowAngle)), centerOfRacket.z));
        racketMatrix = rotate(racketMatrix, radians(elbowAngle), vec3(0.0f, 0.0f, 1.0f));

        partMatrix = racketMatrix * partMatrix;
        worldMatrix = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f)) * armMatrix * partMatrix;
         
        // First draw the base
        SetUniformMat4(shader, "world_matrix", worldMatrix);
        SetUniformVec3(shader, "object_color", vec3(0.643f, 0.455f, 0.286f));
        glDrawArrays(renderMode, 0, 36);

        // Do the sides of the racket now
        partMatrix = scale(mat4(1.0f), vec3(0.2f, 1.8f, 0.2f));

        racketMatrix = translate(mat4(1.0f), vec3(centerOfRacket.x - 0.5f * cos(radians(elbowAngle)), centerOfRacket.y - 0.5f * sin(radians(elbowAngle)), centerOfRacket.z));
        racketMatrix = rotate(racketMatrix, radians(elbowAngle), vec3(0.0f, 0.0f, 1.0f));

        partMatrix = racketMatrix * partMatrix;
        worldMatrix = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f)) * armMatrix * partMatrix;

        // Draw the left side of the racket
        SetUniformMat4(shader, "world_matrix", worldMatrix);
        SetUniformVec3(shader, "object_color", vec3(0.643f, 0.455f, 0.286f));
        glDrawArrays(renderMode, 0, 36);

        partMatrix = scale(mat4(1.0f), vec3(0.2f, 1.8f, 0.2f));

        racketMatrix = translate(mat4(1.0f), vec3(centerOfRacket.x + 0.5f * cos(radians(elbowAngle)), centerOfRacket.y + 0.5f * sin(radians(elbowAngle)), centerOfRacket.z));
        racketMatrix = rotate(racketMatrix, radians(elbowAngle), vec3(0.0f, 0.0f, 1.0f));

        partMatrix = racketMatrix * partMatrix;
        worldMatrix = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f)) * armMatrix * partMatrix;

        // Draw the right side of the racket
        SetUniformMat4(shader, "world_matrix", worldMatrix);
        SetUniformVec3(shader, "object_color", vec3(0.643f, 0.455f, 0.286f));
        glDrawArrays(renderMode, 0, 36);

        // Finish with the top
        partMatrix = scale(mat4(1.0f), vec3(1.2f, 0.2f, 0.2f));

        racketMatrix = translate(mat4(1.0f), vec3(centerOfRacket.x - 0.8f * sin(radians(elbowAngle)), centerOfRacket.y + 0.8f * cos(radians(elbowAngle)), centerOfRacket.z));
        racketMatrix = rotate(racketMatrix, radians(elbowAngle), vec3(0.0f, 0.0f, 1.0f));

        partMatrix = racketMatrix * partMatrix;
        worldMatrix = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f)) * armMatrix * partMatrix;

        // Draw the top of the racket
        SetUniformMat4(shader, "world_matrix", worldMatrix);
        SetUniformVec3(shader, "object_color", vec3(0.643f, 0.455f, 0.286f));
        glDrawArrays(renderMode, 0, 36);

        // Draw the net of the racket

        for (int i = 0; i < 7; i++)
        {
            partMatrix = scale(mat4(1.0f), vec3(0.05f, 1.6f, 0.05f));
            racketMatrix = translate(mat4(1.0f), vec3(centerOfRacket.x - (0.3f * cos(radians(elbowAngle))) + 0.1f * i * cos(radians(elbowAngle)), centerOfRacket.y - (0.3f * sin(radians(elbowAngle))) + 0.1f * i * sin(radians(elbowAngle)), centerOfRacket.z));
            racketMatrix = rotate(racketMatrix, radians(elbowAngle), vec3(0.0f, 0.0f, 1.0f));

            partMatrix = racketMatrix * partMatrix;
            worldMatrix = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f)) * armMatrix * partMatrix;

            // Draw the vertical string
            SetUniformMat4(shader, "world_matrix", worldMatrix);
            SetUniformVec3(shader, "object_color", vec3(0.0f, 0.0f, 0.0f));
            glDrawArrays(renderMode, 0, 36);
        }

        for (int i = 0; i < 14; i++)
        {
            partMatrix = scale(mat4(1.0f), vec3(1.0f, 0.05f, 0.05f));
            racketMatrix = translate(mat4(1.0f), vec3(centerOfRacket.x + (0.7f * sin(radians(elbowAngle))) - 0.1f * i * sin(radians(elbowAngle)), centerOfRacket.y - (0.7f * cos(radians(elbowAngle))) + 0.1f * i * cos(radians(elbowAngle)), centerOfRacket.z));
            racketMatrix = rotate(racketMatrix, radians(elbowAngle), vec3(0.0f, 0.0f, 1.0f));

            partMatrix = racketMatrix * partMatrix;
            worldMatrix = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f)) * armMatrix * partMatrix;

            // Draw the horizontal string
            SetUniformMat4(shader, "world_matrix", worldMatrix);
            SetUniformVec3(shader, "object_color", vec3(0.0f, 0.0f, 0.0f));
            glDrawArrays(renderMode, 0, 36);
        }
    }
};

void drawScoreboard(GLuint shader, int p1Score, int p2Score)
{
    mat4 scoreBoardMatrix = scale(mat4(1.0f), vec3(0.2f, 3.0f, 5.0f));
    mat4 location = translate(mat4(1.0f), vec3(-0.5f, 10.0f, 0.0f));
    location = rotate(location, radians(30.0f), vec3(0.0f, 0.0f, 1.0f));

    mat4 worldMatrix = location * scoreBoardMatrix;
    // Draw the horizontal string
    SetUniformMat4(shader, "world_matrix", worldMatrix);
    SetUniformVec3(shader, "object_color", vec3(0.0f, 0.0f, 0.0f));
    glDrawArrays(GL_TRIANGLES, 0, 36);
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Step3 Load Textures with dimension data
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (stbi_failure_reason())
        std::cout << stbi_failure_reason();
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

bool CheckCollision(vec3 ballPosition, vec3 racketPosition)
{
    // get difference vector between both centers
    vec3 difference = ballPosition - racketPosition;
    vec3 clamped = clamp(difference, vec3(-0.1f, -0.9f, -0.6f), vec3(0.1f, 0.9f, 0.6f));
    // add clamped value to center of racket to find the position closest to the ball
    vec3 closest = racketPosition + clamped;
    // retrieve vector between center circle and closest point AABB and check if length <= radius
    difference = closest - ballPosition;
    return length(difference) < 0.5f;
}

// 3d model VBO (lab 8)
GLuint setupModelVBO(string path, int& vertexCount) {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> UVs;

    // read the vertex data from the model's OBJ file
    loadOBJ(path.c_str(), vertices, normals, UVs);

    GLuint VAO3d;
    glGenVertexArrays(1, &VAO3d);
    glBindVertexArray(VAO3d);  // Becomes active VAO
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and
    // attribute pointer(s).

    // Vertex VBO setup
    GLuint vertices_VBO;
    glGenBuffers(1, &vertices_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
        &vertices.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
        (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Normals VBO setup
    GLuint normals_VBO;
    glGenBuffers(1, &normals_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3),
        &normals.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
        (GLvoid*)0);
    glEnableVertexAttribArray(1);

    // UVs VBO setup
    GLuint uvs_VBO;
    glGenBuffers(1, &uvs_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, uvs_VBO);
    glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs.front(),
        GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
        (GLvoid*)0);
    glEnableVertexAttribArray(2);

    
    // Additional attribute setup
    GLuint additionalAttr_VBO;
    glGenBuffers(1, &additionalAttr_VBO);
    //glBindBuffer(GL_ARRAY_BUFFER, additionalAttr_VBO);
    // Assuming your additional attribute is of type glm::vec3
    //glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),&vertices.front(), GL_STATIC_DRAW);
    //glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    //glEnableVertexAttribArray(3);
    

    glBindBuffer(GL_ARRAY_BUFFER, 0); // VAO already stored the state we just defined, safe to unbind buffer

    glBindVertexArray(0);
    // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent
    // strange bugs, as we are using multiple VAOs)
    vertexCount = vertices.size();
    return VAO3d;
}

int main(int argc, char* argv[])
{
    // Initialize GLFW and OpenGL version
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create Window and rendering context using GLFW, resolution is 1024x786
    GLFWwindow* window = glfwCreateWindow(1024, 786, "Comp371 - Quiz 2", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Green background
    glClearColor(0.184f, 0.310f, 0.310f, 1.0f);

    // Load Textures
    GLuint glossTextureID = loadTexture("code/assets/textures/gloss.jpg");
    GLuint whiteTextureID = loadTexture("code/assets/textures/white.jpg");
    GLuint lTextureID = loadTexture("code/assets/textures/water.jpg");
    GLuint iTextureID = loadTexture("code/assets/textures/fire.jpg");
    GLuint aTextureID = loadTexture("code/assets/textures/lightning.jpg");
    GLuint nTextureID = loadTexture("code/assets/textures/grass.jpg");
    GLuint metalicTextureID = loadTexture("code/assets/textures/metal.jpg");
    GLuint tatooTextureID = loadTexture("code/assets/textures/tatoo.jpg");
    GLuint ropeTextureID = loadTexture("code/assets/textures/rope.jpg");
    GLuint tennisBallTextureID = loadTexture("code/assets/textures/tennisball.jpg");

    // Load shaders
    string shaderPathPrefix = "code/assets/shaders/";
    GLuint shaderScene = loadSHADER(shaderPathPrefix + "scene_vertex.glsl", shaderPathPrefix + "scene_fragment.glsl");
    GLuint shaderShadow = loadSHADER(shaderPathPrefix + "shadow_vertex.glsl", shaderPathPrefix + "shadow_fragment.glsl");
    GLuint shaderSkybox = loadSHADER(shaderPathPrefix + "skybox_vertex.glsl", shaderPathPrefix + "skybox_fragment.glsl");

    // Begin music
    ISoundEngine* SoundEngine = createIrrKlangDevice();
    SoundEngine->play2D("code/assets/audio/themeSong.mp3", true);

    // Used to trigger specific flags in the scene shader
    GLuint textureFlag = glGetUniformLocation(shaderScene, "useTexture"); // Determines if textures need to be turned on
    GLint p_textureArray[1]{};
    GLint* currentTextureValue = p_textureArray;

    GLuint shadowFlag = glGetUniformLocation(shaderScene, "useShadow"); // Determines if shadows need to be turned on
    GLint p_shadowArray[1]{};
    GLint* currentShadowValue = p_shadowArray;

    GLuint zSpotlightFlag = glGetUniformLocation(shaderScene, "useSpotlightZ"); // Determines if the spotlight infront of the model is turned on
    GLint p_zSpotlightArray[1]{};
    GLint* currentZSpotlightValue = p_zSpotlightArray;

    GLuint ySpotlightFlag = glGetUniformLocation(shaderScene, "useSpotlightY"); // Determines if the spotlight fixed above on the y axis is turned on
    GLint p_ySpotlightArray[1]{};
    GLint* currentYSpotlightValue = p_ySpotlightArray;

    // Setup texture and framebuffer for creating shadow map

    // Dimensions of the shadow texture, which should cover the viewport window size and shouldn't be oversized and waste resources
    const unsigned int DEPTH_MAP_TEXTURE_SIZE = 1024;

    // Variable storing index to texture used for shadow mapping
    GLuint depth_map_texture;

    // Get the texture
    glGenTextures(1, &depth_map_texture);

    // Bind the texture so the next glTex calls affect it
    glBindTexture(GL_TEXTURE_2D, depth_map_texture);

    // Create the texture and specify it's attributes, including widthn height, components (only depth is stored, no color information)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, DEPTH_MAP_TEXTURE_SIZE, DEPTH_MAP_TEXTURE_SIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    // Set texture sampler parameters.
    // The two calls below tell the texture sampler inside the shader how to upsample and downsample the texture. Here we choose the nearest filtering option, which means we just use the value of the closest pixel to the chosen image coordinate.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // The two calls below tell the texture sampler inside the shader how it should deal with texture coordinates outside of the [0, 1] range. Here we decide to just tile the image.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    float borderColor[] = { 1.0f,1.0f,1.0f,1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // Variable storing index to framebuffer used for shadow mapping
    GLuint depth_map_fbo;  // fbo: framebuffer object

    // Get the framebuffer
    glGenFramebuffers(1, &depth_map_fbo);

    // Bind the framebuffer so the next glFramebuffer calls affect it
    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);

    // Attach the depth map texture to the depth map framebuffer
    //glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depth_map_texture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map_texture, 0);
    glDrawBuffer(GL_NONE); //disable rendering colors, only write depth values
    glDrawBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Skybox textures
    vector<string> faces
    {
        "code/assets/textures/spaceright.jpg",
        "code/assets/textures/spaceleft.jpg",
        "code/assets/textures/spacetop.jpg",
        "code/assets/textures/spacebottom.jpg",
        "code/assets/textures/spacefront.jpg",
        "code/assets/textures/spaceback.jpg"
    };
    unsigned int cubemapTexture = loadCubemap(faces);

    // Camera parameters for view transform
    vec3 cameraPosition(0.0f, 6.0f, 30.0f);
    vec3 cameraLookAt(0.0f, -6.0f, -30.0f);
    vec3 cameraUp(0.0f, 1.0f, 0.0f);

    // Spinning spotlight positioning
    vec3 circleCamera(0.0f, 15.0f, 30.0f);
    vec3 circleFocus(0.0f, 0.0f, 0.0f);
    vec3 circleLookAt = circleFocus - circleCamera;
    float circleCameraAngle = 0.0f;

    // Other camera parameters
    float cameraHorizontalAngle = 90.0f;
    float cameraVerticalAngle = 0.0f;

    // Used to modify the FOV for zooming in or out
    float FOV = 70.6f;

    // Used to keep track of moving the camera lookat at a specific position
    vec3 viewChange = vec3(0.0f, 0.0f, 0.0f);

    // Set initial view matrix
    mat4 viewMatrix = lookAt(cameraPosition,  // eye
        cameraPosition + cameraLookAt + viewChange,  // center, this is 0.0, 0.0, 0.0 just needed to eqaulize the camera position
        cameraUp); // up

    // Set View and Projection matrices on both shaders
    SetUniformMat4(shaderScene, "view_matrix", viewMatrix);
    SetUniformVec3(shaderScene, "view_position", cameraPosition);

    // light parameters
    vec3 lightPosition = vec3(0.0f, 30.0f, 0.0f); // the location of the point light in 3D space
    vec3 pointLightFocus(0.0, 0.0f, -1.0f); // The location the light is "looking" at
    float lightNearPlane = 1.0f;
    float lightFarPlane = 120.0f;

    // Set light color on scene shader
    SetUniformVec3(shaderScene, "light_color", vec3(1.0f, 1.0f, 1.0f));

    // Define and upload geometry to the GPU here ...
    int vao = createTexturedCubeVertexArrayObject();
    int sphereVAO = createSphereObject();

    // Prepare the skybox vertex array object
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // For frame time
    double lastFrameTime = glfwGetTime();
    int lastMouseLeftState = GLFW_RELEASE;
    double lastMousePosX, lastMousePosY;
    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

    // Other OpenGL states to set once
    glEnable(GL_CULL_FACE);

    // Enable Depth Test
    glEnable(GL_DEPTH_TEST);

    // Enable transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Intial angle of the world
    float worldXAngle = 0.0f;
    float worldYAngle = 0.0f;

    // Default rendering mode
    GLenum renderMode = GL_TRIANGLES;

    // Initilize variables to allow holding the button to not spam the button
    int previousXstate = GLFW_RELEASE;
    int previousBstate = GLFW_RELEASE;
    int previousLstate = GLFW_RELEASE;
    int previousKstate = GLFW_RELEASE;


    // Player poisition reference and initial positions
    vec3* playerPosition = NULL;
    Racket* currentRacket = NULL;
    vec3 playerOneLoc = vec3(-16.0f, 1.3f, 0.0f);
    vec3 playerTwoLoc = vec3(16.0f, 1.3f, 0.0f);
    vec3 ballLocation = vec3(0.0f, 6.0f, 0.0f);
    vec3 ballColor = vec3(0.224f, 1.0f, 0.078f);

    float acceleration = 1.0f;
    // Randomize the initial direction the ball will move in
    srand(time(0));
    float xdir;
    if (rand() % 2 == 1)
    {
        xdir = ((rand() % 3) + 3.0f) / 10;
        playerPosition = (&playerTwoLoc);
        
    }
    else
    {
        xdir = -((rand() % 3) + 3.0f) / 10;
        playerPosition = (&playerOneLoc);
    }
    float ydir = 0.0f;
    float zdir = ((rand() % 10) - 5.0f) / 10;
    vec3 zLightFocus(0.0, 0.0f, -1.0f);      // the point in 3D space the light "looks" at

    // Player colors, contains the possible colors for the player letter
    vec3 playerColors[4][3] = { {vec3(0.0f, 0.0f, 0.5f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 1.0f)},
                               {vec3(0.8235f, 0.0157f, 0.176f), vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.25f, 0.25f)},
                               {vec3(1.0f, 1.0f, 0.25f), vec3(1.0f, 1.0f, 0.0f), vec3(1.0f, 0.753f, 0.0f)},
                               {vec3(0.059f, 1.0f, 0.314f), vec3(0.0f, 0.5f, 0.0f), vec3(0.18f, 0.545f, 0.341f)} };

    
    //intital arm elbow racket, initially planned for some features that I didnt have time to finish
    float armAngles[2][4] = { {0.0f, 90.0f, 0.0f, 0.0f},
                              {0.0f, 90.0f, 0.0f, 0.0f} };
    
    double mousePosX, mousePosY;
    bool sideCamera = false;

    // Setup 3d models (lab 8)
    string fenceModel = "code/assets/3dModels/fence.obj";

    // 3d models vertices (lab 8)
    int fenceVertices;

    // setup VBO (lab 8)
    GLuint fenceVAO = setupModelVBO(fenceModel, fenceVertices);

    // Entering Main Loop
    while (!glfwWindowShouldClose(window))
    {
        // Frame time calculation
        double dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;

        mat4 lightProjectionMatrix = perspective(radians(90.0f), (float)DEPTH_MAP_TEXTURE_SIZE / (float)DEPTH_MAP_TEXTURE_SIZE, lightNearPlane, lightFarPlane);
        mat4 lightViewMatrix = lookAt(lightPosition, pointLightFocus, vec3(0.0f, 0.0f, 1.0f));
        mat4 lightSpaceMatrix = lightProjectionMatrix * lightViewMatrix;

        // Set the view for the second rotating camera
        circleCamera.x = sin(radians(circleCameraAngle)) * 30.0f;
        circleCamera.z = cos(radians(circleCameraAngle)) * 30.0f;
        circleLookAt = circleFocus - circleCamera;

        // Bounce on the invisible z walls and the set y limit
        if ((ballLocation.y < 0.5f) | (ballLocation.y > 10.0f))
        {
            ydir *= -1.0f;
        }
        if ((ballLocation.z < -17.5f) | (ballLocation.z > 17.5f))
        {
            zdir *= -1.0f;
        }

        // This is the actual movement of the focus of the spotlight
        ballLocation.x += xdir * dt * 5.0f * acceleration;
        ballLocation.y += ydir * dt * 5.0f * acceleration;
        ballLocation.z += zdir * dt * 5.0f * acceleration;

        vec3 zLightPosition = vec3(0.0f, 5.0f, 30.0f); // the location of the light in 3D space
        vec3 zLightDirection = normalize(ballLocation - zLightPosition);
        vec3 zSpotlightColor = vec3(1.0f, 1.0f, 0.0f);
        SetUniformVec3(shaderScene, "zSpotLight_pos", zLightPosition);
        SetUniformVec3(shaderScene, "zSpotLight_dir", zLightDirection);
        SetUniformVec3(shaderScene, "zSpotLight_color", zSpotlightColor);

        vec3 yLightPosition = circleCamera; // the location of the y spotlight in 3D space
        vec3 yLightFocus = circleLookAt;      // the point in 3D space the spotlight "looks" at
        vec3 yLightDirection = normalize(yLightFocus - yLightPosition);
        vec3 ySpotlightColor = vec3(1.0f, 1.0f, 0.0f); // Set the color of the spotlight to something different
        SetUniformVec3(shaderScene, "ySpotLight_pos", yLightPosition);
        SetUniformVec3(shaderScene, "ySpotLight_dir", yLightDirection);
        SetUniformVec3(shaderScene, "ySpotLight_color", ySpotlightColor);

        // Set light space matrix on both shaders
        SetUniformMat4(shaderShadow, "light_view_proj_matrix", lightSpaceMatrix);
        SetUniformMat4(shaderScene, "light_view_proj_matrix", lightSpaceMatrix);
 
        // Set light cutoff angles on scene shader
        float lightAngleOuter = 7.5f;
        float lightAngleInner = 5.0f;
        SetUniformfValue(shaderScene, "light_cutoff_inner", cos(radians(lightAngleInner)));
        SetUniformfValue(shaderScene, "light_cutoff_outer", cos(radians(lightAngleOuter)));

        // Set light far and near planes on scene shader
        SetUniformfValue(shaderScene, "light_near_plane", lightNearPlane);
        SetUniformfValue(shaderScene, "light_far_plane", lightFarPlane);

        // Set light position on scene shader
        SetUniformVec3(shaderScene, "pointLight_Pos", lightPosition);

        // Set the view matrix
        mat4 viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt + viewChange, cameraUp);
        SetUniformMat4(shaderScene, "view_matrix", viewMatrix);
        SetUniformVec3(shaderScene, "view_position", cameraPosition);

        // First pass
        {
            // Use proper shader
            glUseProgram(shaderShadow);
            // Use proper image output size
            glViewport(0, 0, DEPTH_MAP_TEXTURE_SIZE, DEPTH_MAP_TEXTURE_SIZE);
            // Bind depth map texture as output framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
            // Clear depth data on the framebuffer
            glClear(GL_DEPTH_BUFFER_BIT);

            // ================================================================ //
            //						Draw 3D Models (lab 8)						//
            // ================================================================ //

            glBindVertexArray(fenceVAO);    // bind
            //glBindTexture(GL_TEXTURE_2D, whiteTextureID);   // set fence texture

            // "left" fence
            mat4 fenceLeft = translate(mat4(1.0f), vec3(0.0f, 0.0f, 30.0f)) *
                scale(mat4(1.0f), vec3(0.15f, 0.04f, 0.1f));
            SetUniformMat4(shaderShadow, "world_matrix", fenceLeft);
            glDrawArrays(GL_TRIANGLES, 0, fenceVertices);

            // "right" fence
            mat4 fenceRight = translate(mat4(1.0f), vec3(0.0f, 0.0f, -30.0f)) *
                scale(mat4(1.0f), vec3(0.15f, 0.04f, 0.1f));
            SetUniformMat4(shaderShadow, "world_matrix", fenceRight);
            glDrawArrays(GL_TRIANGLES, 0, fenceVertices);

            // "back" fence
            mat4 fenceBack = translate(mat4(1.0f), vec3(-53.0f, 0.0f, 0.0f)) *
                rotate(mat4(1.0f), glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f)) *
                scale(mat4(1.0f), vec3(0.08f, 0.04f, 0.1f));
            SetUniformMat4(shaderShadow, "world_matrix", fenceBack);
            glDrawArrays(GL_TRIANGLES, 0, fenceVertices);

            // "front" fence
            mat4 fenceFront = translate(mat4(1.0f), vec3(53.0f, 0.0f, 0.0f)) *
                rotate(mat4(1.0f), glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f)) *
                scale(mat4(1.0f), vec3(0.08f, 0.04f, 0.1f));
            SetUniformMat4(shaderShadow, "world_matrix", fenceFront);
            glDrawArrays(GL_TRIANGLES, 0, fenceVertices);


            //Draw tennis ball
            glBindVertexArray(sphereVAO);
            mat4 sphereSize = scale(mat4(1.0f), vec3(0.5f, 0.5f, 0.5f));
            mat4 sphereMatrix = translate(mat4(1.0f), vec3(ballLocation));
            sphereMatrix = sphereMatrix * sphereSize;

            mat4 spherePosistion = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f)) * sphereMatrix;
            SetUniformMat4(shaderShadow, "world_matrix", spherePosistion);
            glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);

            glBindVertexArray(vao);
            // Draw X axis, colored red
            mat4 xAxis = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            xAxis = xAxis * translate(mat4(1.0f), vec3(2.5f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(5.0f, 0.1f, 0.1f));
            SetUniformMat4(shaderShadow, "world_matrix", xAxis);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Draw Y axis, colored green
            mat4 yAxis = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            yAxis = yAxis * translate(mat4(1.0f), vec3(0.0f, 2.5f, 0.0f)) * scale(mat4(1.0f), vec3(0.1f, 5.0f, 0.1f));
            SetUniformMat4(shaderShadow, "world_matrix", yAxis);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Draw Z axis, colored blue
            mat4 zAxis = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            zAxis = zAxis * translate(mat4(1.0f), vec3(0.0f, 0.0f, 2.5f)) * scale(mat4(1.0f), vec3(0.1f, 0.1f, 5.0f));
            SetUniformMat4(shaderShadow, "world_matrix", zAxis);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Drawing the court a 78x36 surface
            mat4 court = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            court = court * translate(mat4(1.0f), vec3(0.0f, -0.1f, 0.0f)) * scale(mat4(1.0f), vec3(78.0f, 0.1f, 36.0f));
            SetUniformMat4(shaderShadow, "world_matrix", court);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Drawing the white lines on the court
            // Drawing the single + doubles sidelines
            mat4 lines = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            lines = lines * translate(mat4(1.0f), vec3(0.0f, -0.05f, -17.9f)) * scale(mat4(1.0f), vec3(78.0f, 0.1f, 0.2f));
            SetUniformMat4(shaderShadow, "world_matrix", lines);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            lines = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            lines = lines * translate(mat4(1.0f), vec3(0.0f, -0.05f, -13.4f)) * scale(mat4(1.0f), vec3(78.0f, 0.1f, 0.2f));
            SetUniformMat4(shaderShadow, "world_matrix", lines);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            lines = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            lines = lines * translate(mat4(1.0f), vec3(0.0f, -0.05f, 13.4f)) * scale(mat4(1.0f), vec3(78.0f, 0.1f, 0.2f));
            SetUniformMat4(shaderShadow, "world_matrix", lines);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            lines = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            lines = lines * translate(mat4(1.0f), vec3(0.0f, -0.05f, 17.9f)) * scale(mat4(1.0f), vec3(78.0f, 0.1f, 0.2f));
            SetUniformMat4(shaderShadow, "world_matrix", lines);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Drawing the baselines + service lines
            lines = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            lines = lines * translate(mat4(1.0f), vec3(-38.9f, -0.05f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.1f, 36.0f));
            SetUniformMat4(shaderShadow, "world_matrix", lines);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            lines = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            lines = lines * translate(mat4(1.0f), vec3(-21.0f, -0.05f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.1f, 27.0f));
            SetUniformMat4(shaderShadow, "world_matrix", lines);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            lines = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            lines = lines * translate(mat4(1.0f), vec3(21.0f, -0.05f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.1f, 27.0f));
            SetUniformMat4(shaderShadow, "world_matrix", lines);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            lines = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            lines = lines * translate(mat4(1.0f), vec3(38.9f, -0.05f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.1f, 36.0f));
            SetUniformMat4(shaderShadow, "world_matrix", lines);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Drawing the center service line
            lines = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            lines = lines * translate(mat4(1.0f), vec3(0.0f, -0.05f, 0.0f)) * scale(mat4(1.0f), vec3(42.0f, 0.1f, 0.2f));
            SetUniformMat4(shaderShadow, "world_matrix", lines);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Drawing the net, first the posts
            mat4 netPost = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            netPost = netPost * translate(mat4(1.0f), vec3(0.0f, 2.0f, -19.0f)) * scale(mat4(1.0f), vec3(0.2f, 4.0f, 0.2f));
            SetUniformMat4(shaderShadow, "world_matrix", netPost);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            netPost = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            netPost = netPost * translate(mat4(1.0f), vec3(0.0f, 2.0f, 19.0f)) * scale(mat4(1.0f), vec3(0.2f, 4.0f, 0.2f));
            SetUniformMat4(shaderShadow, "world_matrix", netPost);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            netPost = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            netPost = netPost * translate(mat4(1.0f), vec3(0.0f, 2.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 4.0f, 0.2f));
            SetUniformMat4(shaderShadow, "world_matrix", netPost);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Now draw the net
            mat4 net = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            net = net * translate(mat4(1.0f), vec3(0.0f, 4.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.4f, 0.4f, 38.0f));
            SetUniformMat4(shaderShadow, "world_matrix", net);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            for (int i = 0; i < 39; ++i)
            {
                // Draw vertical lines for the net
                net = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
                net = net * translate(mat4(1.0f), vec3(0.0f, 2.1f, -19.5f + 0.5f * i)) * scale(mat4(1.0f), vec3(0.1f, 3.8f, 0.1f));
                SetUniformMat4(shaderShadow, "world_matrix", net);
                glDrawArrays(GL_TRIANGLES, 0, 36);

                net = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
                net = net * translate(mat4(1.0f), vec3(0.0f, 2.1f, 0.0f + 0.5f * i)) * scale(mat4(1.0f), vec3(0.1f, 3.8f, 0.1f));
                SetUniformMat4(shaderShadow, "world_matrix", net);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            for (int i = 0; i < 7; ++i)
            {
                // Draw vertical lines for the net
                net = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
                net = net * translate(mat4(1.0f), vec3(0.0f, 0.2f + 0.5429f * i, 0.0f)) * scale(mat4(1.0f), vec3(0.1f, 0.1f, 38.0f));
                SetUniformMat4(shaderShadow, "world_matrix", net);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            // Draw the racket where player 1 is
            Racket playerOne = Racket(worldXAngle, worldYAngle, playerOneLoc, shaderShadow, renderMode, 1, armAngles[0][0], armAngles[0][1], armAngles[0][2], armAngles[0][3], tatooTextureID, metalicTextureID, false);

            // Draw the letter l, bound to player 1 position
            mat4 lCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            lCharacter = lCharacter * translate(mat4(1.0f), playerOneLoc) * translate(mat4(1.0f), vec3(0.0f, 1.6f, 0.0f)) * translate(mat4(1.0f), playerOne.centerOfRacket);
            lCharacter = rotate(lCharacter, radians(armAngles[0][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f));
            lCharacter = translate(lCharacter, vec3(0.0f, 0.0f, -0.2f)) * scale(mat4(1.0f), vec3(0.2f, 0.2f, 0.5f));
            SetUniformMat4(shaderShadow, "world_matrix", lCharacter);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            lCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            lCharacter = lCharacter * translate(mat4(1.0f), playerOneLoc) * translate(mat4(1.0f), playerOne.centerOfRacket) * translate(mat4(1.0f), vec3(0.0f, 1.875f, 0.0f));
            lCharacter = rotate(lCharacter, radians(armAngles[0][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f));
            lCharacter = translate(lCharacter, vec3(0.0f, 0.0f, -0.35f)) * scale(mat4(1.0f), vec3(0.2f, 0.75f, 0.2f));
            SetUniformMat4(shaderShadow, "world_matrix", lCharacter);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Draw the letter i, bound to player 1 position
            mat4 iCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            iCharacter = iCharacter * translate(mat4(1.0f), playerOneLoc) * translate(mat4(1.0f), playerOne.centerOfRacket) * translate(mat4(1.0f), vec3(0.0f, 1.875f, 0.0f));
            iCharacter = rotate(iCharacter, radians(armAngles[0][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f));
            iCharacter = translate(iCharacter, vec3(0.0f, 0.0f, 0.3f)) * scale(mat4(1.0f), vec3(0.2f, 0.75f, 0.2f));
            SetUniformMat4(shaderShadow, "world_matrix", iCharacter);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Draw the racket where player 2 is
            Racket playerTwo = Racket(worldXAngle, worldYAngle, playerTwoLoc, shaderShadow, renderMode, 2, armAngles[1][0], armAngles[1][1], armAngles[1][2], armAngles[1][3], tatooTextureID, metalicTextureID, false);

            // Draw the letter a, bound to player 2 position
            mat4 aCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            aCharacter = aCharacter * translate(mat4(1.0f), playerTwoLoc) * translate(mat4(1.0f), playerTwo.centerOfRacket) * translate(mat4(1.0f), vec3(0.0f, 1.875f, 0.0f));
            aCharacter = rotate(aCharacter, radians(armAngles[1][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f));
            aCharacter = translate(aCharacter, vec3(0.0f, 0.0f, 0.2f)) * scale(mat4(1.0f), vec3(0.2f, 0.75f, 0.2f));
            SetUniformMat4(shaderShadow, "world_matrix", aCharacter);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            aCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            aCharacter = aCharacter * translate(mat4(1.0f), playerTwoLoc) * translate(mat4(1.0f), playerTwo.centerOfRacket) * translate(mat4(1.0f), vec3(0.0f, 1.875f, 0.0f));
            aCharacter = rotate(aCharacter, radians(armAngles[1][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f));
            aCharacter = translate(aCharacter, vec3(0.0f, 0.0f, 0.6f)) * scale(mat4(1.0f), vec3(0.2f, 0.75f, 0.2f));
            SetUniformMat4(shaderShadow, "world_matrix", aCharacter);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            aCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            aCharacter = aCharacter * translate(mat4(1.0f), playerTwoLoc) * translate(mat4(1.0f), playerTwo.centerOfRacket) * translate(mat4(1.0f), vec3(0.0f, 2.15f, 0.0f));
            aCharacter = rotate(aCharacter, radians(armAngles[1][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f));
            aCharacter = translate(aCharacter, vec3(0.0f, 0.0f, 0.35f)) * scale(mat4(1.0f), vec3(0.2f, 0.2f, 0.5f));
            SetUniformMat4(shaderShadow, "world_matrix", aCharacter);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            aCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            aCharacter = aCharacter * translate(mat4(1.0f), playerTwoLoc) * translate(mat4(1.0f), playerTwo.centerOfRacket) * translate(mat4(1.0f), vec3(0.0f, 1.8f, 0.0f));
            aCharacter = rotate(aCharacter, radians(armAngles[1][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f));
            aCharacter = translate(aCharacter, vec3(0.0f, 0.0f, 0.35f)) * scale(mat4(1.0f), vec3(0.2f, 0.2f, 0.5f));
            SetUniformMat4(shaderShadow, "world_matrix", aCharacter);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Draw the letter n, bound to player 2 position
            mat4 nCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            nCharacter = nCharacter * translate(mat4(1.0f), playerTwoLoc) * translate(mat4(1.0f), playerTwo.centerOfRacket) * translate(mat4(1.0f), vec3(0.0f, 1.875f, 0.0f));
            nCharacter = rotate(nCharacter, radians(armAngles[1][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f));
            nCharacter = translate(nCharacter, vec3(0.0f, 0.0f, -0.55f)) * scale(mat4(1.0f), vec3(0.2f, 0.75f, 0.2f));
            SetUniformMat4(shaderShadow, "world_matrix", nCharacter);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            nCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            nCharacter = nCharacter * translate(mat4(1.0f), playerTwoLoc) * translate(mat4(1.0f), playerTwo.centerOfRacket) * translate(mat4(1.0f), vec3(0.0f, 1.875f, 0.0f));
            nCharacter = rotate(nCharacter, radians(armAngles[1][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f));
            nCharacter = translate(nCharacter, vec3(0.0f, 0.0f, -0.35f));
            nCharacter = rotate(nCharacter, radians(30.0f), vec3(1.0f, 0.0, 0.0)) * scale(mat4(1.0f), vec3(0.2f, 0.8f, 0.2f));
            SetUniformMat4(shaderShadow, "world_matrix", nCharacter);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            nCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            nCharacter = nCharacter * translate(mat4(1.0f), playerTwoLoc) * translate(mat4(1.0f), playerTwo.centerOfRacket) * translate(mat4(1.0f), vec3(0.0f, 1.875f, 0.0f));
            nCharacter = rotate(nCharacter, radians(armAngles[1][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f));
            nCharacter = translate(nCharacter, vec3(0.0f, 0.0f, -0.1f)) * scale(mat4(1.0f), vec3(0.2f, 0.75f, 0.2f));
            SetUniformMat4(shaderShadow, "world_matrix", nCharacter);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        drawScoreboard(shaderShadow, 0, 0);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Second pass
        // Use proper shader
        glUseProgram(shaderScene);

        glActiveTexture(GL_TEXTURE1);
        GLuint textureLocation = glGetUniformLocation(shaderScene, "textureSampler");
        glUniform1i(textureLocation, 1);                // Set our Texture sampler to user Texture Unit 1

        // Use proper image output size
        // Side note: we get the size from the framebuffer instead of using WIDTH and HEIGHT because of a bug with highDPI displays
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        // Set projection matrix for shader, this won't change
        mat4 projectionMatrix = perspective(FOV,            // field of view in degrees
            (float)width / (float)height,  // aspect ratio
            0.01f, 100.0f);   // near and far (near > 0)
        SetUniformMat4(shaderScene, "projection_matrix", projectionMatrix);

        // Bind screen as output framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // Clear color and depth data on framebuffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // ================================================================ //
        //						Draw 3D Models (lab 8)						//
        // ================================================================ //
       
        glBindVertexArray(fenceVAO);    // bind
        glBindTexture(GL_TEXTURE_2D, iTextureID);   // set fence texture

        // "left" fence
        mat4 fenceLeft = translate(mat4(1.0f), vec3(0.0f, 0.0f, 30.0f))*
            scale(mat4(1.0f), vec3(0.15f, 0.04f, 0.1f));
        SetUniformMat4(shaderScene, "world_matrix", fenceLeft);
        SetUniformVec3(shaderScene, "object_color", vec3(1.0f, 1.0f, 1.0f));
        glDrawArrays(GL_TRIANGLES, 0, fenceVertices);

        // "right" fence
        mat4 fenceRight = translate(mat4(1.0f), vec3(0.0f, 0.0f, -30.0f)) *
            scale(mat4(1.0f), vec3(0.15f, 0.04f, 0.1f));
        SetUniformMat4(shaderScene, "world_matrix", fenceRight);
        SetUniformVec3(shaderScene, "object_color", vec3(1.0f, 1.0f, 1.0f));
        glDrawArrays(GL_TRIANGLES, 0, fenceVertices);

        // "back" fence
        mat4 fenceBack = translate(mat4(1.0f), vec3(-53.0f, 0.0f, 0.0f)) *
            rotate(mat4(1.0f), glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(0.08f, 0.04f, 0.1f));
        SetUniformMat4(shaderScene, "world_matrix", fenceBack);
        SetUniformVec3(shaderScene, "object_color", vec3(1.0f, 1.0f, 1.0f));
        glDrawArrays(GL_TRIANGLES, 0, fenceVertices);

        // "front" fence
        mat4 fenceFront = translate(mat4(1.0f), vec3(53.0f, 0.0f, 0.0f)) *
            rotate(mat4(1.0f), glm::radians(90.0f), vec3(0.0f, 1.0f, 0.0f)) *
            scale(mat4(1.0f), vec3(0.08f, 0.04f, 0.1f));
        SetUniformMat4(shaderScene, "world_matrix", fenceFront);
        SetUniformVec3(shaderScene, "object_color", vec3(1.0f, 1.0f, 1.0f));
        glDrawArrays(GL_TRIANGLES, 0, fenceVertices);

        // Draw tennis ball
        glBindVertexArray(sphereVAO);
        glBindTexture(GL_TEXTURE_2D, tennisBallTextureID);
        mat4 sphereSize = scale(mat4(1.0f), vec3(0.5f, 0.5f, 0.5f));
        mat4 sphereMatrix = translate(mat4(1.0f), ballLocation);
        sphereMatrix = sphereMatrix * sphereSize;

        mat4 spherePosistion = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f)) * sphereMatrix;
        SetUniformMat4(shaderScene, "world_matrix", spherePosistion);
        SetUniformVec3(shaderScene, "object_color", vec3(ballColor));
        glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);

        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D, whiteTextureID);
        // Draw X axis, colored red
        mat4 xAxis = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        xAxis = xAxis * translate(mat4(1.0f), vec3(2.5f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(5.0f, 0.1f, 0.1f));
        SetUniformMat4(shaderScene, "world_matrix", xAxis);
        SetUniformVec3(shaderScene, "object_color", vec3(1.0f, 0.0f, 0.0f));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Draw Y axis, colored green
        mat4 yAxis = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        yAxis = yAxis * translate(mat4(1.0f), vec3(0.0f, 2.5f, 0.0f)) * scale(mat4(1.0f), vec3(0.1f, 5.0f, 0.1f));
        SetUniformMat4(shaderScene, "world_matrix", yAxis);
        SetUniformVec3(shaderScene, "object_color", vec3(0.0f, 1.0f, 0.0f));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Draw Z axis, colored blue
        mat4 zAxis = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        zAxis = zAxis * translate(mat4(1.0f), vec3(0.0f, 0.0f, 2.5f)) * scale(mat4(1.0f), vec3(0.1f, 0.1f, 5.0f));
        SetUniformMat4(shaderScene, "world_matrix", zAxis);
        SetUniformVec3(shaderScene, "object_color", vec3(0.0f, 0.0f, 1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Drawing the court a 78x36 surface
        mat4 court = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        court = court * translate(mat4(1.0f), vec3(0.0f, -0.1f, 0.0f)) * scale(mat4(1.0f), vec3(78.0f, 0.1f, 36.0f));
        SetUniformMat4(shaderScene, "world_matrix", court);
        SetUniformVec3(shaderScene, "object_color", vec3(0.0f, 0.522f, 0.4f));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Drawing the white lines on the court
        // Drawing the single + doubles sidelines
        mat4 lines = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        lines = lines * translate(mat4(1.0f), vec3(0.0f, -0.05f, -17.9f)) * scale(mat4(1.0f), vec3(78.0f, 0.1f, 0.2f));
        SetUniformMat4(shaderScene, "world_matrix", lines);
        SetUniformVec3(shaderScene, "object_color", vec3(1.0f, 1.0f, 1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lines = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        lines = lines * translate(mat4(1.0f), vec3(0.0f, -0.05f, -13.4f)) * scale(mat4(1.0f), vec3(78.0f, 0.1f, 0.2f));
        SetUniformMat4(shaderScene, "world_matrix", lines);
        SetUniformVec3(shaderScene, "object_color", vec3(1.0f, 1.0f, 1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lines = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        lines = lines * translate(mat4(1.0f), vec3(0.0f, -0.05f, 13.4f)) * scale(mat4(1.0f), vec3(78.0f, 0.1f, 0.2f));
        SetUniformMat4(shaderScene, "world_matrix", lines);
        SetUniformVec3(shaderScene, "object_color", vec3(1.0f, 1.0f, 1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lines = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        lines = lines * translate(mat4(1.0f), vec3(0.0f, -0.05f, 17.9f)) * scale(mat4(1.0f), vec3(78.0f, 0.1f, 0.2f));
        SetUniformMat4(shaderScene, "world_matrix", lines);
        SetUniformVec3(shaderScene, "object_color", vec3(1.0f, 1.0f, 1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Drawing the baselines + service lines
        lines = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        lines = lines * translate(mat4(1.0f), vec3(-38.9f, -0.05f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.1f, 36.0f));
        SetUniformMat4(shaderScene, "world_matrix", lines);
        SetUniformVec3(shaderScene, "object_color", vec3(1.0f, 1.0f, 1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lines = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        lines = lines * translate(mat4(1.0f), vec3(-21.0f, -0.05f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.1f, 27.0f));
        SetUniformMat4(shaderScene, "world_matrix", lines);
        SetUniformVec3(shaderScene, "object_color", vec3(1.0f, 1.0f, 1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lines = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        lines = lines * translate(mat4(1.0f), vec3(21.0f, -0.05f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.1f, 27.0f));
        SetUniformMat4(shaderScene, "world_matrix", lines);
        SetUniformVec3(shaderScene, "object_color", vec3(1.0f, 1.0f, 1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lines = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        lines = lines * translate(mat4(1.0f), vec3(38.9f, -0.05f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.1f, 36.0f));
        SetUniformMat4(shaderScene, "world_matrix", lines);
        SetUniformVec3(shaderScene, "object_color", vec3(1.0f, 1.0f, 1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Drawing the center service line
        lines = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        lines = lines * translate(mat4(1.0f), vec3(0.0f, -0.05f, 0.0f)) * scale(mat4(1.0f), vec3(42.0f, 0.1f, 0.2f));
        SetUniformMat4(shaderScene, "world_matrix", lines);
        SetUniformVec3(shaderScene, "object_color", vec3(1.0f, 1.0f, 1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Drawing the net, first the posts
        glBindTexture(GL_TEXTURE_2D, glossTextureID);
        mat4 netPost = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        netPost = netPost * translate(mat4(1.0f), vec3(0.0f, 2.0f, -19.0f)) * scale(mat4(1.0f), vec3(0.2f, 4.0f, 0.2f));
        SetUniformMat4(shaderScene, "world_matrix", netPost);
        SetUniformVec3(shaderScene, "object_color", vec3(0.5f, 0.5f, 0.5f));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        netPost = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        netPost = netPost * translate(mat4(1.0f), vec3(0.0f, 2.0f, 19.0f)) * scale(mat4(1.0f), vec3(0.2f, 4.0f, 0.2f));
        SetUniformMat4(shaderScene, "world_matrix", netPost);
        SetUniformVec3(shaderScene, "object_color", vec3(0.5f, 0.5f, 0.5f));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        netPost = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        netPost = netPost * translate(mat4(1.0f), vec3(0.0f, 2.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.2f, 4.0f, 0.2f));
        SetUniformMat4(shaderScene, "world_matrix", netPost);
        SetUniformVec3(shaderScene, "object_color", vec3(0.5f, 0.5f, 0.5f));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindTexture(GL_TEXTURE_2D, ropeTextureID);
        // Now draw the net
        mat4 net = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        net = net * translate(mat4(1.0f), vec3(0.0f, 4.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.4f, 0.4f, 38.5f));
        SetUniformMat4(shaderScene, "world_matrix", net);
        SetUniformVec3(shaderScene, "object_color", vec3(1.0f, 1.0f, 1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindTexture(GL_TEXTURE_2D, whiteTextureID);
        for (int i = 0; i < 39; ++i)
        {
            // Draw vertical lines for the net
            net = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            net = net * translate(mat4(1.0f), vec3(0.0f, 2.1f, -19.0f + 0.5f * i)) * scale(mat4(1.0f), vec3(0.1f, 3.8f, 0.1f));
            SetUniformMat4(shaderScene, "world_matrix", net);
            SetUniformVec3(shaderScene, "object_color", vec3(1.0f, 1.0f, 1.0f));
            glDrawArrays(GL_TRIANGLES, 0, 36);

            net = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            net = net * translate(mat4(1.0f), vec3(0.0f, 2.1f, 0.0f + 0.5f * i)) * scale(mat4(1.0f), vec3(0.1f, 3.8f, 0.1f));
            SetUniformMat4(shaderScene, "world_matrix", net);
            SetUniformVec3(shaderScene, "object_color", vec3(1.0f, 1.0f, 1.0f));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        for (int i = 0; i < 7; ++i)
        {
            // Draw vertical lines for the net
            net = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
            net = net * translate(mat4(1.0f), vec3(0.0f, 0.2f + 0.5429f * i, 0.0f)) * scale(mat4(1.0f), vec3(0.1f, 0.1f, 38.0f));
            SetUniformMat4(shaderScene, "world_matrix", net);
            SetUniformVec3(shaderScene, "object_color", vec3(1.0f, 1.0f, 1.0f));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Draw the two rackets at the specific spots the players should be
        Racket playerOne = Racket(worldXAngle, worldYAngle, playerOneLoc, shaderScene, renderMode, 1, armAngles[0][0], armAngles[0][1], armAngles[0][2], armAngles[0][3], tatooTextureID, metalicTextureID, true);
        Racket playerTwo = Racket(worldXAngle, worldYAngle, playerTwoLoc, shaderScene, renderMode, 2, armAngles[1][0], armAngles[1][1], armAngles[1][2], armAngles[1][3], tatooTextureID, metalicTextureID, true);
        
        // Change transparency
        SetUniformfValue(shaderScene, "transparency", 0.4f);

        // Draw the letter l, bound to player 1 position
        mat4 lCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        glBindTexture(GL_TEXTURE_2D, lTextureID);
        lCharacter = lCharacter * translate(mat4(1.0f), playerOneLoc) * translate(mat4(1.0f), vec3(0.0f, 1.6f, 0.0f)) * translate(mat4(1.0f), playerOne.centerOfRacket);
        lCharacter = rotate(lCharacter, radians(armAngles[0][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f));
        lCharacter = translate(lCharacter, vec3(0.0f, 0.0f, -0.2f)) * scale(mat4(1.0f), vec3(0.2f, 0.2f, 0.5f));
        SetUniformMat4(shaderScene, "world_matrix", lCharacter);
        SetUniformVec3(shaderScene, "object_color", vec3(playerColors[0][1]));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        lCharacter = lCharacter * translate(mat4(1.0f), playerOneLoc) * translate(mat4(1.0f), playerOne.centerOfRacket) * translate(mat4(1.0f), vec3(0.0f, 1.875f, 0.0f));
        lCharacter = rotate(lCharacter, radians(armAngles[0][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f));
        lCharacter = translate(lCharacter, vec3(0.0f, 0.0f, -0.35f)) * scale(mat4(1.0f), vec3(0.2f, 0.75f, 0.2f));
        SetUniformMat4(shaderScene, "world_matrix", lCharacter);
        SetUniformVec3(shaderScene, "object_color", vec3(playerColors[0][1]));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Draw the letter i, bound to player 1 position
        glBindTexture(GL_TEXTURE_2D, iTextureID);
        mat4 iCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        iCharacter = iCharacter * translate(mat4(1.0f), playerOneLoc) * translate(mat4(1.0f), playerOne.centerOfRacket) * translate(mat4(1.0f), vec3(0.0f, 1.875f, 0.0f));
        iCharacter = rotate(iCharacter, radians(armAngles[0][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f));
        iCharacter = translate(iCharacter, vec3(0.0f, 0.0f, 0.3f)) * scale(mat4(1.0f), vec3(0.2f, 0.75f, 0.2f));
        SetUniformMat4(shaderScene, "world_matrix", iCharacter);
        SetUniformVec3(shaderScene, "object_color", vec3(playerColors[1][1]));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Draw the letter a, bound to player 2 position
        glBindTexture(GL_TEXTURE_2D, aTextureID);
        mat4 aCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        aCharacter = aCharacter * translate(mat4(1.0f), playerTwoLoc) * translate(mat4(1.0f), playerTwo.centerOfRacket) * translate(mat4(1.0f), vec3(0.0f, 1.875f, 0.0f));
        aCharacter = rotate(aCharacter, radians(armAngles[1][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f));
        aCharacter = translate(aCharacter, vec3(0.0f, 0.0f, 0.2f)) * scale(mat4(1.0f), vec3(0.2f, 0.75f, 0.2f));
        SetUniformMat4(shaderScene, "world_matrix", aCharacter);
        SetUniformVec3(shaderScene, "object_color", vec3(playerColors[2][1]));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        aCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        aCharacter = aCharacter * translate(mat4(1.0f), playerTwoLoc) * translate(mat4(1.0f), playerTwo.centerOfRacket) * translate(mat4(1.0f), vec3(0.0f, 1.875f, 0.0f));
        aCharacter = rotate(aCharacter, radians(armAngles[1][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f));
        aCharacter = translate(aCharacter, vec3(0.0f, 0.0f, 0.6f)) * scale(mat4(1.0f), vec3(0.2f, 0.75f, 0.2f));
        SetUniformMat4(shaderScene, "world_matrix", aCharacter);
        SetUniformVec3(shaderScene, "object_color", vec3(playerColors[2][1]));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        aCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        aCharacter = aCharacter * translate(mat4(1.0f), playerTwoLoc) * translate(mat4(1.0f), playerTwo.centerOfRacket) * translate(mat4(1.0f), vec3(0.0f, 2.15f, 0.0f));
        aCharacter = rotate(aCharacter, radians(armAngles[1][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f));
        aCharacter = translate(aCharacter, vec3(0.0f, 0.0f, 0.35f)) * scale(mat4(1.0f), vec3(0.2f, 0.2f, 0.5f));
        SetUniformMat4(shaderScene, "world_matrix", aCharacter);
        SetUniformVec3(shaderScene, "object_color", vec3(playerColors[2][1]));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        aCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        aCharacter = aCharacter * translate(mat4(1.0f), playerTwoLoc) * translate(mat4(1.0f), playerTwo.centerOfRacket) * translate(mat4(1.0f), vec3(0.0f, 1.8f, 0.0f));
        aCharacter = rotate(aCharacter, radians(armAngles[1][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f));
        aCharacter = translate(aCharacter, vec3(0.0f, 0.0f, 0.35f)) * scale(mat4(1.0f), vec3(0.2f, 0.2f, 0.5f));
        SetUniformMat4(shaderScene, "world_matrix", aCharacter);
        SetUniformVec3(shaderScene, "object_color", vec3(playerColors[2][1]));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Draw the letter n, bound to player 2 position
        glBindTexture(GL_TEXTURE_2D, nTextureID);
        mat4 nCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        nCharacter = nCharacter * translate(mat4(1.0f), playerTwoLoc) * translate(mat4(1.0f), playerTwo.centerOfRacket) * translate(mat4(1.0f), vec3(0.0f, 1.875f, 0.0f));
        nCharacter = rotate(nCharacter, radians(armAngles[1][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f));
        nCharacter = translate(nCharacter, vec3(0.0f, 0.0f, -0.55f)) * scale(mat4(1.0f), vec3(0.2f, 0.75f, 0.2f));
        SetUniformMat4(shaderScene, "world_matrix", nCharacter);
        SetUniformVec3(shaderScene, "object_color", vec3(playerColors[3][1]));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        nCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        nCharacter = nCharacter * translate(mat4(1.0f), playerTwoLoc) * translate(mat4(1.0f), playerTwo.centerOfRacket) * translate(mat4(1.0f), vec3(0.0f, 1.875f, 0.0f));
        nCharacter = rotate(nCharacter, radians(armAngles[1][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f));
        nCharacter = translate(nCharacter, vec3(0.0f, 0.0f, -0.35f));
        nCharacter = rotate(nCharacter, radians(30.0f), vec3(1.0f, 0.0, 0.0)) * scale(mat4(1.0f), vec3(0.2f, 0.8f, 0.2f));
        SetUniformMat4(shaderScene, "world_matrix", nCharacter);
        SetUniformVec3(shaderScene, "object_color", vec3(playerColors[3][1]));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        nCharacter = rotate(mat4(1.0), radians(worldXAngle), vec3(1.0f, 0.0f, 0.0f)) * rotate(mat4(1.0), radians(worldYAngle), vec3(0.0f, 1.0f, 0.0f));
        nCharacter = nCharacter * translate(mat4(1.0f), playerTwoLoc) * translate(mat4(1.0f), playerTwo.centerOfRacket) * translate(mat4(1.0f), vec3(0.0f, 1.875f, 0.0f));
        nCharacter = rotate(nCharacter, radians(armAngles[1][1] + 90.0f), vec3(0.0f, 1.0f, 0.0f));
        nCharacter = translate(nCharacter, vec3(0.0f, 0.0f, -0.1f)) * scale(mat4(1.0f), vec3(0.2f, 0.75f, 0.2f));
        SetUniformMat4(shaderScene, "world_matrix", nCharacter);
        SetUniformVec3(shaderScene, "object_color", vec3(playerColors[3][1]));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Retrun the transparency to 0
        SetUniformfValue(shaderScene, "transparency", 1.0f);

        drawScoreboard(shaderScene, 0, 0);

        //begin drawing the skybox
        GLuint skyboxTextures = glGetUniformLocation(shaderSkybox, "skybox");
        glUniform1i(skyboxTextures, 1);
        glDepthFunc(GL_LEQUAL);
        glUseProgram(shaderSkybox);
        // set view and projection matrix
        SetUniformMat4(shaderSkybox, "view_matrix", viewMatrix);
        SetUniformMat4(shaderSkybox, "projection_matrix", projectionMatrix);
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS);

        if (*(playerPosition) == playerOneLoc)
        {
            currentRacket = &playerOne;
        }
        else if (*(playerPosition) == playerTwoLoc)
        {
            currentRacket = &playerTwo;
        }

        if (CheckCollision(ballLocation, playerOneLoc + playerOne.centerOfRacket))
        {   
            float reflectedAngle = 2 * armAngles[0][1];
            xdir = cos(radians(reflectedAngle)) * -1.0f;
            zdir = sin(radians(reflectedAngle)) * 1.0f;
            ballColor = vec3(1.0f, 0.0f, 0.0f);
            currentRacket = &playerTwo;
            playerPosition = &playerTwoLoc;
            acceleration *= 1.05f;
            SoundEngine->play2D("code/assets/audio/tennisHit.mp3", false);
        }
        if (CheckCollision(ballLocation, playerTwoLoc + playerTwo.centerOfRacket))
        {
            float reflectedAngle = 2 * armAngles[1][1];
            xdir = cos(radians(reflectedAngle)) * 1.0f;
            zdir = sin(radians(reflectedAngle)) * -1.0f;
            ballColor = vec3(0.0f, 0.0f, 1.0f);
            currentRacket = &playerOne;
            playerPosition = &playerOneLoc;
            acceleration *= 1.05f;
            SoundEngine->play2D("code/assets/audio/tennisHit.mp3", false);
        }
        if ((ballLocation.x < -35.5f) | (ballLocation.x > 35.5f))
        {
            // Rerandomize starting movement
            srand(time(0));
            if (rand() % 2 == 1)
            {
                xdir = ((rand() % 3) + 3.0f) / 10;
                currentRacket = &playerTwo;
                playerPosition = &playerTwoLoc;
                acceleration = 1.0f;
            }
            else
            {
                xdir = -((rand() % 3) + 3.0f) / 10;
                currentRacket = &playerOne;
                playerPosition = &playerOneLoc;
                acceleration = 1.0f;
            }
            zdir = ((rand() % 10) - 5.0f) / 10;
            ballLocation = vec3(0.0f, 6.0f, 0.0f);
        }

        // End Frame
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Handle inputs
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Camera lookat and side vectors to update positions with ASDW
        if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) & (playerPosition != NULL))
        {
            // Uppercase A - move camera to the left
            if ((glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) | glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) == GLFW_PRESS)
            {
                // Movement is revered for players 1 on the other side of the net
                if ((*currentRacket).playerNumber < 2)
                {
                    (*playerPosition).z -= 4.0f * dt;
                    if ((*playerPosition).z < -16.0f)
                    {
                        (*playerPosition).z = -16.0f;
                    }
                }
                else
                {
                    (*playerPosition).z += 4.0f * dt;
                    if ((*playerPosition).z > 16.0f)
                    {
                        (*playerPosition).z = 16.0f;
                    }
                }
            }
            // Lowercase A - Rotate left
            else
            {
                if (armAngles[(*currentRacket).playerNumber - 1][1] > 120.0f)
                {
                    armAngles[(*currentRacket).playerNumber - 1][1] = 120.0f;
                }
                else
                {
                    armAngles[(*currentRacket).playerNumber - 1][1] += 10.0f * dt;
                }
            }
        }

        if ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) & (playerPosition != NULL)) // D - move camera to the right
        {
            // Uppercase D - move camera to the right
            if ((glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) | glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) == GLFW_PRESS)
            {
                // Movement is revered for players 1 on the other side of the net
                if ((*currentRacket).playerNumber < 2)
                {
                    (*playerPosition).z += 4.0f * dt;
                    if ((*playerPosition).z > 16.0f)
                    {
                        (*playerPosition).z = 16.0f;
                    }
                }
                else
                {
                    (*playerPosition).z -= 4.0f * dt;
                    if ((*playerPosition).z < -16.0f)
                    {
                        (*playerPosition).z = -16.0f;
                    }
                }
            }
            // Lowercase D - Rotate right
            else
            {
                if (armAngles[(*currentRacket).playerNumber - 1][1] < 60.0f)
                {
                    armAngles[(*currentRacket).playerNumber - 1][1] = 60.0f;
                }
                else
                {
                    armAngles[(*currentRacket).playerNumber - 1][1] -= 10.0f * dt;
                }
            }
        }

        if ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) & (playerPosition != NULL)) // S - move camera up
        {
            // Movement is revered for players 1 on the other side of the net
            if ((*currentRacket).playerNumber < 2)
            {
                (*playerPosition).x -= 4.0f * dt;
                if ((*playerPosition).x < -32.0f)
                {
                    (*playerPosition).x = -32.0f;
                }
            }
            else
            {
                (*playerPosition).x += 4.0f * dt;
                if ((*playerPosition).x > 32.0f)
                {
                    (*playerPosition).x = 32.0f;
                }
            }
        }

        if ((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) & (playerPosition != NULL)) // W - move camera down
        {
            // Movement is revered for players 1 on the other side of the net
            if ((*currentRacket).playerNumber < 2)
            {
                (*playerPosition).x += 4.0f * dt;
                if ((*playerPosition).x > -1.0f)
                {
                    (*playerPosition).x = -1.0f;
                }
            }
            else
            {
                (*playerPosition).x -= 4.0f * dt;
                if ((*playerPosition).x < 1.0f)
                {
                    (*playerPosition).x = 1.0f;
                }
            }
        }

        // Arrow keys
        if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)) // Left arrow
        {
            circleCameraAngle -= 10.0f * dt;
            cameraPosition = circleCamera;
            cameraLookAt = circleLookAt;

            // Reset view change so focus point does not go off track
            viewChange = vec3(0.0f, 0.0f, 0.0f);
        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // Up arrow
        {
            // Return current player to default null value
            sideCamera = true;

            cameraPosition = circleCamera;
            cameraLookAt = circleLookAt;
        }

        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // Right arrow 
        {
            circleCameraAngle += 10.0f * dt;
            cameraPosition = circleCamera;
            cameraLookAt = circleLookAt;

            // Reset view change so focus point does not go off track
            viewChange = vec3(0.0f, 0.0f, 0.0f);
        }

        // Arrow keys
        if ((glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)) // F - Rotate world in the postive x axis
        {
            worldXAngle -= 2.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) // T - Rotate world in the positive y axis
        {
            worldYAngle -= 2.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) // G - Rotate world in the negative y axis
        {
            worldYAngle += 2.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) // H - Rotate world in the negative x axis
        {
            worldXAngle += 2.0f;;
        }

        if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS) // HOME - Reset camera to default location, also removes current player
        {
            // Reset world rotation
            worldXAngle = 0.0f;
            worldYAngle = 0.0f;

            // Return to looking at the original spot
            cameraPosition = vec3(0.0f, 6.0f, 30.0f);
            cameraLookAt = vec3(0.0f, -6.0f, -30.0f);
            viewChange = vec3(0.0f, 0.0f, 0.0f);

            // Return current player to default null value
            playerPosition = NULL;

            FOV = 70.6f;
        }

        if (previousXstate == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) // X - Toggle textures
        {
            glGetUniformiv(shaderScene, textureFlag, currentTextureValue);
            if (currentTextureValue[0] == 1)
            {
                glUseProgram(shaderScene);
                glUniform1i(textureFlag, 0);
            }
            else
            {

                glUseProgram(shaderScene);
                glUniform1i(textureFlag, 1);
            }
        }
        previousXstate = glfwGetKey(window, GLFW_KEY_X);

        if (previousBstate == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) // B - Toggle shadows
        {
            glGetUniformiv(shaderScene, shadowFlag, currentShadowValue);
            if (currentShadowValue[0] == 1)
            {
                glUseProgram(shaderScene);
                glUniform1i(shadowFlag, 0);
            }
            else
            {
                glUseProgram(shaderScene);
                glUniform1i(shadowFlag, 1);
            }
        }
        previousBstate = glfwGetKey(window, GLFW_KEY_B);

        if (previousLstate == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) // L - Toggle z spotlight
        {
            glGetUniformiv(shaderScene, zSpotlightFlag, currentZSpotlightValue);
            if (currentZSpotlightValue[0] == 1)
            {
                glUseProgram(shaderScene);
                glUniform1i(zSpotlightFlag, 0);
            }
            else
            {
                glUseProgram(shaderScene);
                glUniform1i(zSpotlightFlag, 1); 
            }
        }
        previousLstate = glfwGetKey(window, GLFW_KEY_L);

        if (previousKstate == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) // K - Toggle y spotlight
        {
            glGetUniformiv(shaderScene, ySpotlightFlag, currentYSpotlightValue);
            if (currentYSpotlightValue[0] == 1)
            {
                glUseProgram(shaderScene);
                glUniform1i(ySpotlightFlag, 0);
            }
            else
            {
                glUseProgram(shaderScene);
                glUniform1i(ySpotlightFlag, 1);
            }
        }
        previousKstate = glfwGetKey(window, GLFW_KEY_K);

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) // 1 - Set player 1 as the current player
        {
            // Reset view difference
            viewChange = vec3(0.0f, 0.0f, 0.0f);
            sideCamera = false;

            // Change current player position reference to player 1
            playerPosition = &playerOneLoc;

            // Assign current racket as the first players racket
            currentRacket = &playerOne;

            // Reset world rotation
            worldXAngle = 0.0f;
            worldYAngle = 0.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) // 2 - Set player 2 as the current player
        {
            // Reset view difference
            viewChange = vec3(0.0f, 0.0f, 0.0f);
            sideCamera = false;

            // Change current player position reference to player 2
            playerPosition = &playerTwoLoc;

            // Assign current racket as the second players racket
            currentRacket = &playerTwo;

            // Reset world rotation
            worldXAngle = 0.0f;
            worldYAngle = 0.0f;
        }

        // Different rendering modes
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) // P - Render model as points
        {
            renderMode = GL_POINTS;
        }

        if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) // O - Render model as lines
        {
            renderMode = GL_LINES;
        }

        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) // I - Render model as triangles
        {
            renderMode = GL_TRIANGLES;
        }

        // Mouse buttons
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) // Pressing right mouse button and moving pans the camera
        {
            glfwGetCursorPos(window, &mousePosX, &mousePosY);
            double dx = mousePosX - lastMousePosX;

            if (currentRacket == NULL)
            {
                viewChange.x -= dx * 0.01;
            }
            else if ((*currentRacket).playerNumber < 2)
            {
                viewChange.z += dx * 0.01;
            }
            else
            {
                viewChange.z -= dx * 0.01;
            }
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) // Pressing middle mouse button and moving tilts the camera
        {
            glfwGetCursorPos(window, &mousePosX, &mousePosY);
            double dy = mousePosY - lastMousePosY;

            viewChange.y += dy * 0.01;

            // Clamp Y values so nothing wacky happens
            if (viewChange.y < -2.0)
            {
                viewChange.y = -2.0f;
            }
            else if (viewChange.y > 2.0)
            {
                viewChange.y = 2.0f;
            }
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) // Pressing left mouse button and moving zooms in/out
        {
            glfwGetCursorPos(window, &mousePosX, &mousePosY);
            double dy = mousePosY - lastMousePosY;

            FOV += dy * 0.01;

            // Clamp FOV values so nothing wacky happens
            if (FOV < 69.3)
            {
                FOV = 69.3f;
            }
            else if (FOV > 71.8)
            {
                FOV = 71.8f;
            }
        }
        glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);
        if (sideCamera) 
        { }
        else if (playerPosition != NULL)
        {
            if (playerPosition == &playerOneLoc)
            {
                // Place the camera beside to allow a proper view
                cameraPosition = vec3((*playerPosition).x + playerOne.centerOfRacket.x, (*playerPosition).y + playerOne.centerOfRacket.y + 2.0f, (*playerPosition).z) + playerOne.centerOfRacket.z - 2.0f;

                // Change the lookAt
                cameraLookAt = vec3(15.0f, -1.0f, 0.0f);
            }
            else if (playerPosition == &playerTwoLoc)
            {
                // Place the camera beside to allow a proper view
                cameraPosition = vec3((*playerPosition).x + playerTwo.centerOfRacket.x, (*playerPosition).y + playerTwo.centerOfRacket.y - 2.0f, (*playerPosition).z) + playerTwo.centerOfRacket.z + 2.0f;

                // Change the lookAt
                cameraLookAt = vec3(-15.0f, -1.0f, 0.0f);
            }
        }
    }

    // Shutdown GLFW
    glfwTerminate();

    return 0;
}