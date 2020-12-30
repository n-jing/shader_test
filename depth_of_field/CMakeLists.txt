find_package(GLEW REQUIRED)
if (GLEW_FOUND)
  include_directories(${GLEW_INCLUDE_DIRS})
  link_libraries(${GLEW_LIBRARIES})
endif()


ADD_EXECUTABLE(depth_of_field display.cpp)
TARGET_INCLUDE_DIRECTORIES(depth_of_field PUBLIC ${CMAKE_SOURCE_DIR}/source)
TARGET_LINK_LIBRARIES(depth_of_field glad dl GL GLU glut glfw win_img)
SET_TARGET_PROPERTIES(depth_of_field
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
