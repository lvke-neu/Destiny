cmake_minimum_required(VERSION 3.14)

project(Destiny)

add_compile_definitions(UNICODE _UNICODE)
if (WIN7_SYSTEM_SUPPORT MATCHES ON)
    add_compile_definitions(_WIN32_WINNT=0x601)
endif()

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

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

file(GLOB SRC_MAIN src/*)
file(GLOB SRC_ENGINE src/Engine/*)
file(GLOB SRC_ENGINE_DETAIL src/Engine/Detail/*)
file(GLOB SRC_EDITOR src/Editor/*)
file(GLOB SRC_EDITOR_IMGUI src/Editor/ImGui/*)
file(GLOB SRC_GRAPHICS src/Graphics/*)
file(GLOB SRC_GRAPHICS_GRAPHICPIPELINE src/Graphics/GraphicsPipeline/*)
file(GLOB SRC_Math src/Math/*)
file(GLOB SRC_SCENE src/Scene/*)
file(GLOB SRC_SCENE_MODEL src/Scene/Model/*)
file(GLOB SRC_SCENE_RUNTIMEEFFECT src/Scene/RuntimeEffect/*)
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
include_directories("3rdparty/assimp/include")

link_directories("3rdparty/rttr/lib/")
link_libraries(rttr_core.lib)
link_directories("3rdparty/assimp/lib/")
link_libraries(assimp.lib)

add_executable(Destiny WIN32 ${SRC_MAIN} 
${SRC_ENGINE} ${SRC_ENGINE_DETAIL} 
${SRC_EDITOR} ${SRC_EDITOR_IMGUI} 
${SRC_GRAPHICS} 
${SRC_GRAPHICS_GRAPHICPIPELINE} 
${SRC_Math} 
${SRC_SCENE} ${SRC_SCENE_MODEL}
${SRC_SCENE} ${SRC_SCENE_RUNTIMEEFFECT}
${BUILTIN} ${BUILTIN_RENDERER} ${BUILTIN_TEXTURE})
target_link_libraries(Destiny d3d11.lib dxgi.lib dxguid.lib D3DCompiler.lib d2d1.lib dwrite.lib winmm.lib)

file(COPY builtin DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY builtin DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
