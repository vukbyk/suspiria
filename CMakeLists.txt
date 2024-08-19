# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.17) #big resources supported but i dont use it
# cmake_minimum_required(VERSION 3.24)
# cmake_minimum_required(VERSION 3.6)
project(suspiria LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui OpenGL OpenGLWidgets Widgets)

### Enable console output on Windows
if(WIN32)
    set_target_properties(suspiria PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
endif()

if(ANDROID OR EMSCRIPTEN)
    add_definitions(-DGLESVuk)
endif()

### Set logging rules to ensure qDebug is enabled
add_definitions(-DQT_MESSAGELOGCONTEXT)
add_definitions(-DQT_DEBUG)



################### Include Assimp  BEGIN #########################
if(ANDROID)
    find_package(Threads REQUIRED)
endif()

set(BUILD_SHARED_LIBS OFF)  # Build Assimp as a static library

set(ASSIMP_BUILD_TESTS OFF)
# set(BUILD_ASSIMP_TOOLS OFF)
set(ASSIMP_BUILD_ASSIMP_VIEW OFF)
set(ASSIMP_BUILD_STATIC_LIB OFF)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)
# Usage: Use option() when you want to provide a default value
# that can be easily overridden by the user.
# OPTION( ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF)#
# OPTION( ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)

# set(ASSIMP_BUILD_3DS_IMPORTER OFF CACHE BOOL "Disable 3DS importer")
# set(ASSIMP_BUILD_FBX_IMPORTER OFF CACHE BOOL "Disable FBX importer")
# set(ASSIMP_BUILD_GLTF_IMPORTER OFF CACHE BOOL "Disable glTF importer")

# Disable all importers by default
set(ASSIMP_BUILD_OBJ_IMPORTER ON)    # Enable OBJ Importer
set(ASSIMP_BUILD_FBX_IMPORTER OFF)    # Enable FBX Importer
set(ASSIMP_BUILD_3DS_IMPORTER OFF)
set(ASSIMP_BUILD_AC_IMPORTER OFF)
set(ASSIMP_BUILD_ASE_IMPORTER OFF)
set(ASSIMP_BUILD_ASSBIN_IMPORTER OFF)
set(ASSIMP_BUILD_ASSXML_IMPORTER OFF)
set(ASSIMP_BUILD_B3D_IMPORTER OFF)
set(ASSIMP_BUILD_BLEND_IMPORTER OFF)
set(ASSIMP_BUILD_BVH_IMPORTER OFF)
set(ASSIMP_BUILD_C4D_IMPORTER OFF)
set(ASSIMP_BUILD_COB_IMPORTER OFF)
set(ASSIMP_BUILD_COLLADA_IMPORTER OFF)
set(ASSIMP_BUILD_CSM_IMPORTER OFF)
set(ASSIMP_BUILD_DXF_IMPORTER OFF)
set(ASSIMP_BUILD_HMP_IMPORTER OFF)
set(ASSIMP_BUILD_IFC_IMPORTER OFF)
set(ASSIMP_BUILD_IRRMESH_IMPORTER OFF)
set(ASSIMP_BUILD_IRR_IMPORTER OFF)
set(ASSIMP_BUILD_LWO_IMPORTER OFF)
set(ASSIMP_BUILD_LWS_IMPORTER OFF)
set(ASSIMP_BUILD_MD2_IMPORTER OFF)
set(ASSIMP_BUILD_MD3_IMPORTER OFF)
set(ASSIMP_BUILD_MD5_IMPORTER OFF)
set(ASSIMP_BUILD_MDC_IMPORTER OFF)
set(ASSIMP_BUILD_MDL_IMPORTER OFF)
set(ASSIMP_BUILD_MS3D_IMPORTER OFF)
set(ASSIMP_BUILD_NDO_IMPORTER OFF)
set(ASSIMP_BUILD_NFF_IMPORTER OFF)
set(ASSIMP_BUILD_OFF_IMPORTER OFF)
set(ASSIMP_BUILD_OPENCTM_IMPORTER OFF)
set(ASSIMP_BUILD_OPENGEX_IMPORTER OFF)
set(ASSIMP_BUILD_PLY_IMPORTER OFF)
set(ASSIMP_BUILD_Q3BSP_IMPORTER OFF)
set(ASSIMP_BUILD_Q3D_IMPORTER OFF)
set(ASSIMP_BUILD_RAW_IMPORTER OFF)
set(ASSIMP_BUILD_SIB_IMPORTER OFF)
set(ASSIMP_BUILD_SMD_IMPORTER OFF)
set(ASSIMP_BUILD_STL_IMPORTER OFF)
set(ASSIMP_BUILD_TERRAGEN_IMPORTER OFF)
set(ASSIMP_BUILD_X3D_IMPORTER OFF)
set(ASSIMP_BUILD_XGL_IMPORTER OFF)



# Add Assimp as a subdirectory if it's included in your project
add_subdirectory(dep/assimp)  # Adjust the path if Assimp is located elsewhere
################### Include Assimp END ####################

############### Bullet Physics BEGIN ################
set(BUILD_STATIC_LIB ON)

INCLUDE_DIRECTORIES(
        dep/bullet/src/
        dep/bullet/src/BulletCollision/BroadphaseCollision
        dep/bullet/src/BulletCollision/CollisionDispatch
        dep/bullet/src/BulletCollision/CollisionShapes
        dep/bullet/src/BulletCollision/NarrowPhaseCollision
        dep/bullet/src/BulletDynamics/ConstraintSolver
        dep/bullet/src/BulletDynamics/Dynamics/
        dep/bullet/src/BulletDynamics/Vehicle
#       dep/bullet/src/BulletSoftBody/
        dep/bullet/src/LinearMath
)

FILE(GLOB BULLET_SRCGLOB
        dep/bullet/src/BulletCollision/BroadphaseCollision/*.cpp
        dep/bullet/src/BulletCollision/CollisionDispatch/*.cpp
        dep/bullet/src/BulletCollision/CollisionShapes/*.cpp
        dep/bullet/src/BulletCollision/NarrowPhaseCollision/*.cpp
        dep/bullet/src/BulletDynamics/ConstraintSolver/*.cpp
        dep/bullet/src/BulletDynamics/Dynamics/*.cpp
        dep/bullet/src/BulletDynamics/Vehicle/*.cpp
#       dep/bullet/src/BulletSoftBody/*.cpp
        dep/bullet/src/LinearMath/*.cpp
)
ADD_LIBRARY(twbullet STATIC ${BULLET_SRCGLOB})
qt_standard_project_setup()

############# Bullet Physics END ##############

add_subdirectory(dep/entt)

INCLUDE_DIRECTORIES( dep/glm/ )
INCLUDE_DIRECTORIES( dep/stb/ )

INCLUDE_DIRECTORIES(
    src/
    ${ASSIMP_INCLUDE_DIRS}  # Include Assimp headers
)
file(
    GLOB SUSPIRIA_SRCGLOB
    src/*.h
    src/*.c
    src/*.hpp
    src/*.cpp
)

#for qrc files
# set(CMAKE_AUTORCC ON)

qt_add_executable(suspiria
    ${SUSPIRIA_SRCGLOB}
    ${BULLET_SRCGLOB}
    # dev/assimp  # probably not needed
    # assets.qrc
    # resource.qrc
    # shaders.qrc
)

set_target_properties(suspiria PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

if(ANDROID)
message("!!!!!!!!! ANDROID target_link_libraries !!!!!!!!!!")
    target_link_libraries(suspiria PRIVATE #PUBLIC??
        Qt6::Core
        Qt6::Gui
        Qt6::OpenGL
        twbullet
        assimp
        EnTT
        ${CMAKE_THREAD_LIBS_INIT} #Assimp android needs it for -pthreads
)
else()
    message("!!!!!!!!! DEFUALT target_link_libraries !!!!!!!!!!")
    target_link_libraries(suspiria PRIVATE #PUBLIC??
        Qt6::Core
        Qt6::Gui
        Qt6::OpenGL
        twbullet
        assimp
        EnTT
)
endif()
# #Resources:

# set(suspiria_resource_files
#     "qtlogo.png"
# )


qt_add_resources(suspiria "hellogles3"
    PREFIX "/resources/"
    FILES ${suspiria_resource_files}
)
file( GLOB Assets_files_GLOB ${CMAKE_CURRENT_SOURCE_DIR}/assets/*)


# Use absolute paths with resource aliasing
set(suspiria_assets_files ${Assets_files_GLOB})

foreach(resource_file ${suspiria_assets_files})
    get_filename_component(resource_name ${resource_file} NAME)
    set_source_files_properties(${resource_file} PROPERTIES QT_RESOURCE_ALIAS ${resource_name})
endforeach()

qt_add_resources(suspiria "suspiria_assets_files"
    PREFIX
        "/assets/"
    FILES
        # assets.qrc #for qrc? #probably not needed for filws
        ${suspiria_assets_files}
)


file(
    GLOB shader_files_GLOB
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*
)

# Use absolute paths with resource aliasing
set(suspiria_shaders_files
${shader_files_GLOB}
# "${CMAKE_CURRENT_SOURCE_DIR}/resources/qtlogo.png"
# "${CMAKE_CURRENT_SOURCE_DIR}/resources/tllogo.png"
)

foreach(resource_file ${suspiria_shaders_files})
    get_filename_component(resource_name ${resource_file} NAME)
    set_source_files_properties(${resource_file} PROPERTIES QT_RESOURCE_ALIAS ${resource_name})

endforeach()


qt_add_resources(suspiria "suspiria_shaders_files"
    PREFIX
        "/shaders/"
    FILES
        ${suspiria_shaders_files}
)


install(TARGETS suspiria
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET suspiria
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

# message("!!!!!!!!!!!!!!!!!!!")
# message(${})
# message("!!!!!!!!!!!!!!!!!!!")
