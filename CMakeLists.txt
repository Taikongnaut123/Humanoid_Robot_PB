cmake_minimum_required(VERSION 3.8)
project(PB
  VERSION 1.0.0.0
  DESCRIPTION "Protocol Buffer components"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 执行 generate_proto.sh 脚本生成 Protobuf 代码
set(GENERATE_PROTO_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/generate_proto.sh")
set(PROTO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/proto")

if(EXISTS "${GENERATE_PROTO_SCRIPT}")
    message(STATUS "Checking Protobuf code generation...")
    
    # 查找所有 .proto 文件
    file(GLOB_RECURSE PROTO_FILES "${PROTO_DIR}/*.proto")
    
    if(PROTO_FILES)
        # 获取所有 proto 文件的最新修改时间
        set(LATEST_PROTO_TIME 0)
        foreach(PROTO_FILE ${PROTO_FILES})
            file(TIMESTAMP "${PROTO_FILE}" PROTO_TIME "%s")
            if(PROTO_TIME GREATER LATEST_PROTO_TIME)
                set(LATEST_PROTO_TIME ${PROTO_TIME})
            endif()
        endforeach()
        
        # 检查生成的代码是否存在且是最新的
        set(NEED_REGENERATE FALSE)
        file(GLOB_RECURSE GENERATED_FILES 
            "${CMAKE_CURRENT_SOURCE_DIR}/include/*.pb.h"
            "${CMAKE_CURRENT_SOURCE_DIR}/source/*.pb.cc"
        )
        
        if(NOT GENERATED_FILES)
            set(NEED_REGENERATE TRUE)
            message(STATUS "No generated Protobuf files found, will generate...")
        else()
            # 检查生成文件是否比 proto 文件旧
            foreach(GENERATED_FILE ${GENERATED_FILES})
                if(EXISTS "${GENERATED_FILE}")
                    file(TIMESTAMP "${GENERATED_FILE}" GEN_TIME "%s")
                    if(GEN_TIME LESS LATEST_PROTO_TIME)
                        set(NEED_REGENERATE TRUE)
                        message(STATUS "Generated files are outdated, will regenerate...")
                        break()
                    endif()
                endif()
            endforeach()
        endif()
        
        # 如果需要重新生成，执行脚本
        if(NEED_REGENERATE)
            message(STATUS "Generating Protobuf code from .proto files...")
            
            # 获取所有 proto 子目录
            file(GLOB PROTO_SUBDIRS RELATIVE "${PROTO_DIR}" "${PROTO_DIR}/*")
            
            foreach(SUBDIR ${PROTO_SUBDIRS})
                if(IS_DIRECTORY "${PROTO_DIR}/${SUBDIR}")
                    # 为每个子目录执行生成脚本
                    # 根据子目录名称确定导出符号
                    string(TOUPPER "${SUBDIR}" SUBDIR_UPPER)
                    set(EXPORT_SYMBOL "${SUBDIR_UPPER}_API")
                    
                    message(STATUS "  Generating for proto/${SUBDIR} with export symbol ${EXPORT_SYMBOL}...")
                    
                    execute_process(
                        COMMAND bash "${GENERATE_PROTO_SCRIPT}" "proto/${SUBDIR}" "${EXPORT_SYMBOL}"
                        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
                        RESULT_VARIABLE SCRIPT_RESULT
                        OUTPUT_VARIABLE SCRIPT_OUTPUT
                        ERROR_VARIABLE SCRIPT_ERROR
                    )
                    
                    if(NOT SCRIPT_RESULT EQUAL 0)
                        message(WARNING "Failed to generate Protobuf code for ${SUBDIR}:")
                        message(WARNING "  Output: ${SCRIPT_OUTPUT}")
                        message(WARNING "  Error: ${SCRIPT_ERROR}")
                    else()
                        message(STATUS "  ✓ Successfully generated Protobuf code for ${SUBDIR}")
                    endif()
                endif()
            endforeach()
        else()
            message(STATUS "Protobuf code is up to date, skipping generation.")
        endif()
    else()
        message(STATUS "No .proto files found in ${PROTO_DIR}")
    endif()
else()
    message(WARNING "generate_proto.sh script not found at ${GENERATE_PROTO_SCRIPT}")
endif()

# 查找protobuf和grpc依赖 (通过vcpkg)
find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# 设置输出目录

option(INSTALL_PB_HEADERS "Install PB header files" OFF)
option(INSTALL_PB_LIBS "Install PB library files" OFF)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_BIN_DIR}) # 动态库输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_BIN_DIR}) # 静态库输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_BIN_DIR}) # 可执行文件输出目录

# 自动发现source目录下的所有子模块
file(GLOB SUBMODULES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/source" "${CMAKE_CURRENT_SOURCE_DIR}/source/*")

# 存储实际找到的子模块列表（用于配置文件生成）
set(FOUND_SUBMODULES)
set(LIBRARY_TARGETS_CONFIG)

foreach(SUBMODULE ${SUBMODULES})
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/source/${SUBMODULE}")
        message(DEBUG "Found PB submodule: ${SUBMODULE}")
        
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
            set(LIBRARY_NAME "libCHRIC_${SUBMODULE}PB")
            add_library(${LIBRARY_NAME} SHARED ${SUBMODULE_SOURCES})
            
            # 创建带命名空间的别名，方便其他项目使用
            add_library(PB::CHRIC_${SUBMODULE}PB ALIAS ${LIBRARY_NAME})
            
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
                EXPORT_NAME "CHRIC_${SUBMODULE}PB"  # 导出时使用子模块名+PB
                RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_BIN_DIR}
            )
            
            # 添加包含目录
            target_include_directories(${LIBRARY_NAME} 
                PUBLIC 
                    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include># 源码树中头文件位于 ${CMAKE_CURRENT_SOURCE_DIR}/include    
                    $<BUILD_INTERFACE:${SUBMODULE_INCLUDE_DIR}># 源码树中子模块头文件位于 ${SUBMODULE_INCLUDE_DIR}
                PUBLIC
                    $<INSTALL_INTERFACE:include/${SUBMODULE}># 安装时头文件将被放到 ${CMAKE_INSTALL_PREFIX}/include/${SUBMODULE}
            )
            
            # 链接protobuf和grpc库 (vcpkg版本)
            target_link_libraries(${LIBRARY_NAME}
                PUBLIC
                    protobuf::libprotobuf
                    gRPC::grpc++
                    # gRPC::grpc++_reflection

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
 
            if(INSTALL_PB_LIBS)           
                # 安装库文件
                install(TARGETS ${LIBRARY_NAME}
                    EXPORT PB-targets
                    LIBRARY DESTINATION ${OUTPUT_LIB_DIR} # 共享库(.so)安装路径
                    ARCHIVE DESTINATION ${OUTPUT_LIB_DIR} # 静态库(.a)安装路径
                    RUNTIME DESTINATION ${OUTPUT_BIN_DIR} # 可执行文件安装路径
                )
            endif()

            # 安装库文件到端和云
            install(TARGETS ${LIBRARY_NAME}
                LIBRARY DESTINATION ${CHRIC_TERMINAL_FOLDER} # 共享库(.so)安装路径
            )

            # 安装库文件到端和云
            install(TARGETS ${LIBRARY_NAME}
                LIBRARY DESTINATION ${CHRIC_CLOUD_FOLDER} # 共享库(.so)安装路径
            )

            if(INSTALL_PB_HEADERS)
                # 安装头文件
                install(DIRECTORY ${SUBMODULE_INCLUDE_DIR}/
                    DESTINATION ${OUTPUT_INCLUDE_DIR}/PB/${SUBMODULE}
                    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
                )
            endif()
            

            # 安装头文件
            install(DIRECTORY ${SUBMODULE_INCLUDE_DIR}/
                DESTINATION ${CHRIC_TERMINAL_FOLDER}/PB/${SUBMODULE}
                FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
            )
            

            # 安装头文件
            install(DIRECTORY ${SUBMODULE_INCLUDE_DIR}/
                DESTINATION ${CHRIC_CLOUD_FOLDER}/PB/${SUBMODULE}
                FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
            )
            
            message(DEBUG "Created library target: ${LIBRARY_NAME} for submodule: ${SUBMODULE}")
        else()
            message(DEBUG "No source files found for submodule: ${SUBMODULE}")
        endif()
    endif()
endforeach()

# 安装通用头文件
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include/pb_export.h")
    # 安装到cloud
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/pb_export.h"
        DESTINATION ${CHRIC_CLOUD_FOLDER}
    )
    # 安装到terminal
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/pb_export.h"
        DESTINATION ${CHRIC_TERMINAL_FOLDER}
    )


    if(INSTALL_PB_HEADERS)
        # 安装头文件
        install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/pb_export.h"
            DESTINATION ${OUTPUT_INCLUDE_DIR}/PB
        )
    endif()
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
message(DEBUG "=========================PB module configuration begin=========================")
message(DEBUG "Library output directory: ${OUTPUT_LIB_DIR}")
message(DEBUG "Include output directory: ${OUTPUT_INCLUDE_DIR}")

# 添加测试选项
option(BUILD_PB_TESTS "Build PB tests" ON)

# 如果启用测试，添加 tests 子目录
if(BUILD_PB_TESTS)
    message(DEBUG "Adding PB tests subdirectory...")
    add_subdirectory(tests)
endif()

# 添加PB工具编译选项
option(BUILD_PB_UTILS "Build PB utils" ON)

# 如果启用工具，添加 utils 子目录
if(BUILD_PB_UTILS)
    message(DEBUG "Adding PB utils subdirectory...")
    add_subdirectory(utils)
endif()
