cmake_minimum_required(VERSION 3.14)

project(Destiny)

add_compile_definitions(UNICODE _UNICODE)
if (WIN7_SYSTEM_SUPPORT MATCHES ON)
    add_compile_definitions(_WIN32_WINNT=0x601)
endif()

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

file(GLOB SRC_MAIN src/main.cpp)
file(GLOB SRC_ENGINE src/Engine/*)

add_executable(Destiny WIN32 ${SRC_MAIN} ${SRC_ENGINE})
target_link_libraries(Destiny d3d11.lib dxgi.lib dxguid.lib D3DCompiler.lib d2d1.lib dwrite.lib winmm.lib)

source_group("Engine" FILES ${SRC_ENGINE})


include_directories("3rdparty/spdlog/include")

