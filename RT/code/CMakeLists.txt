CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(RT CXX)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF()

SET(RT_SOURCES
        src/main.cpp
        )

SET(RT_INCLUDES
        include/parser.hpp
        include/camera.hpp
        include/group.hpp
        include/hit.h
        include/hit.cpp
        include/image.hpp
        include/material.hpp
        include/mesh.hpp
        include/object3d.hpp
        include/ray.hpp
        include/sphere.hpp
        include/transform.hpp
        include/triangle.hpp
        include/pugi/pugiconfig.hpp
        include/pugi/pugixml.hpp
        include/vecmath/Matrix2d.h
        include/vecmath/Matrix3d.h
        include/vecmath/Matrix4d.h
        include/vecmath/Quat4d.h
        include/vecmath/vecmath.h
        include/vecmath/Vector2d.h
        include/vecmath/Vector3d.h
        include/vecmath/Vector4d.h
        )

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS "-O2 -fopenmp")

ADD_EXECUTABLE(${PROJECT_NAME} ${RT_SOURCES} ${RT_INCLUDES})
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PRIVATE include)
