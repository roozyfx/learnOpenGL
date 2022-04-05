cmake_minimum_required(VERSION 3.10)

project(openGL_Tutorial_0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package (PkgConfig REQUIRED)
find_package (OpenGL REQUIRED)
find_package (glfw3 REQUIRED)

set(SRCs
    main.cpp
    glad.c
    Shader.cpp
    Vao.cpp
)

set(HDRs

)

set(3rdparty_INCLUDE_DIR "/home/user/fx/localLibs/include")
include_directories(${3rdparty_INCLUDE_DIR})

add_executable(${PROJECT_NAME}
    ${SRCs} ${HDRs}
    )

target_link_libraries(${PROJECT_NAME}
                        OpenGL::GL
                        glfw
                        dl
                        #to use std::filesystem with GCC8/Clang10
                        stdc++fs
)
