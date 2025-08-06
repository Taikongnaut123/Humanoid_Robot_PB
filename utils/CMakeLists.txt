cmake_minimum_required(VERSION 3.8)

set(TARGET_NAME libPBUtils)

project(${TARGET_NAME})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 输出目录设置 - 使用父项目的 OUTPUT_BIN_DIR
if(NOT DEFINED OUTPUT_BIN_DIR)
    message(FATAL_ERROR "OUTPUT_BIN_DIR must be defined by parent project")
endif()

find_package(PkgConfig REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${Protobuf_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-linux/include
    ${PB_UTILS_INCLUDE_DIR}
    )


# # 设置库搜索路径
# link_directories(
#     ${OUTPUT_LIB_DIR}
#     ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-linux/lib
#     ${CMAKE_CURRENT_SOURCE_DIR}/../include
    
# )

add_library(${TARGET_NAME} SHARED
    source/printUtil.cpp
)

target_include_directories(${TARGET_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${PB_INCLUDE_DIR}  # PB 生成的头文件
)

# 链接依赖库
target_link_libraries(${TARGET_NAME}
    PUBLIC
    protobuf::libprotobuf
    gRPC::grpc++
)

