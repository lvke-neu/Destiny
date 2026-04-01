cmake_minimum_required(VERSION 3.16)

project(VulkanDemo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/utf-8)
endif()

# Find Vulkan
find_package(Vulkan REQUIRED)

# Include GLFW
include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.8
)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw)

# 引入 GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        0.9.9.8
)
FetchContent_MakeAvailable(glm)

# 引入 stb_image 用于加载图片
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(stb)

add_executable(VulkanDemo src/main.cpp)

# 链接库
target_include_directories(VulkanDemo PRIVATE ${stb_SOURCE_DIR})
target_link_libraries(VulkanDemo PRIVATE Vulkan::Vulkan glfw glm::glm)

# Shader Compilation Logic
find_program(GLSLC_EXECUTABLE glslc HINTS $ENV{VULKAN_SDK}/Bin)

if(GLSLC_EXECUTABLE)
    message(STATUS "Found glslc: ${GLSLC_EXECUTABLE}")
    
    file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/builtin/shaders")
    set(SPV_SHADERS "")

    # Compile Vertex Shader (HLSL)
    set(VERT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/builtin/shaders/shader.vert.hlsl")
    set(VERT_SPV "${CMAKE_BINARY_DIR}/builtin/shaders/shader.vert.hlsl.spv")
    add_custom_command(
        OUTPUT ${VERT_SPV}
        COMMAND ${GLSLC_EXECUTABLE} -fshader-stage=vertex -fentry-point=main ${VERT_SOURCE} -o ${VERT_SPV}
        DEPENDS ${VERT_SOURCE}
        COMMENT "Compiling HLSL Vertex Shader"
    )
    list(APPEND SPV_SHADERS ${VERT_SPV})

    # Compile Fragment Shader (HLSL)
    set(FRAG_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/builtin/shaders/shader.frag.hlsl")
    set(FRAG_SPV "${CMAKE_BINARY_DIR}/builtin/shaders/shader.frag.hlsl.spv")
    add_custom_command(
        OUTPUT ${FRAG_SPV}
        COMMAND ${GLSLC_EXECUTABLE} -fshader-stage=fragment -fentry-point=main ${FRAG_SOURCE} -o ${FRAG_SPV}
        DEPENDS ${FRAG_SOURCE}
        COMMENT "Compiling HLSL Fragment Shader"
    )
    list(APPEND SPV_SHADERS ${FRAG_SPV})
    
    add_custom_target(CompileShaders ALL DEPENDS ${SPV_SHADERS})
    add_dependencies(VulkanDemo CompileShaders)

    # Copy shaders to executable directory
    add_custom_command(TARGET VulkanDemo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:VulkanDemo>/builtin/shaders
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/builtin/shaders $<TARGET_FILE_DIR:VulkanDemo>/builtin/shaders
        COMMENT "Copying shaders to executable directory"
    )
    
    # Copy texture to executable directory
    add_custom_command(TARGET VulkanDemo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:VulkanDemo>/builtin/texture
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/builtin/texture $<TARGET_FILE_DIR:VulkanDemo>/builtin/texture
        COMMENT "Copying texture to executable directory"
    )

else()
    message(WARNING "glslc not found! You may need to compile shaders manually.")
endif()
