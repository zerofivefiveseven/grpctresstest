cmake_minimum_required(VERSION 3.4)
project(grpcstresstest)
#cmake_policy(CMP0079 1)
include(ExternalProject)
include(FetchContent)
FetchContent_Declare(D1CMakeModules
        GIT_REPOSITORY https://git.protei.ru/yaskevich-p/d1cmakemodules.git
)
FetchContent_MakeAvailable(D1CMakeModules)

protei_init(CXX_STANDARD c++20)

project(example-grpc LANGUAGES CXX)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 17)
find_package(Threads REQUIRED)
SET(Abseil_PROPAGATE_CXX_STD ON)
add_subdirectory(client)
add_subdirectory(server)

#find_package(Protobuf REQUIRED)
#find_package(grpc CONFIG REQUIRED)

include(FetchContent)

INCLUDE(FindPkgConfig)
find_package(PkgConfig REQUIRED)
pkg_check_modules(SYSTEMD REQUIRED IMPORTED_TARGET systemd)
#set(CMAKE_PREFIX_PATH "cmake-build-debug/_deps/grpc-subbuild")

set(FETCHCONTENT_QUIET OFF)


#FetchContent_Declare(
#        Protobuf
#        GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
#        GIT_TAG v27.0
#        #        SOURCE_SUBDIR cmake
#        FIND_PACKAGE_ARGS NAMES Protobuf
#)

#FetchContent_MakeAvailable(Protobuf)
#find_package(Protobuf CONFIG REQUIRED)
#set(BUILD_SHARED_LIBS OFF) # up to you
#set(BUILD_TESTING OFF) # to disable abseil test
#set(Abseil_ENABLE_INSTALL ON) # now you can enable install rules even in subproject...
#FetchContent_Declare(
#        Abseil
#        GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
#        GIT_TAG 20240116.2
#        )
#FetchContent_MakeAvailable(Abseil)
FetchContent_Declare(
        yandexapi
        GIT_REPOSITORY https://github.com/yandex-cloud/cloudapi.git
        GIT_TAG master
)

FetchContent_MakeAvailable(yandexapi)

#find_package(Abseil REQUIRED)
set(YAPIMOVE "${yandexapi_SOURCE_DIR}/google")
file(COPY ${yandexapi_SOURCE_DIR}/third_party/googleapis/google DESTINATION ${yandexapi_SOURCE_DIR})
file(GLOB_RECURSE PROTO_FILES1 RELATIVE ${CMAKE_SOURCE_DIR} ${yandexapi_SOURCE_DIR}/google/*.proto)
file(GLOB_RECURSE PROTO_FILES2 RELATIVE ${CMAKE_SOURCE_DIR} ${yandexapi_SOURCE_DIR}/yandex/cloud/ai/*.proto)
file(GLOB_RECURSE PROTO_FILES3 RELATIVE ${CMAKE_SOURCE_DIR} ${yandexapi_SOURCE_DIR}/yandex/cloud/api/*.proto)
file(GLOB_RECURSE PROTO_FILES4 RELATIVE ${CMAKE_SOURCE_DIR} ${yandexapi_SOURCE_DIR}/yandex/cloud/operation/*.proto)
file(GLOB_RECURSE PROTO_FILES5 RELATIVE ${CMAKE_SOURCE_DIR} ${yandexapi_SOURCE_DIR}/yandex/cloud/validation.proto)
set(PROTO_FILES  ${PROTO_FILES1} ${PROTO_FILES2} ${PROTO_FILES3} ${PROTO_FILES4} ${PROTO_FILES5})
include_directories(${PROTOBUF_INCLUDE_DIR})
find_package(Threads)


set(Protobuf_MODULE_COMPATIBLE ON CACHE BOOL "")

set(Protobuf_IMPORT_DIRS ${yandexapi_SOURCE_DIR})
find_package(PkgConfig REQUIRED)
#find_package(protobuf CONFIG REQUIRED)
#include_directories(${PROTOBUF_INCLUDE_DIR})


FetchContent_Declare(
        gRPC
        GIT_REPOSITORY https://github.com/grpc/grpc
        GIT_TAG        v1.64.0  # e.g v1.28.0
)
set(FETCHCONTENT_QUIET OFF)
FetchContent_MakeAvailable(gRPC)
#pkg_search_module(grpc REQUIRED grpc)
#pkg_search_module(grpc REQUIRED grpc++>=1.22.0)
find_package(gRPC REQUIRED)
#find_package(c-aresAlt)


include(CMakePrintHelpers)
cmake_print_variables(CMAKE_SOURCE_DIR)
cmake_print_variables(yandexapi_SOURCE_DIR)
cmake_print_variables(PROTO_FILES)
add_library(myproto OBJECT ${PROTO_FILES})
target_link_libraries(myproto
        PUBLIC
#        Protobuf::libprotobuf
        gRPC::grpc
        gRPC::grpc++
#        Abseil::flags
#        Abseil::base
#        Abseil::synchronization
#        Abseil::strings
)

set(PROTO_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(PROTO_IMPORT_DIRS "${yandexapi_SOURCE_DIR}")
#set(Protobuf_INCLUDE_DIRS "cmake-build-debug/_deps/grpc-src/include")
set(PROTO_IMPORT_DIRS3 "cmake-build-debug/_deps/grpc-src/third_party/googleapis")
set(PROTO_IMPORT_DIRS2 "cmake-build-debug/_deps/yandexapi-src/google")
set(PROTO_IMPORT_DIRS2 "cmake-build-debug/_deps/yandexapi-src/yandex")

cmake_print_variables(PROTO_BINARY_DIR)
cmake_print_variables(PROTO_IMPORT_DIRS)
cmake_print_variables(CMAKE_CURRENT_BINARY_DIR)
cmake_print_variables(gRPC_BINARY_DIR)

get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
cmake_print_variables(grpc_cpp_plugin_location)

target_include_directories(myproto PUBLIC "$<BUILD_INTERFACE:${PROTO_BINARY_DIR}>")
set(Protobuf_MODULE_COMPATIBLE on)
#protobuf_generate(
#        TARGET myproto
#        LANGUAGE cpp
#        IMPORT_DIRS
#        ${PROTO_IMPORT_DIRS} ${PROTO_IMPORT_DIRS2} ${PROTO_IMPORT_DIRS3} ${PROTO_IMPORT_DIRS4} ${PROTO_IMPORT_DIRS5}
#        PROTOC_OUT_DIR "${PROTO_BINARY_DIR}"
#        PROTOC_OPTIONS
#)
set(Protobuf_MODULE_COMPATIBLE ON CACHE BOOL "")
protobuf_generate(
        TARGET myproto
        LANGUAGE grpc
        IMPORT_DIRS
        ${PROTO_IMPORT_DIRS} ${PROTO_IMPORT_DIRS2} ${PROTO_IMPORT_DIRS3} ${PROTO_IMPORT_DIRS4} ${PROTO_IMPORT_DIRS5}
        PROTOC_OUT_DIR "${PROTO_BINARY_DIR}"
        GENERATE_EXTENSIONS
        .grpc.pb.h
        .grpc.pb.cc
        PLUGIN
        "protoc-gen-grpc=${grpc_cpp_plugin_location}"
)
