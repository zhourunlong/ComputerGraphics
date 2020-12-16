CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(RT CXX)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF()

ADD_SUBDIRECTORY(deps/vecmath)

SET(RT_SOURCES
        src/main.cpp
        )

SET(RT_INCLUDES
        include/parser.hpp
        include/camera.hpp
        include/group.hpp
        include/hit.hpp
        include/image.hpp
        include/light.hpp
        include/material.hpp
        include/mesh.hpp
        include/object3d.hpp
        include/plane.hpp
        include/ray.hpp
        include/sphere.hpp
        include/transform.hpp
        include/triangle.hpp
        include/pugiconfig.hpp
        include/pugixml.hpp
        )

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS "-O2")

ADD_EXECUTABLE(${PROJECT_NAME} ${RT_SOURCES} ${RT_INCLUDES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} vecmath)
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PRIVATE include)
