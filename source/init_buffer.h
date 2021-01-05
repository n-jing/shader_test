#ifndef INIT_BUFFER_JJ_H
#define INIT_BUFFER_JJ_H

#include <GLFW/glfw3.h>
#include <vector>
#include <tuple>


namespace Jing
{
  //! \brief generate vertex array buffer
  //! \param [in] vert_len, vertices size
  //! \param [in] vertices, address of vertices
  //! \param [in] R, size of one vertex and its attribute
  //! \param [in] len, size of each attribute
  template<typename T = float>
  void InitBuffer(unsigned int &VAO, unsigned int &VBO, unsigned int vert_len, T *vertices, int R,
                  const std::vector<int> &len, const std::vector<int> &idx)
  {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vert_len, vertices, GL_STATIC_DRAW);

    const int N = len.size();
    for (int i = 0; i < N; ++i)
    {
      // position attribute
      glVertexAttribPointer(i, len[i], GL_FLOAT, GL_FALSE, R * sizeof(T), (void*)(idx[i] * sizeof(T)));
      glEnableVertexAttribArray(i);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 
  }
  
  template<typename T = float>
  void InitBufferEBO(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO, unsigned int vert_len, T *vertices, int R,
                     unsigned int ele_len, unsigned int *ele_idx,
                     const std::vector<int> &len, const std::vector<int> &idx)
  {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vert_len, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ele_len, ele_idx, GL_STATIC_DRAW);

    const int N = len.size();
    for (int i = 0; i < N; ++i)
    {
      // position attribute
      glVertexAttribPointer(i, len[i], GL_FLOAT, GL_FALSE, R * sizeof(T), (void*)(idx[i] * sizeof(T)));
      glEnableVertexAttribArray(i);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); 
  }

  std::tuple<int, int> GenerateFrameBufferWithColor(unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT)
  {
    unsigned int FBO;
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    unsigned int colorBuffer;
    glGenTextures(1, &colorBuffer);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return {FBO, colorBuffer};
  }

  std::tuple<int, int> GenerateFrameBufferWithDepth(unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT)
  {
    unsigned int FBO;
    glGenFramebuffers(1, &FBO);
    // create depth texture
    unsigned int depthBuffer;
    glGenTextures(1, &depthBuffer);
    glBindTexture(GL_TEXTURE_2D, depthBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);
    glDrawBuffer(GL_NONE); // add color buffer to make framebuffer complete
    glReadBuffer(GL_NONE); // add color buffer to make framebuffer complete

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return {FBO, depthBuffer};
  }
}


#endif // INIT_BUFFER_JJ_H
