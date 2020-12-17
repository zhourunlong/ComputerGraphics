CMAKE_MINIMUM_REQUIRED(VERSION 3.2)
PROJECT(vecmath CXX)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF()

SET(VECMATH_INCLUDES
        include/Matrix2d.h
        include/Matrix3d.h
        include/Matrix4d.h
        include/Quat4d.h
        include/vecmath.h
        include/Vector2d.h
        include/Vector3d.h
        include/Vector4d.h)

SET(VECMATH_SOURCES
        src/Matrix2d.cpp
        src/Matrix3d.cpp
        src/Matrix4d.cpp
        src/Quat4d.cpp
        src/Vector2d.cpp
        src/Vector3d.cpp
        src/Vector4d.cpp)

SET(CMAKE_CXX_STANDARD 11)

ADD_LIBRARY(${PROJECT_NAME} STATIC ${VECMATH_INCLUDES} ${VECMATH_SOURCES})
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC include)
