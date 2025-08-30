cmake_minimum_required(VERSION 3.25.1)
project(educationalManagementSystem VERSION 1.0)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/RBTree)
option(USE_RBTree "RBTree USE ?" ON)
if(USE_RBTree)
    List(APPEND EXTERA_LIBS RBTree)
    List(APPEND EXTERA_INCLUDE RBTree)
    message(STATUS "RBTre is used!")
endif()


FILE(GLOB SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")


FILE(GLOB SDL2_LIBS "${CMAKE_CURRENT_SOURCE_DIR}/SDL2/lib/*.so")
set(SDL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SDL2/include")

FILE(COPY "${CMAKE_CURRENT_SOURCE_DIR}/resources"  DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/course.csv ${CMAKE_CURRENT_BINARY_DIR}/data/course.csv COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/root.csv ${CMAKE_CURRENT_BINARY_DIR}/data/root.csv COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/student.csv ${CMAKE_CURRENT_BINARY_DIR}/data/student.csv COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/teacher.csv ${CMAKE_CURRENT_BINARY_DIR}/data/teacher.csv COPYONLY)


add_executable(educationalManagementSystem ${SOURCE})
target_include_directories(educationalManagementSystem PRIVATE ${INCLUDE_DIR} ${SDL_INCLUDE_DIR} ${EXTERA_INCLUDE})
target_link_libraries(educationalManagementSystem PRIVATE ${SDL2_LIBS} ${EXTERA_LIBS}) 