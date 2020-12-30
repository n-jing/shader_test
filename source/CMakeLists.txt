ADD_LIBRARY(glad STATIC ${CMAKE_SOURCE_DIR}/extern/glad/src/glad.c)
TARGET_INCLUDE_DIRECTORIES(glad PUBLIC ${CMAKE_SOURCE_DIR}/extern/glad/include)


find_package(GLEW REQUIRED)
if (GLEW_FOUND)
  include_directories(${GLEW_INCLUDE_DIRS})
  link_libraries(${GLEW_LIBRARIES})
endif()

FIND_PACKAGE(assimp REQUIRED)

ADD_LIBRARY(win_img stb_image.cpp init_window.cpp)
TARGET_INCLUDE_DIRECTORIES(win_img PUBLIC  ${CMAKE_SOURCE_DIR}/extern/glad/include)
TARGET_LINK_LIBRARIES(win_img glfw ${ASSIMP_LIBRARIES})

