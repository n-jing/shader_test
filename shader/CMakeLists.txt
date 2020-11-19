ADD_LIBRARY(glad STATIC ${CMAKE_SOURCE_DIR}/extern/glad/src/glad.c)
TARGET_INCLUDE_DIRECTORIES(glad PUBLIC ${CMAKE_SOURCE_DIR}/extern/glad/include)

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
  include_directories(${GLEW_INCLUDE_DIRS})
  link_libraries(${GLEW_LIBRARIES})
endif()


find_package(assimp REQUIRED)

ADD_LIBRARY(win_img stb_image.cpp init_window.cpp)
TARGET_INCLUDE_DIRECTORIES(win_img PUBLIC  ${CMAKE_SOURCE_DIR}/extern/glad/include)
TARGET_LINK_LIBRARIES(win_img glfw)

ADD_EXECUTABLE(display display.cpp)
TARGET_LINK_LIBRARIES(display glad dl GL GLU glut glfw ${ASSIMP_LIBRARIES} win_img)
SET_TARGET_PROPERTIES(display
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")


ADD_EXECUTABLE(text_render text_render.cpp)
TARGET_INCLUDE_DIRECTORIES(text_render PUBLIC ${CMAKE_SOURCE_DIR}/extern/glad/include
  ${CMAKE_SOURCE_DIR}/extern/freetype/freetype)
TARGET_LINK_LIBRARIES(text_render glad dl GL GLU glut glfw ${ASSIMP_LIBRARIES} win_img  freetype png z harfbuzz png m)
SET_TARGET_PROPERTIES(text_render
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")


# ADD_SUBDIRECTORY(doc EXCLUDE_FROM_ALL)
