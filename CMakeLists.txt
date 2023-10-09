cmake_minimum_required(VERSION 3.14)

project(Destiny)

add_compile_definitions(UNICODE _UNICODE)
if (WIN7_SYSTEM_SUPPORT MATCHES ON)
    add_compile_definitions(_WIN32_WINNT=0x601)
endif()

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

file(GLOB SRC_MAIN src/main.cpp)
file(GLOB SRC_ENGINE src/Engine/*)
file(GLOB SRC_EDITOR src/Editor/*)
file(GLOB SRC_GRAPHICS src/Graphics/*)

set(CMAKE_AUTOMOC ON)      
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

source_group("Engine" FILES ${SRC_ENGINE})
source_group("Editor" FILES ${SRC_EDITOR})
source_group("Graphics" FILES ${SRC_GRAPHICS})

include_directories("3rdparty/spdlog/include")
include_directories("src/")

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

file(GLOB file_ui src/Editor/*.ui)
QT5_WRAP_UI(ui_FILES ${file_ui} )

add_executable(Destiny ${SRC_MAIN} ${SRC_ENGINE} ${SRC_EDITOR} ${SRC_GRAPHICS})
target_link_libraries(Destiny d3d11.lib dxgi.lib dxguid.lib D3DCompiler.lib d2d1.lib dwrite.lib winmm.lib Qt5::Core Qt5::Gui Qt5::Widgets)
