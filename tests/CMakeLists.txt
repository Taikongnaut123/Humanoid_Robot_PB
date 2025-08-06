# PB 测试工程 CMakeLists.txt
cmake_minimum_required(VERSION 3.8)

project(PBTests LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 输出目录设置 - 使用父项目的 OUTPUT_BIN_DIR
if(NOT DEFINED OUTPUT_BIN_DIR)
    message(FATAL_ERROR "OUTPUT_BIN_DIR must be defined by parent project")
endif()

# 设置测试可执行文件的输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_BIN_DIR})

message(STATUS "PB Tests output directory: ${OUTPUT_BIN_DIR}")

# 查找依赖
find_package(PkgConfig REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

message(STATUS "PB utils include directory: ${PB_UTILS_INCLUDE_DIR}")

# 设置包含目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${Protobuf_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-linux/include
    ${PB_UTILS_INCLUDE_DIR}
)

# 设置库搜索路径
link_directories(
    ${OUTPUT_LIB_DIR}
    ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-linux/lib
)

# 收集所有测试源文件
file(GLOB TEST_SOURCES "*.cpp")

# 为每个测试文件创建可执行文件
foreach(TEST_SOURCE ${TEST_SOURCES})
    # 获取文件名（不含扩展名）
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    
    # 创建测试可执行文件
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    
    # 设置编译属性
    set_target_properties(${TEST_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_BIN_DIR}
    )
    
    # 根据测试名称链接对应的库
    if(TEST_NAME MATCHES ".*common.*")
        target_link_libraries(${TEST_NAME}
            libcommonPB
            libPBUtils
            protobuf::libprotobuf
            gRPC::grpc++
        )
        add_dependencies(${TEST_NAME} libcommonPB libPBUtils)
        message(STATUS "Created common test: ${TEST_NAME}")
        
    elseif(TEST_NAME MATCHES ".*interfaces.*")
        target_link_libraries(${TEST_NAME}
            libinterfacesPB
            libcommonPB
            libPBUtils
            protobuf::libprotobuf
            gRPC::grpc++
        )
        add_dependencies(${TEST_NAME} libinterfacesPB libcommonPB libPBUtils)
        message(STATUS "Created interfaces test: ${TEST_NAME}")
        
    else()
        # 通用测试，链接所有库
        target_link_libraries(${TEST_NAME}
            libdetectionPB
            libperceptionPB
            libcommonPB
            libinterfacesPB
            libPBUtils
            protobuf::libprotobuf
            gRPC::grpc++
        )
        add_dependencies(${TEST_NAME} libdetectionPB libperceptionPB libcommonPB libinterfacesPB libPBUtils)
        message(STATUS "Created general test: ${TEST_NAME}")
    endif()
    
    # 添加到测试列表
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()

# 创建一个总的测试目标
add_custom_target(run_all_tests
    COMMENT "Running all PB tests"
    DEPENDS ${TEST_TARGETS}
)

# 显示测试信息
message(STATUS "PB Tests configuration completed")
message(STATUS "Found test sources: ${TEST_SOURCES}")
message(STATUS "Tests will be built to: ${OUTPUT_BIN_DIR}")
