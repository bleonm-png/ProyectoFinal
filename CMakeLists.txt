cmake_minimum_required(VERSION 4.1)
project(ProyectoFinal)

set(CMAKE_CXX_STANDARD 20)

add_executable(ProyectoFinal main.cpp
        ventas.cpp
        ventas.h
        auxiliar.cpp
        auxiliar.h
        productos.h
        productos.cpp
        reportes.cpp
        reportes.h
)

