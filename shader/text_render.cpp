#include <iostream>
#include <map>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../extern/glm/glm/glm.hpp"
#include "../extern/glm/glm/gtc/matrix_transform.hpp"
#include "../extern/glm/glm/gtc/type_ptr.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "shader.h"
#include "text_render.h"
#include "init_window.h"
#include "camera.h"

using namespace Jing;


Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));

int main()
{
  const unsigned int SCR_WIDTH = 800;
  const unsigned int SCR_HEIGHT = 600;

  GLFWwindow* window = InitWindow(SCR_WIDTH, SCR_HEIGHT);
  // OpenGL state
  // ------------
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  std::map<GLchar, Character> Characters;
  unsigned int VBO, VAO;
  Set26Letter(Characters, VBO, VAO);
  Shader shader = GetTextRender(SCR_WIDTH, SCR_HEIGHT);
  
  // render loop
  // -----------
  double time = glfwGetTime();
  while (!glfwWindowShouldClose(window))
  {
    // input
    // -----
    processInput(window, 0.0f);

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    int fps = 1.0/(glfwGetTime() - time);
    time = glfwGetTime();
    RenderText(shader, "Frame rate:", 25.0f, SCR_HEIGHT - 30.0f, 0.4f, glm::vec3(0.5, 0.8f, 0.2f), Characters, VBO, VAO);
    RenderText(shader, std::to_string(fps), 110.0f, SCR_HEIGHT - 30.0f, 0.4f, glm::vec3(0.5, 0.8f, 0.2f), Characters, VBO, VAO);
    RenderText(shader, "(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f), Characters, VBO, VAO);
       
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
