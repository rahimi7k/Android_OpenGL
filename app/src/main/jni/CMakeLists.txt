# Minimum required version of CMake
cmake_minimum_required(VERSION 3.22.1)

# Project name
project("game")


# Add Assimp library
add_subdirectory(assimp-5.4.2)  # Adjust the path to where you placed Assimp



set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "Disable all importers by default")
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "Enable OBJ importer")
set(ASSIMP_ANDROID_JNIIOSYSTEM ON CACHE BOOL "Enable Android JNI IO System")



include_directories(
        gl
        graphic
        math
        object
        #assimp-5.4.2/include
        assimp-5.4.2/port/AndroidJNI
)
#include_directories(./)

# Add your source files
add_library(game SHARED
        native-lib.cpp
        AssetManager.h
        AssetManager.cpp
        stb_image.h
        tiny_obj_loader.h
        object/Object3D.h
        Log.h
        Test.h
        Test.cpp


        gl/Buffer.cpp
        gl/Shader.cpp

        graphic/Sprite.cpp
        graphic/Sprite3D.cpp
        graphic/Texture.cpp


        math/Vector2.cpp
        math/Vector3.cpp

        assimp-5.4.2/port/AndroidJNI/AndroidJNIIOSystem.cpp
        assimp-5.4.2/port/AndroidJNI/BundledAssetIOSystem.cpp
)



#find_package(assimp REQUIRED)
#include_directories(assimp-5.4.2/include/assimp)

# Find and link the OpenGL and EGL libraries
find_library(log-lib log)
find_library(android-lib android)
find_library(GLESv3-lib GLESv3)
find_library(EGL-lib EGL)

# Link your native library with the required libraries
target_link_libraries(game
        ${log-lib}
        ${android-lib}
        ${GLESv3-lib}
        ${EGL-lib}
        assimp)


# Ensure proper linking order if there are dependencies
#add_dependencies(game assimp)
