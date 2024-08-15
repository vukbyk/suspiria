# Copyright (C) 2024 For Vuks use only

#cmake_minimum_required(VERSION 3.5)
cmake_minimum_required(VERSION 3.9)
#cmake_minimum_required(VERSION 3.24)

project(suspiria LANGUAGES CXX)

##set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_C_STANDARD 99)

INCLUDE_DIRECTORIES(
    src/
)
file(
    GLOB SUSPIRIA_SRCGLOB
    src/*.h
    src/*.c
    src/*.hpp
    src/*.cpp
)


find_package(Qt6 REQUIRED COMPONENTS Core Gui OpenGL OpenGLWidgets Widgets)
qt_standard_project_setup()


set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#set(CMAKE_CXX_FLAGS "-O0")
#set(CMAKE_C_FLAGS "-O0")


INCLUDE_DIRECTORIES(
        dep/bullet/src/
        dep/bullet/src/BulletCollision/BroadphaseCollision
        dep/bullet/src/BulletCollision/CollisionDispatch
        dep/bullet/src/BulletCollision/CollisionShapes
        dep/bullet/src/BulletCollision/NarrowPhaseCollision
        dep/bullet/src/BulletDynamics/ConstraintSolver
        dep/bullet/src/BulletDynamics/Dynamics
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
INCLUDE_DIRECTORIES( dep/glm/ )
INCLUDE_DIRECTORIES( dep/stb/ )
#include_directories( assimp-3/include/ )
# set variables that are needed

set(ZLIB_LIBRARIES zlibstatic)
set(ENABLE_BOOST_WORKAROUND ON)
set(BUILD_STATIC_LIB ON)
set(BUILD_ASSIMP_TOOLS ON)
set(ASSIMP_BUILD_STATIC_LIB ON)
#Minimal build test, not working with emscripten, must use
#cmake -Wno-dev -DEMSCRIPTEN=1 -DCMAKE_TOOLCHAIN_FILE=/home/vuk/Desktop/Install/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake ../..
#      -DCMAKE_BUILD_TYPE=MinSizeRel -DASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT=No  -DASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT=No -DASSIMP_BUILD_OBJ_IMPORTER=yes -DASSIMP_BUILD_TESTS=no

OPTION ( ASSIMP_BUILD_TESTS
  "If the test suite for Assimp is built in addition to the library."
 OFF
)
OPTION( ASSIMP_BUILD_ASSIMP_TOOLS
  "If the supplementary tools for Assimp are built in addition to the library."
 OFF
)
set(ASSIMP_BUILD_ASSIMP_VIEW OFF)
set(ASSIMP_BUILD_STATIC_LIB OFF)
OPTION( ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF)
OPTION( ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)
# Disable all importers by default
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
set(ASSIMP_BUILD_OBJ_IMPORTER ON)    # Enable OBJ Importer
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
set(ASSIMP_BUILD_FBX_IMPORTER ON)    # Enable FBX Importer

add_subdirectory(dep/assimp)
add_subdirectory(dep/entt)

message("DEFAULT build")

qt_add_executable(suspiria
    ${SUSPIRIA_SRCGLOB}
    ${BULLET_SRCGLOB}
    dep/assimp
    resource.qrc
    shaders.qrc
#    glwindow.h
#    glwindow.cpp
#    logo.h
#    logo.cpp
#    ${TS_FILES}
)

set_target_properties(suspiria PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

#if(ANDROID)
#    find_package(OpenGLES 3.0 REQUIRED)
#    target_link_libraries(suspiria PRIVATE GLESv3)
#endif()

#target_link_libraries(suspiria PRIVATE Qt6::Widgets EnTT twbullet assimp)# LinearMath BulletCollision BulletDynamics )
target_link_libraries(suspiria PUBLIC
# Add the linker flag to the target for max size
    #${WASM_LINK_FLAGS}
    Qt::Core
    Qt::Gui
    Qt::OpenGL
    EnTT
    twbullet
    assimp
    ${XKBCOMMON_LIBRARIES}
)

qt_add_resources(suspiria "suspiria"
    PREFIX
        "/"
    FILES
        ${suspiria_resource_files}
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
