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
        include/hit.h
        include/ray.hpp
        include/utils.hpp
        include/emitter.hpp
        include/texture/texture.h
        include/texture/bump.hpp
        include/bsdf/bsdf.h
        include/image/image.hpp
        include/image/ClmbsImg.hpp
        include/object/group.hpp
        include/object/mesh.hpp
        include/object/object3d.hpp
        include/object/sphere.hpp
        include/object/transform.hpp
        include/object/triangle.hpp
        include/object/curve.hpp
        include/object/revsurface.hpp
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
SET(CMAKE_CXX_FLAGS "-O2 -fopenmp")

ADD_EXECUTABLE(${PROJECT_NAME} ${RT_SOURCES} ${RT_INCLUDES})
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} png16 jpeg)