#For try to make wasm file smmaller change to 3.9
cmake_minimum_required(VERSION 3.9)
#cmake_minimum_required(VERSION 3.5)
#cmake_minimum_required(VERSION 3.24)

project(suspiria LANGUAGES CXX)

if(EMSCRIPTEN)
#    set(CMAKE_TOOLCHAIN_FILE="/home/vuk/Desktop/Install/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")
    include(wasm.cmake)
    ##wsam file size include
    #include(CheckIPOSupported)
    #check_ipo_supported(RESULT supported OUTPUT error)
    #wsam file size include
endif()

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


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

##set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_C_STANDARD 99)

#set(CMAKE_CXX_FLAGS "-O0")
#set(CMAKE_C_FLAGS "-O0")


## search all source and header files deluje kao da mogu iskoristiti
#   file(GLOB source_files *.c *.cpp *.qrc)
#   file(GLOB header_files *.h *.hpp *.txt)

find_package(Qt6 REQUIRED COMPONENTS Core Gui OpenGL OpenGLWidgets Widgets)
#set(TS_FILES suspiria_en_US.ts)

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


if(EMSCRIPTEN)
  message("EMSCRIPTEN build")

  set(CMAKE_EXECUTABLE_SUFFIX ".wasm")
  # Set the initial memory size flag
  # Set(WASM_LINK_FLAGS "-Wl,--initial-memory=100000000")


  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
  qt_add_executable(suspiria
    main.cpp
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
else()
  message("DEFAULT build")
  qt_add_executable(suspiria
    main.cpp
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
#    #wsam file size
#    if( supported )
#        message(STATUS "IPO / LTO enabled")
#        set_property(TARGET suspiria PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
#    else()
#        message(STATUS "IPO / LTO not supported: <${error}>")
#    endif()
    #wsam file size
endif()

#if(ANDROID)
#    find_package(OpenGLES 3.0 REQUIRED)
#    target_link_libraries(suspiria PRIVATE GLESv3)
#endif()

if(EMSCRIPTEN)
#      link_qt_static(suspiria)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
#    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s INITIAL_MEMORY=268435456 -s MAXIMUM_MEMORY=268435456 -s ALLOW_MEMORY_GROWTH=1")
# Set linker flags for initial memory and memory growth
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s INITIAL_MEMORY=268435456")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ALLOW_MEMORY_GROWTH=1")

#    # Linking flags
#    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s INITIAL_MEMORY=128MB -s ALLOW_MEMORY_GROWTH=1")
#        set_target_properties(suspiria PROPERTIES LINK_FLAGS "-s TOTAL_MEMORY=4GB")
#        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s INITIAL_MEMORY=134217728")
#        target_link_options(suspiria PRIVATE "--linker-option=-s" "--linker-option=MAXIMUM_MEMORY=4gb")
#        set_target_properties(suspiria PROPERTIES LINK_FLAGS "-s MAXIMUM_MEMORY=4gb")
#      target_link_options(suspiria PRIVATE "--memory-init-size=4294967296")
endif()

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
