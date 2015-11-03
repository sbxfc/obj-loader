#include <stdio.h>
#include <OpenGL/gl3.h>
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include "ShaderLoader.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <iostream>
#include "RGObjLoader.h"

ShaderLoader shaderLoader;

GLint matview_fun;
GLint matproj_fun;
GLint matrot_fun;

glm::mat4 Model;
glm::mat4 View;
glm::mat4 Projection;

std::vector<rg::shape_r> shapes;
std::vector<rg::material_r> materials;

 /**
 *  加载obj模型
 */
void loadModel()
{
    std::string objfile = "/Users/wistoneqqx/Documents/github/sbxfc/OpenGLDrawOBJ/triangel_index/triangel_index/data/porsche.obj";
    std::string mtlpath = "/Users/wistoneqqx/Documents/github/sbxfc/OpenGLDrawOBJ/triangel_index/triangel_index/data/";

    rg::LoadObj(
        shapes,
        materials,
        objfile.c_str(),
        mtlpath.c_str());
}

void initModel(GLuint m_vao[],size_t shapersNum)
{
    //视图矩阵
    View = glm::lookAt(
                       glm::vec3(0.0f,50.0f,100.0f),
                       glm::vec3(0,0,0),
                       glm::vec3(0,1,0)
                       );
    
    //透视投影矩阵
    Projection = glm::perspective(
                      45.0f,
                      800.0f/600.0f,
                      2.0f,
                      1000.0f
                      );
    
    Model = glm::mat4(1.0f);
    Model = glm::rotate(Model, 45.0f, glm::vec3(0,1,0));
    
    for(int i = 0 ; i < shapersNum ;i++)
    {
        glGenVertexArrays(1,&m_vao[i]);
        glBindVertexArray(m_vao[i]);
        
            GLuint m_vbo[4];
            glGenBuffers(4, m_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
            glBufferData(GL_ARRAY_BUFFER, shapes[i].mesh.positions.size()*sizeof(GLfloat), &shapes[i].mesh.positions[0], GL_STATIC_DRAW);
        
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
        
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
            glBufferData(GL_ARRAY_BUFFER, shapes[i].mesh.normals.size()*sizeof(GLfloat), &shapes[i].mesh.normals[0], GL_STATIC_DRAW);
        
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);
        
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo[3]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, shapes[i].mesh.indices.size()*sizeof(GLuint), &shapes[i].mesh.indices[0], GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

}

void drawModel(GLuint m_vao[],size_t size)
{
    for(int i = 0 ; i < size ;i++)
    {
        glBindVertexArray(m_vao[i]);
        glDrawElements(GL_TRIANGLES,(GLsizei)shapes[i].mesh.indices.size(),GL_UNSIGNED_INT,0);
        glBindVertexArray(0);
    }
}

//设置并使用着色器
void initShader(void)
{
    shaderLoader.load(
            "/Users/wistoneqqx/Documents/github/sbxfc/OpenGLDrawOBJ/triangel_index/triangel_index/simpleShader.vert",
            "/Users/wistoneqqx/Documents/github/sbxfc/OpenGLDrawOBJ/triangel_index/triangel_index/simpleShader.frag"
            );
    shaderLoader.bind();
    
    //获取着色文件中的矩阵指针
    matview_fun = glGetUniformLocation(shaderLoader.getProgramID(), "mat_view");
    matproj_fun = glGetUniformLocation(shaderLoader.getProgramID(), "mat_proj");
    matrot_fun = glGetUniformLocation(shaderLoader.getProgramID(), "mat_rot");
    
    glUniformMatrix4fv(matview_fun,1,GL_FALSE,&View[0][0]);
    glUniformMatrix4fv(matproj_fun,1,GL_FALSE,&Projection[0][0]);
    glUniformMatrix4fv(matrot_fun,1,GL_FALSE,&Model[0][0]);
}

int main(void)
{
    GLFWwindow* window;
    
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    //使用OpenGL 3.2
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    //加载模型并返回模型的面数
    loadModel();
    size_t facecount = shapes.size();
    
    GLuint m_vao[facecount];
    initModel(m_vao,facecount);
    initShader();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_BLEND);
        //深度测试，这个很重要
        glEnable(GL_DEPTH_TEST);
        
        drawModel(m_vao,facecount);
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

