cmake_minimum_required (VERSION 2.6)

project(pbs)
set(CMAKE_CXX_STANDARD 20)
set(PYBIND11_PYTHON_VERSION 3.12)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -fPIC")

find_package(pybind11 REQUIRED)
find_package(Eigen3 REQUIRED)

include_directories("inc")
include_directories("src")
file(GLOB SOURCES "src/*.cpp")

ADD_LIBRARY(LibsModule
    inc/common.h
    inc/Conflict.h
    inc/ConstraintTable.h
    inc/Instance.h
    inc/PBS.h
    inc/PBSNode.h
    inc/ReservationTable.h
    inc/SingleAgentSolver.h
    inc/SIPP.h
    inc/SpaceTimeAStar.h

    src/common.cpp
    src/Conflict.cpp
    src/ConstraintTable.cpp
    src/driver.cpp
    src/Instance.cpp
    src/PBS.cpp
    src/PBSNode.cpp
    src/ReservationTable.cpp
    src/SingleAgentSolver.cpp
    src/SIPP.cpp
    src/SpaceTimeAStar.cpp
)

pybind11_add_module(${PROJECT_NAME} src/pywrap.cpp)
target_link_libraries(${PROJECT_NAME} LibsModule)

target_compile_definitions(${PROJECT_NAME} PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})
target_include_directories(${PROJECT_NAME} PRIVATE ${PYBIND11_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE Eigen3::Eigen)

# Find Boost
find_package(Boost REQUIRED COMPONENTS program_options system filesystem)
include_directories( ${Boost_INCLUDE_DIRS} )
target_link_libraries(pbs ${Boost_LIBRARIES})
