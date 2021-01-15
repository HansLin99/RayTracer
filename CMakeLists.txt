cmake_minimum_required(VERSION 3.15)
project(RayTracer)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(external)

add_executable(RayTracer
        build/cmake_install.cmake
        build/CMakeCache.txt
        build/Makefile
        build/output.ppm
        build/output1.ppm
        build/output2.ppm
        build/output3.ppm
        build/output4.ppm
        build/Raytracer
        external/stb_image.h
        external/stb_image_write.h
        aabb.h
        camera.h
        CMakeLists.txt
        color.h
        hittable.h
        hittable_list.h
        main.cc
        material.h
        perlin.h
        ray.h
        rtw_stb_image.h
        rtweekend.h
        sphere.h
        texture.h
        vec3.h cube.h light.h triangle.h)
