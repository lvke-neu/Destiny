cmake_minimum_required(VERSION 3.14)

project(Destiny)

add_compile_definitions(UNICODE _UNICODE)
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



#spdlog
include_directories("3rdparty/spdlog/include")

#rapidjson
include_directories("3rdparty/rapidjson/include")

#assimp
set(ASSIMP_BUILD_ZLIB ON)
set(ASSIMP_BUILD_ASSIMP_TOOLS ON)
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
   ELSEIF(MSVC142)
     SET(ASSIMP_MSVC_VERSION "vc142")
   ELSEIF(MSVC143)
     SET(ASSIMP_MSVC_VERSION "vc143")
   ENDIF()
 ENDIF()
if(TARGET assimp)
    target_compile_options(assimp PRIVATE /wd4100) # 忽略未使用参数警告
endif()


#rttr
set(BUILD_UNIT_TESTS OFF)
set(BUILD_EXAMPLES OFF)
set(BUILD_DOCUMENTATION OFF)
set(BUILD_INSTALLER OFF)
set(BUILD_PACKAGE OFF)
set(USE_PCH OFF)
set(CUSTOM_DOXYGEN_STYLE OFF)
add_subdirectory("3rdparty/rttr")

#freetype
set(FT_DISABLE_ZLIB ON)
set(DISABLE_FORCE_DEBUG_POSTFIX ON)
add_subdirectory("3rdparty/freetype")
set_target_properties(freetype PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/3rdparty/freetype/$<CONFIG>"
    LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/3rdparty/freetype/$<CONFIG>"
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/3rdparty/freetype/$<CONFIG>"
)

#bullet3
# 【修改点2：强制设置BUILD_SHARED_LIBS并写入CMake缓存，添加BT_SHARED_LIBS到target_compile_definitions】
add_compile_definitions(BT_SHARED_LIBS) # 全局定义导出宏
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries (DLL)" FORCE) # 强制传递给子项目
set(BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_EXTRAS OFF CACHE BOOL "" FORCE)
set(BUILD_CPU_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_OPENCL_DEMOS OFF CACHE BOOL "" FORCE)
set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "" FORCE)
add_subdirectory("3rdparty/bullet3")

# 【修改点3：定义Bullet3输出目录，并设置全局库搜索路径】
set(BULLET3_OUTPUT_DIR "${PROJECT_BINARY_DIR}/3rdparty/bullet3/$<CONFIG>")
link_directories("${BULLET3_OUTPUT_DIR}") # 让链接器找到Bullet3的.lib文件

# 【修改点4：完善Bullet3的target列表，添加WINDOWS_EXPORT_ALL_SYMBOLS确保导出符号】
foreach(bullet_target LinearMath BulletCollision BulletDynamics BulletSoftBody Bullet3Common Bullet3Geometry Bullet3Dynamics Bullet3Collision Bullet3OpenCL_clew BulletInverseDynamics Bullet2FileLoader)
    if(TARGET ${bullet_target})
        set_target_properties(${bullet_target} PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${BULLET3_OUTPUT_DIR}"
            LIBRARY_OUTPUT_DIRECTORY "${BULLET3_OUTPUT_DIR}"
            RUNTIME_OUTPUT_DIRECTORY "${BULLET3_OUTPUT_DIR}"
            WINDOWS_EXPORT_ALL_SYMBOLS ON # 强制导出所有符号，兜底方案
        )
    endif()
endforeach()

# curl
set(BUILD_CURL_EXE OFF)
set(BUILD_TESTING OFF)

# 禁用各种协议
set(CURL_DISABLE_LDAP ON)
set(CURL_DISABLE_TELNET ON)
set(CURL_DISABLE_DICT ON)
set(CURL_DISABLE_FILE ON)
set(CURL_DISABLE_GOPHER ON)
set(CURL_DISABLE_IMAP ON)
set(CURL_DISABLE_POP3 ON)
set(CURL_DISABLE_RTSP ON)
set(CURL_DISABLE_SMTP ON)
set(CURL_DISABLE_TFTP ON)
set(CURL_DISABLE_WEBSOCKET ON)

# SSL 设置
set(CURL_USE_SCHANNEL ON)  # Windows 原生 SSL
set(CURL_STATICLIB OFF)    # 编译为动态库

# 禁用所有可选压缩库
set(CURL_ZLIB OFF)
set(CURL_BROTLI OFF)
set(CURL_ZSTD OFF)

# 禁用 HTTP/2 支持
set(CURL_HTTP2 OFF)
set(CURL_NGHTTP2 OFF)

# 如果想禁用 HTTP/3，也可以
set(CURL_HTTP3 OFF)

# 添加子目录
add_subdirectory("3rdparty/curl")



# include
include_directories("3rdparty/curl/include")



if(MSVC)
    foreach(tgt assimp rttr_core freetype LinearMath BulletCollision BulletDynamics BulletSoftBody)
        if(TARGET ${tgt})
            # Debug 配置使用 /MDd
            target_compile_options(${tgt} PRIVATE
                $<$<CONFIG:Debug>:/MDd>
                $<$<CONFIG:Release>:/MD>
                $<$<CONFIG:RelWithDebInfo>:/MD>
                $<$<CONFIG:MinSizeRel>:/MD>
            )
        endif()
    endforeach()
endif()



if(MSVC)
    set_property(TARGET assimp PROPERTY FOLDER "ThirdParty")
    set_property(TARGET rttr_core PROPERTY FOLDER "ThirdParty")
    set_property(TARGET freetype PROPERTY FOLDER "ThirdParty")

    set_property(TARGET LinearMath PROPERTY FOLDER "ThirdParty/Bullet3")
    set_property(TARGET BulletCollision PROPERTY FOLDER "ThirdParty/Bullet3")
    set_property(TARGET BulletDynamics PROPERTY FOLDER "ThirdParty/Bullet3")
    set_property(TARGET BulletSoftBody PROPERTY FOLDER "ThirdParty/Bullet3")
    set_property(TARGET Bullet3Common PROPERTY FOLDER "ThirdParty/Bullet3")
    set_property(TARGET Bullet3Geometry PROPERTY FOLDER "ThirdParty/Bullet3")
    set_property(TARGET Bullet3Dynamics PROPERTY FOLDER "ThirdParty/Bullet3")
    set_property(TARGET Bullet2FileLoader PROPERTY FOLDER "ThirdParty/Bullet3")
    set_property(TARGET Bullet3Collision PROPERTY FOLDER "ThirdParty/Bullet3")
    set_property(TARGET Bullet3OpenCL_clew PROPERTY FOLDER "ThirdParty/Bullet3")
    set_property(TARGET BulletInverseDynamics PROPERTY FOLDER "ThirdParty/Bullet3")
endif()




# include
include_directories(
    "src"
    "3rdparty/bullet3/src"
    "3rdparty/bullet3/src/LinearMath"
    "3rdparty/bullet3/src/BulletCollision"
    "3rdparty/bullet3/src/BulletDynamics"
)


include_directories("src/")

file(GLOB SRC_MAIN src/*)
file(GLOB SRC_ENGINE src/Engine/*)
file(GLOB SRC_ENGINE_DETAIL src/Engine/Detail/*)
file(GLOB SRC_EDITOR src/Editor/*)
file(GLOB SRC_EDITOR_IMGUI src/Editor/ImGui/*)
file(GLOB SRC_GRAPHICS src/Graphics/*)
file(GLOB SRC_GRAPHICS_GRAPHICPIPELINE src/Graphics/GraphicsPipeline/*)
file(GLOB SRC_GRAPHICS_GUI src/Graphics/Gui/*)
file(GLOB SRC_GRAPHICS_THIRDPARTY_STBIMAGE src/Graphics/ThirdParty/stb_image/*)
file(GLOB SRC_GRAPHICS_THIRDPARTY_DIRECTXTEX src/Graphics/ThirdParty/DirectXTex/*)
file(GLOB SRC_Math src/Math/*)
file(GLOB SRC_SCENE src/Scene/*)
file(GLOB SRC_SCENE_RUNTIMEEFFECT src/Scene/RuntimeEffect/*)
file(GLOB SRC_SCENE_RUNTIMEEFFECT_PARTICLE src/Scene/RuntimeEffect/Particle/*)
file(GLOB SRC_SCENE_RUNTIMEEFFECT_GPUBVH2BUILDER src/Scene/RuntimeEffect/GpuBvh2Builder/*)
file(GLOB SRC_SCENE_MODEL src/Scene/Model/*)
file(GLOB SRC_PHYSICS src/Physics/*)
file(GLOB SRC_PHYSICS_RIGIDBODYVISUAL src/Physics/RigidBodyVisual/*)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

source_group("Engine" FILES ${SRC_ENGINE})
source_group("Engine\\Detail" FILES ${SRC_ENGINE_DETAIL})
source_group("Editor" FILES ${SRC_EDITOR})
source_group("Editor\\ImGui" FILES ${SRC_EDITOR_IMGUI})
source_group("Graphics" FILES ${SRC_GRAPHICS})
source_group("Graphics\\GraphicsPipeline" FILES ${SRC_GRAPHICS_GRAPHICPIPELINE})
source_group("Graphics\\Gui" FILES ${SRC_GRAPHICS_GUI})
source_group("Graphics\\ThirdParty\\stb_image" FILES ${SRC_GRAPHICS_THIRDPARTY_STBIMAGE})
source_group("Graphics\\ThirdParty\\DirectXTex" FILES ${SRC_GRAPHICS_THIRDPARTY_DIRECTXTEX})
source_group("Math" FILES ${SRC_Math})
source_group("Scene" FILES ${SRC_SCENE})
source_group("Scene\\RuntimeEffect" FILES ${SRC_SCENE_RUNTIMEEFFECT})
source_group("Scene\\RuntimeEffect\\Particle" FILES ${SRC_SCENE_RUNTIMEEFFECT_PARTICLE})
source_group("Scene\\RuntimeEffect\\GpuBvh2Builder" FILES ${SRC_SCENE_RUNTIMEEFFECT_GPUBVH2BUILDER})
source_group("Scene\\Model" FILES ${SRC_SCENE_MODEL})
source_group("Physice" FILES ${SRC_PHYSICS})
source_group("Physice\\RigidBodyVisual" FILES ${SRC_PHYSICS_RIGIDBODYVISUAL})

add_executable(
    Destiny WIN32 ${SRC_MAIN} 
    ${SRC_ENGINE} ${SRC_ENGINE_DETAIL} 
    ${SRC_EDITOR} ${SRC_EDITOR_IMGUI} 
    ${SRC_GRAPHICS} 
    ${SRC_GRAPHICS_GRAPHICPIPELINE} 
    ${SRC_GRAPHICS_MODEL} 
    ${SRC_GRAPHICS_GUI} 
    ${SRC_GRAPHICS_THIRDPARTY_STBIMAGE} 
    ${SRC_GRAPHICS_THIRDPARTY_DIRECTXTEX} 
    ${SRC_Math} 
    ${SRC_SCENE} 
    ${SRC_SCENE_RUNTIMEEFFECT}
    ${SRC_SCENE_RUNTIMEEFFECT_PARTICLE}
    ${SRC_SCENE_RUNTIMEEFFECT_GPUBVH2BUILDER}
    ${SRC_SCENE_MODEL}
    ${SRC_PHYSICS}
    ${SRC_PHYSICS_RIGIDBODYVISUAL}
)

target_link_libraries(Destiny
    assimp
    rttr_core
    freetype
    BulletDynamics
    BulletCollision
    LinearMath
    libcurl
    d3d11.lib dxgi.lib dxguid.lib D3DCompiler.lib d2d1.lib dwrite.lib winmm.lib
)


add_custom_command(TARGET Destiny
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${PROJECT_SOURCE_DIR}/builtin/ 
                    ${PROJECT_BINARY_DIR}/$(Configuration)/builtin/
                   COMMENT "Copying builtin to  ${PROJECT_BINARY_DIR}/$(Configuration)")
add_custom_command(TARGET Destiny
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   ${PROJECT_BINARY_DIR}/3rdparty/assimp/bin/$(Configuration)/assimp-${ASSIMP_MSVC_VERSION}-mt.dll
                    ${PROJECT_BINARY_DIR}/$(Configuration)
                   COMMENT "Copying file assimp-${ASSIMP_MSVC_VERSION}-mt.dll to ${PROJECT_BINARY_DIR}/$(Configuration)")
add_custom_command(TARGET Destiny
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   ${PROJECT_BINARY_DIR}/3rdparty/rttr/bin/$(Configuration)/rttr_core.dll
                    ${PROJECT_BINARY_DIR}/$(Configuration)
                   COMMENT "Copying file rttr_core.dll to ${PROJECT_BINARY_DIR}/$(Configuration)")
add_custom_command(TARGET Destiny
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   ${PROJECT_BINARY_DIR}/3rdparty/freetype/$(Configuration)/freetype.dll
                    ${PROJECT_BINARY_DIR}/$(Configuration)
                   COMMENT "Copying file freetype.dll to ${PROJECT_BINARY_DIR}/$(Configuration)")
add_custom_command(TARGET Destiny
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   ${PROJECT_BINARY_DIR}/lib/$(Configuration)/libcurl.dll
                    ${PROJECT_BINARY_DIR}/$(Configuration)
                   COMMENT "Copying file curl.dll to ${PROJECT_BINARY_DIR}/$(Configuration)")


add_custom_command(TARGET Destiny
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   ${PROJECT_BINARY_DIR}/3rdparty/bullet3/$(Configuration)/BulletDynamics.dll
                    ${PROJECT_BINARY_DIR}/$(Configuration)
                   COMMENT "Copying file BulletDynamics.dll to ${PROJECT_BINARY_DIR}/$(Configuration)")
add_custom_command(TARGET Destiny
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   ${PROJECT_BINARY_DIR}/3rdparty/bullet3/$(Configuration)/BulletCollision.dll
                    ${PROJECT_BINARY_DIR}/$(Configuration)
                   COMMENT "Copying file BulletCollision.dll to ${PROJECT_BINARY_DIR}/$(Configuration)")
add_custom_command(TARGET Destiny
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   ${PROJECT_BINARY_DIR}/3rdparty/bullet3/$(Configuration)/LinearMath.dll
                    ${PROJECT_BINARY_DIR}/$(Configuration)
                   COMMENT "Copying file LinearMath.dll to ${PROJECT_BINARY_DIR}/$(Configuration)")