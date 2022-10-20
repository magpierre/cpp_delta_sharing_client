include( ExternalProject )
include( FetchContent )
include(GNUInstallDirs)

cmake_minimum_required(VERSION 3.11)
set(CMAKE_BUILD_TYPE "Release")
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT DEFINED TARGET)
  set(TARGET DeltaSharingClient)
endif()


# Get Google TEST
FetchContent_Declare(googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG release-1.12.1
)
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()




# Get restclient-cpp from github
ExternalProject_Add(restclient
  GIT_REPOSITORY https://github.com/mrtazz/restclient-cpp
 GIT_TAG 0.5.2
 SOURCE_DIR restclient
 BUILD_IN_SOURCE 1
 UPDATE_COMMAND ""
 PATCH_COMMAND ""
 CONFIGURE_COMMAND ./autogen.sh && ./configure
 BUILD_COMMAND make
 INSTALL_COMMAND sudo make install
)

if(NOT DEFINED PROJECT_VERSION)
    set(PROJECT_VERSION 0)
endif()
# We require a C++11 compliant compiler

# set the project name
project(${TARGET} VERSION 0.0.1 DESCRIPTION "DeltaSharingClient for C++")

set(CMAKE_CXX_STANDARD_REQUIRED ON)

execute_process(
  COMMAND brew --prefix apache-arrow 
  RESULT_VARIABLE BREW_ARROW_RESULT
  OUTPUT_VARIABLE BREW_ARROW_PATH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# add cmake information from arrow to CMAKE_MODULE_PATH
list(APPEND CMAKE_MODULE_PATH "${BREW_ARROW_PATH}/lib/cmake/arrow/")

# for apache arrow
set(OPENSSL_ROOT_DIR /opt/homebrew/opt/openssl@1.1/)

# add the executable
add_library(${TARGET} SHARED ../DeltaSharingRestClient.cpp ../DeltaSharingClient.cpp ../functions.cpp)

find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(Boost 1.79.0 REQUIRED COMPONENTS Program_options)
find_package(Arrow REQUIRED)
find_package(Parquet REQUIRED)
#find_package(ArrowDataset REQUIRED)

#include all necessary external includes
target_include_directories(${TARGET} PRIVATE ${CMAKE_INSTALL_FULL_INCLUDEDIR})

target_link_libraries(
  ${TARGET} 
  PRIVATE  Boost::program_options 
  PRIVATE nlohmann_json::nlohmann_json 
  PRIVATE ${CMAKE_INSTALL_FULL_LIBDIR}/librestclient-cpp.dylib 
  PRIVATE ${ARROW_SHARED_LIB} 
 # ${ARROW_DATASET_SHARED_LIB} 
  PRIVATE ${PARQUET_SHARED_LIB}
)

target_compile_features(${TARGET} PRIVATE cxx_std_17)
set_target_properties(${TARGET} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${TARGET} PROPERTIES SOVERSION 0)
set_target_properties(${TARGET} PROPERTIES PUBLIC_HEADER "../include/DeltaSharingClient.h;../include/DeltaSharingRestClient.h;../include/Protocol.h;../include/functions.h")

install(
    TARGETS ${TARGET} 
    EXPORT DeltaSharingConfig
    CONFIGURATIONS Release
    LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/DeltaSharing
)

# Export CMake config 
install(EXPORT DeltaSharingConfig DESTINATION ${CMAKE_INSTALL_FULL_DATAROOTDIR}/DeltaSharing/cmake)
export(TARGETS ${TARGET} FILE DeltaSharingConfig.cmake)


# Add cmake testing framework
enable_testing()

# Add test executable
add_executable(
  TestRunner
  ../tests/functions_test.cpp
)

# Link libraries used in library
target_link_libraries(
  TestRunner
  PRIVATE GTest::gtest_main
  PRIVATE nlohmann_json::nlohmann_json 
  Boost::program_options 
  PRIVATE /usr/local/lib/librestclient-cpp.dylib 
  PRIVATE ${ARROW_SHARED_LIB} 
  #${ARROW_DATASET_SHARED_LIB} 
  PRIVATE ${PARQUET_SHARED_LIB}
  ${TARGET}
)

#Add Google Test
include(GoogleTest)
gtest_discover_tests(TestRunner)

message("Parquet: ${PARQUET_HOME} ${PARQUET_INCLUDE_DIR}")