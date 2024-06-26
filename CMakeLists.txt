cmake_minimum_required(VERSION 3.15)
project(demo VERSION 1.0)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)

# 添加包含目录
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/GLFW/include
    ${PROJECT_SOURCE_DIR}/include/glm
    ${PROJECT_SOURCE_DIR}/include/imgui
)

file(GLOB SRC
        ${PROJECT_SOURCE_DIR}/include/imgui/*.h
        ${PROJECT_SOURCE_DIR}/include/imgui/*.cpp
)

# 设置代码文件
aux_source_directory(.  SS1)

set(SRC_LIST
    ${SS1}
    ${SRC}
    SoftRenderer.cpp
)

# 添加目标链接
if(WIN32)
    set(GLFW_LINK ${PROJECT_SOURCE_DIR}/lib/GLFW/WIN/lib/libglfw3.a)
    set(OPENGL_LINK ${PROJECT_SOURCE_DIR}/lib/OpenGL32.Lib)
    link_libraries(${GLFW_LINK} ${OPENGL_LINK})
    message("Windows platform detected")
elseif(APPLE)
    set(GLFW_LINK ${PROJECT_SOURCE_DIR}/lib/GLFW/MAC/lib-arm64/libglfw.3.dylib)
    link_libraries(${GLFW_LINK})
    message("Apple platform detected")
else()  # Linux
    set(GLFW_LINK ${PROJECT_SOURCE_DIR}/lib/GLFW/LINUX/libglfw3.a)
    set(OPENGL_LINK ${PROJECT_SOURCE_DIR}/lib/libGL.so)
    set(OPENGLU_LINK ${PROJECT_SOURCE_DIR}/lib/libGLU.so)
    link_libraries(${GLFW_LINK} ${OPENGL_LINK} ${OPENGLU_LINK})
    message("Linux platform detected")
endif()

# 添加可执行文件
add_executable(demo ${SRC_LIST})

# 链接系统的 OpenGL 框架
if (APPLE)
    target_link_libraries(demo "-framework OpenGL")
endif()

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)