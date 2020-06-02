cmake_minimum_required(VERSION 3.10)

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.14")
    cmake_policy(SET CMP0086 NEW)
endif()

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.12")
    cmake_policy(SET CMP0074 NEW)
endif()

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.13")
    cmake_policy(SET CMP0078 NEW)
endif()


# install paths
if(SKBUILD)
    set(SIQADCONN_SWIG_MODULE_DEST ${CMAKE_CURRENT_BINARY_DIR}/../cmake-install/siqadtools)
    set(SIQADCONN_PY_DEST ${CMAKE_CURRENT_BINARY_DIR}/../cmake-install/siqadtools)
elseif(DEFINED SIQADCONN_ROOT)
    set(SIQADCONN_SWIG_MODULE_DEST ${SIQADCONN_ROOT})
    set(SIQADCONN_PY_DEST ${SIQADCONN_ROOT})
else()
    set(SIQADCONN_SWIG_MODULE_DEST ${CMAKE_CURRENT_BINARY_DIR}/siqadtools)
    set(SIQADCONN_PY_DEST ${CMAKE_CURRENT_BINARY_DIR}/siqadtools)
endif()


#set_property(TARGET ${SWIG_MODULE_siqadconn_REAL_NAME} PROPERTY CXX_STANDARD_REQUIRED 11)
add_definitions(-Wall -g -std=c++11)

find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(
    .
    src
    swig
)
set(CMAKE_SWIG_FLAGS "")

set_property(SOURCE swig/siqadconn.i PROPERTY CPLUSPLUS ON)
file(COPY swig/__init__.py DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/swig")
#set_source_files_properties(swig/siqadconn.i PROPERTIES SWIG_FLAGS "-includeall")


# build the SWIG src and C++ code
swig_add_library(siqadconn LANGUAGE python SOURCES swig/siqadconn.i src/siqadconn.cc)

# link PYTHON for Windows, don't link for Linux
if (WIN32)
    swig_link_libraries(siqadconn ${PYTHON_LIBRARIES} ${Boost_LIBRARIES})
else()
    swig_link_libraries(siqadconn ${Boost_LIBRARIES})
endif()

install(TARGETS ${SWIG_MODULE_siqadconn_REAL_NAME} 
        DESTINATION ${SIQADCONN_SWIG_MODULE_DEST})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/siqadconn.py 
              ${CMAKE_CURRENT_SOURCE_DIR}/swig/__init__.py
              DESTINATION ${SIQADCONN_PY_DEST})
