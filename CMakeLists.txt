cmake_minimum_required(VERSION 3.16)
project(grpcstresstest)
project(example-grpc LANGUAGES CXX)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 17)
find_package(Threads REQUIRED)
SET(ABSL_PROPAGATE_CXX_STD ON)
add_subdirectory(client)
add_subdirectory(server)

INCLUDE(FindPkgConfig)

pkg_check_modules(SYSTEMD REQUIRED IMPORTED_TARGET libsystemd)
set(CMAKE_PREFIX_PATH "cmake-build-debug/_deps/grpc-subbuild")
include(FetchContent)
FetchContent_Declare(
        yandexapi
        GIT_REPOSITORY https://github.com/yandex-cloud/cloudapi.git
        GIT_TAG master
)

FetchContent_MakeAvailable(yandexapi)
set(BUILD_SHARED_LIBS OFF) # up to you
set(BUILD_TESTING OFF) # to disable abseil test
set(ABSL_ENABLE_INSTALL ON) # now you can enable install rules even in subproject...
set(ABSL_ENABLE_INSTALL ON)
FetchContent_Declare(
        absl
        GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
        GIT_TAG        origin/master
        OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(absl)


find_package(absl CONFIG REQUIRED)
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


set(protobuf_MODULE_COMPATIBLE ON CACHE BOOL "")

FetchContent_Declare(
        protobuf
        GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
        GIT_TAG v24.3
        SOURCE_SUBDIR cmake
        FIND_PACKAGE_ARGS NAMES protobuf
)

FetchContent_MakeAvailable(protobuf)

set(ABSL_ENABLE_INSTALL ON)
set(FETCHCONTENT_QUIET OFF)
FetchContent_Declare(
        gRPC
        GIT_REPOSITORY https://github.com/grpc/grpc
        GIT_TAG v1.58.1
)

FetchContent_MakeAvailable(gRPC)
find_package(PkgConfig REQUIRED)
pkg_search_module(GRPC REQUIRED grpc)
pkg_search_module(GRPCPP REQUIRED grpc++>=1.22.0)
set(Protobuf_IMPORT_DIRS ${yandexapi_SOURCE_DIR})
find_package(PkgConfig REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIR})
find_package(protobuf CONFIG REQUIRED)
find_package(gRPC REQUIRED)
find_package(c-aresAlt)







include(CMakePrintHelpers)
cmake_print_variables(CMAKE_SOURCE_DIR)
cmake_print_variables(yandexapi_SOURCE_DIR)
cmake_print_variables(PROTO_FILES)
add_library(myproto OBJECT ${PROTO_FILES})
target_link_libraries(myproto
        PUBLIC
        protobuf::libprotobuf
        gRPC::grpc
        gRPC::grpc++
        absl::flags
        absl::base
        absl::synchronization
        absl::strings
)

set(PROTO_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(PROTO_IMPORT_DIRS "${yandexapi_SOURCE_DIR}")
set(Protobuf_INCLUDE_DIRS "cmake-build-debug/_deps/grpc-src/include")
set(PROTO_IMPORT_DIRS3 "cmake-build-debug/_deps/grpc-src/third_party/googleapis")
set(PROTO_IMPORT_DIRS2 "cmake-build-debug/_deps/yandexapi-src/google")
set(PROTO_IMPORT_DIRS2 "cmake-build-debug/_deps/yandexapi-src/yandex")

cmake_print_variables(PROTO_BINARY_DIR)
cmake_print_variables(PROTO_IMPORT_DIRS)
cmake_print_variables(CMAKE_CURRENT_BINARY_DIR)


get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
cmake_print_variables(grpc_cpp_plugin_location)
target_include_directories(myproto PUBLIC "$<BUILD_INTERFACE:${PROTO_BINARY_DIR}>")

protobuf_generate(
        TARGET myproto
        LANGUAGE cpp
        IMPORT_DIRS
        ${PROTO_IMPORT_DIRS} ${PROTO_IMPORT_DIRS2} ${PROTO_IMPORT_DIRS3} ${PROTO_IMPORT_DIRS4} ${PROTO_IMPORT_DIRS5}
        PROTOC_OUT_DIR "${PROTO_BINARY_DIR}"
        PROTOC_OPTIONS
)
protobuf_generate(
        TARGET
        myproto
        LANGUAGE
        grpc
        IMPORT_DIRS
        ${PROTO_IMPORT_DIRS} ${PROTO_IMPORT_DIRS2} ${PROTO_IMPORT_DIRS3} ${PROTO_IMPORT_DIRS4} ${PROTO_IMPORT_DIRS5}
        PROTOC_OUT_DIR "${PROTO_BINARY_DIR}"
        GENERATE_EXTENSIONS
        .grpc.pb.h
        .grpc.pb.cc
        PLUGIN
        "protoc-gen-grpc=${grpc_cpp_plugin_location}"
)
