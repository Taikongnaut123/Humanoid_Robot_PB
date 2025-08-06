cmake_minimum_required(VERSION 3.8)
project(PB
  VERSION 1.0
  DESCRIPTION "Protocol Buffer components"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找protobuf和grpc依赖 (通过vcpkg)
find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# 设置输出目录
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_LIB_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_LIB_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_BIN_DIR})

# 自动发现source目录下的所有子模块
file(GLOB SUBMODULES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/source" "${CMAKE_CURRENT_SOURCE_DIR}/source/*")

# 存储实际找到的子模块列表（用于配置文件生成）
set(FOUND_SUBMODULES)
set(LIBRARY_TARGETS_CONFIG)

foreach(SUBMODULE ${SUBMODULES})
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/source/${SUBMODULE}")
        message(STATUS "Found PB submodule: ${SUBMODULE}")
        
        # 设置子模块的源文件路径
        set(SUBMODULE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source/${SUBMODULE}")
        set(SUBMODULE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/${SUBMODULE}")
        
        # 收集子模块的源文件
        file(GLOB_RECURSE SUBMODULE_SOURCES 
            "${SUBMODULE_SOURCE_DIR}/*.cc"
            "${SUBMODULE_SOURCE_DIR}/*.cpp"
        )
        
        # 收集子模块的头文件
        file(GLOB_RECURSE SUBMODULE_HEADERS 
            "${SUBMODULE_INCLUDE_DIR}/*.h"
            "${SUBMODULE_INCLUDE_DIR}/*.hpp"
        )
        
        if(SUBMODULE_SOURCES)
            # 创建动态库目标，库名为lib+子模块名+PB
            set(LIBRARY_NAME "lib${SUBMODULE}PB")
            add_library(${LIBRARY_NAME} SHARED ${SUBMODULE_SOURCES})
            
            # 创建带命名空间的别名，方便其他项目使用
            add_library(PB::${SUBMODULE}PB ALIAS ${LIBRARY_NAME})
            
            # 添加到找到的子模块列表
            list(APPEND FOUND_SUBMODULES ${SUBMODULE})
            
            # 为配置文件添加库目标信息
            string(APPEND LIBRARY_TARGETS_CONFIG "# Target for ${SUBMODULE} submodule\n")
            string(APPEND LIBRARY_TARGETS_CONFIG "set(PB_${SUBMODULE}_FOUND TRUE)\n")
            string(APPEND LIBRARY_TARGETS_CONFIG "set(PB_${SUBMODULE}_LIBRARY PB::${SUBMODULE}PB)\n\n")
            
            # 设置目标属性
            set_target_properties(${LIBRARY_NAME} PROPERTIES
                OUTPUT_NAME "${SUBMODULE}PB"  # 输出文件名包含PB后缀
                VERSION ${PROJECT_VERSION}
                SOVERSION ${PROJECT_VERSION_MAJOR}
                POSITION_INDEPENDENT_CODE ON
                EXPORT_NAME ${SUBMODULE}PB  # 导出时使用子模块名+PB
            )
            
            # 添加包含目录
            target_include_directories(${LIBRARY_NAME} 
                PUBLIC 
                    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                    $<BUILD_INTERFACE:${SUBMODULE_INCLUDE_DIR}>
                    $<INSTALL_INTERFACE:include>
                PRIVATE
                    ${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${SUBMODULE_INCLUDE_DIR}
            )
            
            # 链接protobuf和grpc库 (vcpkg版本)
            target_link_libraries(${LIBRARY_NAME}
                PUBLIC
                    protobuf::libprotobuf
                    gRPC::grpc++
                    gRPC::grpc++_reflection
            )
            
            # 添加编译定义
            target_compile_definitions(${LIBRARY_NAME} 
                PRIVATE 
                    PB_EXPORTS
                    ${SUBMODULE}_EXPORTS  # 为每个子模块设置特定的导出宏
            )
            
            # 设置编译选项
            target_compile_options(${LIBRARY_NAME}
                PRIVATE
                    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra>
            )
            
            # 安装目标
            install(TARGETS ${LIBRARY_NAME}
                EXPORT PB-targets
                LIBRARY DESTINATION ${OUTPUT_LIB_DIR}
                ARCHIVE DESTINATION ${OUTPUT_LIB_DIR}
                RUNTIME DESTINATION ${OUTPUT_BIN_DIR}
            )
            
            # 安装头文件
            install(DIRECTORY ${SUBMODULE_INCLUDE_DIR}/
                DESTINATION ${OUTPUT_INCLUDE_DIR}/${SUBMODULE}
                FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
            )
            
            message(STATUS "Created library target: ${LIBRARY_NAME} for submodule: ${SUBMODULE}")
        else()
            message(WARNING "No source files found for submodule: ${SUBMODULE}")
        endif()
    endif()
endforeach()

# 安装通用头文件
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include/pb_export.h")
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/pb_export.h"
        DESTINATION ${OUTPUT_INCLUDE_DIR}
    )
endif()

# 创建PB配置文件
# 将子模块列表转换为字符串格式
string(REPLACE ";" " " SUBMODULES_STRING "${FOUND_SUBMODULES}")
set(SUBMODULES "${SUBMODULES_STRING}")

# 设置配置文件中需要的变量
set(LIBRARY_TARGETS "${LIBRARY_TARGETS_CONFIG}")

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/PBConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/PBConfig.cmake"
    @ONLY
)

# 输出构建信息
message(STATUS "PB module configuration completed")
message(STATUS "Library output directory: ${OUTPUT_LIB_DIR}")
message(STATUS "Include output directory: ${OUTPUT_INCLUDE_DIR}")

# 添加测试选项
option(BUILD_PB_TESTS "Build PB tests" ON)

# 如果启用测试，添加 tests 子目录
if(BUILD_PB_TESTS)
    message(STATUS "Adding PB tests subdirectory...")
    add_subdirectory(tests)
endif()

# 添加PB工具编译选项
option(BUILD_PB_UTILS "Build PB utils" ON)

# 如果启用工具，添加 utils 子目录
if(BUILD_PB_UTILS)
    message(STATUS "Adding PB utils subdirectory...")
    add_subdirectory(utils)
endif()
