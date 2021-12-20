#For try to make wasm file smmaller change to 3.9
#cmake_minimum_required(VERSION 3.9)
cmake_minimum_required(VERSION 3.5)

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

#       set(CMAKE_INCLUDE_CURRENT_DIR ON)
#       set(CMAKE_AUTOMOC ON)
#       set(CMAKE_AUTORCC ON)
#       set(CMAKE_AUTOUIC ON)
#       find_package(Qt5Widgets REQUIRED)
#       find_package(Qt5WebEngineWidgets REQUIRED)
#       find_package(Qt5WebChannel REQUIRED)


#       #qt5_add_resources(COLYOO_QT_RESOURCES app.qrc web.qrc)

#       message(STATUS "SLEIDEBUG:${COLYOO_QT_RESOURCES}")

if(EMSCRIPTEN)
    set(Qt6_DIR "/home/vuk/Qt/6.2.2/wasm_32/lib/cmake/Qt6")
endif()
#if(EMSCRIPTEN)
#    set(Qt5_DIR "/home/vuk/Qt/5.15.0/wasm_32/lib/cmake/Qt5")
##    set(Qt5Widgets_DIR "/home/vuk/Qt/5.15.0/wasm_32/lib/cmake/Qt5Widgets")
##    set(Qt5Gui_DIR "/home/vuk/Qt/5.15.0/wasm_32/lib/cmake/Qt5Gui")
##    set(Qt5Core_DIR "/home/vuk/Qt/5.15.0/wasm_32/lib/cmake/Qt5Core")
##    set(Qt5Svg_DIR "/home/vuk/Qt/5.15.0/wasm_32/lib/cmake/Qt5Svg")
##    set(Qt5Zlib_DIR "/home/vuk/Qt/5.15.0/wasm_32/lib/cmake/Qt5Zlib")
##    set(Qt5Network_DIR "/home/vuk/Qt/5.15.0/wasm_32/lib/cmake/Qt5Network")
##    set(Qt5EventDispatcherSupport_DIR "/home/vuk/Qt/5.15.0/wasm_32/lib/cmake/Qt5EventDispatcherSupport")
##    set(Qt5FontDatabaseSupport_DIR "/home/vuk/Qt/5.15.0/wasm_32/lib/cmake/Qt5FontDatabaseSupport")
##    set(Qt5EglSupport_DIR "/home/vuk/Qt/5.15.0/wasm_32/lib/cmake/Qt5EglSupport")
##    set(Qt5ThemeSupport_DIR "/home/vuk/Qt/5.15.0/wasm_32/lib/cmake/Qt5ThemeSupport")
##    set(Qt5Quick_DIR "/home/vuk/Qt/5.15.0/wasm_32/lib/cmake/Qt5Quick")
##    set(Qt5QmlModels_DIR "/home/vuk/Qt/5.15.0/wasm_32/lib/cmake/Qt5QmlModels")
##    set(Qt5Qml_DIR "/home/vuk/Qt/5.15.0/wasm_32/lib/cmake/Qt5Qml")
##    set(Qt5WebSockets_DIR "/home/vuk/Qt/5.15.0/wasm_32/lib/cmake/Qt5WebSockets")
##    set(CMAKE_TOOLCHAIN_FILE="/home/vuk/Desktop/Install/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")
#endif()


#   # add the GUI WIN32 executable
#   cyo_add_executable(${PROJECT_NAME} WIN32 ${source_files} ${header_files} chatwidget.ui ${COLYOO_QT_RESOURCES})

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

#find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)

#if(ANDROID)
#    set(CMAKE_PREFIX_PATH "/home/vuk/Qt/5.15.0/gcc_64")
##    set(CMAKE_PREFIX_PATH "/home/vuk/qt6/6.1.0/gcc_64")
##    set(Qt5_DIR "${CMAKE_PREFIX_PATH}/lib/cmake/Qt5")
#    set(Qt5_DIR "${CMAKE_PREFIX_PATH}/lib/cmake/Qt6")
#    set(Qt5Widgets_DIR "${CMAKE_PREFIX_PATH}/lib/cmake/Qt5Widgets")
#    set(Qt5Network_DIR "${CMAKE_PREFIX_PATH}/lib/cmake/Qt5Network")
#    set(Qt5LinguistTools_DIR "${CMAKE_PREFIX_PATH}/lib/cmake/Qt5LinguistTools")
#endif()
#find_package(Qt5 COMPONENTS Widgets REQUIRED)
#find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Gui OpenGL)

#set(TS_FILES suspiria_en_US.ts)

#list (APPEND RESOURCES resource.qrc)
#qt5_add_resources (RCC_SOURCES ${RESOURCES})
#add_library(resource ${RCC_SOURCES} ${SOURCES})


## set any variables you might need to set for your app and assimp
#set(BUILD_ASSIMP_TOOLS  ON)
#set(ASSIMP_BUILD_STATIC_LIB ON)

## add assimp source dir as a subdirectory, effectively making
## assimp's CMakeLists.txt part of your build
#add_subdirectory(assimp)
#include_directories(assimp/include/)

#add_subdirectory(bullet)
#find_package(Bullet REQUIRED)
#include_directories(${BULLET_INCLUDE_DIR})


#set(BULLET_ROOT bullet) # where to find Bullet
#find_package(BULLET REQUIRED) # find Bullet and fill environment variables


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
#set(BUILD_SHARED_LIBS ON)
#set(BUILD_ASSIMP_TOOLS OFF)
#set(ASSIMP_BUILD_ASSIMP_VIEW OFF)
#set(ASSIMP_BUILD_STATIC_LIB OFF)
#set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT FALSE)
#set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT FALSE)
#set(ASSIMP_BUILD_FBX_IMPORTER OFF)
#set(ASSIMP_BUILD_TESTS OFF)
#set(ASSIMP_BUILD_OBJ_IMPORTER ON)

OPTION ( ASSIMP_BUILD_TESTS
  "If the test suite for Assimp is built in addition to the library."
 OFF
)
OPTION( ASSIMP_BUILD_ASSIMP_TOOLS
  "If the supplementary tools for Assimp are built in addition to the library."
 OFF
)
OPTION( ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF)
#OPTION( ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)


add_subdirectory(dep/assimp)


add_subdirectory(dep/entt)

#add_library(EnTT INTERFACE)
#target_sources(EnTT INTERFACE EnTT.h)


#target_include_directories(EnTT INTERFACE
#    "${PROJECT_SOURCE_DIR}/dep/entt"
#)
#INCLUDE_DIRECTORIES( dep/entt/ )
#target_include_directories(EnTT INTERFACE
#    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/dep/entt>
#    $<INSTALL_INTERFACE:${YOUR_INSTALL_DIR}/build/entt>
#)


#add_subdirectory(entt)
#Then my top level CMakeLists.txt file looks something like this:
#CODE: SELECT ALL

#CMAKE_MINIMUM_REQUIRED(VERSION 2.4)
#PROJECT(HelloWorld)
#INCLUDE_DIRECTORIES(bullet/src) # Add bullet to the user include dir

#SUBDIRS(bullet/) # This line will read in the CMakeLists.txt for Bullet

#FILE(GLOB ALL_SRCGLOB src/*.cpp) # all .cpp files in src/

#ADD_EXECUTABLE(HelloWorld ALL_SRCGLOB) # Add all those source files to HelloWorld
#TARGET_LINK_LIBRARIES(HelloWorld twbullet)

#include(FindPkgConfig)
#pkg_check_modules(Bullet REQUIRED bullet)
#if(Bullet_FOUND)
#        message("bullet is found:")
#        message("  Bullet_INCLUDE_DIRS= ${Bullet_INCLUDE_DIRS}")
#        message("  Bullet_LIBRARIES= ${Bullet_LIBRARIES}")
#        message("  Bullet_LIBRARY_DIRS= ${Bullet_LIBRARY_DIRS}")
#        link_directories(${Bullet_LIBRARY_DIRS})
#        include_directories(${Bullet_INCLUDE_DIRS})
#else()
#        message("Bullet is NOT found")
#endif()

#message("!!!!!!!!PORUKA!!!!!!!!!!")

#else()
#        message("!!!!!!!!EST ELSE!!!!!!!")
#endif()

#if(ANDROID)
#  message("ANDROID build")
#  add_library(suspiria SHARED
#    main.cpp
#    ${SUSPIRIA_SRCGLOB}
#    ${BULLET_SRCGLOB}
##    EnTT
#    assimp
#    resource.qrc
#    shaders.qrc
##    glwindow.h
##    glwindow.cpp
##    logo.h
##    logo.cpp
##    ${TS_FILES}
#  )
#else

if(EMSCRIPTEN)
  message("EMSCRIPTEN build")
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

if(ANDROID)
    target_link_libraries(suspiria PRIVATE GLESv2)
endif()

if(EMSCRIPTEN)
#  link_qt_static(suspiria)
#    set_target_properties(suspiria PROPERTIES LINK_FLAGS "-s TOTAL_MEMORY=4GB")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s INITIAL_MEMORY=134217728")
endif()

#target_link_libraries(suspiria PRIVATE Qt5::Widgets EnTT twbullet assimp)# LinearMath BulletCollision BulletDynamics )
#target_link_libraries(suspiria PRIVATE Qt6::Widgets EnTT twbullet assimp)# LinearMath BulletCollision BulletDynamics )
target_link_libraries(suspiria PUBLIC
    Qt::Core
    Qt::Gui
    Qt::OpenGL
    EnTT
    twbullet
    assimp
)

#qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
