#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;

#include <iostream>
using std::cerr;
using std::endl;

#include "helper/glutils.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "helper/stb/stb_image.h"

using glm::vec3;

SceneBasic_Uniform::SceneBasic_Uniform() : angle(0.0f) {}


unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}



void SceneBasic_Uniform::initScene()
{
    compile();

    std::cout << std::endl;

    prog.printActiveUniforms();

    /////////////////// Create the VBO ////////////////////
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };

    float normals[] = {
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,

        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,

        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,


        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };

    float textureCoords[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f
    };

    // Create and populate the buffer objects
    GLuint quadVboHandles[3];
    glGenBuffers(3, quadVboHandles);
    GLuint quadPositionBufferHandle = quadVboHandles[0];
    GLuint quadNormalBufferHandle = quadVboHandles[1];
    GLuint quadTexCoordBufferHandle = quadVboHandles[2];

    glBindBuffer(GL_ARRAY_BUFFER, quadPositionBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, quadNormalBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, quadTexCoordBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);

    // Create and set-up the vertex array object
    glGenVertexArrays( 1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    glEnableVertexAttribArray(0);  // Vertex position
    glEnableVertexAttribArray(1);  // Vertex normal
    glEnableVertexAttribArray(2);  // Vertex texCoord



    #ifdef __APPLE__
        glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);

        glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
    #else
        glBindVertexBuffer(0, quadPositionBufferHandle, 0, sizeof(GLfloat) * 3);
        glBindVertexBuffer(1, quadNormalBufferHandle, 0, sizeof(GLfloat) * 3);
        glBindVertexBuffer(2, quadTexCoordBufferHandle, 0, sizeof(GLfloat) * 2);

        glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(0, 0);
        glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(1, 1);
        glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(2, 2);

    #endif
        glBindVertexArray(0);


     //model
    pModel = ObjMesh::load("banana.obj");


    texVec[0] = loadTexture("floor_diffuse.png");
    texVec[1] = loadTexture("desk.jpg");
    texVec[2] = loadTexture("table.jpg");
    texVec[3] = loadTexture("table1.jpg");
    texVec[4] = loadTexture("Bannana_v01.jpg");

    glEnable(GL_DEPTH_TEST);
}

void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
{
	//update your angle here
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //create the rotation matrix here and update the uniform in the shader 
    glm::vec3 cameraPosition = glm::vec3(0, 2.5, 4);
    glm::mat4 projection = glm::perspective(glm::radians(60.f), (float)width / (float)height, 0.1f, 100.0f);
    glm::mat4 view = glm::lookAt(cameraPosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    prog.use();
    prog.setUniform("projection", projection);
    prog.setUniform("view", view);
    
    glm::vec3 pointLightPositions[] = {
    glm::vec3(-1,  5,  2.0f),
    glm::vec3(2.3f, 5, -4.0f),
    };
    // point light 1
    prog.setUniform("pointLights[0].position", pointLightPositions[0]);
    prog.setUniform("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    prog.setUniform("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    prog.setUniform("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    prog.setUniform("pointLights[0].constant", 1.0f);
    prog.setUniform("pointLights[0].linear", 0.001f);
    prog.setUniform("pointLights[0].quadratic", 0.002f);
    // point light 2
    prog.setUniform("pointLights[1].position", pointLightPositions[1]);
    prog.setUniform("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    prog.setUniform("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    prog.setUniform("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    prog.setUniform("pointLights[1].constant", 1.0f);
    prog.setUniform("pointLights[1].linear", 0.09f);
    prog.setUniform("pointLights[1].quadratic", 0.032f);


    prog.setUniform("viewPos", cameraPosition);
    // floor
    glBindVertexArray(vaoHandle);
    glm::mat4 model = glm::mat4(1.f);
    model = glm::scale(model, glm::vec3(5, 0.1, 5));
    prog.setUniform("model", model);
    prog.setUniform("color", glm::vec3(1.0, 1.0, 1.0));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texVec[0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // desk
    model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(0, 0.6, 0));
    model = glm::scale(model, glm::vec3(1, 1.2, 1));
    prog.setUniform("model", model);
    prog.setUniform("color", glm::vec3(1.0, 1.0, 1.0));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texVec[1]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // desk
    model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(0, 1.2, 0));
    model = glm::scale(model, glm::vec3(3, 0.1, 2));
    prog.setUniform("model", model);
    prog.setUniform("color", glm::vec3(1.0, 1.0, 1.0));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texVec[2]);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //an ban
    model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(0, 1.3, 0));
    model = glm::scale(model, glm::vec3(1.6, 0.08, 1.2));
    prog.setUniform("model", model);
    prog.setUniform("color", glm::vec3(1.0, 1.0, 1.0));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texVec[3]);
    glDrawArrays(GL_TRIANGLES, 0, 36);


    // banana
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0, 1.45, 0));
    model = glm::rotate(model, glm::radians(225.f), glm::vec3(0, 1, 0));
    model = glm::scale(model, glm::vec3(1.5, 1.5, 1.5));
    prog.setUniform("model", model);
    prog.setUniform("color", glm::vec3(1.0, 1.0, 1.0));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texVec[4]);
    pModel->render();


    glBindVertexArray(0);
}

void SceneBasic_Uniform::resize(int w, int h)
{
    width = w;
    height = h;
    glViewport(0,0,w,h);
}
