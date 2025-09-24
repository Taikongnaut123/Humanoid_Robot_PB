# PB 测试工程 CMakeLists.txt
cmake_minimum_required(VERSION 3.8)

project(PBTests LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 输出目录设置 - 使用父项目的 OUTPUT_BIN_DIR
if(NOT DEFINED OUTPUT_BIN_DIR)
    message(DEBUG "OUTPUT_BIN_DIR must be defined by parent project")
endif()

# 设置测试可执行文件的输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_BIN_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_BIN_DIR})

# 查找依赖
find_package(PkgConfig REQUIRED)
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# 启用测试
enable_testing()

# 收集所有测试源文件
file(GLOB TEST_SOURCES "*.cpp")

# 创建测试目标列表
set(TEST_TARGETS)

# 为每个测试文件创建可执行文件
foreach(TEST_SOURCE ${TEST_SOURCES})
    # 获取文件名（不含扩展名）
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    
    set(TEST_NAME "CHRIC_PB${TEST_NAME}")

    # 创建测试可执行文件
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    
    # 添加到测试目标列表
    list(APPEND TEST_TARGETS ${TEST_NAME})
    
    # 设置编译属性
    set_target_properties(${TEST_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_BIN_DIR}
    )
    
    # 设置包含目录
    target_include_directories(${TEST_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
        ${PB_UTILS_INCLUDE_DIR}
    )
    
    # 根据测试名称链接对应的库
    if(TEST_NAME MATCHES ".*common.*")
        target_link_libraries(${TEST_NAME} PRIVATE
            libCHRIC_commonPB
            CHRIC_PBUtils
            protobuf::libprotobuf
            gRPC::grpc++
        )
        message(DEBUG "Created common test: ${TEST_NAME}")
        
    elseif(TEST_NAME MATCHES ".*interfaces.*")
        target_link_libraries(${TEST_NAME} PRIVATE
            libCHRIC_interfacesPB
            libCHRIC_commonPB
            CHRIC_PBUtils
            protobuf::libprotobuf
            gRPC::grpc++
        )
        message(DEBUG "Created interfaces test: ${TEST_NAME}")
        
    else()
        # 通用测试，链接所有库
        target_link_libraries(${TEST_NAME} PRIVATE
            libCHRIC_detectionPB
            libCHRIC_perceptionPB
            libCHRIC_commonPB
            libCHRIC_interfacesPB
            CHRIC_PBUtils
            protobuf::libprotobuf
            gRPC::grpc++
        )
        message(DEBUG "Created general test: ${TEST_NAME}")
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
message(DEBUG "=========================PB Tests configuration=========================")
message(DEBUG "PB Tests configuration completed")
message(DEBUG "Found test sources: ${TEST_SOURCES}")
message(DEBUG "Tests will be built to: ${OUTPUT_BIN_DIR}")
