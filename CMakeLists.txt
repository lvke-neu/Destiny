cmake_minimum_required(VERSION 3.14)

project(Destiny)

add_compile_definitions(UNICODE _UNICODE)
if (WIN7_SYSTEM_SUPPORT MATCHES ON)
    add_compile_definitions(_WIN32_WINNT=0x601)
endif()

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

file(GLOB SRC_MAIN src/*)
file(GLOB SRC_ENGINE src/Engine/*)
file(GLOB SRC_ENGINE_DETAIL src/Engine/Detail/*)
file(GLOB SRC_EDITOR src/Editor/*)
file(GLOB SRC_GRAPHICS src/Graphics/*)
file(GLOB SRC_SCENE src/Scene/*)
file(GLOB BUILTIN builtin/*)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

source_group("Engine" FILES ${SRC_ENGINE})
source_group("Engine\\Detail" FILES ${SRC_ENGINE_DETAIL})
source_group("Editor" FILES ${SRC_EDITOR})
source_group("Graphics" FILES ${SRC_GRAPHICS})
source_group("Scene" FILES ${SRC_SCENE})
source_group("builtin" FILES ${BUILTIN})

include_directories("3rdparty/spdlog/include")
include_directories("src/")


add_executable(Destiny WIN32 ${SRC_MAIN} ${SRC_ENGINE} ${SRC_ENGINE_DETAIL} ${SRC_EDITOR} ${SRC_GRAPHICS} ${SRC_SCENE} ${BUILTIN})
target_link_libraries(Destiny d3d11.lib dxgi.lib dxguid.lib D3DCompiler.lib d2d1.lib dwrite.lib winmm.lib)

file(COPY builtin DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY builtin DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)

