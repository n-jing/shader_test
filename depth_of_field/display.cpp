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
Camera camera(glm::vec3(0.0f, 5.3f, 18.0f));

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

  GLFWwindow* window = InitWindow(SCR_WIDTH, SCR_HEIGHT, "shadow_mapping");

  // build and compile our shader zprogram
  // ------------------------------------
  Shader testShader("../depth_of_field/demo.vs", "../depth_of_field/demo.fs");
  Shader screenShader("../depth_of_field/dof.vs", "../depth_of_field/dof.fs");

  Model book("../resource/book/book.obj");
  unsigned int screenFBO;
  unsigned int screenBuffer;
  tie(screenFBO, screenBuffer) = Jing::GenerateFrameBufferWithColor(SCR_WIDTH, SCR_HEIGHT);

  Model screen("../resource/screen.obj");
  float screen_quad[] = {
    1.0f,  1.0f, 0.0f, 1.0f, 1.0f,  // top right
    1.0f, -1.0f, 0.0f,  1.0f, 0.0f,// bottom right
    -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,// bottom left
    -1.0f,  1.0f, 0.0f,   0.0f, 1.0f// top left 
  };
  unsigned int screen_indices[] = {  // note that we start from 0!
    0, 1, 3,  // first Triangle
    1, 2, 3   // second Triangle
  };
 
  unsigned int screenVBO, screenVAO, screenEBO;
  InitBufferEBO(screenVAO, screenVBO, screenEBO, sizeof(screen_quad), screen_quad, 5,
                sizeof(screen_indices), screen_indices, {3, 2}, {0, 3});

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
    glm::mat4 projection =glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    testShader.use();
    testShader.setMat4("projection", projection);
    testShader.setMat4("view", view);
    glm::mat4 model = glm::mat4(1.0f);
    // model = glm::translate(model, glm::vec3(-1.f, -2.0f, -3.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // model = glm::scale(model, glm::vec3(0.001f, 0.001f, 0.001f));
    // model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
    testShader.setMat4("model", model);
    book.Draw(testShader);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(1.f, 1.f, 1.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    
    screenShader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, screenBuffer);
    screenShader.setVec2("screen_size", 1.0f/SCR_WIDTH, 1.0f/SCR_HEIGHT);
    // glBindVertexArray(screenVAO);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    screen.Draw(screenShader);
    
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
