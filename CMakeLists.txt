cmake_minimum_required(VERSION 3.14)

project(Destiny)

add_compile_definitions(UNICODE _UNICODE)
if (WIN7_SYSTEM_SUPPORT MATCHES ON)
    add_compile_definitions(_WIN32_WINNT=0x601)
endif()

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

file(GLOB SRC_MAIN src/main.cpp)
file(GLOB SRC_ENGINE src/Engine/*)
file(GLOB SRC_ENGINE_DETAIL src/Engine/Detail/*)
file(GLOB SRC_EDITOR src/Editor/*)
file(GLOB SRC_GRAPHICS src/Graphics/*)
file(GLOB ASSETS_HLSL assets/HLSL/*)
file(GLOB ASSETS_TEXTURE assets/Texture/*)

set(CMAKE_AUTOMOC ON)      
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

source_group("Engine" FILES ${SRC_ENGINE})
source_group("Engine\\Detail" FILES ${SRC_ENGINE_DETAIL})
source_group("Editor" FILES ${SRC_EDITOR})
source_group("Graphics" FILES ${SRC_GRAPHICS})
source_group("assets\\HLSL" FILES ${ASSETS_HLSL})
source_group("assets\\Texture" FILES ${ASSETS_TEXTURE})

include_directories("3rdparty/spdlog/include")
include_directories("3rdparty/rapidxml")
include_directories("src/")

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

file(GLOB file_ui src/Editor/*.ui)
QT5_WRAP_UI(ui_FILES ${file_ui} )

add_executable(Destiny ${SRC_MAIN} ${SRC_ENGINE} ${SRC_ENGINE_DETAIL} ${SRC_EDITOR} ${SRC_GRAPHICS} ${ASSETS_HLSL} ${ASSETS_TEXTURE})
target_link_libraries(Destiny d3d11.lib dxgi.lib dxguid.lib D3DCompiler.lib d2d1.lib dwrite.lib winmm.lib Qt5::Core Qt5::Gui Qt5::Widgets)

foreach(HLSL_FILE ${ASSETS_HLSL})

    get_filename_component(HLSL_FDIR ${HLSL_FILE} DIRECTORY)
    get_filename_component(HLSL_FBASENAME_WE ${HLSL_FILE} NAME_WE)
    string(CONCAT HLSL_FNAME_WE ${HLSL_FDIR} / ${HLSL_FBASENAME_WE})

    string(LENGTH ${HLSL_FBASENAME_WE} LEN_FNAME_WE)
    
    math(EXPR LEN_FNAME_WE "${LEN_FNAME_WE}-2")
    string(SUBSTRING ${HLSL_FBASENAME_WE} ${LEN_FNAME_WE} 2 ENTRY_POINT)
    string(TOLOWER ${ENTRY_POINT} SHADER_TYPE)

    if ("${SHADER_TYPE}" STREQUAL "vs") 
        set(SHADER_TYPE "Vertex")
    elseif("${SHADER_TYPE}" STREQUAL "hs") 
        set(SHADER_TYPE "Hull")
    elseif("${SHADER_TYPE}" STREQUAL "ds") 
        set(SHADER_TYPE "Domain")
    elseif("${SHADER_TYPE}" STREQUAL "gs") 
        set(SHADER_TYPE "Geometry")
    elseif("${SHADER_TYPE}" STREQUAL "ps")
        set(SHADER_TYPE "Pixel")
    elseif("${SHADER_TYPE}" STREQUAL "cs")
        set(SHADER_TYPE "Compute")
    endif()
    set_source_files_properties(${HLSL_FILE} PROPERTIES
                                VS_SHADER_OBJECT_FILE_NAME HLSL/${HLSL_FBASENAME_WE}.cso
                                VS_SHADER_TYPE ${SHADER_TYPE}
                                VS_SHADER_MODEL 5.0
                                VS_SHADER_ENTRYPOINT ${ENTRY_POINT}
                                VS_SHADER_DISABLE_OPTIMIZATIONS $<$<CONFIG:Debug>:true>
                                VS_SHADER_ENABLE_DEBUG $<$<CONFIG:Debug>:true>)
endforeach()

file(COPY assets/Texture DESTINATION ${CMAKE_CURRENT_BINARY_DIR})