#ifndef INIT_BUFFER_JJ_H
#define INIT_BUFFER_JJ_H

#include <GLFW/glfw3.h>
#include <vector>

namespace Jing
{
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
  }
  
}


#endif // INIT_BUFFER_JJ_H
