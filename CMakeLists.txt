cmake_minimum_required(VERSION 3.24)
project(engine)

set(CMAKE_CXX_STANDARD 20)

add_executable(engine main.cpp Particle.cpp Particle.h Random.h Interaction.cpp Interaction.h System.cpp System.h Writer.cpp Writer.h)
