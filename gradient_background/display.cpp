#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../extern/glm/glm/glm.hpp"
#include "../extern/glm/glm/gtc/matrix_transform.hpp"
#include "../extern/glm/glm/gtc/type_ptr.hpp"

#include "shader.h"
#include "camera.h"
#include "init_window.h"
#include "init_buffer.h"
#include "model.h"


#include <iostream>

// camera
Camera camera(glm::vec3(0.0f, 2.5f, 10.0f));

// lighting
// glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightPos(2.0f, 4.0f, 1.f);


using namespace std;
using namespace Jing;


int main()
{
// settings
  const unsigned int SCR_WIDTH = 800;
  const unsigned int SCR_HEIGHT = 600;

  GLFWwindow* window = InitWindow(SCR_WIDTH, SCR_HEIGHT, "gradient_background");

  // build and compile our shader zprogram
  // ------------------------------------
  Shader planeShader("../gradient_background/plane.vs", "../gradient_background/plane.fs");
  Shader screenShader("../gradient_background/screen.vs", "../gradient_background/screen.fs");

  float level = -2.0;
  float plane_vertices[] = {
    // positions          // colors           // texture coords
    100.5f,  level, 100.0f,   0.0f, 1.0f, 0.0f,   100.0f, 100.0f, // top right
    100.5f, level, -100.0f,   0.0f, 1.0f, 0.0f,   100.0f, -100.0f, // bottom right
    -100.5f, level, -100.0f,   0.0f, 1.0f, 0.0f,   -100.0f, -100.0f, // bottom left
    -100.5f,  level, 100.0f,   0.0f, 1.0f, 0.0f,   -100.0f, 100.0f  // top left 
  };
  unsigned int plane_indices[] = {  
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
  };

  unsigned int planeVBO, planeVAO, planeEBO;
  InitBufferEBO(planeVAO, planeVBO, planeEBO, sizeof(plane_vertices), plane_vertices, 8,
                sizeof(plane_indices), plane_indices, {3, 3, 2}, {0, 3, 6});


  unsigned int screenFBO;
  unsigned int screenBuffer;
  tie(screenFBO, screenBuffer) = Jing::GenerateFrameBufferWithColor(SCR_WIDTH, SCR_HEIGHT);
  // Model screen("../resource/screen.obj", false);

  unsigned int texture = loadTexture("../resource/grid.png");

  Model screen_model("../resource/screen.obj", false);
  screenShader.use();
  screenShader.setInt("texture1", 0);

  // render loop
  // -----------
  float lastFrame = 0.0f;
  while (!glfwWindowShouldClose(window))
  {
    // per-frame time logic
    // --------------------
    float currentFrame = glfwGetTime();
    float deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // input
    // -----
    processInput(window, deltaTime);

    // render
    // ------
    glBindFramebuffer(GL_FRAMEBUFFER, screenFBO);
    glClearColor(1.f, 1.f, 1.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);


    // 1. render depth of scene to texture (from light's perspective)
    // --------------------------------------------------------------
    glm::mat4 lightProjection, lightView;
    float near_plane = 0.1f, far_plane = 17.5f;
    lightProjection = glm::ortho(-20.0f, 20.0f, -6.0f, 20.0f, near_plane, far_plane);
    lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    // render scene from light's point of view
    
    glm::mat4 projection =glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    planeShader.use();
    glEnable(GL_DEPTH_TEST);  
    planeShader.setMat4("projection", projection);
    planeShader.setMat4("view", view);
    planeShader.setMat4("model", glm::mat4(1.0f));
    planeShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    planeShader.setVec3("lightPos", lightPos);
    planeShader.setVec3("viewPos", camera.Position);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glBindVertexArray(planeVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1.f, 1.f, 1.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    
    screenShader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, screenBuffer);
    screenShader.setVec2("screen_size", 1.0f/SCR_WIDTH, 1.0f/SCR_HEIGHT);
    screen_model.Draw(screenShader);
    
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}
