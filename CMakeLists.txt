cmake_minimum_required(VERSION 3.14)

project(Destiny)

add_compile_definitions(UNICODE _UNICODE)
if (WIN7_SYSTEM_SUPPORT MATCHES ON)
    add_compile_definitions(_WIN32_WINNT=0x601)
endif()

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")

######################使用多处理器编译########################
IF (WIN32)
		if(MSVC)
				OPTION(USE_MP "use multiple" ON)
				OPTION(ProjectConfig_Global_COMPILE_FLAGS_WITH_MP 
					"Set The Global Option COMPILE_FLAGS /MP to target." ON)
				if(ProjectConfig_Global_COMPILE_FLAGS_WITH_MP OR USE_MP)
					set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
					set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
				endif()
				set(VS_STARTUP_PROJECT ${PROJECT_NAME})
		ENDIF(MSVC)
ENDIF()

#assimp
set(ASSIMP_BUILD_ZLIB ON)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_INSTALL OFF)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF)
add_subdirectory("3rdparty/assimp")
 IF(MSVC_TOOLSET_VERSION)
    SET(MSVC_PREFIX "vc${MSVC_TOOLSET_VERSION}")
    SET(ASSIMP_MSVC_VERSION ${MSVC_PREFIX})
 ELSE()
   IF(MSVC12)
     SET(ASSIMP_MSVC_VERSION "vc120")
   ELSEIF(MSVC14)
     SET(ASSIMP_MSVC_VERSION "vc140")
   ELSEIF(MSVC15)
     SET(ASSIMP_MSVC_VERSION "vc141")
   ELSEIF(MSV142)
     SET(ASSIMP_MSVC_VERSION "vc142")
   ELSEIF(MSVC143)
     SET(ASSIMP_MSVC_VERSION "vc143")
   ENDIF()
 ENDIF()

file(GLOB SRC_MAIN src/*)
file(GLOB SRC_ENGINE src/Engine/*)
file(GLOB SRC_ENGINE_DETAIL src/Engine/Detail/*)
file(GLOB SRC_EDITOR src/Editor/*)
file(GLOB SRC_EDITOR_IMGUI src/Editor/ImGui/*)
file(GLOB SRC_GRAPHICS src/Graphics/*)
file(GLOB SRC_GRAPHICS_GRAPHICPIPELINE src/Graphics/GraphicsPipeline/*)
file(GLOB SRC_GRAPHICS_GUI src/Graphics/Gui/*)
file(GLOB SRC_Math src/Math/*)
file(GLOB SRC_SCENE src/Scene/*)
file(GLOB SRC_SCENE_RUNTIMEEFFECT src/Scene/RuntimeEffect/*)
file(GLOB SRC_SCENE_MODEL src/Scene/Model/*)
file(GLOB BUILTIN_RENDERER builtin/renderer/*)
file(GLOB BUILTIN_TEXTURE builtin/texture/*)
file(GLOB BUILTIN_MODEL builtin/model/*)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

source_group("Engine" FILES ${SRC_ENGINE})
source_group("Engine\\Detail" FILES ${SRC_ENGINE_DETAIL})
source_group("Editor" FILES ${SRC_EDITOR})
source_group("Editor\\ImGui" FILES ${SRC_EDITOR_IMGUI})
source_group("Graphics" FILES ${SRC_GRAPHICS})
source_group("Graphics\\GraphicsPipeline" FILES ${SRC_GRAPHICS_GRAPHICPIPELINE})
source_group("Graphics\\Gui" FILES ${SRC_GRAPHICS_GUI})
source_group("Math" FILES ${SRC_Math})
source_group("Scene" FILES ${SRC_SCENE})
source_group("Scene\\RuntimeEffect" FILES ${SRC_SCENE_RUNTIMEEFFECT})
source_group("Scene\\Model" FILES ${SRC_SCENE_MODEL})
source_group("builtin\\renderer" FILES ${BUILTIN_RENDERER})
source_group("builtin\\texture" FILES ${BUILTIN_TEXTURE})
source_group("builtin\\model" FILES ${BUILTIN_MODEL})

include_directories("src/")
include_directories("3rdparty/spdlog/include")
include_directories("3rdparty/rttr/include")
include_directories("3rdparty/freetype/include")
include_directories("3rdparty/assimp/include")
include_directories("${PROJECT_BINARY_DIR}/3rdparty/assimp/include")

link_directories("3rdparty/rttr/lib/")
link_libraries(rttr_core.lib)
link_directories("3rdparty/freetype/lib/")
link_libraries(freetype.lib)
link_directories("${PROJECT_BINARY_DIR}/3rdparty/assimp/lib/${CMAKE_BUILD_TYPE}/")
link_libraries(assimp-${ASSIMP_MSVC_VERSION}-mt.lib)

add_executable(Destiny WIN32 ${SRC_MAIN} 
${SRC_ENGINE} ${SRC_ENGINE_DETAIL} 
${SRC_EDITOR} ${SRC_EDITOR_IMGUI} 
${SRC_GRAPHICS} 
${SRC_GRAPHICS_GRAPHICPIPELINE} 
${SRC_GRAPHICS_MODEL} 
${SRC_GRAPHICS_GUI} 
${SRC_Math} 
${SRC_SCENE} 
${SRC_SCENE_RUNTIMEEFFECT}
${SRC_SCENE_MODEL}
${BUILTIN} ${BUILTIN_TEXTURE})
target_link_libraries(Destiny d3d11.lib dxgi.lib dxguid.lib D3DCompiler.lib d2d1.lib dwrite.lib winmm.lib)
add_dependencies(Destiny assimp)

add_custom_command(TARGET Destiny
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${PROJECT_SOURCE_DIR}/builtin/ 
                    ${PROJECT_BINARY_DIR}/$(Configuration)/builtin/
                   COMMENT "Copying builtin to  ${PROJECT_BINARY_DIR}/$(Configuration)")


add_custom_command(TARGET Destiny
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   ${PROJECT_BINARY_DIR}/3rdparty/assimp/bin/Debug/assimp-${ASSIMP_MSVC_VERSION}-mt.dll
                    ${PROJECT_BINARY_DIR}/$(Configuration)
                   COMMENT "Copying file assimp-${ASSIMP_MSVC_VERSION}-mt.dll to ${PROJECT_BINARY_DIR}/$(Configuration)")
